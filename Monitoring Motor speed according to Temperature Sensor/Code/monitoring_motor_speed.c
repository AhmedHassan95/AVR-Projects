 /*********************************************************************************************
 *
 * [FILE NAME] : monitoring_motor_speed.c
 *
 * [AUTHOR(S)] : Ahmed Hassan
 *
 * [DATE CREATED] :	Feb 14, 2021
 *
 * [DESCRIPTION] :	This Project is to design a system that control DC Motor speed through PWM
 * 					according to temperature sensor degree that connected through ADC Driver.
 *
 * 					Design Considerations:
 *
 * 					- MCU : ATmega 16 (F_CPU = 1 MHZ).
 *					- Display Temperature degree on LCD (2 * 16) in PORTC.
 *					- Control Motor speed using PWM [TIMER CTC Mode].
 *					- Use SW connected in (Pin3) in PORTD to revert the Motor Direction[INT0].
 *					- Sensor is connected through the ADC Driver, then Display the result on
 *				      the LCD.
 *
 ***********************************************************************************************/

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"
#include "adc.h"
#include "lcd.h"
#include "dc_motor.h"
#include "interrupt.h"

/*****************************************************************************************
 *                                MAIN FUNCTION                                          *
 *****************************************************************************************/

int main(void)
{
	/********************************************************************************
	 *                           SYSTEM INITIALIZATION                              *
	 ********************************************************************************/

	uint16 res_value;	/* Global variable to store ADC register value in it */

	/*
	 * Set PB3/OC0 as output pin --> pin where the PWM signal is generated from MC.
	 * Set PB0 & PB1 as output pin --> pin determines DC MOTOR Direction.
	 */
	DDRB = DDRB | 0x0B;
	PORTB = PORTB & 0xF4; /* Set pins 0 & pin 1 & pin 3 with value 0 at the beginning (MOTOR STOP) */

	/*
	 * Insert DC MOTOR Direction in only First two bits in PORTB
	 * Assume that DC MOTOR rotates Clockwise at the beginning.
	 */
	PORTB = (PORTB & 0xFC) | (CLOCKWISE & 0x03);

	INT1_init(); /* Initialize interrupt driver */
	LCD_init();  /* Initialize LCD driver */
	ADC_init();  /* Initialize ADC driver */

	LCD_clearScreen(); /* Clear LCD at the beginning */
	LCD_displayString("ADC Value = ");	/* Display this string "ADC Value = " only once at LCD */

	/********************************************************************************
	 *                        APPLICATION	(SUPER LOOP)						    *
	 ********************************************************************************/

	while(1)
	{
		LCD_goToRowColumn(0,12); 		/* Display the number every time at this position */
		res_value = ADC_readChannel(0); /* Read channel zero where the potentiometer is connect */
		LCD_intgerToString(res_value); 	/* Display the ADC value on LCD screen */


		if(res_value > ZERO_TEMP && res_value <= LOW_TEMP)
		{
			PWM_Timer0_Init(QUARTER_SPEED);			/* Motor rotates with its Quarter speed */
		}
		else if (res_value > LOW_TEMP && res_value <= MODERAT_TEMP)
		{
			PWM_Timer0_Init(HALF_MAXIMUM_SPEED);	/* Motor rotates with its Half speed */
		}

		else if (res_value > MODERAT_TEMP && res_value <= HIGH_TEMP)
		{
			PWM_Timer0_Init(THREE_QUARTERS_SPEED);	/* Motor rotates with its Three quarters speed */
		}
		else if (res_value > HIGH_TEMP)
		{
			PWM_Timer0_Init(MAXIMUM_SPEED);			/* Motor rotates with its Full speed */
		}
	}
}
