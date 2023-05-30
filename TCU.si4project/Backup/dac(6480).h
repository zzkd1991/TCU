#ifndef __BSP_DAC_H
#define	__BSP_DAC_H

#include "stm32f4xx.h"
#include "main.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define POINT_NUM	32

extern DAC_HandleTypeDef DAC1_Handler;

extern uint32_t DualSine12bit[POINT_NUM];

void DAC1_Init(void);

void DAC_Mode_Init(void);

void DAC1_Set_Vol(uint16_t vol);

#define TIMER_DAC           		TIM6
#define TIMER_DAC_CLK_ENABLE()     	__HAL_RCC_TIM6_CLK_ENABLE()
#define TIMER_DAC_CLK_DISABLE()    	__HAL_RCC_TIM6_CLK_DISABLE()

#define TIMER_DAC_PRESCALER			84 -1
#define TIMER_DAC_PERIOD			(uint32_t)(0xFFFF - 1)

extern TIM_HandleTypeDef TIM3_TimeBaseStructure;

#define TIMER_DAC_DMA_CLK_ENABLE()       __DMA1_CLK_ENABLE()
#define TIMER_DAC_DMA_CHANNEL            DMA_CHANNEL_7
#define TIMER_DAC_DMA_STREAM             DMA1_Stream1

#define TIMER_DAC_DMA_SRC_ADDR			(uint32_t)DualSine12bit

#define TIMER_DAC_DMA_DST_ADDR			((uint32_t)DAC + DAC_DHR12R1_ALIGNMENT(DAC_ALIGN_12B_R))

void AO_outVoltage(uint16_t Voltage);

uint16_t API_GET_AO_Voltage(void);

uint8_t AO_Diag_State_Get(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif


