 /******************************************************************************
 *
 * [MODULE]: LED
 *
 * [FILE NAME]: led.h
 *
 * [DESCRIPTION]: Header file for the LED Driver
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#ifndef LED_H_
#define LED_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                             Preprocessor Macros                             *
 *******************************************************************************/

/* LED Pin Configurations */
#define LED_PORT_DIR 	   DDRA
#define LED_PORT_OUT	   PORTA
#define LED_PIN 	  	   PA5

/*******************************************************************************
 *                             Functions Prototypes                            *
 *******************************************************************************/

/*******************************************************************************
 * [Function Name]: LED_init
 *
 * [Description]: 	Function to Initialize the LED Driver
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
 *******************************************************************************/
void LED_init(void);

/*******************************************************************************
 * [Function Name]: LED_turnOn
 *
 * [Description]: 	Function to turn on the LED
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
 *******************************************************************************/
void LED_turnOn(void);

/*******************************************************************************
 * [Function Name]: LED_turnOff
 *
 * [Description]: 	Function to turn off the LED
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
 *******************************************************************************/
void LED_turnOff(void);

#endif /* LED_H_ */
