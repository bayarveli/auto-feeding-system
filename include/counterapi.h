/*
 * timerapi.h
 *
 *  Created on: Dec 26, 2020
 *      Author: veli-
 */

#ifndef INCLUDE_COUNTERAPI_H_
#define INCLUDE_COUNTERAPI_H_

#include "stdint.h"



void initCounter();
void setTimeout(uint16_t aTimeout);
void clearTimeout();
int8_t getTimeoutStatus();


#endif /* INCLUDE_COUNTERAPI_H_ */
