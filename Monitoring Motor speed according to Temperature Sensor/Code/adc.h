 /******************************************************************************
 *
 * [MODULE]: ADC
 *
 * [FILE NAME]: adc.h
 *
 * [DESCRIPTION]: Header file for the ATmega16 ADC driver
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                           Preprocessor Macros                               *
 *******************************************************************************/
 
 #define ENABLE 1
 #define DISABLE 0
 /* ADC interrupt configuration */
 #define ACD_Interrupt DISABLE
 
/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/
 
#if (ACD_Interrupt == ENABLE)
/* Extern Public global variables to be used by other modules */
extern volatile uint16 g_adcResult;
#endif

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
/*******************************************************************************
 * [Function Name]: ADC_init
 *
 * [Description]:   Function responsible for initializing the ADC driver
 *
 * [Args]:	   None
 *
 * [in]		   None
 *
 * [out]	   None
 *
 * [in/out]	   None
 *
 * [Returns]:      None
 *******************************************************************************/
void ADC_init(void);

#if (ACD_Interrupt == DISABLE)
/*******************************************************************************
 * [Function Name]: ADC_readChannel
 *
 * [Description]:   Function responsible for reading analog data from a certain ADC channel
 *                  and convert it to digital using the ADC driver
 *
 * [Args]:	 channel_num
 *
 * [in]	       channel_num: Unsigned Character to store the channel number in it
 * 
 * [out]	Unsigned Character
 *
 * [in/out]	   None
 *
 * [Returns]:      The data converted by the ADC
 *******************************************************************************/
uint16 ADC_readChannel(uint8 channel_num);
#endif

#if (ACD_Interrupt == ENABLE)
/*******************************************************************************
 * [Function Name]: ADC_readChannel
 *
 * [Description]:   Function responsible for reading analog data from a certain ADC channel
 * 					            and convert it to digital using the ADC driver
 *
 * [Args]:	 channel_num
 *
 * [in]			  channel_num: Unsigned Character to store the channel number in it
 *
 * [out]		  None
 *
 * [in/out]	None
 *
 * [Returns]:    None
 *******************************************************************************/
void ADC_readChannel(uint8 channel_num);
#endif

#endif /* ADC_H_ */
