#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#include "stm32f4xx.h"

//R
#define LED1_PIN 	GPIO_PIN_9
#define LED1_GPIO_PORT		GPIOF
#define LED1_GPIO_CLK_ENABLE()		__GPIOF_CLK_ENABLE()
#define LED1_GPIO_CLK_DISABLE()		__GPIOF_CLK_DISABLE()


//Y
#define LED2_PIN	GPIO_PIN_10
#define LED2_GPIO_PORT	GPIOF
#define LED2_GPIO_CLK_ENABLE()		__GPIOF_CLK_ENABLE()
#define LED2_GPIO_CLK_DISABLE()		__GPIOF_CLK_DISABLE()


//G
#define LED3_PIN	GPIO_PIN_12
#define LED3_GPIO_PORT	GPIOB
#define LED3_GPIO_CLK_ENABLE()		__GPIOB_CLK_ENABLE()
#define LED3_GPIO_CLK_DISABLE()		__GPIOB_CLK_DISABLE()


//测试用
#define LED4_PIN	GPIO_PIN_0
#define LED4_GPIO_PORT	GPIOB
#define LED4_GPIO_CLK_ENABLE()		__GPIOB_CLK_ENABLE()
#define LED4_GPIO_CLK_DISABLE()		__GPIOB_CLK_DISABLE()

#define ON 	GPIO_PIN_SET
#define OFF GPIO_PIN_RESET

#define LED1(a)	HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_PIN, a)
#define LED2(a) HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, a)
#define LED3(a) HAL_GPIO_WritePin(LED3_GPIO_PORT, LED3_PIN, a)


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

//测试用
#define LED4_TOGGLE		led_digitalToggle(LED4_GPIO_PORT, LED4_PIN)
#define LED4_ON			led_digitalHi(LED4_GPIO_PORT, LED4_PIN)
#define LED4_OFF		led_digitalLo(LED4_GPIO_PORT, LED4_PIN)

#define USART3_ON		led_digitalHi(GPIOC, GPIO_PIN_10)
#define USART3_OFF		led_digitalLo(GPIOC, GPIO_PIN_10)
#define USART3_TOGGLE	led_digitalToggle(GPIOC, GPIO_PIN_10)

#define LED_RGYOFF	\
			LED1_OFF;\
			LED2_OFF\
			LED3_OFF


void LED_GPIO_Config(void);

void LED_GPIO_DeConfig(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif

