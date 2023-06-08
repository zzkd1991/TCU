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

#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim1;

extern TIM_HandleTypeDef htim2;

extern TIM_HandleTypeDef htim3;

extern TIM_HandleTypeDef htim4;

extern TIM_HandleTypeDef htim5;

extern TIM_HandleTypeDef htim6;

extern TIM_HandleTypeDef htim8;

extern TIM_HandleTypeDef htim9;

extern TIM_HandleTypeDef htim12;

/* USER CODE BEGIN Private defines */
#define abs(x)    ((x)<=0?(-x):(x))
extern uint16_t Frequency[6];
extern uint32_t PI_Single_Pulse_Count[6];
extern uint8_t OverflowCount[2];
extern uint32_t PI_Double_Pulse_Count[2];
/* USER CODE END Private defines */

void MX_TIM1_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_TIM4_Init(void);
void MX_TIM5_Init(void);
void MX_TIM6_Init(void);
void MX_TIM8_Init(void);
void MX_TIM9_Init(void);
void MX_TIM12_Init(void);

/* USER CODE BEGIN Prototypes */
uint16_t API_PI_Double_Dir_Get(uint8_t group_u8);
void API_PI_Double_Count_Reset(uint8_t group_u8);
/* USER CODE END Prototypes */

	

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
