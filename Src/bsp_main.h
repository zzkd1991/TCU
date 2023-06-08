#ifndef BSP_MAIN_H
#define BSP_MAIN_H

#include "main.h"
#include "temp_pres_drv.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "bsp_can.h"
#include "temp_pres_drv.h"
#include "bsp_input_ai.h"
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
#include "bsp_input_di.h"
#include "bsp_input_ai.h"
#include "bsp_output_ao.h"
#include "usart_queue.h"

void Driver_Init(void);

void bsp_driver_task_1ms(void);
void bsp_driver_task_5ms(void);
void bsp_driver_task_10ms(void);
void bsp_driver_task_100ms(void);




#endif



