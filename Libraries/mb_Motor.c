/*
 * mb_Motor.c
 *
 *  Created on: 16 maj 2016
 *      Author: Mateusz
 */

/* MOTOR1:
 * D0(M1PWM0), D1(M1PWM1) - PWMA, PWMB
 * D2 - - - - - - - - - - - ENABLE
 * E3(AIN0) - - - - - - - - CURRENT SENSOR
 * F3, C4 - - - - - - - - - ENDSTOPS(LEFT, RIGHT)
 * B3 - - - - - - - - - - - HOME SWITCH
 * D6(PhA0), D7(PhB0), F4 - ENCODER(A, B, I)
 * B0, F1 - - - - - - - - - WARNING SIGNALS(ACTIVE LOW)*/

/* MOTOR2:
 * E4(M0PWM4), E5(M0PWM5) - PWMA, PWMB
 * B1 - - - - - - - - - - - ENABLE
 * E2(AIN1) - - - - - - - - CURRENT SENSOR
 * A2, A4 - - - - - - - - - ENDSTOPS(LEFT, RIGHT)
 * A3 - - - - - - - - - - - HOME SWITCH
 * C5(PhA1), C6(PhB1), C7 - ENCODER(A, B, I)*/

/* COMMON:
 * A5, D3 - - - - - - - - - SAFETY INPUTS
 * E1(AIN2) - - - - - - - - INPUT VOLTAGE MEASURE*/


#include "mb_Motor.h"

static const uint32_t mbMotorGPIOEnablePort[] = {GPIO_PORTD_BASE,
		                                         GPIO_PORTB_BASE};
static const uint32_t mbMotorGPIOEnablePin[] = {GPIO_PIN_2, GPIO_PIN_1};
static const uint32_t mbMotorPWMBase[] = {PWM1_BASE, PWM0_BASE};
static const uint32_t mbMotorPWMOut1[] = {PWM_OUT_0, PWM_OUT_4};
static const uint32_t mbMotorPWMOut2[] = {PWM_OUT_1, PWM_OUT_5};
static const uint32_t mbMotorPWMOut[] = {(PWM_OUT_0_BIT | PWM_OUT_1_BIT),
		                                 (PWM_OUT_4_BIT | PWM_OUT_5_BIT)};

///////////////////////////////////////////////////////////////////////////////
void mb_Motor_Enable(MbMotor motor)
{
	//PWM output enable
	PWMOutputState(mbMotorPWMBase[motor], mbMotorPWMOut[motor], true);

	//ENABLE pin in high state
	GPIOPinWrite(mbMotorGPIOEnablePort[motor], mbMotorGPIOEnablePin[motor],
			mbMotorGPIOEnablePin[motor]);
}


///////////////////////////////////////////////////////////////////////////////
void mb_Motor_Disable(MbMotor motor)
{
	//PWM output disable
	PWMOutputState(mbMotorPWMBase[motor], mbMotorPWMOut[motor], false);

	//ENABLE pin in low state
	GPIOPinWrite(mbMotorGPIOEnablePort[motor], mbMotorGPIOEnablePin[motor], 0);
}


///////////////////////////////////////////////////////////////////////////////
void mb_Motor_Set_Pulse_Width(MbMotor motor, int16_t width)
{
	//if width is negative, set PWM signal at first output
	if(width < 0)
	{
		PWMPulseWidthSet(mbMotorPWMBase[motor], mbMotorPWMOut1[motor], -width);
		PWMPulseWidthSet(mbMotorPWMBase[motor], mbMotorPWMOut2[motor], 1);
	}

	//if width is positive, set PWM signal at second output
	else if(width > 0)
	{
		PWMPulseWidthSet(mbMotorPWMBase[motor], mbMotorPWMOut1[motor], 1);
		PWMPulseWidthSet(mbMotorPWMBase[motor], mbMotorPWMOut2[motor], width);
	}

	//if width is equal zero, set low state signal at both outputs
	else
	{
		PWMPulseWidthSet(mbMotorPWMBase[motor], mbMotorPWMOut1[motor], 1);
		PWMPulseWidthSet(mbMotorPWMBase[motor], mbMotorPWMOut2[motor], 1);
	}
}

///////////////////////////////////////////////////////////////////////////////
uint8_t mb_Motor_Synchro_HomeSwitch(MbMotor motor)
{
	uint8_t state;
	if(motor == MOTOR1)
		state = (uint8_t)GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_3);
	else
		state = (uint8_t)GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_3);
	if(state)
		state = 1;
	return state;
}

///////////////////////////////////////////////////////////////////////////////
uint32_t measureMotor1ZeroCurrent(void)
{
	uint8_t i;
	uint32_t result = 0;

	isMeasureZeroCurrent = 1;

	while (zeroCurrentAdcIter < 32);

	isMeasureZeroCurrent = 0;

	for (i = 0; i < 32; i++)
	{
		result += zeroCurrentAdcTab[i];
	}

	return (result >> 5);
}

