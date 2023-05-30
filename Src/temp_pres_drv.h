#ifndef __BSP_TEMP_PRES_DRV_H
#define	__BSP_TEMP_PRES_DRV_H

#include "stm32f4xx.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

#define BATP_EN_Pin GPIO_PIN_15
#define BATP_EN_GPIO_Port GPIOC
#define AI1_Pin GPIO_PIN_3
#define AI1_GPIO_Port GPIOF
#define AI2_Pin GPIO_PIN_4
#define AI2_GPIO_Port GPIOF
#define AI3_Pin GPIO_PIN_5
#define AI3_GPIO_Port GPIOF
#define AI4_Pin GPIO_PIN_6
#define AI4_GPIO_Port GPIOF
#define AI5_Pin GPIO_PIN_7
#define AI5_GPIO_Port GPIOF
#define AI6_Pin GPIO_PIN_8
#define AI6_GPIO_Port GPIOF
#define RI1_Pin GPIO_PIN_9
#define RI1_GPIO_Port GPIOF
#define RI2_Pin GPIO_PIN_10
#define RI2_GPIO_Port GPIOF
#define RI3_Pin GPIO_PIN_0
#define RI3_GPIO_Port GPIOC
#define BATS_M_Pin GPIO_PIN_1
#define BATS_M_GPIO_Port GPIOC
#define AI_AOUT_Pin GPIO_PIN_3
#define AI_AOUT_GPIO_Port GPIOC
#define UBP_ADIN_Pin GPIO_PIN_2
#define UBP_ADIN_GPIO_Port GPIOA
#define ADIN_12V_Pin GPIO_PIN_3
#define ADIN_12V_GPIO_Port GPIOA
#define AO_MCU_Pin GPIO_PIN_4
#define AO_MCU_GPIO_Port GPIOA
#define VREF_ADIN_Pin GPIO_PIN_5
#define VREF_ADIN_GPIO_Port GPIOA
#define Detetor_5V_Pin GPIO_PIN_4
#define Detetor_5V_GPIO_Port GPIOC
#define Detector_3V3_Pin GPIO_PIN_5
#define Detector_3V3_GPIO_Port GPIOC



void MX_ADC1_Init(void);

void MX_ADC3_Init(void);

void MX_DMA_Init(void);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif






