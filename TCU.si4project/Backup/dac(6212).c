#include "dac.h"

extern void Error_Handler(void);

DMA_HandleTypeDef DMA_DAC_Init_Handle;

const uint16_t Sine12bit[POINT_NUM] = {
	2014, 2460, 2856, 3218, 3532, 3786, 3969, 4072,
	4093, 4031, 3887, 3668, 3382, 3042,	2661, 2255, 
	1841, 1435, 1054, 714, 428, 209	, 65, 3 ,
	24, 127, 310, 564, 878, 1240, 1636, 2048
};

uint32_t DualSine12bit[POINT_NUM];


DAC_HandleTypeDef DAC1_Handler;

void DAC1_Init(void)
{
	DAC_ChannelConfTypeDef DACCH1_Config;

	DAC1_Handler.Instance = DAC;
	HAL_DAC_Init(&DAC1_Handler);

	DACCH1_Config.DAC_Trigger = DAC_TRIGGER_T6_TRGO;//定时器6触发
	DACCH1_Config.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
	HAL_DAC_ConfigChannel(&DAC1_Handler, &DACCH1_Config, DAC_CHANNEL_1);

	if(HAL_OK != HAL_DAC_Start(&DAC1_Handler, DAC_CHANNEL_1))
	{
		Error_Handler();
	}
}

void HAL_DAC_MspInit(DAC_HandleTypeDef *hdac)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_DAC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_Initure.Pin = GPIO_PIN_4;
	GPIO_Initure.Mode = GPIO_MODE_ANALOG;
	GPIO_Initure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &GPIO_Initure);
}

void DAC1_Set_Vol(uint16_t vol)
{
	float temp = vol;
	temp /= 1000;
	temp = temp * 4096 / 3.3f;

	HAL_DAC_SetValue(&DAC1_Handler, DAC_CHANNEL_1, DAC_ALIGN_12B_R, temp);
}

void TIM_DAC_Mode_Config(void)
{
	TIMER_DAC_CLK_ENABLE(); 	

	//定时器时钟源TIMxCLK = 2 * PCLK1
	//				PCLK1 = HCLK / 4
	//				TIMxCLK = HCLK / 2 = SystemCoreClock / 2 = 84MHz
	TIM6_TimeBaseStructure.Instance = TIMER_DAC;
	TIM6_TimeBaseStructure.Init.Prescaler = TIMER_DAC_PRESCALER;
	TIM6_TimeBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
	TIM6_TimeBaseStructure.Init.Period = TIMER_DAC_PERIOD;
	TIM6_TimeBaseStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_Base_Init(&TIM6_TimeBaseStructure);

	if(HAL_OK != HAL_TIM_Base_Start_DMA(&TIM6_TimeBaseStructure, (uint32_t *)&DualSine12bit[0], sizeof(DualSine12bit[0])))
	{
		Error_Handler();
	}

}

void DAC_DMA_Config(void)
{
	HAL_StatusTypeDef DMA_status = HAL_ERROR;
	TIMER_DAC_DMA_CLK_ENABLE();
	
	DMA_DAC_Init_Handle.Instance = TIMER_DAC_DMA_STREAM;
	DMA_DAC_Init_Handle.Init.Direction = DMA_MEMORY_TO_PERIPH;//内存到外设
	DMA_DAC_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_DAC_Init_Handle.Init.MemInc = DMA_MINC_ENABLE;
	DMA_DAC_Init_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
	DMA_DAC_Init_Handle.Init.MemDataAlignment = DMA_PDATAALIGN_WORD;
	
	DMA_DAC_Init_Handle.Init.Mode = DMA_CIRCULAR;
	DMA_DAC_Init_Handle.Init.Priority = DMA_PRIORITY_HIGH;
	DMA_DAC_Init_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	DMA_DAC_Init_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
	DMA_DAC_Init_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
	DMA_DAC_Init_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;
	
	DMA_DAC_Init_Handle.Init.Channel = TIMER_DAC_DMA_CHANNEL;
	
	HAL_DMA_Init(&DMA_DAC_Init_Handle);
	
	DMA_status = HAL_DMA_Start(&DMA_DAC_Init_Handle, (uint32_t)DualSine12bit, (uint32_t)TIMER_DAC_DMA_DST_ADDR, sizeof(DualSine12bit[0]));
	if(DMA_status != HAL_OK)
	{
		Error_Handler();
	}
}

void DAC_Mode_Init(void)
{
	uint32_t Idx = 0;

	TIM_DAC_Mode_Config();

	DAC1_Init();

	for(Idx = 0; Idx < POINT_NUM; Idx++)
	{
		DualSine12bit[Idx] = (Sine12bit[Idx] << 16) + (Sine12bit[Idx]);
	}

	DAC_DMA_Config();
}



