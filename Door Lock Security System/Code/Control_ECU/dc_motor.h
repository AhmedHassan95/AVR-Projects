 /******************************************************************************
 *
 * [MODULE]: DC MOTOR
 *
 * [FILE NAME]: dc_motor.h
 *
 * [DESCRIPTION]: Header file for the DC MOTOR Driver
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
 *                             Preprocessor Macros                             *
 *******************************************************************************/

/* DC MOTOR Pins Configuration */
#define MOTOR_PORT_OUT	   PORTA
#define MOTOR_PORT_DIR 	   DDRA

#define MOTOR_PIN_OUT1 	   PA3
#define MOTOR_PIN_OUT2 	   PA4

/*******************************************************************************
 *                             Functions Prototypes                            *
 *******************************************************************************/

/*******************************************************************************
 * [Function Name]: DCMOTOR_init
 *
 * [Description]: 	Function to Initialize the DC MOTOR Driver
 *					- Decide DC MOTOR pins direction
 * 					- Turn off the DC MOTOR at the beginning as the initial state
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
void DCMOTOR_init(void);

/*******************************************************************************
 * [Function Name]: DCMOTOR_rotateClockwise
 *
 * [Description]: 	Function to force the DC MOTOR to rotate Clockwise
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
void DCMOTOR_rotateClockwise(void);

/*******************************************************************************
 * [Function Name]: DCMOTOR_rotateAntiClockwise
 *
 * [Description]: 	Function to force the DC MOTOR to rotate Anti Clockwise
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
void DCMOTOR_rotateAntiClockwise(void);

/*******************************************************************************
 * [Function Name]: DCMOTOR_stop
 *
 * [Description]: 	Function to stop the DC MOTOR
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
void DCMOTOR_stop(void);

#endif /* DC_MOTOR_H_ */
