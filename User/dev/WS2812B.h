#ifndef	DEV_WS2812B_H
#define DEV_WS2812B_H



/*
T0H			0.4us		+/-150ns
T0L			0.85us		+/-150ns
T1H			0.85us		+/-150ns
T1L			0.4us		+/-150ns
RES			>50us
-----------------------------------------
T0H			220ns ~ 380ns	
T0L			580ns ~ 1us
T1H			580ns ~ 1us
T1L			580ns ~ 1us
RES			>280us

*/



/*
24bit data
G7 G6 G5 G4 G3 G2 G1 G0 R7 R6 R5 R4 R3 R2 R1 R0 B7 B6 B5 B4 B3 B2 B1 B0
msb first, follow with GRB 
*/

#include <stdint.h>

#define MAX_NUM_WS2812B		12


#define WS2812B_COLOR(g, r, b)		((((g)&0xff) << 16) | (((r)&0xff) << 8) | ((b)&0xff))

void WS2812B_SET_STRIP(uint32_t colors[], int n);


void WS2812B_Init(void);


#endif
