 /******************************************************************************
 *
 * [MODULE]: LED
 *
 * [FILE NAME]: led.c
 *
 * [DESCRIPTION]: Source file for the LED Driver
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#include "led.h"

/*******************************************************************************
 *                           Functions Definitions                             *
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
void LED_init(void)
{
	/* Configure LED pin as an output pin */
	SET_BIT(LED_PORT_DIR, LED_PIN);
	/* Turn off LED at the beginning as the initial state */
	CLEAR_BIT(LED_PORT_OUT, LED_PIN);
}

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
void LED_turnOn(void)
{
	SET_BIT(LED_PORT_OUT, LED_PIN);	/* Turn On LED */
}

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
void LED_turnOff(void)
{
	CLEAR_BIT(LED_PORT_OUT, LED_PIN);	/* Turn Off LED */
}
