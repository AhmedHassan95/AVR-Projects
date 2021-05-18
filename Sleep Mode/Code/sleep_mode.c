/*******************************************************************************
 *
 * [FILE NAME] : sleep_mode.c
 *
 * [AUTHOR(S)] : Ahmed Hassan
 *
 * [DATE CREATED]: May 15, 2021
 *
 * [DESCRIPTION]: This Project is to run the MC in the sleep Mode (Idle Mode).
 *
 *  		  Design Considerations MCU:
 *
 *	 	  	- Target: AVR MCU ATmega 16 (F_CPU = 1 MHZ).
 *	 	 	- LCD connected to PORTC to display messages.
 *			- SW connected to pin 2 in PORTB.
 *			- Pin 1 in PORTB used for testing that the MCU enter correctly the sleep
 *			  mode or not.
 *
 *                The sequence of the Program:
 *
 *                 	- MCU Initialization then, MCU enters the sleep mode (Idle).
 *                 	- When the button pressed (Interrupt occurred), MCU wakes up,
 *                 	  serves the interrupt, and increments the numbers of presses.
 *                 	- Also, set pin 1 in PORTB and clear it after the interrupt.
 *                 	- Then when the next round came MCU enters the sleep mode again.
 *
 *******************************************************************************/

#include "micro_config.h"
#include "lcd.h"
#include <avr/sleep.h>

/*******************************************************************************
 *                               Global Variables                              *
 *******************************************************************************/

uint8 g_press = 0;	/* Global variable to store the numbers of the press */

/*******************************************************************************
 *                        Interrupt Service Routine                            *
 *******************************************************************************/

ISR(INT2_vect)
{
	++g_press;	/* Increment the numbers of the presses  */
    PORTB |= (1<<PB1); /* Set pin 1 in PORTB with value 1 */
	LCD_goToRowColumn(0, 16);
	LCD_intgerToString(g_press);
}

/*******************************************************************************
 *                               Function Prototype                            *
 *******************************************************************************/

void INT2_Init(void)
{
	DDRB &= ~(1<<PB2);	/* Configure INT2/PB2 as input pin */
	GICR |= (1<<INT2);	/* Enable external interrupt pin INT2 */
	MCUCSR &= ~(1<<ISC2);	/* Trigger INT2 with the falling edge */
}

/********************************************************************************
 *                                MAIN FUNCTION                                 *
 ********************************************************************************/

int main (void)
{
	/********************************************************************************
	 *                           SYSTEM INITIALIZATION                              *
	 ********************************************************************************/

	 LCD_init();	/* LCD Initialization */
	 LCD_clearScreen();	/* Clear LCD screen */
	 LCD_displayStringRowColumn( 0, 5, "HELLO DUDE");	/* Welcome Message */
	 _delay_ms(1000); /* Display message for 1 second */

	 LCD_clearScreen();	/* Clear LCD screen */
	 LCD_displayStringRowColumn( 0, 5, "SLEEP MODE");
	 _delay_ms(500); /* Display message for 500ms */

	 LCD_clearScreen();	/* Clear LCD screen */
	 LCD_displayStringRowColumn( 0, 0, "NUMBER OF PRESS:");
	 LCD_goToRowColumn(0, 16);
	 LCD_intgerToString(g_press); /* Display the numbers of presses */

	 DDRB |= (1<<PB1);	/* Configure pin 1 in PORTB as output pin */

	 INT2_Init();	/* External Interrupt2 Initialization */

	 SREG |= (1<<7);	/* Enable Global Interrupt, I-bit in SREG register */

	 set_sleep_mode(SLEEP_MODE_IDLE);	/* Set the MC in the Idle Mode */

	/********************************************************************************
	 *                        APPLICATION	(SUPER LOOP)			        *
	 ********************************************************************************/

	 while(1)
	 {
		_delay_ms(1);
		sleep_mode();	/* Run the MC in the sleep Mode (Idle) */
		PORTB &= ~(1<<PB1);	/* Set pin 1 in PORTB with value 0 */
		_delay_ms(1);
	 }
}
