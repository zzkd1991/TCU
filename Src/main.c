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
#include "bsp_input_ai.h"
#include "bsp_input_di.h"
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
#include "bsp_output_do.h"
#include "can_mcp2515.h"
#include "exti.h"

#include "bsp_main.h"


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
extern int ccpBootTransmitCrmPossible( void );
extern void _timer_0(void);
extern int flag;
extern void init_timer_0 (unsigned long millisec);

int main(void)
{
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();
	SystemClock_Config(); 

   	Driver_Init();

	
   //ccpInit();

   /* Infinite loop */
   /* USER CODE BEGIN WHILE */
	

	uint8_t message_write[8]= "hello\r\n";
	
	uint8_t message_read[8] = {0};
	
	if(HAL_OK != I2C_Fram_PageWrite(message_write, 0, 8))
	{
		printf("write error\r\n");
	}
	else
	{
		printf("write ok\r\n");
	}
	
	if(HAL_OK != I2C_Fram_BufferRead(message_read, 0, 8))
	{
		printf("read error\r\n");
	}
	else
	{
		printf("read ok\r\n");
	}

  	printf("message_read %s\n", message_read);
	
	//printf("message_write %s\n", message_write);
	extern uint32_t SPI_FLASH_ReadDeviceID(void);
	extern uint32_t SPI_FLASH_ReadID(void);
	
	uint16_t device_id = 0;
	
	device_id = SPI_FLASH_ReadDeviceID();

	uint32_t FlashID = 0;

	FlashID = SPI_FLASH_ReadID();

	printf("device_id is %x\r\n", device_id);
	//printf("FlashID %x\r\n", FlashID);
	printf("FlashID %d\r\n", FlashID);
	
	Reset_DI_Chara(DIN1,0);//复用DI-测试


  while (1)
  {

	ADC_Smooth();//AI 采集

  DI_Screen();//DI 采集
	AI_Diagnose_State_Get();//AI1-AI6的诊断
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
	printf("%s, %d\r\n", __FUNCTION__, __LINE__);
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
