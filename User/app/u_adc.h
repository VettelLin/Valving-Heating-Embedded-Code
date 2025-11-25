/*
 * u_adc.h
 *
 *  Created on: Sep 6, 2021
 *      Author: abel
 */

#ifndef INC_U_ADC_H_
#define INC_U_ADC_H_

#include "lib/l_adc.h"

void setup_adc(void);

void do_start_adc(void);
void do_stop_adc(void);

void pool_multichan_adc(void);

extern adc_st adc1;

#endif /* INC_U_ADC_H_ */
