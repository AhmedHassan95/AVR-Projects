/******************************************************************************************
 *
 * [MODULE]: External EEPROM
 *
 * [FILE NAME]: external_eeprom.c
 *
 * [DESCRIPTION]: Source file for the External EEPROM Memory
 *
 * [AUTHOR]: Ahmed Hassan
 *
 ******************************************************************************************/

#include "i2c.h"
#include "external_eeprom.h"

/******************************************************************************************
 *                  		      Functions Definitions                                   *
 ******************************************************************************************/

/******************************************************************************************
 * [Function Name]: EEPROM_init
 *
 * [Description]: 	Function responsible for initializing EEPROM driver
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
 ******************************************************************************************/
void EEPROM_init(void)
{
	/* TWI Driver Configurations */
	TWI_ConfigType TWI_Config = {0x01,SCL_FREQ_400KBPS,PRESCALER_1};

	/* Just initialize the I2C(TWI) module inside the MC */
	TWI_init(&TWI_Config);
}

/******************************************************************************************
 * [Function Name]: EEPROM_writeByte
 *
 * [Description]: 	Function responsible for writing 1 Byte in the EEPROM
 *
 * [Args]:			u16addr, u8data
 *
 * [in]			  	u16addr: Unsigned Short (EEPROM Byte Address)
 * 					u8data:	 Unsigned Character (Data need to be written in the EEPROM)
 *
 * [out]		  	Unsigned Character
 *
 * [in/out]		 	None
 *
 * [Returns]:       Status of this function (SUCCESS, ERROR)
 ******************************************************************************************/
uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TW_START)
        return ERROR;
		
    /*
     * Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write)
     */
    TWI_write((uint8)(0xA0 | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR; 
		 
    /* Send the required memory location address */
    TWI_write((uint8)(u16addr));
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;
		
    /* Write byte to EEPROM */
    TWI_write(u8data);
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();
	
    return SUCCESS;
}

/******************************************************************************************
 * [Function Name]: EEPROM_readByte
 *
 * [Description]: 	Function responsible for reading 1 Byte from the EEPROM
 *
 * [Args]:			u16addr, u8data
 *
 * [in]			  	u16addr: Unsigned Short (Address where the data will be read from the EEPROM)
 * 					u8data: Pointer to Character (The data that will read from the EEPROM)
 *
 * [out]		  	Unsigned Character
 *
 * [in/out]		 	None
 *
 * [Returns]:       Status of this function (SUCCESS, ERROR)
 ******************************************************************************************/
uint8 EEPROM_readByte(uint16 u16addr, uint8 * u8data)
{
	/* Send the Start Bit */
    TWI_start();
    if (TWI_getStatus() != TW_START)
        return ERROR;
		
    /*
     * Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=0 (write)
     */
    TWI_write((uint8)((0xA0) | ((u16addr & 0x0700)>>7)));
    if (TWI_getStatus() != TW_MT_SLA_W_ACK)
        return ERROR;
		
    /* Send the required memory location address */
    TWI_write((uint8)(u16addr));
    if (TWI_getStatus() != TW_MT_DATA_ACK)
        return ERROR;
		
    /* Send the Repeated Start Bit */
    TWI_start();
    if (TWI_getStatus() != TW_REP_START)
        return ERROR;
		
    /*
     * Send the device address, we need to get A8 A9 A10 address bits from the
     * memory location address and R/W=1 (Read)
     */
    TWI_write((uint8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));
    if (TWI_getStatus() != TW_MT_SLA_R_ACK)
        return ERROR;

    /* Read Byte from Memory without send ACK */
    *u8data = TWI_readWithNACK();
    if (TWI_getStatus() != TW_MR_DATA_NACK)
        return ERROR;

    /* Send the Stop Bit */
    TWI_stop();
    return SUCCESS;
}
