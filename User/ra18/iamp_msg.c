

#include <stdarg.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib/l_uart.h"

#include "heat.h"
#include "can_msg.h"
#include "iamp_msg.h"

uint8_t calc_rcc(uint8_t *pdata, int len)
{
	int i;

	uint8_t sum = 0;

	for(i=0; i<len; i++){
		sum += pdata[i];
	}

	return sum;
}

int check_rcc(uint8_t *pdata, int len)
{
	int i;

	uint8_t sum = 0;

	for(i=0; i<len; i++){
		sum += pdata[i];
	}

	return (sum ==  pdata[len]);
}

















void uart4_user_RxCallback(UART_HandleTypeDef *huart, uint8_t *rx_data, uint16_t rx_len)
{

}





int iampmsg_send_inplace(void)
{
	int i;
	uint8_t txbuff[8];
	uint8_t txlen = 0;
	uint8_t status = 0;

	for(i = 0; i < NUM_DOCKS; i++){
		txbuff[txlen++] = (heat_docks[i].in_place) | (i << 1);
	}

	txbuff[txlen++] = status;


	can_replymsg(PCR_INPLACE, txbuff, txlen);

	return 0;

}
