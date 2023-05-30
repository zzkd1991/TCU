/**
  ******************************************************************************
  * File Name          : USART.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "stm32f4xx.h"	 

#define DEBUG_UART_BAUDRATE										115200
#define DEBUG_UART													USART3
#define DEBUG_UART_CLK_ENABLE()							__USART3_CLK_ENABLE()

#define RCC_PERIPHCLK_UARTx									RCC_PERIPHCLK_USART4
#define RCC_UARTxCLKSOURCE_SYSCLK						RCC_UART4CLKSOURCE_SYSCLK


#define DEBUG_UART_RX_GPIO_PORT					GPIOC
#define DEBUG_UART_RX_GPIO_CLK_ENABLE()		__GPIOC_CLK_ENABLE()
#define DEBUG_UART_RX_PIN								GPIO_PIN_11
#define DEBUG_UART_RX_AF								GPIO_AF7_USART3


#define DEBUG_UART_TX_GPIO_PORT				GPIOC
#define DEBUG_UART_TX_GPIO_CLK_ENABLE()			__GPIOC_CLK_ENABLE()
#define DEBUG_UART_TX_PIN								GPIO_PIN_10
#define DEBUG_UART_TX_AF								GPIO_AF7_USART3


#define DEBUG_UART_IRQHandler					UART4_IRQHandler
#define DEBUG_UART_IRQ								USART3_IRQn
	 
void DEBUG_UART_Config(void);	 

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
