#ifndef	BSP_LED_STRIP_H
#define BSP_LED_STRIP_H


#include "dev/WS2812B.h"

#define LED_STRIP_LAMPS		6

#define LAMP_COLOR(g, r, b)		WS2812B_COLOR(g, r, b)

void led_strip_light_lamps(uint32_t colors[], int s, int n);
void led_strip_light(int idx, uint32_t color);
void led_strip_set(int idx, uint32_t color);

void led_strip_refresh(void);
void led_strip_poll(void);

void led_strip_init(void);



#endif
