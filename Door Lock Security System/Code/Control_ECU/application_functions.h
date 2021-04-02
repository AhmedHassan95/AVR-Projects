 /********************************************************************************
 *
 * [MODULE]: APPLICATION
 *
 * [FILE NAME]: application_functions.h
 *
 * [DESCRIPTION]: Header file for the Application Functions Used by the Control ECU
 *
 * [AUTHOR]: Ahmed Hassan
 *
 **********************************************************************************/

#ifndef APPLICATION_FUNCTIONS_H_
#define APPLICATION_FUNCTIONS_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include "timer.h"
#include "uart.h"
#include "external_eeprom.h"
#include "dc_motor.h"
#include "buzzer.h"
#include "led.h"

/*********************************************************************************
 *                              Preprocessor Macros                              *
 *********************************************************************************/

#define ECU_READY         	    0x10	/* Byte sent to the receiver to indicate that it is ready */
#define PASSWORD_WIDTH    	    5		/* Password consists of 5 digits */

#define DEFAULT_VALUE		    0xFF	/* The default value for each EEPROM location (Erased) */
#define ADDRESS_PASSWORD_STORED	    0x000	/* Address where password stored */

#define STORE_TIME     		    100 	/* Time taken for storing 1 Byte in the EEPROM */
#define READ_TIME     		    100 	 /* Time taken for reading 1 Byte in the EEPROM */

#define ANY_TIME_TO_LOGIN   	    0		/* Any time to login to the system */
#define FIRST_TIME_TO_LOGIN	    1		 /* First time to login to the system */

#define MATCHED                     0	       /* Two password are matched */
#define MISMATCHED                  1		/* Two password are not matched */

#define NO_ATTEMPT		    0		/* There are not attempt yet */
#define MAXIMUM_ATTEMPTS            3		/* Maximum wrong attempts to enter the system */

#define  INITIAL_VALUE              0          /* Initial value to start the timer count from it */
#define  FIRST_TICK                 1		/* First time the interrupt occurred */

#define  UNLOCKED                   0          /* Unlock the system */
#define  OPENED                     1		/* System Opened */

#define MAXIMUM_ALARM_TIME          15           /* Maximum time to turn on the Alarm */
#define	MAXIMUM_RUNNING_TIME        15		/* Maximum time to turn on the Motor */
#define MAXIMUM_STOPING_TIME        3		 /* Maximum time to turn off the Motor */

#define  COUNT_ONE_SECOND           8000      /* Number be loaded in the timer to count 1S */

#define CHANGE_PASSWORD     	    43 		/* ACII Code for '+' */
#define DOOR_OPEN          	    45		 /* ACII Code for '-' */

/**********************************************************************************
 *                             External Variables                                 *
 **********************************************************************************/

extern uint8 g_newPassword[PASSWORD_WIDTH];     /* Global Array to store new password */
extern uint8 g_currentPassword[PASSWORD_WIDTH];	/* Global Array to store current password */
extern uint8 g_oldPassword[PASSWORD_WIDTH];	/* Global Array to store old password */
extern uint8 g_tick;                            /* Global tick flag */
extern uint8 g_lockSystem;                      /* Global flag to store the system status */
extern uint8 g_wrongAttempts;                   /* Global variable to store the number of wrong attempts */

/**********************************************************************************
 *                             Functions Prototypes                               *
 **********************************************************************************/

/**********************************************************************************
 * [Function Name]: APP_checkPasswordMismatch
 *
 * [Description]: 	Function to check if the two passwords are identical or not
 *
 * [Args]:			Password1_Ptr, Password2_Ptr
 *
 * [in]		Password1_Ptr: Pointer to character
 * 					Password2_Ptr: Pointer to character
 *
 * [out]		 	Unsigned Character
 *
 * [in/out]		 	None
 *
 * [Returns]:       Result of the comparison between two passwords
 **********************************************************************************/
uint8 APP_checkPasswordMismatch(uint8 * Password1_Ptr , uint8 * Password2_Ptr);

/**********************************************************************************
 * [Function Name]: APP_receiveNewPassword
 *
 * [Description]: 	Function to receive the new password from the HMI ECU
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
void APP_receiveNewPassword(uint8 * Password_Ptr);

/**********************************************************************************
 * [Function Name]: APP_receiveCurrentPassword
 *
 * [Description]: 	Function to receive the new password from the HMI ECU, and store it
 *				  	in the EEPROM
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
void APP_receiveCurrentPassword(uint8 * Password_Ptr);

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
void APP_readOldPassword(void);

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
void APP_incorrectPassword(void);

/**********************************************************************************
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
 ***********************************************************************************/
void APP_rotateMotorCW(void);

/***********************************************************************************
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
 ***********************************************************************************/
void APP_stopMotor(void);

/***********************************************************************************
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
 ***********************************************************************************/
void APP_rotateMotorAntiCW(void);

#endif /* APPLICATION_FUNCTIONS_H_ */
