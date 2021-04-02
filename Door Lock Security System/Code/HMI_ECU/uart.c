/******************************************************************************************
 *
 * [MODULE]: UART
 *
 * [FILE NAME]: uart.c
 *
 * [DESCRIPTION]: Source file for the UART AVR driver
 *
 * [AUTHOR]: Ahmed Hassan
 *
 ******************************************************************************************/

#include "uart.h"

/******************************************************************************************
 *                  		        Functions Definitions                                 *
 ******************************************************************************************/

/******************************************************************************************
 * [Function Name]: UART_init
 *
 * [Description]: 	Function to Initialize the UART Driver
 *					- Decide UART Mode (Normal, Double Speed)
 * 					- Decide Baud Rate of the UART Module (100 BPS, 200 BPS, 300 BPS, ..)
 * 					- Decide Number of Data Bits to be transmitted or Received (5,6,7,8 or 9)
 * 					- Decide UART Parity Type (Disable, Even, Odd)
 * 					- Decide Number of Stop Bits (One, Two)
 * 					- Enable Transmitter & Receiver of the UART Module
 *
 * [Args]:			Config_Ptr
 *
 * [in]			  	Config_Ptr: Pointer to UART Configuration Structure
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 ******************************************************************************************/
void UART_init(const UART_ConfigType * Config_Ptr)
{
	/* Insert the required Mode in U2X bit in UCSRA Register */
	UCSRA = (UCSRA & 0xFD) | ( (Config_Ptr -> uart_Mode) << 1);

	/* In Case of Nine Bit Data */
	if(Config_Ptr -> uart_DataBits == NINE_BITS)
	{
		/* Insert the last bit of required data bits in bit UCSZ2 in UCSRB Register */
		UCSRB = (UCSRB & 0xFB) | ( (Config_Ptr -> uart_DataBits) & 0x04);

		/* Two bits RXB8 & TXB8 must be Enabled in UCSRB Register */
		UCSRB |= (1 << RXB8) | (1 << TXB8);
	}

	/*
	 * Enable Transmitter & Receiver to be able to transmit and receive thought
	 * the UART driver
	 */
	UCSRB |= (1 << RXEN) | (1 << TXEN);

	/* URSEL must be one when writing the UCSRC */
	UCSRC |= (1 << URSEL);

	/*
	 * Insert the First two bits of required data bits in bits (UCSZ0, UCSZ1)
	 * in UCSRC Register in case of (5,6,7 or 8) Data bits
	 */
	UCSRC = (UCSRC & 0xF9) | ( (Config_Ptr -> uart_DataBits & 0x03) << 1);

	/* Insert the required parity type in bits (UPM0, UPM1) in UCSRC Register */
	UCSRC = (UCSRC & 0xCF) | ( (Config_Ptr -> uart_Parity) << 4);

	/* Insert the required number of stop bit(s) in bit USBS in UCSRC Register */
	UCSRC = (UCSRC & 0xF7) | ( (Config_Ptr -> uart_StopBit) << 3);

	/*
	 * This switch case is to choose between two Modes (Normal, Double Speed),
	 * then calculate and insert the required Baud rate in First 8 bits from the
	 * BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH
	 */
	switch(Config_Ptr -> uart_Mode)
	{
	case NORMAL_SPEED:	UBRRH = ( (((F_CPU / (Config_Ptr -> uart_BaudRate * 16UL))) - 1) >> 8 );
						UBRRL = ( ((F_CPU / (Config_Ptr -> uart_BaudRate * 16UL))) - 1 );
						break;

	case DOUBLE_SPEED:	UBRRH = ( (((F_CPU / (Config_Ptr -> uart_BaudRate * 8UL))) - 1) >> 8 );
						UBRRL = ( ((F_CPU / (Config_Ptr -> uart_BaudRate * 8UL))) - 1 );
						break;
	}
}

/***************************************************************************************
 * [Function Name]: UART_sendByte
 *
 * [Description]: 	Function responsible for sending 1 Byte through UART driver
 *
 * [Args]:			data
 *
 * [in]			  	data: Unsigned Character (Byte need to be sent through the UART)
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 ****************************************************************************************/
void UART_sendByte(const uint8 data)
{
	/*
	 * UDRE flag is set when the TX buffer (UDR) is empty and ready for
	 * transmitting a new byte so wait until this flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,UDRE)){}

	/*
	 * Put the required data in the UDR register and it also clear the UDRE
	 * flag as the UDR register is not empty now
	 */
	UDR = data;
}

/****************************************************************************************
 * [Function Name]: UART_recieveByte
 *
 * [Description]: 	Function responsible for receiving 1 Byte through UART driver
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	Unsigned Character
 *
 * [in/out]		 	None
 *
 * [Returns]:       The Byte that received through the UART
 ****************************************************************************************/
uint8 UART_recieveByte(void)
{
	/*
	 * RXC flag is set when the UART receive data so wait until this
	 * flag is set to one
	 */
	while(BIT_IS_CLEAR(UCSRA,RXC)){}

	/*
	 * Read the received data from the RX buffer (UDR) and the RXC flag
	 * will be cleared after read this data
	 */
	return UDR;
}

/****************************************************************************************
 * [Function Name]: UART_sendString
 *
 * [Description]: 	Function responsible for sending a string (more than 1 character)
 * 					through UART driver
 *
 * [Args]:			Str
 *
 * [in]				Str: Pointer to Unsigned Character (String need to be sent through the UART)
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 ****************************************************************************************/
void UART_sendString(const uint8 * Str)
{
	uint8 i = 0;

	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i]);
		i++;
	}
}

/****************************************************************************************
 * [Function Name]: UART_receiveString
 *
 * [Description]: 	Function responsible for receiving a string (more than 1 character)
 * 				  	until receiving a special character ('#') through UART driver
 *
 * [Args]:			Str
 *
 * [in]				Str: Pointer to Unsigned Character (String need to be received through the UART)
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 ****************************************************************************************/
void UART_receiveString(uint8 * Str)
{
	uint8 i = 0;

	Str[i] = UART_recieveByte();
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_recieveByte();
	}
	Str[i] = '\0';
}

/****************************************************************************************
 * [Function Name]: UART_setParityType
 *
 * [Description]: 	Function to set the required Parity Type
 *
 * [Args]:			a_parityType
 *
 * [in]				a_parityType: Enumerator to UART Parity Type
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 ****************************************************************************************/
void UART_setParityType(const UART_ParityType a_parityType)
{
	/* Insert the required parity type in bits (UPM0, UPM1) in UCSRC Register */
	UCSRC = (UCSRC & 0xCF) | (a_parityType << 4);
}

/****************************************************************************************
 * [Function Name]: UART_setStopBit
 *
 * [Description]: 	Function to set the required Stop bit(s)
 *
 * [Args]:			a_stopBit
 *
 * [in]				a_stopBit: Enumerator to UART Stop Bit
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 ****************************************************************************************/
void UART_setStopBit(const UART_StopBit a_stopBit)
{
	/* Insert the required number of stop bit(s) in bit USBS in UCSRC Register */
	UCSRC = (UCSRC & 0xF7) | ( a_stopBit << 3);
}

/****************************************************************************************
 * [Function Name]: UART_DeInit
 *
 * [Description]: 	Function to disable the UART Driver
 *
 * [Args]:			None
 *
 * [in]				None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 ****************************************************************************************/
void UART_DeInit(void)
{
	/* Reset All UART Registers to its initial value */
	UCSRA = 0x20;
	UCSRB = 0;
	UCSRC = 0x86;

	/* Clear UART Baud Rate Registers */
	UBRRH = 0;
	UBRRL = 0;
}
