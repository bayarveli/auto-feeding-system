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
#define DEBUG					0


#include "board.h"
#include "util/delay.h"
#include "m_usb.h"
#include "eepromapi.h"
#include "motorapi.h"
#include "message.h"
#include "taskInfo.h"
#include "rtcapi.h"
#include "counterapi.h"
#include "esp32.h"
#include "ledapi.h"
#include "timer.h"

// ENUM
typedef enum
{
	eSYS_INIT,
	eSYS_FAIL,
	eSYS_READ_TASK,
	eSYS_WAIT_TASK,
	eSYS_START_TASK,
	eSYS_FINISH_TASK,
	eSYS_ABORT_TASK,
	eSYS_CONFIG,
	eSYS_IDLE,
} SystemState;

typedef struct
{
	uint8_t isDriveEnabled;
	uint8_t isDriveMotorStarted;
	uint8_t isFeedingEnabled;
	uint8_t isFeedingMotorStarted;
	uint8_t switchStatus;
	uint32_t totalMoveTime;
} StateFlag;

// VARIABLES
static SystemState gSystemState = eSYS_INIT;
static uint8_t gEepromBuffer[TASK_SIZE];
uint8_t gEspResponseBuffer[8];
uint8_t gMsgBuf[64];
uint8_t gMsgBufLen = 0;

CommStatus espCommStatus = eDISCONNECTED;

//
void delaySecond(uint16_t aSecDiv100)
{
	int idx = 0;
	for (idx = 0; idx < aSecDiv100; idx++)
	{
		_delay_ms(10);
	}
}

// MAIN FUNCTION
int main(void)
{
	int8_t status = 0;
	Task currentTask;
	uint8_t taskIdx = 0;
	uint16_t taskInfoAddr = 0;
	uint8_t pondIdx = 0;
	DateTime currTime;
	DateTime setTime;
	uint8_t previousDay;
	StateFlag operationFlags;

	uint16_t startAddress = 0;
	int8_t tt = 0;

	// SYSTEM INITIALIZATION
    board_init();
    led_init();
    switch_init();
    motor_init();
    esp32_init();
    eepromInit();
    rtcInit();
    initCounter();
    sei();

#if DEBUG
    m_usb_init();
	while (!m_usb_isconnected())
		; // wait for a connection
#endif

	led_changeState(eGREEN, eLEDBLINK250MS);
	led_changeState(eBLUE, eLEDBLINK250MS);

	delaySecond(1500);

	led_changeState(eGREEN, eLEDOFF);
	led_changeState(eBLUE, eLEDOFF);

	motor_driveSetMotorSpeed(255);
	motor_feedingSetMotorSpeed(255);

	while (1)
	{
		esp32_isConnected(&espCommStatus);

		if (espCommStatus == eCONNECTED)
		{
			led_changeState(eGREEN, eLEDBLINK500MS);
			esp32_isDataReceived(gMsgBuf, &gMsgBufLen);

			if (gMsgBufLen > 0)
			{
				uint8_t messageId = gMsgBuf[0];

				switch(messageId)
				{
				case MSG_ID_START_CONFIG:
					if ((gSystemState != eSYS_FINISH_TASK) && (gSystemState != eSYS_START_TASK))
					{
							gSystemState = eSYS_CONFIG;
							led_changeState(eBLUE, eLEDON);
					}
					break;
				case MSG_ID_END_CONFIG:
					gSystemState = eSYS_READ_TASK;
					led_changeState(eBLUE, eLEDOFF);
					break;
				case MSG_ID_TASK_INFO:
					if (gSystemState == eSYS_CONFIG)
					{
						gEspResponseBuffer[0] = MSG_ID_TASK_INFO_RESP;
						startAddress |= gMsgBuf[1] << 8;
						startAddress |= gMsgBuf[2];
						status = eepromWrite(startAddress, &gMsgBuf[3], TASK_SIZE);

						if (status != 0)
						{
							gSystemState = eSYS_FAIL;
							gEspResponseBuffer[1] = 0x22;
							led_changeState(eGREEN, eLEDBLINK250MS);
							// break;
						}
						else
						{
							gEspResponseBuffer[1] = 0x11;
						}


						esp32_sendMessage(gEspResponseBuffer, 2);
						taskIdx = 0;
					}
					break;
				case MSG_ID_ABORT_TASK:
					if (gSystemState != eSYS_ABORT_TASK)
					{
						if (gSystemState == eSYS_START_TASK)
						{
							gSystemState = eSYS_ABORT_TASK;
						}
					}
					break;

				case MSG_ID_SET_TIME:
					memcpy(&setTime, &gMsgBuf[1], sizeof(setTime));
					status = setRtcTime(&setTime);
					if (status != 0)
					{
						led_changeState(eGREEN, eLEDBLINK500MS);
						gSystemState = eSYS_FAIL;
						break;
					}

					break;
				case MSG_ID_GET_TIME:
					// TODO: When requested, send current rtc time
					status = getRtcTime(&setTime);

					if (status != 0)
					{
						led_changeState(eGREEN, eLEDBLINK1000MS);
						gSystemState = eSYS_FAIL;
						break;
					}
#if DEBUG
					usb_tx_decimal(setTime.year);
					m_usb_tx_char(' ');
					usb_tx_decimal(setTime.month);
					m_usb_tx_char(' ');
					usb_tx_decimal(setTime.day);
					m_usb_tx_char(' ');
					usb_tx_decimal(setTime.hours);
					m_usb_tx_char(' ');
					usb_tx_decimal(setTime.minutes);
					m_usb_tx_char(' ');
					usb_tx_decimal(setTime.seconds);
					m_usb_tx_char(' ');
					usb_tx_decimal(setTime.dayOfWeek);
					m_usb_tx_char(' ');
#endif
					break;
				case MSG_ID_GET_STATE:
					// TODO: When requested, send gSystemState value.
					break;
				case MSG_ID_DRIVE_FRWD:
					if (gSystemState == eSYS_CONFIG)
					{
						motor_driveMoveForward();
					}
					break;
				case MSG_ID_DRIVE_BCWD:
					if (gSystemState == eSYS_CONFIG)
					{
						motor_driveMoveBackward();
					}
					break;
				case MSG_ID_DRIVE_STOP:
					if (gSystemState == eSYS_CONFIG)
					{
						motor_driveStop();
					}
					break;
				case MSG_ID_DRIVE_SET_SPEED:
					if (gSystemState == eSYS_CONFIG)
					{
						motor_driveSetMotorSpeed(gMsgBuf[1]);
					}
					break;
				case MSG_ID_FEED_START:
					if (gSystemState == eSYS_CONFIG)
					{
						motor_feedingMoveForward();
					}
					break;
				case MSG_ID_FEED_STOP:
					if (gSystemState == eSYS_CONFIG)
					{
						motor_feedingStop();
					}
					break;
				case MSG_ID_FEED_SET_SPEED:
					if (gSystemState == eSYS_CONFIG)
					{
						motor_feedingSetMotorSpeed(gMsgBuf[1]);
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
			led_changeState(eGREEN, eLEDON);
		}
		else
		{
			// Nothing
		}

		switch (gSystemState)
		{
		case eSYS_INIT:

			if (esp32_isReady() > 0)
			{
				//! ESP initialization is OK
			}
			else
			{
				//! ESP initialization error
				led_changeState(eBLUE, eLEDBLINK250MS);
				gSystemState = eSYS_FAIL;
				break;
			}

			gSystemState = eSYS_READ_TASK;

			// TODO: Can be added retry count for esp32
			break;
		case eSYS_READ_TASK:
			if (taskIdx > MAX_TASK_COUNT)
			{
				gSystemState = eSYS_IDLE;
				previousDay = currTime.day;
				taskIdx = 0;
				break;
			}

			// Operation flags is assigned to their initial value for each task
			operationFlags.isDriveEnabled = 1;
			operationFlags.isDriveMotorStarted = 0;
			operationFlags.isFeedingEnabled = 0;
			operationFlags.isFeedingMotorStarted = 0;
			operationFlags.switchStatus = 0;
			operationFlags.totalMoveTime = 0;

			// Calculate the task info address
			taskInfoAddr = TASK_START_ADDR + (taskIdx * TASK_SIZE);

			// Read the task info
			status = eepromRead(taskInfoAddr, gEepromBuffer, TASK_SIZE);

			if (status != 0)
			{
				led_changeState(eBLUE, eLEDBLINK500MS);
				gSystemState = eSYS_FAIL;
				break;
			}

			// Transform array to task structure
			memcpy(&currentTask, gEepromBuffer, TASK_SIZE);

			if (currentTask.isEnabled == 1)
			{
				gSystemState = eSYS_WAIT_TASK;
			}
			else if (currentTask.isEnabled == 0)
			{
				// If the task is disabled, go for the next task
				gSystemState = eSYS_READ_TASK;
			}
			else
			{
				// If the task is invalid, change state to SYSCONFIG
				gSystemState = eSYS_CONFIG;
			}

			taskIdx++;
			break;
		case eSYS_WAIT_TASK:
			status = getRtcTime(&currTime);

			if (status != 0)
			{
				led_changeState(eBLUE, eLEDBLINK1000MS);
				gSystemState = eSYS_FAIL;
				break;
			}

			if (compareDateTime(&currTime, &currentTask.startTime) == 1)
			{
				gSystemState = eSYS_START_TASK;
			}

			delaySecond(100);
			break;
		case eSYS_START_TASK:
			if (pondIdx >= currentTask.totalPond)
			{
				gSystemState = eSYS_FINISH_TASK;
				break;
			}

			if (1 == operationFlags.isDriveEnabled)
			{
				if (0 == operationFlags.isDriveMotorStarted) {
					motor_driveMoveForward();

					// TODO: relate pond idx and time
					// TODO: Handler if pondTime is 0
					setTimeout(currentTask.pondList[pondIdx].pondTime);
					operationFlags.isDriveMotorStarted = 1;
					operationFlags.totalMoveTime += currentTask.pondList[pondIdx].pondTime;
				}

				if (getTimeoutStatus() == 1) {
					clearTimeout();
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
					// TODO: Handle if feedingTime is 0
					setTimeout(currentTask.pondList[pondIdx].feedingTime);
					operationFlags.isFeedingMotorStarted = 1;
				}

				if (getTimeoutStatus())
				{
					clearTimeout();
					motor_feedingStop();
					operationFlags.isFeedingEnabled = 0;
					operationFlags.isDriveEnabled = 1;
					operationFlags.isDriveMotorStarted = 0;
					operationFlags.isFeedingMotorStarted = 0;
					pondIdx++;
				}
			}

			break;
		case eSYS_FINISH_TASK:
			if (0 == operationFlags.isDriveMotorStarted)
			{
				motor_driveMoveBackward();
				// TODO: relate pond idx and time
				// TODO: check total move time can not overflow
				setTimeout(operationFlags.totalMoveTime);
				operationFlags.isDriveMotorStarted = 1;
			}

			getSwitchStatus(&operationFlags.switchStatus);

			if ((getTimeoutStatus()) || (operationFlags.switchStatus))
			{
				clearTimeout();

				if (operationFlags.switchStatus)
				{
					motor_driveStop();
					gSystemState = eSYS_READ_TASK;
				}
			}
			break;
		case eSYS_ABORT_TASK:
			// TODO: run motor backward
			break;
		case eSYS_IDLE:
			// Check time and wait for the next day.
			// TODO: MCU can sleep in given periods. Think about that.
			status = getRtcTime(&currTime);
			if (status != 0)
			{
				led_changeState(eBLUE, eLEDBLINK1000MS);
				gSystemState = eSYS_FAIL;
				break;
			}

			if (previousDay != currTime.day)
			{
				gSystemState = eSYS_READ_TASK;
			}

			previousDay = currTime.day;

			// wait for 5 secs.
			delaySecond(500);

			break;
		case eSYS_CONFIG:
			break;
		case eSYS_FAIL:
			return 0;
			break;
		default:
			break;
		}
	}

	return 0;
}

