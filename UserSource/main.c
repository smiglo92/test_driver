/*
 * main.c
 *
 *  Created on: 28 lut 2017
 *      Author: Mateusz
 */

#include "main.h"

int main(void)
{
	/*Global enable all interrupts*/
	IntMasterEnable();

	/*Init peripherals*/
	initAllPeripherals();

	/*Set PID parameters*/
	mb_PID_init();

    /*Enable motors*/
    mb_Motor_Enable(MOTOR1);
    mb_Motor_Enable(MOTOR2);

    /*Light LEDs*/
    mb_LED_On(LED1);
    mb_LED_On(LED2);

    motor1Struct.currentTargetFromCpu = 40;
    motor2Struct.currentTargetFromCpu = 15;

	/*----------------------------Main while loop----------------------------*/
	while (1) {

		/*PID regulator for MOTOR1*/
		if (isPid1Switch) {

			mb_PID_current_compute(MOTOR1);

			mb_Motor_Set_Pulse_Width(MOTOR1, motor1Struct.pwmInput);

			isPid1Switch = 0;
		}

		/*PID regulator for MOTOR2*/
		if (isPid2Switch) {

			mb_PID_current_compute(MOTOR2);

			mb_Motor_Set_Pulse_Width(MOTOR2, motor2Struct.pwmInput);

			isPid2Switch = 0;
		}
	}
}



