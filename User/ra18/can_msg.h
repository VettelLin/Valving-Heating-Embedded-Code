/*
 * can_msg.h
 *
 *  Created on: May 9, 2025
 *      Author: abel
 */

#ifndef RA18_CAN_MSG_H_
#define RA18_CAN_MSG_H_

#include "app/u_can.h"

#include "com.h"
#include "iamp_msg.h"

static inline int can_sendmsg(uint16_t canId, uint8_t *txbuff, uint8_t dsize)
{

	printf("can_sendmsg : id %#x, %d bytes\r\n", canId, dsize);
	return can_sendStdMessage(&hcan, canId | (com_id << 8), txbuff, dsize);
}

static inline int can_replymsg(uint16_t canId, uint8_t *txbuff, uint8_t dsize)
{
	return can_sendmsg(canId | 0x80, txbuff, dsize);
}

#endif /* RA18_CAN_MSG_H_ */
