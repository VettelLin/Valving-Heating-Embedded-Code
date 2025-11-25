
#include "led_strip.h"
#include <stdio.h>

#if 0
static uint32_t led_lamps_color[LED_STRIP_LAMPS] = {
	0x000000, 0xFFFFFF, 0x000000, 0xFFFFFF, 0x000000, 0xFFFFFF,
	0x000000, 0xFFFFFF, 0x000000, 0xFFFFFF
};
#else
static uint32_t led_lamps_color[LED_STRIP_LAMPS] = {
	0x555555, 0x555555, 0x555555, 0x555555, 0x555555,
	0x555555, 0x555555, 0x555555, 0x555555, 0x555555
};
#endif


#define LED_SET_STRIP(colors, n)		WS2812B_SET_STRIP(colors, n);

void led_strip_light_lamps(uint32_t colors[], int s, int n)
{
	int i;

	for(i=0; i<n; i++){
		led_lamps_color[s+i] = colors[i];
	}
	LED_SET_STRIP(led_lamps_color, LED_STRIP_LAMPS);
}


void led_strip_light(int idx, uint32_t color)
{
	led_strip_light_lamps(&color, idx, 1);
}

void led_strip_set(int idx, uint32_t color)
{
	led_lamps_color[idx] = color;
}

void led_strip_refresh(void)
{
	LED_SET_STRIP(led_lamps_color, LED_STRIP_LAMPS);
}


// 00, 55, aa, ff


static uint32_t led_lamps_gradual_color[] = {
		0x000000, 0x000080, 0x0000ff, 0x00ffff,
		0x008000, 0x00ff00, 0x800080, 0x008080
};


void led_strip_poll(void)
{
	static int s_idx = 0;
	int i, c_idx;

	for(i=0; i<LED_STRIP_LAMPS; i++){
		c_idx = s_idx + i;
		
		led_lamps_color[i] = led_lamps_gradual_color[c_idx % LED_STRIP_LAMPS];
		
//		printf("led_lamps_color[%d] = %#.6x\r\n", i, led_lamps_color[i]);
	}

//	printf("\r\n");
	
	s_idx++;
}

void led_strip_init(void)
{
	WS2812B_Init();
	
}
