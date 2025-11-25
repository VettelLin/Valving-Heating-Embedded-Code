#include "heat.h"

#include "thermistor.h"
#include "lib/systime.h"
#include "lib/l_uart.h"
#include "usize.h"
#include <math.h>

// PID ????
#define KP 120      // ???????????
#define KI 8      // ???????????
#define KD 2      // ????????

#define TEMP_TOLERANCE 0.5  // ????? 0.5?C
#define INTEGRAL_MAX 100.0  // ??????
#define INTEGRAL_MIN 0.0    // ??????

#define OUTPUT_MAX 100.0     // PWM ????
#define OUTPUT_ 100.0     // PWM ????


// ???? OUTPUT_MIN
float calculate_dynamic_output_min(float error, float current_temperature, float setpoint) {
    if (fabs(error) > 10.0) {
        return 60.0;  // ?? >10 C?PWM 60%
    } else if (fabs(error) > 5.0) {
        return 40.0;  // ?? 5 C~10 C?PWM 40%
    } else if (fabs(error) > 1.0) {
        return 25.0;  // ?? 1 C~5 C?PWM 25%
    } else {
        return 10.0;  // ?? <1 C????? 10% PWM ??????
    }
}




float pid_control(PID_t *pid, float setpoint, float current_temperature) {
    float error = setpoint - current_temperature;
    float derivative = error - pid->prev_error;

    // **??????**
    if (current_temperature >= setpoint + 1.0) {
        pid->integral *= 0.9;  // ????????????
        return 0.0;
    }

    // **?? PID ???????????**
    float scale = (fabs(error) < 5.0) ? 0.3 : 1.0;

    // **?????**
    if (fabs(error) > TEMP_TOLERANCE && fabs(error) < 15.0) {
        pid->integral += error * scale;
    } else {
        pid->integral *= 0.95;  // ??????
    }

    // **???????**
    if (pid->integral > 50.0) pid->integral = 50.0;
    if (pid->integral < -50.0) pid->integral = -50.0;

    pid->prev_error = error;

    // **?? PID ??**
    float output = (KP * scale * error) + (KI * pid->integral) + (KD * derivative);

    // **???? PWM**
    float output_min = calculate_dynamic_output_min(error, current_temperature, setpoint);

    // **?? PWM ????**
    if (output > OUTPUT_MAX) output = OUTPUT_MAX;
    if (output < output_min) output = output_min;

    return output;
}




int HEAT_MAX_TEMPERATURE[] = {100, 82, 62};

int HPV_TARGET_TEMPERATURE[] = {95, 70, 60};
int RNA_TARGET_TEMPERATURE[] = {25, 70, 61};
int DNA_TARGET_TEMPERATURE[] = {95, 70, 61};


heat_channel_st heat_tcs[] = {
{0, 0xff, 0, 0, &gpio_b6, &pwm_4_1},	// ad 10
{0, 0xff, 0, 0, &gpio_a1, NULL},
{0, 0xff, 0, 0, &gpio_b7, &pwm_4_2},	// ad11

{0, 0xff, 0, 0, &gpio_b9, &pwm_4_4},	// ad 12
{0, 0xff, 0, 0, &gpio_a0, NULL},
{0, 0xff, 0, 0, &gpio_b8, &pwm_4_3},	// ad 2

{0, 0xff, 0, 0, &gpio_b11, &pwm_2_4},      // ad 3
{0, 0xff, 0, 0, &gpio_b12, NULL},
{0, 0xff, 0, 0, &gpio_c6, &pwm_3_1},      // ad 4

{0, 0xff, 0, 0, &gpio_c7, &pwm_3_2},      // ad 5
{0, 0xff, 0, 0, &gpio_b13, NULL},
{0, 0xff, 0, 0, &gpio_c8, &pwm_3_3},      // ad 8

{0, 0xff, 0, 0, &gpio_c9, &pwm_3_4},      // ad 14
{0, 0xff, 0, 0, &gpio_b14, NULL},
{0, 0xff, 0, 0, &gpio_a8, &pwm_1_1},      // ad 15

{0, 0xff, 0, 0, &gpio_a10, &pwm_1_3},      // ad 6
{0, 0xff, 0, 0, &gpio_b15, NULL},
{0, 0xff, 0, 0, &gpio_a9, &pwm_1_2},      // ad 7
};

heat_dock_st heat_docks[] = {
{1, 0, 0, 0, &gpio_b5, {&heat_tcs[0], &heat_tcs[1], &heat_tcs[2]}},
{2, 0, 0, 0, &gpio_b4, {&heat_tcs[3], &heat_tcs[4], &heat_tcs[5]}},
{3, 0, 0, 0, &gpio_b10, {&heat_tcs[6], &heat_tcs[7], &heat_tcs[8]}},
{4, 0, 0, 0, &gpio_b1, {&heat_tcs[9], &heat_tcs[10], &heat_tcs[11]}},
{5, 0, 0, 0, &gpio_b2, {&heat_tcs[12], &heat_tcs[13], &heat_tcs[14]}},
{6, 0, 0, 0, &gpio_a15, {&heat_tcs[15], &heat_tcs[16], &heat_tcs[17]}},
};

heat_step_st heat_steps[] = {
		{0, 0, 60, 60000},
		{0, 0, 50, 60000},
		//{0, 0, 30, 60000},
};

float HEAT_TARGET_TEMPERATURE[] = {50, 82, 62};
//uint32_t HEAT_DURATION_MS[] = {60000, 15000, 20000};
uint32_t HEAT_DURATION_MS[] = {120000, 20000, 20000};
// ✅ 正确的位置（全局变量）
static uint8_t h1_subphase[NUM_DOCKS] = {0}; // H1 两阶段状态追踪

static inline void tc_set_pwm(heat_channel_st *p_tc, uint16_t duty)
{
    if(p_tc->pwm){
    	pwm_set_percent(p_tc->pwm, duty);
    }
}

void init_heat_docks(void)
{
    heat_channel_st *tc;


    for(int i=0; i<NUM_DOCKS; i++){
        for(int j=0; j<NUM_HEAT_TC; j++){
            tc = heat_docks[i].tcs[j];
            tc->dock_id = heat_docks[i].dock_id;
            tc->idx = j + 1;
            tc->target_temperature = HEAT_TARGET_TEMPERATURE[j];
            tc->duration = HEAT_DURATION_MS[j];
            tc->s_time = 0;
            tc->last_temperature = 0;
            tc->current_temperature = 0;

            if(tc->thermistor_channel != 0xff){
                tc->pid_controller.kp = KP;
                tc->pid_controller.ki = KI;
                tc->pid_controller.kd = KD;
                tc->pid_controller.prev_error = 0;
                tc->pid_controller.integral = 0;
            }
            if(tc->pwm){
                tc_set_pwm(tc, 0);
            }else{
                turn_heat_tc(tc, 0);
            }

        }
        heat_docks[i].hsteps = heat_steps;
        heat_docks[i].num_step = ARRAY_SIZE(heat_steps);
    }

}

void stop_heat_dock(int idx)
{
	heat_dock_st *dock = &heat_docks[idx];
	heat_channel_st *tc;

	printfln("stop heat dock !");

    if(dock->do_flag){
        dock->do_flag = 0;
        for(int j=0; j<NUM_HEAT_TC; j++){
            tc = dock->tcs[j];
            tc->s_time = 0;
            tc->flag = 0;
            if(tc->pwm){
//                tc_set_pwm(tc, 0);
                pwm_stop(tc->pwm);
            }else{
                turn_heat_tc(tc, 0);
            }
        }
    }
}

//static uint8_t h1_subphase[NUM_DOCKS] = {0}; // H1 两阶段状态追踪

void start_heat_dock(int idx)
{
    heat_dock_st *dock = &heat_docks[idx];
    heat_channel_st *tc;
    int i;

    for (i = 0; i < NUM_HEAT_TC; i++) {
        tc = dock->tcs[i];
        tc->flag = 0;
        if (tc->pwm) {
            tc_set_pwm(tc, 0);
        } else {
            turn_heat_tc(tc, 0);
        }
    }

    dock->phase = 0;
    dock->do_flag = 1;

	dock->s_time = get_timestamp_ms();

    // ✅ 正确放在这里，确保每次点击“开始”按钮都会重置 H1 状态
    h1_subphase[idx] = 0;

    printfln("start heat dock, %u ...", dock->s_time);

//    for (i = 0; i < dock->num_step; i++) {
//    	printf("step[%d] : duty %lu , duration %lu\r\n", i, dock->hsteps[i].target.duty_pecent, dock->hsteps[i].duration);
//    }
}



int detect_heat_docks()
{
    int ret = 0;
    heat_dock_st *dock;
    static char in_place[NUM_DOCKS] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    for(int i=0; i<NUM_DOCKS; i++){
        dock = &heat_docks[i];
        dock->in_place = (gpio_get_status(dock->detect_pin) == 0) ? 1 : 0;
        if(dock->in_place != in_place[i]){
            ret = 1;
            if(dock->in_place && (dock->do_flag == 0)){
//				dock->tcs[0]->duration =  7 * 60 * 1000;
				//dock->do_flag = 1;
				start_heat_dock(i);
			}
        }
        in_place[i] = dock->in_place;
    }

    return ret;
}


#define UPDATE_TEMPERATURE_MODE     0       // 0 all channel , 1 current channel

#define HEAT_LATENCY (7 * 60 * 1000)

void do_heat_dock(int idx) 
{
    heat_channel_st *tc;
    heat_dock_st *dock = &heat_docks[idx];
    heat_step_st *hstep;
    float control_signal;
    uint16_t duty_cycle;

    for (int i = 0; i < NUM_HEAT_TC; i++) {
		tc = dock->tcs[i];
		if (tc->thermistor_channel != 0xff) {
			tc->last_temperature = tc->current_temperature;
			tc->current_temperature = get_temperature(tc->thermistor_channel);
//
//			printfln("dock [%d : %d] - %.2f deg.c, timestamp %u",
//					 tc->dock_id, tc->idx, tc->current_temperature, get_timestamp_ms());
		}
	}


    if(dock->in_place == 0){
    	if(dock->do_flag){
    		stop_heat_dock(idx);
    	}
    	return;
    }

    if ((dock->do_flag == 0) || (dock->phase >= dock->num_step)) {
        return;
    }


    if((get_timestamp_ms() - dock->s_time) < HEAT_LATENCY){
    	return;
    }

    // 添加静态变量来追踪 H1 的两个阶段
    //static uint8_t h1_subphase[NUM_DOCKS] = {0};  // 0=升温至50, 1=降温至40, 2=完成


    // ???????????????

    // ???? phase ???????
    hstep = &dock->hsteps[dock->phase];
    tc = dock->tcs[hstep->tc_idx];

    if(hstep->duration == 0){
    	dock->phase++;
    	return;
    }

    if (tc->flag == 0) {
    #if 1
		tc->flag = 1;
	#else
		tc->flag = 2;
		tc->s_time = get_timestamp_ms();
	#endif
		if(tc->pwm){
			pwm_start(tc->pwm);
		}else{
        	turn_heat_tc(tc, 1);
        }

	}

    if (tc->thermistor_channel != 0xff) {
        // ????
//        tc->last_temperature = tc->current_temperature;
//        tc->current_temperature = get_temperature(tc->thermistor_channel);

        // **????????????**
        if (tc->current_temperature >= hstep->target.temperature)
        {
            if (tc->flag == 1) {
                tc->flag = 2;
                tc->s_time = get_timestamp_ms();
            }
        }

        // **??? PWM ??**
        if(tc->pwm){
			if (tc->current_temperature < (hstep->target.temperature - 20)) {
				duty_cycle = 1000;  // ?????? 20 C???????
			}
			else {
				// PID 输出
				control_signal = pid_control(&tc->pid_controller, hstep->target.temperature, tc->current_temperature);
				duty_cycle = (uint16_t)(control_signal);

				// 接近目标温度时降低一半输出
				if (tc->current_temperature > (hstep->target.temperature - 1.5)) {
					duty_cycle *= 0.5;
				}

				// 限制 duty 范围，允许 PID 返回 0
				if (duty_cycle > 100)
					duty_cycle = 100;

				if (duty_cycle < 10) {
					if (control_signal <= 0.0f)
						duty_cycle = 0;  // ⛔ 真正停热
					else
						duty_cycle = 31; // ✅ 最小维持功率
				}
			}

			// ? ?? printfln
			printfln("heat step %d, dock[%d : %d], PWM %u, current temperature = %.2f, s_time %lu, current time %lu",
					dock->phase, tc->dock_id, tc->idx, duty_cycle, tc->current_temperature, tc->s_time, get_timestamp_ms());

			tc_set_pwm(tc, duty_cycle);
        }else{
        	if (tc->current_temperature > hstep->target.temperature){
        		turn_heat_tc(tc, 0);
        	}else{
        		turn_heat_tc(tc, 1);
        	}
        	printfln("heat step %d, dock[%d : %d], %s, current temperature = %.2f, s_time %lu, current time %lu",
        	  dock->phase, tc->dock_id, tc->idx, tc->pin->status ? "on" : "off", tc->current_temperature, tc->s_time, get_timestamp_ms());
        }
    } else {
        if (tc->flag == 1) {
            tc->flag = 2;
            tc->s_time = get_timestamp_ms();
            if(tc->pwm){
            	tc_set_pwm(tc, hstep->target.duty_pecent);
            	printfln("heat step %d, dock[%d : %d], PWM %u, s_time %lu, current time %lu",
            						dock->phase, tc->dock_id, tc->idx, hstep->target.duty_pecent, tc->s_time, get_timestamp_ms());
            }else{
            	printfln("heat step %d, dock[%d : %d], %s, s_time %lu, current time %lu",
            	        	  dock->phase, tc->dock_id, tc->idx, tc->pin->status ? "on" : "off", tc->s_time, get_timestamp_ms());
            }
        }


        // ? ?? printfln

    }

    // **?????????????**
	if (tc->flag == 2) {
		if ((get_timestamp_ms() - tc->s_time) >= hstep->duration) {
#if 0
			if (dock->phase == 0) {
				if (h1_subphase[idx] == 0) {
					h1_subphase[idx] = 1;
					tc->target_temperature = 40;
					tc->duration = 120000;
					tc->s_time = 0;  // 🟡 暂不计时，等达到40°C再重新开始
					tc->flag = 0;    // 🔁 重新进入加热逻辑
					printfln(">> H1 start phase 2: hold at 40C\r\n");
					return;
				} else if (h1_subphase[idx] == 1) {
					h1_subphase[idx] = 2;
					printfln(">> H1 Phase 2 done\r\n");
				}
			}
#else

#endif

			// 其他通道，或 H1 两阶段完成后：
			if (tc->pwm) {
				pwm_stop(tc->pwm);
			} else {
				turn_heat_tc(tc, 0);
			}
			printfln("heat step %d, dock[%d : %d], finished, s_time %lu, current time %lu",
					dock->phase, tc->dock_id, tc->idx, tc->s_time, get_timestamp_ms());
			tc->flag = 0;
			dock->phase++;
		}
	}

}
