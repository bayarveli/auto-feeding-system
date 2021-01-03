/**
 * @file i2c.h
 * @author Veli Bayar
 * @date 04 Jul 2020
 * @brief A template of IIC library for AVR - Atmega32U4
 */

#ifndef INCLUDE_I2C_H_
#define INCLUDE_I2C_H_

#include <stdint.h>
#include "atmega32u4.h"

/**
 * @brief 	Init IIC interface - data transfer speed
 * @param 	N/A
 * @return	N/A
 * @note	Transfer speed is fixed to 100 kHz
 * @warning N/A
 */
void i2c_init();

/**
 * @brief 	Write a register of the slave device
 * @param 	slaveAddress	Slave device address
 * @param 	wordAddress		Register address for the memory map of the slave device
 * @param 	*dataToSent		Data address
 * @return	status 			0 (success) or TWI Status Register value
 * @note	N/A
 * @warning Wait (TBD) us between the two write operations.
 */
uint8_t i2c_write(uint8_t aSlaveAddress, uint8_t *aRegisterAddress, uint8_t aRegLen, uint8_t *aDataToReceive, uint8_t aDataLen);

/**
 * @brief 	Read after write from a register of the slave device
 * @param 	slaveAddress	Slave device address
 * @param 	wordAddress		Register address for the memory map of the slave device
 * @param 	*dataToReceive	Data address
 * @return	status 			0 (success) or TWI Status Register value
 * @note	There is a note.
 * @warning Wait (TBD) us between the two write operations.
 */
uint8_t i2c_read_after_write(uint8_t aSlaveAddress, uint8_t *aRegisterAddress, uint8_t aRegLen, uint8_t *aDataToReceive, uint8_t aDataLen);

/**
 * @brief 	N/A
 * @param 	N/A
 * @return	N/A
 * @note	N/A
 * @warning N/A
 */
void i2c_deinit();


#endif /* INCLUDE_I2C_H_ */
