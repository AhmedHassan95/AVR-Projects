 /******************************************************************************
 *
 * [MODULE]: INTERRUPT
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
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(INT0_vect)
{
	if(g_callBackPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after the edge is detected */
		(*g_callBackPtr)(); /* another method to call the function using pointer to function g_callBackPtr(); */
	}
}
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/********************************************************************************
 * [Function Name]: INT0_init
 *
 * [Description]:   Function to enable and configure External INT0
 *
 * [Args]:	    None
 *
 * [in]		    None
 *
 * [out]	    None
 *
 * [in/out]	    None
 *
 * [Returns]:       None
 ********************************************************************************/
void INT0_init(void)
{
	DDRD &= ~(1<<PD2);	/* Configure INT0/PD2 as input pin */
	GICR |= (1<<INT0);	/* Enable external interrupt pin INT0 */
	/* Trigger INT0 with the raising edge */
	MCUCR |= (1<<ISC00) | (1<<ISC01);
}

/********************************************************************************
 * [Function Name]: INT0_setCallBack
 *
 * [Description]:   Function to set the Call Back function address
 *
 * [Args]:	    a_ptr
 *
 * [in]		    a_Ptr: Pointer to function (Receive to the address of application function)
 *
 * [out]	    None
 *
 * [in/out]	    None
 *
 * [Returns]:       None
 ********************************************************************************/
void INT0_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr = a_ptr;
}

/********************************************************************************
 * [Function Name]: INT0_DeInit
 *
 * [Description]:   Function to disable the Interrupt0 (INT0)
 *
 * [Args]:	    None
 *
 * [in]		    None
 *
 * [out]	    None
 *
 * [in/out]	    None
 *
 * [Returns]:       None
 ********************************************************************************/
void INT0_DeInit(void)
{
	/* Disable Interrupt0 (INT0), after calculate the Duty Cycle of the signal */
	GICR &= ~(1<<INT0);
}
