/*
 * systime.h
 *
 *  Created on: May 11, 2025
 *      Author: abel
 */

#ifndef LIB_SYSTIME_H_
#define LIB_SYSTIME_H_


#include "l_timer.h"

//#define get_timestamp_ms()	HAL_GetTick()
#define get_timestamp_ms()	get_delaytimer_timestamp_ms()


#endif /* LIB_SYSTIME_H_ */
