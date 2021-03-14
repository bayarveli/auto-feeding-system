/*
 * eepromapi.h
 *
 *  Created on: Dec 26, 2020
 *      Author: veli-
 */

#ifndef INCLUDE_EEPROMAPI_H_
#define INCLUDE_EEPROMAPI_H_

#include "stdint.h"

#define EEPROM_SLAVE_ADDR		(0x54)

void eepromInit(void);
int8_t eepromWrite(uint16_t aStartAddress, uint8_t* aData, uint8_t aDataLen);
int8_t eepromRead(uint16_t aStartAddress, uint8_t* aData, uint8_t aDataLen);

#endif /* INCLUDE_EEPROMAPI_H_ */
