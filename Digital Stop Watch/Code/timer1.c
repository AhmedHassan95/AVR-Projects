 /***************************************************************************************
 *
 * [MODULE]: Timer
 *
 * [FILE NAME] : timer1.c
 *
 * [DESCRIPTION]:  Source file for timer1
 *
 * [AUTHOR(S)] : Ahmed Hassan
 *
 ****************************************************************************************/

#include "timer1.h"

/****************************************************************************************
 *                      		    ISR's Definitions                      		        *
 ****************************************************************************************/
ISR(TIMER1_COMPA_vect)
{
	seconds++;

	if(seconds == 60)
	{
		seconds = 0;
		minutes++;
	}
	if(minutes == 60)
	{
		minutes = 0;
		hours++;
	}
	if(hours > 23)
	{
		hours = 0;
	}
}

/****************************************************************************************
 *                         		 Functions Definitions                                  *
 ****************************************************************************************/
/****************************************************************************************
 * [Function Name]: Timer1_CTC_Init
 *
 * [Description]: 	Function to Function to count the required time (1 Second)
 *
 * [Args]:			tick
 *
 * [in]			  	tick: Unsigned Character to store the compare value in it
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 ****************************************************************************************/
void Timer1_CTC_Init(unsigned int tick)
{
	TCNT1 = 0;		/* Set Counter initial value to 0 */
	OCR1A = tick;	/* Set compare value */
	TIMSK = (1<<OCIE1A);	/* Enable Timer 1 Compare A Match Interrupt */

	/* Configure Timer 1 Control Register:
	 * 1. Non PWM Mode FOC1A = 1 & FOC1B = 1
	 * 2. CRC Mode WGM10 = 0 & WGM12 = 1 & WGM13 = 0
	 * 3. Clock = F_CPU/1024 CS10 = 1 & CS11 = 0& CS12 =1
	 */
	TCCR1A = (1 << FOC1A)  | (1 << FOC1B);

	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS12) ;
}
