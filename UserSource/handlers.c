/*
 * handlers.c
 *
 *  Created on: 3 mar 2017
 *      Author: Mateusz
 */

#include "handlers.h"

/*variables with ADC measures*/
uint32_t adc0[8];
uint32_t adc1[8];

/*Structures with motors parameters*/
extern MbMotorStruct motor1Struct, motor2Struct;

/*Input voltage: inputVoltage1 - before sign, inputVoltage2 - after sign*/
extern volatile uint32_t inputVoltage1;
extern volatile uint32_t inputVoltage2;

/*Variables controls PID frequency*/
extern volatile uint8_t isPid1Switch;
extern volatile uint8_t isPid2Switch;

/*Variables to compute MOTOR1 zero current*/
extern volatile uint8_t isMeasureZeroCurrent;
extern volatile uint32_t zeroCurrentAdcTab[32];
extern volatile uint32_t zeroCurrentAdc;
extern volatile uint8_t zeroCurrentAdcIter;

void ADC0IntHandler(void)
{
	uint8_t i;
	static int32_t currentMotor1[8];
	static uint8_t currentMotor1Iter;
	int32_t currentMotorAvg;

	while (!ADCIntStatus(ADC0_BASE, 0, false));
	ADCIntClear(ADC0_BASE, 0);

	ADCSequenceDataGet(ADC0_BASE, 0, adc0);

    if (isMeasureZeroCurrent) {
    	if(zeroCurrentAdcIter < 32) {
    		zeroCurrentAdcTab[zeroCurrentAdcIter] = adc0[0];
    		zeroCurrentAdcIter++;
    	}
    	else {
    		isMeasureZeroCurrent = 0;
    	}
    }
    else {
    	currentMotor1[currentMotor1Iter] = ((int32_t)(adc0[0]) -
    		                         (int32_t)zeroCurrentAdc) * 100000 / 14978;
    	currentMotor1Iter++;
    	if(currentMotor1Iter == 8)
    		currentMotor1Iter = 0;

    	currentMotorAvg = 0;
    	for(i = 0; i < 8; i++)
    		currentMotorAvg += currentMotor1[i];

    	motor1Struct.current = currentMotorAvg >> 3;

    	motor1Struct.direction = QEIDirectionGet(QEI0_BASE);

    	motor1Struct.position = QEIPositionGet(QEI0_BASE);

    	motor1Struct.velocity = QEIVelocityGet(QEI0_BASE) * 30 *
    		                    motor1Struct.direction;
    }
}

void ADC1IntHandler(void)
{
	int32_t common;

	while (!ADCIntStatus(ADC1_BASE, 0, false));
	ADCIntClear(ADC1_BASE, 0);

	ADCSequenceDataGet(ADC1_BASE, 0, adc1);

    common = (int32_t)(adc1[0]) * 1078605 / 4037670;

    if(common < 0)
    	common = 0;

    motor2Struct.direction = QEIDirectionGet(QEI1_BASE);

    motor2Struct.current = common * motor2Struct.direction;

    motor2Struct.position = QEIPositionGet(QEI1_BASE);

    motor2Struct.velocity = (int32_t)QEIVelocityGet(QEI1_BASE) * 30 *
    		                motor2Struct.direction;

    inputVoltage2 = (adc1[1]*182380/12467) % 1000;
    inputVoltage1 = adc1[1]*182380/12467000;
}

void PWM0IntHandler(void)
{
	PWMGenIntClear(PWM0_BASE, PWM_GEN_2, PWM_INT_CNT_LOAD);

	PWMSyncTimeBase(PWM1_BASE, PWM_GEN_0_BIT);

	isPid2Switch = 1;

	SysCtlDelay(3);
}

void PWM1IntHandler(void)
{
	PWMGenIntClear(PWM1_BASE, PWM_GEN_0, PWM_INT_CNT_LOAD);

	isPid1Switch = 1;

	SysCtlDelay(3);
}
