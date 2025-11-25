
#include <math.h>
#include "thermistor.h"

#include "app/u_adc.h"
#include "lib/l_uart.h"

#define ADC_PREF_VOLT 3.3

// Function to convert ADC value to temperature using Steinhart-Hart equation
float convert_adc_to_temp(unsigned int adc_value) {
    const float V_in = ADC_PREF_VOLT;      // Supply voltage
    const int ADC_RESOLUTION = 4096;
    const float R_fixed = 10000; // Fixed resistor value (10k?)

//    float V_out = (adc_value / (float)ADC_RESOLUTION) * V_in;
    float V_out = adc_val2volt(&adc1, adc_value);
    
    float R_thermistor = R_fixed * (V_out / (V_in - V_out));

    const float A = 0.0007821069f;
    const float B = 0.0002711233f;
    const float C = 0.00000009166338f;
    const float Ka = 273.15f;

    float lnR = log(R_thermistor);
    float temp_kelvin = 1.0f / (A + (B * lnR) + (C * lnR * lnR * lnR));
    
    return temp_kelvin - Ka; // Convert Kelvin to Celsius
}

// Function to convert ADC value to temperature using Steinhart-Hart equation
float convert_volt_to_temp(float V_out) {
    const float V_in = ADC_PREF_VOLT;      // Supply voltage
    const int ADC_RESOLUTION = 4096;
    const float R_fixed = 10000; // Fixed resistor value (10k?)


    float R_thermistor = R_fixed * (V_out / (V_in - V_out));

    const float A = 0.0007821069f;
    const float B = 0.0002711233f;
    const float C = 0.00000009166338f;
    const float Ka = 273.15f;

    float lnR = log(R_thermistor);
    float temp_kelvin = 1.0f / (A + (B * lnR) + (C * lnR * lnR * lnR));

    return temp_kelvin - Ka; // Convert Kelvin to Celsius
}


float MYLOG(float a)
{
    int N = 1000; //我们取了前15+1项来估算
    int k, nk;
    float x, xx, y;

    x = (a - 1) / (a + 1);
    xx = x * x;
    nk = 2 * N + 1;
    y = 1.0 / nk;
    for (k = N; k > 0; k--)
    {
        nk = nk - 2;
        y = 1.0 / nk + xx * y;

    }

    return 2.0 * x * y;

}


/**
 * @brief	ADC的电压转换为温度
 * @param[in]  电压.
 * @return     温度.
 */
float adc_volt2temp(float volt)
{

	 float Rt ;//Rt是热敏电阻在所求温度下的标称阻值 :
	//电压对应电阻，电阻对应温度，

	 Rt = (volt * 20000)/(adc1.vref_top - volt);

	 float Rp=10000.0; //10K
	 float T2 =298.15; //T2=(273.15+25Rt 是热敏电阳在T1温度下的阴值:
	 float Bx = 3435.0;//B3435.0
	 float Ka = 273.15;
	 //float RX ;
	 float temp;

	temp = 1 / ((1 / Bx) * log(Rt/Rp) +( 1 / T2));//1/(((MYLOG(Rt/Rp))/Bx) + (1/T2));

	temp = (temp - Ka + 0.5);

	return temp;
}

#if 1

uint8_t adc_index[16] = {
		[3] = 0,
		[5] = 1,
		[6] = 2,
		[10] = 3,
		[12] = 4,
		[14] = 5,
};

#else
uint8_t adc_index[16] = {
		[2] = 0,
		[3] = 1,
		[4] = 2,
		[5] = 3,
		[6] = 4,
		[7] = 5,
		[8] = 6,
		[10] = 7,
		[11] = 8,
		[12] = 9,
		[14] = 10,
		[15] = 11,
};

#endif


void get_temperatures(void)
{

//	printfln("--------get_temperature : %d ----", cd_idx);

	do_start_adc();
	while(adc1.do_flag != 2){

	}
	adc1.do_flag = 0;
//	printfln("--------get_temperatures :  ----");
}


float get_temperature(int ch)
{
	float volt, temp;
	volt = adc_val2volt(&adc1, adc1.regular_channels[adc_index[ch]].value);
	temp = convert_volt_to_temp(volt);
//	temp = adc_volt2temp(volt);
//	printf("get_temperature : volt %f, temp %f\r\n", volt, temp);
	return temp;
}


void dock_get_temperature(heat_dock_st *dock, int ch)
{
	float volt;
	heat_channel_st *tc;

	if(ch >= NUM_DOCKS){
		return;
	}
	tc = dock->tcs[ch];
	if(tc->thermistor_channel < 0x20){
		tc->last_temperature = tc->current_temperature;
		tc->current_temperature = get_temperature(tc->thermistor_channel);
	}

//		heat_tc->current_temperature = convert_adc_to_temp(adc1.regular_channels[i].value);
//		printfln("get tc[%d] temperature : adval = %#.4x, %f --- %f"

}


int thermistor_adc_handler(adc_st* adc, void *priv)
{
	int i, ch;
	float volt;
	heat_channel_st *tc;

	for(i=0; i<NUM_DOCKS; i++){

		for (ch = 0; ch < NUM_HEAT_TC; ch++) {
			tc = heat_docks[i].tcs[ch];
			if (tc->thermistor_channel != 0xff) {
				tc->last_temperature = tc->current_temperature;
				tc->current_temperature = get_temperature(tc->thermistor_channel);

				// ? ?? printfln
	//			printfln("dock [%d : %d] - %.2f deg.c, timestamp %u",
	//					 tc->dock_id, tc->idx, tc->current_temperature, get_timestamp_ms());
			}
		}
		//			do_heat_dock(i);
	}

//	adc1.do_flag = 0;

//	do_start_adc();

	return 0;
}

