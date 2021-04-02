 /******************************************************************************
 *
 * [MODULE]: INTERRUPT
 *
 * [FILE NAME]: interrupt.h
 *
 * [DESCRIPTION]: Header file for interrupt
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
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
void INT0_init(void);

/********************************************************************************
 * [Function Name]: INT0_setCallBack
 *
 * [Description]:   Function to set the Call Back function address
 *
 * [Args]:	a_ptr
 *
 * [in]			 	a_Ptr: Pointer to function (Receive to the address of application function)
 *
 * [out]		 None
 *
 * [in/out]			None
 *
 * [Returns]:       None
 ********************************************************************************/
void INT0_setCallBack(void(*a_ptr)(void));

/********************************************************************************
 * [Function Name]: INT0_DeInit
 *
 * [Description]:   Function to disable the Interrupt0 (INT0)
 *
 * [Args]:	None
 *
 * [in]			 None
 *
 * [out]		 None
 *
 * [in/out]	 None
 *
 * [Returns]:       None
 ********************************************************************************/
void INT0_DeInit(void);

#endif /* INTERRUPT_H_ */
