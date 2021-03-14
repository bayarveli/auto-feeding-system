/*
 * timerapi.c
 *
 *  Created on: Dec 26, 2020
 *      Author: veli-
 */


#include "counterapi.h"
#include "timer.h"
#include "m_usb.h"

#define COUNTER_1_SEC_VALUE	(15625)

static uint16_t counterTimeout = 0;
static uint16_t counterValue = 0;

void counter_timer3_ocra(void)
{
	counterValue++;
	timer_clear(TIMER_3);
	//
}

void initCounter()
{
	timer_configure(TIMER_3, (TimerWaveformType)TIMER_13_NORMAL);
	timer_ocr_configure(TIMER_3, OCR3_A, NORMAL);
	timer_set_ocr_value(TIMER_3, OCR3_A, COUNTER_1_SEC_VALUE);
	timer_setCallbackTimer3OCRAMatch(counter_timer3_ocra);
}

void setTimeout(uint16_t aTimeout)
{
	counterTimeout = aTimeout;
	timer_start(TIMER_3, CLK_IO_DIV_BY_1024);
}

void clearTimeout()
{
	counterValue = 0;
	counterTimeout = 0;
	timer_stop(TIMER_3);
}

int8_t getTimeoutStatus()
{
	if (counterValue < counterTimeout)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
