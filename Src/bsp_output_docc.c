#include "bsp_output_docc.h"
#include "spi.h"
#include "delay.h"
#include "gpio_spi.h"

extern tag_TLE_CONFIG_RECORD tag_tle_record;

uint8_t API_Manufacturer_Info_Get(uint8_t chan_u8)
{
	uint8_t ret = 0;

	if(chan_u8 > PO12)
	{
		Error_Handler();
		return 1;
	}

	ret = TLE_Manufacturer_Info_Read(chan_u8);


	return ret;
}


void API_ConstantCurrent_Drive(uint8_t chan_u8, uint16_t current_u16, uint16_t freq_u16,
								float kp_f, float ki_f)
{
	uint16_t kp;
	uint16_t ki;
	uint16_t conved_current;

	kp = (uint16_t)(kp_f * 4095);
	ki = (uint16_t)(ki_f * 4095);

	conved_current = (current_u16 * (1 << 11) * Rsense) / 320;

	if(chan_u8 > PO12)
	{
		Error_Handler();
		return;
	}

	TLE_Channel_Mode_Config(chan_u8, TLE_MODE_CONST_CURRENT);//恒流模式
	TLE_Channel_Constant_Current_Set(chan_u8, conved_current);
	//Message #1
	TLE_Channel_Pwm_Freq_Set(chan_u8, freq_u16);		
	TLE_Channel_KP_KI_Set(chan_u8, kp, ki);		
}


void API_PO_Mode_Config(uint8_t chan_u8, uint16_t mode_u8)
{
	TLE_Channel_Mode_Config(chan_u8, mode_u8);
}

void API_Dither_Par_Config(uint8_t chan_u8, uint8_t dither_enable, uint16_t dither_freq, uint8_t dither_amp)
{
	uint16_t conved_dither_amp;
	uint8_t dither_steps;

	dither_steps = (uint8_t)((FCLK) / (dither_freq * 4));
	conved_dither_amp = ((1 << 15) * Rsense * dither_amp) / (2 * 320  * tag_tle_record.record_dither_steps);

	tag_tle_record.record_dither_amp = dither_amp;

	if(chan_u8 > PO12)
	{
		Error_Handler();
		return;
	}

	TLE_Channel_Dither_Enable(chan_u8, dither_enable, conved_dither_amp);

}

uint16_t API_DynamicCurrent_Read(uint8_t chan_u8)
{
	uint16_t current_value;

	if(chan_u8 > PO12)
	{
		Error_Handler();
	}

	TLE_Channel_Current_Read(chan_u8, &current_value);


	return current_value;
}

void API_Power_Switch_Set(uint8_t chan_u8, uint8_t on_off_u8)
{
	if(chan_u8 > PO12)
	{
		Error_Handler();
		return;
	}

	TLE_Channel_Mode_Config(chan_u8, TLE_MODE_ON_OFF);//首先设置成On/Off模式
	TLE_Channel_OnOff_Operate(chan_u8, on_off_u8);	

}

uint16_t API_Duty_Feedback_Read(uint8_t chan_u8)
{
	uint32_t duty_cycle = 0;
	uint8_t actual_duty = 0;

	if(chan_u8 > PO12)
	{
		Error_Handler();
		return 0xff;
	}

	TLE_Channel_Duty_Read(chan_u8, &duty_cycle);	
	actual_duty = 100 * ((float)duty_cycle / (32 * tag_tle_record.record_PWM_Divider));

	return actual_duty;
	
}

uint32_t flag_5ms_passed = 0;


uint32_t bsp_Diag_Reset_Fault_PO(uint8_t chan_u8)
{
	uint16_t current_value = 0;
	tag_Diagnostic_Read Diagnostic_info = {0};
	tag_Autozero_Read autozero = {0};

	if(chan_u8 > PO12)
	{
		Error_Handler();
		return 0xff;
	}

	TLE_Channel_Diagnostic_Read(chan_u8, &Diagnostic_info.U);
	TLE_Channel_Autozero_Read(chan_u8, &autozero.U);
	TLE_Channel_Current_Read(chan_u8, &current_value);


	if(Diagnostic_info.B.SB3 == 1 || Diagnostic_info.B.SB2 == 1 || Diagnostic_info.B.SB1 == 1 || Diagnostic_info.B.SB0 == 1)
	{
		return 1;
	}

	if(Diagnostic_info.B.SG3 == 1 || Diagnostic_info.B.SG2 == 1 || Diagnostic_info.B.SG1 == 1 || Diagnostic_info.B.SG0 == 1)
	{
		return 2;
	}

	if(Diagnostic_info.B.OL_ON3 == 1 || Diagnostic_info.B.OL_ON2 == 1 || Diagnostic_info.B.OL_ON1 == 1 || Diagnostic_info.B.OL_ON0 == 1
		|| Diagnostic_info.B.OL_OFF3 == 1 || Diagnostic_info.B.OL_OFF2 == 1 || Diagnostic_info.B.OL_OFF1 == 1 || Diagnostic_info.B.OL_OFF0 == 1)
	{
		return 3;
	}

	if(current_value >= 3000)
	{
		flag_5ms_passed++;
		if(flag_5ms_passed >= 6)
		{
			flag_5ms_passed = 0;
			return 4;
		}
	}
	else
	{
		flag_5ms_passed = 0;
	}

	if(autozero.B.OVL == 1)//Over Voltage
	{
		return 5;
	}

	return 0;
}

void bsp_Diag_PO_Detect_Protect(void)//每5ms周期调用用于保护端口
{
	uint32_t result = 0;
	int channel_num = 0;

	for(channel_num = 0; channel_num < 12; channel_num++)
	{
		result = bsp_Diag_Reset_Fault_PO(channel_num);
		if(result == 0)
		{
			return;
		}
		else if(result == 1)//shutdown current channel
		{
			API_ConstantCurrent_Drive(channel_num, 0, 0, 0, 0);
			printf("Short to Battery Fault\r\n");
		}
		else if(result == 2)//Short to Ground Fault
		{
			printf("Short to Ground Fault\r\n");
			API_ConstantCurrent_Drive(1, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(2, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(3, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(4, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(5, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(6, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(7, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(8, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(9, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(10, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(11, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(12, 0, 0, 0, 0);
			//BATP_EN引脚拉低
		}
		else if(result == 3)
		{
			API_ConstantCurrent_Drive(channel_num, 0, 0, 0, 0);
			printf("Open Load fault\r\n");
		}
		else if(result == 4)
		{
			API_ConstantCurrent_Drive(channel_num, 0, 0, 0, 0);
			printf("Over Current fault\r\n");
		}
		else if(result == 5)
		{
			printf("Over Voltage\r\n");		
			API_ConstantCurrent_Drive(1, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(2, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(3, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(4, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(5, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(6, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(7, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(8, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(9, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(10, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(11, 0, 0, 0, 0);
			API_ConstantCurrent_Drive(12, 0, 0, 0, 0);		
		}
	}
}


