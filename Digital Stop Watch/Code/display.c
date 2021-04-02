 /******************************************************************************************
 *
 * [MODULE]: Display
 *
 * [FILE NAME] : display.c
 *
 * [DESCRIPTION]: Source file for display
 *
 * [AUTHOR] : Ahmed Hassan
 *
 ******************************************************************************************/

#include "display.h"

/******************************************************************************************
 *                                  Functions Definitions                                 *
 ******************************************************************************************/
/******************************************************************************************
 * [Function Name]: SevenSegment_Display
 *
 * [Description]: 	Function to display the number in the 7-Segment
 *
 * [Args]:			value
 *
 * [in]			  	value: Unsigned Character to be displayed through 7-Segment
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       		None
 ******************************************************************************************/
void SevenSegment_Display(unsigned char value)
{
	switch(value)
	{
	case 0 :
		PORTC &= ~(1<<PC0);
		PORTC &= ~(1<<PC1);
		PORTC &= ~(1<<PC2);
		PORTC &= ~(1<<PC3);
		break;
	case 1 :
		PORTC |=  (1<<PC0);
		PORTC &= ~(1<<PC1);
		PORTC &= ~(1<<PC2);
		PORTC &= ~(1<<PC3);
		break;
	case 2 :
		PORTC &= ~(1<<PC0);
		PORTC |=  (1<<PC1);
		PORTC &= ~(1<<PC2);
		PORTC &= ~(1<<PC3);
		break;
	case 3 :
		PORTC |=  (1<<PC0);
		PORTC |=  (1<<PC1);
		PORTC &= ~(1<<PC2);
		PORTC &= ~(1<<PC3);
		break;
	case 4 :
		PORTC &= ~(1<<PC0);
		PORTC &= ~(1<<PC1);
		PORTC |=  (1<<PC2);
		PORTC &= ~(1<<PC3);
		break;
	case 5 :
		PORTC |=  (1<<PC0);
		PORTC &= ~(1<<PC1);
		PORTC |=  (1<<PC2);
		PORTB &= ~(1<<PB3);
		break;
	case 6 :
		PORTC &= ~(1<<PC0);
		PORTC |=  (1<<PC1);
		PORTC |=  (1<<PC2);
		PORTC &= ~(1<<PC3);
		break;
	case 7 :
		PORTC |=  (1<<PC0);
		PORTC |=  (1<<PC1);
		PORTC |=  (1<<PC2);
		PORTC &= ~(1<<PC3);
		break;
	case 8 :
		PORTC &= ~(1<<PC0);
		PORTC &= ~(1<<PC1);
		PORTC &= ~(1<<PC2);
		PORTC |=  (1<<PC3);
		break;
	case 9 :
		PORTC |=  (1<<PC0);
		PORTC &= ~(1<<PC1);
		PORTC &= ~(1<<PC2);
		PORTC |=  (1<<PC3);
		break;
	}
}
