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
#include "driverlib/qei.h"
#include "driverlib/sysctl.h"

#include "mb_Motor.h"

/*variables to compute MOTOR1 zero current*/
extern volatile uint8_t isMeasureZeroCurrent;
extern volatile uint32_t zeroCurrentAdcTab[32];
extern volatile uint32_t zeroCurrentAdc;
extern volatile uint8_t zeroCurrentAdcIter;

/*variables with ADC measures*/
uint32_t adc0[1];
uint32_t adc1[2];

/*structures with motors parameters*/
extern MbMotorStruct motor1Struct, motor2Struct;

/*Input voltage: inputVoltage1 - before sign, inputVoltage2 - after sign*/
extern volatile uint32_t inputVoltage1;
extern volatile uint32_t inputVoltage2;


/*ADC0 measures current of MOTOR1 in handler*/
void ADC0IntHandler(void);


/*ADC1 measures input voltage and current of MOTOR2 in handler*/
void ADC1IntHandler(void);


/*PWM0 synchronizes with PWM1 in handler*/
void PWM0IntHandler(void);


/*PWM1 handler*/
void PWM1IntHandler(void);


#endif /* USERSOURCE_HANDLERS_H_ */
