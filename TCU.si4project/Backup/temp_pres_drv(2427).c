#include "temp_pres_drv.h"

__IO uint16_t ADC_ConvertedValue[RHEOSTAT_NOFCHANEL]={0};
uint32_t ACT_ADC_ConvertedValue[RHEOSTAT_NOFCHANEL] = {0};
uint16_t ADC_AVG[RHEOSTAT_NOFCHANEL] = {0};
DMA_HandleTypeDef DMA_Init_Handle;
ADC_HandleTypeDef ADC_Handle1;
ADC_HandleTypeDef ADC_Handle2;
ADC_HandleTypeDef ADC_Handle3;
ADC_ChannelConfTypeDef ADC_Config;
uint16_t DMA_ATransfer_Complete_Count = 60;
uint16_t DMA_BTransfer_Complete_Count = 60;
uint16_t RWV_ACurrent_Cv = 50;//???63
uint16_t RWV_BCurrent_Cv = 50;

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

static void Rheostat_ADC_Mode_Config(void)
{
    ADC_MultiModeTypeDef   mode;
    // ------------------DMA Init ½á¹¹Ìå²ÎÊý ³õÊ¼»¯--------------------------
    // ADC1Ê¹ÓÃDMA2£¬Êý¾ÝÁ÷0£¬Í¨µÀ0£¬Õâ¸öÊÇÊÖ²á¹Ì¶¨ËÀµÄ
    // ¿ªÆôDMAÊ±ÖÓ
    RHEOSTAT_ADC_DMA_CLK_ENABLE();
    // Êý¾Ý´«ÊäÍ¨µÀ
    DMA_Init_Handle.Instance = RHEOSTAT_ADC_DMA_STREAM;
    // Êý¾Ý´«Êä·½ÏòÎªÍâÉèµ½´æ´¢Æ÷	
    DMA_Init_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;	
    // ÍâÉè¼Ä´æÆ÷Ö»ÓÐÒ»¸ö£¬µØÖ·²»ÓÃµÝÔö
    DMA_Init_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
    // ´æ´¢Æ÷µØÖ·¹Ì¶¨
    DMA_Init_Handle.Init.MemInc = DMA_MINC_ENABLE; 
    // // ÍâÉèÊý¾Ý´óÐ¡Îª°ë×Ö£¬¼´Á½¸ö×Ö½Ú 
    DMA_Init_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; 
    //	´æ´¢Æ÷Êý¾Ý´óÐ¡Ò²Îª°ë×Ö£¬¸úÍâÉèÊý¾Ý´óÐ¡ÏàÍ¬
    DMA_Init_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;	
    // Ñ­»·´«ÊäÄ£Ê½
    DMA_Init_Handle.Init.Mode = DMA_CIRCULAR;
    // DMA ´«ÊäÍ¨µÀÓÅÏÈ¼¶Îª¸ß£¬µ±Ê¹ÓÃÒ»¸öDMAÍ¨µÀÊ±£¬ÓÅÏÈ¼¶ÉèÖÃ²»Ó°Ïì
    DMA_Init_Handle.Init.Priority = DMA_PRIORITY_HIGH;
    // ½ûÖ¹DMA FIFO	£¬Ê¹ÓÃÖ±Á¬Ä£Ê½
    DMA_Init_Handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;  
    // FIFO ´óÐ¡£¬FIFOÄ£Ê½½ûÖ¹Ê±£¬Õâ¸ö²»ÓÃÅäÖÃ	
    DMA_Init_Handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DMA_Init_Handle.Init.MemBurst = DMA_MBURST_SINGLE;
    DMA_Init_Handle.Init.PeriphBurst = DMA_PBURST_SINGLE;  
    // Ñ¡Ôñ DMA Í¨µÀ£¬Í¨µÀ´æÔÚÓÚÁ÷ÖÐ
    DMA_Init_Handle.Init.Channel = RHEOSTAT_ADC_DMA_CHANNEL; 
    //³õÊ¼»¯DMAÁ÷£¬Á÷Ïàµ±ÓÚÒ»¸ö´óµÄ¹ÜµÀ£¬¹ÜµÀÀïÃæÓÐºÜ¶àÍ¨µÀ
    HAL_DMA_Init(&DMA_Init_Handle);

    // ¿ªÆôADCÊ±ÖÓ
    RHEOSTAT_ADC1_CLK_ENABLE();
    RHEOSTAT_ADC2_CLK_ENABLE();
    RHEOSTAT_ADC3_CLK_ENABLE();
    // -------------------ADC1 Init ½á¹¹Ìå ²ÎÊý ³õÊ¼»¯------------------------
    // ADC1
    ADC_Handle1.Instance = RHEOSTAT_ADC1;
    // Ê±ÖÓÎªfpclk 4·ÖÆµ	
    ADC_Handle1.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    // ADC ·Ö±æÂÊ
    ADC_Handle1.Init.Resolution = ADC_RESOLUTION_12B;
    // ½ûÖ¹É¨ÃèÄ£Ê½£¬¶àÍ¨µÀ²É¼¯²ÅÐèÒª	
    ADC_Handle1.Init.ScanConvMode = ENABLE; 
    // Á¬Ðø×ª»»	
    ADC_Handle1.Init.ContinuousConvMode = ENABLE;
    // ·ÇÁ¬Ðø×ª»»	
    ADC_Handle1.Init.DiscontinuousConvMode = DISABLE;
    // ·ÇÁ¬Ðø×ª»»¸öÊý
    ADC_Handle1.Init.NbrOfDiscConversion   = 0;
    //½ûÖ¹Íâ²¿±ßÑØ´¥·¢    
    ADC_Handle1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    //Ê¹ÓÃÈí¼þ´¥·¢£¬Íâ²¿´¥·¢²»ÓÃÅäÖÃ£¬×¢ÊÍµô¼´¿É
    //ADC_Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    //Êý¾ÝÓÒ¶ÔÆë	
    ADC_Handle1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    //×ª»»Í¨µÀ¸öÊý
    ADC_Handle1.Init.NbrOfConversion = 6;
    //Ê¹ÄÜÁ¬Ðø×ª»»ÇëÇó
    ADC_Handle1.Init.DMAContinuousRequests = ENABLE;
    //×ª»»Íê³É±êÖ¾
    ADC_Handle1.Init.EOCSelection          = DISABLE;    
    // ³õÊ¼»¯ADC	                          
    HAL_ADC_Init(&ADC_Handle1);
    //---------------------------------------------------------------------------
    // ÅäÖÃ ADC1 Í¨µÀ13×ª»»Ë³ÐòÎª1£¬µÚÒ»¸ö×ª»»£¬²ÉÑùÊ±¼äÎª3¸öÊ±ÖÓÖÜÆÚ
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL1;
    ADC_Config.Rank         = 1;    
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle1, &ADC_Config);
		
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL2;
    ADC_Config.Rank         = 2;    
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle1, &ADC_Config);

    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL3;
    ADC_Config.Rank         = 3;    
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle1, &ADC_Config);
    
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL4;
    ADC_Config.Rank         = 4;    
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle1, &ADC_Config);		
		
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL5;
    ADC_Config.Rank         = 5;    
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle1, &ADC_Config);		
		
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL6;
    ADC_Config.Rank         = 6;    
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle1, &ADC_Config);		
		
    // ADC2
    ADC_Handle2.Instance = RHEOSTAT_ADC2;
    // Ê±ÖÓÎªfpclk 4·ÖÆµ	
    ADC_Handle2.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    // ADC ·Ö±æÂÊ
    ADC_Handle2.Init.Resolution = ADC_RESOLUTION_12B;
    // ½ûÖ¹É¨ÃèÄ£Ê½£¬¶àÍ¨µÀ²É¼¯²ÅÐèÒª	
    ADC_Handle1.Init.ScanConvMode = ENABLE; 
    // Á¬Ðø×ª»»	
    ADC_Handle2.Init.ContinuousConvMode = ENABLE;
    // ·ÇÁ¬Ðø×ª»»	
    ADC_Handle2.Init.DiscontinuousConvMode = DISABLE;
    // ·ÇÁ¬Ðø×ª»»¸öÊý
    ADC_Handle2.Init.NbrOfDiscConversion   = 0;
    //½ûÖ¹Íâ²¿±ßÑØ´¥·¢    
    ADC_Handle2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    //Ê¹ÓÃÈí¼þ´¥·¢£¬Íâ²¿´¥·¢²»ÓÃÅäÖÃ£¬×¢ÊÍµô¼´¿É
    //ADC_Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    //Êý¾ÝÓÒ¶ÔÆë	
    ADC_Handle2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    //×ª»»Í¨µÀ¸öÊý
    ADC_Handle2.Init.NbrOfConversion = 3;
    //Ê¹ÄÜÁ¬Ðø×ª»»ÇëÇó
    ADC_Handle2.Init.DMAContinuousRequests = ENABLE;
    //×ª»»Íê³É±êÖ¾
    ADC_Handle2.Init.EOCSelection          = DISABLE;    
    // ³õÊ¼»¯ADC	                          
    HAL_ADC_Init(&ADC_Handle2);
    // ÅäÖÃ ADC2 Í¨µÀ13×ª»»Ë³ÐòÎª1£¬µÚÒ»¸ö×ª»»£¬²ÉÑùÊ±¼äÎª3¸öÊ±ÖÓÖÜÆÚ
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL7;
    ADC_Config.Rank         = 7;    
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle1, &ADC_Config);		
		
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL8;
    ADC_Config.Rank         = 8;    
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle1, &ADC_Config);		
		
    ADC_Config.Channel      = RHEOSTAT_ADC_CHANNEL9;
    ADC_Config.Rank         = 9;    
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô	
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle1, &ADC_Config);		
    // -------------------ADC2 Init ½á¹¹Ìå ²ÎÊý ³õÊ¼»¯------------------------		
		
    // -------------------ADC33 Init ½á¹¹Ìå ²ÎÊý ³õÊ¼»¯------------------------
    // ADC3
    ADC_Handle3.Instance = RHEOSTAT_ADC3;
    // Ê±ÖÓÎªfpclk 4·ÖÆµ	
    ADC_Handle3.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;
    // ADC ·Ö±æÂÊ
    ADC_Handle3.Init.Resolution = ADC_RESOLUTION_12B;
    // ½ûÖ¹É¨ÃèÄ£Ê½£¬¶àÍ¨µÀ²É¼¯²ÅÐèÒª	
    ADC_Handle1.Init.ScanConvMode = ENABLE; 
    // Á¬Ðø×ª»»	
    ADC_Handle3.Init.ContinuousConvMode = ENABLE;
    // ·ÇÁ¬Ðø×ª»»	
    ADC_Handle3.Init.DiscontinuousConvMode = DISABLE;
    // ·ÇÁ¬Ðø×ª»»¸öÊý
    ADC_Handle3.Init.NbrOfDiscConversion   = 0;
    //½ûÖ¹Íâ²¿±ßÑØ´¥·¢    
    ADC_Handle3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    //Ê¹ÓÃÈí¼þ´¥·¢£¬Íâ²¿´¥·¢²»ÓÃÅäÖÃ£¬×¢ÊÍµô¼´¿É
    //ADC_Handle.Init.ExternalTrigConv      = ADC_EXTERNALTRIGCONV_T1_CC1;
    //Êý¾ÝÓÒ¶ÔÆë	
    ADC_Handle3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    //×ª»»Í¨µÀ¸öÊý
    ADC_Handle3.Init.NbrOfConversion = 9;
    //Ê¹ÄÜÁ¬Ðø×ª»»ÇëÇó
    ADC_Handle3.Init.DMAContinuousRequests = ENABLE;
    //×ª»»Íê³É±êÖ¾
    ADC_Handle3.Init.EOCSelection          = DISABLE;    
    // ³õÊ¼»¯ADC	                          
    HAL_ADC_Init(&ADC_Handle3);
    // ÅäÖÃ ADC3 Í¨µÀ13×ª»»Ë³ÐòÎª1£¬µÚÒ»¸ö×ª»»£¬²ÉÑùÊ±¼äÎª3¸öÊ±ÖÓÖÜÆÚ
    ADC_Config.Channel      = _RHEOSTAT_ADC_CHANNEL1;
    ADC_Config.Rank         = 1;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle3, &ADC_Config);    

    ADC_Config.Channel      = _RHEOSTAT_ADC_CHANNEL2;
    ADC_Config.Rank         = 2;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle3, &ADC_Config);

    ADC_Config.Channel      = _RHEOSTAT_ADC_CHANNEL3;
    ADC_Config.Rank         = 3;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle3, &ADC_Config);

    ADC_Config.Channel      = _RHEOSTAT_ADC_CHANNEL4;
    ADC_Config.Rank         = 4;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle3, &ADC_Config);

    ADC_Config.Channel      = _RHEOSTAT_ADC_CHANNEL5;
    ADC_Config.Rank         = 5;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle3, &ADC_Config);

    ADC_Config.Channel      = _RHEOSTAT_ADC_CHANNEL6;
    ADC_Config.Rank         = 6;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle3, &ADC_Config);

    ADC_Config.Channel      = _RHEOSTAT_ADC_CHANNEL7;
    ADC_Config.Rank         = 6;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle3, &ADC_Config);

    ADC_Config.Channel      = _RHEOSTAT_ADC_CHANNEL8;
    ADC_Config.Rank         = 6;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle3, &ADC_Config);

    ADC_Config.Channel      = _RHEOSTAT_ADC_CHANNEL9;
    ADC_Config.Rank         = 6;    	
    ADC_Config.SamplingTime = ADC_SAMPLETIME_3CYCLES;// ²ÉÑùÊ±¼ä¼ä¸ô
    ADC_Config.Offset       = 0;
    HAL_ADC_ConfigChannel(&ADC_Handle3, &ADC_Config);	

    /*ÅäÖÃÈýÖØAD²ÉÑù*/
    mode.Mode = ADC_TRIPLEMODE_INTERL;
    mode.DMAAccessMode = ADC_DMAACCESSMODE_2;
    mode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
    
    HAL_ADCEx_MultiModeConfigChannel(&ADC_Handle1, &mode);
    
    HAL_ADC_Start(&ADC_Handle2);
    HAL_ADC_Start(&ADC_Handle3);
    
    __HAL_LINKDMA(&ADC_Handle1, DMA_Handle, DMA_Init_Handle);
    __HAL_LINKDMA(&ADC_Handle2, DMA_Handle, DMA_Init_Handle);
    __HAL_LINKDMA(&ADC_Handle3, DMA_Handle, DMA_Init_Handle);    
    HAL_ADCEx_MultiModeStart_DMA(&ADC_Handle1, (uint32_t *)ADC_ConvertedValue, 18);
}

void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

	//HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_1);
	
  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  //HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  //HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	uint16_t i;
#if 0
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
#endif	
}

void Rheostat_Init(void)
{
	MX_DMA_Init();
	Rheostat_ADC_GPIO_Config();
	Rheostat_ADC_Mode_Config();
}



