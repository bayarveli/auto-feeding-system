/*
 * board.c
 *
 *  Created on: Nov 22, 2020
 *      Author: veli-
 */

#include "board.h"

void board_init()
{
	// NOTE: No need to init pin direction for IIC and USART

	// CONFIGURE PIN DIRECTIONS
	gpio_setDir(PORT_F, PIN_4, OUT); // User LED-1
	gpio_setDir(PORT_F, PIN_5, OUT); // User LED-2

}
