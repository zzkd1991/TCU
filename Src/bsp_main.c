#include "bsp_main.h"

uint8_t Time_1ms_Flag, Time_5ms_Flag, Time_10ms_Flag, Time_20ms_Flag,
Time_50ms_Flag, Time_100ms_Flag, Time_500ms_Flag, Time_1000ms_Flag,
Time_2000ms_Flag;

uint8_t Time_Bsp_1ms_Flag, Time_Bsp_5ms_Flag, Time_Bsp_10ms_Flag, Time_Bsp_100ms_Flag;

extern void ccpBootInit (int cro_id, int dto_id);
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;

uint16_t ADC_ConvertedValue[6];
uint16_t _ADC_ConvertedValue[10];
uint32_t ACT_ADC_ConvertedValue[6];
uint32_t _ACT_ADC_ConvertedValue[10];
uint16_t ADC_AVG[6];//adc1
uint16_t _ADC_AVG[10];//adc3 
uint16_t ADC_FINAL[20];


void Driver_Init(void)
{
	/*led */
	LED_GPIO_Config();
	/*tle7242*/
	TLE7242_GPIO_Init();
	/*AI*/
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_ADC3_Init();
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_ConvertedValue, 6);
	HAL_ADC_Start_DMA(&hadc3,(uint32_t *)_ADC_ConvertedValue, 10);	
	DEBUG_UART_Config();
	ClearUsartQueue();
	MX_SPI2_Init();
	MX_SPI3_Init();
	
	/*DO*/
	API_DO_FY_Cfg();
	API_OUT_DO_IN_Cfg();
	API_OUT_Do_Cfg(0, 0);
	
	/*AO*/
	bsp_DAC_Init();
	
	/*DI*/
	Bsp_Di_Init();
	
	//Pi_Drv_Init();
	//mcp2515_hw_init();
	
	/*PI*/
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  MX_TIM8_Init();
  MX_TIM9_Init();
  MX_TIM12_Init();
	/*Fram */
	I2C_Fram_Init();

	ClearCanQueue();
	//init_timer_0(10);
	/*CAN1 + CAN2*/
	//ccpBootInit(CCP_CRO_ID, CCP_DTO_ID);

	printf("Init.....OK!!!\r\n");
}

void bsp_driver_task_loop(void)
{
	if(Time_Bsp_1ms_Flag)
	{
		bsp_driver_task_1ms();
		Time_Bsp_1ms_Flag = 0;
	}

	if(Time_Bsp_5ms_Flag)
	{
		bsp_driver_task_5ms();
		Time_Bsp_5ms_Flag = 0;
	}
	
	if(Time_10ms_Flag)
	{
		bsp_driver_task_10ms();
		Time_Bsp_10ms_Flag = 0;	
	}

	if(Time_100ms_Flag)
	{
		bsp_driver_task_100ms();
		Time_Bsp_100ms_Flag = 0;	
	}	
}

void bsp_driver_task_1ms(void)
{

}


void bsp_driver_task_5ms(void)
{

	
}

void bsp_driver_task_10ms(void)
{


}


void bsp_driver_task_100ms(void)
{


}


