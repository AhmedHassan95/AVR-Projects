/**********************************************************************************
 *
 * [MODULE]: APPLICATION
 *
 * [FILE NAME]: applications_functions.c
 *
 * [DESCRIPTION]: Source file for the Application Functions Used by the HMI ECU
 *
 * [AUTHOR]: Ahmed Hassan
 *
 **********************************************************************************/

#include "application_functions.h"

/**********************************************************************************
 *                              Global Variables                                  *
 **********************************************************************************/

uint8 g_password1[PASSWORD_WIDTH]; /* Global Array to store password1 */
uint8 g_password2[PASSWORD_WIDTH]; /* Global Array to store password2 */
uint8 g_systemStatus;		   /* Global variable to store the system status */
uint8 g_tick = INITIAL_VALUE;	   /* Global tick flag */
uint8 g_lockSystem = UNLOCKED;	   /* Global flag to store the lock system status */
uint8 g_wrongAttempts = NO_ATTEMPT;/* Global variable to store the number of wrong attempts */

/**********************************************************************************
 *                      Functions Prototypes (Private)                            *
 **********************************************************************************/

/**********************************************************************************
 * [Function Name]: APP_init
 *
 * [Description]: Function to Initialize the Application only for the first time
 * 		  - Ask the User to Enter Password for two times
 * 		  - Store each password in a separate global array
 *
 * [Args]:	    None
 *
 * [in]		    None
 *
 * [out]	    None
 *
 * [in/out]	    None
 *
 * [Returns]:       None
 **********************************************************************************/
static void APP_init(void);

/**********************************************************************************
 * [Function Name]: APP_checkPasswordMismatch
 *
 * [Description]: Function to check if the two passwords are identical or not
 *
 * [Args]:	  Password1_Ptr, Password2_Ptr
 *
 * [in]		  Password1_Ptr: Pointer to character
 * 		  Password2_Ptr: Pointer to character
 *
 * [out]	  Unsigned Character
 *
 * [in/out]	  None
 *
 * [Returns]:     Result of the comparison between two passwords
 **********************************************************************************/
static uint8 APP_checkPasswordMismatch(uint8 * Password1_Ptr , uint8 * Password2_Ptr);


/**********************************************************************************
 *                         Functions Definitions                                  *
 **********************************************************************************/

/**********************************************************************************
 * [Function Name]: APP_setNewPassword
 *
 * [Description]: Function to set the new user password
 * 		  - Display a message, if this is the first time to login to the system
 * 		  - Take new password from the user for two times
 * 		  - Compare between these two passwords
 * 		  - If two passwords are identical, send password to the Control ECU to be
 * 	            stored in the EEPROM
 * 		  - If two passwords are not identical, repeat these steps again
 *
 * [Args]:	  a_systemStatus
 *
 * [in]		  a_systemStatus: Unsigned Character (store the password status)
 *
 * [out]	  None
 *
 * [in/out]	  None
 *
 * [Returns]:     None
 **********************************************************************************/
void APP_setNewPassword(uint8 a_systemStatus)
{
	uint8 a_passwordMatch;	/* Local variable to store the password status */

	do
	{
		/* Check if this is the first time to login the system or not */
		if(a_systemStatus == FIRST_TIME_TO_LOGIN)
		{
			/* This message appears only in the first time to login to the system */
			LCD_clearScreen();
			LCD_displayString("FIRST TIME TO LOGIN");
			_delay_ms(LONG_DISPLAY_TIME);	/* Keep the message on the screen for a while */
		}

		/* Call function to initialize the system for the first time */
		APP_init();

		/* Call function to compare between two passwords, and get the result */
		a_passwordMatch = APP_checkPasswordMismatch(g_password1, g_password2);

		/*
		 * Check if the two passwords are identical or not:
		 * 	 - If two passwords are not identical, repeat theses steps again
		 * 	 - If two passwords are identical, break the loop
		 */
		if(a_passwordMatch == MISMATCHED)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 1, "PASSWORD UNMATCHED");
			LCD_displayStringRowColumn(1, 5, "TRY AGAIN");
			_delay_ms(SHORT_DISPLAY_TIME);	/* Keep the message on the screen for a while */
		}

	}while(a_passwordMatch == MISMATCHED);

	/*
	 * If two passwords are identical, send the password to the Control ECU,
	 * and display message "PASSWORD MATCHED"
	 */
	if(a_passwordMatch == MATCHED)
	{
		/* Call function that responsible for sending password to the Control ECU */
		APP_sendPassword(g_password1);

		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 2, "PASSWORD MATCHED");
		_delay_ms(SHORT_DISPLAY_TIME);	/* Keep the message on the screen for a while */
	}
}

/**********************************************************************************
 * [Function Name]: APP_init
 *
 * [Description]: Function to Initialize the Application only for the first time
 *		  - Display Message "FIRST TIME TO LOGIN"
 * 		  - Ask the User to Enter Password for two times
 * 		  - Store each password in a separate global array
 *
 * [Args]:          None
 *
 * [in]		    None
 *
 * [out]	    None
 *
 * [in/out]	    None
 *
 * [Returns]:       None
 **********************************************************************************/
static void APP_init(void)
{
	/* Display message to the user, to Enter a new password */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "ENTER NEW PASSWORD");
	LCD_displayStringRowColumn(1, 0, "= : TO SUMBIT");
	LCD_displayStringRowColumn(2, 0, "X : TO CLEAR");
	LCD_goToRowColumn(3, 7);

	/* Call function to take the password from the user for the First time */
	APP_getPassword(g_password1);

	/* Display message to the user, that the password is submitted correctly */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 1, "PASSWORD SUBMITTED");
	_delay_ms(SHORT_DISPLAY_TIME);	/* Keep the message on the screen for a while */

	/* Display message to the user, to ReEnter the same password */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "REENTER PASS AGAIN");
	LCD_displayStringRowColumn(1, 0, "= : TO SUMBIT");
	LCD_displayStringRowColumn(2, 0, "X : TO CLEAR");
	LCD_goToRowColumn(3, 7);

	/* Call function to take the password from the user for the Second time */
	APP_getPassword(g_password2);
}

/**********************************************************************************
 * [Function Name]: APP_getPassword
 *
 * [Description]: Function to get password from the User
 *		  - Store password consists of 5 digits in a global array
 * 	          - Give the User two options ('=' : TO SUBMIT, 'X' : TO CLEAR)
 *
 * [Args]:	    Password_Ptr
 *
 * [in]		    Password_Ptr: Pointer to character
 *
 * [out]	    None
 *
 * [in/out]	    None
 *
 * [Returns]:       None
 **********************************************************************************/
void APP_getPassword(uint8 * Password_Ptr)
{
	static uint8 counter; /* Local variable used as a counter for the for Loop */
	static uint8 key;	  /* Local variable used to store the value of the pressed Button */

	for(counter = 0; counter < PASSWORD_WIDTH ;counter++)
	{
		key = KeyPad_getPressedKey();

		/* Check if the Button pressed is a digit or not */
		if(key >= 0 && key <= 9)
		{
			Password_Ptr[counter] = key; /* Store only digits from (0 -> 9) in the Password */
		}

		/*
		 * Check if the Button pressed is equal to 'X' or not, in Case equal to 'X' clear the
		 * written password and restart write it again
		 * (User Enter a wrong digit and need to correct it)
		 */
		else if(key == '*')
		{
			counter = -1;	/* Reset the counter */
			/* Clear Row in LCD by writing spaces " " */
			LCD_displayStringRowColumn(3, 7,"         ");
			continue;
		}

		/* Check if the Button Pressed is not digit, decrement the counter */
		else
		{
			--counter;
			continue;
		}

		_delay_ms(PRESS_TIME); /* Press Time */
		LCD_goToRowColumn(3, counter+7);
		LCD_displayCharacter('*');	/* Display '*' for Security issue */

		/*
		 * This while loop used to handle two cases:
		 * 	- If the Button Pressed is equal to '=', break the while loop, then submit the Password
		 * 	- If the Button Pressed is equal to 'X' (in case of 5 digits are written in the screen),
		 * 	  clear the written password, then break the while loop and restart write the Correct
		 * 	  Password again (User Enter a wrong digit and need to correct it)
		 */
		while(counter == 4)
		{
			key = KeyPad_getPressedKey(); /* Get the next Press */

			if(key == '=')
			{
				break;
			}
			else if(key == '*')
			{
				counter = -1;	/* Reset the counter */
				/* Clear Row in LCD by writing spaces " " */
				LCD_displayStringRowColumn(3, 7,"         ");
				continue;
			}
		}
	}
}

/**********************************************************************************
 * [Function Name]: APP_checkPasswordMismatch
 *
 * [Description]: Function to check if the two passwords are identical or not
 *
 * [Args]:        Password1_Ptr, Password2_Ptr
 *
 * [in]		  Password1_Ptr: Pointer to character
 * 		  Password2_Ptr: Pointer to character
 *
 * [out]          Unsigned Character
 *
 * [in/out]	  None
 *
 * [Returns]:     Result of the comparison between two passwords
 **********************************************************************************/
uint8 APP_checkPasswordMismatch(uint8 * Password1_Ptr , uint8 * Password2_Ptr)
{
	uint8 counter;	/* Local variable used as a counter for the for Loop */
	uint8 password_match = 0;	/* Local variable used to store password status */

	for(counter = 0; counter < PASSWORD_WIDTH  ;counter++)
	{
		/* Check if the two passwords are identical or not */
		if(Password1_Ptr[counter] != Password2_Ptr[counter])
		{
			/*
			 * If two passwords are not identical, change password_match value,
			 * then break the loop as there is no need to continue looping
			 */
			password_match = MISMATCHED;
			break;
		}
	}
	return password_match ;
}

/**********************************************************************************
 * [Function Name]: APP_getUserDecision
 *
 * [Description]:  Function to get the User decision
 *
 * [Args]:	   None
 *
 * [in]		   None
 *
 * [out]           Unsigned Character
 *
 * [in/out]	   None
 *
 * [Returns]:      The User Decision
 **********************************************************************************/
uint8 APP_getUserDecision(void)
{
	static uint8 key;	/* Local variable used to store the value of the pressed Button */
	static uint8 decision;	/* Local variable used to store user decision */

	/*
	 * Display message to the user, to select between Two Choices:
	 * 	"+" : TO CHANGE PASSWORD
	 * 	"-" : TO OPEN DOOR
	 */
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+ : CHANGE PASSWORD");
	LCD_displayStringRowColumn(1, 0, "- : OPEN DOOR");

	/*
	 * Take choice from the user once at least, then check if it is not equal to one of the given
	 * choices or not:
	 *	 - If choice equal to one of the given choices (CHANGE PASSWORD, OPEN DOOR), do while
	 *	   condition will be false, then break the loop, and go next step
	 * 	 - If choice is not equal to one of the given choices (CHANGE PASSWORD, OPEN DOOR),
	 * 	   do while condition will be true, so still in the loop until be false
	 */
	do
	{
		key = KeyPad_getPressedKey();
		_delay_ms(PRESS_TIME);	/* Press Time */

	}while(key != CHANGE_PASSWORD && key != DOOR_OPEN);

	/* Check if the key pressed is equal to (CHANGE_PASSWORD or DOOR_OPEN) */
	if(key == CHANGE_PASSWORD)
	{
		decision = CHANGE_PASSWORD;
	}
	else if(key == DOOR_OPEN)
	{
		decision = DOOR_OPEN;
	}

	return decision;	/* Return User decision */
}

/**********************************************************************************
 * [Function Name]: APP_sendPassword
 *
 * [Description]: Function to send password to the Control ECU
 *
 * [Args]:	   Password_Ptr
 *
 * [in]		   Password_Ptr: Pointer to character
 *
 * [out]           None
 *
 * [in/out]	   None
 *
 * [Returns]:      None
 **********************************************************************************/
void APP_sendPassword(uint8 * Password1_Ptr)
{
	static uint8 counter; /* Local variable used as a counter for the for Loop */

	/* For loop to send the password to the Control ECU */
	for(counter = 0; counter < PASSWORD_WIDTH ;counter++)
	{
		while(UART_recieveByte() != ECU_READY){}	/* Wait until Control ECU be ready */

		/* Send 1 Byte of the password to the Control ECU */
		UART_sendByte(Password1_Ptr[counter]);
	}
}

/********************************************************************************
 * [Function Name]: APP_incorrectPassword
 *
 * [Description]:   Function that is responsible for doing the following steps:
 *                  - Increment the number of wrong attempts
 *                  - Display the message "INCORRECT PASSWORD"
 *		    - Check the number of wrong attempts:
 *			1- If it is not equal to MAXIMUM NUMBER OF ATTEMPTS, return
 *			   from the function to get the password again from the user
 *			2- If it is equal to MAXIMUM NUMBER OF ATTEMPTS, count the
 *			   alarm time and hold the system for (15 Seconds), then return
 *			   back to the MAIN LOOP again
 *
 * [Args]:	    None
 *
 * [in]		    None
 *
 * [out]	    None
 *
 * [in/out]	    None
 *
 * [Returns]:       None
 ********************************************************************************/
void APP_incorrectPassword(void)
{
	++g_wrongAttempts;	/* Increment the number of wrong choice */

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 1, "INCORRECT PASSWORD");
	_delay_ms(SHORT_DISPLAY_TIME);	/* Keep the message on the screen for a while */

	/*
	 * Check if the next attempt is the last one, alert the user by displaying
	 * a message "AFTER 1 MORE FAILED ATTEMPT SYSTEM WILL BE LOCKED FOR 10 SECONDS"
	 */
	if(g_wrongAttempts == MAXIMUM_ATTEMPTS-1)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "AFTER 1 MORE FAILED");
		LCD_displayStringRowColumn(1, 0, "ATTEMPT SYSTEM WILL");
		LCD_displayStringRowColumn(2, 2, "BE LOCKED FOR 15");
		LCD_displayStringRowColumn(3, 6, "SECONDS");
		_delay_ms(LONG_DISPLAY_TIME);
	}

	/*
	 * Check if this attempt is less than MAXIMUM NUMBER OF ATTEMPTS,
	 * displaying a message "ENTER PASSWORD AGAIN"
	 */
	if(g_wrongAttempts < MAXIMUM_ATTEMPTS)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 0, "ENTER PASSWORD AGAIN");
		LCD_displayStringRowColumn(1, 0, "= : TO SUMBIT");
		LCD_displayStringRowColumn(2, 0, "X : TO CLEAR");
	}

	/*
	 * Check if this attempt is equal to MAXIMUM NUMBER OF ATTEMPTS, run the buzzer
	 * and hold system for 15 Seconds
	 */
	if(g_wrongAttempts == MAXIMUM_ATTEMPTS)
	{
		LCD_clearScreen();
		LCD_displayStringRowColumn(0, 1, "MAXIMUM NUMBER OF");
		LCD_displayStringRowColumn(1, 1, "ATTEMPTS EXCEEDED");
		_delay_ms(SHORT_DISPLAY_TIME);

		/* TIMER Driver Configurations */
		TIMER_ConfigType TIMER_Config = {INITIAL_VALUE,COUNT_ONE_SECOND,TIMER_1,COMPARE,\
										 T1_F_CPU_1024};
		/*
		 * Call function that call back the alarm function if the TIMER1
		 * interrupt is fired
		 */
		TIMER_setCallBack(APP_countAlarmTime, TIMER_1);

		/* TIMER Driver Initialization */
		TIMER_init(&TIMER_Config);

		/* Display message to the */
		LCD_clearScreen();
		LCD_displayStringRowColumn(1, 2, "SYSTEM IS LOCKED");

		/* Wait until the alarm turned off */
		while(g_lockSystem != OPENED){}
		/* Reset the lock system status to its initial value (SYSTEM UNLOCKED) */
		g_lockSystem = UNLOCKED;

	}	/* END OF MAXIMUM_ATTEMPTS */
}	/* END OF INCORRECT PASSWORD */

/********************************************************************************
 * [Function Name]: APP_countAlarmTime
 *
 * [Description]: Function to count the Alarm time
 *
 * [Args]:	  None
 *
 * [in]		  None
 *
 * [out]	  None
 *
 * [in/out]	  None
 *
 * [Returns]:     None
 ********************************************************************************/
void APP_countAlarmTime(void)
{
	++g_tick;	/* Increment global tick flag each time the interrupt fired */

	if(g_tick == MAXIMUM_ALARM_TIME)
	{
		/*
		 * De-initialization for the timer to be able to use it again correctly
		 */
		TIMER_DeInit(TIMER_1);
		g_tick = INITIAL_VALUE;	/* Clear global tick flag for the next time */
		g_lockSystem = OPENED;	/* Open the system again */
	}
}

/********************************************************************************
 * [Function Name]: APP_countMotorRunTime
 *
 * [Description]: Function to count the Motor running time
 *
 * [Args]:	  None
 *
 * [in]		  None
 *
 * [out]	  None
 *
 * [in/out]	  None
 *
 * [Returns]:     None
 ********************************************************************************/
void APP_countMotorRunTime(void)
{
	++g_tick;	/* Increment global tick flag each time the interrupt fired */

	if(g_tick == MAXIMUM_RUNNING_TIME)
	{
		/*
		 * De-initialization for the timer to be able to use it again correctly
		 */
		TIMER_DeInit(TIMER_1);
		g_tick = INITIAL_VALUE;	/* Clear global tick flag for the next time */
		g_lockSystem = OPENED;	/* Open the system again */
	}
}

/********************************************************************************
 * [Function Name]: APP_countMotorStopTime
 *
 * [Description]: Function to count the Motor stopping time
 *
 * [Args]:        None
 *
 * [in]		  None
 *
 * [out]          None
 *
 * [in/out]	  None
 *
 * [Returns]:     None
 ********************************************************************************/
void APP_countMotorStopTime(void)
{
	++g_tick;	/* Increment global tick flag each time the interrupt fired */

	if(g_tick == MAXIMUM_STOPING_TIME)
	{
		/*
		 * De-initialization for the timer to be able to use it again correctly
		 */
		TIMER_DeInit(TIMER_1);
		g_tick = INITIAL_VALUE;	/* Clear global tick flag for the next time */
		g_lockSystem = OPENED;	/* Open the system again */
	}
}
