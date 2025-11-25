/* 
 * File:   thermistor.h
 * Author: atila
 *
 * Created on November 22, 2024, 2:23 PM
 */

#ifndef THERMISTOR_H
#define THERMISTOR_H


#include "heat.h"
#include "lib/l_adc.h"

extern int poll_tc_temperature;

#define HEAT_TC_TEMPERATURE(tc)		(tc).current_temperature
void get_temperatures();

int thermistor_adc_handler(adc_st* adc, void *priv);
// Reads the ADC value from the specified thermistor channel
//unsigned int read_thermistor(unsigned char idx);

// Converts the ADC value to a temperature in Celsius
float convert_adc_to_temp(unsigned int adc_value);
void dock_get_temperature(heat_dock_st *dock, int ch);

float get_temperature(int ch);

#endif

