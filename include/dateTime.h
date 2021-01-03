/*
 * dateTime.h
 *
 *  Created on: Jan 3, 2021
 *      Author: veli-
 */

#ifndef INCLUDE_DATETIME_H_
#define INCLUDE_DATETIME_H_

typedef struct
{
   uint16_t year;
   uint8_t month;
   uint8_t day;
   uint8_t dayOfWeek;
   uint8_t hours;
   uint8_t minutes;
   uint8_t seconds;
} DateTime;

#endif /* INCLUDE_DATETIME_H_ */
