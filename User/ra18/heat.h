
#ifndef APP_HEAT_H_
#define APP_HEAT_H_

#include "app/u_gpio.h"
#include "app/u_pwm.h"

#define NUM_DOCKS		 6
#define NUM_HEAT_TC		2

#define HEAT_DURATION_2    15      // 10 - 20 s


extern int HEAT_MAX_TEMPERATURE[];
extern int HPV_TARGET_TEMPERATURE[];
extern int RNA_TARGET_TEMPERATURE[];
extern int DNA_TARGET_TEMPERATURE[];

typedef struct {
    float kp;
    float ki;
    float kd;
    float prev_error;
    float integral;
    int step;
} PID_t;


typedef struct{
	volatile char flag;
    unsigned char thermistor_channel;
	volatile char dock_id;
	volatile char idx;
	gpio_st *pin;
    volatile pwm_st *pwm;
	volatile uint32_t s_time;	// in millisecond
	volatile uint32_t duration;	// in millisecond
	volatile float last_temperature;	// in deg.c
	volatile float current_temperature;	// in deg.c
	volatile float target_temperature;		// in deg.c
    PID_t pid_controller;
}heat_channel_st;

typedef struct{
	int tc_idx;
	struct{
		float temperature;		// in deg.c
		uint32_t duty_pecent;
	} target;
	uint32_t duration;	// in millisecond
}heat_step_st;

typedef struct{
	volatile char dock_id;
	volatile char phase;
	volatile char do_flag;
	volatile char in_place;
    gpio_st *detect_pin;
	heat_channel_st *tcs[3];
	volatile uint32_t s_time;	// in millisecond
	int num_step;
	heat_step_st *hsteps;
}heat_dock_st;

extern heat_channel_st heat_tcs[];
extern heat_dock_st heat_docks[];



extern void init_heat_docks(void);
extern int detect_heat_docks();
extern void do_heat_dock(int idx);
extern void start_heat_dock(int idx);
extern void stop_heat_dock(int idx);

#define turn_heat_tc(p_tc, on)	gpio_set_status((p_tc)->pin, on)

//#define tc_set_pwm(p_tc, duty)      (p_tc)->set_pwm(duty)
//#define tc_set_pwm(p_tc, duty)      *(p_tc)->pwm_duty = (uint16_t)(duty)



#endif /* APP_HEAT_H_ */
