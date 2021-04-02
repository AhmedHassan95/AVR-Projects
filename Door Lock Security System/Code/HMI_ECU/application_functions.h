 /*******************************************************************************
 *
 * [MODULE]: APPLICATION
 *
 * [FILE NAME]: application_functions.h
 *
 * [DESCRIPTION]: Header file for the Application Functions Used by the HMI ECU
 *
 * [AUTHOR]: Ahmed Hassan
 *
 ********************************************************************************/

#ifndef APPLICATION_FUNCTIONS_H_
#define APPLICATION_FUNCTIONS_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include "timer.h"
#include "uart.h"
#include "lcd.h"
#include "keypad.h"

/********************************************************************************
 *                           Preprocessor Macros                                *
 ********************************************************************************/

#define PASSWORD_WIDTH      	5		/* Password consists of 5 digits */

#define ANY_TIME_TO_LOGIN   	0		/* Any time to login to the system */
#define FIRST_TIME_TO_LOGIN 	1		/* First time to login in the system */

#define MATCHED                 0		/* Two passwords are matched */
#define MISMATCHED              1		/* Two passwords are not matched */

#define NO_ATTEMPT		0		/* There are not try yet */
#define MAXIMUM_ATTEMPTS        3		/* Maximum wrong attempts to enter the system */

#define  INITIAL_VALUE          0               /* Initial value to start the timer count from it */
#define  COUNT_ONE_SECOND       8000            /* Number be loaded in the timer to count 1S */

#define  UNLOCKED               0               /* Unlock the system */
#define  OPENED                 1		/* System Opened */

#define MAXIMUM_ALARM_TIME      15              /* Maximum time to turn on the Alarm */
#define	MAXIMUM_RUNNING_TIME    15		/* Maximum time to turn on the Motor */
#define MAXIMUM_STOPING_TIME    3		/* Maximum time to turn off the Motor */

#define CHANGE_PASSWORD     	43 		/* ACII Code for '+' */
#define DOOR_OPEN               45		/* ACII Code for '-' */

#define PRESS_TIME              350		/* Time taken for each press */
#define LONG_DISPLAY_TIME       800		/* Long Time taken for displaying message in the screen */
#define SHORT_DISPLAY_TIME      500		/* Short Time taken for displaying message in the screen */

#define ECU_READY           	0x10	/* Byte sent to the receiver to indicate that it is ready */

/********************************************************************************
 *                            External Variables                                *
 ********************************************************************************/

/* Extern Public global variables to be used by other modules */
extern uint8 g_password1[PASSWORD_WIDTH];/* Global Array to store password1 */
extern uint8 g_password2[PASSWORD_WIDTH];/* Global Array to store password2 */
extern uint8 g_systemStatus;	         /* Global variable to store the system status */
extern uint8 g_tick;	                 /* Global tick flag */
extern uint8 g_lockSystem;		 /* Global flag to store the system status */
extern uint8 g_wrongAttempts;            /* Global variable to store the number of wrong attempts */

/********************************************************************************
 *                         Functions Prototypes                                 *
 ********************************************************************************/

/********************************************************************************
 * [Function Name]: APP_setNewPassword
 *
 * [Description]: Function to set the new user password
 * 		  - Display a message, if this is the first time to login to the system
 * 		  - Take new password from the user for two times
 * 		  - Compare between these two passwords
 * 		  - If two passwords are identical, send password to the Control ECU to be
 * 		    stored in the EEPROM
 * 		  - If two passwords are not identical, repeat these steps again
 *
 * [Args]:	a_systemStatus
 *
 * [in]		a_systemStatus: Unsigned Character (store the password status)
 *
 * [out]       None
 *
 * [in/out]	 None
 *
 * [Returns]:   None
 ********************************************************************************/
void APP_setNewPassword(uint8 a_systemStatus);

/********************************************************************************
 * [Function Name]: APP_getPassword
 *
 * [Description]:   Function to get password from the User
 *				  	- Store password consists of 5 digits in a global array
 * 					- Give the User two options ('=' : TO SUBMIT, 'X
