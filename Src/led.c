#include "led.h"

void LED_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	LED1_GPIO_CLK_ENABLE();
	LED2_GPIO_CLK_ENABLE();
	LED3_GPIO_CLK_ENABLE();
	LED4_GPIO_CLK_ENABLE();
	LED5_GPIO_CLK_ENABLE();
	LED6_GPIO_CLK_ENABLE();


	GPIO_InitStruct.Pin = LED1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LED2_PIN;
	HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LED3_PIN;
	HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LED4_PIN;
	HAL_GPIO_Init(LED4_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LED5_PIN;
	HAL_GPIO_Init(LED5_GPIO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LED6_PIN;
	HAL_GPIO_Init(LED6_GPIO_PORT, &GPIO_InitStruct);
}

/*
 红+绿 = 黄
 红+蓝 = 紫
 蓝+绿 = 青
 红+绿+蓝 = 白
*/

void led1_show_yellow(void)
{
	LED1(OFF);
	LED2(ON);
	LED3(ON);
}

void led2_show_yellow(void)
{
	LED4(OFF);
	LED5(ON);
	LED6(ON);
}


void led1_show_purple(void)
{
	LED1(ON);
	LED2(OFF);
	LED3(ON);
}

void led2_show_purple(void)
{
	LED4(ON);
	LED5(OFF);
	LED6(ON);
}

void led1_show_cyan(void)//青色
{
	LED1(ON);
	LED2(ON);
	LED3(OFF);
}

void led2_show_cyan(void)
{
	LED4(ON);
	LED5(ON);
	LED6(OFF);
}

void led1_show_white(void)
{
	LED1(ON);
	LED2(ON);
	LED3(ON);
}

void led2_show_white(void)
{
	LED4(ON);
	LED5(ON);
	LED6(ON);
}

void LED_GPIO_DeConfig(void)
{
	LED1_GPIO_CLK_DISABLE();
	LED2_GPIO_CLK_DISABLE();
	LED3_GPIO_CLK_DISABLE();
	LED4_GPIO_CLK_DISABLE();
	LED5_GPIO_CLK_DISABLE();
	LED6_GPIO_CLK_DISABLE();

	HAL_GPIO_DeInit(LED1_GPIO_PORT, LED1_PIN);
	HAL_GPIO_DeInit(LED2_GPIO_PORT, LED2_PIN);
	HAL_GPIO_DeInit(LED3_GPIO_PORT, LED3_PIN);
	HAL_GPIO_DeInit(LED4_GPIO_PORT, LED4_PIN);
	HAL_GPIO_DeInit(LED5_GPIO_PORT, LED5_PIN);
	HAL_GPIO_DeInit(LED6_GPIO_PORT, LED6_PIN);
}


