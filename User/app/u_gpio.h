/*
 * u_gpio.h
 *
 *  Created on: Nov 25, 2021
 *      Author: abel
 */

#ifndef INC_U_GPIO_H_
#define INC_U_GPIO_H_

#include "lib/l_gpio.h"

#ifdef GPIOA
extern gpio_st gpio_a0;
extern gpio_st gpio_a1;
extern gpio_st gpio_a2;
extern gpio_st gpio_a3;
extern gpio_st gpio_a4;
extern gpio_st gpio_a5;
extern gpio_st gpio_a6;
extern gpio_st gpio_a7;
extern gpio_st gpio_a8;
extern gpio_st gpio_a9;
extern gpio_st gpio_a10;
extern gpio_st gpio_a11;
extern gpio_st gpio_a12;
extern gpio_st gpio_a13;
extern gpio_st gpio_a14;
extern gpio_st gpio_a15;
#endif

#ifdef GPIOB
extern gpio_st gpio_b0;
extern gpio_st gpio_b1;
extern gpio_st gpio_b2;
extern gpio_st gpio_b3;
extern gpio_st gpio_b4;
extern gpio_st gpio_b5;
extern gpio_st gpio_b6;
extern gpio_st gpio_b7;
extern gpio_st gpio_b8;
extern gpio_st gpio_b9;
extern gpio_st gpio_b10;
extern gpio_st gpio_b11;
extern gpio_st gpio_b12;
extern gpio_st gpio_b13;
extern gpio_st gpio_b14;
extern gpio_st gpio_b15;
#endif


#ifdef GPIOC
extern gpio_st gpio_c0;
extern gpio_st gpio_c1;
extern gpio_st gpio_c2;
extern gpio_st gpio_c3;
extern gpio_st gpio_c4;
extern gpio_st gpio_c5;
extern gpio_st gpio_c6;
extern gpio_st gpio_c7;
extern gpio_st gpio_c8;
extern gpio_st gpio_c9;
extern gpio_st gpio_c10;
extern gpio_st gpio_c11;
extern gpio_st gpio_c12;
extern gpio_st gpio_c13;
extern gpio_st gpio_c14;
extern gpio_st gpio_c15;
#endif


#ifdef GPIOD
extern gpio_st gpio_d0;
extern gpio_st gpio_d1;
extern gpio_st gpio_d2;
extern gpio_st gpio_d3;
extern gpio_st gpio_d4;
extern gpio_st gpio_d5;
extern gpio_st gpio_d6;
extern gpio_st gpio_d7;
extern gpio_st gpio_d8;
extern gpio_st gpio_d9;
extern gpio_st gpio_d10;
extern gpio_st gpio_d11;
extern gpio_st gpio_d12;
extern gpio_st gpio_d13;
extern gpio_st gpio_d14;
extern gpio_st gpio_d15;
#endif



#ifdef GPIOE
extern gpio_st gpio_e0;
extern gpio_st gpio_e1;
extern gpio_st gpio_e2;
extern gpio_st gpio_e3;
extern gpio_st gpio_e4;
extern gpio_st gpio_e5;
extern gpio_st gpio_e6;
extern gpio_st gpio_e7;
extern gpio_st gpio_e8;
extern gpio_st gpio_e9;
extern gpio_st gpio_e10;
extern gpio_st gpio_e11;
extern gpio_st gpio_e12;
extern gpio_st gpio_e13;
extern gpio_st gpio_e14;
extern gpio_st gpio_e15;
#endif

extern void init_gpios(void);


#endif /* INC_U_GPIO_H_ */
