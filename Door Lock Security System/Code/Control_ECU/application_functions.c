/**********************************************************************************
 *
 * [MODULE]: APPLICATION
 *
 * [FILE NAME]: applications_functions.c
 *
 * [DESCRIPTION]: Source file for the Application Functions Used by the Control ECU
 *
 * [AUTHOR]: Ahmed Hassan
 *
 **********************************************************************************/

#include "application_functions.h"

/**********************************************************************************
 *                              Global Variables                                  *
 **********************************************************************************/

uint8 g_newPassword[PASSWORD_WIDTH];	 /* Global Array to store new password */
uint8 g_currentPassword[PASSWORD_WIDTH]; /* Global Array to store current password */
uint8 g_oldPassword[PASSWORD_WIDTH]; 	 /* Global Array to store old password */
uint8 g_tick = INITIAL_VALUE;		     /* Global tick flag */
uint8 g_lockSystem = UNLOCKED ;			 /* Global flag to store the system status */
uint8 g_wrongAttempts = NO_ATTEMPT;		 /* Global variable to store the number of wrong attempts */

/**********************************************************************************
 *                      Functions Prototypes (Private)                            *
 **********************************************************************************/

/**********************************************************************************
 * [Function Name]: APP_turnOnAlarm
 *
 * [Description]:   Function to turn on the Alarm
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 **********************************************************************************/
static void APP_turnOnAlarm(void);

/**********************************************************************************
 *                         Functions Definitions                                  *
 **********************************************************************************/

/**********************************************************************************
 * [Function Name]: APP_checkPasswordMismatch
 *
 * [Description]: 	Function to check if the two passwords are identical or not
 *
 * [Args]:			Password1_Ptr, Password2_Ptr
 *
 * [in]			  	Password1_Ptr: Pointer to character
 * 					Password2_Ptr: Pointer to character
 *
 * [out]		  	Unsigned Character
 *
 * [in/out]		 	None
 *
 * [Returns]:       Result of the comparison between two passwords
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
 * [Function Name]: APP_receiveNewPassword
 *
 * [Description]: 	Function to receive the new password from the HMI ECU, and store it
 * 				  	in the EEPROM
 *
 * [Args]:			Password_Ptr
 *
 * [in]			  	Password_Ptr: Pointer to character
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 **********************************************************************************/
void APP_receiveNewPassword(uint8 * Password_Ptr)
{
	static uint8 counter;

	/* For loop to receive the new password send by HMI ECU to be stored in the EEPROM */
	for(counter = 0; counter < PASSWORD_WIDTH ;counter++)
	{
		/* Check if the HMI ECU is ready */
		UART_sendByte(ECU_READY);

		/* Receive 1 Byte of the new password from the HMI ECU */
		Password_Ptr[counter] = UART_recieveByte();

		/*
		 * Store 1 Byte of the new password in the EEPROM
		 * To store password consists of 5 digits (5 characters), we need each time to
		 * increment the address location by 1 character, or by making OR with the
		 * address location ( ADDRESS_PASSWORD_STORED | counter )
		 */
		EEPROM_writeByte(( ADDRESS_PASSWORD_STORED | counter ), Password_Ptr[counter]);

		_delay_ms(STORE_TIME); /* Give the EEPROM some time to store 1 Byte */
	}
}

/**********************************************************************************
 * [Function Name]: APP_receiveCurrentPassword
 *
 * [Description]: 	Function to receive the current password from the HMI ECU
 *
 * [Args]:			Password_Ptr
 *
 * [in]			  	Password_Ptr: Pointer to character
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 **********************************************************************************/
void APP_receiveCurrentPassword(uint8 * Password_Ptr)
{
	static uint8 counter;

	/* For loop to receive the current password send by HMI ECU */
	for(counter = 0; counter < PASSWORD_WIDTH ;counter++)
	{
		/* Check if the HMI ECU is ready */
		UART_sendByte(ECU_READY);

		/* Receive 1 Byte of the current password from the HMI ECU */
		Password_Ptr[counter] = UART_recieveByte();
	}
}

/**********************************************************************************
 * [Function Name]: APP_readOldPassword
 *
 * [Description]: 	Function that responsible for reading the old password from the EEPROM
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 **********************************************************************************/
void APP_readOldPassword(void)
{
	static uint8 counter;

	/* For loop to read the old password stored in the EEPROM */
	for(counter = 0; counter < PASSWORD_WIDTH ;counter++)
	{
		/*
		 * Read 1 Byte of the old password from the EEPROM
		 * To read password consists of 5 digits (5 characters), we need each time to
		 * increment the address location by 1 character, or by making OR with the
		 * address location ( ADDRESS_PASSWORD_STORED | counter )
		 */
		EEPROM_readByte( (ADDRESS_PASSWORD_STORED | counter), &g_oldPassword[counter]);
		_delay_ms(READ_TIME);	/* Give the EEPROM some time to read 1 Byte */
	}
}

/**********************************************************************************
 * [Function Name]: APP_incorrectPassword
 *
 * [Description]:   Function that is responsible for running the alarm system "BUZZER + LED"
 * 					in case of the user exceeding the maximum number of wrong attempts
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 **********************************************************************************/
void APP_incorrectPassword(void)
{
	++g_wrongAttempts;	/* Increment the number of wrong choice */

	if(g_wrongAttempts == MAXIMUM_ATTEMPTS)
	{
		_delay_ms(700);

		/* TIMER Driver Configurations */
			TIMER_ConfigType TIMER_Config = {INITIAL_VALUE,COUNT_ONE_SECOND,TIMER_1,
											 COMPARE,T1_F_CPU_1024};
		/*
		 * Call function that call back the alarm function if the TIMER1
		 * interrupt is fired
		 */
		TIMER_setCallBack(APP_turnOnAlarm, TIMER_1);

		/* TIMER Driver Initialization */
		TIMER_init(&TIMER_Config);

		/* Wait until the alarm turned off */
		while(g_lockSystem != OPENED){}
		/* Reset the lock system status to its initial value (SYSTEM UNLOCKED) */
		g_lockSystem = UNLOCKED;

	}	/* END OF MAXIMUM_ATTEMPTS */
}

/**********************************************************************************
 * [Function Name]: APP_turnOnAlarm
 *
 * [Description]:   Function to turn on the Alarm
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 **********************************************************************************/
static void APP_turnOnAlarm(void)
{
	++g_tick;	/* Increment global tick flag each time the interrupt fired */

	if(g_tick == FIRST_TICK)
	{
		LED_turnOn();
		BUZZER_turnOn();
	}

	if(g_tick == MAXIMUM_ALARM_TIME)
	{
		BUZZER_turnOff();
		LED_turnOff();
		/*
		 * De-initialization for the timer to be able to use it again correctly
		 */
		TIMER_DeInit(TIMER_1);
		g_tick = INITIAL_VALUE; /* Clear global tick flag for the next time */
		g_lockSystem = OPENED;	/* Open the system again */
	}
}

/********************************************************************************
 * [Function Name]: APP_rotateMotorCW
 *
 * [Description]:   Function to rotate the Motor Clockwise
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 *********************************************************************************/
void APP_rotateMotorCW(void)
{
	++g_tick;	/* Increment global tick flag each time the interrupt fired */

	if(g_tick == FIRST_TICK)
	{
		DCMOTOR_rotateClockwise();
	}

	if(g_tick == MAXIMUM_RUNNING_TIME)
	{
		DCMOTOR_stop();
		/*
		 * De-initialization for the timer to be able to use it again correctly
		 */
		TIMER_DeInit(TIMER_1);
		g_tick = INITIAL_VALUE;	/* Clear global tick flag for the next time */
		g_lockSystem = OPENED;	/* Open the system again */
	}
}

/*********************************************************************************
 * [Function Name]: APP_stopMotor
 *
 * [Description]:   Function to stop the Motor
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 *********************************************************************************/
void APP_stopMotor(void)
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

/********************************************************************************
 * [Function Name]: APP_rotateMotorAntiCW
 *
 * [Description]:   Function to rotate the Motor Anti Clockwise
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 *********************************************************************************/
void APP_rotateMotorAntiCW(void)
{
	++g_tick;	/* Increment global tick flag each time the interrupt fired */

	if(g_tick == FIRST_TICK)
	{
		DCMOTOR_rotateAntiClockwise();
	}

	if(g_tick == MAXIMUM_RUNNING_TIME)
	{
		DCMOTOR_stop();
		/*
		 * De-initialization for the timer to be able to use it again correctly
		 */
		TIMER_DeInit(TIMER_1);
		g_tick = INITIAL_VALUE;	/* Clear global tick flag for the next time */
		g_lockSystem = OPENED;	/* Open the system again */
	}
}
