#ifndef __BSP_TEMP_PRES_DRV_H
#define	__BSP_TEMP_PRES_DRV_H

#include "stm32f4xx.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif


#define RHEOSTAT_NOFCHANEL      9

//Vol_Drive1_Cur1
#define RHEOSTAT_ADC_GPIO_PORT1             GPIOC
#define RHEOSTAT_ADC_GPIO_PIN1              GPIO_PIN_0
#define RHEOSTAT_ADC_GPIO_CLK1_ENABLE()     __GPIOC_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL1               ADC_CHANNEL_10


//Vol_Drive1_Cur2(PC1比例电磁铁输出电压放大)
#define RHEOSTAT_ADC_GPIO_PORT2             GPIOC
#define RHEOSTAT_ADC_GPIO_PIN2              GPIO_PIN_1
#define RHEOSTAT_ADC_GPIO_CLK2_ENABLE()     __GPIOC_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL2               ADC_CHANNEL_11

//Vin_ADC(PC2采样电源电压)
#define RHEOSTAT_ADC_GPIO_PORT3				GPIOC
#define RHEOSTAT_ADC_GPIO_PIN3				GPIO_PIN_2
#define RHEOSTAT_ADC_GPIO_CLK3_ENABLE()		__GPIOC_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL3				ADC_CHANNEL_12

//EX_CH1(PC3温压一体传感器)
#define RHEOSTAT_ADC_GPIO_PORT4             GPIOC
#define RHEOSTAT_ADC_GPIO_PIN4              GPIO_PIN_3
#define RHEOSTAT_ADC_GPIO_CLK4_ENABLE()     __GPIOC_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL4               ADC_CHANNEL_13

//EX_CH2(PC4温压一体传感器)
#define RHEOSTAT_ADC_GPIO_PORT5             GPIOC
#define RHEOSTAT_ADC_GPIO_PIN5              GPIO_PIN_4
#define RHEOSTAT_ADC_GPIO_CLK5_ENABLE()     __GPIOC_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL5               ADC_CHANNEL_14

//EX_CH3(PC5温压一体传感器)
#define RHEOSTAT_ADC_GPIO_PORT6             GPIOC
#define RHEOSTAT_ADC_GPIO_PIN6              GPIO_PIN_5
#define RHEOSTAT_ADC_GPIO_CLK6_ENABLE()     __GPIOC_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL6               ADC_CHANNEL_15

//EX_CH4(PB0温压一体传感器)
#define RHEOSTAT_ADC_GPIO_PORT7             GPIOB
#define RHEOSTAT_ADC_GPIO_PIN7              GPIO_PIN_0
#define RHEOSTAT_ADC_GPIO_CLK7_ENABLE()     __GPIOB_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL7               ADC_CHANNEL_8

//Vol_Drive2_Cur1
#define RHEOSTAT_ADC_GPIO_PORT8				GPIOA
#define RHEOSTAT_ADC_GPIO_PIN8				GPIO_PIN_0
#define RHEOSTAT_ADC_GPIO_CLK8_ENABLE()		__GPIOA_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL8				ADC_CHANNEL_0

//Vol_Drive2_Cur2
#define RHEOSTAT_ADC_GPIO_PORT9				GPIOA
#define RHEOSTAT_ADC_GPIO_PIN9				GPIO_PIN_1
#define RHEOSTAT_ADC_GPIO_CLK9_ENABLE()		__GPIOA_CLK_ENABLE()
#define RHEOSTAT_ADC_CHANNEL9				ADC_CHANNEL_1

#define RHEOSTAT_ADC                        ADC1
#define RHEOSTAT_ADC_CLK_ENABLE()           __ADC1_CLK_ENABLE()


#define RHEOSTAT_ADC_DR_ADDR                ((uint32_t)ADC1+0x4c)


#define RHEOSTAT_ADC_DMA_CLK_ENABLE()       __DMA2_CLK_ENABLE()
#define RHEOSTAT_ADC_DMA_CHANNEL            DMA_CHANNEL_0
#define RHEOSTAT_ADC_DMA_STREAM             DMA2_Stream0

extern void Error_Handler(void);

void Rheostat_Init(void);

extern void Rheostat_Init(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif






