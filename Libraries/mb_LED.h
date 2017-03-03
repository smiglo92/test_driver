/*
 * mb_LED.h
 *
 *  Created on: 16 maj 2016
 *      Author: Mateusz
 */

#ifndef MB_LED_H_
#define MB_LED_H_

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

typedef enum {
    LED1 = 0,
    LED2 = 1
}MbDiodaLed;


/* Switch on LED
 * arg:
 * 		- LED: LED1 or LED2*/
void mb_LED_On(MbDiodaLed LED);


/* Switch off LED
 * arg:
 * 		- LED: LED1 or LED2*/
void mb_LED_Off(MbDiodaLed LED);


/* Switch LED state
 * arg:
 * 		- LED: LED1 or LED2*/
void mb_LED_Switch(MbDiodaLed LED);

#endif /* MB_LED_H_ */
