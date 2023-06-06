#include "can_queue.h"
#include <string.h>

struct _CANQueue CAN1QueueRx;

struct _CANQueue CAN2QueueRx;

struct _CANQueue CANQueueTx;

void ClearCan1RxQueue(void)
{
	int i;
	for(i = 0; i < MAX_CAN_SIZE; i++)
	{
		memset(&CAN1QueueRx.Elem[i], 0, sizeof(Message));
	}

	CAN1QueueRx.front = 0;
	CAN1QueueRx.rear = 0;
}

void ClearCan2RxQueue(void)
{
	int i;
	for(i = 0; i < MAX_CAN_SIZE; i++)
	{
		memset(&CAN2QueueRx.Elem[i], 0, sizeof(Message));
	}

	CAN2QueueRx.front = 0;
	CAN2QueueRx.rear = 0;
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
	ClearCan1RxQueue();
	ClearCan2RxQueue();
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

uint8_t IsEmptyCan1QueueRx(void)
{
	if(CAN1QueueRx.front == CAN1QueueRx.rear)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t IsEmptyCan2QueueRx(void)
{
	if(CAN2QueueRx.front == CAN2QueueRx.rear)
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

uint8_t IsFullCan1QueueRx(void)
{
	if(CAN1QueueRx.front == (CAN1QueueRx.rear + 1) % MAX_CAN_SIZE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t IsFullCan2QueueRx(void)
{
	if(CAN2QueueRx.front == (CAN2QueueRx.rear + 1) % MAX_CAN_SIZE)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


uint8_t InsertCan1QueueRx(Message element)
{
	if(!IsFullCan1QueueRx())
	{
		memcpy(&CAN1QueueRx.Elem[CAN1QueueRx.rear], &element, sizeof(Message));
		CAN1QueueRx.rear = (CAN1QueueRx.rear + 1) % MAX_CAN_SIZE;
		return 1;
	}
	else	//队列满
	{
		return 0;
	}
}

uint8_t InsertCan2QueueRx(Message element)
{
	if(!IsFullCan2QueueRx())
	{
		memcpy(&CAN2QueueRx.Elem[CAN2QueueRx.rear], &element, sizeof(Message));
		CAN2QueueRx.rear = (CAN2QueueRx.rear + 1) % MAX_CAN_SIZE;
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

void SetHeadCan1QueueRx(uint16_t head)
{
	if(CAN1QueueRx.front != CAN1QueueRx.rear)
	{
		CAN1QueueRx.front = head;
	}
}

void SetHeadCan2QueueRx(uint16_t head)
{
	if(CAN2QueueRx.front != CAN2QueueRx.rear)
	{
		CAN2QueueRx.front = head;
	}
}


void SetHeadCanQueueTx(uint16_t head)
{
	if(CANQueueTx.front != CANQueueTx.rear)
	{
		CANQueueTx.front = head;
	}
}

uint8_t GetCan1QueueRx(uint16_t head, Message *element)
{
	if(head != CAN1QueueRx.rear)		//到队列尾
	{
		memcpy(element, &CAN1QueueRx.Elem[head], sizeof(Message));//得到数据
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t GetCan2QueueRx(uint16_t head, Message *element)
{
	if(head != CAN2QueueRx.rear)		//到队列尾
	{
		memcpy(element, &CAN2QueueRx.Elem[head], sizeof(Message));//得到数据
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
	uint16_t head1;
	uint16_t head2;
	Message RxMessage;
	extern int NEW_Can_Message_Dispatch(Message *m);
	
	head1 = CAN1QueueRx.front;
	head2 = CAN2QueueRx.front;
	
	if(1 == GetCan1QueueRx(head1, &RxMessage))
	{
		head1 = (head1 + 1) % MAX_CAN_SIZE;
		SetHeadCan1QueueRx(head1);

		//消息处理
		//printf("%s, %d\n", __func__, __LINE__);
		//NEW_Can_Message_Dispatch(&RxMessage);
	}
	else
	{
		//printf("CAN queue is empty\r\n");
	}

	if(1 == GetCan2QueueRx(head2, &RxMessage))
	{
		head2 = (head2 + 1) % MAX_CAN_SIZE;
		SetHeadCan2QueueRx(head1);

		//消息处理
		//printf("%s, %d\n", __func__, __LINE__);
		//NEW_Can_Message_Dispatch(&RxMessage);
	}
	else
	{
		//printf("CAN queue is empty\r\n");
	}	
}
















