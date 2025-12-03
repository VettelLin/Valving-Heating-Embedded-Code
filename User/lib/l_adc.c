/*
 * u_adc.c
 *
 *  Created on: Sep 6, 2021
 *      Author: abel
 */



#include "l_adc.h"

float adc_val2volt(adc_st *adc, uint16_t val)
{
	return val * (adc->vref_top - adc->vref_low) / ((1 << adc->resolution)) + adc->vref_low;
}


uint32_t adc_get_injectedVal(ADC_HandleTypeDef *hadcx, int rank)
{
//	switch(rank){
//		case 1:
//			rank = ADC_INJECTED_RANK_1;
//			break;
//		case 2:
//			rank = ADC_INJECTED_RANK_2;
//			break;
//		case 3:
//			rank = ADC_INJECTED_RANK_3;
//			break;
//		case 4:
//			rank = ADC_INJECTED_RANK_4;
//			break;
//		default :
//			rank = ADC_INJECTED_RANK_1;
//			break;
//
//	}

	return HAL_ADCEx_InjectedGetValue(hadcx, rank);
}
