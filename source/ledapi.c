/*
 * ledapi.c
 *
 *  Created on: Apr 4, 2021
 *      Author: veli-
 */

#include "ledapi.h"
#include "gpio.h"
#include "timer.h"

static uint16_t ledGreenTimer = 0;
static uint16_t ledBlueTimer = 0;
static LedState ledGreenState = 1;
static LedState ledBlueState = 1;

static void led_changeStateBlue(LedState aState);
static void led_changeStateGreen(LedState aState);


void led_timer0_ocra(void)
{
	ledGreenTimer++;
	ledBlueTimer++;
	led_changeStateBlue(ledBlueState);
	led_changeStateGreen(ledGreenState);
	timer_clear(TIMER_0);
}

void led_init(void)
{
	// CONFIGURE PIN DIRECTIONS
	gpio_setDir(PORT_F, PIN_4, OUT); // User LED-1 - Green
	gpio_setDir(PORT_F, PIN_5, OUT); // User LED-2 - Blue

	timer_configure(TIMER_0, (TimerWaveformType)TIMER_0_NORMAL);
	timer_ocr_configure(TIMER_0, OCR0_A, NORMAL);
	timer_set_ocr_value(TIMER_0, OCR0_A, 250);
	timer_setCallbackTimer0OCRAMatch(led_timer0_ocra);

	timer_start(TIMER_0, CLK_IO_DIV_BY_1024);
}

void led_changeState(LedType aType, LedState aState)
{
	if (eGREEN == aType)
	{
		ledGreenState = aState;
	}
	else if (eBLUE == aType)
	{
		ledBlueState = aState;
	}
	else
	{

	}
}

static void led_changeStateGreen(LedState aState)
{
	switch(aState)
	{
	case eLEDON:
		gpio_setValue(PORT_F, PIN_4);
		ledGreenTimer = 0;
		break;
	case eLEDOFF:
		gpio_clearValue(PORT_F, PIN_4);
		ledGreenTimer = 0;
		break;
	case eLEDBLINK1000MS:
		if (ledGreenTimer <= 64)
		{
			gpio_setValue(PORT_F, PIN_4);
		}
		else if ((ledGreenTimer > 64) && (ledGreenTimer < 128))
		{
			gpio_clearValue(PORT_F, PIN_4);
		}
		else
		{
			ledGreenTimer = 0;
		}
		break;
	case eLEDBLINK500MS:
		if (ledGreenTimer <= 32)
		{
			gpio_setValue(PORT_F, PIN_4);
		}
		else if ((ledGreenTimer > 32) && (ledGreenTimer < 64))
		{
			gpio_clearValue(PORT_F, PIN_4);
		}
		else
		{
			ledGreenTimer = 0;
		}
		break;
	case eLEDBLINK250MS:
		if (ledGreenTimer <= 16)
		{
			gpio_setValue(PORT_F, PIN_4);
		}
		else if ((ledGreenTimer > 16) && (ledGreenTimer < 32))
		{
			gpio_clearValue(PORT_F, PIN_4);
		}
		else
		{
			ledGreenTimer = 0;
		}
		break;
	}
}

static void led_changeStateBlue(LedState aState)
{
	switch(aState)
	{
	case eLEDON:
		gpio_setValue(PORT_F, PIN_5);
		ledBlueTimer = 0;
		break;
	case eLEDOFF:
		gpio_clearValue(PORT_F, PIN_5);
		ledBlueTimer = 0;
		break;
	case eLEDBLINK1000MS:
		if (ledBlueTimer <= 64)
		{
			gpio_setValue(PORT_F, PIN_5);
		}
		else if ((ledBlueTimer > 64) && (ledBlueTimer < 128))
		{
			gpio_clearValue(PORT_F, PIN_5);
		}
		else
		{
			ledBlueTimer = 0;
		}
		break;
	case eLEDBLINK500MS:
		if (ledBlueTimer <= 32)
		{
			gpio_setValue(PORT_F, PIN_5);
		}
		else if ((ledBlueTimer > 32) && (ledBlueTimer < 64))
		{
			gpio_clearValue(PORT_F, PIN_5);
		}
		else
		{
			ledBlueTimer = 0;
		}
		break;
	case eLEDBLINK250MS:
		if (ledBlueTimer <= 16)
		{
			gpio_setValue(PORT_F, PIN_5);
		}
		else if ((ledBlueTimer > 16) && (ledBlueTimer < 32))
		{
			gpio_clearValue(PORT_F, PIN_5);
		}
		else
		{
			ledBlueTimer = 0;
		}
		break;
	}
}


