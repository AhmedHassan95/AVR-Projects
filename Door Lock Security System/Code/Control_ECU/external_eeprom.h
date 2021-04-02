  /******************************************************************************
 *
 * [MODULE]: External EEPROM
 *
 * [FILE NAME]: external_eeprom.h
 *
 * [DESCRIPTION]: Header file for the External EEPROM Memory
 *
 * [AUTHOR]: Ahmed Hassan
 *
 ********************************************************************************/
 
#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "std_types.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
 
#define ERROR 0
#define SUCCESS 1

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*******************************************************************************
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
 *******************************************************************************/
void EEPROM_init(void);

/*******************************************************************************
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
 *******************************************************************************/
uint8 EEPROM_writeByte(uint16 u16addr, uint8 u8data);

/*******************************************************************************
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
 *******************************************************************************/
uint8 EEPROM_readByte(uint16 u16addr,uint8 * u8data);
 
#endif /* EXTERNAL_EEPROM_H_ */
