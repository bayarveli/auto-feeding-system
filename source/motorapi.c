/*
 * motor.c
 *
 *  Created on: Dec 6, 2020
 *      Author: veli-
 */

#include "motorapi.h"
#include "gpio.h"
#include "timer.h"

void motor_init()
{
	gpio_setDir(PORT_B,	PIN_5, OUT); // Driver Motor PWM - OC1A
	gpio_setDir(PORT_D, PIN_7, OUT); // Driver Motor IN-A
	gpio_setDir(PORT_D, PIN_6, OUT); // Driver Motor IN-B
	gpio_setDir(PORT_C, PIN_7, OUT); // Driver Motor Enable
	gpio_setDir(PORT_F, PIN_0, IN); // Driver Motor Current Sense

	gpio_setDir(PORT_B, PIN_6, OUT); // Feeding Motor PWM - OC1B
	gpio_setDir(PORT_D, PIN_4, OUT); // Feeding Motor IN-A
	gpio_setDir(PORT_D, PIN_5, OUT); // Feeding Motor IN-B
	gpio_setDir(PORT_C, PIN_6, OUT); // Feeding Motor Enable
	gpio_setDir(PORT_F, PIN_1, IN); // Feeding Motor Current Sense

	// Clear gpio outputs
	gpio_clearValue(PORT_B, PIN_5);
	gpio_clearValue(PORT_D, PIN_7);
	gpio_clearValue(PORT_D, PIN_6);
	gpio_clearValue(PORT_C, PIN_7);

	gpio_clearValue(PORT_B, PIN_6);
	gpio_clearValue(PORT_D, PIN_4);
	gpio_clearValue(PORT_D, PIN_5);
	gpio_clearValue(PORT_C, PIN_6);

	timer_configure(TIMER_1, (TimerWaveformType)TIMER_13_FAST_PWM_8_BIT_TOP);
	timer_ocr_configure(TIMER_1, OCR1_A, NOT_INVERTED);
	timer_ocr_configure(TIMER_1, OCR1_B, NOT_INVERTED);

	motor_driveSetMotorSpeed(0);
	motor_feedingSetMotorSpeed(0);

	timer_start(TIMER_1, CLK_IO_DIV_BY_1024);

	gpio_setValue(PORT_C, PIN_7);
	gpio_setValue(PORT_C, PIN_6);
}

void motor_driveSetMotorSpeed(uint8_t aSpeed)
{
	timer_set_ocr_value(TIMER_1, OCR1_A, aSpeed);
}

void motor_driveMoveForward()
{
	// Clockwise direction
	gpio_setValue(PORT_D, PIN_7); // INA = 1
	gpio_clearValue(PORT_D, PIN_6); // INB = 0

}

void motor_driveMoveBackward()
{
	// Counter Clockwise direction
	gpio_setValue(PORT_D, PIN_6); // INB = 1
	gpio_clearValue(PORT_D, PIN_7); // INA = 0
}

void motor_driveStop()
{
	// Brake to GND
	gpio_clearValue(PORT_D, PIN_6); // INB = 0
	gpio_clearValue(PORT_D, PIN_7); // INA = 0
}

void motor_feedingSetMotorSpeed(uint8_t aSpeed)
{
	timer_set_ocr_value(TIMER_1, OCR1_B, aSpeed);
}


void motor_feedingMoveForward()
{
	// Clockwise direction
	gpio_setValue(PORT_D, PIN_4); // IN-A = 1
	gpio_clearValue(PORT_D, PIN_5); // IN-B = 0
}

void motor_feedingMoveBackward()
{
	// Counter Clockwise direction
	gpio_setValue(PORT_D, PIN_5); // IN-B = 1
	gpio_clearValue(PORT_D, PIN_4); // IN-A = 0
}

void motor_feedingStop()
{
	// Brake to GND
	gpio_clearValue(PORT_D, PIN_5); // IN-B = 0
	gpio_clearValue(PORT_D, PIN_4); // IN-A = 0
}


