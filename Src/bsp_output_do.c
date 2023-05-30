#include "bsp_output_do.h"

//DO1------PG3
//DO2------PG4
//DO3------PG5
//DO4------PG6
//DO5------PG7
//DO6------PG8

#define DO1_PIN						GPIO_PIN_3
#define DO1_GPIO_PORT				GPIOG
#define DO1_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()

#define DO2_PIN						GPIO_PIN_4
#define DO2_GPIO_PORT				GPIOG
#define DO2_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()

#define DO3_PIN						GPIO_PIN_5
#define DO3_GPIO_PORT				GPIOG
#define DO3_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()

#define DO4_PIN						GPIO_PIN_6
#define DO4_GPIO_PORT				GPIOG
#define DO4_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()

#define DO5_PIN						GPIO_PIN_7
#define DO5_GPIO_PORT				GPIOG
#define DO5_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()

#define DO6_PIN						GPIO_PIN_8
#define DO6_GPIO_PORT				GPIOG
#define DO6_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()


uint8_t API_OUT_Do_Cfg(uint8_t channel_u8, uint8_t OutMode_u8)
{
	if(channel_u8 < DOUT1 || channel_u8 > DOUT6)
		return 1;

	GPIO_InitTypeDef GPIO_InitStructure;

	DO1_GPIO_CLK_ENABLE();
	DO2_GPIO_CLK_ENABLE();
	DO3_GPIO_CLK_ENABLE();
	DO4_GPIO_CLK_ENABLE();
	DO5_GPIO_CLK_ENABLE();
	DO6_GPIO_CLK_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStructure.Pin = DO1_PIN;
	HAL_GPIO_Init(DO1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = DO2_PIN;
	HAL_GPIO_Init(DO2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = DO3_PIN;
	HAL_GPIO_Init(DO3_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = DO4_PIN;
	HAL_GPIO_Init(DO4_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = DO5_PIN;
	HAL_GPIO_Init(DO5_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = DO6_PIN;
	HAL_GPIO_Init(DO6_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = DO3_PIN;
	HAL_GPIO_Init(DO3_GPIO_PORT, &GPIO_InitStructure);
	
	return 0;
}

uint8_t API_OUT_Do_Set(uint8_t channel_u8, uint8_t value)
{
	if(channel_u8 < DOUT1 || channel_u8 > DOUT6)
		return 1;

	if(value != 0 || value != 1)
		return 2;

	if(channel_u8 == DOUT1)
	{
		if(value == 1)		
			HAL_GPIO_WritePin(DO1_GPIO_PORT, DOUT1, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(DO1_GPIO_PORT, DOUT1, GPIO_PIN_RESET);
	}
	else if(channel_u8 == DOUT2)
	{
		if(value == 1)		
			HAL_GPIO_WritePin(DO2_GPIO_PORT, DOUT2, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(DO2_GPIO_PORT, DOUT2, GPIO_PIN_RESET);
	}
	else if(channel_u8 == DOUT3)
	{
		if(value == 1)		
			HAL_GPIO_WritePin(DO3_GPIO_PORT, DOUT3, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(DO3_GPIO_PORT, DOUT3, GPIO_PIN_RESET);
	}
	else if(channel_u8 == DOUT4)
	{
		if(value == 1)		
			HAL_GPIO_WritePin(DO4_GPIO_PORT, DOUT4, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(DO4_GPIO_PORT, DOUT4, GPIO_PIN_RESET);
	}
	else if(channel_u8 == DOUT5)
	{
		if(value == 1)		
			HAL_GPIO_WritePin(DO1_GPIO_PORT, DOUT5, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(DO1_GPIO_PORT, DOUT5, GPIO_PIN_RESET);
	}
	else if(channel_u8 == DOUT6)
	{
		if(value == 1)		
			HAL_GPIO_WritePin(DO6_GPIO_PORT, DOUT6, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(DO6_GPIO_PORT, DOUT6, GPIO_PIN_RESET);
	}

	return 0;

}



