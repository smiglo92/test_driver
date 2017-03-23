/*
 * init.h
 *
 *  Created on: 3 mar 2017
 *      Author: Mateusz
 */

#ifndef USERSOURCE_INIT_H_
#define USERSOURCE_INIT_H_

#include "stdint.h"
#include "stdbool.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/interrupt.h"
#include "driverlib/adc.h"
#include "driverlib/qei.h"
#include "driverlib/pin_map.h"

#include "mb_Motor.h"

/*initialize all peripherals used in project*/
void initAllPeripherals(void);



#endif /* USERSOURCE_INIT_H_ */
