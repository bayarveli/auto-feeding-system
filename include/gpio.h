/**
 * @file gpio.h
 * @author Veli Bayar
 * @date 29 Dec 2019
 * @brief A template of GPIO library for AVR - Atmega32U4
 */

#include <stdint.h>
#include "atmega32u4.h"

/**
 * @brief Port name definition
 */
typedef enum
{
	PORT_A,
	PORT_B,
	PORT_C,
	PORT_D,
	PORT_E,
	PORT_F
}GpioPortType;

/**
 * @brief Pin direction definition
 */
typedef enum
{
	IN,
	OUT
}GpioPinDir;

/**
 * @brief Pin number definition
 */
typedef enum
{
	PIN_0,
	PIN_1,
	PIN_2,
	PIN_3,
	PIN_4,
	PIN_5,
	PIN_6,
	PIN_7
}GpioPinNumber;


/**
 * @brief Sets pin direction as input or output
 * @param portType 	Port type
 * @param pinNo		Pin number
 * @param pinDir	Pin direction
 * @return	N/A
 * @note	N/A
 * @warning N/A
 */
void gpio_setDir(GpioPortType portType, GpioPinNumber pinNo, GpioPinDir pinDir);

/**
 * @brief Sets pin value - LOGIC HIGH
 * @param portType 	Port type
 * @param pinNo		Pin number
 * @return	N/A
 * @note	N/A
 * @warning N/A
 */
void gpio_setValue(GpioPortType portType, GpioPinNumber pinNo);

/**
 * @brief Clears pin value - LOGIC LOW
 * @param portType 	Port type
 * @param pinNo		Pin number
 * @return	N/A
 * @note	N/A
 * @warning N/A
 */
void gpio_clearValue(GpioPortType portType, GpioPinNumber pinNo);

/**
 * @brief Reads pin value
 * @param portType 	Port type
 * @param pinNo		Pin number
 * @return	uint8_t	Pin value
 * @note	N/A
 * @warning N/A
 */
uint8_t gpio_readValue(GpioPortType portType, GpioPinNumber pinNo);
