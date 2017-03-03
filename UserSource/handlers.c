/*
 * handlers.c
 *
 *  Created on: 3 mar 2017
 *      Author: Mateusz
 */

#include "handlers.h"

void ADC0IntHandler(void)
{
	while (!ADCIntStatus(ADC0_BASE, 0, false));
	ADCIntClear(ADC0_BASE, 0);

	ADCSequenceDataGet(ADC0_BASE, 0, &adc0);
}

void PWM0IntHandler(void)
{
	PWMGenIntClear(PWM0_BASE, PWM_GEN_2, PWM_INT_CNT_LOAD);

	PWMSyncTimeBase(PWM1_BASE, PWM_GEN_0_BIT);

	SysCtlDelay(3);
}

void PWM1IntHandler(void)
{
	PWMGenIntClear(PWM1_BASE, PWM_GEN_0, PWM_INT_CNT_LOAD);

	SysCtlDelay(3);
}
