/*
 * test.c
 *
 *  Created on: Jan 9, 2021
 *      Author: veli-
 */

// Test functions
int8_t driveMotorTest();
int8_t rtcTest();
int8_t eepromTest();
int8_t counterTest();

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
	int8_t xx;


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

int8_t counterTest()
{
	setTimeout(5);

	while(1)
	{

		if (getTimeoutStatus() == 1)
		{
			usb_tx_string("Get Timeout Stat");
			clearTimeout();
			setTimeout(5);
		}
	}

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

	status = eepromWrite(aStartAddress, gEepromBuffer, TASK_SIZE);

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
