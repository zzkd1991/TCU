#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "tim.h"

TIM_HandleTypeDef TIM1_TimeBaseStructure;

uint32_t tle7242_clk_freq = 1200;
#define PERIOD_VALUE		(uint32_t)(20000 - 1)

TIM_OC_InitTypeDef sConfig = {0};

void TIMx1_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	
	TLE7242_GPIO_CLK_ENABLE();
	
	GPIO_InitStructure.Pin = TLE7242_CLK_PIN;
	GPIO_InitStructure.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.Alternate = TLE7242_CLK_AF;
	HAL_GPIO_Init(TLE7242_GPIO_PORT, &GPIO_InitStructure);
}

void TIM1_Mode_Config(uint32_t pwm_freq)
{
	extern void Error_Handler(void);

	uint32_t prescaler = 0;
	
	prescaler = 8400 / pwm_freq;
	
	TLE7242_TIM1_CLK_ENABLE();
	
	TIM1_TimeBaseStructure.Instance = TLE7242_TIM1;
	
	TIM1_TimeBaseStructure.Init.Prescaler = prescaler - 1;
	
	TIM1_TimeBaseStructure.Init.Period	= PERIOD_VALUE;
	TIM1_TimeBaseStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	TIM1_TimeBaseStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_PWM_Init(&TIM1_TimeBaseStructure) != HAL_OK)
	{
		Error_Handler();
	}
	
	sConfig.Pulse = 0;
	sConfig.OCPolarity = TIM_OCPOLARITY_HIGH;
	
	if(HAL_TIM_PWM_ConfigChannel(&TIM1_TimeBaseStructure, &sConfig, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	
	HAL_TIM_PWM_Start(&TIM1_TimeBaseStructure, TIM_CHANNEL_2);
}

