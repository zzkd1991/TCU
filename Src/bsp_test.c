#include "bsp_can.h"
#include "bsp_output_ao.h"
#include "bsp_input_di.h"
#include "usart.h"
#include "tle7242.h"
#include "main.h"
#include "bsp_test.h"
#include "led.h"
#include "usart_queue.h"
#include "bsp_output_docc.h"
#include "fm24cl64b_i2c.h"
#include "bsp_par.h"

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;
extern struct _USARTQueue USARTQueueRx;
extern void DI_Screen(void);
extern void Uart_SendString(uint8_t *str);
char RecvElement[50] = {0};
U16 wordpar_store[512] = {0};
U8 bytepar_store[512] = "1112223332343254325234234242342345634566776567";
REAL32 realpar_store[512] = {0};


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

void Test_Function_Can3_Send(void)
{
	uint8_t i = 0;
	Message m = {0};
	uint8_t Send_Buffer[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x00, 0x00, 0x00};

	memcpy(m.data, Send_Buffer, 8);
	m.len = 8;

	for(i = 0; i < 10; i++)
	{
		m.cob_id = i + 0x10;
		m.rtr = 0;
		m.ide = 0;
		CanInsert_SendQueue(NULL, &m);
	}

	for(i = 0; i < 10; i++)
	{
		m.cob_id = i + 0x20;
		m.rtr = 0;
		m.ide = 1;
		CanInsert_SendQueue(NULL, &m);
	}

	for(i = 0; i < 10; i++)
	{
		m.cob_id = i + 0x30;
		m.rtr = 1;
		m.ide = 0;
		CanInsert_SendQueue(NULL, &m);
	}

	for(i = 0; i < 10; i++)
	{
		m.cob_id = i + 0x40;
		m.rtr = 1;
		m.ide = 1;
		CanInsert_SendQueue(NULL, &m);
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
	extern U8 PAR_BYTES[Max_BYTE_NUM];
	extern uint16_t PAR_WORDS[Max_WORD_NUM];
	U8 result = 0;


	f32_sep write_data = {-32.65};
	f32_sep read_data = {0};
	int i;
	REAL32 write_data1 = 12.1;
	REAL32 read_data1 = 0;
	
#if 1
	wordpar_store[0] = 'a',
	wordpar_store[1] = 'b';
	wordpar_store[2] = 'c';
	wordpar_store[3] = 'd';
	wordpar_store[4] = 'e';
	wordpar_store[5] = 'f';
	wordpar_store[6] = '1';
	wordpar_store[7] = '2';
	wordpar_store[8] = '3';
	wordpar_store[9] = '4';
	wordpar_store[10] = 's';
	wordpar_store[11] = '2';
	wordpar_store[12] = '1';
	wordpar_store[13] = 'z';
	wordpar_store[14] = 'x';
	wordpar_store[15] = 's';
	wordpar_store[16] = 'v';
	wordpar_store[17] = 'b';
	wordpar_store[18] = 'g';
	wordpar_store[19] = 'h';
	wordpar_store[20] = 't';
	wordpar_store[21] = ',';
	wordpar_store[22] = '4';
	wordpar_store[23] = '5';
	wordpar_store[24] = '2';
	wordpar_store[25] = 'r';
	wordpar_store[26] = 'y';
	wordpar_store[27] = 'r';
	wordpar_store[28] = 's';
	wordpar_store[29] = 'q';
	wordpar_store[30] = 'o';
	wordpar_store[31] = 'p';
	wordpar_store[32] = 'l';
	wordpar_store[33] = '6';
	wordpar_store[34] = '0';
	wordpar_store[35] = '1';
	wordpar_store[36] = '2';
	wordpar_store[37] = 'r';
	wordpar_store[38] = 's';
	wordpar_store[39] = 'q';
	wordpar_store[40] = 'o';
	wordpar_store[41] = 'p';
	wordpar_store[42] = 'l';
	wordpar_store[43] = '6';
	wordpar_store[44] = '0';
	wordpar_store[45] = '1';
	wordpar_store[46] = '2';
	wordpar_store[47] = '3';
	wordpar_store[48] = '4';
	wordpar_store[49] = '5';
	wordpar_store[50] = ';';
	wordpar_store[60] = '*';
	wordpar_store[500] = '%';

#if 1
	realpar_store[0] = 35.1;
	realpar_store[1] = -12.1;
	realpar_store[2] = 2.1;
	realpar_store[3] = 22.1;
	realpar_store[4] = 11.1;
	realpar_store[5] = -0.1;
	realpar_store[6] = 1.1;
	realpar_store[7] = 34.1;
	realpar_store[8] = 2.1;
	realpar_store[9] = 11.1;
	realpar_store[10] = 13.1;
	realpar_store[11] = 1.1;
	realpar_store[12] = 0.1;
	realpar_store[13] = -12.1;
	realpar_store[14] = -111.7;
	realpar_store[15] = 99.1;
	realpar_store[16] = 898.1;
	realpar_store[17] = 10.1;
	realpar_store[18] = 100.1;
	realpar_store[19] = -1299.1;
	realpar_store[20] = -100;
	realpar_store[21] = -89.1;
	realpar_store[22] = -35.1;
	realpar_store[23] = 9.1;
	realpar_store[24] = -120.1;
	realpar_store[25] = -101.7;
	realpar_store[26] = 56.1;
	realpar_store[27] = 898.1;
	realpar_store[28] = 100.1;
	realpar_store[29] = 99.1;
	realpar_store[30] = -15.61;
	realpar_store[31] = -99.82;
	realpar_store[32] = 732;
	realpar_store[33] = 54.12;
	realpar_store[34] = 78.98;
	realpar_store[35] = 76.98;
	realpar_store[36] = 54.21;
	realpar_store[37] = -23.45;
	realpar_store[38] = -67.89;
	realpar_store[39] = 98.678;
	realpar_store[40] = 76.98;
	realpar_store[41] = 76.99;
	realpar_store[42] = 98.99;
	realpar_store[43] = 65;
	realpar_store[44] = 24.789;
	realpar_store[45] = 45.78l;
	realpar_store[46] = 1234;
	realpar_store[47] = 54.67;
	realpar_store[48] = 98.76;
	realpar_store[49] = 77.67;
	realpar_store[50] = 71.11;
	realpar_store[51] = 66.78;
	realpar_store[52] = 89.88;
	realpar_store[53] = 54.44;
	realpar_store[54] = 76.99;
	realpar_store[55] = 12.31;
#if 1
	realpar_store[101] = 123.45;
#else
	realpar_store[101] = 123.45;
#endif
	/*realpar_store[103] = 67.88;
	realpar_store[150] = 776.98;
	realpar_store[200] = 15.25;
	realpar_store[246] = 67.89;
	realpar_store[267] = 66.78;
	realpar_store[312] = 57.90;
	realpar_store[324] = 99.89;
	realpar_store[412] = 34.67;
	realpar_store[415] = 89.99;
	realpar_store[500] = 56.908;
	realpar_store[510] = 78.998;*/

#else
	realpar_store[0] = 100;
	realpar_store[1] = 100;
	realpar_store[2] = 100;
	realpar_store[3] = 100;
	realpar_store[4] = 100;
	realpar_store[5] = 100;
	realpar_store[6] = 100;
	realpar_store[7] = 100;
	realpar_store[8] = 100;
	realpar_store[9] = 100;
	realpar_store[10] = 100;
	realpar_store[11] = 100;
	realpar_store[12] = 100;
	realpar_store[13] = 100;
	realpar_store[14] = 100;

#endif


	/*printf("************************\r\n");

	I2C_Fram_BufferWrite((uint8_t *)&write_data1, ParBack_REAL_BASE, sizeof(write_data1));
	
	I2C_Fram_BufferRead((uint8_t *)&read_data1, ParBack_REAL_BASE, sizeof(read_data1));
	
	if(read_data1 == write_data1)
	{
		printf("11111");
	}
	else
	{
		printf("22222");
	}

	printf("read_data1 %f\r\n", read_data1);*/
	 
	 
	printf("************************\r\n");
   

	result = API_PAR_Data_Init(PAR_TYPE_BYTE, bytepar_store, 512);
	printf("result %d\r\n", result);
	
	result = API_PAR_Data_Init(PAR_TYPE_WORD, (U8 *)wordpar_store, 512);
	printf("result %d\r\n", result);

	result = API_PAR_Data_Init(PAR_TYPE_DWORD, (U8 *)&realpar_store[0], 512);
	printf("result %d\r\n", result);
	
	API_PAR_Init();
	/*printf("PAR_BYTES %s\r\n", PAR_BYTES);
	printf("PAR_WORDS\r\n");
	for(i = 0; i < 512; i++)
	{
		printf("%c", PAR_WORDS[i]);
	}*/

	printf("\r\n");
	for(i = 0; i < 100; i++)
	{
		printf("%f\r\n", PAR_REALS[i]);
	}
#endif

}

void Test_Function_LED(void)
{

	Api_Led_Color_Blink_Freq(LED1, LED_COLOR_GREEN, LED_COLOR_MAGENTA, 1);

	Api_Led_Color_Blink_Freq(LED2, LED_COLOR_RED, LED_COLOR_MAGENTA, 2);
}

void Test_Function_TLE7242_PO(uint8_t channel)
{
	int i = 0;
	uint16_t current_value[12] = {0};
	uint16_t duty_value[12] = {0};

	//for(i = 0; i < 12; i++)
	//{
	//	API_PO_Mode_Config(i, TLE_MODE_CONST_CURRENT);
	//}
#if 1
	API_ConstantCurrent_Drive(channel, 500, 2000, 0.05, 0.05);
	HAL_Delay(1000);
	current_value[0] = API_DynamicCurrent_Read(channel);
	duty_value[0] = API_Duty_Feedback_Read(channel);
	printf("duty_value[0], current_value[0] %d %d\r\n", duty_value[0], current_value[0]);
	HAL_Delay(4000);


	API_ConstantCurrent_Drive(channel, 600, 2000, 0.05, 0.05);
	HAL_Delay(1000);
	current_value[1] = API_DynamicCurrent_Read(channel);
	duty_value[1] = API_Duty_Feedback_Read(channel);
	printf("duty_value[1], current_value[1] %d %d\r\n", duty_value[1], current_value[1]);
	HAL_Delay(4000);


	API_ConstantCurrent_Drive(channel, 700, 2000, 0.05, 0.05);
	HAL_Delay(1000);
	current_value[2] = API_DynamicCurrent_Read(channel);
	duty_value[2] = API_Duty_Feedback_Read(channel);
	printf("duty_value[2], current_value[2] %d %d\r\n", duty_value[2], current_value[2]);
	HAL_Delay(4000);


	API_ConstantCurrent_Drive(channel, 800, 2000, 0.05, 0.05);
	HAL_Delay(1000);
	current_value[3] = API_DynamicCurrent_Read(channel);
	duty_value[3] = API_Duty_Feedback_Read(channel);
	printf("duty_value[3], current_value[3] %d %d\r\n", duty_value[3], current_value[3]);
	HAL_Delay(4000);


	API_ConstantCurrent_Drive(channel, 900, 2000, 0.05, 0.05);
	HAL_Delay(1000);
	current_value[4] = API_DynamicCurrent_Read(channel);
	duty_value[4] = API_Duty_Feedback_Read(channel);
	printf("duty_value[4], current_value[4] %d %d\r\n", duty_value[4], current_value[4]);
	HAL_Delay(4000);


	API_ConstantCurrent_Drive(channel, 1000, 2000, 0.05, 0.05);
	HAL_Delay(1000);
	current_value[5] = API_DynamicCurrent_Read(channel);
	duty_value[5] = API_Duty_Feedback_Read(channel);
	printf("duty_value[0], current_value[5] %d %d\r\n", duty_value[5], current_value[5]);
	HAL_Delay(4000);

	/*for(i = 0; i < 10; i++)
	{
		
		API_Power_Switch_Set(channel, 1);
		//printf("%s, %d\r\n", __FUNCTION__, __LINE__);
		HAL_Delay(1000);
		API_Power_Switch_Set(channel, 0);
		HAL_Delay(1000);
	}*/

#endif


#if 0
	API_ConstantCurrent_Drive(PO1, 1100, 2000, 0.05, 0.05);
	HAL_Delay(1000);
	//current_value[6] = API_DynamicCurrent_Read(6);
	//duty_value[6] = API_Duty_Feedback_Read(6);
	HAL_Delay(4000);


	API_ConstantCurrent_Drive(PO1, 1200, 2000, 0.05, 0.05);
	HAL_Delay(1000);
	//current_value[7] = API_DynamicCurrent_Read(7);
	//duty_value[7] = API_Duty_Feedback_Read(7);
	HAL_Delay(4000);

	
	API_ConstantCurrent_Drive(PO1, 1300, 2000, 0.05, 0.05);
	HAL_Delay(1000);
	//current_value[8] = API_DynamicCurrent_Read(8);
	//duty_value[8] = API_Duty_Feedback_Read(8);
	HAL_Delay(4000);


	API_ConstantCurrent_Drive(PO1, 1400, 2000, 0.05, 0.05);
	HAL_Delay(1000);
	//current_value[9] = API_DynamicCurrent_Read(9);
	//duty_value[9] = API_Duty_Feedback_Read(9);
	HAL_Delay(4000);


	API_ConstantCurrent_Drive(PO1, 1500, 2000, 0.05, 0.05);
	HAL_Delay(1000);
	///current_value[10] = API_DynamicCurrent_Read(10);
	//duty_value[10] = API_Duty_Feedback_Read(10);
	HAL_Delay(4000);


	API_ConstantCurrent_Drive(PO1, 1600, 2000, 0.05, 0.05);
	HAL_Delay(1000);
	//current_value[11] = API_DynamicCurrent_Read(11);
	//duty_value[11] = API_Duty_Feedback_Read(11);
	HAL_Delay(4000);
#endif	
}

