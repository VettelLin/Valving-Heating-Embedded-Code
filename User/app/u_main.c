/*
 * u_main.c
 *
 *  Created on: Sep 3, 2021
 *      Author: abel
 */




#include "version.h"
#include "lib/l_gpio.h"
#include "lib/l_timer.h"
#include "lib/l_uart.h"
#include "lib/l_delay.h"
#include "lib/systime.h"
#include "app/u_gpio.h"
#include "app/u_can.h"
#include "ra18/can_msg.h"
#include "app/led_strip.h"
#include "app/u_adc.h"
#include "app/u_led.h"
#include "ra18/heat.h"
#include "ra18/thermistor.h"
#include "ra18/can_msg.h"
#include "ra18/iamp_msg.h"

void user_setup(void)
{

	Init_UART_Receive_IT();

	start_delay_timer();

	get_com_id();

	init_adc();

//	led_strip_init();

	init_heat_docks();


	printfln("start up device, system core clock frequence : %u, tick : %u ...", SystemCoreClock, HAL_GetTick());
//	printfln("delay timer : psc %lu, arr %lu, cnt %lu", DELAY_TIMER->PSC, DELAY_TIMER->ARR, DELAY_TIMER->CNT);
	printfln("firmware version : %s", VERSION_STR);

	printfln("com id = %#.2x", com_id);


	can_config_filter(&hcan, com_id << 8, 0xff00, 0, CAN_FILTERMODE_IDMASK, 0, CAN_FILTER_FIFO0);
	can_start(&hcan);
	can_activate_interrupt(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
//	can_activate_error_it(&hcan);

	do_start_adc();
}


static volatile uint32_t detect_timestamp_ms = 0;
static volatile uint32_t heat_timestamp_ms = 0;
static volatile uint32_t diff_ms = 0;

static volatile uint32_t heart_beat_ms = 0;

#define DOCK_HEAT_PERIOD	1000
#define DOCK_DETECT_PERIOD	10



void user_loop(void)
{

	if(get_timestamp_ms() - heart_beat_ms > 500){
		heart_beat_ms = get_timestamp_ms();
		reverse_led4();
	}


#if 0
led_strip_poll();
led_strip_refresh();
#endif

//	printfln("user loop : timestamp - %lu ms", get_timestamp_ms());

	if(get_timestamp_ms() - detect_timestamp_ms > DOCK_DETECT_PERIOD){
		detect_timestamp_ms = get_timestamp_ms();
		if(detect_heat_docks()){
			iampmsg_send_inplace();
		}
	}

#if 1
	diff_ms = get_timestamp_ms();
//	printfln("heat_timestamp 1 diff ms %lu - %lu ms", heat_timestamp_ms, diff_ms);

	diff_ms -= heat_timestamp_ms;

//	printfln("heat_timestamp 2 diff ms %lu - %lu ms", heat_timestamp_ms, diff_ms);

	if(diff_ms > DOCK_HEAT_PERIOD){
		heat_timestamp_ms = get_timestamp_ms();
//		printfln("heat : timestamp - %lu ms", heat_timestamp_ms);
//		do_start_adc();
//		get_temperatures();
		for(int i=0; i<NUM_DOCKS; i++){
			do_heat_dock(i);
		}

	}
#endif
//	HAL_Delay(500);
}



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{

}

//
void timer_base_user_handler(TIM_HandleTypeDef *htim)
{


}
