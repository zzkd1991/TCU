#include "stm32f4xx.h"
#include "main.h"
#include "dac.h"

DAC_HandleTypeDef g_dac1_handle;

void dac_init(uint8_t outx)
{
    __HAL_RCC_DAC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    GPIO_InitTypeDef gpio_init_struct;

    gpio_init_struct.Pin = (outx==1) ? GPIO_PIN_4 : GPIO_PIN_5;
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;
    gpio_init_struct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    g_dac1_handle.Instance = DAC;
    HAL_DAC_Init(&g_dac1_handle);

    DAC_ChannelConfTypeDef dac_ch_conf;
    dac_ch_conf.DAC_Trigger = DAC_TRIGGER_NONE;
    dac_ch_conf.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
    
    switch(outx)
    {
        case 1:
            HAL_DAC_ConfigChannel(&g_dac1_handle, &dac_ch_conf, DAC_CHANNEL_1);
            HAL_DAC_Start(&g_dac1_handle,DAC_CHANNEL_1);
            break;
        case 2:
            HAL_DAC_ConfigChannel(&g_dac1_handle, &dac_ch_conf, DAC_CHANNEL_2);
            HAL_DAC_Start(&g_dac1_handle,DAC_CHANNEL_2);
            break;
        default : break;
    }
}


void dac_set_voltage(uint8_t outx, uint16_t vol)
{
    double temp = vol;
    temp /= 1000;
    temp = temp * 4096 / 3.3;

    if (temp >= 4096) temp = 4095;

    if (outx == 1)
    {
        HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, temp);
    }
    else
    {
        HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_2, DAC_ALIGN_12B_R, temp);
    }
}



