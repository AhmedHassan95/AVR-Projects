 /******************************************************************************
 *
 * [MODULE]: DC Motor
 *
 * [FILE NAME]: dc_motor.h
 *
 * [DESCRIPTION]: Header file for DC Motor
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                              Preprocessor Macros                            *
 *******************************************************************************/

#define MAXIMUM_SPEED 		   255	/* Duty cycle 100% */
#define THREE_QUARTERS_SPEED       192	/* Duty cycle 75% */
#define HALF_MAXIMUM_SPEED         128	/* Duty cycle 50% */
#define QUARTER_SPEED   	   64	/* Duty cycle 25% */

#define CLOCKWISE   	  	   1	/* Set Pin 0 in PORTB with value 0 & Pin 1 in PORTB with value 1 */
#define ANTI_CLOCKWISE     	   2	/* Set Pin 0 in PORTB with value 1 & Pin 1 in PORTB with value 0 */

#define ZERO_TEMP 		   0	/* Temperature = 0 */
#define LOW_TEMP		   256	/* Low Temperature	(quarter range of the sensor)*/
#define MODERAT_TEMP      	   512	/* Moderate Temperature (half range of the sensor)*/
#define HIGH_TEMP   		   768	/* High Temperature	(Three quarters range of the sensor)*/


/*******************************************************************************
 *                            Functions Prototypes                             *
 *******************************************************************************/
/********************************************************************************
 * [Function Name]: PWM_Timer0_Init
 *
 * [Description]: 	Function to Control the DC Motor direction using L293D H-bridge
 *   				- Control The DC Motor Speed using PWM from MC.
 *   				- Connect the PWM PIN of Timer0 OC0(PB3) to the enable of the Motor
 *   				- Duty cycle 100% --> Maximum Speed
 *   				- Duty cycle 75%  --> Three Quarters Speed
 *   				- Duty cycle 50%  --> Half Maximum Speed
 *   				- Duty cycle 25%  --> Quarter Speed
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
 * [Returns]:                 None
 ********************************************************************************/
void PWM_Timer0_Init(uint8 set_duty_cycle);

#endif /* DC_MOTOR_H_ */
