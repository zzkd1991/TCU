#include <string.h>
#include "tle7242.h"
#include "spi.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx.h"
#include "bsp_output_docc.h"
/* Fclk∑∂ŒßŒ™20-40  ±æ¿˙≥Ã÷–»°20Mhz */
/* fpwm∆µ¬ ∑∂ŒßŒ™50-4000Hz*/


extern SPI_HandleTypeDef hspi1;

extern uint8_t spi2_trx(uint16_t len, uint8_t *w, uint8_t *r);

tag_TLE_CONFIG_RECORD tag_tle_record = {0};


void TLE7242_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

	TLE7242_CS1_GPIO_CLK_ENABLE();
	TLE7242_CS2_GPIO_CLK_ENABLE();
	TLE7242_CS3_GPIO_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = TLE7242_CS1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TLE7242_CS1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = TLE7242_CS2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(TLE7242_CS2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = TLE7242_CS3_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(TLE7242_CS3_PORT, &GPIO_InitStruct);


	TLE7242_FAULT1_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = TLE7242_FAULT1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(TLE7242_FAULT1_INT_GPIO_PORT, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(TLE7242_FAULT1_EXTI_IRQ, 0, 0);
	HAL_NVIC_EnableIRQ(TLE7242_FAULT1_EXTI_IRQ);

	GPIO_InitStruct.Pin = TLE7242_FAULT2_PIN;
	HAL_GPIO_Init(TLE7242_FAULT2_INT_GPIO_PORT, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(TLE7242_FAULT2_EXIT_IRQ, 0, 0);
	HAL_NVIC_EnableIRQ(TLE7242_FAULT2_EXIT_IRQ);

	GPIO_InitStruct.Pin = TLE7242_FAULT3_PIN;
	HAL_GPIO_Init(TLE7242_FAULT3_INT_GPIO_PORT, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(TLE7242_FAULT3_EXIT_IRQ, 0, 0);
	HAL_NVIC_EnableIRQ(TLE7242_FAULT3_EXIT_IRQ);	
	
}


uint32_t TLE_Register_Operation_Data(uint32_t SPI_Data_u32)
{
	uint8_t spi_tx_buf[4];
	uint32_t result = 0;
	uint8_t ret = HAL_OK;
	
	memcpy(spi_tx_buf, &SPI_Data_u32, 4);

	ret = spi2_trx(4, spi_tx_buf, (uint8_t *)&result);
	if(ret != HAL_OK)
	{
		Error_Handler();
	}
	
	return result;
}

void TLE_Chip_Output_Enable(void)
{
	//TLE7242_ENABLE_DIS();
	//TLE7242_RESET_DIS();
	return;
}
void TLE_Chip_Output_Disable(void)
{
	//TLE7242_ENABLE_ENA();
	//TLE7242_RESET_ENA();	
	return;
}
void TLE_Power_On_Init(void)
{
	//TLE7242_ENABLE_DIS();
	//TLE7242_RESET_DIS();
	return;
}

uint8_t TLE_Manufacturer_Info_Read(void)
{
	tag_TLE_Register tle_register = {0};

	tle_register.IC_Version.B.RW = 0;
	tle_register.IC_Version.B.MSG_ID = TLE_MSG0_ID;

	tle_register.IC_Version.U = TLE_Register_Operation_Data(tle_register.IC_Version.U);

	tag_tle_record.record_Version_Number = tle_register.IC_Version.B.Version_Number;
	tag_tle_record.record_Manuf_ID = tle_register.IC_Version.B.Manuf_ID;

	return HAL_OK;
}


uint8_t TLE_Channel_Pwm_Freq_Set(uint8_t channel_u8, uint16_t freq_u16)
{
	uint16_t N;
	tag_TLE_Register tle_register = {0};
	
	tag_tle_record.record_freq = freq_u16;	
	N = (uint16_t )((FCLK) / (32 * freq_u16));

	tag_tle_record.record_PWM_Divider = N;

	tle_register.Main_Period.B.RW = 1;
	tle_register.Main_Period.B.MSG_ID = TLE_MSG1_ID;
	tle_register.Main_Period.B.PWM_Divider = N;
	tle_register.Main_Period.B.CH = channel_u8;
	
	TLE_Register_Operation_Data(tle_register.Main_Period.U);
	
	return HAL_OK;
}
uint8_t TLE_Channel_Time_Offset_Set(uint8_t channel_u8, uint16_t offset_u16)
{
	tag_TLE_Register tle_register = {0};

	tle_register.PWM_Offset.B.RW = 1;
	tle_register.PWM_Offset.B.Phase_Sync_Offset = offset_u16;
	tle_register.PWM_Offset.B.MSG_ID = TLE_MSG2_ID;
	tle_register.PWM_Offset.B.CH = channel_u8;

	TLE_Register_Operation_Data(tle_register.PWM_Offset.U);
	
	return HAL_OK;
}
uint8_t TLE_Channel_Constant_Current_Set(uint8_t channel_u8, uint16_t current_u16)
{
	tag_TLE_Register tle_register = {0};

	tag_tle_record.record_current = current_u16;
	tle_register.Current_Set.B.RW = 1;
	tle_register.Current_Set.B.Current_Set_Point = current_u16;
	tle_register.Current_Set.B.MSG_ID = TLE_MSG3_ID;
	tle_register.Current_Set.B.CH = channel_u8;
	
	TLE_Register_Operation_Data(tle_register.Current_Set.U);
	
	return HAL_OK;
}
uint8_t TLE_Channel_Dither_Enable(uint8_t channel_u8, uint8_t dither_en, uint16_t dither_step)
{
	tag_TLE_Register tle_register = {0};

	tag_tle_record.record_dither_en = dither_en;
	tle_register.Current_Set.B.RW = 1;
	tle_register.Current_Set.B.Dither_Enable = dither_en;
	tle_register.Current_Set.B.Step_Size = dither_step;
	tle_register.Current_Set.B.MSG_ID = TLE_MSG3_ID;
	tle_register.Current_Set.B.CH = channel_u8;
	
	TLE_Register_Operation_Data(tle_register.Current_Set.U);
	
	return HAL_OK;
}
uint8_t TLE_Channel_OnOff_Operate(uint8_t channel_u8, uint8_t on_off_u8)
{
	tag_TLE_Register tle_register = {0};

	tle_register.Current_Set.B.RW = 1;
	tle_register.Current_Set.B.ON_OFF = on_off_u8;
	tle_register.Current_Set.B.MSG_ID = TLE_MSG3_ID;
	tle_register.Current_Set.B.CH = channel_u8;
	
	TLE_Register_Operation_Data(tle_register.Current_Set.U);
	
	return HAL_OK;
}
uint8_t TLE_Channel_Dither_Freq_Set(uint8_t channel_u8, uint16_t dither_freq_u16)
{
	uint8_t Dither_Steps = 0;
	tag_TLE_Register tle_register = {0};
	
	Dither_Steps = (uint8_t)((FCLK) / (dither_freq_u16 * 4));

	tag_tle_record.record_dither_freq = dither_freq_u16;
	tag_tle_record.record_dither_steps = Dither_Steps;
	tle_register.Dither_Period.B.RW = 1;
	tle_register.Dither_Period.B.Dither_Steps = Dither_Steps;
	tle_register.Dither_Period.B.MSG_ID = TLE_MSG4_ID;
	tle_register.Dither_Period.B.CH = channel_u8;
	
	TLE_Register_Operation_Data(tle_register.Dither_Period.U);
	
	return HAL_OK;
}
uint8_t TLE_Channel_KP_KI_Set(uint8_t channel_u8, uint16_t kp_u16, uint16_t ki_u16)
{
	tag_TLE_Register tle_register = {0};

	tag_tle_record.record_ki_value = ki_u16;
	tag_tle_record.record_kp_value = kp_u16;
	tle_register.KP_KI.B.RW = 1;
	tle_register.KP_KI.B.KI = ki_u16;
	tle_register.KP_KI.B.KP = kp_u16;
	tle_register.KP_KI.B.MSG_ID = TLE_MSG5_ID;
	tle_register.KP_KI.B.CH = channel_u8;
	
	TLE_Register_Operation_Data(tle_register.KP_KI.U);
	
	return HAL_OK;
}
uint8_t TLE_Channel_Dynamic_Threshold_Set(uint8_t channel_u8, uint16_t integrator_preload, uint16_t transient_threshold)
{
	tag_TLE_Register tle_register = {0};

	tle_register.Dynamic_Threshold.B.RW = 1;
	tle_register.Dynamic_Threshold.B.Integrator_preload = integrator_preload;
	tle_register.Dynamic_Threshold.B.Transient_Threshold = transient_threshold;
	tle_register.Dynamic_Threshold.B.MSG_ID = TLE_MSG6_ID;
	tle_register.Dynamic_Threshold.B.CH = channel_u8;

	TLE_Register_Operation_Data(tle_register.Dynamic_Threshold.U);
	
	return HAL_OK;
}
uint8_t TLE_Channel_Mode_Config(uint8_t channel_u8, uint8_t mode_u8)
{
	tag_TLE_Register tle_register = {0};

	tle_register.Fault_Mask.B.RW = 1;
	tle_register.Fault_Mask.B.MSG_ID = TLE_MSG7_ID;
	
	if(mode_u8 == TLE_MODE_ON_OFF)//ÂºÄÂÖ≥Ê®°Âºè
	{
		if(channel_u8 == PO1)
		{
			tle_register.Fault_Mask.B.CM0 = 1;
		}
		else if(channel_u8 == PO2)
		{
			tle_register.Fault_Mask.B.CM1 = 1;
		}
		else if(channel_u8 == PO3)
		{
			tle_register.Fault_Mask.B.CM2 = 1;
		}
		else if(channel_u8 == PO4)
		{
			tle_register.Fault_Mask.B.CM3 = 1;
		}

		TLE_Register_Operation_Data(tle_register.Fault_Mask.U);
	}
	else if(mode_u8 == TLE_MODE_CONST_CURRENT)//ÊÅíÊµÅÊ®°Âºè
	{
		if(channel_u8 == PO1)
		{
			tle_register.Fault_Mask.B.CM0 = 0;
		}
		else if(channel_u8 == PO2)
		{
			tle_register.Fault_Mask.B.CM1 = 0;
		}
		else if(channel_u8 == PO3)
		{
			tle_register.Fault_Mask.B.CM2 = 0;
		}
		else if(channel_u8 == PO4)
		{
			tle_register.Fault_Mask.B.CM3 = 0;
		}

		TLE_Register_Operation_Data(tle_register.Fault_Mask.U);
	}

	
	return HAL_OK;	
}

void TLE_AutoZero_Enable(int enable)
{
	tag_TLE_Register tle_register = {0};	

	tle_register.Fault_Mask.B.RW = 1;
	if(enable == 1)
	{
		tle_register.Fault_Mask.B.AZ_Disable = 0;	
	}
	else
	{
		tle_register.Fault_Mask.B.AZ_Disable = 1;	
	}
	tle_register.Fault_Mask.B.MSG_ID = TLE_MSG7_ID;

	
	TLE_Register_Operation_Data(tle_register.Fault_Mask.U);
}



uint32_t TLE_Channel_Diagnostic_Read(uint8_t channel_u8)
{
	tag_TLE_Register tle_register = {0};

	
	tle_register.Diagnostic_Read.B.RW = 0;
	tle_register.Diagnostic_Read.B.MSG_ID = TLE_MSG9_ID;
	tle_register.Diagnostic_Read.U = TLE_Register_Operation_Data(tle_register.Diagnostic_Read.U);

	
	return tle_register.Diagnostic_Read.U;	
}


uint16_t TLE_Channel_Current_Read(uint8_t channel_u8)//mA
{
	tag_TLE_Register tle_register = {0};
	uint16_t current_value;

	tle_register.Current_Read.B.RW = 0;
	tle_register.Current_Read.B.CH = channel_u8;
	tle_register.Current_Read.B.MSG_ID = TLE_MSG10_ID;
	
	tle_register.Current_Read.U = TLE_Register_Operation_Data(tle_register.Current_Read.U);
	
	current_value = (uint16_t)((tle_register.Current_Read.B.Current_Read * (1 << 14)) * (320 / Rsense));
	
	return current_value;
}
uint32_t TLE_Channel_Autozero_Read(uint8_t channel_u8)
{
	tag_TLE_Register tle_register = {0};

	tle_register.Autozero_Read.B.RW = 0;
	tle_register.Autozero_Read.B.CH = channel_u8;
	tle_register.Autozero_Read.B.MSG_ID = TLE_MSG11_ID;
	
	tle_register.Autozero_Read.U = TLE_Register_Operation_Data(tle_register.Autozero_Read.U);
	
	return tle_register.Autozero_Read.U;
}
uint32_t TLE_Channel_Duty_Read(uint8_t channel_u8)
{
	tag_TLE_Register tle_register = {0};

	
	tle_register.DutyCycle_Read.B.CH = channel_u8;
	tle_register.DutyCycle_Read.B.MSG_ID = TLE_MSG12_ID;
	tle_register.DutyCycle_Read.U = TLE_Register_Operation_Data(tle_register.DutyCycle_Read.U);
	 	
	return tle_register.DutyCycle_Read.B.Duty_Cycle;
}
void TLE_ALL_Register_Period_Read(uint8_t channel_u8, uint16_t period_u16)
{
	return;
}


