 /******************************************************************************
 *
 * [MODULE]: UART
 *
 * [FILE NAME]: uart.h
 *
 * [DESCRIPTION]: Header file for the UART AVR driver
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

 /******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/

typedef enum
{
	BPS_100 = 100, BPS_200 = 200, BPS_300 = 300, BPS_1200 = 1200, BPS_2400 = 2400, BPS_4800 = 4800,
	BPS_9600 = 9600, BPS_19200 = 19200, BPS_38400 = 38400, BPS_57600 = 57600, BPS_115200 = 115200
}UART_BaudRate;

typedef enum
{
	NORMAL_SPEED, DOUBLE_SPEED
}UART_ModeType;

typedef enum
{
	FIVE_BITS, SIX_BITS, SEVEN_BITS, EIGHT_BITS, NINE_BITS = 7
}UART_NumberOfDataBits;

typedef enum
{
	DISABLED = 0, EVEN = 2, ODD = 3
}UART_ParityType;

typedef enum
{
	ONE_BIT, TWO_BITS
}UART_StopBit;

typedef struct
{
	UART_BaudRate uart_BaudRate;
	UART_ModeType uart_Mode;
	UART_NumberOfDataBits uart_DataBits;
	UART_ParityType uart_Parity;
	UART_StopBit uart_StopBit;
}UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*******************************************************************************
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
 *******************************************************************************/
void UART_init(const UART_ConfigType * Config_Ptr);

/*******************************************************************************
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
 *******************************************************************************/
void UART_sendByte(const uint8 data);

/*******************************************************************************
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
 *******************************************************************************/
uint8 UART_recieveByte(void);

/*******************************************************************************
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
 *******************************************************************************/
void UART_sendString(const uint8 * Str);

/*******************************************************************************
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
 *******************************************************************************/
void UART_receiveString(uint8 * Str);

/*******************************************************************************
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
 *******************************************************************************/
void UART_setParityType(const UART_ParityType a_parityType);

/*******************************************************************************
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
 *******************************************************************************/
void UART_setStopBit(const UART_StopBit a_stopBit);

/*******************************************************************************
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
 *******************************************************************************/
void UART_DeInit(void);

#endif /* UART_H_ */
