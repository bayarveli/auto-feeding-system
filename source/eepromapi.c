/*
 * eepromapi.c
 *
 *  Created on: Dec 26, 2020
 *      Author: veli-
 */

#define F_CPU 					16000000UL

#include "eepromapi.h"
#include "i2c.h"
#include <util/delay.h>

void eepromInit(void)
{
	i2c_init();
}

int8_t eepromWrite(uint16_t aStartAddress, uint8_t* aData, uint8_t aDataLen)
{
	int8_t status = 0;
	uint8_t registerAddress[2] = { 0 };
	int8_t idx = 0;
	uint8_t data = 0;

	for (idx = 0; idx < aDataLen; idx++)
	{
		aStartAddress += idx;

		registerAddress[0] = (aStartAddress & 0xFF00) >> 8;
		registerAddress[1] = (aStartAddress & 0x00FF);

		data = aData[idx];

		status = i2c_write(EEPROM_SLAVE_ADDR, registerAddress, 2, &data, 1);

		if (status != 0)
		{
			return status;
		}

		_delay_ms(6);
	}

	return status;
}

int8_t eepromRead(uint16_t aStartAddress, uint8_t* aData, uint8_t aDataLen)
{
	int8_t status = 0;
	uint8_t registerAddress[2] = { 0 };
	uint8_t data = 0;
	int8_t idx = 0;

	for (idx = 0; idx < aDataLen; idx++)
	{
		aStartAddress += idx;

		registerAddress[0] = (aStartAddress & 0xFF00) >> 8;
		registerAddress[1] = aStartAddress & 0x00FF;

		status = i2c_read_after_write(EEPROM_SLAVE_ADDR, registerAddress, 2, &data,
				1);

		if (status != 0)
		{
			return -1;
		}

		*(aData++) = data;

		_delay_ms(10);
		_delay_ms(10);
		_delay_ms(10);
	}

	return 0;
}

