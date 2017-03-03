/*
 * main.c
 *
 *  Created on: 28 lut 2017
 *      Author: Mateusz
 */

#include "stdint.h"
#include "stdbool.h"
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
#include "init.h"


int main(void)
{

	setMainClock();

	enablePeripheralClocks();

	unlockNmiPins();

	setGpioOutputs();

	initAdc();

	initPwm0();

	initPwm1();

    IntMasterEnable();

	/*Enable motors*/
    mb_Motor_Enable(MOTOR1);
    mb_Motor_Enable(MOTOR2);

    /*Light LEDs*/
    mb_LED_On(LED1);
    mb_LED_On(LED2);

	while(1)
	{

	}
}



