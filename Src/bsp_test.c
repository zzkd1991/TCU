#include "bsp_can.h"
#include "bsp_output_ao.h"
#include "bsp_input_di.h"
#include "usart.h"
#include "tle7242.h"
#include "main.h"
#include "bsp_test.h"
#include "usart_queue.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern struct _USARTQueue USARTQueueRx;
extern uint32_t DI_Screen(void);
extern void Uart_SendString(uint8_t *str);
char RecvElement[50] = {0};


void Test_Function_Can1_Send(void)
{
	uint8_t i = 0;
	//uint32_t message_id = 0;
	Message m = {0};
	uint8_t Send_Buffer[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00, 0x00};

	memcpy(m.data, Send_Buffer, 8);
	m.len = 8;

	for(i = 0; i < 10; i++)
	{
		m.cob_id = i + 0x10;		
		m.rtr = 0;
		m.ide = 0;
		Can_Send(NULL, &m, &hcan1);
	}

	for(i = 0; i < 10; i++)
	{
		m.cob_id = i + 0x20;
		m.rtr = 0;
		m.ide = 1;
		Can_Send(NULL, &m, &hcan1);
	}

	for(i = 0; i < 10; i++)
	{
		m.cob_id = i + 0x30;
		m.rtr = 1;
		m.ide = 0;
		Can_Send(NULL, &m, &hcan1);
	}

	for(i = 0; i < 10; i++)
	{
		m.cob_id = i + 0x40;
		m.rtr = 1;
		m.ide = 1;
		Can_Send(NULL, &m, &hcan1);
	}	
}

void Test_Function_Can2_Send(void)
{
	uint8_t i = 0;
	//uint32_t message_id = 0;
	Message m = {0};
	uint8_t Send_Buffer[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00, 0x00};

	memcpy(m.data, Send_Buffer, 8);
	m.len = 8;

	for(i = 0; i < 10; i++)
	{
		m.cob_id = i + 0x10;		
		m.rtr = 0;
		m.ide = 0;
		Can_Send(NULL, &m, &hcan2);
	}

	for(i = 0; i < 10; i++)
	{
		m.cob_id = i + 0x20;
		m.rtr = 0;
		m.ide = 1;
		Can_Send(NULL, &m, &hcan2);
	}

	for(i = 0; i < 10; i++)
	{
		m.cob_id = i + 0x30;
		m.rtr = 1;
		m.ide = 0;
		Can_Send(NULL, &m, &hcan2);
	}

	for(i = 0; i < 10; i++)
	{
		m.cob_id = i + 0x40;
		m.rtr = 1;
		m.ide = 1;
		Can_Send(NULL, &m, &hcan2);
	}
}


void Test_Function_Can1_Receive(void)
{	
	uint32_t ID = 0x12;
	uint8_t len = 0;
	uint8_t Recv_Buffer[8] = {0};

	CAN1_ReceiveObj(ID, &len, Recv_Buffer);
}


void Test_Function_Can2_Receive(void)
{
	uint32_t ID = 0x15;
	uint8_t len = 0;
	uint8_t Recv_Buffer[8] = {0};

	CAN2_ReceiveObj(ID, &len, Recv_Buffer);
}

void Test_Function_USART_Send(void)
{
	uint8_t Send_Buffer[50] = "aaaabbbbbccccdddd";

	Uart_SendString(Send_Buffer);
}

void Test_Function_USART_Receive(void)
{
	uint16_t head;
	char element;
	int index = 0;
	
	head = USARTQueueRx.front;
	if(1 == GetUsartQueueRx(head, &element))
	{
			head = (head + 1) % MAX_USART_SIZE;
			SetHeadUsartQueueRx(head);
			RecvElement[index] = element;
			index++;
			if(index == 49)
			{
				index = 0;			
			}
	}
}

void Test_Function_Output_AO(void)
{
	uint8_t Diag_Result = 0;
	AO_outVoltage(1500);
	
	Diag_Result = AO_Diag_State_Get();
}

void Test_Function_DI_Screen(void)
{
	uint8_t i = 0;

	for(i = 0; i < 21; i++)
	{
		API_DI_Cfg(i, 1);
	}

	while(1)
	{
		DI_Screen();
	}
}

void Test_Function_Fram_WR(void)
{
	

}



