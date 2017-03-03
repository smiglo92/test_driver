/*
 * handlers.h
 *
 *  Created on: 3 mar 2017
 *      Author: Mateusz
 */

#ifndef USERSOURCE_HANDLERS_H_
#define USERSOURCE_HANDLERS_H_

#include "stdint.h"
#include "stdbool.h"
#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "driverlib/adc.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"

/*variable for measure analog voltage*/
uint32_t adc0;


/*ADC0 measures input voltage in handler*/
void ADC0IntHandler(void);


/*PWM0 synchronizes with PWM1 in handler*/
void PWM0IntHandler(void);


/*PWM1 handler*/
void PWM1IntHandler(void);


#endif /* USERSOURCE_HANDLERS_H_ */
