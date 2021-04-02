/********************************************************************************************
 *
 * [FILE NAME] : signal_parameters_calculation.c
 *
 * [AUTHOR(S)] : Ahmed Hassan
 *
 * [DATE CREATED] :	Feb 28, 2021
 *
 * [DESCRIPTION] :	This project is Design a system to calculate the duty Cycle of any
 * 					input signal using only Timer and Interrupt, then display the result
 * 					on LCD (2 * 16).
 *
 * 					Design Considerations:
 *
 * 						- MCU : ATmega 16 (F_CPU = 1 MHZ).
 *						- Use Timer1 Overflow Mode (NO_Prescaler).
 *						- Use External Interrupt0 (INT0), to enter the input Signal into MCU.
 *						- Display result on LCD (2 * 16) in PORTC.
 *
 *******************************************************************************************/

#include "interrupt.h"
#include "lcd.h"
#include "timer.h"

/*****************************************************************************************
 *                   				 Global Variables                                    *
 *****************************************************************************************/

uint8 g_edgeCount = 0;
uint16 g_timeHigh = 0;
uint16 g_timePeriod = 0;
uint16 g_timePeriodPlusHigh = 0;

/*******************************************************************************************
 * [Function Name]: APP_measure_Duty_Cycle
 *
 * [Description]: This function is responsible for calculate the Duty Cycle of
 * 				  input signal, by storing the Timer1 value of High, Period and
 * 				  PeriodPlusHigh, then use this equation:
 * 				  	DutyCycle = ((g_timePeriodPlusHigh-g_timePeriod) /
 * 				  				 (g_timePeriodPlusHigh - g_timeHigh)) * 100
 * [Args]:
 *
 * [in]			  None
 *
 * [out]		  None
 *
 * [in/out]		  None
 *
 * [Returns]:     None
 *********************************************************************************/
void APP_measure_Duty_Cycle(void)
{
	g_edgeCount++;
	if(g_edgeCount == 1)
	{
		/*
		 * Clear the timer counter register to start measurements from the
		 * first detected rising edge
		 */
		TIMER1_clearTimerValue();
		/* Detect falling edge */
		CLEAR_BIT(MCUCR,ISC00);
	}
	else if(g_edgeCount == 2)
	{
		/* Store the High time value */
		g_timeHigh = TIMER1_getTimerValue();
		/* Detect rising edge */
		SET_BIT(MCUCR,ISC00);
	}
	else if(g_edgeCount == 3)
	{
		/* Store the Period time value */
		g_timePeriod = TIMER1_getTimerValue();
		/* Detect falling edge */
		CLEAR_BIT(MCUCR,ISC00);
	}
	else if(g_edgeCount == 4)
	{
		/* Store the Period time value + High time value */
		g_timePeriodPlusHigh = TIMER1_getTimerValue();
		/* Clear the timer counter register to start measurements again */
		TIMER1_clearTimerValue();
		/* Detect rising edge */
		SET_BIT(MCUCR,ISC00);
	}
}

/*****************************************************************************************
 *                                MAIN FUNCTION                                          *
 *****************************************************************************************/

int main(void)
{
	/********************************************************************************
	 *                           SYSTEM INITIALIZATION                              *
	 ********************************************************************************/

	uint32 dutyCycle = 0;

	/* Enable Global Interrupt I-Bit */
	SREG |= (1<<7);

	/* Set the Call back function pointer in the Inetrrupt0 */
	INT0_setCallBack(APP_measure_Duty_Cycle);

	/* Initialize LCD driver, Interrupt0, and Timer1 */
	LCD_init();
	INT0_init();
	TIMER1_overFlow_init();

	/********************************************************************************
	 *                        APPLICATION	(SUPER LOOP)						    *
	 ********************************************************************************/

	while(1)
	{
		if(g_edgeCount == 4)
		{
			TIMER1_DeInit();	/* Disable Timer1 */
			INT0_DeInit();		/* Disable Interrupt0 (INT0) */
			g_edgeCount = 0;
			LCD_displayString("Duty = ");
			/* Calculate the Duty Cycle */
			dutyCycle = ((float)(g_timePeriodPlusHigh-g_timePeriod) / (g_timePeriodPlusHigh - g_timeHigh)) * 100;
			/* Display the Duty Cycle on LCD screen */
			LCD_intgerToString(dutyCycle);
			LCD_displayCharacter('%');
		}
	}
}
