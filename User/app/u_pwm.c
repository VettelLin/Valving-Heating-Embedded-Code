/*
 * u_pwm.c
 *
 *  Created on: May 8, 2025
 *      Author: abel
 */

#include "u_pwm.h"

pwm_st pwm_1_1 = {&htim1, 1, 0, 0};
pwm_st pwm_1_2 = {&htim1, 2, 0, 0};
pwm_st pwm_1_3 = {&htim1, 3, 0, 0};

pwm_st pwm_2_2 = {&htim2, 2, 0, 0};
pwm_st pwm_2_4 = {&htim2, 4, 0, 0};

pwm_st pwm_3_1 = {&htim3, 1, 0, 0};
pwm_st pwm_3_2 = {&htim3, 2, 0, 0};
pwm_st pwm_3_3 = {&htim3, 3, 0, 0};
pwm_st pwm_3_4 = {&htim3, 4, 0, 0};

pwm_st pwm_4_1 = {&htim4, 1, 0, 0};
pwm_st pwm_4_2 = {&htim4, 2, 0, 0};
pwm_st pwm_4_3 = {&htim4, 3, 0, 0};
pwm_st pwm_4_4 = {&htim4, 4, 0, 0};

pwm_st pwm_5_1 = {&htim5, 1, 0, 0};
pwm_st pwm_5_2 = {&htim5, 2, 0, 0};

