/*****************************************************************************************
 *
 * [FILE NAME] : hmi_ecu.c
 *
 * [AUTHOR(S)] : Ahmed Hassan
 *
 * [DATE CREATED]: Mar 11, 2021
 *
 * [DESCRIPTION]: This Project is to design a Door Lock Security System consists of two ECU's.The
 * 				  First ECU is called HMI (Human Machine Interface) ECU responsible for interfacing
 * 				  with the user through "KEYPAD + LCD" to take the user password.
 * 				  The Second ECU is called CONTROL ECU connected with "DC MOTOR" to open and close
 * 				  the door, Alarm System "BUZZER + LED" to alert if a stranger trying to open the
 * 				  door, and "External EEPROM" to store the user password.The two ECU's communicate
 * 				  with each other through UART Protocol.
 *
 *
 * 				   The First ECU is called HMI (Human Machine Interface) ECU responsible for
 * 				   	- Getting the system status from the CONTROL ECU.
 * 				   	- Taking a new password from the user two times.
 * 				   	- Sending the password to the CONTROL ECU to store it in the EEPROM.
 * 				   	- Displaying two options to the user "+" (TO CHANGE PASSWORD),
 * 				   	  and "-" (TO OPEN DOOR).
 * 				   	- Sending the user option to the CONTROL ECU.
 *
 * 				   Design Considerations for HMI ECU:
 *
 *	 	 	 		 - AVR MCU ATmega 16 (F_CPU = 8 MHZ).
 *			     	 - Communicate with the CONTROL ECU through the UART Protocol.
 *			     	 - LCD (4 * 16) connected to PORTC
 *			     	 - KEYPAD connected to PORTA
 *					 - TIMER1 is used to count the "DOOR OPENING & CLOSING" time, and
 *					   "ALARM SYSTEM" time.
 *
 *                The sequence of the HMI ECU Program:
 *
 *                 	Phase 1:
 * 				   	 - Get the system status from the CONTROL ECU.
 * 				   	 - If this is the first time to login to the system, take a new password
 * 				   	   two times.
 * 				   	 - Check if these two passwords are identical or not.
 *						- If the two passwords are identical, sending the password to the Control
 *						  ECU to store it in the EEPROM.
 * 				   		- If the two passwords are not identical, still in this step until
 * 				   		  be identical.
 *
 *					Phase 2:
 *					 - If this is not the first time to login to the system, give the user two
 *					   options "+" (TO CHANGE PASSWORD), "-" (TO OPEN DOOR).
 *
 *					Phase 3:
 *					 - User choice is (TO CHANGE PASSWORD)
 *					 - Ask the user for the current password
 *					   	  - If it is matched with the old one, Change the password.
 *					   	  - If it is unmatched with the old one, tell the user to try again
 *							(for two additional times), it is still unmatched count the
 *							Alarm system time (15 Seconds).
 *
 *		            Phase 4:
 *					 - User choice is (TO OPEN DOOR)
 *					 - Ask the user for the current password
 *					   	  - If it is matched with the old one, Open the door.
 *					   	  - If it is unmatched with the old one, tell the user to try again
 *							(for two additional times), it is still unmatched count the
 *							Alarm system time (15 Seconds).
 *
 *****************************************************************************************/

#include "application_functions.h"

/*****************************************************************************************
 *                    Global Variables  (Private to this File)                           *
 *****************************************************************************************/

static uint8 g_choice;			/* Global variable to store the user choice */
static uint8 g_passwordStatus;	/* Global variable to store the current password status */

/*****************************************************************************************
 *                                MAIN FUNCTION                                          *
 *****************************************************************************************/

int main(void)
{
	/********************************************************************************
	 *                           SYSTEM INITIALIZATION                              *
	 ********************************************************************************/

	LCD_init(); /* LCD Driver Initialization */

	/* UART Driver Configurations */
	UART_ConfigType UART_Config = {BPS_9600,DOUBLE_SPEED,EIGHT_BITS,DISABLED,ONE_BIT};

	/* TIMER Driver Configurations */
	TIMER_ConfigType TIMER_Config = {INITIAL_VALUE,COUNT_ONE_SECOND,TIMER_1,COMPARE,T1_F_CPU_1024};

	UART_init(&UART_Config); /* UART Driver Initialization */

	SREG |= (1 << 7);	/* Enable Global Interrupt, I-bit in SREG register */

	UART_sendByte(ECU_READY);	/* Check if the Control ECU is ready */

	g_systemStatus = UART_recieveByte();	/* Receive system status from Control ECU */

	/********************************************************************************
	 *                     FIRST TIME TO LOGIN TO THE SYSTEM                        *
	 ********************************************************************************/

	/*
	 * Check the system status:
	 *
	 * 		- If this is the first time to run the system, make the following steps:
	 * 		  	1- Initialize the system (For the first time)
	 * 		    2- Take new password from the user for two times
	 * 		 	3- Compare between these two passwords
	 * 		  	4- If two passwords are identical, send password to the Control ECU to be
	 * 		  	    stored in the EEPROM
	 * 		  	5- If two passwords are not identical, repeat these steps again
	 *
	 * 		- If this is not the first time to run the system, skip this condition
	 */
	if (g_systemStatus == FIRST_TIME_TO_LOGIN)
	{
		APP_setNewPassword(FIRST_TIME_TO_LOGIN);
	}

	/********************************************************************************
	 *                        APPLICATION	(SUPER LOOP)						    *
	 ********************************************************************************/

	while(1)
	{
		/********************************************************************************
		 *                             USER DECISION								    *
		 ********************************************************************************/

		/*
		 * Call function that responsible for asking user to Enter one of these options:
		 * 	 "+" : TO CHANGE PASSWORD
		 * 	 "-" : TO DOOR OPEN
		 */
		g_choice = APP_getUserDecision();

		while(UART_recieveByte() != ECU_READY){}	/* Wait until Control ECU be ready */
		UART_sendByte(g_choice);					/* Send the user choice to the Control ECU */

		LCD_clearScreen();	/* Display message to the user to enter the current password */
		LCD_displayStringRowColumn(0, 4, "ENTER CURRENT");
		LCD_displayStringRowColumn(1, 6, "PASSWORD");


		/********************************************************************************
		 *                    FIRST CHOICE : CHANGE SYSTEM PASSWORD				 	    *
		 ********************************************************************************/

		/*
		 *  If the user decision is to change password, make the following steps:
		 *	 - Get the current password from the user
		 *	 - Send the current password to the Control ECU to (check is this user is owner or not)
		 *	 - Receive the comparison result between the current password and the old password
		 */
		if(g_choice == CHANGE_PASSWORD)
		{
			do
			{
				/* Call function that responsible for taking password from the user */
				APP_getPassword(g_password1);

				/* Call function that responsible for sending password to the Control ECU */
				APP_sendPassword(g_password1);

				UART_sendByte(ECU_READY);	/* Check if the Control ECU is ready */
				/* Receive the current password status from the Control ECU */
				g_passwordStatus = UART_recieveByte();

				/********************************************************************************
				 *                  CASE 1:  CORRECT PASSWORD  (USER IS THE OWNER)           	*
				 ********************************************************************************/

				/*
				 *	If the current and the old password are identical, call function that is
				 *	responsible for doing the following steps:
				 *
				 * 		- Take new password from the user for two times
				 *	 	- Compare between these two passwords
				 *   	- If two passwords are identical, send password to the Control ECU to be
				 *   	  stored in the EEPROM
				 *      - If two passwords are not identical, repeat these steps again
				 */
				if(g_passwordStatus == MATCHED)
				{
					APP_setNewPassword(ANY_TIME_TO_LOGIN);

					break;	/* Return back to the MAIN LOOP */

				}	/* END OF CORRECT PASSWORD */

				/********************************************************************************
				 *             CASE 2:   INCORRECT PASSWORD  (USER IS NOT THE OWNER)	        *
				 ********************************************************************************/

				/*
				 *	If the current and the old password are not identical, call function that is
				 *	responsible for doing the following steps:
				 *
				 *		- Increment the number of wrong attempts
				 *		- Display the message "INCORRECT PASSWORD"
				 *		- Check the number of wrong attempts:
				 *			1- If it is not equal to MAXIMUM NUMBER OF ATTEMPTS, return
				 *		       from the function to get the password again from the user
				 *			2- If it is equal to MAXIMUM NUMBER OF ATTEMPTS, count the alarm time
				 *			   and hold the system for (15 Seconds), then return back to the MAIN
				 *			   LOOP again
				 */
				else if(g_passwordStatus == MISMATCHED)
				{
					APP_incorrectPassword();
				}

			}while(MAXIMUM_ATTEMPTS > g_wrongAttempts);

		} /* END OF FIRST CHOICE "CHANGE PASSWORD */


		/********************************************************************************
		 *                            SECOND CHOICE : OPEN DOOR	      	       	        *
		 ********************************************************************************/

		/*
		 *  If the user decision is to open door, make the following steps:
		 *	 - Get the current password from the user
		 *	 - Send the current password to the Control ECU to (check is this user is owner or not)
		 *	 - Receive the comparison result between the current password and the old password
		 */
		if(g_choice == DOOR_OPEN)
		{
			do
			{
				/* Call function that responsible for taking password from the user */
				APP_getPassword(g_password2);

				/* Call function that responsible for sending password to the Control ECU */
				APP_sendPassword(g_password2);

				UART_sendByte(ECU_READY);	/* Check if the Control ECU is ready */
				/* Receive the current password status from the Control ECU */
				g_passwordStatus = UART_recieveByte();

				/********************************************************************************
				 *                 CASE 1:  CORRECT PASSWORD  (USER IS THE OWNER)  		        *
				 ********************************************************************************/

				/*
				 *	If the current and the old password are identical, make the following steps:
				 *
				 *		- Count time of rotating the motor ClockWise, when the Control ECU open
				 *		  the door for (15 seconds)
				 *		- Count time of stopping the motor, when the Control ECU hold the door
				 *		  for (3 seconds)
				 *	 	- Count time of rotating the motor Anti ClockWise, when the Control ECU
				 *	 	  close the door for (15 seconds)
				 */
				if(g_passwordStatus == MATCHED)
				{

					/************************************************************
					 *                     DOOR IS OPENING                     *
					 ************************************************************/

					/*
					 * Call function that call back the function that is responsible
					 * for counting the motor RunTime if the TIMER1 interrupt is fired
					 */
					TIMER_setCallBack(APP_countMotorRunTime, TIMER_1);

					/* TIMER Driver Initialization */
					TIMER_init(&TIMER_Config);

					/* Display message to the user, that the door is opening */
					LCD_clearScreen();
					LCD_displayStringRowColumn(1, 2, "DOOR IS OPENING");

					/* Wait until the system open again */
					while(g_lockSystem != OPENED){}

					/* Reset the lock system status to its initial value (SYSTEM UNLOCKED) */
					g_lockSystem = UNLOCKED;

					/************************************************************
					 *                        DOOR OPEN                         *
					 ************************************************************/

					/*
					 * Call function that call back the function that is responsible
					 * for counting the motor StopTime if the TIMER1 interrupt is fired
					 */
					TIMER_setCallBack(APP_countMotorStopTime, TIMER_1);

					/* TIMER Driver Initialization */
					TIMER_init(&TIMER_Config);

					/* Display message to the user, that the door open */
					LCD_clearScreen();
					LCD_displayStringRowColumn(1, 5, "DOOR OPEN");

					/* Wait until the system open again */
					while(g_lockSystem != OPENED){}

					/* Reset the lock system status to its initial value (SYSTEM UNLOCKED) */
					g_lockSystem = UNLOCKED;

					/************************************************************
					 *                      DOOR IS CLOSING                     *
					 ************************************************************/

					/*
					 * Call function that call back the function that is responsible
					 * for counting the motor RunTime if the TIMER1 interrupt is fired
					 */
					TIMER_setCallBack(APP_countMotorRunTime, TIMER_1);

					/* TIMER Driver Initialization */
					TIMER_init(&TIMER_Config);

					/* Display message to the user, that the door is closing */
					LCD_clearScreen();
					LCD_displayStringRowColumn(1, 3, "DOOR IS CLOSING");

					/* Wait until the system open again */
					while(g_lockSystem != OPENED){}

					/* Display message to the user, that the door closed */
					LCD_clearScreen();
					LCD_displayStringRowColumn(1, 5, "DOOR CLOSED");
					_delay_ms(SHORT_DISPLAY_TIME);

					/* Reset the lock system status to its initial value (SYSTEM UNLOCKED) */
					g_lockSystem = UNLOCKED;

					break;	/* Return back to the Main Loop */
				}	/* END OF CORRECT PASSWORD */

				/********************************************************************************
				 *             CASE 2:   INCORRECT PASSWORD  (USER IS NOT THE OWNER)	        *
				 ********************************************************************************/

				/*
				 *	If the current and the old password are not identical, call function that is
				 *	responsible for doing the following steps:
				 *
				 *		- Increment the number of wrong attempts
				 *		- Display the message "INCORRECT PASSWORD"
				 *		- Check the number of wrong attempts:
				 *			1- If it is not equal to MAXIMUM NUMBER OF ATTEMPTS, return
				 *		       from the function to get the password again from the user
				 *			2- If it is equal to MAXIMUM NUMBER OF ATTEMPTS, count the alarm time
				 *			   and hold the system for (15 Seconds), then return back to the MAIN
				 *			   LOOP again
				 */
				else if(g_passwordStatus == MISMATCHED)
				{
					APP_incorrectPassword();
				}	/* END OF INCORRECT PASSWORD */

			}while(MAXIMUM_ATTEMPTS > g_wrongAttempts);

		} /* END OF SECOND CHOICE "CHANGE PASSWORD */

		g_wrongAttempts = NO_ATTEMPT; /* Reset number of attempts for next time */

	}	/* END OF SUPER LOOP */

}	/* END OF MAIN FUNCTION */
