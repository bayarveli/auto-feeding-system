/*
 * motor.h
 *
 *  Created on: Dec 6, 2020
 *      Author: veli-
 */

#ifndef INCLUDE_MOTORAPI_H_
#define INCLUDE_MOTORAPI_H_

#include "stdint.h"

void motor_init();

void motor_driveMoveForward();
void motor_driveMoveBackward();
void motor_driveStop();
void motor_driveSetMotorSpeed(uint8_t aSpeed);

void motor_feedingMoveForward();
void motor_feedingMoveBackward();
void motor_feedingStop();
void motor_feedingSetMotorSpeed(uint8_t aSpeed);

#endif /* INCLUDE_MOTORAPI_H_ */
