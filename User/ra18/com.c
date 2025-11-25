/*
 * com.c
 *
 *  Created on: May 11, 2025
 *      Author: abel
 */


#include "com.h"

uint8_t com_id;

#include "app/u_gpio.h"


uint8_t get_com_id(void)
{
	uint8_t id1, id2;

	id1 = gpio_get_status(&gpio_c10);
	id2 = gpio_get_status(&gpio_c11);

	com_id = (id1 << 1) | id2;

	return com_id;
}
