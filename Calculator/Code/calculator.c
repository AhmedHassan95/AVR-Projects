/********************************************************************************************
 *
 * [FILE NAME] : calculator.c
 *
 * [AUTHOR(S)] : Ahmed Hassan.
 *
 * [DATE CREATED] :	Feb 20, 2021
 *
 * [DESCRIPTION] :	This Project is to design a simple calculator to perform the
 * 			simple operations.
 * 			This operations include (Addition, Subtraction, Multiplication,
 * 			Division) between two operands.
 *
 * 			Design Considerations:
 *
 * 			- MCU : ATmega 16 (F_CPU = 1 MHZ).
 *			- Display result on LCD (4 * 16) in PORTD.
 *			- Result should consider Negative number, Floating Point,
 *			  More than one operand.
 *
 *
 * Note: "Project doesn't work correctly without these Steps"
 *
 * As “There is no Floating Point Unit in AVR”. We can do that using a software library,
 * so follow these steps to add it:
 *
 * 1- After create your Project -> Right Click “Choose Properties” -> C/C++ Build “Setting”
 *    -> AVR C Linker “Libraries” -> Choose Add Button in “Libraries (-l)”
 *    -> Then Add (printf_flt).
 * 2- Also AVR C Linker “General” -> Then in “Other Arguments Tab” -> Then Add (-uvfprintf).
 *
 *******************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "lcd.h"
#include "keypad.h"

/******************************************************************************************
 * 				 Preprocessor Macros   			   	          *
 ******************************************************************************************/

#define NO_OP 0 /* NO operation */
#define ADD 1 	/* Addition operation */
#define SUB 2 	/* Subtraction operation */
#define MUL 3 	/* Multiplication operation */
#define DIV 4 	/* Division operation */
#define EQL 5 	/* Equal operation */
#define CLS 13 	/* Clear operation */

/*****************************************************************************************
 *                                MAIN FUNCTION                                          *
 *****************************************************************************************/

int main(void)
{
	/********************************************************************************
	 *                           SYSTEM INITIALIZATION                              *
	 ********************************************************************************/

	uint8 key, operation, buf[16] = {0};	/* Buffer to store data that needed to display */
	uint8 flage_op = 0;
	float32 result;
	sint16 var1 = 0, var2 = 0;	/* Two variables to store operands */

	LCD_init();	/*Initialize LCD */
	LCD_displayStringRowColumn(0,0,"M.T Diploma 51");
	LCD_displayStringRowColumn(1,0,"Enter operation:");
	LCD_goToRowColumn(2,0);

	/********************************************************************************
	 *                        APPLICATION	(SUPER LOOP)			        *
	 ********************************************************************************/

	while(1)
	{
		/* If any switch pressed for more than 200 ms it counts more than one press */
		/* get the pressed key number */
		key = KeyPad_getPressedKey();
		_delay_ms(200); 			   /* Press time */

		if( !( key >= 0 && key <= 9 ) )
		{
			/* Check if the key pressed is operator (+ , -, *, /, =) */
			switch(key)
			{
			case '+': flage_op = ADD;
				  operation = '+';
				  break;

			case '-': flage_op = SUB;
				  operation = '-';
				  break;

			case '*': flage_op = MUL;
				  operation = '*';
				  break;

			case '/': flage_op = DIV;
			          operation = '/';
			          break;

			case '=': flage_op = EQL;
			          break;
			}
		}

			if(flage_op == NO_OP)	/* Check if the key pressed is the first operand */
			{
				var1 = var1*10 + key;
				sprintf(buf,"%i",var1);
				LCD_displayStringRowColumn(2, 0, buf);  	/* Display the pressed keypad switch */
			}

			/* Check if the key pressed is the second operand */
			else if(flage_op == ADD || flage_op == SUB || flage_op == MUL || flage_op == DIV)
			{
				if( !(key >= 0 && key <= 9) )
				{
					sprintf(buf,"%i%c",var1,operation);
					LCD_displayStringRowColumn(2, 0, buf);  /* Display First operand and operator */
				}
				else
				{
					var2 = var2*10 + key;
					sprintf(buf,"%i%c%i", var1, operation, var2);
					/* Display First, Second operand and operator */
					LCD_displayStringRowColumn(2, 0, buf);
				}
			}
			/* Check if the Equal Button is pressed or not to display the result */
			else if(flage_op == EQL)
			{
				switch(operation)
				{
				case '+': result = (float)var1+(float)var2;
					  break;
				case '-': result = (float)var1-(float)var2;
					  break;
				case '*': result = (float)var1*(float)var2;
					  break;
				case '/': result =(float)var1/(float)var2;
					  break;
				}

				sprintf(buf, "=%.2f", result);
				LCD_displayString(buf);
				flage_op = NO_OP;
				key = var1 = var2 = operation = 0;
				while(KeyPad_getPressedKey() == '='){};	/* If pressed key is equal to '=' do nothing */
			}

			if(key == CLS)	/* Check if the 'C' Button is pressed or not,if it pressed clear the screen */
			{
				LCD_clearScreen();
				LCD_displayStringRowColumn(0,0,"M.T Diploma 51");
				LCD_displayStringRowColumn(1,0,"Enter operation:");
				LCD_goToRowColumn(2,0);
				flage_op = 0;
				key = var1 = var2 = operation = 0;
			}

			key = 0;	/* Clear the key for Next round */
	}
}
