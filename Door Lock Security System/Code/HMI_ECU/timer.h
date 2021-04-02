/******************************************************************************
 *
 * [MODULE]: TIMER
 *
 * [FILE NAME]: timer.h
 *
 * [DESCRIPTION]: Header file for the TIMER AVR driver
 *
 * [AUTHOR]: Ahmed Hassan
 *
 ******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/******************************************************************************
 *                         Types Declaration                                  *
 ******************************************************************************/

typedef enum
{
	TIMER_0, TIMER_1, TIMER_2
}TIMER_ID;

typedef enum
{
	NORMAL, COMPARE
}TIMER_ModeType;

typedef enum
{
	T0_NO_CLOCK = 0, T0_F_CPU_CLOCK, T0_F_CPU_8, T0_F_CPU_64, T0_F_CPU_256, T0_F_CPU_1024,
	T1_NO_CLOCK = 0, T1_F_CPU_CLOCK, T1_F_CPU_8, T1_F_CPU_64, T1_F_CPU_256, T1_F_CPU_1024,
	T2_NO_CLOCK = 0, T2_F_CPU_CLOCK, T2_F_CPU_8, T2_F_CPU_32, T2_F_CPU_64 ,  T2_F_CPU_128,
	T2_F_CPU_256, T2_F_CPU_1024
}TIMER_Clock;

typedef struct
{
	uint16 intialValue;
	uint16 compareValue;
	TIMER_ID timer_ID;
	TIMER_ModeType timer_Mode;
	TIMER_Clock timer_Clock;
}TIMER_ConfigType;

/******************************************************************************
 *                          Functions Prototypes                              *
 ******************************************************************************/

/******************************************************************************
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
 ******************************************************************************/
void TIMER_init(const TIMER_ConfigType * Config_Ptr);

/******************************************************************************
 * [Function Name]: TIMER_setCallBack
 *
 * [Description]: 	Function to set the Call Back function address
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
 ******************************************************************************/
void TIMER_setCallBack(void(*a_ptr)(void), const TIMER_ID a_timerID);

/******************************************************************************
 * [Function Name]: TIMER_stop
 *
 * [Description]:   Function to stop the TIMER from counting
 *
 * [Args]:			a_timerID
 *
 * [in]				a_timerID: Enumerator to TIMER ID
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 ******************************************************************************/
void TIMER_stop(const TIMER_ID a_timerID);

/******************************************************************************
 * [Function Name]: TIMER_DeInit
 *
 * [Description]: 	Function to disable the TIMER Driver
 *
 * [Args]:			a_timerID
 *
 * [in]				a_timerID: Enumerator to TIMER ID
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 ******************************************************************************/
void TIMER_DeInit(const TIMER_ID a_timerID);

#endif /* TIMER_H_ */
