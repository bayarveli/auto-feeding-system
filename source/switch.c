/*
 * switch.c
 *
 *  Created on: Dec 26, 2020
 *      Author: veli-
 */

#include "switch.h"
#include "gpio.h"

void getSwitchStatus(uint8_t *aStatus)
{
	*aStatus = gpio_readValue(PORT_B, PIN_0);
}

void switch_init()
{
	gpio_setDir(PORT_B, PIN_0, IN); // Slide Switch
}
