/*
 * timerapi.h
 *
 *  Created on: Dec 26, 2020
 *      Author: veli-
 */

#ifndef INCLUDE_TIMERAPI_H_
#define INCLUDE_TIMERAPI_H_

#include "stdint.h"

void setTimeout(uint16_t aTimeout);
int8_t getTimeoutStatus();


#endif /* INCLUDE_TIMERAPI_H_ */
