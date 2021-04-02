 /******************************************************************************
 *
 * [MODULE]: DC Motor
 *
 * [FILE NAME]: dc_motor.c
 *
 * [DESCRIPTION]: Source file for DC Motor
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#include "dc_motor.h"

/********************************************************************************
 * [Function Name]: PWM_Timer0_Init
 *
 * [Description]: 	Function to Control the DC Motor direction using L293D H-bridge
 *   				- Control The DC Motor Speed using PWM from MC.
 *   				- Connect the PWM PIN of Timer0 OC0(PB3) to the enable of the Motor
 *   				- Duty cycle 100% --> Maximum Speed
 *   				- Duty cycle 75% --> Three Quarters Speed
 *   				- Duty cycle 50%  --> Half Maximum Speed
 *   				- Duty cycle 25% --> Quarter Speed
 *   				- Duty cycle 0%   --> Motor Stop
 *
 * [Args]:			set_duty_cycle
 *
 * [in]				set_duty_cycle: Unsigned Character to store the duty cycle in it
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 ********************************************************************************/
void PWM_Timer0_Init(uint8 set_duty_cycle)
{

	TCNT0 = 0; /* Set Timer Initial value */

	OCR0  = set_duty_cycle; /* Set Compare Value */

	/*
	 * Configure timer control register
	 * 1. Fast PWM mode FOC0 = 0
	 * 2. Fast PWM Mode WGM01 = 1 & WGM00 = 1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00 = 0 & COM01 = 1
	 * 4. clock = F_CPU/8 CS00 = 0 CS01 = 1 CS02 = 0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}
