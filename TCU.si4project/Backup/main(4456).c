/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
//#include "adc.h"
//#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "bsp_can.h"
#include "temp_pres_drv.h"
#include "spi_flash.h"
#include "ccp.h"
#include "tle7242.h"
#include "fm25cl64b.h"
#include "pi.h"
#include "led.h"
#include "mcp2515.h"
#include "can_queue.h"
#include "dac.h"
#include "bsp_user.h"
#include "fm24cl64b_i2c.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/


/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

unsigned int p_counter = 0;	//primitive counter
unsigned char receive_buffer[8];	//receive buffer


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
extern int ccpBootReceiveCro (unsigned char *msg);
extern void ccpBootInit (int cro_id, int dto_id);
extern int ccpBootTransmitCrmPossible( void );
extern void _timer_0(void);
extern int flag;
extern void init_timer_0 (unsigned long millisec);

uint8_t Time_1ms_Flag, Time_5ms_Flag, Time_10ms_Flag, Time_20ms_Flag, Time_50ms_Flag, Time_100ms_Flag, Time_500ms_Flag, Time_1000ms_Flag;

int main(void)
{
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	SystemClock_Config();
	//LED_GPIO_Config();
	//TLE7242_GPIO_Init();
	//FM25CL_GPIO_Init();
	Rheostat_Init();
	//TIM1_Mode_Config(10000);
	DEBUG_UART_Config();	
	//MX_SPI2_Init();
	//MX_SPI3_Init();
	//Pi_Drv_Init();
	//mcp2515_hw_init();
	dac_init(1);
	//printf("Init.....OK!!!\r\n");

	dac_set_voltage(1, 3000);

	I2C_Fram_Init();

	//ClearCanQueue();
	//init_timer_0(10);
	ccpBootInit(CCP_CRO_ID, CCP_DTO_ID);

	//ccpInit();

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  extern uint8_t Can_Send(CAN_PORT notused, Message *m, CAN_HandleTypeDef *hcan);
  Message m = {0};
  
  m.cob_id = 0x0a;
  m.rtr = 0;
	uint8_t value = 0x33;
  //memcpy(m.data, &value, 1);
  int i = 0;
	for(i = 0; i < 8; i++)
		m.data[i] = 0x33;
  m.len = 8;

	printf("hello world\r\n");
	
	uint8_t message_write[20]= "hello world\r\n";
	
	uint8_t message_read[20] = {0};
	
	if(HAL_OK != I2C_Fram_BufferWrite(message_write, 0, 20))
	{
		printf("write error\r\n");
	}
	else
	{
		printf("write ok\r\n");
	}
	
	if(HAL_OK != I2C_Fram_BufferRead(message_read, 0, 20))
	{
		printf("read error\r\n");
	}
	else
	{
		printf("read ok\r\n");
	}

  	printf("message_read %s\n", message_read);
	
	printf("message_write %s\n", message_write);

  while (1)
  {
	extern __IO uint32_t uwTick;
	//Can_Send(NULL, &m, &hcan1);
	//mdelay(1000);
	//uwTick = 0;
	bsp_application_task_loop();
		
	//printf("message_read %s\n", message_read);
	//printf("hello world\r\n");
 #if 0	
	if(ccpBootReceiveCro(receive_buffer))
	{   // if new CRO message received,
		ccpCommand(receive_buffer);                 // call ccpCommand
	}

	CCP_DISABLE_INTERRUPT;

	if(ccpBootTransmitCrmPossible())
	{
		ccpSendCallBack();
	}

	CCP_ENABLE_INTERRUPT;

	ccpBackground();

	init_timer_0(10);

	if(flag == 1)
	{
		_timer_0();
		flag = 0;
	}

	MCP2515_CanGet_SendQueue();

	LED1_OFF;
#endif	
  }  
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
