/**
  ******************************************************************************
  * @file    tim.h
  * @brief   This file contains all the function prototypes for
  *          the tim.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>	
	
#define TLE7242_TIM1	TIM1
#define TLE7242_TIM1_CLK_ENABLE()		__TIM1_CLK_ENABLE()

#define TLE7242_CLK_PIN							GPIO_PIN_8
#define TLE7242_GPIO_PORT						GPIOA
#define TLE7242_GPIO_CLK_ENABLE()		__GPIOA_CLK_ENABLE()
#define TLE7242_CLK_AF							GPIO_AF1_TIM1
	
void TIMx1_GPIO_Config(void);
void TIM1_Mode_Config(uint32_t pwm_freq);
	

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
