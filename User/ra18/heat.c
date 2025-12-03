#include "heat.h"

#include "thermistor.h"
#include "lib/systime.h"
#include "lib/l_uart.h"
#include "usize.h"
#include <math.h>
#include "app/u_adc.h"
extern adc_st adc1;
extern uint8_t adc_index[16];  // thermistor.c 里已有

static inline void dbg_print_raw(int phys_ch) {
    int idx = adc_index[phys_ch];            // 由 thermistor.c 提供的映射
    uint16_t raw = adc1.regular_channels[idx].value;
    float volt = adc_val2volt(&adc1, raw);
    printfln("[ADC] ch=%d idx=%d raw=%u volt=%.4fV", phys_ch, idx, raw, volt);
}




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

    // 高于设点 1℃：泄放积分并停热
    if (current_temperature >= setpoint + 1.0f) {
        pid->integral *= 0.9f;
        pid->prev_error = error;
        return 0.0f;
    }

    // 小误差降比例增益
    float scale = (fabsf(error) < 5.0f) ? 0.3f : 1.0f;

    // 积分（容差外才积，否则泄放）
    if (fabsf(error) > TEMP_TOLERANCE && fabsf(error) < 15.0f) {
        pid->integral += error * scale;
    } else {
        pid->integral *= 0.95f;
    }

    // 对称限幅（使用你的宏）
    if (pid->integral >  INTEGRAL_MAX) pid->integral =  INTEGRAL_MAX;
    if (pid->integral < -INTEGRAL_MAX) pid->integral = -INTEGRAL_MAX;

    pid->prev_error = error;

    float output = (KP * scale * error) + (KI * pid->integral) + (KD * derivative);

    // 动态最小输出：接近目标允许 0
    float output_min;
    if (fabsf(error) > 10.0f)      output_min = 60.0f;
    else if (fabsf(error) > 5.0f)  output_min = 40.0f;
    else if (fabsf(error) > 2.0f)  output_min = 15.0f;
    else                           output_min = 0.0f;

    // 夹到 [0, OUTPUT_MAX]
    if (output > OUTPUT_MAX) output = OUTPUT_MAX;
    if (output < 0.0f)       output = 0.0f;

    // 如非 0，保证不低于下限
    if (output > 0.0f && output < output_min) output = output_min;

    return output;  // 0~100（对应 pwm_set_percent 的百分比）
}






int HEAT_MAX_TEMPERATURE[] = {100, 82, 62};

int HPV_TARGET_TEMPERATURE[] = {95, 70, 60};
int RNA_TARGET_TEMPERATURE[] = {25, 70, 61};
int DNA_TARGET_TEMPERATURE[] = {95, 70, 61};


heat_channel_st heat_tcs[] = {
    {0, 10, 0, 0, &gpio_b6, &pwm_4_1},   // ad 10
    {0, 0xff, 0, 0, &gpio_a1, NULL},
    {0, 11, 0, 0, &gpio_b7, &pwm_4_2},   // ad 11

    {0, 12, 0, 0, &gpio_b9, &pwm_4_4},   // ad 12
    {0, 0xff, 0, 0, &gpio_a0, NULL},
    {0,  2, 0, 0, &gpio_b8, &pwm_4_3},   // ad 2

    {0,  3, 0, 0, &gpio_b11, &pwm_2_4},  // ad 3
    {0, 0xff, 0, 0, &gpio_b12, NULL},
    {0,  4, 0, 0, &gpio_c6, &pwm_3_1},   // ad 4

    {0,  5, 0, 0, &gpio_c7, &pwm_3_2},   // ad 5
    {0, 0xff, 0, 0, &gpio_b13, NULL},
    {0,  8, 0, 0, &gpio_c8, &pwm_3_3},   // ad 8

    {0, 14, 0, 0, &gpio_c9, &pwm_3_4},   // ad 14
    {0, 0xff, 0, 0, &gpio_b14, NULL},
    {0, 15, 0, 0, &gpio_a8, &pwm_1_1},   // ad 15

    {0,  6, 0, 0, &gpio_a10, &pwm_1_3},  // ad 6
    {0, 0xff, 0, 0, &gpio_b15, NULL},
    {0,  7, 0, 0, &gpio_a9, &pwm_1_2},   // ad 7
};


heat_dock_st heat_docks[] = {
{1, 0, 0, 0, &gpio_b5, {&heat_tcs[0], &heat_tcs[1], &heat_tcs[2]}},
{2, 0, 0, 0, &gpio_b4, {&heat_tcs[3], &heat_tcs[4], &heat_tcs[5]}},
{3, 0, 0, 0, &gpio_b10, {&heat_tcs[6], &heat_tcs[7], &heat_tcs[8]}},
{4, 0, 0, 0, &gpio_b1, {&heat_tcs[9], &heat_tcs[10], &heat_tcs[11]}},
{5, 0, 0, 0, &gpio_b2, {&heat_tcs[12], &heat_tcs[13], &heat_tcs[14]}},
{6, 0, 0, 0, &gpio_a15, {&heat_tcs[15], &heat_tcs[16], &heat_tcs[17]}},
};

// heat.h 或本文件靠前处
heat_step_st heat_steps[] = {
    { .tc_idx = 0, .target = { .duty_pecent = 0, .temperature = 180 }, .duration = 100000 },
    { .tc_idx = 0, .target = { .duty_pecent = 0, .temperature = 120 }, .duration = 80000 },
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

    printfln("[START] dock=%d s_time=%lu in_place=%d num_step=%d",
             dock->dock_id, dock->s_time, dock->in_place, dock->num_step);
    for (int i=0;i<NUM_HEAT_TC;i++){
        heat_channel_st *t = dock->tcs[i];
        printfln("  tc[%d]: pwm=%p therm_ch=%d", i, (void*)t->pwm, t->thermistor_channel);
    }

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

#define HEAT_LATENCY (2 * 1* 1000)




extern void get_temperatures(void);   // 在 heat.c 顶部声明

void do_heat_dock(int idx)
{
    heat_channel_st *tc;
    heat_dock_st *dock = &heat_docks[idx];
    heat_step_st *hstep;
    float control_signal;
    uint16_t duty_cycle = 0;

    // ★ 每次循环先触发一次 ADC 采样，保证 regular_channels[] 已更新
    get_temperatures();

    // 刷新当前 dock 下的每一路温度并打印
    for (int i = 0; i < NUM_HEAT_TC; i++) {
        tc = dock->tcs[i];
        if (tc->thermistor_channel != 0xff) {
            tc->last_temperature    = tc->current_temperature;
            tc->current_temperature = get_temperature(tc->thermistor_channel);

            printfln("dock[%d:%d] Temp = %.2f C (time=%lu ms)",
                     dock->dock_id, tc->idx,
                     tc->current_temperature,
                     get_timestamp_ms());
        }
    }

    // 未就位：若在跑则停止
    if (dock->in_place == 0) {
        if (dock->do_flag) stop_heat_dock(idx);
        return;
    }

    // 未启动或已完成
    if (dock->do_flag == 0 || dock->phase >= dock->num_step) {
        return;
    }

    // 冻结门槛（7 分钟内不启动控温）
    if ((get_timestamp_ms() - dock->s_time) < HEAT_LATENCY) {
        return;
    }

    // 当前 step 与通道
    hstep = &dock->hsteps[dock->phase];
    tc    = dock->tcs[hstep->tc_idx];

    // 空步骤跳过
    if (hstep->duration == 0) {
        dock->phase++;
        return;
    }

    // 首次进入本 step
    if (tc->flag == 0) {
        tc->flag = 1;  // 1=拉温；2=恒温计时
        printfln("[STEP-ENTER] dock=%d phase=%d tc_idx=%d flag=%d pwm=%p therm_ch=%d",
                 dock->dock_id, dock->phase, hstep->tc_idx, tc->flag,
                 (void*)tc->pwm, tc->thermistor_channel);

        if (tc->pwm) {
            pwm_start(tc->pwm);
            tc_set_pwm(tc, 0);
        } else {
            turn_heat_tc(tc, 1);
        }
    }

    // 有热敏电阻：PID 控制
    if (tc->thermistor_channel != 0xff) {

        // 到温（带容差）开始恒温计时
        if (tc->current_temperature >= ((float)hstep->target.temperature - TEMP_TOLERANCE)) {
            if (tc->flag == 1) {
                tc->flag  = 2;
                tc->s_time = get_timestamp_ms();
                printfln("[HOLD-START] dock=%d phase=%d T=%.2f s_time=%lu",
                         dock->dock_id, dock->phase, tc->current_temperature, tc->s_time);
            }
        }

        if (tc->pwm) {
            // 远离目标 >20℃：满功率
            if (tc->current_temperature < ((float)hstep->target.temperature - 20.0f)) {
                duty_cycle = 100;
            } else {
                // PID 输出
                control_signal = pid_control(&tc->pid_controller,
                                             (float)hstep->target.temperature,
                                             tc->current_temperature);
                duty_cycle = (uint16_t)control_signal;

                // 接近目标抑冲
                if (tc->current_temperature >= ((float)hstep->target.temperature - 1.5f)) {
                    duty_cycle = (uint16_t)((float)duty_cycle * 0.5f);
                }
                if (duty_cycle > 100) duty_cycle = 100;
            }

            tc_set_pwm(tc, duty_cycle);

            printfln("[CTRL] dock=%d phase=%d T=%.2f set=%.2f err=%.2f duty=%u flag=%d",
                     dock->dock_id, dock->phase,
                     tc->current_temperature, (float)hstep->target.temperature,
                     (float)hstep->target.temperature - tc->current_temperature,
                     duty_cycle, tc->flag);
        } else {
            // 开关控温
            if (tc->current_temperature > (float)hstep->target.temperature) {
                turn_heat_tc(tc, 0);
            } else {
                turn_heat_tc(tc, 1);
            }
        }
    }
    // 无热敏：固定占空
    else {
        if (tc->flag == 1) {
            tc->flag  = 2;
            tc->s_time = get_timestamp_ms();
            if (tc->pwm) {
                tc_set_pwm(tc, hstep->target.duty_pecent);
            }
            printfln("[HOLD-START] dock=%d phase=%d (no-thermistor)", dock->dock_id, dock->phase);
        }
    }

    // 步骤完成：仅在恒温阶段判定
    if (tc->flag == 2) {
        if ((get_timestamp_ms() - tc->s_time) >= hstep->duration) {
            if (tc->pwm) pwm_stop(tc->pwm);
            else         turn_heat_tc(tc, 0);

            printfln("[STEP-DONE] dock=%d phase=%d", dock->dock_id, dock->phase);

            tc->flag = 0;
            dock->phase++;
        }
    }
}
