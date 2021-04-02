 /******************************************************************************
 *
 * [MODULE]: ADC
 *
 * [FILE NAME]: adc.c
 *
 * [DESCRIPTION]: Source file for the ATmega16 ADC driver.
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#include "adc.h"

/*******************************************************************************
 *                          Global Variables                                   *
 *******************************************************************************/
 
#if (ACD_Interrupt == ENABLE)
volatile uint16 g_adcResult = 0;
#endif

/*******************************************************************************
 *                          ISR's Definitions                                  *
 *******************************************************************************/

#if (ACD_Interrupt == ENABLE)
ISR(ADC_vect)
{
	/* Read ADC Data after conversion complete */
	g_adcResult = ADC;
}
#endif

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/
/*******************************************************************************
 * [Function Name]: ADC_init
 *
 * [Description]:   Function responsible for initializing the ADC driver
 *
 * [Args]:	    None
 *
 * [in]		    None
 *
 * [out]	    None
 *
 * [in/out]	    None
 *
 * [Returns]:       None
 *******************************************************************************/
void ADC_init(void)
{
	/* 
	 * ADMUX Register Bits Description:
	 * REFS1:0 = 00 to choose to connect external reference voltage by input this voltage through AREF pin
	 * ADLAR   = 0 right adjusted
	 * MUX4:0  = 00000 to choose channel 0 as initialization
	 */
	ADMUX = 0;

	/* 
	 * ADCSRA Register Bits Description:
	 * ADEN    = 1 Enable ADC
	 * ADIE    = 1 Enable ADC Interrupt
	 * ADPS2:0 = 011 to choose ADC_Clock=F_CPU/8=1Mhz/8=125Khz --> ADC must operate in range 50-200Khz
	 */
	 
	#if (ACD_Interrupt == ENABLE)
	ADCSRA = (1<<ADEN) | (1<<ADIE) | (1<<ADPS1) | (1<<ADPS0);
	#endif
	
	/* 
	 * ADCSRA Register Bits Description:
	 * ADEN    = 1 Enable ADC
	 * ADIE    = 0 Disable ADC Interrupt
	 * ADPS2:0 = 011 to choose ADC_Clock=F_CPU/8=1Mhz/8=125Khz --> ADC must operate in range 50-200Khz
	 */
	 
	#if (ACD_Interrupt == DISABLE)
	ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);
	#endif
}

#if (ACD_Interrupt == ENABLE)
/*******************************************************************************
 * [Function Name]: ADC_readChannel
 *
 * [Description]:   Function responsible for reading analog data from a certain ADC channel
 * 		    and convert it to digital using the ADC driver
 *
 * [Args]:	   channel_num
 *
 * [in]		   channel_num: Unsigned Character to store the channel number in it
 *
 * [out]	    None
 *
 * [in/out]	    None
 *
 * [Returns]:       None
 *******************************************************************************/
void ADC_readChannel(uint8 channel_num)
{
	channel_num &= 0x07; /* Channel number must be from 0 --> 7 */
	ADMUX &= 0xE0; /* Clear first 5 bits in the ADMUX (channel number MUX4:0 bits) before set the required channel */
	ADMUX = ADMUX | channel_num; /* Choose the correct channel by setting the channel number in MUX4:0 bits */
	SET_BIT(ADCSRA,ADSC); /* Start conversion write '1' to ADSC */
}
#endif


#if (ACD_Interrupt == DISABLE)
/*******************************************************************************
 * [Function Name]: ADC_readChannel
 *
 * [Description]:   Function responsible for reading analog data from a certain ADC channel
 * 		    and convert it to digital using the ADC driver
 *
 * [Args]:	    channel_num
 *
 * [in]		    channel_num: Unsigned Character to store the channel number in it
 *
 * [out]	    Unsigned Character
 *
 * [in/out]	    None
 *
 * [Returns]:       The data converted by the ADC
 *******************************************************************************/
uint16 ADC_readChannel(uint8 channel_num)
{
	channel_num &= 0x07; /* Channel number must be from 0 --> 7 */
	ADMUX &= 0xE0; /* Clear first 5 bits in the ADMUX (channel number MUX4:0 bits) before set the required channel */
	ADMUX = ADMUX | channel_num; /* Choose the correct channel by setting the channel number in MUX4:0 bits */
	SET_BIT(ADCSRA,ADSC); /* Start conversion write '1' to ADSC */
	while(BIT_IS_CLEAR(ADCSRA,ADIF)); /* Wait for conversion to complete ADIF becomes '1' */
	SET_BIT(ADCSRA,ADIF); /* Clear ADIF by write '1' to it :) */
	return ADC; /* Return the data register */
}
#endif
