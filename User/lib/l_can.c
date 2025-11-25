/*
 * l_can.c
 *
 *  Created on: Jun 10, 2022
 *      Author: abel
 */


#include "l_can.h"

//#include "stm32f7xx_hal_can.h"


__weak void can_rxMsg_handler(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef *rxHeader, uint8_t *rxbuff)
{
	UNUSED(hcan);
	UNUSED(rxHeader);
	UNUSED(rxbuff);
}

__weak void can_error_handler(CAN_HandleTypeDef *hcan)
{
	UNUSED(hcan);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rxHeader;
	uint8_t rxbuff[8];

	do{
		if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxbuff) != HAL_OK){
			Error_Handler();
		}

		can_rxMsg_handler(hcan, &rxHeader, rxbuff);
	}while(HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO0) > 0);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef rxHeader;
	uint8_t rxbuff[8];

	do{
		if(HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO1, &rxHeader, rxbuff) != HAL_OK){
			Error_Handler();
		}

		can_rxMsg_handler(hcan, &rxHeader, rxbuff);
	}while(HAL_CAN_GetRxFifoFillLevel(hcan, CAN_RX_FIFO1) > 0);
}


void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	can_error_handler(hcan);
}

void can_activate_error_it(CAN_HandleTypeDef *hcan)
{
	can_activate_interrupt(hcan, CAN_IT_ERROR);
	can_activate_interrupt(hcan, CAN_IT_BUSOFF);
	can_activate_interrupt(hcan, CAN_IT_LAST_ERROR_CODE);
	can_activate_interrupt(hcan, CAN_IT_ERROR_PASSIVE);
	can_activate_interrupt(hcan, CAN_IT_ERROR_WARNING);
}


//#define CAN_TX_MAILBOX0             (0x00000001U)  /*!< Tx Mailbox 0  */
//#define CAN_TX_MAILBOX1             (0x00000002U)  /*!< Tx Mailbox 1  */
//#define CAN_TX_MAILBOX2             (0x00000004U)  /*!< Tx Mailbox 2  */

int can_sendStdMessage(CAN_HandleTypeDef *hcan, uint16_t stdId, uint8_t *txbuff, uint8_t dsize)
{
	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;

	txHeader.IDE = CAN_ID_STD;
	txHeader.StdId = stdId;
//	txHeader.ExtId = 0;
	txHeader.RTR = CAN_RTR_DATA;
	txHeader.DLC = dsize;
	txHeader.TransmitGlobalTime = DISABLE;
	if(HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0){
		return HAL_CAN_AddTxMessage(hcan, &txHeader, txbuff, &txMailbox);
	}else{
		return -1;
	}
}

int can_sendExtMessage(CAN_HandleTypeDef *hcan, uint32_t extId, uint8_t *txbuff, uint8_t dsize)
{
	CAN_TxHeaderTypeDef txHeader;
	uint32_t txMailbox;

	txHeader.IDE = CAN_ID_EXT;
//	txHeader.StdId = stdId;
	txHeader.ExtId = extId;
	txHeader.RTR = CAN_RTR_DATA;
	txHeader.DLC = dsize;
	txHeader.TransmitGlobalTime = DISABLE;

	if(HAL_CAN_GetTxMailboxesFreeLevel(hcan) > 0){
		return HAL_CAN_AddTxMessage(hcan, &txHeader, txbuff, &txMailbox);
	}else{
		return -1;
	}
}



int can_config_filter(CAN_HandleTypeDef *hcan, uint32_t FxID, uint32_t FxMask, uint8_t bank, uint8_t mode, uint8_t scale, uint8_t rxfifo)
{
    CAN_FilterTypeDef sFilterConfig;

    sFilterConfig.FilterBank           = bank;												// 设置过滤器组编号

	sFilterConfig.FilterMode = mode;
	if(scale){
		sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
		sFilterConfig.FilterIdHigh         = ((FxID<<3)&0xFFFF0000)>>16;
		sFilterConfig.FilterIdLow          = ((FxID<<3)|CAN_ID_EXT|CAN_RTR_DATA)&0xFFFF;
		sFilterConfig.FilterMaskIdHigh     = ((FxMask<<3)&0xFFFF0000)>>16;
		sFilterConfig.FilterMaskIdLow      = ((FxMask<<3)|0x06)&0xFFFF;
	}else{
		sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
		sFilterConfig.FilterIdHigh         = ((FxID<<5)|((CAN_ID_STD|CAN_RTR_DATA)<<2))&0xFFFF;
		sFilterConfig.FilterIdLow          = ((FxID<<5)|((CAN_ID_STD|CAN_RTR_DATA)<<2))&0xFFFF;
		sFilterConfig.FilterMaskIdHigh     = ((FxMask<<5)|(0x06<< 2))&0xFFFF;
		sFilterConfig.FilterMaskIdLow      = ((FxMask<<5)|(0x06<< 2))&0xFFFF;
	}
    sFilterConfig.FilterFIFOAssignment = rxfifo;
    sFilterConfig.FilterActivation     = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;
    if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK){
        Error_Handler();
        return -1;
    }

    return 0;
}


void can_default_setup(CAN_HandleTypeDef *hcan)
{
	can_config_filter(hcan, 0x0, 0, 0, CAN_FILTERMODE_IDMASK, 0, CAN_FILTER_FIFO0);

	can_start(hcan);

	can_activate_interrupt(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
	can_activate_interrupt(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);
	can_activate_error_it(hcan);
}

