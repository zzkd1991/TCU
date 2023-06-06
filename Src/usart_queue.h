#ifndef USART_QUEUE_H_
#define USART_QUEUE_H_

#include <stdint.h>
#include "usart_queue.h"

#define MAX_USART_SIZE 50

struct _USARTQueue
{
	char message[MAX_USART_SIZE];
	uint16_t front;
	uint16_t rear;
};

void ClearUsartRxQueue(void);
void ClearUsartTxQueue(void);
void ClearUsartQueue(void);
uint8_t IsEmptyUsartQueueTx(void);
uint8_t IsEmptyUsartQueueRx(void);
uint8_t IsFullUsartQueueTx(void);
uint8_t IsFullUsartQueueRx(void);
uint8_t InsertUsartQueueRx(char element);
uint8_t InsertUsartQueueTx(char element);
void SetHeadUsartQueueTx(uint16_t head);
void SetHeadUsartQueueRx(uint16_t head);
uint8_t GetUsartQueueRx(uint16_t head, char * element);
uint8_t GetUsartQueueTx(uint16_t head, char *element);



#endif



