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
uint8_t IsEmptyCan1QueueRx(void);
uint8_t IsEmptyCan2QueueRx(void);
uint8_t IsFullCan1QueueRx(void);
uint8_t IsFullCan2QueueRx(void);
uint8_t IsFullCanQueueTx(void);
uint8_t InsertCan1QueueRx(Message element);
uint8_t InsertCan2QueueRx(Message element);
uint8_t InsertCanQueueTx(Message head);
void SetHeadCan1QueueRx(uint16_t head);
void SetHeadCan2QueueRx(uint16_t head);
void SetHeadCanQueueTx(uint16_t head);
uint8_t GetCan1QueueRx(uint16_t head, Message *element);
uint8_t GetCan2QueueRx(uint16_t head, Message * element);
uint8_t GetCanQueueTx(uint16_t head, Message *element);
void Can_data_process(void);
void ClearCan1TxQueue(void);
void ClearCan2TxQueue(void);
void ClearCanRxQueue(void);


#endif



