 /**********************************************************************************************
 *
 * [FILE NAME] : Mini_Project_1.c
 *
 * [AUTHOR(S)] : Ahmed Hassan
 *
 * [DATE CREATED]: Jan 30, 2021
 *
 * [DESCRIPTION]:  This Project is to design Digital Stop Watch.
 *
 * 		Design Considerations:
 *
 *	 	 - AVR MCU ATmega 16 (F_CPU = 1 MHZ).
 *		 - SW1 -> INT0 "Internal pull up Resistor" (Falling Edge)  -> (Reset Watch).
 *		 - SW2 -> INT1 "Pull up Resistor"          (Raising Edge)  -> (Pause Watch).
 *	         - SW3 -> INT2 "Internal pull up Resistor" (Falling Edge)  -> (Resume Watch).
 *
 ***********************************************************************************************/

#include "micro_config.h"
#include "display.h"
#include "interrupt.h"
#include "timer1.h"

/*****************************************************************************************
 *                                MAIN FUNCTION                                          *
 *****************************************************************************************/

int main(void)
{
	/********************************************************************************
	 *                           SYSTEM INITIALIZATION                              *
	 ********************************************************************************/

	SREG |= (1<<7);			/* Enable interrupts by setting I-bit */

	/* Interrupts Initialization */
	INT0_Init();
	INT1_Init();
	INT2_Init();

	Timer1_CTC_Init(1000); /* Start Counter to count 1 Second */

	DDRC = 0x0F;	  	   /* Configure First 4 pins in PORTC as output pins */
	DDRA = 0x3F;	 	   /* Configure First 5 pins in PORTA as output pins */

	PORTC = PORTC & 0xF0;  /* Set First 4 pins in PORTC with value 0 at the beginning (LEDs OFF) */
	PORTA = PORTA & 0xC0;  /* Set First 5 pins in PORTA with value 0 to disable All 7-Segments */

	while(1)
	{
		/********************************************************************************
		 *                   		    APPLICATION CODE              	        *
		 ********************************************************************************/

		PORTA = 0x01;	  /* Enable First 7-segment and disable others */
		SevenSegment_Display(seconds%10);	/* Display Seconds from 0 -> 9 */
		_delay_ms(3);
		PORTA = 0x02;	  /* Enable Second 7-segment and disable others */
		SevenSegment_Display(seconds/10);	/* Display Seconds from 10 -> 59 */
		_delay_ms(3);
		PORTA = 0x04;	  /* Enable Third 7-segment and disable others */
		SevenSegment_Display(minutes%10);	/* Display Minutes from 0 -> 9 */
		_delay_ms(3);
		PORTA = 0x08;	  /* Enable Fourth 7-segment and disable others */
		SevenSegment_Display(minutes/10);	/* Display Minutes from 10 -> 59 */
		_delay_ms(3);
		PORTA = 0x10;	  /* Enable Fifth 7-segment and disable others */
		SevenSegment_Display(hours%10);	    /* Display Hours from 0 -> 9 */
		_delay_ms(3);
		PORTA = 0x20;	  /* Enable Sixth 7-segment and disable others */
		SevenSegment_Display(hours/10);	  	/* Display Hours from 10 -> 23 */
		_delay_ms(3);

	}
}
