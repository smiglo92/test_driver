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
#include "driverlib/pin_map.h"

/*clock is 200MHz / 2,5 = 80MHz*/
void setMainClock(void);


/*enables GPIO, PWM and ADC*/
void enablePeripheralClocks(void);


/*before this PD7 and PF0 are locked*/
void unlockNmiPins(void);


/*set pins for motors enable and LEDs*/
void setGpioOutputs(void);


/*ADC measures input voltage*/
void initAdc(void);


/*PWM0 generate signals for MOTOR2*/
void initPwm0(void);


/*PWM1 generate signals for MOTOR1*/
void initPwm1(void);

#endif /* USERSOURCE_INIT_H_ */
