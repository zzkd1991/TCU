#include "can_queue.h"
#include <string.h>

struct _CANQueue CANQueueRx;

struct _CANQueue CANQueueTx;

void ClearCanRxQueue(void)
{
	int i;
	for(i = 0; i < MAX_CAN_SIZE; i++)
	{
		memset(&CANQueueRx.Elem[i], 0, sizeof(Message));
	}

	CANQueueRx.front = 0;
	CANQueueRx.rear = 0;
}

void ClearCanTxQueue(void)
{
	int i;
	for(i = 0; i < MAX_CAN_SIZE; i++)
	{
		memset(&CANQueueTx.Elem[i], 0, sizeof(Message));
	}

	CANQueueTx.front = 0;
	CANQueueTx.rear = 0;
}

void ClearCanQueue(void)
{
	ClearCanRxQueue();
	ClearCanTxQueue();
}

uint8_t IsEmptyCanQueueTx(void)
{
	if(CANQueueTx.front == CANQueueTx.rear)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t IsEmptyCanQueueRx(void)
{
	if(CANQueueRx.front == CANQueueRx.rear)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t IsFullCanQueueTx(void)
{
	if(CANQueueTx.front == (CANQueueTx.rear + 1) % MAX_CAN_SIZE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t IsFullCanQueueRx(void)
{
	if(CANQueueRx.front == (CANQueueRx.rear + 1) % MAX_CAN_SIZE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t InsertCanQueueRx(Message element)
{
	if(!IsFullCanQueueRx())
	{
		memcpy(&CANQueueRx.Elem[CANQueueRx.rear], &element, sizeof(Message));
		CANQueueRx.rear = (CANQueueRx.rear + 1) % MAX_CAN_SIZE;
		return 1;
	}
	else	//队列满
	{
		return 0;
	}
}

uint8_t InsertCanQueueTx(Message element)
{
	if(!IsFullCanQueueTx())
	{
		memcpy(&CANQueueTx.Elem[CANQueueTx.rear], &element, sizeof(Message));
		CANQueueTx.rear = (CANQueueTx.rear + 1) % MAX_CAN_SIZE;
		return 1;
	}
	else
	{
		return 0;
	}
}

void SetHeadCanQueueRx(uint16_t head)
{
	if(CANQueueRx.front != CANQueueRx.rear)
	{
		CANQueueRx.front = head;
	}
}

void SetHeadCanQueueTx(uint16_t head)
{
	if(CANQueueTx.front != CANQueueTx.rear)
	{
		CANQueueTx.front = head;
	}
}

uint8_t GetCanQueueRx(uint16_t head, Message *element)
{
	if(head != CANQueueRx.rear)		//到队列尾
	{
		memcpy(element, &CANQueueRx.Elem[head], sizeof(Message));//得到数据
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t GetCanQueueTx(uint16_t head, Message *element)
{
	if(head != CANQueueTx.rear)
	{
		memcpy(element, &CANQueueTx.Elem[head], sizeof(Message));
		return 1;
	}
	else
	{
		return 0;
	}
}

void Can_data_Process(void)
{
	uint16_t head;
	Message RxMessage;
	extern int NEW_Can_Message_Dispatch(Message *m);
	
	head = CANQueueRx.front;
	
	if(1 == GetCanQueueRx(head, &RxMessage))
	{
		head = (head + 1) % MAX_CAN_SIZE;
		SetHeadCanQueueRx(head);

		//消息处理
		//printf("%s, %d\n", __func__, __LINE__);
		//NEW_Can_Message_Dispatch(&RxMessage);
	}
	else
	{
		//printf("CAN queue is empty\r\n");
	}
}
















