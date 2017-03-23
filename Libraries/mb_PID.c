/*
 * mb_PID.c
 *
 *  Created on: 5 sty 2017
 *      Author: Mateusz
 */

#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"

#include "mb_Motor.h"
#include "mb_PID.h"
#include "user_defines.h"

extern MbMotorStruct motor1Struct, motor2Struct;

static MbMotorStruct* const mbPidMotorStruct [] = {&motor1Struct,
		                                           &motor2Struct};

static const uint16_t mbPidMaxForwardPwm [] = {MOTOR1_MAX_FORWARD_PWM,
		                                       MOTOR2_MAX_FORWARD_PWM};
static const uint16_t mbPidMaxBackwardPwm [] = {MOTOR1_MAX_BACKWARD_PWM,
		                                        MOTOR2_MAX_BACKWARD_PWM};
static const uint16_t mbPidMaxForwardPwmChange [] =
		{MOTOR1_MAX_FORWARD_PWM_CHANGE, MOTOR2_MAX_FORWARD_PWM_CHANGE};
static const uint16_t mbPidMaxBackwardPwmChange [] =
		{MOTOR1_MAX_BACKWARD_PWM_CHANGE, MOTOR2_MAX_BACKWARD_PWM_CHANGE};

///////////////////////////////////////////////////////////////////////////////
void mb_PID_init(void)
{
	/*Current PID for MOTOR1 parameters*/
	motor1Struct.currentPid.kp = MOTOR1_PID_CURRENT_KP;
	motor1Struct.currentPid.ti = MOTOR1_PID_CURRENT_TI;
	motor1Struct.currentPid.td = MOTOR1_PID_CURRENT_TD;
	motor1Struct.currentPid.tp = MOTOR1_PID_CURRENT_TP;

	motor1Struct.currentPid.a[0] = motor1Struct.currentPid.kp +
			motor1Struct.currentPid.tp * motor1Struct.currentPid.kp /
			2 / motor1Struct.currentPid.ti + motor1Struct.currentPid.td *
			motor1Struct.currentPid.kp / motor1Struct.currentPid.tp;

	motor1Struct.currentPid.a[1] = - motor1Struct.currentPid.kp +
			motor1Struct.currentPid.tp * motor1Struct.currentPid.kp /
			2 / motor1Struct.currentPid.ti - 2 * motor1Struct.currentPid.td *
			motor1Struct.currentPid.kp / motor1Struct.currentPid.tp;

	motor1Struct.currentPid.a[2] = motor1Struct.currentPid.td *
			motor1Struct.currentPid.kp / motor1Struct.currentPid.tp;

	/*Current PID for MOTOR2 parameters*/
	motor2Struct.currentPid.kp = MOTOR2_PID_CURRENT_KP;
	motor2Struct.currentPid.ti = MOTOR2_PID_CURRENT_TI;
	motor2Struct.currentPid.td = MOTOR2_PID_CURRENT_TD;
	motor2Struct.currentPid.tp = MOTOR2_PID_CURRENT_TP;

	motor2Struct.currentPid.a[0] = motor2Struct.currentPid.kp +
			motor2Struct.currentPid.tp * motor2Struct.currentPid.kp /
			2 / motor2Struct.currentPid.ti + motor2Struct.currentPid.td *
			motor2Struct.currentPid.kp / motor2Struct.currentPid.tp;

	motor2Struct.currentPid.a[1] = - motor2Struct.currentPid.kp +
			motor2Struct.currentPid.tp * motor2Struct.currentPid.kp /
			2 / motor2Struct.currentPid.ti - 2 * motor2Struct.currentPid.td *
			motor2Struct.currentPid.kp / motor2Struct.currentPid.tp;

	motor2Struct.currentPid.a[2] = motor2Struct.currentPid.td *
			motor2Struct.currentPid.kp / motor2Struct.currentPid.tp;
}


int32_t mb_PID_compute_output_change(MbMotorPid* pid)
{
	int32_t change;

	change = pid->a[0] * pid->e[0];
	change += pid->a[1] * pid->e[1];
	change += pid->a[2] * pid->e[2];

	change = change >> 16;

	return change;
}

void mb_PID_update_error(MbMotorStruct* motorStruct)
{
	/*Error is difference between target and measured value multiply by 1024*/
	motorStruct->currentError = (motorStruct->currentTargetFromCpu -
			                     motorStruct->current) << 10;
	motorStruct->currentPid.e[2] = motorStruct->currentPid.e[1];
	motorStruct->currentPid.e[1] = motorStruct->currentPid.e[0];
	motorStruct->currentPid.e[0] = motorStruct->currentError;
}


int32_t mb_PID_compute_pwm_change(MbMotor motor, int32_t outputChange)
{
	int32_t pwmChange;

	if (outputChange > mbPidMaxForwardPwmChange[motor]) {
		pwmChange = mbPidMaxForwardPwmChange[motor];
	}
	else if (outputChange < - mbPidMaxBackwardPwmChange[motor]) {
		pwmChange = - mbPidMaxBackwardPwmChange[motor];
	}
	else {
		pwmChange = outputChange;
	}

	return pwmChange;
}


int32_t mb_PID_compute_pwm(MbMotor motor, int32_t output)
{
	int32_t pwm;

	if (output > mbPidMaxForwardPwm[motor]) {
		pwm = mbPidMaxForwardPwm[motor];
	}
	else if (output < - mbPidMaxBackwardPwm[motor]) {
		pwm = - mbPidMaxBackwardPwm[motor];
	}
	else {
		pwm = output;
	}

	return pwm;
}

///////////////////////////////////////////////////////////////////////////////
void mb_PID_current_compute(MbMotor motor)
{
	/*Used variables definitions*/
	MbMotorStruct* motorStruct = mbPidMotorStruct[motor];
	int32_t outputChange;
	int32_t outputChangeLim;
	int32_t v;

	mb_PID_update_error(motorStruct);

	outputChange = mb_PID_compute_output_change(&(motorStruct->currentPid));

	motorStruct->pwmInputChange = outputChange;

	outputChangeLim = mb_PID_compute_pwm_change(motor, outputChange);

	/*Previous values of PWM after limit (pwmInputPrevious) and before limit*/
	motorStruct->pwmInputPrevious = motorStruct->pwmInput;
	motorStruct->pwmInputUnlimitedPrevious = motorStruct->pwmInputUnlimited;

	v = outputChangeLim + motorStruct->pwmInputPrevious -
			motorStruct->pwmInputUnlimitedPrevious;

	motorStruct->pwmInputUnlimited = motorStruct->pwmInputUnlimitedPrevious +
			                         v;

	motorStruct->pwmInput = mb_PID_compute_pwm(motor,
			                                   motorStruct->pwmInputUnlimited);
}
