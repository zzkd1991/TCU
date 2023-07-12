/**
  ******************************************************************************
  * @file    spi.h
  * @brief   This file contains all the function prototypes for
  *          the spi.c file
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
#ifndef __SPI_H__
#define __SPI_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

#define SPI_WAIT (1000)	
	
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_SPI2_Init(void);
void MX_SPI3_Init(void);
/* USER CODE BEGIN Prototypes */
uint8_t spi2_trx(uint16_t len, uint8_t *w, uint8_t *r);

uint8_t spi3_trx(uint16_t len, uint8_t *w, uint8_t *r);
/* USER CODE END Prototypes */

uint8_t spi3_read_write_byte(uint8_t txdata);

uint32_t __spi2_trx(uint32_t data);

uint8_t __SPI_Send(uint8_t write_data);


#define MOSI_SET      HAL_GPIO_WritePin(GPIOB , GPIO_PIN_15, GPIO_PIN_SET)
#define MOSI_CLR      HAL_GPIO_WritePin(GPIOB , GPIO_PIN_15, GPIO_PIN_RESET)
#define SCK_SET      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
#define SCK_CLR      HAL_GPIO_WritePin(GPIOB , GPIO_PIN_13, GPIO_PIN_RESET)

#define mcp2515_mosi_set	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_SET)
#define mcp2515_mosi_clr	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, GPIO_PIN_RESET)
#define mcp2515_sck_set		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_SET)
#define mcp2515_sck_clr		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, GPIO_PIN_RESET)

#ifdef __cplusplus
}
#endif

#endif /* __SPI_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
