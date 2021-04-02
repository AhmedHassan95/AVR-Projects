 /*****************************************************************************
 *
 * [MODULE]: Display
 *
 * [FILE NAME] : display.h
 *
 * [DESCRIPTION]: Header file for display
 *
 * [AUTHOR] : Ahmed Hassan
 *
 ******************************************************************************/

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "micro_config.h"

/******************************************************************************
 * [Function Name]: SevenSegment_Display
 *
 * [Description]: Function to display the number in the 7-Segment
 *
 * [Args]:	  value
 *
 * [in]		  value: Unsigned Character to be displayed through 7-Segment
 *
 * [out]          None
 *
 * [in/out]       None
 *
 * [Returns]:     None
 ******************************************************************************/
void SevenSegment_Display(unsigned char value);

#endif /* DISPLAY_H_ */
