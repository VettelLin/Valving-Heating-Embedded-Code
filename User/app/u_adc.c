/*
 * u_adc.c
 *
 *  Created on: Sep 6, 2021
 *      Author: abel
 */

#include "porting.h"
#include "usize.h"

#include "u_adc.h"

#include "lib/l_timer.h"
#include "lib/l_uart.h"
#include "utils/l_string.h"


#include "u_gpio.h"

#include "ra18/thermistor.h"


#define SHAKE_AMP       20
#define SHAKE_TIMES     4

#define ADC1_REGCHANNEL_QTY		6
#define ADC1_CHANNEL_BUFFSIZE	8
#define ADC1_DATA_BUFFSIZE	(ADC1_CHANNEL_BUFFSIZE * ADC1_REGCHANNEL_QTY)

//static uint16_t adc1_regchannels_buff[ADC1_REGCHANNEL_QTY][ADC1_CHANNEL_BUFFSIZE];


static uint16_t adc1_databuff[ADC1_DATA_BUFFSIZE];



ad_filter_st adc1_filter = {
		.shake_amp = SHAKE_AMP,
		.shake_times = SHAKE_TIMES,
		.handle = default_ad_filter_handler,
		.value = 0
};

adc_channel_st adc1_regChannels[ADC1_REGCHANNEL_QTY];


adc_st adc1 = {
		.hadc = &hadc1,
		.data_buff = adc1_databuff,
		.buff_size = ADC1_DATA_BUFFSIZE,
		.conv_count = 0,
		.regular_channels = adc1_regChannels,
		.injected_channels = NULL,
		.reg_chanqty = ADC1_REGCHANNEL_QTY,
		.inj_chanqty = 0,
		.resolution = 12,
		.do_flag = 0,
		.vref_top = 3.3,
		.vref_low = 0,
		.priv = NULL,
		.cb_func = NULL,
//		.cb_func = thermistor_adc_handler,
};

void init_adc(void)
{
	int i;
	adc_channel_st *adc_channel;

	for(i = 0; i < ADC1_REGCHANNEL_QTY; i++){
		adc_channel = &adc1_regChannels[i];
//		adc_channel->data_buff = &adc1_regchannels_buff[i];
		adc_channel->buff_size = ADC1_CHANNEL_BUFFSIZE;
		adc_channel->data_count = 0;
		adc_channel->value = 0;
		adc_channel->filter = &adc1_filter;
		adc_channel->priv = NULL;
	}
}

void setup_adc(void)
{

}

void do_start_adc(void)
{

	if(adc1.do_flag){
		return;
	}

	adc1.do_flag = 1;

//	adc_injected_start(&hadc2);
//	adc_injected_start_it(&hadc3);

	adc_start_dma(adc1.hadc, adc1.data_buff, adc1.buff_size * sizeof(adc1.data_buff[0]));

//	printfln("start adc1 ...");
}

void do_stop_adc(void)
{
//	adc_injected_stop_it(&hadc3);
	adc1.do_flag = 0;
	adc_stop_dma(adc1.hadc);

	printfln("stop adc1 ...");
}




void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	int i, idx;

	uint32_t val, vals[4];


//	for(i=0; i<adc1.injected_channels; i++){
//		vals[i] = adc_get_injectedVal(hadc, i+1);
//	}


#if 1
//	console_printx(fln, "adc injected value 1 : %u", adc_get_injectedVal(hadc, 1));
//	console_printx(fln, "adc injected value 2 : %u", adc_get_injectedVal(hadc, 2));

	return;
#endif


//	adc_injected_stop(hadc);




}


uint16_t ad_filter_handler(uint16_t *vals, int qty)
{
	int i;
	uint8_t v_count;	// normal value count;
		// shacked value count;
	uint32_t v_sum;		// normal values sum
	uint16_t min_val = 65535, max_val = 0;	// shacked values sum




	v_count = 0;
	v_sum = 0;


	for (i = 0; i < qty; i++) {
		if(vals[i] < min_val ){
			min_val = vals[i];
		}
		if(vals[i] > max_val ){
			max_val = vals[i];
		}
	   v_count++;
	   v_sum += vals[i];
//	   printf("%u ", vals[i]);
	}
//	printfln(NULL);

	v_sum -= min_val;
	v_sum -= max_val;
	v_count -= 2;


	return (v_sum / v_count);
}

void filter_adc_channel(adc_channel_st *ad_chan)
{
#if 1
		ad_chan->value = ad_filter_handler(ad_chan->data_buff, ad_chan->buff_size);
#else
	if(ad_chan && ad_chan->filter && ad_chan->filter->handle){
		ad_chan->value = ad_chan->filter->handle(ad_chan->filter, ad_chan->data_buff, ad_chan->buff_size, ad_chan->priv);
	}
#endif
}


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	int i, j, offs = 0;
	uint16_t *pdata;
	adc_channel_st *ad_chan;
	uint16_t databuff[ADC1_CHANNEL_BUFFSIZE];


//	do_stop_adc();

//	printfln("adc ConvCpltCallback");/

//	adc1.do_flag = 2;

//	return;

	if(hadc->Instance == adc1.hadc->Instance){

//		adc1.conv_count++;


#if 1

		for(i = 0; i<adc1.reg_chanqty; i++){
			ad_chan = &adc1.regular_channels[i];
			for(j=0; j<ADC1_CHANNEL_BUFFSIZE; j++){
				databuff[j] = adc1.data_buff[j*adc1.reg_chanqty + i];
			}
			ad_chan->value = ad_filter_handler(databuff, ADC1_CHANNEL_BUFFSIZE);
		}

#endif

#if 0
		for(i=0; i<adc1.buff_size; i++){
			if(i>0 && i%adc1.reg_chanqty == 0){
				printf(EndCharsOfLine);
			}
			printf("%u ", adc1.data_buff[i]);
		}

		printf(EndCharsOfLine);
#endif

#if 0
		for(i = 0; i<adc1.reg_chanqty; i++){
			ad_chan = &adc1.regular_channels[i];
			printfln("ad channel[%d] : val %u, volt %f", i, ad_chan->value, adc_val2volt(&adc1, ad_chan->value));
		}
#endif

		adc1.do_flag = 2;

		if(adc1.cb_func){
			adc1.cb_func(&adc1, adc1.priv);
		}

//		do_stop_adc();

		adc_stop_dma(adc1.hadc);
	}

//	do_start_adc();

}
