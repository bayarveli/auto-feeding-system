/*
 * main.c
 *
 *  Created on: Dec 7, 2019
 *      Author: Veli Bayar
 */

// INCLUDES
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define F_CPU 					16000000UL

#include "board.h"
#include "util/delay.h"
#include "m_usb.h"
#include "eepromapi.h"
#include "motorapi.h"
#include "message.h"
#include "taskInfo.h"
#include "rtcapi.h"
#include "timerapi.h"
#include "esp32.h"

#include "i2c.h"

// ENUM
typedef enum
{
	eSYSINIT,
	eSYSFAIL,
	eSYSRUN,
	eSYSCONFIG,
	eSYSTEST
} SystemState;

typedef enum
{
	eOPINIT,
	eOPIDLE,
	eOPTASK,
	eOPRETURN,
	eOPABORT,
	eOPFAIL
} OperationState;

typedef struct
{
	uint8_t isDriveEnabled;
	uint8_t isDriveMotorStarted;
	uint8_t isFeedingEnabled;
	uint8_t isFeedingMotorStarted;
	uint32_t totalMoveTime;
} StateFlag;

// VARIABLES
static SystemState gSystemState = eSYSINIT;
static OperationState gOperationState = eOPINIT;
static uint8_t gEepromBuffer[TASK_SIZE];
static uint8_t gEepromBufferTest[TASK_SIZE];
uint8_t gMsgBuf[64];
uint8_t gMsgBufLen = 0;

CommStatus espCommStatus = eDISCONNECTED;

// Test functions
int8_t driveMotorTest();
int8_t eepromTest();

// MAIN FUNCTION
int main(void)
{
	// SYSTEM INITIALIZATION
    board_init();
    motor_init();
    esp32_init();
    eepromInit();
    rtcInit();
    m_usb_init();
    sei();

	while (!m_usb_isconnected())
		; // wait for a connection

	gpio_setValue(PORT_F, PIN_4);
	gpio_setValue(PORT_F, PIN_5);

	int xx  = 0;
	for (xx = 0; xx < 1500; xx++)
	{
		_delay_ms(10);
	}

	gpio_clearValue(PORT_F, PIN_4);
	gpio_clearValue(PORT_F, PIN_5);

	Task currentTask;
	uint8_t taskIdx = 0;
	uint16_t taskInfoAddr = 0;
	uint8_t pondIdx = 0;
	DateTime currTime;

	int8_t status = 0;

	StateFlag operationFlags;

	while (1)
	{
		esp32_isConnected(&espCommStatus);

		if (espCommStatus == eCONNECTED)
		{
			// usb_tx_string("Comm Status = CONNECTED \r\n");
			esp32_isDataReceived(gMsgBuf, &gMsgBufLen);

			if (gMsgBufLen > 0)
			{
				uint8_t messageId = gMsgBuf[0];

				switch(messageId)
				{
				case MSG_ID_START_CONFIG:
					if (gSystemState != eSYSCONFIG)
					{
						gSystemState = eSYSCONFIG;
					}
					break;
				case MSG_ID_END_CONFIG:
						gSystemState = eSYSRUN;
					break;
				case MSG_ID_ABORT_TASK:
					if (gOperationState != eOPABORT)
					{
						gOperationState = eOPABORT;
					}
					break;
				default:
					// Do nothing
					break;
				}
			}
		}
		else if (espCommStatus == eDISCONNECTED)
		{
			// usb_tx_string("Comm Status = DISCONNECTED \r\n");
		}
		else
		{
			// Nothing
		}

		switch (gSystemState)
		{
		case eSYSINIT:
			if (esp32_isReady() > 0)
			{
				usb_tx_string("ESP is READY  \r\n");
			}
			else
			{
				usb_tx_string("ESP cannot start.  \r\n");
			}

			gSystemState = eSYSRUN;

			break;
		case eSYSFAIL:
			break;
		case eSYSRUN:
			switch (gOperationState)
			{
			case eOPINIT:
				operationFlags.isDriveEnabled = 1;
				operationFlags.isDriveMotorStarted = 0;
				operationFlags.isFeedingEnabled = 0;
				operationFlags.isFeedingMotorStarted = 0;
				operationFlags.totalMoveTime = 0;

				// TODO: read task info from eeprom
				taskInfoAddr = TASK_START_ADDR + (taskIdx * TASK_SIZE);
				eepromRead(taskInfoAddr, gEepromBuffer, TASK_SIZE);
				memcpy(&currentTask, gEepromBuffer, TASK_SIZE);

				if (currentTask.isEnabled)
				{
					gOperationState = eOPIDLE;
				}

				taskIdx++;
				break;
			case eOPIDLE:
				// TODO: get current time and compare with task start time
				status = getRtcTime(&currTime);

				if (status != 0)
				{

				}

				if (compareDateTime(&currTime, &currentTask.startTime))
				{
					gOperationState = eOPTASK;
				}

				break;
			case eOPTASK:
				// TODO:
				if (1 == operationFlags.isDriveEnabled)
				{
					if (0 == operationFlags.isDriveMotorStarted) {
						motor_driveMoveForward();
						// TODO: relate pond idx and time

						setTimeout(currentTask.pondList[pondIdx].pondIdx);
						operationFlags.isDriveMotorStarted = 1;
						operationFlags.totalMoveTime += currentTask.pondList[pondIdx].pondIdx;
					}

					if (getTimeoutStatus()) {
						motor_driveStop();
						operationFlags.isFeedingEnabled = 1;
						operationFlags.isDriveEnabled = 0;
					}
				}

				if (1 == operationFlags.isFeedingEnabled)
				{
					if (0 == operationFlags.isFeedingMotorStarted)
					{
						motor_feedingMoveForward();

						setTimeout(currentTask.pondList[pondIdx].feedingTime);
						operationFlags.isFeedingMotorStarted = 1;
					}

					if (getTimeoutStatus())
					{
						motor_feedingStop();
						operationFlags.isFeedingEnabled = 0;
						operationFlags.isDriveEnabled = 1;
						operationFlags.isDriveMotorStarted = 0;
						operationFlags.isFeedingMotorStarted = 0;
						pondIdx++;
					}
				}

				if (pondIdx >= currentTask.totalPond)
				{
					gOperationState = eOPRETURN;
				}
				break;
			case eOPRETURN:
				if (0 == operationFlags.isDriveMotorStarted) {
					motor_driveMoveBackward();
					// TODO: relate pond idx and time

					setTimeout(operationFlags.totalMoveTime);
					operationFlags.isDriveMotorStarted = 1;
				}

				if (getTimeoutStatus())
				{
					motor_driveStop();
					gOperationState = eOPINIT;
				}
				break;
			case eOPABORT:
				break;
			case eOPFAIL:
				break;
			default:
				break;
			}


			break;
		case eSYSTEST:
			break;
		case eSYSCONFIG:
			break;
		default:
			break;
		}
	}

	return 0;
}

int8_t driveMotorTest()
{
	gpio_setDir(PORT_F, PIN_4, OUT);
	gpio_setDir(PORT_F, PIN_5, OUT);

	motor_driveSetMotorSpeed(255);
	motor_feedingSetMotorSpeed(127);

	while(1)
	{

		_delay_ms(5000);
		motor_driveMoveForward();
		gpio_setValue(PORT_F, PIN_4);
		gpio_setValue(PORT_F, PIN_5);
	//	PORTF |= 1 << 4;
	//	PORTF |= 1 << 5;
		_delay_ms(5000);
		gpio_clearValue(PORT_F, PIN_4);
		gpio_clearValue(PORT_F, PIN_5);
	//	PORTF &= ~(1 << 4);
	//	PORTF &= ~(1 << 5);
		motor_driveMoveBackward();
		_delay_ms(5000);
		gpio_setValue(PORT_F, PIN_4);
		gpio_setValue(PORT_F, PIN_5);
	//	PORTF |= 1 << 4;
	//	PORTF |= 1 << 5;
		_delay_ms(5000);
		gpio_clearValue(PORT_F, PIN_4);
		gpio_clearValue(PORT_F, PIN_5);
		motor_driveStop();
		//	PORTF &= ~(1 << 4);
	//	PORTF &= ~(1 << 5);
	}

	return 0;
}

int8_t rtcTest()
{
	int8_t status = 0;
	uint32_t currentEpoch = 0;
	uint8_t aDate[7] = { 0 };
	uint8_t epochByte[4] = { 0 };
	DateTime aDate2;
	DateTime taskDate;


	taskDate.year = 2021;
	taskDate.month = 1;
	taskDate.day = 3;
	taskDate.hours = 9;
	taskDate.minutes= 19;
	taskDate.seconds = 30;
	taskDate.dayOfWeek = 1;

	aDate2.year = 2021;
	aDate2.month = 1;
	aDate2.day = 3;
	aDate2.hours = 9;
	aDate2.minutes= 16;
	aDate2.seconds = 0;
	aDate2.dayOfWeek = 1;

	status = setRtcTime(&aDate2);

	if (status != 0)
	{
		usb_tx_string("Error ");
	}
//	else
//	{
//		usb_tx_string("OK ");
//	}
//	usb_tx_string("Stat ");
//	usb_tx_decimal(status);

	while(1)
	{
		status = getRtcTime(&aDate2);

		if (status != 0)
		{
			usb_tx_string("Error ");
		}

		if (compareDateTime(&aDate2, &taskDate))
		{
			gpio_setValue(PORT_F, PIN_4);
		}
		else
		{
			gpio_setValue(PORT_F, PIN_5);
		}
//		else
//		{
//			usb_tx_string("OK ");
//		}
//		usb_tx_string("Stat ");
//		usb_tx_decimal(status);
//
//		m_usb_tx_char(' ');
//		usb_tx_decimal(aDate2.seconds);
//		m_usb_tx_char(' ');
//		usb_tx_decimal(aDate2.minutes);
//		m_usb_tx_char(' ');
//		usb_tx_decimal(aDate2.hours);
//		m_usb_tx_char(' ');
//		usb_tx_decimal(aDate2.dayOfWeek);
//		m_usb_tx_char(' ');
//		usb_tx_decimal(aDate2.day);
//		m_usb_tx_char(' ');
//		usb_tx_decimal(aDate2.month);
//		m_usb_tx_char(' ');
//		usb_tx_decimal(aDate2.year);
//		m_usb_tx_char(' ');

		usb_tx_push();
		for (xx = 0; xx < 200; xx++)
		{
			_delay_ms(10);
		}

	}

	return 0;
}

int8_t eepromTest()
{
	usb_tx_string("EEPROM TEST \r\n");

	uint8_t registerAddress[2] = { 0 };
	int8_t status = 0;
	uint8_t data = 0xBC;
	int8_t tIdx = 0;
	uint16_t aStartAddress = 0;

	for (tIdx = 0; tIdx < TASK_SIZE; tIdx++)
	{
		gEepromBuffer[tIdx] = tIdx + 0x11;
	}

	status = eepromWrite(aStartAddress, gEepromBuffer, TASK_SIZE, &tIdx);

	if (status != 0)
	{

		usb_tx_string("eeprom error.\r\n");
	}
	else
	{
		usb_tx_string("eeprom ok \r\n");
	}

	usb_tx_decimal(status);
	usb_tx_decimal(13);
	usb_tx_hex(status);

	status = eepromRead(0x0, gEepromBufferTest, TASK_SIZE);

	if (status != 0)
	{
		usb_tx_string("EEPROM read failed. \r\n");
	}
	else
	{
		usb_tx_string("EEPROM read ok. \r\n");
	}

	Task testTask;
	memcpy(&testTask, gEepromBufferTest, TASK_SIZE);

	usb_tx_decimal(testTask.isEnabled);

	// m_usb_tx_long(testTask.startTime);
	// usb_tx_decimal(testTask.startTime);
	usb_tx_decimal(testTask.totalPond);

	usb_tx_push();

	return 0;
}
