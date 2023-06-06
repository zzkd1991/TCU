#include "usart_queue.h"
#include <string.h>

struct _USARTQueue USARTQueueRx;

struct _USARTQueue USARTQueueTx;

void ClearUsartRxQueue(void)
{
	memset(USARTQueueRx.message, 0, MAX_USART_SIZE);

	USARTQueueRx.front = 0;
	USARTQueueTx.rear = 0;
}

void ClearUsartTxQueue(void)
{
	memset(USARTQueueTx.message, 0, MAX_USART_SIZE);

	USARTQueueTx.front = 0;
	USARTQueueTx.rear = 0;
}

void ClearUsartQueue(void)
{
	ClearUsartRxQueue();
	ClearUsartTxQueue();
}

uint8_t IsEmptyUsartQueueTx(void)
{
	if(USARTQueueTx.front == USARTQueueTx.rear)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t IsEmptyUsartQueueRx(void)
{
	if(USARTQueueRx.front == USARTQueueRx.rear)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t IsFullUsartQueueTx(void)
{
	if(USARTQueueTx.front == (USARTQueueTx.rear + 1) % MAX_USART_SIZE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t IsFullUsartQueueRx(void)
{
	if(USARTQueueRx.front == (USARTQueueRx.rear + 1) % MAX_USART_SIZE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t InsertUsartQueueRx(char element)
{
	if(!IsFullUsartQueueRx())
	{
		memcpy(&USARTQueueRx.message[USARTQueueRx.rear], &element, 1);
		USARTQueueRx.rear = (USARTQueueRx.rear + 1) % MAX_USART_SIZE;
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t InsertUsartQueueTx(char element)
{
	if(!IsFullUsartQueueTx())
	{
		memcpy(&USARTQueueTx.message[USARTQueueTx.rear], &element, 1);
		USARTQueueTx.rear = (USARTQueueTx.rear + 1) % MAX_USART_SIZE;
		return 1;
	}
	else
	{
		return 0;
	}
}


void SetHeadUsartQueueRx(uint16_t head)
{
	if(USARTQueueRx.front != USARTQueueRx.rear)
	{
		USARTQueueRx.front = head;
	}
}

void SetHeadUsartQueueTx(uint16_t head)
{
	if(USARTQueueTx.front != USARTQueueTx.rear)
	{
		USARTQueueTx.front = head; 
	}
}


uint8_t GetUsartQueueRx(uint16_t head, char *element)
{
	if(head != USARTQueueRx.rear)
	{
		memcpy(element, &USARTQueueRx.message[head], 1);
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t GetUsartQueueTx(uint16_t head, char *element)
{
	if(head != USARTQueueTx.rear)
	{
		memcpy(element, &USARTQueueTx.message[head], 1);
		return 1;
	}
	else
	{
		return 0;
	}
}


