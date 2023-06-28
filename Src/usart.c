/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"
#include "stm32f4xx_hal_uart.h"
#include <stdio.h>

UART_HandleTypeDef UartHandle;
extern void Error_Handler(void);


void DEBUG_UART_Config(void)
{
	UartHandle.Instance = DEBUG_UART;
	
	UartHandle.Init.BaudRate = DEBUG_UART_BAUDRATE;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits   = UART_STOPBITS_1;
	UartHandle.Init.Parity			= UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode  			= UART_MODE_TX_RX;
	UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
	
	if(HAL_UART_Init(&UartHandle) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_UART_ENABLE_IT(&UartHandle, UART_IT_RXNE);
}


void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	DEBUG_UART_CLK_ENABLE();
	DEBUG_UART_RX_GPIO_CLK_ENABLE();
	DEBUG_UART_TX_GPIO_CLK_ENABLE();	
	/**UART3 GPIO Configuration
	PB10	----------> USART3_TX
	PB11	----------> USART3_RX
	*/
	GPIO_InitStruct.Pin = DEBUG_UART_TX_PIN | DEBUG_UART_RX_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = DEBUG_UART_TX_AF;
	HAL_GPIO_Init(DEBUG_UART_TX_GPIO_PORT, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(DEBUG_UART_IRQ, 4, 0);
	HAL_NVIC_EnableIRQ(DEBUG_UART_IRQ);

}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{
	DEBUG_UART_CLK_DISABLE();
	HAL_GPIO_DeInit(DEBUG_UART_TX_GPIO_PORT, DEBUG_UART_TX_PIN | DEBUG_UART_RX_PIN);
	HAL_NVIC_DisableIRQ(USART3_IRQn);
}


void Uart_SendString(uint8_t *str)
{
	unsigned int k = 0;
	do
	{
		HAL_UART_Transmit(&UartHandle, (uint8_t *)(str + k), 1, 1000);
		k++;
	} while(*(str + k) != '\0');
}

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 1000);
	
	return (ch);
}

int fgetc(FILE *f)
{
	int ch;
	HAL_UART_Receive(&UartHandle, (uint8_t *)&ch, 1, 1000);
	return (ch);
}








