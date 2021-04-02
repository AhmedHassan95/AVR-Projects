 /******************************************************************************
 *
 * [MODULE]: BUZZER
 *
 * [FILE NAME]: buzzer.c
 *
 * [DESCRIPTION]: Source file for the BUZZER Driver
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#include "buzzer.h"

/*******************************************************************************
 *                            Functions Definitions                            *
 *******************************************************************************/

/*******************************************************************************
 * [Function Name]: BUZZER_init
 *
 * [Description]: 	Function to Initialize the BUZZZER Driver
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
void BUZZER_init(void)
{
	/* Configure BUZZER pin as an output pin */
	SET_BIT(BUZZER_PORT_DIR, BUZZER_PIN);
	/* Turn off BUZZER at the beginning as the initial state */
	CLEAR_BIT(BUZZER_PORT_OUT, BUZZER_PIN);
}

/*******************************************************************************
 * [Function Name]: BUZZER_turnOn
 *
 * [Description]: 	Function to turn on the BUZZZER
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
void BUZZER_turnOn(void)
{
	SET_BIT(BUZZER_PORT_OUT, BUZZER_PIN);	/* Turn On BUZZER */
}

/*******************************************************************************
 * [Function Name]: BUZZER_turnOff
 *
 * [Description]: 	Function to turn off the BUZZZER
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
void BUZZER_turnOff(void)
{
	CLEAR_BIT(BUZZER_PORT_OUT, BUZZER_PIN);	/* Turn Off BUZZER */
}
