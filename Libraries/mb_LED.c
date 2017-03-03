/*
 * mb_LED.c
 *
 *  Created on: 16 maj 2016
 *      Author: Mateusz
 */

/* LEDs:
 * E0 - - - - - - - - - - - LED1
 * F0 - - - - - - - - - - - LED2*/

#include "mb_LED.h"

static volatile const uint32_t mbLedBase [] = {GPIO_PORTE_BASE,
		                                       GPIO_PORTF_BASE};
static volatile const uint32_t mbLedPin [] = {GPIO_PIN_0, GPIO_PIN_0};


///////////////////////////////////////////////////////////////////////////////
void mb_LED_On(MbDiodaLed LED)
{
	/*GPIO pin in high state*/
	GPIOPinWrite(mbLedBase[LED], mbLedPin[LED], mbLedPin[LED]);
}


///////////////////////////////////////////////////////////////////////////////
void mb_LED_Off(MbDiodaLed LED)
{
	/*GPIO pin in low state*/
	GPIOPinWrite(mbLedBase[LED], mbLedPin[LED], 0);
}


///////////////////////////////////////////////////////////////////////////////
void mb_LED_Switch(MbDiodaLed LED)
{
	/*switch GPIO pin*/
	if(GPIOPinRead(mbLedBase[LED], mbLedPin[LED]))
		mb_LED_Off(LED);
	else
		mb_LED_On(LED);
}
