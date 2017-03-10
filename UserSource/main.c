/*
 * main.c
 *
 *  Created on: 28 lut 2017
 *      Author: Mateusz
 */

#include "main.h"

int main(void)
{
	setMainClock();

	enablePeripheralClocks();

	unlockNmiPins();

	setGpioOutputs();

	initAdc();

	zeroCurrentAdc = measureMotor1ZeroCurrent();

	initPwm0();

	initPwm1();

	initQEI0();

	initQEI1();

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



