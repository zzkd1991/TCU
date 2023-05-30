#ifndef CAN_QUEUE_H_
#define CAN_QUEUE_H_

#include <stdint.h>
#include "bsp_can.h"

#define MAX_CAN_SIZE	50

struct _CANQueue
{
	Message Elem[MAX_CAN_SIZE];
	uint16_t front;
	uint16_t rear;
};

void ClearCanQueue(void);
uint8_t IsEmptyCanQueueTx(void);
uint8_t IsEmptyCanQueueRx(void);
uint8_t IsFullCanQueueRx(void);
uint8_t IsFullCanQueueTx(void);
uint8_t InsertCanQueueRx(Message head);
uint8_t InsertCanQueueTx(Message head);
void SetHeadCanQueueRx(uint16_t head);
void SetHeadCanQueueTx(uint16_t head);
uint8_t GetCanQueueRx(uint16_t head, Message *element);
uint8_t GetCanQueueTx(uint16_t head, Message *element);
void Can_data_process(void);
void ClearCanTxQueue(void);
void ClearCanRxQueue(void);


#endif



