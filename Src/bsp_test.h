#ifndef BSP_TEST_H
#define BSP_TEST_H

#include <stdint.h>

typedef struct
{
	uint16_t CAN1_Band;
	uint16_t CAN2_Band;
	
}TEST_CAN_TYPE;

typedef struct
{
	uint8_t PO_Channel;
	uint16_t mode_set;
	uint16_t current_set;
	uint16_t current_read;
	uint16_t ki_set;
	uint16_t kp_set;
	uint16_t pwm_freq_set;
	uint16_t duty_read;
	uint16_t on_off_op;
}TEST_TLE7242_TYPE;


extern TEST_TLE7242_TYPE TLE7242_Test_Struct;
extern tag_TLE_Register TLE7242_Test_Register_Read;

void Test_Function_Can1_Send(void);
void Test_Function_Can2_Send(void);
void Test_Function_Can1_Receive(void);
void Test_Function_Can2_Receive(void);
void Test_Function_Init_Handle(void);
void Test_Function_USART_Send(void);
void Test_Function_USART_Receive(void);
void Test_Function_Output_AO(void);
void Test_Function_Output_PI(void);
void Test_Function_DI_Screen(void);
void Test_Function_Fram_WR(void);
void Test_Function_Output_DO(void);
void Test_Function_CAN_CCP(void);
void Test_Function_LED(void);
void Test_Function_CAN_Interface(void);
void Test_Function_PI_Double_Pulse(void);



#endif





