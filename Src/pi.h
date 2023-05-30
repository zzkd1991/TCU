#ifndef __PI_H__
#define __PI_H__

#ifdef __cplusplus
extern "C" {
#endif

	
#include <stdint.h>
#include <stm32f4xx_hal_tim.h>	
	
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

void MX_TIM2_Init(void);

void MX_TIM4_Init(void);

extern uint32_t pi1_capture_data1;
extern uint32_t pi1_capture_data2;
extern uint32_t pi2_capture_data1;
extern uint32_t pi2_capture_data2;
extern float pi1_freq_capture;
extern float pi2_freq_capture;


extern void Pi_Drv_Init(void);


#ifdef __cplusplus
}
#endif

#endif




