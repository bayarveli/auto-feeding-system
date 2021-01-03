/*
 * usart.h
 *
 *  Created on: Apr 21, 2020
 *      Author: veli-
 */

#include <stdint.h>
#include "atmega32u4.h"

static void (*usartRxCallback)(void);

// uart mode selection
typedef enum
{
	USART_ASYNCH = 0,
	USART_SYNC = 1,
	USART_SPI = 3
}UartModeSelect;

// baudrate

typedef enum
{
	BAUDRATE_9600,
	BAUDRATE_19200,
	BAUDRATE_38400,
	BAUDRATE_115200
}BaudrateSelect;

// data bit
typedef enum
{
	DATABIT_5 = 0,
	DATABIT_6 = 1,
	DATABIT_7 = 2,
	DATABIT_8 = 3,
	DATABIT_9 = 7
}DataBitSelect;

// parity
typedef enum
{
	PARITY_NONE = 0,
	PARITY_EVEN = 2,
	PARITY_ODD = 3
}ParityModeSelect;

// stop bit
typedef enum
{
	STOPBIT_1 = 0,
	STOPBIT_2 = 1
}StopBitSelect;

// clock polarity
typedef enum
{
	RISING_EDGE = 0,
	FALLING_EDGE = 1
}PolaritySelect;

// Speed Selection
typedef enum
{
	NOMINAL_SPEED = 0,
	DOUBLE_SPEED = 1
}SpeedSelect;

/**
 * @brief Initialize USART interface -
 * Mode, Baudrate, DataBit, Parity, StopBit,
 * Polarity, Speed
 * @param uartMode 		USART operation mode
 * @param baudrate		Data transfer speed
 * @param dataBit		Data size
 * @param parityMode	Parity selection
 * @param stopBit		Stop bit size
 * @param polarity		Operating edge selection - rising or falling
 * @param speed			Speed selection between double and nominal
 * @return	N/A
 * @note	N/A
 * @warning N/A
 */
void usart_init(UartModeSelect uartMode,
				BaudrateSelect baudrate,
				DataBitSelect dataBit,
				ParityModeSelect parityMode,
				StopBitSelect stopBit,
				PolaritySelect polarity,
				SpeedSelect speed);

/**
 * @brief Reads pin value
 * @param portType 	Port type
 * @param pinNo		Pin number
 * @return	uint8_t	Pin value
 * @note	There is a note.
 * @warning No warning
 */
void usart_open();

/**
 * @brief Reads pin value
 * @param portType 	Port type
 * @param pinNo		Pin number
 * @return	uint8_t	Pin value
 * @note	There is a note.
 * @warning No warning
 */
void usart_transmit(uint8_t * dataToTransmit, uint8_t isString);

/**
 * @brief Reads pin value
 * @param portType 	Port type
 * @param pinNo		Pin number
 * @return	uint8_t	Pin value
 * @note	There is a note.
 * @warning No warning
 */
void usart_receive();

/**
 * @brief Reads pin value
 * @param portType 	Port type
 * @param pinNo		Pin number
 * @return	uint8_t	Pin value
 * @note	There is a note.
 * @warning No warning
 */
void usart_close();

/**
 * @brief Reads pin value
 * @param portType 	Port type
 * @param pinNo		Pin number
 * @return	uint8_t	Pin value
 * @note	There is a note.
 * @warning No warning
 */
void usart_flush();

void usart_setCallback(void (*ptr)());
