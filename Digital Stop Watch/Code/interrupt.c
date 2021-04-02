 /*********************************************************************************************
 *
 * [MODULE]: Interrupt
 *
 * [FILE NAME] : interrupt.c
 *
 * [DESCRIPTION]:  Source file for interrupt
 *
 * [AUTHOR] : Ahmed Hassan
 *
 **********************************************************************************************/

#include "interrupt.h"

/**********************************************************************************************
 *                             		 Global Variables                                         *
 **********************************************************************************************/

unsigned char seconds = 0;	/* Global variable to store the number of seconds */
unsigned char minutes = 0;	/* Global variable to store the number of minutes */
unsigned char hours = 0;	/* Global variable to store the number of hours */

/**********************************************************************************************/
/**********************************************************************************************
 *                      		    ISR's Definitions                      		              *
 **********************************************************************************************/
ISR(INT0_vect)
{
	/* Reset Timer 1 by Clearing Global variables */
	seconds = 0;
	minutes = 0;
	hours = 0;
}

ISR(INT1_vect)
{
	/*
	 * Disable Timer 1
	 * CS10 = 1 & CS11 = 1 & CS12 =1
	 */
	TCCR1B = (1<<CS10)  | (1<<CS11) | (1<<CS12) ;
}

ISR(INT2_vect)
{
	/*
	 * ReEnable Timer 1 :
	 * 1. CRC Mode WGM10 = 0 & WGM12 = 1 & WGM13 = 0
	 * 2. Clock = F_CPU/1024 CS10 = 1 & CS11 = 0& CS12 =1
	 */
	TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS12) ;
}

/**********************************************************************************************
 *                         			 Functions Definitions                                    *
 **********************************************************************************************/
/**********************************************************************************************
 * [Function Name]: INT0_Init
 *
 * [Description]: 	Function to enable and configuration Interrupt0
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
 **********************************************************************************************/
void INT0_Init(void)
{
	DDRD &= ~(1<<PD2);	/* Configure INT0/PD2 as input pin */
	PORTD |= (1<<PD2);	/* Enable the internal pull up resistor at PD2 */
	GICR |= (1<<INT0);	/* Enable external interrupt pin INT0 */
	/* Trigger INT0 with the falling edge */
	MCUCR |= (1<<ISC01);
	MCUCR &= ~(1<<ISC00);
	SREG |= (1<<7);		/* Enable interrupts by setting I-bit */
}

/**********************************************************************************************
 * [Function Name]: INT1_Init
 *
 * [Description]: 	Function to enable and configuration Interrupt1
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
 **********************************************************************************************/
void INT1_Init(void)
{
	DDRD &= ~(1<<PD3);	/* Configure INT1/PD3 as input pin */
	GICR |= (1<<INT1);	/* Enable external interrupt pin INT1 */
	/* Trigger INT1 with the Raising edge */
	MCUCR |= (1<<ISC10) | (1<<ISC11);
	SREG |= (1<<7);		/* Enable interrupts by setting I-bit */
}

/**********************************************************************************************
 * [Function Name]: INT2_Init
 *
 * [Description]: 	Function to enable and configuration Interrupt2
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
 **********************************************************************************************/
void INT2_Init(void)
{
	DDRB &= ~(1<<PB2);	/* Configure INT2/PB2 as input pin */
	PORTB |= (1<<PB2);	/* Enable the internal pull up resistor at PB2 */
	GICR |= (1<<INT2);	/* Enable external interrupt pin INT2 */
	MCUCSR &= ~(1<<ISC2);	/* Trigger INT2 with the falling edge */
	SREG |= (1<<7);			/* Enable interrupts by setting I-bit */
}
