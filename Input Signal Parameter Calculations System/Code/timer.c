 /******************************************************************************
 *
 * [MODULE]: TIMER
 *
 * [FILE NAME]: timer.c
 *
 * [DESCRIPTION]: Source file for timer
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#include "timer.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/********************************************************************************
 * [Function Name]: TIMER1_overFlow_init
 *
 * [Description]:   Function to enable and configure Timer1
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
 ********************************************************************************/
void TIMER1_overFlow_init(void)
{
	TCNT1 = 0;		/* Set Timer1 initial value to 0 */

	TCCR1A = (1<<FOC1A) | (1<<FOC1B);	/* Timer1 always operates in Normal Mode */

	/* Configure Timer 1 Control Register:
	1. Normal Mode WGM10 = 0 & WGM12 = 0 & WGM13 = 0
	2. Clock = F_CPU CS10 = 1 & CS11 = 0 & CS12 = 0
	 */
	TCCR1B = (1<<CS10) ;
}

/********************************************************************************
 * [Function Name]: TIMER1_clearTimerValue
 *
 * [Description]:   Function to clear the Timer1 Value to start count from ZERO
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
 ********************************************************************************/
void TIMER1_clearTimerValue(void)
{
	TCNT1 = 0;
}

/********************************************************************************
 * [Function Name]: TIMER1_getTimerValue
 *
 * [Description]:   Function to get the Timer1 Value, to store it in the global variables,
 * 					then use these variables to calculate the Duty Cycle of the signal
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	Unsigned Character
 *
 * [in/out]		 	None
 *
 * [Returns]:       The TIMER1 register value
 ********************************************************************************/
uint16 TIMER1_getTimerValue(void)
{
	return TCNT1;
}

/********************************************************************************
 * [Function Name]: TIMER1_DeInit
 *
 * [Description]:   Function to disable the Timer1
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
 ********************************************************************************/
void TIMER1_DeInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
}
