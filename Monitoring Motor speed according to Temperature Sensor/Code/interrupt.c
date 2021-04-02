 /******************************************************************************
 *
 * [MODULE]: Interrupt
 *
 * [FILE NAME]: interrupt.c
 *
 * [DESCRIPTION]: Source file for interrupt
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#include "interrupt.h"

/*******************************************************************************
 *                          ISR's Definitions                                  *
 *******************************************************************************/
ISR(INT1_vect)
{
	/*
	 *	Toggle pin 0 & pin 1 in PORTB in order to revert the diction of the DC Motor
	 */
	TOGGLE_BIT(PORTB,PB0);
	TOGGLE_BIT(PORTB,PB1);
}

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/
/*******************************************************************************
 * [Function Name]: INT1_init
 *
 * [Description]:   Function to enable and configure External INT1
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
void INT1_init(void)
{
	CLEAR_BIT(DDRD,PD3);	/* Configure INT1/PD3 as input pin */
	SET_BIT(GICR,INT1);		/* Enable external interrupt pin INT1 */
	/* Trigger INT1 with the Raising edge */
	SET_BIT(MCUCR,ISC10);
	SET_BIT(MCUCR,ISC11);
	SREG |= (1<<7);		/* Enable interrupts by setting I-bit */
}
