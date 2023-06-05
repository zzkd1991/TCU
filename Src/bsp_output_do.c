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

#define DO_IN1_Pin 					GPIO_PIN_0
#define DO_IN1_GPIO_Port 			GPIOE
#define DO_IN2_Pin 					GPIO_PIN_1
#define DO_IN2_GPIO_Port 			GPIOE
#define DO_IN3_Pin 					GPIO_PIN_2
#define DO_IN3_GPIO_Port 			GPIOE
#define DO_IN4_Pin 					GPIO_PIN_3
#define DO_IN4_GPIO_Port 			GPIOE
#define DO_IN5_Pin 					GPIO_PIN_4
#define DO_IN5_GPIO_Port 			GPIOE
#define DO_IN6_Pin 					GPIO_PIN_6
#define DO_IN6_GPIO_Port 			GPIOE

//DO1_FY------PG9
//DO2_FY------PD5
//DO3_FY------PD6
//DO6_FY------PD7

#define DO1_FY_PIN						GPIO_PIN_9
#define DO1_FY_GPIO_PORT				GPIOG
#define DO1_FY_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()

#define DO2_FY_PIN						GPIO_PIN_5
#define DO2_FY_GPIO_PORT				GPIOD
#define DO2_FY_GPIO_CLK_ENABLE()		__GPIOD_CLK_ENABLE()

#define DO3_FY_PIN						GPIO_PIN_6
#define DO3_FY_GPIO_PORT				GPIOD
#define DO3_FY_GPIO_CLK_ENABLE()		__GPIOD_CLK_ENABLE()

#define DO6_FY_PIN						GPIO_PIN_7
#define DO6_FY_GPIO_PORT				GPIOD
#define DO6_FY_GPIO_CLK_ENABLE()		__GPIOD_CLK_ENABLE()

/* do_side = 1------high side
   do_side = 0------low side
*/
uint8_t do_side = 1;

void API_DO_FY_Cfg(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	DO1_FY_GPIO_CLK_ENABLE();
	DO2_FY_GPIO_CLK_ENABLE();
	DO3_FY_GPIO_CLK_ENABLE();
	DO6_FY_GPIO_CLK_ENABLE();

	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLDOWN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;

	GPIO_InitStructure.Pin = DO1_FY_PIN;
	HAL_GPIO_Init(DO1_FY_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = DO2_FY_PIN;
	HAL_GPIO_Init(DO2_FY_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.Pin = DO3_FY_PIN;
	HAL_GPIO_Init(DO3_FY_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.Pin = DO6_FY_PIN;
	HAL_GPIO_Init(DO6_FY_GPIO_PORT, &GPIO_InitStructure);
}


void API_OUT_DO_IN_Cfg(void)
{
  	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

  	/* GPIO Ports Clock Enable */
  	__HAL_RCC_GPIOE_CLK_ENABLE();	
	/*Configure GPIO pins : DO_IN3_Pin DO_IN4_Pin DO_IN5_Pin DO_IN6_Pin
							 DI8_Pin DI9_Pin DI10_Pin DI11_Pin
							 DI12_Pin DI13_Pin DI14_Pin DO_IN1_Pin
							 DO_IN2_Pin */
	GPIO_InitStruct.Pin = DO_IN3_Pin|DO_IN4_Pin|DO_IN5_Pin|DO_IN6_Pin|DO_IN1_Pin|DO_IN2_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

uint8_t API_OUT_Do_Get_State(uint8_t channel_u8)
{
	if(channel_u8 == DOUT1)
	{
		if(do_side == 1)
		{
			if(HAL_GPIO_ReadPin(DO_IN1_GPIO_Port, DO_IN1_Pin) == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			if(HAL_GPIO_ReadPin(DO_IN1_GPIO_Port, DO_IN1_Pin) == 1)
			{
				return 0;
			}
			else
			{
				return 1;
			}			
		}

	}
	else if(channel_u8 == DOUT2)
	{
		if(do_side == 1)
		{
			if(HAL_GPIO_ReadPin(DO_IN2_GPIO_Port, DO_IN2_Pin) == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			if(HAL_GPIO_ReadPin(DO_IN2_GPIO_Port, DO_IN2_Pin) == 1)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}

	}
	else if(channel_u8 == DOUT3)
	{
		if(do_side == 1)
		{
			if(HAL_GPIO_ReadPin(DO_IN3_GPIO_Port, DO_IN3_Pin) == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			if(HAL_GPIO_ReadPin(DO_IN3_GPIO_Port, DO_IN3_Pin) == 1)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}

	}	
	else if(channel_u8 == DOUT4)
	{
		if(HAL_GPIO_ReadPin(DO_IN4_GPIO_Port, DO_IN4_Pin) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DOUT5)
	{
		if(HAL_GPIO_ReadPin(DO_IN5_GPIO_Port, DO_IN5_Pin) == 1)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else if(channel_u8 == DOUT6)
	{
		if(do_side == 1)
		{
			if(HAL_GPIO_ReadPin(DO_IN6_GPIO_Port, DO_IN6_Pin) == 1)
			{
				return 1;
			}
			else
			{
				return 0;
			}
		}
		else
		{
			if(HAL_GPIO_ReadPin(DO_IN6_GPIO_Port, DO_IN6_Pin) == 1)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}	
	
	return 0xff;
}


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
		if(do_side == 1)
		{
			HAL_GPIO_WritePin(DO1_FY_GPIO_PORT, DO1_FY_PIN, GPIO_PIN_SET);
		
			if(value == 1)
				HAL_GPIO_WritePin(DO1_GPIO_PORT, DO1_PIN, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(DO1_GPIO_PORT, DO1_PIN, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(DO1_FY_GPIO_PORT, DO1_FY_PIN, GPIO_PIN_RESET);
		
			if(value == 1)
				HAL_GPIO_WritePin(DO1_GPIO_PORT, DO1_PIN, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(DO1_GPIO_PORT, DO1_PIN, GPIO_PIN_SET);
		}

	}
	else if(channel_u8 == DOUT2)
	{
		if(do_side == 1)
		{
			HAL_GPIO_WritePin(DO2_FY_GPIO_PORT, DO2_FY_PIN, GPIO_PIN_SET);
		
			if(value == 1)		
				HAL_GPIO_WritePin(DO2_GPIO_PORT, DO2_PIN, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(DO2_GPIO_PORT, DO2_PIN, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(DO2_FY_GPIO_PORT, DO2_FY_PIN, GPIO_PIN_RESET);
		
			if(value == 1)
				HAL_GPIO_WritePin(DO2_GPIO_PORT, DO2_PIN, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(DO2_GPIO_PORT, DO2_PIN, GPIO_PIN_SET);
		}

	}
	else if(channel_u8 == DOUT3)
	{
		if(do_side == 1)
		{
			HAL_GPIO_WritePin(DO3_FY_GPIO_PORT, DO3_FY_PIN, GPIO_PIN_SET);
		
			if(value == 1)		
				HAL_GPIO_WritePin(DO3_GPIO_PORT, DO3_PIN, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(DO3_GPIO_PORT, DO3_PIN, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(DO3_FY_GPIO_PORT, DO3_FY_PIN, GPIO_PIN_RESET);
		
			if(value == 1)
				HAL_GPIO_WritePin(DO3_GPIO_PORT, DO3_PIN, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(DO3_GPIO_PORT, DO3_PIN, GPIO_PIN_SET);
		}

	}
	else if(channel_u8 == DOUT4)
	{
		if(do_side == 1)
		{		
			if(value == 1)		
				HAL_GPIO_WritePin(DO4_GPIO_PORT, DO4_PIN, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(DO4_GPIO_PORT, DO4_PIN, GPIO_PIN_RESET);
		}
		else
		{
			if(value == 1)
				HAL_GPIO_WritePin(DO4_GPIO_PORT, DO4_PIN, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(DO4_GPIO_PORT, DO4_PIN, GPIO_PIN_SET);
		}

	}
	else if(channel_u8 == DOUT5)
	{
		if(do_side == 1)
		{
			if(value == 1)		
				HAL_GPIO_WritePin(DO5_GPIO_PORT, DO5_PIN, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(DO5_GPIO_PORT, DO5_PIN, GPIO_PIN_RESET);
		}
		else
		{
			if(value == 1)
				HAL_GPIO_WritePin(DO5_GPIO_PORT, DO5_PIN, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(DO5_GPIO_PORT, DO5_PIN, GPIO_PIN_SET);
		}

	}
	else if(channel_u8 == DOUT6)
	{
		if(do_side == 1)
		{
			HAL_GPIO_WritePin(DO6_FY_GPIO_PORT, DO6_FY_PIN, GPIO_PIN_SET);
		
			if(value == 1)		
				HAL_GPIO_WritePin(DO6_GPIO_PORT, DO6_PIN, GPIO_PIN_SET);
			else
				HAL_GPIO_WritePin(DO6_GPIO_PORT, DO6_PIN, GPIO_PIN_RESET);
		}
		else
		{
			HAL_GPIO_WritePin(DO6_FY_GPIO_PORT, DO6_FY_PIN, GPIO_PIN_RESET);

		
			if(value == 1)
				HAL_GPIO_WritePin(DO6_GPIO_PORT, DO6_PIN, GPIO_PIN_RESET);
			else
				HAL_GPIO_WritePin(DO6_GPIO_PORT, DO6_PIN, GPIO_PIN_SET);
		}

	}

	return 0;
}



