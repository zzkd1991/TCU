#include "led.h"

//B
#define LED1_PIN 	GPIO_PIN_15
#define LED1_GPIO_PORT		GPIOG
#define LED1_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()		__GPIOG_CLK_DISABLE()


//G
#define LED2_PIN	GPIO_PIN_14
#define LED2_GPIO_PORT	GPIOG
#define LED2_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()		__GPIOG_CLK_DISABLE()


//R
#define LED3_PIN	GPIO_PIN_13
#define LED3_GPIO_PORT	GPIOG
#define LED3_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()		__GPIOG_CLK_DISABLE()


//B
#define LED4_PIN	GPIO_PIN_12
#define LED4_GPIO_PORT	GPIOG
#define LED4_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()		__GPIOG_CLK_DISABLE()

//G
#define LED5_PIN	GPIO_PIN_11
#define LED5_GPIO_PORT	GPIOG
#define LED5_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()
#define LED5_GPIO_CLK_DISABLE()		__GPIOG_CLK_DISABLE()


//R
#define LED6_PIN	GPIO_PIN_10
#define LED6_GPIO_PORT	GPIOG	
#define LED6_GPIO_CLK_ENABLE()		__GPIOG_CLK_ENABLE()
#define LED6_GPIO_CLK_DISABLE()		__GPIOG_CLK_DISABLE()

#define ON 	GPIO_PIN_SET
#define OFF GPIO_PIN_RESET

#define LED1(a)	HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, a)
#define LED2(a) HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, a)
#define LED3(a) HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, a)
#define LED4(a) HAL_GPIO_WritePin(LED4_GPIO_PORT, LED4_PIN, a)
#define LED5(a) HAL_GPIO_WritePin(LED5_GPIO_PORT, LED5_PIN, a)
#define LED6(a) HAL_GPIO_WritePin(LED6_GPIO_PORT, LED6_PIN, a)


#define led_digitalHi(p, i)			{p->BSRR = i;}
#define led_digitalLo(p, i)			{p->BSRR = (uint32_t)i << 16;}
#define led_digitalToggle(p, i)		{p->ODR ^= i;}


#define LED1_TOGGLE		led_digitalToggle(LED1_GPIO_PORT, LED1_PIN)
#define LED1_ON			led_digitalHi(LED1_GPIO_PORT, LED1_PIN)
#define LED1_OFF		led_digitalLo(LED1_GPIO_PORT, LED1_PIN)


#define LED2_TOGGLE		led_digitalToggle(LED2_GPIO_PORT, LED2_PIN)
#define LED2_ON			led_digitalHi(LED2_GPIO_PORT, LED2_PIN)
#define LED2_OFF		led_digitalLo(LED2_GPIO_PORT, LED2_PIN)

#define LED3_TOGGLE		led_digitalToggle(LED3_GPIO_PORT, LED3_PIN)
#define LED3_ON			led_digitalHi(LED3_GPIO_PORT, LED3_PIN)
#define LED3_OFF		led_digitalLo(LED3_GPIO_PORT, LED3_PIN)

#define LED4_TOGGLE		led_digitalToggle(LED4_GPIO_PORT, LED4_PIN)
#define LED4_ON			led_digitalHi(LED4_GPIO_PORT, LED4_PIN)
#define LED4_OFF		led_digitalLo(LED4_GPIO_PORT, LED4_PIN)

#define LED5_TOGGLE		led_digitalToggle(LED5_GPIO_PORT, LED5_PIN)
#define LED5_ON			led_digitalHi(LED5_GPIO_PORT, LED5_PIN)
#define LED5_OFF		led_digitalLo(LED5_GPIO_PORT, LED5_PIN)

#define LED6_TOGGLE		led_digitalToggle(LED6_GPIO_PORT, LED6_PIN)
#define LED6_ON			led_digitalHi(LED6_GPIO_PORT, LED6_PIN)
#define LED6_OFF		led_digitalLo(LED6_GPIO_PORT, LED6_PIN)


#define LED_RGBOFF	\
			LED1_OFF;\
			LED2_OFF\
			LED3_OFF


void bsp_Led_Init(void)
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
void API_Led_Color_Control(LED_NUM led_num, uint8_t color_u8)
{
	if(color_u8 > LED_COLOR_MAX)
	{
		return;
	}

	if(led_num == LED2)
	{
		if(color_u8 == LED_COLOR_OFF)
		{
			LED3(OFF);
			LED4(OFF);
			LED5(OFF);		
		}
		else if(color_u8 == LED_COLOR_BLUE)
		{
			LED3(ON);
			LED4(OFF);
			LED5(OFF);		
		}
		else if(color_u8 == LED_COLOR_CYAN)
		{
			LED4(ON);
			LED5(ON);
			LED6(OFF);		
		}
		else if(color_u8 == LED_COLOR_GREEN)
		{
			LED3(OFF);
			LED4(ON);
			LED5(OFF);		
		}
		else if(color_u8 == LED_COLOR_MAGENTA)
		{
			LED4(ON);
			LED5(OFF);
			LED6(ON);

		}
		else if(color_u8 == LED_COLOR_YELLOW)
		{
			LED4(OFF);
			LED5(ON);
			LED6(ON);		
		}
		else if(color_u8 == LED_COLOR_WHITE)
		{
			LED4(ON);
			LED5(ON);
			LED6(ON);		
		}
	}
	else if(led_num == LED1)
	{
		if(color_u8 == LED_COLOR_OFF)
		{
			LED1(OFF);
			LED2(OFF);
			LED3(OFF);		
		}
		else if(color_u8 == LED_COLOR_BLUE)
		{
			LED1(ON);
			LED2(OFF);
			LED3(OFF);	
		}
		else if(color_u8 == LED_COLOR_CYAN)
		{
			LED1(ON);
			LED2(ON);
			LED3(OFF);		
		}
		else if(color_u8 == LED_COLOR_GREEN)
		{
			LED1(OFF);
			LED2(ON);
			LED3(OFF);		
		}
		else if(color_u8 == LED_COLOR_MAGENTA)
		{
			LED1(ON);
			LED2(OFF);
			LED3(ON);
		}
		else if(color_u8 == LED_COLOR_YELLOW)
		{
			LED1(OFF);
			LED2(ON);
			LED3(ON);		
		}
		else if(color_u8 == LED_COLOR_WHITE)
		{
			LED1(ON);
			LED2(ON);
			LED3(ON);		
		}			
	}
}

void Api_Led_Color_Blink_Freq(LED_NUM led_num, uint8_t color1_u8, uint8_t color2_u8, uint16_t blink_freq)
{
	uint32_t period;
	period = 1000 / blink_freq;
	
	if(color1_u8 > LED_COLOR_MAX || color2_u8 > LED_COLOR_MAX)
	{
		return;
	}

	API_Led_Color_Control(led_num, color1_u8);
	HAL_Delay(period);
	API_Led_Color_Control(led_num, color2_u8);
	HAL_Delay(period);
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


