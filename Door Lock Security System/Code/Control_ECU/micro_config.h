 /******************************************************************************
 *
 * [MODULE]: Micro - Configuration
 *
 * [FILE NAME]: Micro_Config.h
 *
 * [DESCRIPTION]: File include all Microcontroller libraries
 *
 * [AUTHOR]: Ahmed Hassan
 *
 *******************************************************************************/

#ifndef MICRO_CONFIG_H_
#define MICRO_CONFIG_H_

#ifndef F_CPU
#define F_CPU 8000000UL /* 8 MHz Clock Frequency */
#endif  

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#endif /* MICRO_CONFIG_H_ */
