/*
 * ledapi.h
 *
 *  Created on: Apr 4, 2021
 *      Author: veli-
 */

#ifndef INCLUDE_LEDAPI_H_
#define INCLUDE_LEDAPI_H_

#include "stdint.h"

typedef enum
{
	eLEDON = 0,
	eLEDOFF = 1,
	eLEDBLINK1000MS = 2,
	eLEDBLINK500MS = 3,
	eLEDBLINK250MS = 4
} LedState;

typedef enum
{
	eGREEN = 0,
	eBLUE = 1
} LedType;

void led_init(void);
void led_changeState(LedType aType, LedState aState);
//void led_changeStateBlue(LedState aState);
//void led_changeStateGreen(LedState aState);

#endif /* INCLUDE_LEDAPI_H_ */
