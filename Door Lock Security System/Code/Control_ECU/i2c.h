 /****************************************************************************************
 *
 * [MODULE]: I2C(TWI)
 *
 * [FILE NAME]: i2c.h
 *
 * [DESCRIPTION]: Header file for the I2C (TWI) AVR driver
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *****************************************************************************************/
 
#ifndef I2C_H_
#define I2C_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*****************************************************************************************
 *                              Preprocessor Macros                                      *
 *****************************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TW_START         0x08 /* Start has been sent */
#define TW_REP_START     0x10 /* Repeated start */
#define TW_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + Ack received from slave */
#define TW_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + Ack received from slave */
#define TW_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave */
#define TW_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave */
#define TW_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave */

 /****************************************************************************************
 *                                 Types Declaration                                     *
 *****************************************************************************************/

typedef enum
{
	PRESCALER_1, PRESCALER_4, PRESCALER_16, PRESCALER_64
}TWI_Prescaler;

typedef enum
{
	SCL_FREQ_100KBPS = 100000, SCL_FREQ_400KBPS = 400000, SCL_FREQ_1MBPS = 1000000
}TWI_ClockFrequency;

typedef struct
{
	uint8 twi_Address;
	TWI_ClockFrequency twi_Clock;
	TWI_Prescaler twi_Prescaler;
}TWI_ConfigType;

/*****************************************************************************************
 *                               Functions Prototypes                                    *
 *****************************************************************************************/

/*****************************************************************************************
 * [Function Name]: TWI_init
 *
 * [Description]: 	Function to Initialize the TWI (I2C) Driver
 *					- Decide Bit Rate of the TWI (100k BPS, 400k BPS, 1M BPS)
 * 					- Decide Pre-scaler of the TWI (1, 4, 16, 64)
 * 					- Decide the Address of MCU (If any device need to contact with it)
 * 					- Enable TWI Module
 *
 * [Args]:			Config_Ptr
 *
 * [in]			  	Config_Ptr: Pointer to TWI Configuration Structure
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 *****************************************************************************************/
void TWI_init(const TWI_ConfigType * Config_Ptr);

/*****************************************************************************************
 * [Function Name]: TWI_start
 *
 * [Description]: 	Function responsible for sending start bit successfully in TWI Bus
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 *****************************************************************************************/
void TWI_start(void);

/*****************************************************************************************
 * [Function Name]: TWI_stop
 *
 * [Description]: 	Function responsible for sending stop bit successfully in TWI Bus
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 *****************************************************************************************/
void TWI_stop(void);

/*****************************************************************************************
 * [Function Name]: TWI_write
 *
 * [Description]: 	Function responsible for writing 1 Byte in the TWI Bus
 *
 * [Args]:			data
 *
 * [in]			  	data: Unsigned Character (data need to be written in the TWI)
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 *****************************************************************************************/
void TWI_write(uint8 data);

/*****************************************************************************************
 * [Function Name]: TWI_readWithACK
 *
 * [Description]: 	Function responsible for reading 1 Byte (with ACK) from the TWI Bus
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	Unsigned Character
 *
 * [in/out]		 	None
 *
 * [Returns]:       Data read (with ACK) from the TWI Bus
 *****************************************************************************************/
uint8 TWI_readWithACK(void);

/*****************************************************************************************
 * [Function Name]: TWI_readWithNACK
 *
 * [Description]: 	Function responsible for reading 1 Byte (without ACK) from the TWI Bus
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	Unsigned Character
 *
 * [in/out]		 	None
 *
 * [Returns]:       Data read (without ACK) from the TWI Bus
 *****************************************************************************************/
uint8 TWI_readWithNACK(void);

/*****************************************************************************************
 * [Function Name]: TWI_getStatus
 *
 * [Description]: 	Function responsible for getting status of the TWI Bus
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	Unsigned Character
 *
 * [in/out]		 	None
 *
 * [Returns]:       Status of the TWI Bus
 *****************************************************************************************/
uint8 TWI_getStatus(void);

/*****************************************************************************************
 * [Function Name]: TWI_DeInit
 *
 * [Description]: 	Function to disable the TWI Driver
 *
 * [Args]:			None
 *
 * [in]			  	None
 *
 * [out]		  	None
 *
 * [in/out]		 	None
 *
 * [Returns]:       None
 *****************************************************************************************/
void TWI_DeInit(void);

#endif /* I2C_H_ */
