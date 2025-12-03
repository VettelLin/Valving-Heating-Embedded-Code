/*
 * u_can.c
 *
 *  Created on: Jun 10, 2022
 *      Author: abel
 */



#include "u_can.h"
#include "lib/l_uart.h"

__weak void can_user_RxCallback(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef *pHead, uint8_t *rxbuff)
{
	UNUSED(hcan);
	UNUSED(pHead);
	UNUSED(rxbuff);
}



void can_rxMsg_handler(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef *pHead, uint8_t *rxbuff)
{
	int i;

	printf("can rxHeader : ide %#x, stdid %#x, extid %#x\r\n", pHead->IDE, pHead->StdId, pHead->ExtId);
	printf("can rxHeader : RTR %#x, dlc %#x\r\n", pHead->RTR, pHead->DLC);
	printf("can rxHeader : Timestamp %#x, FilterMatchIndex %#x\r\n", pHead->Timestamp, pHead->FilterMatchIndex);

	for(i=0; i<pHead->DLC; i++){
		printf("byte[%d] = %#.2x\r\n", i, rxbuff[i]);
	}

	can_user_RxCallback(hcan, pHead, rxbuff);
}

void can_error_handler(CAN_HandleTypeDef *hcan)
{
	printf("can error code : %#x\r\n", hcan->ErrorCode);
	printf("can state code : %#x\r\n", hcan->State);
}
