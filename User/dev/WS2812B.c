
#include "lib/l_delay.h"
#include "lib/l_timer.h"
#include "WS2812B.h"

extern DMA_HandleTypeDef hdma_tim2_up;

#define CCR_BufferSize	(24 * MAX_NUM_WS2812B)

static uint16_t Color_CCR_Buffer[CCR_BufferSize] = {0};



#define BIT0_PWM_DUTY		24 // 28	//24
#define BIT1_PWM_DUTY		66 // 61	//45

static uint16_t DataBitDuty[] = {BIT0_PWM_DUTY, BIT1_PWM_DUTY};


static void colorToPWMDuty(uint16_t *pBuffer, uint32_t color)
{
	int i;
	
	for(i=23; i>=0; i--){
		*pBuffer++ = DataBitDuty[(color >> i) & 1];
	}
	
}



#define DO_RESET()		delay_us(280)


static inline void CCR_Buffer_SET(int idx, uint32_t color)
{
	colorToPWMDuty(&Color_CCR_Buffer[idx*24], color);
	
}


void WS2812B_SET_STRIP(uint32_t colors[], int n)
{
	int i;

	for(i=0; i<n; i++){
		CCR_Buffer_SET(i, colors[i]);
	}
	
	HAL_TIM_PWM_Start_DMA(&hdma_tim2_up, 2, (uint32_t *)Color_CCR_Buffer, 24 * n * 2);

	DO_RESET();

}

void WS2812B_Init(void)
{


}

#if 0
void DMA1_Channel2_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC2) == SET){
		DMA_ClearITPendingBit(DMA1_IT_TC2);
		DMA_Cmd(DMA1_Channel2, DISABLE);
		__NOP();__NOP();__NOP();__NOP();__NOP();
		TIM2->CCR2 = 0;
		TIM_Cmd(TIM2, DISABLE);
	}
}
#endif
