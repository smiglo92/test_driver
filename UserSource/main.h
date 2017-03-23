/*
 * main.h
 *
 *  Created on: 10 mar 2017
 *      Author: Mateusz
 */

#ifndef USERSOURCE_MAIN_H_
#define USERSOURCE_MAIN_H_

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/adc.h"

#include "mb_LED.h"
#include "mb_Motor.h"
#include "mb_PID.h"
#include "init.h"

MbMotorStruct motor1Struct, motor2Struct;

volatile uint8_t isPid1Switch;
volatile uint8_t isPid2Switch;

volatile uint32_t inputVoltage1;
volatile uint32_t inputVoltage2;

volatile uint8_t isMeasureZeroCurrent;
volatile uint32_t zeroCurrentAdcTab[32];
volatile uint32_t zeroCurrentAdc;
volatile uint8_t zeroCurrentAdcIter;


#endif /* USERSOURCE_MAIN_H_ */
