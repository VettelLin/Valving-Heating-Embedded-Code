/*
 * u_led.h
 *
 *  Created on: Jul 25, 2025
 *      Author: admin
 */

#ifndef APP_U_LED_H_
#define APP_U_LED_H_

#include "u_gpio.h"

#define led4		gpio_c3

#define turn_led4(on)	gpio_set_status(&led4, on)
#define reverse_led4()	gpio_reverse(&led4)

#endif /* APP_U_LED_H_ */
