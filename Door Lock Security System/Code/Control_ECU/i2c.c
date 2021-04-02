/********************************************************************************************
 *
 * [MODULE]: I2C(TWI)
 *
 * [FILE NAME]: i2c.c
 *
 * [DESCRIPTION]: Source file for the I2C (TWI) AVR driver
 *
 * [AUTHOR]: Ahmed Hassan
 *
 ********************************************************************************************/

#include "i2c.h"

/********************************************************************************************
 *                                    Functions Prototypes                                  *
 ********************************************************************************************/

/********************************************************************************************
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
 ********************************************************************************************/
void TWI_init(const TWI_ConfigType * Config_Ptr)
{
	/* Insert the required Pre-scaler in First two bits (TWPS1,TWPS0) in TWSR Register */
	TWSR = (TWSR & 0xFC) | (Config_Ptr -> twi_Prescaler);

	/* Insert the required Bit Rate in TWBR Register */
	TWBR = ( ( (F_CPU/Config_Ptr -> twi_Clock) -16) /2 ) / ( 4 << (Config_Ptr -> twi_Prescaler) );

    /*
     * Insert the required Two Wire Bus address, if any master device want to call MCU
     * (used in case this MC is a slave device), General Call Recognition: Off
     */
    TWAR = ( (Config_Ptr -> twi_Address) << 1 ); /* Shift left to use as address in 7 bits only */
	
    TWCR = (1 << TWEN); /* Enable TWI */
}

/********************************************************************************************
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
 ********************************************************************************************/
void TWI_start(void)
{
    /* 
	 * Clear the TWINT flag before sending the start bit TWINT = 1
	 * send the start bit by TWSTA = 1
	 * Enable TWI Module TWEN = 1 
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

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
void TWI_stop(void)
{
    /* 
	 * Clear the TWINT flag before sending the stop bit TWINT = 1
	 * send the stop bit by TWSTO = 1
	 * Enable TWI Module TWEN = 1 
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

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
void TWI_write(uint8 data)
{
    /* Put data On TWI data Register */
    TWDR = data;

    /* 
	 * Clear the TWINT flag before sending the data TWINT = 1
	 * Enable TWI Module TWEN = 1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN);

    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

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
uint8 TWI_readWithACK(void)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT =  1
	 * Enable sending ACK after reading or receiving data TWEA = 1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
	
    /* Read Data */
    return TWDR;
}

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
uint8 TWI_readWithNACK(void)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT = 1
	 * Enable TWI Module TWEN = 1 
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
	
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
	
    /* Read Data */
    return TWDR;
}

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
uint8 TWI_getStatus(void)
{
    uint8 status;
    /* Masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;
    return status;
}

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
void TWI_DeInit(void)
{
	/* Reset All TWI Registers to its initial value (if they have), and Clear the others */
	TWBR = 0;
	TWSR = 0xF8;
	TWAR = 0xFE;

	/* Disable TWI */
	TWCR &= ~(1 << TWEN);
}
