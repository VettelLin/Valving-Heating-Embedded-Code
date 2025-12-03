/*
 * can_msg.c
 *
 *  Created on: May 9, 2025
 *      Author: abel
 */

#include "heat.h"
#include "can_msg.h"
#include "lib/l_uart.h"



static int pcr_startheat(uint32_t canid, uint8_t *rxdata, uint8_t rxlen)
{
	int i;
	int dock_id;
	uint8_t txbuff[8];
	uint8_t txlen = 0;
	uint8_t status = 0;

	for(i = 0; i < rxlen; i++){
		dock_id= rxdata[i];
		 if(dock_id < NUM_DOCKS){
			 start_heat_dock(dock_id);
			status = 0x00;
		}else{
			status = 0x01;
		}
		txbuff[txlen++] = status | (dock_id << 1);
	}

	can_replymsg(canid, txbuff, txlen);

	return 0;
}

static int pcr_stopheat(uint32_t canid, uint8_t *rxdata, uint8_t rxlen)
{
	int i;
	int dock_id;
	uint8_t txbuff[8];
	uint8_t txlen = 0;
	uint8_t status = 0;

	for(i = 0; i < rxlen; i++){
		dock_id= rxdata[i];
		 if(dock_id < NUM_DOCKS){
			stop_heat_dock(dock_id);
			status = 0x00;
		}else{
			status = 0x01;
		}
		txbuff[txlen++] = status | (dock_id << 1);
	}

	can_replymsg(canid, txbuff, txlen);

	return 0;
}

static int pcr_curtemp(uint32_t canid, uint8_t *rxdata, uint8_t rxlen)
{
	int i, j;
	int dock_id;
	uint8_t txbuff[8];
	uint8_t txlen = 0;

	heat_dock_st *p_dock;
	uint16_t v_u16;
	uint8_t status = 0;

	for(i = 0; i < rxlen; i++){
		dock_id= rxdata[i];
		 if(dock_id < NUM_DOCKS){
			p_dock = &heat_docks[dock_id];
		}else{
			p_dock = NULL;
			status = 0x01;
			v_u16 = 0x00;
		}

		txlen = 0;
		txbuff[txlen++] = dock_id;
		for(j=0; j<NUM_HEAT_TC; j++){
			if(p_dock){
				v_u16 = p_dock->tcs[j]->current_temperature * 10;
			}
			txbuff[txlen++] = (v_u16 >> 8) & 0xff;
			txbuff[txlen++] = (v_u16 >> 0) & 0xff;
		}
		txbuff[txlen++] = status;

		can_replymsg(canid, txbuff, txlen);
	}

	return 0;
}

static int pcr_gettemp(uint32_t canid, uint8_t *rxdata, uint8_t rxlen)
{
	int i, j;
	int dock_id;
	uint8_t txbuff[8];
	uint8_t txlen = 0;

	heat_dock_st *p_dock;
	uint16_t v_u16;
	uint8_t status = 0;

	for(i = 0; i < rxlen; i++){
		dock_id= rxdata[i];
		 if(dock_id < NUM_DOCKS){
			p_dock = &heat_docks[dock_id];
		}else{
			p_dock = NULL;
			status = 0x01;
			v_u16 = 0x00;
		}

		txlen = 0;
		txbuff[txlen++] = dock_id;
		for(j=0; j<NUM_HEAT_TC; j++){
			if(p_dock){
				v_u16 = p_dock->tcs[j]->target_temperature;
			}
			txbuff[txlen++] = (v_u16 >> 8) & 0xff;
			txbuff[txlen++] = (v_u16 >> 0) & 0xff;
		}
		txbuff[txlen++] = status;

		can_replymsg(canid, txbuff, txlen);
	}

	return 0;
}

static int pcr_gettime(uint32_t canid, uint8_t *rxdata, uint8_t rxlen)
{
	int i, j;
	int dock_id;
	uint8_t txbuff[8];
	uint8_t txlen = 0;

	heat_dock_st *p_dock;
	uint16_t v_u16;
	uint8_t status = 0;

	for(i = 0; i < rxlen; i++){
		dock_id= rxdata[i];
		 if(dock_id < NUM_DOCKS){
			p_dock = &heat_docks[dock_id];
		}else{
			continue;
			p_dock = NULL;
			status = 0x01;
			v_u16 = 0x00;
		}

		txlen = 0;
		txbuff[txlen++] = dock_id;
		for(j=0; j<NUM_HEAT_TC; j++){
			if(p_dock){
				v_u16 = p_dock->tcs[j]->duration / 1000;;
			}
			txbuff[txlen++] = (v_u16 >> 8) & 0xff;
			txbuff[txlen++] = (v_u16 >> 0) & 0xff;
		}
		txbuff[txlen++] = status;

		can_replymsg(canid, txbuff, txlen);
	}

	return 0;
}


static int pcr_settemp(uint32_t canid, uint8_t *rxdata, uint8_t rxlen)
{
	int i, j;
	int dock_id;
	uint8_t txbuff[8];
	uint8_t txlen = 0;
	uint8_t offs = 0;

	heat_dock_st *p_dock;
	uint16_t v_u16;
	uint8_t status = 0;

	dock_id= rxdata[0];
	if(dock_id < NUM_DOCKS){
		p_dock = &heat_docks[dock_id];
	}else{
		p_dock = NULL;
		status = 0x01;
	}

	for(i = 1; i < rxlen; ){

		for(j=0; j<NUM_HEAT_TC; j++){
			v_u16 = rxdata[i++] << 8;
			v_u16 |= rxdata[i++] << 0;
			if(p_dock){
				p_dock->tcs[j]->target_temperature = v_u16;
			}
		}
		break;
	}

	txbuff[txlen++] = dock_id;
	txbuff[txlen++] = status;
	can_replymsg(canid, txbuff, txlen);

	return 0;
}

static int pcr_settime(uint32_t canid, uint8_t *rxdata, uint8_t rxlen)
{
	int i, j;
	int dock_id;
	uint8_t txbuff[8];
	uint8_t txlen = 0;
	uint8_t offs = 0;

	heat_dock_st *p_dock;
	uint16_t v_u16;
	uint8_t status = 0;

	dock_id= rxdata[0];
	if(dock_id < NUM_DOCKS){
		p_dock = &heat_docks[dock_id];
	}else{
		p_dock = NULL;
		status = 0x01;
	}

	for(i = 1; i < rxlen; ){

		for(j=0; j<NUM_HEAT_TC; j++){
			v_u16 = rxdata[i++] << 8;
			v_u16 |= rxdata[i++] << 0;
			if(p_dock){
				p_dock->tcs[j]->duration = v_u16 * 1000;
			}
		}
		break;

	}

	txbuff[txlen++] = dock_id;
	txbuff[txlen++] = status;
	can_replymsg(canid, txbuff, txlen);

	return 0;
}


static int pcr_inplace(uint32_t canid, uint8_t *rxdata, uint8_t rxlen)
{
	int i;
	int dock_id;
	uint8_t txbuff[8];
	uint8_t txlen = 0;
	uint8_t status = 0;

	for(i = 0; i < rxlen; i++){
		dock_id= rxdata[i];
		 if(dock_id < NUM_DOCKS){
			txbuff[txlen++] = (heat_docks[dock_id].in_place) | (dock_id << 1);
		}else{
			status |= 0x01 << i;
			txbuff[txlen++] = 0;
		}
	}

	txbuff[txlen++] = status;

	can_replymsg(canid, txbuff, txlen);

	return 0;
}

void can_user_RxCallback(CAN_HandleTypeDef *hcan, CAN_RxHeaderTypeDef *pHead, uint8_t *rxbuff)
{
	int ret = 0;
	uint32_t canid;
	uint8_t *rxdata;
	uint8_t rxlen;

	if(pHead->RTR || pHead->DLC==0){
		return;
	}

	if(pHead->IDE){
		canid = pHead->ExtId;
	}else{
		canid = pHead->StdId;
	}

	rxdata = rxbuff;
	rxlen = pHead->DLC;

	printfln("can_user_RxCallback : %#x canid, %d bytes", canid, rxlen);

	switch(canid & 0xff){
	case  PCR_STARTHEAT :
		ret = pcr_startheat(canid, rxdata, rxlen);
		break;
	case  PCR_STOPHEAT :
		ret = pcr_stopheat(canid, rxdata, rxlen);
		break;
	case  PCR_CURTEMP :
		ret = pcr_curtemp(canid, rxdata, rxlen);
		break;
	case  PCR_GETTEMP :
		ret = pcr_gettemp(canid, rxdata, rxlen);
		break;
	case  PCR_SETTEMP :
		ret = pcr_settemp(canid, rxdata, rxlen);
		break;
	case  PCR_GETTIME :
		ret = pcr_gettime(canid, rxdata, rxlen);
		break;
	case  PCR_SETTIME :
		ret = pcr_settime(canid, rxdata, rxlen);
		break;
	case  PCR_INPLACE :
		ret = pcr_inplace(canid, rxdata, rxlen);
		break;

	}

}
