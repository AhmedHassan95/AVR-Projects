 /******************************************************************************
 *
 * [MODULE]: BUZZER
 *
 * [FILE NAME]: buzzer.h
 *
 * [DESCRIPTION]: Header file for the BUZZER Driver
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"

/*******************************************************************************
 *                             Preprocessor Macros                             *
 *******************************************************************************/

/* BUZZER Pin Configurations */
#define BUZZER_PORT_DIR 	   DDRA
#define BUZZER_PORT_OUT	   	   PORTA
#define BUZZER_PIN 	  	       PA0

/*******************************************************************************
 *                             Functions Prototypes                            *
 *******************************************************************************/

/*******************************************************************************
 * [Function Name]: BUZZER_init
 *
 * [Description]: 	Function to Initialize the BUZZZER Driver
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
void BUZZER_init(void);

/*******************************************************************************
 * [Function Name]: BUZZER_turnOn
 *
 * [Description]: 	Function to turn on the BUZZZER
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
void BUZZER_turnOn(void);

/*******************************************************************************
 * [Function Name]: BUZZER_turnOff
 *
 * [Description]: 	Function to turn off the BUZZZER
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
void BUZZER_turnOff(void);

#endif /* BUZZER_H_ */
