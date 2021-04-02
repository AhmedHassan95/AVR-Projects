 /***************************************************************************************
 *
 * [MODULE]: Timer
 *
 * [FILE NAME] : timer1.c
 *
 * [DESCRIPTION]:  Header file for timer1
 *
 * [AUTHOR] : Ahmed Hassan
 *
 *****************************************************************************************/

#ifndef TIMER1_H_
#define TIMER1_H_

#include "micro_config.h"
#include "interrupt.h"

/**********************************************************************************
 * [Function Name]: Timer1_CTC_Init
 *
 * [Description]: 	Function to count the required time (1 Second)
 *
 * [Args]:			tick
 *
 * [in]			  	tick: Unsigned Character to store the compare value in it
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 **********************************************************************************/
void Timer1_CTC_Init(unsigned int tick);

#endif /* TIMER1_H_ */
