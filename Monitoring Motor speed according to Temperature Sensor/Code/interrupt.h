 /******************************************************************************
 *
 * [MODULE]: Interrupt
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

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*******************************************************************************
 * [Function Name]: INT1_init
 *
 * [Description]:  Function to enable and configure External INT1
 *
 * [Args]:   	   None
 *
 * [in]	    	   None
 *
 * [out]	   None
 *
 * [in/out]        None
 *
 * [Returns]:      None
 *******************************************************************************/
void INT1_init(void);

#endif /* INTERRUPT_H_ */
