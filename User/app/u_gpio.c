/*
 * u_gpio.c
 *
 *  Created on: Nov 25, 2021
 *      Author: abel
 */

#include "u_gpio.h"

#ifdef GPIOA
gpio_st gpio_a0 = {GPIOA, GPIO_PIN_0, GPIO_OUTPUT, 0};
gpio_st gpio_a1 = {GPIOA, GPIO_PIN_1, GPIO_OUTPUT, 0};
gpio_st gpio_a2 = {GPIOA, GPIO_PIN_2, GPIO_OUTPUT, 0};
gpio_st gpio_a3 = {GPIOA, GPIO_PIN_3, GPIO_OUTPUT, 0};
gpio_st gpio_a4 = {GPIOA, GPIO_PIN_4, GPIO_OUTPUT, 0};
gpio_st gpio_a5 = {GPIOA, GPIO_PIN_5, GPIO_OUTPUT, 0};
gpio_st gpio_a6 = {GPIOA, GPIO_PIN_6, GPIO_OUTPUT, 0};
gpio_st gpio_a7 = {GPIOA, GPIO_PIN_7, GPIO_OUTPUT, 0};
gpio_st gpio_a8 = {GPIOA, GPIO_PIN_8, GPIO_OUTPUT, 0};
gpio_st gpio_a9 = {GPIOA, GPIO_PIN_9, GPIO_OUTPUT, 0};
gpio_st gpio_a10 = {GPIOA, GPIO_PIN_10, GPIO_OUTPUT, 0};
gpio_st gpio_a11 = {GPIOA, GPIO_PIN_11, GPIO_OUTPUT, 0};
gpio_st gpio_a12 = {GPIOA, GPIO_PIN_12, GPIO_OUTPUT, 0};
gpio_st gpio_a13 = {GPIOA, GPIO_PIN_13, GPIO_OUTPUT, 0};
gpio_st gpio_a14 = {GPIOA, GPIO_PIN_14, GPIO_OUTPUT, 0};
gpio_st gpio_a15 = {GPIOA, GPIO_PIN_15, GPIO_INPUT, 0};
#endif

#ifdef GPIOB
gpio_st gpio_b0 = {GPIOB, GPIO_PIN_0, GPIO_OUTPUT, 0};
gpio_st gpio_b1 = {GPIOB, GPIO_PIN_1, GPIO_INPUT, 0};
gpio_st gpio_b2 = {GPIOB, GPIO_PIN_2, GPIO_INPUT, 0};
gpio_st gpio_b3 = {GPIOB, GPIO_PIN_3, GPIO_OUTPUT, 0};
gpio_st gpio_b4 = {GPIOB, GPIO_PIN_4, GPIO_INPUT, 0};
gpio_st gpio_b5 = {GPIOB, GPIO_PIN_5, GPIO_INPUT, 0};
gpio_st gpio_b6 = {GPIOB, GPIO_PIN_6, GPIO_OUTPUT, 0};
gpio_st gpio_b7 = {GPIOB, GPIO_PIN_7, GPIO_OUTPUT, 0};
gpio_st gpio_b8 = {GPIOB, GPIO_PIN_8, GPIO_OUTPUT, 0};
gpio_st gpio_b9 = {GPIOB, GPIO_PIN_9, GPIO_OUTPUT, 0};
gpio_st gpio_b10 = {GPIOB, GPIO_PIN_10, GPIO_INPUT, 0};
gpio_st gpio_b11 = {GPIOB, GPIO_PIN_11, GPIO_OUTPUT, 0};
gpio_st gpio_b12 = {GPIOB, GPIO_PIN_12, GPIO_OUTPUT, 0};
gpio_st gpio_b13 = {GPIOB, GPIO_PIN_13, GPIO_OUTPUT, 0};
gpio_st gpio_b14 = {GPIOB, GPIO_PIN_14, GPIO_OUTPUT, 0};
gpio_st gpio_b15 = {GPIOB, GPIO_PIN_15, GPIO_OUTPUT, 0};
#endif


#ifdef GPIOC
gpio_st gpio_c0 = {GPIOC, GPIO_PIN_0, GPIO_OUTPUT, 0};
gpio_st gpio_c1 = {GPIOC, GPIO_PIN_1, GPIO_OUTPUT, 0};
gpio_st gpio_c2 = {GPIOC, GPIO_PIN_2, GPIO_OUTPUT, 0};
gpio_st gpio_c3 = {GPIOC, GPIO_PIN_3, GPIO_OUTPUT, 0};
gpio_st gpio_c4 = {GPIOC, GPIO_PIN_4, GPIO_OUTPUT, 0};
gpio_st gpio_c5 = {GPIOC, GPIO_PIN_5, GPIO_OUTPUT, 0};
gpio_st gpio_c6 = {GPIOC, GPIO_PIN_6, GPIO_OUTPUT, 0};
gpio_st gpio_c7 = {GPIOC, GPIO_PIN_7, GPIO_OUTPUT, 0};
gpio_st gpio_c8 = {GPIOC, GPIO_PIN_8, GPIO_OUTPUT, 0};
gpio_st gpio_c9 = {GPIOC, GPIO_PIN_9, GPIO_OUTPUT, 0};
gpio_st gpio_c10 = {GPIOC, GPIO_PIN_10, GPIO_INPUT, 0};
gpio_st gpio_c11 = {GPIOC, GPIO_PIN_11, GPIO_INPUT, 0};
gpio_st gpio_c12 = {GPIOC, GPIO_PIN_12, GPIO_OUTPUT, 0};
gpio_st gpio_c13 = {GPIOC, GPIO_PIN_13, GPIO_OUTPUT, 0};
gpio_st gpio_c14 = {GPIOC, GPIO_PIN_14, GPIO_OUTPUT, 0};
gpio_st gpio_c15 = {GPIOC, GPIO_PIN_15, GPIO_OUTPUT, 0};
#endif


#ifdef GPIOD
gpio_st gpio_d0 = {GPIOD, GPIO_PIN_0, GPIO_OUTPUT, 0};
gpio_st gpio_d1 = {GPIOD, GPIO_PIN_1, GPIO_OUTPUT, 0};
gpio_st gpio_d2 = {GPIOD, GPIO_PIN_2, GPIO_OUTPUT, 0};
gpio_st gpio_d3 = {GPIOD, GPIO_PIN_3, GPIO_OUTPUT, 0};
gpio_st gpio_d4 = {GPIOD, GPIO_PIN_4, GPIO_OUTPUT, 0};
gpio_st gpio_d5 = {GPIOD, GPIO_PIN_5, GPIO_OUTPUT, 0};
gpio_st gpio_d6 = {GPIOD, GPIO_PIN_6, GPIO_OUTPUT, 0};
gpio_st gpio_d7 = {GPIOD, GPIO_PIN_7, GPIO_OUTPUT, 0};
gpio_st gpio_d8 = {GPIOD, GPIO_PIN_8, GPIO_OUTPUT, 0};
gpio_st gpio_d9 = {GPIOD, GPIO_PIN_9, GPIO_OUTPUT, 0};
gpio_st gpio_d10 = {GPIOD, GPIO_PIN_10, GPIO_OUTPUT, 0};
gpio_st gpio_d11 = {GPIOD, GPIO_PIN_11, GPIO_OUTPUT, 0};
gpio_st gpio_d12 = {GPIOD, GPIO_PIN_12, GPIO_OUTPUT, 0};
gpio_st gpio_d13 = {GPIOD, GPIO_PIN_13, GPIO_OUTPUT, 0};
gpio_st gpio_d14 = {GPIOD, GPIO_PIN_14, GPIO_OUTPUT, 0};
gpio_st gpio_d15 = {GPIOD, GPIO_PIN_15, GPIO_OUTPUT, 0};
#endif



#ifdef GPIOE
gpio_st gpio_e0 = {GPIOE, GPIO_PIN_0, GPIO_OUTPUT, 0};
gpio_st gpio_e1 = {GPIOE, GPIO_PIN_1, GPIO_OUTPUT, 0};
gpio_st gpio_e2 = {GPIOE, GPIO_PIN_2, GPIO_OUTPUT, 0};
gpio_st gpio_e3 = {GPIOE, GPIO_PIN_3, GPIO_OUTPUT, 0};
gpio_st gpio_e4 = {GPIOE, GPIO_PIN_4, GPIO_OUTPUT, 0};
gpio_st gpio_e5 = {GPIOE, GPIO_PIN_5, GPIO_OUTPUT, 0};
gpio_st gpio_e6 = {GPIOE, GPIO_PIN_6, GPIO_OUTPUT, 0};
gpio_st gpio_e7 = {GPIOE, GPIO_PIN_7, GPIO_OUTPUT, 0};
gpio_st gpio_e8 = {GPIOE, GPIO_PIN_8, GPIO_OUTPUT, 0};
gpio_st gpio_e9 = {GPIOE, GPIO_PIN_9, GPIO_OUTPUT, 0};
gpio_st gpio_e10 = {GPIOE, GPIO_PIN_10, GPIO_OUTPUT, 0};
gpio_st gpio_e11 = {GPIOE, GPIO_PIN_11, GPIO_OUTPUT, 0};
gpio_st gpio_e12 = {GPIOE, GPIO_PIN_12, GPIO_OUTPUT, 0};
gpio_st gpio_e13 = {GPIOE, GPIO_PIN_13, GPIO_OUTPUT, 0};
gpio_st gpio_e14 = {GPIOE, GPIO_PIN_14, GPIO_OUTPUT, 0};
gpio_st gpio_e15 = {GPIOE, GPIO_PIN_15, GPIO_OUTPUT, 0};
#endif


void init_gpios(void)
{

}
