#include <string.h>
#include "tle7242.h"
#include "spi.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx.h"
#include "bsp_output_docc.h"
/* Fclk��ΧΪ20-40  ��������ȡ20Mhz */
/* fpwmƵ�ʷ�ΧΪ50-4000Hz*/


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
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(TLE7242_CS1_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = TLE7242_CS2_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	//GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(TLE7242_CS2_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = TLE7242_CS3_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	//GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(TLE7242_CS3_PORT, &GPIO_InitStruct);


	TLE7242_FAULT1_GPIO_CLK_ENABLE();
	GPIO_InitStruct.Pin = TLE7242_FAULT1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
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

	/*普通GPIO模拟SPI*/
	__GPIOB_CLK_ENABLE();
	__GPIOC_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;//GPIO输入模式
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;//管脚下拉
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}


uint8_t TLE_Register_ReadReg(MesgDef Mesg, MesgDef *rd_mesg)
{
	uint8_t ret = HAL_OK;
	uint8_t read_value[4] = {0};
	uint8_t write_value[4] = {0};

	write_value[0] = Mesg.Mes24_31;
	write_value[1] = Mesg.Mes16_23;
	write_value[2] = Mesg.Mes8_15;
	write_value[3] = Mesg.Mes0_7;
	
	ret = spi2_trx(4, write_value, NULL);
	if(ret != HAL_OK)
	{
		Error_Handler();
	}
	

	ret = spi2_trx(4, NULL, (uint8_t *)read_value);
	if(ret != HAL_OK)
	{
		Error_Handler();
	}

	if(read_value[0] == Mesg.Mes24_31)
	{
		memcpy(rd_mesg, read_value, sizeof(MesgDef));
		return 0;
	}
	else
	{
		return 1;
	}
}

uint8_t TLE_Register_WriteReg(MesgDef Mesg)
{
	uint8_t ret = HAL_OK;
	uint8_t read_value[4] = {0};
	uint8_t write_value[4] = {0};

	write_value[0] = Mesg.Mes24_31;
	write_value[1] = Mesg.Mes16_23;
	write_value[2] = Mesg.Mes8_15;
	write_value[3] = Mesg.Mes0_7;
	
	ret = spi2_trx(4, write_value, (uint8_t *)read_value);
	if(ret != HAL_OK)
	{
		Error_Handler();
	}
	

	/*ret = spi2_trx(4, NULL, (uint8_t *)read_value);
	if(ret != HAL_OK)
	{
		Error_Handler();
	}

	if((write_value[0] == read_value[0]) && (write_value[1] == read_value[1]) && (write_value[2] == read_value[2]) && (write_value[3] == read_value[3]))
	{
		return 0;
	}
	else*/
	{
		return 1;
	}
}

uint8_t __TLE_Register_WriteReg(uint32_t write_data)
{
	uint32_t read_data;
	SPI_Send(write_data);
	read_data = SPI_Send(0);
	uint32_t mask_value = 1 << 31;

	//printf("write_data %x, read_data %x\r\n", write_data, read_data);

	
	if((read_data & ~mask_value) != (write_data & ~mask_value))
	{
		Error_Handler();
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t __TLE_Register_ReadReg(uint32_t write_data, uint32_t *read_data)
{
	uint32_t __read_data;
	SPI_Send(write_data);

	__read_data = SPI_Send(0);

	if((__read_data >> 24) != (write_data >> 24))
	{
		Error_Handler();
		*read_data = __read_data;
		return 1;
	}
	else
	{
		return 0;
	}
}



uint32_t TLE_Register_Operation_Data(MesgDef Mesg)
{
	uint32_t result = 0;
	uint32_t writevalue = 0;
	int i = 0;
	uint8_t ret = HAL_OK;
	uint8_t read_value[4] = {0};
	uint8_t write_value[4] = {0};
	uint8_t read_value1[4] = {0};
	uint32_t result1 = 0;

	write_value[0] = Mesg.Mes24_31;
	write_value[1] = Mesg.Mes16_23;
	write_value[2] = Mesg.Mes8_15;
	write_value[3] = Mesg.Mes0_7;

	/*for(i = 0; i < 4; i++)
	{
		printf("write_value[%d] %x\r\n", i, write_value[i]);

	}*/

	writevalue = (Mesg.Mes24_31 << 24) | (Mesg.Mes16_23 << 16) | (Mesg.Mes8_15 << 8) | Mesg.Mes0_7;
	
	ret = spi2_trx(4, write_value, read_value1);
	if(ret != HAL_OK)
	{
		Error_Handler();
	}

	result1 = (read_value1[0] << 24) | (read_value1[1] << 16) | (read_value1[2] << 8) | read_value1[3];
	

	ret = spi2_trx(4, NULL, (uint8_t *)read_value);
	if(ret != HAL_OK)
	{
		Error_Handler();
	}

	
	result = (read_value[0] << 24) | (read_value[1] << 16) | (read_value[2] << 8) | read_value[3];
	//printf("SPI_Data_u32 %x\r\n", SPI_Data_u32);
	printf("writevalue result1 result %x %x %x\r\n", writevalue, result1, result);
	
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

uint32_t test_value = 0;


uint8_t TLE_Manufacturer_Info_Read(void)
{
	tag_TLE_Register tle_register = {0};
	MesgDef rd_mesg = {0};
	uint8_t ret;
	//int i;
	uint8_t read_value1[4] = {0};
	uint8_t read_value2[4] = {0};
	uint8_t write_value[4] = {0};

	//tle_register.IC_Version.U = 0xC10200(0x2c100);
	
	tle_register.IC_Version.B.RW = 0;
	tle_register.IC_Version.B.MSG_ID = TLE_MSG0_ID;

#if 0
	write_value[0] = (tle_register.IC_Version.U & 0xff000000) >> 24;
	write_value[1] = (tle_register.IC_Version.U & 0xff0000) >> 16;
	write_value[2] = (tle_register.IC_Version.U & 0xff00) >> 8;
	write_value[3] = tle_register.IC_Version.U & 0xff;

	/*ret = spi2_trx(4, NULL, read_value1);
	if(ret != HAL_OK)
	{
		Error_Handler();
	}*/

	ret = spi2_trx(4, write_value, read_value1);
	if(ret != 0)
	{
		Error_Handler();
		return 1;
	}

#else
	tle_register.IC_Version.U = SPI_Send(tle_register.IC_Version.U);
#endif

	printf("tle_register.IC_Version.U %x\r\n", tle_register.IC_Version.U);

	//tle_register.IC_Version.B.Manuf_ID = read_value1[1];
	//tle_register.IC_Version.B.Version_Number = read_value1[2];

	if(tle_register.IC_Version.B.Manuf_ID != 0xC1 && tle_register.IC_Version.B.Version_Number != 0x02)
	{
		Error_Handler();
		//while(1);
		return 1;
	}

	tag_tle_record.record_Version_Number = tle_register.IC_Version.B.Version_Number;
	tag_tle_record.record_Manuf_ID = tle_register.IC_Version.B.Manuf_ID;

	return HAL_OK;
}


uint8_t TLE_Channel_Pwm_Freq_Set(uint8_t channel_u8, uint16_t freq_u16)
{
	uint16_t N;
	tag_TLE_Register tle_register = {0};
	uint8_t ret;
	
	tag_tle_record.record_freq = freq_u16;	
	N = (uint16_t )((FCLK) / (32 * freq_u16));

	//printf("N %d\r\n", N);

	tag_tle_record.record_PWM_Divider = N;

	tle_register.Main_Period.B.RW = 1;
	tle_register.Main_Period.B.MSG_ID = TLE_MSG1_ID;
	tle_register.Main_Period.B.PWM_Divider = N;
	tle_register.Main_Period.B.CH = channel_u8;

#if 0
	TLE_Register_WriteReg(tle_register.Main_Period.Mesg);
	if(ret != 0)
	{
		Error_Handler();
	}
#else
	//SPI_Send(tle_register.Main_Period.U);
	__TLE_Register_WriteReg(tle_register.Main_Period.U);
#endif
	
	return ret;
}

uint8_t TLE_Channel_Time_Offset_Set(uint8_t channel_u8, uint16_t offset_u16)
{
	tag_TLE_Register tle_register = {0};
	uint8_t ret = 0;

	tle_register.PWM_Offset.B.RW = 1;
	tle_register.PWM_Offset.B.Phase_Sync_Offset = offset_u16;
	tle_register.PWM_Offset.B.MSG_ID = TLE_MSG2_ID;
	tle_register.PWM_Offset.B.CH = channel_u8;

#if 0
	ret = TLE_Register_WriteReg(tle_register.PWM_Offset.Mesg);
	if(ret != 0)
	{
		Error_Handler();
	}
#else
	//SPI_Send(tle_register.PWM_Offset.U);
	__TLE_Register_WriteReg(tle_register.PWM_Offset.U);
#endif
	return ret;
}


uint8_t TLE_Channel_Constant_Current_Set(uint8_t channel_u8, uint16_t current_u16)
{
	tag_TLE_Register tle_register = {0};
	uint8_t ret = 0;

	tag_tle_record.record_current = current_u16;
	tle_register.Current_Set.B.RW = 1;
	tle_register.Current_Set.B.EN = 1;
	tle_register.Current_Set.B.Dither_Enable = 1;
	tle_register.Current_Set.B.Current_Set_Point = current_u16;
	tle_register.Current_Set.B.MSG_ID = TLE_MSG3_ID;
	tle_register.Current_Set.B.CH = channel_u8;

	//printf("write value %x", tle_register.Current_Set.U);
#if 0
	ret = TLE_Register_WriteReg(tle_register.Current_Set.Mesg);
	if(ret != 0)
	{
		Error_Handler();
	}

	printf("bbbbb\r\n");
#else
	//SPI_Send(tle_register.Current_Set.U);
	__TLE_Register_WriteReg(tle_register.Current_Set.U);
#endif
	return ret;
}
uint8_t TLE_Channel_Dither_Enable(uint8_t channel_u8, uint8_t dither_en, uint16_t dither_step)
{
	tag_TLE_Register tle_register = {0};
	uint8_t ret = 0;

	tag_tle_record.record_dither_en = dither_en;
	tle_register.Current_Set.B.RW = 1;
	tle_register.Current_Set.B.Dither_Enable = dither_en;
	tle_register.Current_Set.B.Step_Size = dither_step;
	tle_register.Current_Set.B.MSG_ID = TLE_MSG3_ID;
	tle_register.Current_Set.B.CH = channel_u8;

#if 0	
	ret = TLE_Register_WriteReg(tle_register.Current_Set.Mesg);
	if(ret != 0)
	{
		Error_Handler();
	}
#else
	//SPI_Send(tle_register.Current_Set.U);
	__TLE_Register_WriteReg(tle_register.Current_Set.U);
#endif
	
	return ret;
}
uint8_t TLE_Channel_OnOff_Operate(uint8_t channel_u8, uint8_t on_off_u8)
{
	tag_TLE_Register tle_register = {0};

	tle_register.Current_Set.B.RW = 1;
	tle_register.Current_Set.B.ON_OFF = on_off_u8;
	tle_register.Current_Set.B.MSG_ID = TLE_MSG3_ID;
	tle_register.Current_Set.B.CH = channel_u8;

#if 0	
	TLE_Register_WriteReg(tle_register.Current_Set.Mesg);
#else
	//SPI_Send(tle_register.Current_Set.U);
	__TLE_Register_WriteReg(tle_register.Current_Set.U);
#endif
	return HAL_OK;
}


uint8_t TLE_Channel_Dither_Freq_Set(uint8_t channel_u8, uint16_t dither_freq_u16)
{
	uint8_t Dither_Steps = 0;
	tag_TLE_Register tle_register = {0};
	uint8_t ret = 0;
	
	Dither_Steps = (uint8_t)((FCLK) / (dither_freq_u16 * 4));

	tag_tle_record.record_dither_freq = dither_freq_u16;
	tag_tle_record.record_dither_steps = Dither_Steps;
	tle_register.Dither_Period.B.RW = 1;
	tle_register.Dither_Period.B.Dither_Steps = Dither_Steps;
	
	tle_register.Dither_Period.B.MSG_ID = TLE_MSG4_ID;
	tle_register.Dither_Period.B.CH = channel_u8;

#if 0	
	ret = TLE_Register_WriteReg(tle_register.Dither_Period.Mesg);
	if(ret != 0)
	{
		Error_Handler();
	}
#else
	//SPI_Send(tle_register.Dither_Period.U);
	__TLE_Register_WriteReg(tle_register.Dither_Period.U);
#endif
	return HAL_OK;
}

uint8_t TLE_Channel_KP_KI_Set(uint8_t channel_u8, uint16_t kp_u16, uint16_t ki_u16)
{
	tag_TLE_Register tle_register = {0};
	uint8_t ret = 0;

	tag_tle_record.record_ki_value = ki_u16;
	tag_tle_record.record_kp_value = kp_u16;
	tle_register.KP_KI.B.RW = 1;
	tle_register.KP_KI.B.KI = ki_u16;
	tle_register.KP_KI.B.KP = kp_u16;
	tle_register.KP_KI.B.MSG_ID = TLE_MSG5_ID;
	tle_register.KP_KI.B.CH = channel_u8;

#if 0
	ret = TLE_Register_WriteReg(tle_register.KP_KI.Mesg);
	if(ret != 0)
	{
		Error_Handler();
	}

	//printf("cccc\r\n");
#else
	//SPI_Send(tle_register.KP_KI.U);
	__TLE_Register_WriteReg(tle_register.KP_KI.U);
#endif
	return ret;
}
uint8_t TLE_Channel_Dynamic_Threshold_Set(uint8_t channel_u8, uint16_t integrator_preload, uint16_t transient_threshold)
{
	tag_TLE_Register tle_register = {0};\
	uint8_t ret = 0;

	tle_register.Dynamic_Threshold.B.RW = 1;
	tle_register.Dynamic_Threshold.B.Integrator_preload = integrator_preload;
	tle_register.Dynamic_Threshold.B.Transient_Threshold = transient_threshold;
	tle_register.Dynamic_Threshold.B.MSG_ID = TLE_MSG6_ID;
	tle_register.Dynamic_Threshold.B.CH = channel_u8;

#if 0
	TLE_Register_WriteReg(tle_register.Dynamic_Threshold.Mesg);
	
	if(ret != 0)
	{
		Error_Handler();
	}
#else
	//SPI_Send(tle_register.Dynamic_Threshold.U);
	__TLE_Register_WriteReg(tle_register.Dynamic_Threshold.U);
#endif
	return ret;
}
uint8_t TLE_Channel_Mode_Config(uint8_t channel_u8, uint8_t mode_u8)
{
	tag_TLE_Register tle_register = {0};
	uint32_t result = 0;
	uint8_t ret = 0;

	tle_register.Fault_Mask.B.RW = 1;
	tle_register.Fault_Mask.B.MSG_ID = TLE_MSG7_ID;
	tle_register.Fault_Mask.B.DIAG_TMR = 3;
	
	if(mode_u8 == TLE_MODE_ON_OFF)//开关模式
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

#if 0
		ret = TLE_Register_WriteReg(tle_register.Fault_Mask.Mesg);
		if(ret != 0)
		{
			Error_Handler();
		}
#else
		//SPI_Send(tle_register.Fault_Mask.U);
		__TLE_Register_WriteReg(tle_register.Fault_Mask.U);
#endif
	}
	else if(mode_u8 == TLE_MODE_CONST_CURRENT)//恒流模式
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

#if 0
		//printf("tle_register.Fault_Mask.U %x\r\n", tle_register.Fault_Mask.U);
		ret = TLE_Register_WriteReg(tle_register.Fault_Mask.Mesg);
		if(ret != 0)
		{
			Error_Handler();
		}		
		//printf("result %x\r\n", result);
#else
		//SPI_Send(tle_register.Fault_Mask.U);
		__TLE_Register_WriteReg(tle_register.Fault_Mask.U);
#endif
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

#if 0
	TLE_Register_WriteReg(tle_register.Fault_Mask.Mesg);
#else
	//SPI_Send(tle_register.Fault_Mask.U);
	__TLE_Register_WriteReg(tle_register.Fault_Mask.U);
#endif

}


uint32_t TLE_Channel_Diagnostic_Read(uint8_t channel_u8, uint32_t *Diagnostic_Info)
{
	tag_TLE_Register tle_register = {0};
	MesgDef rd_mesg = {0};
	uint8_t ret = 0;
	
	tle_register.Diagnostic_Read.B.RW = 0;
	tle_register.Diagnostic_Read.B.MSG_ID = TLE_MSG9_ID;

#if 0
	ret = TLE_Register_ReadReg(tle_register.Diagnostic_Read.Mesg, &rd_mesg);
	if(ret != 0)
	{
		Error_Handler();
	}
	else
	{
		*Diagnostic_Info = (rd_mesg.Mes24_31 << 24) | (rd_mesg.Mes16_23 << 16) | (rd_mesg.Mes8_15 << 8) | rd_mesg.Mes0_7;
	}
#endif
	//*Diagnostic_Info = SPI_Send(tle_register.Diagnostic_Read.U);
	__TLE_Register_ReadReg(tle_register.Diagnostic_Read.U, Diagnostic_Info);
	return ret;	
}


uint16_t TLE_Channel_Current_Read(uint8_t channel_u8, uint16_t *Current_Value)//mA
{
	tag_TLE_Register tle_register = {0};
	MesgDef rd_mesg = {0};
	uint8_t ret = 0;

	tle_register.Current_Read.B.RW = 0;
	tle_register.Current_Read.B.CH = channel_u8;
	tle_register.Current_Read.B.MSG_ID = TLE_MSG10_ID;

#if 0
	ret = TLE_Register_ReadReg(tle_register.Current_Read.Mesg, &rd_mesg);
	if(ret != 0)
	{
		Error_Handler();
	}
	else
	{
		tle_register.Current_Read.U = (rd_mesg.Mes24_31 << 24) | (rd_mesg.Mes16_23 << 16) | (rd_mesg.Mes8_15 << 8) | rd_mesg.Mes0_7;
		*Current_Value = (uint16_t)((tle_register.Current_Read.B.Current_Read * (1 << 14)) * (320 / Rsense));	
	}
#else
	//tle_register.Current_Read.U = SPI_Send(tle_register.Current_Read.U);
	__TLE_Register_ReadReg(tle_register.Current_Read.U, &tle_register.Current_Read.U);
	*Current_Value = (tle_register.Current_Read.B.Current_Read * 320) / ((1 << 14) * Rsense);
#endif
	return ret;
}


uint32_t TLE_Channel_Autozero_Read(uint8_t channel_u8, uint32_t *autozero)
{
	tag_TLE_Register tle_register = {0};
	uint8_t ret = 0;
	MesgDef rd_mesg = {0};

	tle_register.Autozero_Read.B.RW = 0;
	tle_register.Autozero_Read.B.CH = channel_u8;
	tle_register.Autozero_Read.B.MSG_ID = TLE_MSG11_ID;

#if 0
	ret = TLE_Register_ReadReg(tle_register.Autozero_Read.Mesg, &rd_mesg);
	if(ret != 0)
	{
		Error_Handler();
	}
	else
	{
		*autozero = (rd_mesg.Mes24_31 << 24) | (rd_mesg.Mes16_23 << 16) | (rd_mesg.Mes8_15 << 8) | rd_mesg.Mes0_7;
	}
#else
	//tle_register.Autozero_Read.U = SPI_Send(tle_register.Autozero_Read.U);
	__TLE_Register_ReadReg(tle_register.Autozero_Read.U, &tle_register.Autozero_Read.U);
	*autozero = tle_register.Autozero_Read.U;
#endif
	return ret;
}


uint32_t TLE_Channel_Duty_Read(uint8_t channel_u8, uint32_t *DutyCycle)
{
	tag_TLE_Register tle_register = {0};
	uint8_t ret = 0;
	MesgDef rd_mesg = {0};

	
	tle_register.DutyCycle_Read.B.CH = channel_u8;
	tle_register.DutyCycle_Read.B.MSG_ID = TLE_MSG12_ID;

#if 0
	ret = TLE_Register_ReadReg(tle_register.DutyCycle_Read.Mesg, &rd_mesg);
	if(ret != 0)
	{
		Error_Handler();
	}
	else
	{
		*DutyCycle = (rd_mesg.Mes24_31 << 24) | (rd_mesg.Mes16_23 << 16) | (rd_mesg.Mes8_15 << 8) | rd_mesg.Mes0_7;
	}
#else
	//tle_register.DutyCycle_Read.U = SPI_Send(tle_register.DutyCycle_Read.U);
	__TLE_Register_ReadReg(tle_register.DutyCycle_Read.U, &tle_register.DutyCycle_Read.U);
	*DutyCycle = tle_register.DutyCycle_Read.U;
#endif

	return ret;
}
void TLE_ALL_Register_Period_Read(uint8_t channel_u8, uint16_t period_u16)
{
	return;
}


