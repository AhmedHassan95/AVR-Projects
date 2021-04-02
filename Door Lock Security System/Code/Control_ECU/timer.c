/************************************************************************************
 *
 * [MODULE]: TIMER
 *
 * [FILE NAME]: timer.c
 *
 * [DESCRIPTION]: Source file for the TIMER AVR driver
 *
 * [AUTHOR]: Ahmed Hassan
 *
 ************************************************************************************/

#include "timer.h"

/************************************************************************************
 *                              Global Variables                                    *
 ************************************************************************************/

/*
 * Global variable to hold the address of the call back function in the application
 * for TIMER0
 */
static volatile void (*g_callBackPtrTimer0)(void) = NULL_PTR;

/*
 * Global variable to hold the address of the call back function in the application
 * for TIMER1
 */
static volatile void (*g_callBackPtrTimer1)(void) = NULL_PTR;

/*
 * Global variable to hold the address of the call back function in the application
 * for TIMER2
 */
static volatile void (*g_callBackPtrTimer2)(void) = NULL_PTR;

/************************************************************************************
 *                             Interrupt Service Routines                           *
 ************************************************************************************/

/************************************************************************************
 *                                     TIMER0                                       *
 ************************************************************************************/

ISR(TIMER0_OVF_vect)
{
	/* Check if Pointer is not a NULL one, to avoid Segmentation fault */
	if(g_callBackPtrTimer0 != NULL_PTR)
	{
		/*
		 * Call the callback function in the application after the overflow has
		 * occurred in TIMER0 (NORMAL MODE)
		 */
		(*g_callBackPtrTimer0)();
	}
}

ISR(TIMER0_COMP_vect)
{
	/* Check if Pointer is not a NULL one, to avoid Segmentation fault */
	if(g_callBackPtrTimer0 != NULL_PTR)
	{
		/*
		 * Call the callback function in the application after the compare match has
		 * occurred in TIMER0 (COMPARE MODE)
		 */
		(*g_callBackPtrTimer0)();
	}
}
/************************************************************************************
 *                                       TIMER1                                     *
 ************************************************************************************/

ISR(TIMER1_OVF_vect)
{
	/* Check if Pointer is not a NULL one, to avoid Segmentation fault */
	if(g_callBackPtrTimer1 != NULL_PTR)
	{
		/*
		 * Call the callback function in the application after the overflow has
		 * occurred in TIMER1 (NORMAL MODE)
		 */
		(*g_callBackPtrTimer1)();
	}
}

ISR(TIMER1_COMPA_vect)
{
	/* Check if Pointer is not a NULL one, to avoid Segmentation fault */
	if(g_callBackPtrTimer1 != NULL_PTR)
	{
		/*
		 * Call the callback function in the application after the compare match has
		 * occurred in TIMER1 Channel A (COMPARE MODE)
		 */
		(*g_callBackPtrTimer1)();
	}
}

/************************************************************************************
 *                                       TIMER2                                     *
 ************************************************************************************/

ISR(TIMER2_OVF_vect)
{
	/* Check if Pointer is not a NULL one, to avoid Segmentation fault */
	if(g_callBackPtrTimer2 != NULL_PTR)
	{
		/*
		 * Call the callback function in the application after the overflow has
		 * occurred in TIMER2 (NORMAL MODE)
		 */
		(*g_callBackPtrTimer2)();
	}
}

ISR(TIMER2_COMP_vect)
{
	/* Check if Pointer is not a NULL one, to avoid Segmentation fault */
	if(g_callBackPtrTimer2 != NULL_PTR)
	{
		/*
		 * Call the callback function in the application after the compare match has
		 * occurred in TIMER2 (COMPARE MODE)
		 */
		(*g_callBackPtrTimer2)();
	}
}

/************************************************************************************
 *                              Functions Definitions                               *
 ************************************************************************************/

/************************************************************************************
 * [Function Name]: TIMER_init
 *
 * [Description]: 	Function to initialize the TIMER Driver
 *					- Decide TIMER ID (TIMER0, TIMER1, TIMER2)
 *					- Decide TIMER Mode (NORMAL, COMPARE)
 * 					- Decide TIMER Clock (NO CLOCK, F_CPU CLOCK, F_CPU CLOCK/8, ..)
 * 					- Insert the initial value to start counting from it
 * 					- Insert the compare match value if use this mode (CTC Mode)
 * 					- Enable TIMER Interrupt
 *
 * [Args]:			Config_Ptr
 *
 * [in]			  	Config_Ptr: Pointer to TIMER Configuration Structure
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 *************************************************************************************/
void TIMER_init(const TIMER_ConfigType * Config_Ptr)
{
	switch(Config_Ptr -> timer_ID)
	{

	case TIMER_0:

		/*
		 * Clear All timer register before doing any operation,
		 * to ensure that timer will work correctly
		 */
		TCNT0 = 0x00;
		TCCR0 = 0x00;
		OCR0  = 0x00;

		/*
		 * Insert the required initial value in the TCNT0 register
		 * TIMER0 is (8-bit), so mask the initial value with 0xFF to ensure that it
		 * does not exceed 255
		 */
		TCNT0 = ( (Config_Ptr -> intialValue) & 0xFF);

		switch(Config_Ptr -> timer_Mode)
		{

		/* Configure TIMER0 Register (NORMAL MODE):
		 *
		 * 	- Non PWM mode (FOC0 = 1) in TCCR0 register
		 * 	- Normal Mode  (WGM00 = 0 & WGM01 = 0) in TCCR0 register
		 * 	- Normal Mode  (COM01 = 0 & COM00 = 0) in TCCR0 register
		 * 	- Insert the required Clock in the First three bits (CS02, CS01, CS00)
		 * 	  in TCCR0 register
		 * 	- Enable TIMER0 Overflow Interrupt (TOIE0) bit in TIMSK register
		 */
		case NORMAL:

			TCCR0  = (1 << FOC0);
			TCCR0  = (TCCR0 & 0xF8) | (Config_Ptr -> timer_Clock);
			TIMSK |= (1 << TOIE0);
			break;

	    /* Configure TIMER0 Register (COMPARE MODE):
		 *
		 *	- Insert the required compare value in the OCR0 register, TIMER0 is (8-bit),
		 *	  so mask the compare value with 0xFF to ensure that it does not exceed 255
		 * 	- Non PWM Mode (FOC0 = 1) in TCCR0 register
		 * 	- Compare Mode (WGM00 = 0 & WGM01 = 1) in TCCR0 register
		 * 	- Normal  Mode (COM01 = 0 & COM00 = 0) in TCCR0 register
		 * 	- Insert the required Clock in the First three bits (CS02, CS01, CS00)
		 * 	  in TCCR0 register
		 * 	- Enable TIMER0 Compare Match Interrupt (OCIE0) bit in TIMSK register
		 */
		case COMPARE:

			OCR0   = ( (Config_Ptr -> compareValue) & 0xFF);
			TCCR0  = (1 << FOC0) | (1 << WGM01);
			TCCR0  = (TCCR0 & 0xF8) | (Config_Ptr -> timer_Clock);
			TIMSK |= (1 << OCIE0);
			break;

		}	/* END CASE OF TIMER0 */
		break;

	case TIMER_1:

		/*
		 * Clear All timer register before doing any operation,
		 * to ensure that timer will work correctly
		 */
		TCNT1  = 0x0000;
		OCR1A  = 0x0000;
		TCCR1A = 0x00;
		TCCR1B = 0x00;

		/*
		 * Insert the required initial value in the TCNT1 register
		 * TIMER1 is (16-bit), so mask the initial value with 0xFFFF to ensure that it
		 * does not exceed 65535
		 */
		TCNT1 = ( (Config_Ptr -> intialValue) & 0xFFFF);

		switch(Config_Ptr -> timer_Mode)
		{

		/* Configure TIMER1 Register (NORMAL MODE):
		 *
		 * 	- Normal port operation, OC1A/OC1B disconnected
		 * 	- Normal Mode (COM1A1 = 0 & COM1A0 = 0) in TCCR1A register
		 * 	- Normal Mode (COM1B1 = 0 & COM1B0 = 0) in TCCR1A register
		 *  - Non PWM Mode (FOC1A = 1 & FOC1B = 1)  in TCCR1A register
		 * 	- Normal  Mode (WGM10 = 0 & WGM11 = 0 & WGM12 = 0 & WGM13 = 0)
		 * 	- Insert the required Clock in the First three bits (CS12, CS11, CS10)
		 * 	  in TCCR1B register
		 * 	- Enable TIMER1 Overflow Interrupt (TOIE1) bit in TIMSK register
		 */
		case NORMAL:

			TCCR1A = (1 << FOC1A)  | (1 << FOC1B);
			TCCR1B  = (TCCR1B & 0xF8) | (Config_Ptr -> timer_Clock);
			TIMSK |= (1 << TOIE1);
			break;

		/* Configure TIMER1 Register (COMPARE MODE):
		 *
		 *	- Insert the required compare value in the OCR1A register, TIMER1 is (16-bit),
		 *	  so mask the compare value with 0xFFFF to ensure that it does not exceed 65535
		 *	- Normal port operation, OC1A/OC1B disconnected
		 * 	- Normal Mode (COM1A1 = 0 & COM1A0 = 0) in TCCR1A register
	     * 	- Normal Mode (COM1B1 = 0 & COM1B0 = 0) in TCCR1A register
		 * 	- Non PWM Mode (FOC1A = 1 & FOC1B = 1)  in TCCR1A register
		 *	- Compare Mode (WGM10 = 0 & WGM11 = 0 & WGM12 = 1 & WGM13 = 0)
		 * 	- Insert the required Clock in the First three bits (CS22, CS21, CS20)
		 * 	  in TCCR1B register
		 * 	- Enable TIMER2 Compare Match Interrupt (OCIE1A) bit in TIMSK register
		 */
		case COMPARE:

			OCR1A   = ( (Config_Ptr -> compareValue) & 0xFFFF);
			TCCR1A  = (1 << FOC1A) | (1 << FOC1B);
			TCCR1B |= (1 << WGM12);
			TCCR1B  = (TCCR1B & 0xF8) | (Config_Ptr -> timer_Clock);
			TIMSK  |= (1 << OCIE1A);
			break;

		}	/* END CASE OF TIMER1 */
		break;

	case TIMER_2:

		/*
		 * Clear All timer register before doing any operation,
		 * to ensure that timer will work correctly
		 */
		TCNT2 = 0x00;
		TCCR2 = 0x00;
		OCR2  = 0x00;

		/*
		 * Insert the required initial value in the TCNT2 register
		 * TIMER2 is (8-bit), so mask the initial value with 0xFF to ensure that it
	     * does not exceed 255
		 */
		TCNT2 = ( (Config_Ptr -> intialValue) & 0xFF);

		switch(Config_Ptr -> timer_Mode)
		{

		/* Configure TIMER2 Register (NORMAL MODE):
		 *
		 * 	- Non PWM Mode (FOC2 = 1) in TCCR2 register
		 * 	- Normal  Mode (WGM20 = 0 & WGM21 = 0) in TCCR2 register
		 * 	- Normal  Mode (COM21 = 0 & COM20 = 0) in TCCR2 register
		 * 	- Insert the required Clock in the First three bits (CS22, CS21, CS20)
		 * 	  in TCCR2 register
		 * 	- Enable TIMER2 Overflow Interrupt (TOIE2) bit in TIMSK register
		 */
		case NORMAL:

			TCCR2  = (1 << FOC2);
			TCCR2  = (TCCR2 & 0xF8) | (Config_Ptr -> timer_Clock);
			TIMSK |= (1 << TOIE2);
			break;

		/* Configure TIMER2 Register (COMPARE MODE):
		 *
		 *	- Insert the required compare value in the OCR2 register, TIMER2 is (8-bit),
		 *	  so mask the compare value with 0xFF to ensure that it does not exceed 255
		 * 	- Non PWM Mode (FOC2 = 1) in TCCR2 register
		 * 	- Compare Mode (WGM20 = 0 & WGM21 = 1) in TCCR2 register
		 * 	- Normal  Mode (COM21 = 0 & COM20 = 0) in TCCR2 register
		 *  - Insert the required Clock in the First three bits (CS22, CS21, CS20)
		 * 	  in TCCR2 register
		 * 	- Enable TIMER2 Compare Match Interrupt (OCIE2) bit in TIMSK register
		 */
		case COMPARE:

			OCR2   = ( (Config_Ptr -> compareValue) & 0xFF);
			TCCR2  = (1 << FOC2) | (1 << WGM21);
			TCCR2  = (TCCR2 & 0xF8) | (Config_Ptr -> timer_Clock);
			TIMSK |= (1 << OCIE2);
			break;

		}	/* END CASE OF TIMER2 */
		break;
	}
}

/************************************************************************************
 * [Function Name]: TIMER_setCallBack
 *
 * [Description]:   Function to set the Call Back function address
 *
 * [Args]:			a_Ptr, a_timerID
 *
 * [in]				a_Ptr: Pointer to function (Receive to the address of application function)
 * 					a_timerID: Enumerator to TIMER ID
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 *************************************************************************************/
void TIMER_setCallBack(void(*a_ptr)(void), const TIMER_ID a_timerID)
{
	switch(a_timerID)
	{

	case TIMER_0:

		/* Save the address of the Call back function of TIMER0 in a global variable */
		g_callBackPtrTimer0 = a_ptr;
		break;

	case TIMER_1:

		/* Save the address of the Call back function of TIMER1 in a global variable */
		g_callBackPtrTimer1 = a_ptr;
		break;

	case TIMER_2:

		/* Save the address of the Call back function of TIMER2 in a global variable */
		g_callBackPtrTimer2 = a_ptr;
		break;
	}
}

/*************************************************************************************
 * [Function Name]: TIMER_stop
 *
 * [Description]:   Function to stop the TIMER from counting
 *
 * [Args]:			None
 *
 * [in]				None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 *******************************************************************************/
void TIMER_stop(const TIMER_ID a_timerID)
{
	switch(a_timerID)
	{

	case TIMER_0:

		TCCR0  = ( TCCR0 & 0xF8 );	/* Clear Clock bits in TIMER0 */
		break;

	case TIMER_1:

		TCCR1B = (TCCR1B & 0xF8);	/* Clear Clock bits in TIMER1 */
		break;

	case TIMER_2:

		TCCR2  = ( TCCR2 & 0xF8 );	/* Clear Clock bits in TIMER2 */
		break;
	}
}

/****************************************************************************************
 * [Function Name]: TIMER_DeInit
 *
 * [Description]: 	Function to disable the TIMER Driver
 *
 * [Args]:			None
 *
 * [in]				None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 ****************************************************************************************/
void TIMER_DeInit(const TIMER_ID a_timerID)
{
	switch(a_timerID)
	{

	case TIMER_0:

		/* Clear All TIMER0 Register */
		TCNT0 = 0x00;
		TCCR0 = 0x00;
		OCR0  = 0x00;

		/* Disable TIMER0 Interrupt Enable */
		TIMSK &= ~(1 << TOIE0) & ~(1 << OCIE0);

		/* Disable TIMER0 Interrupt Flag, by writing a logic one */
		TIFR |= (1 << OCF0) | (1 << TOV0);
		break;

	case TIMER_1:

		/* Clear All TIMER1 Register */
		TCNT1  = 0x0000;
		OCR1A  = 0x0000;
		TCCR1A = 0x00;
		TCCR1B = 0x00;

		/* Disable TIMER1 Interrupt Enable */
		TIMSK &= ~(1 << TOIE1) & ~(1 << OCIE1A);

		/* Disable TIMER1 Interrupt Flag, by writing a logic one */
		TIFR |= (1 << OCF1A) | (1 << OCF1B) | (1 << TOV1);
		break;

	case TIMER_2:

		/* Clear All TIMER2 Register */
		TCNT2 = 0x00;
		TCCR2 = 0x00;
		OCR2  = 0x00;

		/* Disable TIMER2 Interrupt Flags */
		TIMSK &= ~(1 << TOIE2) & ~(1 << OCIE2);

		/* Disable TIMER2 Interrupt Flag, by writing a logic one */
		TIFR |= (1 << OCF2) | (1 << TOV2);
		break;
	}
}
