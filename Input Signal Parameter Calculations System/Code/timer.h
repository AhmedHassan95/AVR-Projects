 /******************************************************************************
 *
 * [MODULE]: TIMER
 *
 * [FILE NAME]: timer.h
 *
 * [DESCRIPTION]: Header file for timer
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/********************************************************************************
 * [Function Name]: TIMER1_overFlow_init
 *
 * [Description]:  Function to enable and configure Timer1
 *
 * [Args]:	   None
 *
 * [in]		   None
 *
 * [out]	   None
 *
 * [in/out]	   None
 *
 * [Returns]:      None
 ********************************************************************************/
void TIMER1_overFlow_init(void);

/********************************************************************************
 * [Function Name]: TIMER1_clearTimerValue
 *
 * [Description]:  Function to clear the Timer1 Value to start count from ZERO
 *
 * [Args]:        None
 *
 * [in]	          None
 *
 * [out]	  None
 *
 * [in/out]	  None
 *
 * [Returns]:     None
 ********************************************************************************/
void TIMER1_clearTimerValue(void);

/********************************************************************************
 * [Function Name]: TIMER1_getTimerValue
 *
 * [Description]:   Function to get the Timer1 Value, to store it in the global variables,
 * 					then use these variables to calculate the Duty Cycle of the signal
 *
 * [Args]:	None
 *
 * [in]			 None
 *
 * [out]		 Unsigned Character
 *
 * [in/out]		None
 *
 * [Returns]:       The TIMER1 register value
 ********************************************************************************/
uint16 TIMER1_getTimerValue(void);

/********************************************************************************
 * [Function Name]: TIMER1_DeInit
 *
 * [Description]:   Function to disable the Timer1
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
 ********************************************************************************/
void TIMER1_DeInit(void);

#endif /* TIMER_H_ */
