#include "bsp_input_di.h"


//pin define
/*
 DI1------->PF11
 DI2------->PF12
 DI3------->PF13
 DI4------->PF14
 DI5------->PG2
 DI6------->PG0
 DI7------->PG1
 DI8------->PE7
 DI9------->PE8
 DI10------>PE10
 DI11------>PE12
 DI12------>PE13
 DI13------>PE14
 DI14------>PE15
 DI15------>PD8
 DI16------>PD9
 DI17------>PD10
 DI18------>PD11
 DI19------>PD13
 DI20------>PD14
 DI21------>PD15
*/


#define DI1_PIN						GPIO_PIN_11
#define DI1_GPIO_PORT				GPIOF
#define DI1_GPIO_CLK_ENABLE()		__GPIOF_CLK_ENABLE()

#define DI2_PIN						GPIO_PIN_12
#define DI2_GPIO_PORT				GPIOF
#define DI2_GPIO_CLK_ENABLE()		__GPIOF_CLK_ENABLE()

#define DI3_PIN						GPIO_PIN_13
#define DI3_GPIO_PORT				GPIOF
#define DI3_GPIO_CLK_ENABLE()		__GPIOF_CLK_ENABLE()

#define DI4_PIN						GPIO_PIN_14
#define DI4_GPIO_PORT				GPIOF
#define DI4_GPIO_CLK_ENABLE()		__GPIOF_CLK_ENABLE()

#define DI5_PIN						GPIO_PIN_2
#define DI5_GPIO_PORT				GPIOG
#define DI5_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()

#define DI6_PIN						GPIO_PIN_0
#define DI6_GPIO_PORT				GPIOG
#define DI6_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()

#define DI7_PIN						GPIO_PIN_1
#define DI7_GPIO_PORT				GPIOG
#define DI7_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()

#define DI8_PIN						GPIO_PIN_7
#define DI8_GPIO_PORT				GPIOE
#define DI8_GPIO_CLK_ENABLE()		__GPIOE_CLK_ENABLE()

#define DI9_PIN						GPIO_PIN_8
#define DI9_GPIO_PORT				GPIOE
#define DI9_GPIO_CLK_ENABLE()		__GPIOE_CLK_ENABLE()

#define DI10_PIN						GPIO_PIN_10
#define DI10_GPIO_PORT				GPIOE
#define DI10_GPIO_CLK_ENABLE()		__GPIOE_CLK_ENABLE()

#define DI11_PIN						GPIO_PIN_12
#define DI11_GPIO_PORT				GPIOE
#define DI11_GPIO_CLK_ENABLE()		__GPIOE_CLK_ENABLE()

#define DI12_PIN						GPIO_PIN_13
#define DI12_GPIO_PORT				GPIOE
#define DI12_GPIO_CLK_ENABLE()		__GPIOE_CLK_ENABLE()

#define DI13_PIN						GPIO_PIN_14
#define DI13_GPIO_PORT				GPIOE
#define DI13_GPIO_CLK_ENABLE()		__GPIOE_CLK_ENABLE()

#define DI14_PIN						GPIO_PIN_15
#define DI14_GPIO_PORT				GPIOE
#define DI14_GPIO_CLK_ENABLE()		__GPIOE_CLK_ENABLE()

#define DI15_PIN						GPIO_PIN_8
#define DI15_GPIO_PORT				GPIOD
#define DI15_GPIO_CLK_ENABLE()		__GPIOD_CLK_ENABLE()

#define DI16_PIN						GPIO_PIN_9
#define DI16_GPIO_PORT				GPIOD
#define DI16_GPIO_CLK_ENABLE()		__GPIOD_CLK_ENABLE()

#define DI17_PIN						GPIO_PIN_10
#define DI17_GPIO_PORT				GPIOD
#define DI17_GPIO_CLK_ENABLE()		__GPIOD_CLK_ENABLE()

#define DI18_PIN						GPIO_PIN_11
#define DI18_GPIO_PORT				GPIOD
#define DI18_GPIO_CLK_ENABLE()		__GPIOD_CLK_ENABLE()

#define DI19_PIN						GPIO_PIN_13
#define DI19_GPIO_PORT				GPIOD
#define DI19_GPIO_CLK_ENABLE()		__GPIOD_CLK_ENABLE()

#define DI20_PIN						GPIO_PIN_14
#define DI20_GPIO_PORT				GPIOD
#define DI20_GPIO_CLK_ENABLE()		__GPIOD_CLK_ENABLE()

#define DI21_PIN						GPIO_PIN_15
#define DI21_GPIO_PORT				GPIOD
#define DI21_GPIO_CLK_ENABLE()		__GPIOD_CLK_ENABLE()


uint8_t IMCA2136_DI[21] = {0};
uint8_t IMCA2136_Flag[21] = {0};


uint8_t API_DI_Cfg(uint8_t channel_u8, uint8_t signaleType_u8)
{
	if(channel_u8 < DIN1 || channel_u8 > DIN21)
		return 1;
	if(signaleType_u8 != 0 || signaleType_u8 != 1)
		return 2;

	IMCA2136_Flag[channel_u8] = signaleType_u8;
	
	return 0;
}




void Bsp_Di_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	DI1_GPIO_CLK_ENABLE();
	DI2_GPIO_CLK_ENABLE();
	DI3_GPIO_CLK_ENABLE();
	DI4_GPIO_CLK_ENABLE();
	DI5_GPIO_CLK_ENABLE();
	DI6_GPIO_CLK_ENABLE();
	DI7_GPIO_CLK_ENABLE();
	DI8_GPIO_CLK_ENABLE();
	DI9_GPIO_CLK_ENABLE();
	DI10_GPIO_CLK_ENABLE();
	DI11_GPIO_CLK_ENABLE();
	DI12_GPIO_CLK_ENABLE();
	DI13_GPIO_CLK_ENABLE();
	DI14_GPIO_CLK_ENABLE();
	DI15_GPIO_CLK_ENABLE();
	DI16_GPIO_CLK_ENABLE();
	DI17_GPIO_CLK_ENABLE();
	DI18_GPIO_CLK_ENABLE();
	DI19_GPIO_CLK_ENABLE();
	DI20_GPIO_CLK_ENABLE();
	DI21_GPIO_CLK_ENABLE();

	//DI1
	GPIO_InitStructure.Pin =  DI1_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;//GPIO输入模式
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;//管脚下拉
	HAL_GPIO_Init(DI1_GPIO_PORT, &GPIO_InitStructure);

	//DI2
	GPIO_InitStructure.Pin = DI2_PIN;
	HAL_GPIO_Init(DI2_GPIO_PORT, &GPIO_InitStructure);

	//DI3
	GPIO_InitStructure.Pin = DI3_PIN;
	HAL_GPIO_Init(DI3_GPIO_PORT, &GPIO_InitStructure);

	//DI4
	GPIO_InitStructure.Pin = DI4_PIN;
	HAL_GPIO_Init(DI4_GPIO_PORT, &GPIO_InitStructure);

	//DI5
	GPIO_InitStructure.Pin = DI5_PIN;
	HAL_GPIO_Init(DI5_GPIO_PORT,  &GPIO_InitStructure);

	//DI6
	GPIO_InitStructure.Pin = DI6_PIN;
	HAL_GPIO_Init(DI6_GPIO_PORT, &GPIO_InitStructure);

	//DI7
	GPIO_InitStructure.Pin = DI7_PIN;
	HAL_GPIO_Init(DI7_GPIO_PORT, &GPIO_InitStructure);

	//DI8
	GPIO_InitStructure.Pin = DI8_PIN;
	HAL_GPIO_Init(DI8_GPIO_PORT, &GPIO_InitStructure);

	//DI9
	GPIO_InitStructure.Pin = DI9_PIN;
	HAL_GPIO_Init(DI9_GPIO_PORT, &GPIO_InitStructure);

	//DI10
	GPIO_InitStructure.Pin = DI10_PIN;
	HAL_GPIO_Init(DI10_GPIO_PORT, &GPIO_InitStructure);

	//DI11
	GPIO_InitStructure.Pin = DI11_PIN;
	HAL_GPIO_Init(DI11_GPIO_PORT, &GPIO_InitStructure);

	//DI12
	GPIO_InitStructure.Pin = DI12_PIN;
	HAL_GPIO_Init(DI12_GPIO_PORT, &GPIO_InitStructure);

	//DI13
	GPIO_InitStructure.Pin = DI13_PIN;
	HAL_GPIO_Init(DI13_GPIO_PORT, &GPIO_InitStructure);

	//DI14
	GPIO_InitStructure.Pin = DI14_PIN;
	HAL_GPIO_Init(DI14_GPIO_PORT, &GPIO_InitStructure);

	//DI15
	GPIO_InitStructure.Pin = DI15_PIN;
	HAL_GPIO_Init(DI15_GPIO_PORT, &GPIO_InitStructure);

	//DI16
	GPIO_InitStructure.Pin = DI16_PIN;
	HAL_GPIO_Init(DI16_GPIO_PORT, &GPIO_InitStructure);

	//DI17
	GPIO_InitStructure.Pin = DI17_PIN;
	HAL_GPIO_Init(DI17_GPIO_PORT, &GPIO_InitStructure);

	//DI18
	GPIO_InitStructure.Pin = DI18_PIN;
	HAL_GPIO_Init(DI18_GPIO_PORT, &GPIO_InitStructure);	

	//DI19
	GPIO_InitStructure.Pin = DI19_PIN;
	HAL_GPIO_Init(DI19_GPIO_PORT, &GPIO_InitStructure);	

	//DI20
	GPIO_InitStructure.Pin = DI20_PIN;
	HAL_GPIO_Init(DI20_GPIO_PORT, &GPIO_InitStructure);	

	//DI21
	GPIO_InitStructure.Pin = DI21_PIN;
	HAL_GPIO_Init(DI21_GPIO_PORT, &GPIO_InitStructure);		
}


uint8_t Api_Di_Get(uint8_t channel_u8)
{

	if(channel_u8 > DIN21 || channel_u8 < DIN1)
		return 0xff;
	
	if(channel_u8 == DIN1)
	{
		if(HAL_GPIO_ReadPin(DI1_GPIO_PORT, DI1_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN2)
	{
		if(HAL_GPIO_ReadPin(DI2_GPIO_PORT, DI2_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN3)
	{
		if(HAL_GPIO_ReadPin(DI3_GPIO_PORT, DI3_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN4)
	{
		if(HAL_GPIO_ReadPin(DI4_GPIO_PORT, DI4_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN5)
	{
		if(HAL_GPIO_ReadPin(DI5_GPIO_PORT, DI5_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN6)
	{
		if(HAL_GPIO_ReadPin(DI6_GPIO_PORT, DI6_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN7)
	{
		if(HAL_GPIO_ReadPin(DI7_GPIO_PORT, DI7_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN8)
	{
		if(HAL_GPIO_ReadPin(DI8_GPIO_PORT, DI8_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN9)
	{
		if(HAL_GPIO_ReadPin(DI9_GPIO_PORT, DI9_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN10)
	{
		if(HAL_GPIO_ReadPin(DI10_GPIO_PORT, DI10_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN11)
	{
		if(HAL_GPIO_ReadPin(DI11_GPIO_PORT, DI11_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN12)
	{
		if(HAL_GPIO_ReadPin(DI12_GPIO_PORT, DI12_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN13)
	{
		if(HAL_GPIO_ReadPin(DI13_GPIO_PORT, DI13_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN14)
	{
		if(HAL_GPIO_ReadPin(DI14_GPIO_PORT, DI14_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN15)
	{
		if(HAL_GPIO_ReadPin(DI15_GPIO_PORT, DI15_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN16)
	{
		if(HAL_GPIO_ReadPin(DI16_GPIO_PORT, DI16_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN17)
	{
		if(HAL_GPIO_ReadPin(DI17_GPIO_PORT, DI17_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN18)
	{
		if(HAL_GPIO_ReadPin(DI18_GPIO_PORT, DI18_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN19)
	{
		if(HAL_GPIO_ReadPin(DI19_GPIO_PORT, DI19_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN20)
	{
		if(HAL_GPIO_ReadPin(DI20_GPIO_PORT, DI20_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DIN21)
	{
		if(HAL_GPIO_ReadPin(DI21_GPIO_PORT, DI21_PIN) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	
	return 0xff;
}

/*uint8_t IMCA2163_ID1, IMCA2163_ID2, IMCA2163_ID3, IMCA2163_ID4, IMCA2163_ID5, IMCA2163_ID6, IMCA2163_ID7, IMCA2163_ID8, IMCA2163_ID9, IMCA2163_ID10, IMCA2163_ID11, IMCA2163_ID12,IMCA2163_ID13,
IMCA2163_ID14, IMCA2163_ID15, IMCA2163_ID16, IMCA2163_ID17, IMCA2163_ID18, IMCA2163_ID19, IMCA2163_ID20, IMCA2163_ID21;

uint8_t *IMCA_Point[21];*/

uint32_t DI_Screen(void)
{
	uint8_t channel = 0;

	for(channel = 1; channel < 22; channel++)
	{
		if(Api_Di_Get(channel) == 1)
		{
			if(IMCA2136_Flag[channel] == 0)
			{
				IMCA2136_DI[channel] = 1;
			}
			else if(IMCA2136_Flag[channel] == 1)
			{
				IMCA2136_DI[channel] = 0;
			}
		}
		else
		{
			if(IMCA2136_Flag[channel] == 0)
			{
				IMCA2136_DI[channel] = 0;
			}
			else if(IMCA2136_Flag[channel] == 1)
			{
				IMCA2136_DI[channel] = 1;
			}
		}
	}

	return 0;

}




