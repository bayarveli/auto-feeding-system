/*
 * timer.h
 *
 *  Created on: May 30, 2020
 *      Author: veli-
 */

#ifndef INCLUDE_TIMER_H_
#define INCLUDE_TIMER_H_

#include <stdint.h>
#include "atmega32u4.h"

/*
 * WGM2 | WGM1 | WGM0
 */
#define TIMER_0_NORMAL 					0
#define	TIMER_0_PWM_PHASE_CORRECT_TOP 	1
#define	TIMER_0_CTC_OCR					2
#define	TIMER_0_FAST_PWM_TOP			3
#define	TIMER_0_PWM_PHASE_CORRECT_OCR	5
#define	TIMER_0_FAST_PWM_OCR			7

/*
 * WGMn3 | WGMn2 | WGMn1 | WGMn0 where n=1,3
 */
#define TIMER_13_NORMAL								0
#define TIMER_13_PWM_PHASE_CORRECT_8_BIT_TOP		1
#define TIMER_13_PWM_PHASE_CORRECT_9_BIT_TOP		2
#define TIMER_13_PWM_PHASE_CORRECT_10_BIT_TOP		3
#define TIMER_13_CTC_OCR							4
#define TIMER_13_FAST_PWM_8_BIT_TOP					5
#define TIMER_13_FAST_PWM_9_BIT_TOP					6
#define TIMER_13_FAST_PWM_10_BIT_TOP				7
#define TIMER_13_PWM_PHASE_AND_FREQ_CORRECT_ICR		8
#define TIMER_13_PWM_PHASE_AND_FREQ_CORRECT_OCR		9
#define TIMER_13_PWM_PHASE_CORRECT_ICR				10
#define TIMER_13_PWM_PHASE_CORRECT_OCR				11
#define TIMER_13_CTC_ICR							12
#define TIMER_13_FAST_PWM_ICR						14
#define TIMER_13_FAST_PWM_OCR						15

/*
 * Waveform definition
 */
typedef uint8_t TimerWaveformType;

/**
 * @brief Port name definition
 */
typedef enum
{
	TIMER_0,
	TIMER_1,
	TIMER_3,
	TIMER_4
}TimerType;


typedef enum
{
	OCR0_A,
	OCR0_B,
	OCR1_A,
	OCR1_B,
	OCR1_C,
	OCR3_A,
	OCR3_B,
	OCR3_C,
	OCR4_A,
	OCR4_B,
	OCR4_C,
	OCR4_D
}OutputCompareType;

typedef enum
{
	NO_CLOCK = 0,
	CLK_IO = 1,
	CLK_IO_DIV_BY_8 = 2,
	CLK_IO_DIV_BY_64 = 3,
	CLK_IO_DIV_BY_256 = 4,
	CLK_IO_DIV_BY_1024 = 5,
	EXT_CLK_TN_FALLING_EDGE = 6,
	EXT_CLK_TN_RISING_EDGE = 7,
}TimerClockSelect;

typedef enum
{
	NOT_INVERTED = 2,
	INVERTED = 3,
}OutputCompareMode;

void timer_configure(TimerType timer, TimerWaveformType waveform);

void timer_ocr_configure(TimerType timer, OutputCompareType outCompReg, OutputCompareMode mode);

void timer_set_ocr_value(TimerType timer, OutputCompareType outCompReg, uint16_t outCompRegValue);

void timer_start(TimerType timer, TimerClockSelect clock);

void timer_stop(TimerType timer);

#endif /* INCLUDE_TIMER_H_ */
