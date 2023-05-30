#include "temp_pres_drv.h"

uint16_t ADC_ConvertedValue[RHEOSTAT_NOFCHANEL] = {0};
uint16_t _ADC_ConvertedValue[RHEOSTAT_NOFCHANEL] = {0};
uint32_t ACT_ADC_ConvertedValue[RHEOSTAT_NOFCHANEL] = {0};
uint16_t ADC_AVG[RHEOSTAT_NOFCHANEL] = {0};
DMA_HandleTypeDef DMA_Init_Handle;
DMA_HandleTypeDef _DMA_Init_Handle;
ADC_HandleTypeDef ADC_Handle;
ADC_HandleTypeDef _ADC_Handle;
ADC_ChannelConfTypeDef ADC_Config;
uint16_t DMA_ATransfer_Complete_Count = 0;
uint16_t DMA_BTransfer_Complete_Count = 0;
uint16_t RWV_ACurrent_Cv = 50;//原值为63
uint16_t RWV_BCurrent_Cv = 50;

/*ADC DMA采样时间计算*/
/* 系统时钟：168M*/
/* ADC时钟84M（根据下列PLCK2时钟）*/
/*PCLK2 = HCLK2/2*/
/*ADC分频: 2*/
/*ADC时钟：42*/
/*采样间隔：10 cycles*/
/*采样时间：480 Cycles*/
/*采样通道数：9*/
/*采样时间= （10 + 480 + 9.5）* 9/42M = 107.04us*/
/*63次滤波，总时间= 107.04us * 63 = 6.74ms */


static void Rheostat_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//
	RHEOSTAT_ADC_GPIO_CLK1_ENABLE();
	GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN1;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT1, &GPIO_InitStructure);

//
	RHEOSTAT_ADC_GPIO_CLK2_ENABLE();
	GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN2;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT2, &GPIO_InitStructure);

//
	RHEOSTAT_ADC_GPIO_CLK3_ENABLE();
	GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN3;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;	
	HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT3, &GPIO_InitStructure);

//	
	RHEOSTAT_ADC_GPIO_CLK4_ENABLE();
	GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN4;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;	
	HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT4, &GPIO_InitStructure);

//	
	RHEOSTAT_ADC_GPIO_CLK5_ENABLE();
	GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN5;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT5, &GPIO_InitStructure);

//	
	RHEOSTAT_ADC_GPIO_CLK6_ENABLE();
	GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN6;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT6, &GPIO_InitStructure);

//	
	RHEOSTAT_ADC_GPIO_CLK7_ENABLE();
	GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN7;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT7, &GPIO_InitStructure);

//	
	RHEOSTAT_ADC_GPIO_CLK8_ENABLE();
	GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN8;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT8, &GPIO_InitStructure);

//	
	RHEOSTAT_ADC_GPIO_CLK9_ENABLE();
	GPIO_InitStructure.Pin = RHEOSTAT_ADC_GPIO_PIN9;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(RHEOSTAT_ADC_GPIO_PORT9, &GPIO_InitStructure);
}


static void _Rheostat_ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
//
	_RHEOSTAT_ADC_GPIO_CLK1_ENABLE();
	GPIO_InitStructure.Pin = _RHEOSTAT_ADC_GPIO_PIN1;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(_RHEOSTAT_ADC_GPIO_PORT1, &GPIO_InitStructure);

//
	_RHEOSTAT_ADC_GPIO_CLK2_ENABLE();
	GPIO_InitStructure.Pin = _RHEOSTAT_ADC_GPIO_PIN2;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(_RHEOSTAT_ADC_GPIO_PORT2, &GPIO_InitStructure);

	_RHEOSTAT_ADC_GPIO_CLK3_ENABLE();
	GPIO_InitStructure.Pin = _RHEOSTAT_ADC_GPIO_PIN3;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(_RHEOSTAT_ADC_GPIO_PORT3, &GPIO_InitStructure);

	_RHEOSTAT_ADC_GPIO_CLK4_ENABLE();
	GPIO_InitStructure.Pin = _RHEOSTAT_ADC_GPIO_PIN4;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(_RHEOSTAT_ADC_GPIO_PORT4, &GPIO_InitStructure);

	_RHEOSTAT_ADC_GPIO_CLK5_ENABLE();
	GPIO_InitStructure.Pin = _RHEOSTAT_ADC_GPIO_PIN5;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(_RHEOSTAT_ADC_GPIO_PORT5, &GPIO_InitStructure);
	
	_RHEOSTAT_ADC_GPIO_CLK6_ENABLE();
	GPIO_InitStructure.Pin = _RHEOSTAT_ADC_GPIO_PIN6;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(_RHEOSTAT_ADC_GPIO_PORT6, &GPIO_InitStructure);

	_RHEOSTAT_ADC_GPIO_CLK7_ENABLE();
	GPIO_InitStructure.Pin = _RHEOSTAT_ADC_GPIO_PIN7;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(_RHEOSTAT_ADC_GPIO_PORT7, &GPIO_InitStructure);

	_RHEOSTAT_ADC_GPIO_CLK8_ENABLE();
	GPIO_InitStructure.Pin = _RHEOSTAT_ADC_GPIO_PIN8;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(_RHEOSTAT_ADC_GPIO_PORT8, &GPIO_InitStructure);

	_RHEOSTAT_ADC_GPIO_CLK9_ENABLE();
	GPIO_InitStructure.Pin = _RHEOSTAT_ADC_GPIO_PIN9;
	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(_RHEOSTAT_ADC_GPIO_PORT9, &GPIO_InitStructure);	
}

static void _Rheostat_ADC_Mode_Config(void)
{
	ADC_MultiModeTypeDef mode;

	HAL_StatusTypeDef DMA_status = HAL_ERROR;

	_DMA_Init_Handle.Instance = _RHEOSTAT_ADC_DMA_STREAM;
	_DMA_Init_Handle.Init.Channel = _RHEOSTAT_ADC_DMA_CHANNEL;
	_DMA_Init_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	_DMA_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
	_DMA_Init_Handle.Init.MemInc = DMA_MINC_ENABLE;
	_DMA_Init_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	_DMA_Init_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;

	_DMA_Init_Handle.Init.Mode = DMA_CIRCULAR;
	_DMA_Init_Handle.Init.Priority = DMA_PRIORITY_LOW;
	_DMA_Init_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

	if(HAL_OK != HAL_DMA_Init(&_DMA_Init_Handle))
	{
		Error_Handler();
	}
	
	__HAL_LINKDMA(&_ADC_Handle, DMA_Handle, _DMA_Init_Handle);


	_RHEOSTAT_ADC_CLK_ENABLE();

	/* RHEOSTAT_ADC结构体初始化*/
	_ADC_Handle.Instance = _RHEOSTAT_ADC;

	_ADC_Handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;

	_ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;
	
	_ADC_Handle.Init.ScanConvMode = ENABLE;

	_ADC_Handle.Init.ContinuousConvMode = ENABLE;

	_ADC_Handle.Init.DiscontinuousConvMode = DISABLE;

	_ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;

	_ADC_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;//new

	_ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;

	_ADC_Handle.Init.NbrOfConversion = 9;

	_ADC_Handle.Init.DMAContinuousRequests = ENABLE;
	_ADC_Handle.Init.EOCSelection = ADC_EOC_SEQ_CONV;

	if(HAL_ADC_Init(&_ADC_Handle) != HAL_OK)
	{
		Error_Handler();
	}
	ADC_Config.Channel = _RHEOSTAT_ADC_CHANNEL1;
	ADC_Config.Rank = 1;
	//采样时间间隔为480个时钟周期
	ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	ADC_Config.Offset = 0;
	if(HAL_ADC_ConfigChannel(&_ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}

	ADC_Config.Channel = _RHEOSTAT_ADC_CHANNEL2;
	ADC_Config.Rank = 2;
	ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	ADC_Config.Offset = 0;
	if(HAL_ADC_ConfigChannel(&_ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}

	ADC_Config.Channel = _RHEOSTAT_ADC_CHANNEL3;
	ADC_Config.Rank = 3;
	ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	ADC_Config.Offset = 0;
	if(HAL_ADC_ConfigChannel(&_ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}

	ADC_Config.Channel = _RHEOSTAT_ADC_CHANNEL4;
	ADC_Config.Rank = 4;
	ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	ADC_Config.Offset = 0;
	if(HAL_ADC_ConfigChannel(&_ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}	

	ADC_Config.Channel = _RHEOSTAT_ADC_CHANNEL5;
	ADC_Config.Rank = 5;
	ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	ADC_Config.Offset = 0;
	if(HAL_ADC_ConfigChannel(&_ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}

	ADC_Config.Channel = _RHEOSTAT_ADC_CHANNEL6;
	ADC_Config.Rank = 6;
	ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	ADC_Config.Offset = 0;
	if(HAL_ADC_ConfigChannel(&_ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}

	ADC_Config.Channel = _RHEOSTAT_ADC_CHANNEL7;
	ADC_Config.Rank = 7;
	ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	ADC_Config.Offset = 0;
	if(HAL_ADC_ConfigChannel(&_ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}

	ADC_Config.Channel = _RHEOSTAT_ADC_CHANNEL8;
	ADC_Config.Rank = 8;
	ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	ADC_Config.Offset = 0;
	if(HAL_ADC_ConfigChannel(&_ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}	


	ADC_Config.Channel = _RHEOSTAT_ADC_CHANNEL9;
	ADC_Config.Rank = 9;
	ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	ADC_Config.Offset = 0;
	if(HAL_ADC_ConfigChannel(&_ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}		
}

static void Rheostat_ADC_Mode_Config(void)
{

	ADC_MultiModeTypeDef mode;

	HAL_StatusTypeDef DMA_status = HAL_ERROR;

	DMA_Init_Handle.Instance = RHEOSTAT_ADC_DMA_STREAM;
	DMA_Init_Handle.Init.Channel = RHEOSTAT_ADC_DMA_CHANNEL;
	DMA_Init_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
	DMA_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
	DMA_Init_Handle.Init.MemInc = DMA_MINC_ENABLE;
	DMA_Init_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	DMA_Init_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;

	DMA_Init_Handle.Init.Mode = DMA_CIRCULAR;
	DMA_Init_Handle.Init.Priority = DMA_PRIORITY_LOW;
	DMA_Init_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;

	if(HAL_OK != HAL_DMA_Init(&DMA_Init_Handle))
	{
		Error_Handler();
	}
	
	__HAL_LINKDMA(&ADC_Handle, DMA_Handle, DMA_Init_Handle);


	RHEOSTAT_ADC_CLK_ENABLE();
	//_RHEOSTAT_ADC_CLK_ENABLE();

	/* RHEOSTAT_ADC结构体初始化*/
	ADC_Handle.Instance = RHEOSTAT_ADC;

	ADC_Handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV2;

	ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;
	
	ADC_Handle.Init.ScanConvMode = ENABLE;

	ADC_Handle.Init.ContinuousConvMode = ENABLE;

	ADC_Handle.Init.DiscontinuousConvMode = DISABLE;

	ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;

	ADC_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;//new

	ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;

	ADC_Handle.Init.NbrOfConversion = 9;

	ADC_Handle.Init.DMAContinuousRequests = ENABLE;
	ADC_Handle.Init.EOCSelection = ADC_EOC_SEQ_CONV;

	if(HAL_ADC_Init(&ADC_Handle) != HAL_OK)
	{
		Error_Handler();
	}
	
	ADC_Config.Channel = RHEOSTAT_ADC_CHANNEL1;
	ADC_Config.Rank = 1;
	//采样时间间隔为480个时钟周期
	ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	ADC_Config.Offset = 0;
	if(HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}

	ADC_Config.Channel = RHEOSTAT_ADC_CHANNEL2;
	ADC_Config.Rank = 2;
	if(HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}
	
	ADC_Config.Channel = RHEOSTAT_ADC_CHANNEL3;
	ADC_Config.Rank = 3;
	if(HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}

	ADC_Config.Channel = RHEOSTAT_ADC_CHANNEL4;
	ADC_Config.Rank = 4;
	if(HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}

	ADC_Config.Channel = RHEOSTAT_ADC_CHANNEL5;
	ADC_Config.Rank = 5;
	if(HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}

	ADC_Config.Channel = RHEOSTAT_ADC_CHANNEL6;
	ADC_Config.Rank = 6;
	if(HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}

	ADC_Config.Channel = RHEOSTAT_ADC_CHANNEL7;
	ADC_Config.Rank = 7;
	if(HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}


	ADC_Config.Channel = RHEOSTAT_ADC_CHANNEL8;
	ADC_Config.Rank = 8;
	if(HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}

	ADC_Config.Channel = RHEOSTAT_ADC_CHANNEL9;
	ADC_Config.Rank = 9;
	if(HAL_ADC_ConfigChannel(&ADC_Handle, &ADC_Config) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

	//HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	
  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

  HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);  
}

#define QUEUE_LENGTH 50
#define ADC_CONVERTED_UPDATE_FREQ_UP	10
#define ADC_CONVERTED_CHANNEL_NUM	9
uint32_t ADC_Converted_Array[ADC_CONVERTED_CHANNEL_NUM][QUEUE_LENGTH] = {{0, 0}};
uint32_t ADC_Converted_Sum[ADC_CONVERTED_CHANNEL_NUM] = {0};
uint32_t ADC_Converted_cursor = 0;
uint32_t ADC_Converted_Update_Freq = 0;


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
#if 0
	uint16_t i;

	for(i = 0; i < 18; i++)
	{
		ACT_ADC_ConvertedValue[i] += ADC_ConvertedValue[i];
	}


	if(DMA_BTransfer_Complete_Count >= RWV_BCurrent_Cv)
	{
		ADC_AVG[0] = ACT_ADC_ConvertedValue[0] / (RWV_BCurrent_Cv + 1);
		ADC_AVG[1] = ACT_ADC_ConvertedValue[1] / (RWV_BCurrent_Cv + 1);
		ADC_AVG[2] = ACT_ADC_ConvertedValue[2] / (RWV_BCurrent_Cv + 1);
		ADC_AVG[3] = ACT_ADC_ConvertedValue[3] / (RWV_BCurrent_Cv + 1);
		ADC_AVG[4] = ACT_ADC_ConvertedValue[4] / (RWV_BCurrent_Cv + 1);
		ADC_AVG[5] = ACT_ADC_ConvertedValue[5] / (RWV_BCurrent_Cv + 1);
		ADC_AVG[6] = ACT_ADC_ConvertedValue[6] / (RWV_BCurrent_Cv + 1);

		ACT_ADC_ConvertedValue[0] = 0;
		ACT_ADC_ConvertedValue[1] = 0;
		ACT_ADC_ConvertedValue[2] = 0;
		ACT_ADC_ConvertedValue[3] = 0;
		ACT_ADC_ConvertedValue[4] = 0;
		ACT_ADC_ConvertedValue[5] = 0;
		ACT_ADC_ConvertedValue[6] = 0;
		DMA_BTransfer_Complete_Count = 0;
	}

	if(DMA_ATransfer_Complete_Count >= RWV_ACurrent_Cv)
	{
		ADC_AVG[7] = ACT_ADC_ConvertedValue[7] / (RWV_ACurrent_Cv + 1);
		ADC_AVG[8] = ACT_ADC_ConvertedValue[8] / (RWV_ACurrent_Cv + 1);
		ACT_ADC_ConvertedValue[7] = 0;
		ACT_ADC_ConvertedValue[8] = 0;
		DMA_ATransfer_Complete_Count = 0;
	}

	DMA_ATransfer_Complete_Count++;
	DMA_BTransfer_Complete_Count++;

#else

	int i = 0;
	int j = 0;
	for(i = 0 ; i < ADC_CONVERTED_CHANNEL_NUM; i++)
	{
		
		ADC_Converted_Array[i][ADC_Converted_cursor] = ADC_ConvertedValue[i];
	}
	
	ADC_Converted_cursor++;
	ADC_Converted_Update_Freq++;

	if(ADC_Converted_cursor == QUEUE_LENGTH)
	{
		ADC_Converted_cursor = 0;
	}

	if(ADC_Converted_Update_Freq >= ADC_CONVERTED_UPDATE_FREQ_UP)
	{
		ADC_Converted_Update_Freq = 0;
		for(i = 0; i < ADC_CONVERTED_CHANNEL_NUM; i++)
		{
			for(j = 0; j < QUEUE_LENGTH; j++)
			{
				{
					ADC_Converted_Sum[i] += ADC_Converted_Array[i][j];
				}
			}
		
			ADC_AVG[i] = ADC_Converted_Sum[i] / QUEUE_LENGTH;
			ADC_Converted_Sum[i] = 0;
		}
	}

	/*USART3_ON;
	ADC_Converted_cursor++;
	if(ADC_Converted_cursor == 1)
	{
		USART3_OFF;
		ADC_Converted_cursor--;
	}
	else if(ADC_Converted_cursor == 0)
	{
		USART3_ON;
	}*/

#endif
}

void Adc_Start(void){
  HAL_ADC_Start_DMA(&ADC_Handle,(uint32_t *)ADC_ConvertedValue, ADC_CONVERTED_CHANNEL_NUM);
}

void _Adc_Start(void){
  HAL_ADC_Start_DMA(&_ADC_Handle,(uint32_t *)_ADC_ConvertedValue, ADC_CONVERTED_CHANNEL_NUM);
}


void Rheostat_Init(void)
{
	MX_DMA_Init();
	Rheostat_ADC_GPIO_Config();
	_Rheostat_ADC_GPIO_Config();
	Rheostat_ADC_Mode_Config();
	_Rheostat_ADC_Mode_Config();
	Adc_Start();
	_Adc_Start();
}


#if 1
float ch1_get_oilpres_value(void) //COV_RWV_ABPresSens_FORSDO
{
	float curr_value;
	float oilpres;
	curr_value = 1000.0f * (3.3f * ((float)ADC_AVG[3] / 4096.0f)) / 150.0f;

	oilpres = curr_value * 37.5f - 150;

	if(oilpres < 15)
	{
		oilpres = 0;
	}
	else
	{
		oilpres -= 15;
	}
	
	return oilpres;
}

float ch2_get_oilpres_value(void) //COV_RWV_TPresSens_FORSDO
{
	float curr_value;
	float oilpres;
	curr_value = 1000.0f * (3.3f * ((float)ADC_AVG[4] / 4096.0f)) / 150.0f;

	oilpres = curr_value * 37.5f - 150;

	if(oilpres < 15)
	{
		oilpres = 0;
	}
	else
	{
		oilpres -= 15;
	}
	
	return oilpres;
}

float ch3_get_oilpres_value(void) //COV_RFV_PresSens
{
	float curr_value;
	float oilpres;
	curr_value = 1000.0f * (3.3f * ((float)ADC_AVG[5] / 4096.0f)) / 150.0f;

	oilpres = curr_value * 37.5f - 150;

	if(oilpres < 15)
	{
		oilpres = 0;
	}
	else
	{
		oilpres -= 15;
	}
	
	return oilpres;
}

float ch4_get_oilpres_value(void) //未使用
{
	float curr_value;
	float oilpres;
	curr_value = 1000.0f * (3.3f * ((float)ADC_AVG[6] / 4096.0f)) / 150.0f;

	oilpres = curr_value * 37.5f - 150;

	if(oilpres < 15)
	{
		oilpres = 0;
	}
	else
	{
		oilpres -= 15;
	}
	
	return oilpres;
}

float Get_BoardVolt_Value(void)
{
	float boardvolt_value = 0;

	boardvolt_value = 11 * 3.3f * ((float)ADC_AVG[2] / 4096);

	return boardvolt_value;
}
#endif




















