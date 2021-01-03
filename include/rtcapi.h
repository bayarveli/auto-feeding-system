/*
 * rtcapi.h
 *
 *  Created on: Dec 26, 2020
 *      Author: veli-
 */

#ifndef INCLUDE_RTCAPI_H_
#define INCLUDE_RTCAPI_H_

#include "stdint.h"
#include "dateTime.h"

#define DS1307_ADDR		(0x68)

// Address 		Bit7  |  Bit6	Bit5	Bit4  |  Bit3	Bit2	Bit1	Bit0
// -------------------------------------------------------------------------
// 00h			 CH	  |     10 Seconds		  |		      Seconds
// 01h

void rtcInit(void);
int8_t getRtcTime(DateTime *aGetTimeValue);
int8_t setRtcTime(DateTime *aSetTimeValue);
int8_t compareDateTime(DateTime* aCurrTime, DateTime* aTaskTime);

#endif /* INCLUDE_RTCAPI_H_ */
