/*
 * rtcapi.c
 *
 *  Created on: Dec 26, 2020
 *      Author: veli-
 */


#define F_CPU 					16000000UL

#include "rtcapi.h"
#include "i2c.h"
#include <string.h>
#include <util/delay.h>

static uint32_t convertDateToUnixTime(const DateTime *date);
static uint8_t computeDayOfWeek(uint16_t y, uint8_t m, uint8_t d);
static void convertUnixTimeToDate(uint32_t t, DateTime *date);
static int32_t bcdToInt(uint8_t bcdNumber);
static uint8_t intToBcd(int32_t aIntNum);

void rtcInit(void)
{
	i2c_init();
}

int8_t getRtcTime(DateTime *aGetTimeValue)
{
	int8_t status = 0;
	uint8_t rawData[7] = { 0 };
	uint8_t regAddr = 0;
	int8_t idx = 0;

	for (idx = 0; idx < 7; idx++)
	{
		regAddr = (uint8_t)idx;

		status = i2c_read_after_write(DS1307_ADDR, &regAddr, 1, &rawData[idx], 1);

		if (status != 0)
		{
			return status;
		}

		_delay_ms(1);
	}

	memcpy(aGetTimeValue, rawData, 7);

	aGetTimeValue->year = bcdToInt(rawData[6]) + 2000;
	aGetTimeValue->month = bcdToInt(rawData[5]);
	aGetTimeValue->day = bcdToInt(rawData[4]);
	aGetTimeValue->dayOfWeek = bcdToInt(rawData[3]);
	aGetTimeValue->hours = bcdToInt(rawData[2] & 0x3F);
	aGetTimeValue->minutes = bcdToInt(rawData[1]);
	aGetTimeValue->seconds = bcdToInt(rawData[0] & 0x7F);

	return 0;
}

int8_t setRtcTime(DateTime *aSetTimeValue)
{
	int8_t status = 0;
	uint8_t rawData[7] = { 0 };
	uint8_t regAddr = 0;
	int8_t idx = 0;

	rawData[0] = intToBcd(aSetTimeValue->seconds);
	rawData[1] = intToBcd(aSetTimeValue->minutes);
	rawData[2] = intToBcd(aSetTimeValue->hours);
	rawData[3] = intToBcd(aSetTimeValue->dayOfWeek);
	rawData[4] = intToBcd(aSetTimeValue->day);
	rawData[5] = intToBcd(aSetTimeValue->month);
	rawData[6] = intToBcd(aSetTimeValue->year - 2000);

	for (idx = 0; idx < 7; idx++)
	{
		regAddr = idx;

		status = i2c_write(DS1307_ADDR, &regAddr, 1, &rawData[idx], 1);

		if (status != 0)
		{
			return -1;
		}
	}

	return 0;
}

int8_t compareDateTime(DateTime* aCurrTime, DateTime* aTaskTime)
{
	if (aCurrTime->year > aTaskTime->year){}
	else if (aCurrTime->year < aTaskTime->year){}
	else
	{
		if (aCurrTime->month > aTaskTime->month){}
		else if (aCurrTime->month < aTaskTime->month){}
		else
		{
			if (aCurrTime->day > aTaskTime->day){}
			else if (aCurrTime->day < aTaskTime->day){}
			else
			{
				if (aCurrTime->hours > aTaskTime->hours){}
				else if (aCurrTime->hours < aTaskTime->hours){}
				else
				{
					if (aCurrTime->minutes > aTaskTime->minutes){}
					else if (aCurrTime->minutes < aTaskTime->minutes){}
					else
					{
						if (aCurrTime->seconds >= aTaskTime->seconds)
						{
							return 1;
						}
						else
						{
							return 0;
						}
					}
				}
			}
		}
	}

	return -1;
}


// https://www.oryx-embedded.com/doc/date__time_8c_source.html
static uint32_t convertDateToUnixTime(const DateTime *date)
{


//	uint16_t y;
//	uint16_t m;
//	uint16_t d;
//	uint32_t t;
//	uint32_t hh;
//
//	m_usb_tx_char(' ');
//
//	//Year
//	y = aDate2.year;
//	//Month of year
//	m = aDate2.month;
//	//Day of month
//	d = aDate2.day;
//
//	//January and February are counted as months 13 and 14 of the previous year
//	if(m <= 2)
//	{
//	  m += 12;
//	  y -= 1;
//	}
//
//	usb_tx_decimal(m);
//	m_usb_tx_char(' ');
//	usb_tx_decimal(y);
//	m_usb_tx_char(' ');
//
//
//	MultiU16X16to32(t, 365, y);
//
//// 	t = (uint16_t)365 * y;
//	m_usb_tx_ulong(t);
//	m_usb_tx_char(' ');
//
//	//Convert years to days
//	// t = (365 * y) + (y / 4) - (y / 100) + (y / 400);
//	t += (y / 4) - (y / 100) + (y / 400);
//
//	m_usb_tx_ulong(t);
//	m_usb_tx_char(' ');
////	//Convert months to days
//	t += (30 * m) + (3 * (m + 1) / 5) + d;
//
//	m_usb_tx_ulong(t);
//	m_usb_tx_char(' ');
////	//Unix time starts on January 1st, 1970
//	t -= 719561;
////
//	m_usb_tx_ulong(t);
//	m_usb_tx_char(' ');
////	//Convert days to seconds
//// 	t *= 86400;
//
//	MultiU16X16to32(t, t, 4);
//
//	m_usb_tx_ulong(t);
//	m_usb_tx_char(' ');
//
//	MultiU16X16to32(t, t, 21600);
//
//	m_usb_tx_ulong(t);
//	m_usb_tx_char(' ');
//
//
////	if (aDate2.hours > 18)
////	{
//		MultiU16X16to32(hh, 3600, (uint16_t)aDate2.hours);
//		m_usb_tx_ulong(hh);
//		m_usb_tx_char(' ');
//
//		t += (hh + (60 * aDate2.minutes) + aDate2.seconds);
//		m_usb_tx_ulong(t);
//
//
//	usb_tx_push();
//	for (xx = 0; xx < 1000; xx++)
//	{
//		_delay_ms(10);
//	}
//
//	}
//	return 0;


   uint16_t y;
   uint16_t m;
   uint16_t d;
   uint32_t t;

   //Year
   y = date->year;
   //Month of year
   m = date->month;
   //Day of month
   d = date->day;

   //January and February are counted as months 13 and 14 of the previous year
   if(m <= 2)
   {
      m += 12;
      y -= 1;
   }

   //Convert years to days
   t = (365 * y) + (y / 4) - (y / 100) + (y / 400);
   //Convert months to days
   t += (30 * m) + (3 * (m + 1) / 5) + d;
   //Unix time starts on January 1st, 1970
   t -= 719561;
   //Convert days to seconds
   t *= 86400;
   //Add hours, minutes and seconds
   t += (3600 * date->hours) + (60 * date->minutes) + date->seconds;

   //Return Unix time
   return t;
}

static uint8_t computeDayOfWeek(uint16_t y, uint8_t m, uint8_t d)
{
   unsigned int h;
   unsigned int j;
   unsigned int k;

   //January and February are counted as months 13 and 14 of the previous year
   if(m <= 2)
   {
      m += 12;
      y -= 1;
   }

   //J is the century
   j = y / 100;
   //K the year of the century
   k = y % 100;

   //Compute H using Zeller's congruence
   h = d + (26 * (m + 1) / 10) + k + (k / 4) + (5 * j) + (j / 4);

   //Return the day of the week
   return ((h + 5) % 7) + 1;
}


static void convertUnixTimeToDate(uint32_t t, DateTime *date)
{
   uint32_t a;
   uint32_t b;
   uint32_t c;
   uint32_t d;
   uint32_t e;
   uint32_t f;

   //Negative Unix time values are not supported
   if(t < 1)
      t = 0;

   //Clear milliseconds
   // date->milliseconds = 0;

   //Retrieve hours, minutes and seconds
   date->seconds = t % 60;
   t /= 60;
   date->minutes = t % 60;
   t /= 60;
   date->hours = t % 24;
   t /= 24;

   //Convert Unix time to date
   a = (uint32_t) ((4 * t + 102032) / 146097 + 15);
   b = (uint32_t) (t + 2442113 + a - (a / 4));
   c = (20 * b - 2442) / 7305;
   d = b - 365 * c - (c / 4);
   e = d * 1000 / 30601;
   f = d - e * 30 - e * 601 / 1000;

   //January and February are counted as months 13 and 14 of the previous year
   if(e <= 13)
   {
      c -= 4716;
      e -= 1;
   }
   else
   {
      c -= 4715;
      e -= 13;
   }

   //Retrieve year, month and day
   date->year = c;
   date->month = e;
   date->day = f;

   //Calculate day of week
   date->dayOfWeek = computeDayOfWeek(c, e, f);
}

static int32_t bcdToInt(uint8_t bcdNumber)
{
	return (((bcdNumber & 0xF0) >> 4) * 10 + (bcdNumber & 0x0F));
}

static uint8_t intToBcd(int32_t aIntNum)
{
	return ((aIntNum / 10) << 4) | (aIntNum % 10);
}
