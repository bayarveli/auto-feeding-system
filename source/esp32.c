/*
 * esp32.c
 *
 *  Created on: Dec 6, 2020
 *      Author: veli-
 */

#include "esp32.h"
#include <string.h>

typedef enum
{
	eTEST,
	eCONFIG,
	eLISTEN,
	eDATA,
} CommState;

//
typedef enum
{
	eWRITE,
	eREAD
} UsartState;

//void esp32_receiveInterrupt(void);
//int8_t esp32_executeTransaction(char* aCommand, char* aKeyResponse);
//int8_t esp32_buildCommand(char* aCommand);

int8_t esp32_executeTransaction(char* aCommand, char* aKeyResponse);

//
static uint8_t gSerialRxBuffer[SERIAL_RX_BUFFER];
static int32_t gSerialRxIdx = 0;
static uint8_t gSerialTxBuffer[SERIAL_TX_BUFFER];

CommState espCommState = eTEST;
UsartState espUsartState = eREAD;

void esp32_receiveInterrupt(void)
{
	unsigned char data = UDR1;

	// Check for error
	if((UCSR1A & ((1 << FE1) | (1 << DOR1) | (1 << UPE1))) == 0)
	{
		gSerialRxBuffer[gSerialRxIdx++] = data;
		gSerialRxBuffer[gSerialRxIdx] = '\0';

		usb_tx_char(data);

		if(gSerialRxIdx > (SERIAL_RX_BUFFER - 1))
		{
			// if buffer is full, than reset the index.
			gSerialRxIdx = 0;
		}
	}
}

void esp32_init()
{
    // USART initialization
	usart_init(USART_ASYNCH,
			   BAUDRATE_9600,
			   DATABIT_8,
			   PARITY_NONE,
			   STOPBIT_1,
			   RISING_EDGE,
			   NOMINAL_SPEED);
	usart_setCallback(esp32_receiveInterrupt);
	usart_open();
}




int8_t esp32_executeTransaction(char* aCommand, char* aKeyResponse)
{
	int8_t status = -1;
	int8_t isTransactionComplete = 0;

	if (aCommand != 0)
	{
		espUsartState = eWRITE;
	}

	while (!isTransactionComplete)
	{
		switch(espUsartState)
		{
		case eWRITE:
			usart_transmit(aCommand, 1);
			espUsartState = eREAD;
			_delay_ms(500);
			break;
		case eREAD:
			if (strstr(gSerialRxBuffer, aKeyResponse) != 0)
			{
				status = 0;

			}
			isTransactionComplete = 1;
			gSerialRxIdx = 0;
			break;
		default:
			break;
		}
	}

	return status;
}

int8_t esp32_isReady(void)
{
	int8_t status = 0;
	espCommState = eTEST;

	while (status == 0)
	{
		switch (espCommState)
		{
		case eTEST:
			// send "AT/r/n" command for write test and wait for "OK" response
			if (esp32_executeTransaction("AT\r\n", "OK") == 0)
			{
				espCommState = eCONFIG;
			}
			else
			{
				status = -1;
			}
			break;
		case eCONFIG:
			if (esp32_executeTransaction("AT+CIPMUX=1\r\n", "OK") == 0)
			{
				_delay_ms(500);
				if (esp32_executeTransaction("AT+CIPSERVER=1,1001\r\n", "OK") == 0)
				{
					status = 1;
				}
				else
				{
					status = -1;
				}
			}
			else
			{
				status = -1;
			}
			break;
		default:
			break;
		}

		_delay_ms(500);
	}

	return status;
}

void esp32_isConnected(CommStatus* aEspConnectionStatus)
{
	if (esp32_executeTransaction(0, ",CONNECT") == 0)
	{
		*aEspConnectionStatus = eCONNECTED;
	}

	if (esp32_executeTransaction(0, ",CLOSED") == 0)
	{
		*aEspConnectionStatus = eDISCONNECTED;
	}
}

uint8_t esp32_isDataReceived(uint8_t *aData, uint8_t *aDataLen)
{
	char messageLen[4] = { 0 };
	int8_t msgIdx = 0;
	// Example: +IPD,0,11:TESTMESSAGE
	if (strstr(gSerialRxBuffer, "+IPD,") != 0)
	{
		int8_t recMsgLen = strlen(gSerialRxBuffer);
		usb_tx_decimal(recMsgLen);
		usb_tx_char('\n');

		char* messagePtr = gSerialRxBuffer;

		while(*(messagePtr++) != ',' );
		while(*(messagePtr++) != ',' );
		while(*messagePtr != ':')
		{
			messageLen[msgIdx++] = *(messagePtr++);
		}
		*aDataLen = atoi(messageLen);

		msgIdx = 0;
		messagePtr++;
		while(*messagePtr != '\0')
		{
			aData[msgIdx++] = *(messagePtr++);
		}

		gSerialRxBuffer[0] = '\0';
		gSerialRxIdx = 0;
		PORTF |= 1 << 4;
		return 0;
	}
	else
	{
		*aDataLen = 0;
		PORTF &= ~(1 << 4);
		return -1;
	}
}

void esp32_sendMessage(uint8_t *aData, uint8_t *aDataLen)
{
	//				char command[] = "AT+CIPSEND=0,7\r\n";
	//				usart_transmit(command, 1);
	//				_delay_ms(250);
	//				usart_transmit(gMsgBuf, 1);
}
