#ifndef __LED_H
#define __LED_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */


#include "stm32f4xx.h"


void LED_GPIO_Config(void);

void LED_GPIO_DeConfig(void);

void led1_show_yellow(void);
void led2_show_yellow(void);
void led1_show_purple(void);
void led2_show_purple(void);
void led1_show_cyan(void);
void led2_show_cyan(void);
void led1_show_white(void);
void led2_show_white(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */


#endif

