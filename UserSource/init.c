/*
 * init.c
 *
 *  Created on: 3 mar 2017
 *      Author: Mateusz
 */

#include "init.h"

void setMainClock(void)
{
	/*set clock to 80MHz*/
	SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
	               SYSCTL_XTAL_16MHZ);

	/*set PWM clock to 80MHz*/
	SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
}

void enablePeripheralClocks(void)
{
	/*enable GPIO ports*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	/*enable ADC*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);

	/*enable PWM*/
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	SysCtlDelay(3);
}

void unlockNmiPins(void)
{
	/*Unlock PD7*/
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTD_BASE + GPIO_O_CR) |= 0x80;
	HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = 0;

	/*Unlock PF0*/
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) = 0x1;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
}

void setGpioOutputs(void)
{
	/*PB1 - pin enable MOTOR2*/
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_1);

    /*PD2 - pin enable MOTOR1*/
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_2);

    /*PE0 - LED1*/
	GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0);

	/*PF0 - LED2*/
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_0);
}

void initAdc(void)
{
	/*PE1 as analog input*/
	GPIOPinTypeADC(GPIO_PORTE_BASE, GPIO_PIN_1);

	/*input voltage is triggered always*/
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_ALWAYS, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_CH2 | ADC_CTL_IE |
			                 ADC_CTL_END);
	ADCHardwareOversampleConfigure(ADC0_BASE, 32);
	ADCSequenceEnable(ADC0_BASE, 0);

	/*ADC0 interrupt enable*/
	ADCIntClear(ADC0_BASE, 0);
	IntEnable(INT_ADC0SS0);
	ADCIntEnable(ADC0_BASE, 0);
}

void initPwm0(void)
{
	/*PE4 and PE5 as PWM pins*/
	GPIOPinConfigure(GPIO_PE4_M0PWM4);
	GPIOPinConfigure(GPIO_PE5_M0PWM5);
	GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_4);
	GPIOPinTypePWM(GPIO_PORTE_BASE, GPIO_PIN_5);

	/*PWM0 frequency - 80MHz / 4000 = 20kHz*/
    PWMGenConfigure(PWM0_BASE, PWM_GEN_2,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_2, 4000);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_4, 1);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_5, 150);

    /*PWM0 interrupt enable*/
    PWMIntEnable(PWM0_BASE, PWM_INT_GEN_2);
    PWMGenIntTrigEnable(PWM0_BASE, PWM_GEN_2, PWM_INT_CNT_LOAD);
    IntEnable(INT_PWM0_2);

    /*enable PWM0 outputs*/
    PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT, true);
    PWMOutputState(PWM0_BASE, PWM_OUT_5_BIT, true);
    PWMGenEnable(PWM0_BASE, PWM_GEN_2);
}

void initPwm1(void)
{
	/*PD0 and PD1 as PWM pins*/
	GPIOPinConfigure(GPIO_PD0_M1PWM0);
	GPIOPinConfigure(GPIO_PD1_M1PWM1);
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinTypePWM(GPIO_PORTD_BASE, GPIO_PIN_1);

	/*PWM1 frequency - 80MHz / 4000 = 20kHz*/
    PWMGenConfigure(PWM1_BASE, PWM_GEN_0,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_0, 4000);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_0, 1);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_1, 100);

    /*PWM1 interrupt enable*/
    PWMIntEnable(PWM1_BASE, PWM_INT_GEN_0);
    PWMGenIntTrigEnable(PWM1_BASE, PWM_GEN_0, PWM_INT_CNT_LOAD);
    IntEnable(INT_PWM1_0);

    /*enable PWM1 outputs*/
    PWMOutputState(PWM1_BASE, PWM_OUT_0_BIT, true);
    PWMOutputState(PWM1_BASE, PWM_OUT_1_BIT, true);
    PWMGenEnable(PWM1_BASE, PWM_GEN_0);
}
