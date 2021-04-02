/**********************************************************************************************
 *
 * [FILE NAME] : control_ecu.c
 *
 * [AUTHOR(S)] : Ahmed Hassan
 *
 * [DATE CREATED]: Mar 11, 2021
 *
 * [DESCRIPTION]: This Project is to design a Door Lock Security System consists of two ECU's.The
 * 	          First ECU is called HMI (Human Machine Interface) ECU responsible for interfacing
 * 		  with the user through "KEYPAD + LCD" to take the user password.
 * 		  The Second ECU is called CONTROL ECU connected with "DC MOTOR" to open and close
 * 		  the door, Alarm System "BUZZER + LED" to alert if a stranger trying to open the
 * 	          door, and "External EEPROM" to store the user password.The two ECU's communicate
 * 	          with each other through UART Protocol.
 *
 * 		 The Second ECU is called CONTROL ECU responsible for
 * 		  - Sending the system status to the HMI ECU.
 * 		  - Storing the new user password in the EEPROM.
 * 		  - Checking matching between the current and the old password.
 * 		  - Receiving the user option.
 * 		  - Changing the user password.
 * 		  - Opening the door by rotating the DC motor CW, closing the DC by rotating
 * 		    the DC MOTOR ACW.
 *
 *
 *  		 Design Considerations for CONTROL ECU:
 *
 *	 	 - AVR MCU ATmega 16 (F_CPU = 8 MHZ).
 *		 - Communicate with the HMI ECU through the UART Protocol.
 *		 - DC MOTOR connected to PIN 3, 4 in PORTA
 *	         - External EEPROM connected to SCL, SDA
 *	         - BUZZER connected to PIN 0 PORTA
 *	         - LED connected to PIN 5 in PORTA
 *		 - TIMER1 is used to count the "DOOR OPENING & CLOSING" time, and "ALARM SYSTEM" time.
 *
 *                The sequence of the CONTROL ECU Program:
 *
 *                Phase 1:
 * 		  - Read the password address.
 * 		  - If it is equal to the default value that means this is the first time login.
 * 		  - If it is not equal to the default value that means this is the not first time login.
 *		  - Send the system status to the HMI ECU.
 *
 *		  Phase 2:
 *		  - Receiving the user option from the CONTROL ECU.
 *
 *		  Phase 3:
 *		  - User choice is (TO CHANGE PASSWORD)
 *		  - Compare the current password with the password stored in the EEPROM
 *		  - If it is matched, Change the password.
 *		  - If it is unmatched, tell the user to try again (for two additional times),
 *		    it is still unmatched count the Alarm system time (15 Seconds).
 *
 *		  Phase 4:
 *	 	  - User choice is (TO OPEN DOOR)
 *		  - Compare the current password with the password stored in the EEPROM
 * 		  - If it is matched, open the door.
 * 		  - If it is unmatched, tell the user to try again (for two additional times),
 *                  it is still unmatched count the Alarm system time (15 Seconds).
 *
 **********************************************************************************************/

#include "application_functions.h"

/**********************************************************************************************
 *                               Global Variables                                             *
 **********************************************************************************************/

static uint8 g_systemStatus;		/* Global variable to store the system status */
static uint8 g_choice;			/* Global variable to store user choice */
static uint8 g_passwordStatus;		/* Global variable to store the current password status */

/*****************************************************************************************
 *                                MAIN FUNCTION                                          *
 *****************************************************************************************/

int main(void)
{
	/********************************************************************************
	 *                           SYSTEM INITIALIZATION                              *
	 ********************************************************************************/

	/* UART Driver Configurations */
	UART_ConfigType UART_Config = {BPS_9600,DOUBLE_SPEED,EIGHT_BITS,DISABLED,ONE_BIT};

	/* TIMER Driver Configurations */
	TIMER_ConfigType TIMER_Config = {INITIAL_VALUE,COUNT_ONE_SECOND,TIMER_1,COMPARE,T1_F_CPU_1024};

	UART_init(&UART_Config); /* UART Driver Initialization */

	EEPROM_init();		/* EEPROM Driver Initialization */

	DCMOTOR_init();		/* DC MOTOR Driver Initialization */

	BUZZER_init();		/* BUZZER Driver Initialization */

	LED_init();		/* LED Driver Initialization */

	SREG |= (1 << 7);	/* Enable Global Interrupt, I-bit in SREG register */

	/*
	 * To get the system status, read the EEPROM address where the password stored,
	 * then put the result in g_systemStatus
	 */
	EEPROM_readByte(ADDRESS_PASSWORD_STORED, &g_systemStatus);

	/********************************************************************************
	 *                     FIRST TIME TO LOGIN TO THE SYSTEM                        *
	 ********************************************************************************/

	/*
	 * To check the system status, compare the value in the EEPROM address where the password
	 * should be stored with the default value:
	 *
	 * 		- If they equal, so this is the first time to run the system, make the following steps:
	 * 		  1- Wait until receive byte from HMI ECU
	 * 		  2- Send system status to the HMI ECU (FIRST TIME TO LOGIN)
	 * 		  3- Store the password send by HMI ECU in the EEPROM
	 *
	 * 		- If they not equal this is not the first time to run the system,
	 * 		  send system status to the HMI ECU (ANY TIME TO LOGIN)
	 */
	if(g_systemStatus == DEFAULT_VALUE)
	{
		while(UART_recieveByte() != ECU_READY){}	/* Wait until HMI ECU be ready */
		UART_sendByte(FIRST_TIME_TO_LOGIN);		/* Send system status to the HMI ECU */

		/* Call function that responsible for receiving, and storing the new password */
		APP_receiveNewPassword(g_newPassword);
	}
	else if(g_systemStatus != DEFAULT_VALUE)
	{
		while(UART_recieveByte() != ECU_READY){}	/* Wait until HMI ECU be ready */
		UART_sendByte(ANY_TIME_TO_LOGIN);		/* Send system status to the HMI ECU */
	}

	/********************************************************************************
	 *                        APPLICATION	(SUPER LOOP)			        *
	 ********************************************************************************/

	while(1)
	{
		/********************************************************************************
		 *                             USER DECISION			     	        *
		 ********************************************************************************/

		/*
		 * Receive the user decision from the HMI ECU :
		 * 	 "+" : TO CHANGE PASSWORD
		 * 	 "-" : TO DOOR OPEN
		 */
		UART_sendByte(ECU_READY);	/* Check if the HMI ECU is ready */
		g_choice = UART_recieveByte();	/* Receive the user decision from the HMI ECU*/

		/********************************************************************************
		 *                    FIRST CHOICE : CHANGE SYSTEM PASSWORD		        *
		 ********************************************************************************/

		/*
		 * If the user decision is to change password, make the following steps:
		 *
		 *	 - Receive the current password from the HMI ECU to (check is this user is owner or not)
		 *	 - Read he old password from the EEPROM
		 *	 - Compare between the received password by the HMI ECU, and the stored password in the
		 *	   EEPROM, then end the result to the HMI ECU
		 */
		if(g_choice == CHANGE_PASSWORD)
		{
			do
			{
				/* Call function that responsible for receiving the current password */
				APP_receiveCurrentPassword(g_currentPassword);

				/* Call function that responsible for reading the old password from the EEPROM */
				APP_readOldPassword();

				/*
				 * Call function that responsible for comparing two passwords (current password,
				 * old password) and store the result in the g_passwordStatus
				 */
				g_passwordStatus = APP_checkPasswordMismatch(g_currentPassword, g_oldPassword);

				/* Wait until HMI ECU be ready */
				while(UART_recieveByte() != ECU_READY){}

				/* Send the current password status to the HMI ECU */
				UART_sendByte(g_passwordStatus);

				/********************************************************************************
				 *                  CASE 1 :  CORRECT PASSWORD (USER IS THE OWNER)      	*
				 ********************************************************************************/

				/*
				 * If the current and the old password are identical, receive and save the
				 * new password in the EEPROM
				 */
				if(g_passwordStatus == MATCHED)
				{
					/* Call function that responsible for receiving, and storing the new password */
					APP_receiveNewPassword(g_newPassword);
					break;	/* Return back to the Main Loop */
				}	/* END OF CORRECT CHOICE */

				/********************************************************************************
				 *               CASE 2:  INCORRECT PASSWORD (USER IS NOT THE OWNER)	        *
				 ********************************************************************************/

				/*
				 *	If the current and the old password are not identical, make the following steps:
				 *		- Increment the number of wrong attempts
				 *		- Check the number of wrong attempts:
				 *				1- If it is not equal to MAXIMUM NUMBER OF ATTEMPTS, break the
				 *				   condition to repeat the above sequence again
				 *				2- If it is equal to MAXIMUM NUMBER OF ATTEMPTS, run the alarm
				 *				   "BUZZER + LED" and hold the system for (10 Seconds), then
				 *				    return back to the MAIN LOOP after that
				 */
				else if(g_passwordStatus == MISMATCHED)
				{
					APP_incorrectPassword();
				}	/* END OF INCORRECT PASSWORD */

			}while(MAXIMUM_ATTEMPTS > g_wrongAttempts);

		}	/* END OF FIRST CHOICE "CHANGE PASSWORD */


		/********************************************************************************
		 *                            SECOND CHOICE : OPEN DOOR			        *
		 ********************************************************************************/

		/*
		 * If the user decision is to change password, make the following steps:
		 *	 - Receive the current password from the HMI ECU to (check is this user is owner or not)
		 *	 - Read he old password from the EEPROM
		 *	 - Compare between the received password by the HMI ECU, and the stored password in the
		 *	   EEPROM, then end the result to the HMI ECU
		 */
		if(g_choice == DOOR_OPEN)
		{
			do
			{
				/* Call function that responsible for receiving the current password */
				APP_receiveCurrentPassword(g_currentPassword);

				/* Call function that responsible for reading the old password from the EEPROM */
				APP_readOldPassword();

				/* Call function that responsible for comparing two passwords (current password,
				 * old password) and store the result in the g_passwordStatus */
				g_passwordStatus = APP_checkPasswordMismatch(g_currentPassword, g_oldPassword);

				while(UART_recieveByte() != ECU_READY){}	/* Wait until HMI ECU be ready */
				UART_sendByte(g_passwordStatus);	/* Send the current password status to the HMI ECU */


				/********************************************************************************
				 *                 CASE 1:  CORRECT PASSWORD  (USER IS THE OWNER)               *
				 ********************************************************************************/

				/*
				 *	If the current and the old password are identical, make the following steps:
				 *
				 *		- Open the door, rotate the motor Clock wise for (15 seconds)
				 *		- Hold the door, stop the for (3 seconds)
				 *	 	- Close the door, rotate the motor Anti Clock wise for (15 seconds)
				 */
				if(g_passwordStatus == MATCHED)
				{
					/************************************************************
					 *                     DOOR IS OPENNING                     *
					 ************************************************************/

					/*
					 * Call function that call back the function that is responsible
					 * for rotating the motor Clockwise if the TIMER1 interrupt is fired
					 */
					TIMER_setCallBack(APP_rotateMotorCW, TIMER_1);

					/* TIMER Driver Initialization */
					TIMER_init(&TIMER_Config);

					/* Wait until the system open again */
					while(g_lockSystem != OPENED){}

					/* Reset the lock system status to its initial value (SYSTEM UNLOCKED) */
					g_lockSystem = UNLOCKED;

					/************************************************************
					 *                        DOOR OPEN                         *
					 ************************************************************/

					/*
					 * Call function that call back the function that is responsible
					 * for stopping the motor if the TIMER1 interrupt is fired
					 */
					TIMER_setCallBack(APP_stopMotor, TIMER_1);

					/* TIMER Driver Initialization */
					TIMER_init(&TIMER_Config);

					/* Wait until the system open again */
					while(g_lockSystem != OPENED){}

					/* Reset the lock system status to its initial value (SYSTEM UNLOCKED) */
					g_lockSystem = UNLOCKED;

					/************************************************************
					 *                      DOOR IS CLOSING                     *
					 ************************************************************/

					/*
					 * Call function that call back the function that is responsible
					 * for rotating the motor Anti Clockwise if the TIMER1 interrupt is fired
					 */
					TIMER_setCallBack(APP_rotateMotorAntiCW, TIMER_1);

					/* TIMER Driver Initialization */
					TIMER_init(&TIMER_Config);

					/* Wait until the system open again */
					while(g_lockSystem != OPENED){}

					/* Reset the lock system status to its initial value (SYSTEM UNLOCKED) */
					g_lockSystem = UNLOCKED;

					break;	/* Return back to the Main Loop */
				}	/* END OF CORRECT PASSWORD */


				/********************************************************************************
				 *               CASE 2:  INCORRECT PASSWORD (USER IS NOT THE OWNER)	        *
				 ********************************************************************************/

				/*
				 *	If the current and the old password are not identical, make the following steps:
				 *		- Increment the number of wrong attempts
				 *		- Check the number of wrong attempts:
				 *				1- If it is not equal to MAXIMUM NUMBER OF ATTEMPTS, break the
				 *				   condition to repeat the above sequence again
				 *				2- If it is equal to MAXIMUM NUMBER OF ATTEMPTS, run the alarm
				 *				   "BUZZER + LED" and hold the system for (10 Seconds), then
				 *				    return back to the MAIN LOOP after that
				 */
				else if(g_passwordStatus == MISMATCHED)
				{
					APP_incorrectPassword();
				}	/* END OF INCORRECT PASSWORD */

			}while(MAXIMUM_ATTEMPTS > g_wrongAttempts);

		}	/* END OF SECOND CHOICE "DOOR OPEN" */

		g_wrongAttempts = NO_ATTEMPT; /* Reset number of attempts for next time */

	}	/* END OF SUPER LOOP */
}	/* END OF MAIN FUNCTION */
