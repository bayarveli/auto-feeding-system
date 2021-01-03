/*
 * esp32.h
 *
 *  Created on: Dec 6, 2020
 *      Author: veli-
 */

#ifndef INCLUDE_ESP32_H_
#define INCLUDE_ESP32_H_

#include "usart.h"
#include "m_usb.h"

#define SERIAL_RX_BUFFER	(128)
#define SERIAL_TX_BUFFER	(64)

typedef enum
{
	eCONNECTED,
	eDISCONNECTED
} CommStatus;

void esp32_init();
int8_t esp32_isReady(void);
void esp32_isConnected(CommStatus* aEspConnectionStatus);
uint8_t esp32_isDataReceived(uint8_t *aData, uint8_t *aDataLen);
void esp32_sendMessage(uint8_t *aData, uint8_t *aDataLen);

#endif /* INCLUDE_ESP32_H_ */
