 /*********************************************************************************
 *
 * [MODULE]: Interrupt
 *
 * [FILE NAME] : interrupt.c
 *
 * [DESCRIPTION]:  Header file for interrupt
 *
 * [AUTHOR] : Ahmed Hassan
 *
 **********************************************************************************/

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "micro_config.h"

/**********************************************************************************
 *                             External Variables                                 *
 **********************************************************************************/

extern unsigned char seconds;	/* Global variable to store the number of seconds */
extern unsigned char minutes;	/* Global variable to store the number of minutes */
extern unsigned char hours;	/* Global variable to store the number of hours */

/**********************************************************************************
 *                             Functions Prototypes                               *
 **********************************************************************************/
/**********************************************************************************
 * [Function Name]: INT0_Init
 *
 * [Description]: 	Function to enable and configuration Interrupt0
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:                 None
 **********************************************************************************/
void INT0_Init(void);

/**********************************************************************************
 * [Function Name]: INT1_Init
 *
 * [Description]: 	Function to enable and configuration Interrupt1
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:         None
 **********************************************************************************/
void INT1_Init(void);

/**********************************************************************************
 * [Function Name]: INT2_Init
 *
 * [Description]: 	Function to enable and configuration Interrupt2
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:          None
 **********************************************************************************/
void INT2_Init(void);

#endif /* INTERRUPTS_H_ */
