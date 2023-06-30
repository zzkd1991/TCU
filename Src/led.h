#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#include "stm32f4xx.h"


typedef enum
{
	LED1 = 0,
	LED2,

	LED_MAX
}LED_NUM;

enum
{
	LED_COLOR_OFF = 0,
	LED_COLOR_BLUE,
	LED_COLOR_GREEN,
	LED_COLOR_RED,
	LED_COLOR_CYAN,
	LED_COLOR_MAGENTA,
	LED_COLOR_YELLOW,
	LED_COLOR_WHITE,

	LED_COLOR_MAX
};
	
void LED_GPIO_DeConfig(void);

void API_Led_Color_Control(LED_NUM led_num, uint8_t color_u8);
void Api_Led_Color_Blink_Freq(LED_NUM led_num, uint8_t color1_u8, uint8_t color2_u8, uint16_t blink_freq);

void bsp_Led_Init(void);



#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif

