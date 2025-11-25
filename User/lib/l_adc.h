/*
 * u_adc.h
 *
 *  Created on: Sep 6, 2021
 *      Author: abel
 */

#ifndef INC_L_ADC_H_
#define INC_L_ADC_H_

#include "adc.h"

#include "algorithm/filter/ad_filter.h"


typedef struct{
	uint16_t  *data_buff;
	uint16_t buff_size;		// the size of data buffer
	uint16_t data_count;	// data count in data buffer
	uint16_t value;		// final value;
	ad_filter_st *filter;
	void *priv;
}adc_channel_st;

typedef struct adc_typeDef adc_st;
struct adc_typeDef{
	ADC_HandleTypeDef* hadc;
	uint16_t  *data_buff;
	uint16_t buff_size;
	uint16_t conv_count;
	adc_channel_st *regular_channels;
	adc_channel_st *injected_channels;
	uint8_t		reg_chanqty;
	uint8_t		inj_chanqty;
	uint8_t		resolution;
	uint8_t	do_flag;
	float  vref_top;
	float  vref_low;
	void *priv;
	int (*cb_func)(adc_st* adc, void *priv);
};

#define adc_start(hadcx)	HAL_ADC_Start(hadcx)
#define adc_stop(hadcx)	HAL_ADC_Stop(hadcx)
#define adc_get_value(hadcx)	HAL_ADC_GetValue(hadcx)
#define adc_get_state(hadcx)		HAL_ADC_GetState(hadcx)
#define adc_waitfor_conversion(hadcx)		HAL_ADC_PollForConversion(hadcx, 100)

#define adc_is_convert_ok(hadcx)		HAL_IS_BIT_SET(HAL_ADC_GetState(hadcx),HAL_ADC_STATE_REG_EOC)

#define adc_start_it(hadcx)		HAL_ADC_Start_IT(hadcx)
#define adc_stop_it(hadcx)		HAL_ADC_Stop_IT(hadcx)

#define adc_start_dma(hadcx, pdata, len)		HAL_ADC_Start_DMA(hadcx, pdata, len)
#define adc_stop_dma(hadcx)		HAL_ADC_Stop_DMA(hadcx)



#define adc_injected_start(hadc)	 HAL_ADCEx_InjectedStart(hadc)
#define adc_injected_stop(hadc)	 	HAL_ADCEx_InjectedStop(hadc)

#define adc_injected_start_it(hadc)		 HAL_ADCEx_InjectedStart_IT(hadc)
#define adc_injected_stop_it(hadc)		 HAL_ADCEx_InjectedStop_IT(hadc)

//HAL_StatusTypeDef HAL_ADCEx_InjectedPollForConversion(ADC_HandleTypeDef* hadc, uint32_t Timeout);
//HAL_StatusTypeDef HAL_ADCEx_MultiModeStart_DMA(ADC_HandleTypeDef* hadc, uint32_t* pData, uint32_t Length);
//HAL_StatusTypeDef HAL_ADCEx_MultiModeStop_DMA(ADC_HandleTypeDef* hadc);
//uint32_t HAL_ADCEx_MultiModeGetValue(ADC_HandleTypeDef* hadc);



//#define adc_get_injectedVal(hadcx, rank)		HAL_ADCEx_InjectedGetValue(hadcx, rank)
extern uint32_t adc_get_injectedVal(ADC_HandleTypeDef *hadcx, int rank);
extern float adc_val2volt(adc_st *adc, uint16_t val);



#endif /* INC_L_ADC_H_ */
