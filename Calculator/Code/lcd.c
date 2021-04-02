 /******************************************************************************
 *
 * [MODULE]: LCD
 *
 * [FILE NAME]: lcd.c
 *
 * [DESCRIPTION]: Source file for the LCD driver
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#include "lcd.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void LCD_init(void)
{
	LCD_CTRL_PORT_DIR |= (1<<E) | (1<<RS) | (1<<RW); /* Configure the control pins(E,RS,RW) as output pins */
	
	#if (DATA_BITS_MODE == 4)
		#ifdef UPPER_PORT_PINS
			LCD_DATA_PORT_DIR |= 0xF0; /* Configure the highest 4 bits of the data port as output pins */
		#else
			LCD_DATA_PORT_DIR |= 0x0F; /* Configure the lowest 4 bits of the data port as output pins */
		#endif		 
		LCD_sendCommand(FOUR_BITS_DATA_MODE); /* Initialize LCD in 4-bit mode */
		LCD_sendCommand(TWO_LINE_LCD_Four_BIT_MODE); /* Use 2-line lcd + 4-bit Data Mode + 5*7 dot display Mode */
	#elif (DATA_BITS_MODE == 8)
		LCD_DATA_PORT_DIR = 0xFF; /* Configure the data port as output port */ 
		LCD_sendCommand(TWO_LINE_LCD_Eight_BIT_MODE); /* Use 2-line lcd + 8-bit Data Mode + 5*7 dot display Mode */
	#endif
	
	LCD_sendCommand(CURSOR_OFF); /* Cursor off */
	LCD_sendCommand(CLEAR_COMMAND); /* Clear LCD at the beginning */
}

void LCD_sendCommand(uint8 command)
{
	CLEAR_BIT(LCD_CTRL_PORT,RS); /* Instruction Mode RS=0 */
	CLEAR_BIT(LCD_CTRL_PORT,RW); /* Write data to LCD so RW=0 */
	_delay_ms(1); /* Delay for processing Tas = 50ns */
	SET_BIT(LCD_CTRL_PORT,E); /* Enable LCD E=1 */
	_delay_ms(1); /* Delay for processing Tpw - Tdws = 190ns */
#if (DATA_BITS_MODE == 4)
	/* Out the highest 4 bits of the required command to the data bus D4 --> D7 */
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (command & 0xF0);
#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | ((command & 0xF0) >> 4);
#endif

	_delay_ms(1); /* Delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CTRL_PORT,E); /* Disable LCD E=0 */
	_delay_ms(1); /* Delay for processing Th = 13ns */
	SET_BIT(LCD_CTRL_PORT,E); /* Enable LCD E=1 */
	_delay_ms(1); /* Delay for processing Tpw - Tdws = 190ns */

	/* Out the lowest 4 bits of the required command to the data bus D4 --> D7 */
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ((command & 0x0F) << 4);
#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | (command & 0x0F);
#endif

	_delay_ms(1); /* Delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CTRL_PORT,E); /* Disable LCD E=0 */
	_delay_ms(1); /* Delay for processing Th = 13ns */
#elif (DATA_BITS_MODE == 8)
	LCD_DATA_PORT = command; /* Out the required command to the data bus D0 --> D7 */
	_delay_ms(1); /* Delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CTRL_PORT,E); /* Disable LCD E=0 */
	_delay_ms(1); /* Delay for processing Th = 13ns */
#endif
}

void LCD_displayCharacter(uint8 data)
{
	SET_BIT(LCD_CTRL_PORT,RS); /* Data Mode RS=1 */
	CLEAR_BIT(LCD_CTRL_PORT,RW); /* Write data to LCD so RW=0 */
	_delay_ms(1); /* Delay for processing Tas = 50ns */
	SET_BIT(LCD_CTRL_PORT,E); /* Enable LCD E=1 */
	_delay_ms(1); /* Delay for processing Tpw - Tdws = 190ns */
#if (DATA_BITS_MODE == 4)
	/* Out the highest 4 bits of the required data to the data bus D4 --> D7 */
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data & 0xF0);
#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | ((data & 0xF0) >> 4);
#endif

	_delay_ms(1); /* Delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CTRL_PORT,E); /* Disable LCD E=0 */
	_delay_ms(1); /* Delay for processing Th = 13ns */
	SET_BIT(LCD_CTRL_PORT,E); /* Enable LCD E=1 */
	_delay_ms(1); /* Delay for processing Tpw - Tdws = 190ns */

	/* Out the lowest 4 bits of the required data to the data bus D4 --> D7 */
#ifdef UPPER_PORT_PINS
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | ((data & 0x0F) << 4);
#else
	LCD_DATA_PORT = (LCD_DATA_PORT & 0xF0) | (data & 0x0F);
#endif

	_delay_ms(1); /* Delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CTRL_PORT,E); /* Disable LCD E=0 */
	_delay_ms(1); /* Delay for processing Th = 13ns */
#elif (DATA_BITS_MODE == 8)
	LCD_DATA_PORT = data; /* Out the required command to the data bus D0 --> D7 */
	_delay_ms(1); /* Delay for processing Tdsw = 100ns */
	CLEAR_BIT(LCD_CTRL_PORT,E); /* Disable LCD E=0 */
	_delay_ms(1); /* Delay for processing Th = 13ns */
#endif
}

void LCD_displayString(const char *Str)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}
}

void LCD_goToRowColumn(uint8 row,uint8 col)
{
	uint8 Address;
	
	/* First of all calculate the required address */
	switch(row)
	{
		case 0:
			Address=col;
			break;
		case 1:
			Address=col+0x40;
			break;
		case 2:
			Address=col+0x10;
			break;
		case 3:
			Address=col+0x50;
			break;
	}					
	/* To write to a specific address in the LCD
	 * we need to apply the corresponding command 0b10000000+Address */
	LCD_sendCommand(Address | SET_CURSOR_LOCATION); 
}

void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	LCD_goToRowColumn(row,col); /* Go to to the required LCD position */
	LCD_displayString(Str); /* Display the string */
}

void LCD_intgerToString(int data)
{
   char buff[16]; /* String to hold the ASCII result */
   itoa(data,buff,10); /* 10 for decimal */
   LCD_displayString(buff);
}

void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_COMMAND); /* Clear display */
}
