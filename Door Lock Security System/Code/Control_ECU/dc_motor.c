 /******************************************************************************
 *
 * [MODULE]: DC MOTOR
 *
 * [FILE NAME]: dc_motor.c
 *
 * [DESCRIPTION]: Source file for the DC MOTOR Driver
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#include "dc_motor.h"

/*******************************************************************************
 *                             Functions Definitions                           *
 *******************************************************************************/

/*******************************************************************************
 * [Function Name]: DCMOTOR_init
 *
 * [Description]: 	Function to Initialize the DC MOTOR Driver
 *					- Decide DC MOTOR pins direction
 * 					- Turn off DC MOTOR at the beginning as the initial state
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
void DCMOTOR_init(void)
{
	/* Configure MOTOR pins as an output pin */
	SET_BIT(MOTOR_PORT_DIR, MOTOR_PIN_OUT1);
	SET_BIT(MOTOR_PORT_DIR, MOTOR_PIN_OUT2);

	/* Turn off the MOTOR at the beginning as the initial state */
	CLEAR_BIT(MOTOR_PORT_OUT, MOTOR_PIN_OUT1);
	CLEAR_BIT(MOTOR_PORT_OUT, MOTOR_PIN_OUT2);
}

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
void DCMOTOR_rotateClockwise(void)
{
	/* Force the MOTOR to rotate Clockwise */
	SET_BIT(MOTOR_PORT_OUT, MOTOR_PIN_OUT1);
	CLEAR_BIT(MOTOR_PORT_OUT, MOTOR_PIN_OUT2);
}

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
void DCMOTOR_rotateAntiClockwise()
{
	/* Force the MOTOR to rotate Anti Clockwise */
	CLEAR_BIT(MOTOR_PORT_OUT, MOTOR_PIN_OUT1);
	SET_BIT(MOTOR_PORT_OUT, MOTOR_PIN_OUT2);
}

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
void DCMOTOR_stop(void)
{
	/* Turn off the MOTOR */
	CLEAR_BIT(MOTOR_PORT_OUT, MOTOR_PIN_OUT1);
	CLEAR_BIT(MOTOR_PORT_OUT, MOTOR_PIN_OUT2);
}
