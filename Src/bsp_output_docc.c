#include "bsp_output_docc.h"

void API_ConstantCurrent_Drive(uint8_t chan_u8, uint16_t current_u16, uint16_t freq_u16,
								float kp_f, float ki_f)
{
	uint16_t kp;
	uint16_t ki;
	uint16_t conved_current;
	uint8_t conved_chan = 0;

	kp = (uint16_t)(kp_f * 4095);
	ki = (uint16_t)(ki_f * 4095);

	conved_current = (current_u16 * (1 << 11) * Rsense) / 320;

	
	if(chan_u8 == PO1 || chan_u8 == PO2 || chan_u8 == PO3 || chan_u8 == PO4)
	{
		TLE7242_CS1_LOW();
		conved_chan = chan_u8 - 1;
		TLE_Channel_Mode_Config(conved_chan, 0);//恒流模式
		TLE_Channel_Constant_Current_Set(conved_chan, conved_current);
		TLE_Channel_KP_KI_Set(conved_chan, kp, ki);	
		TLE_Channel_Pwm_Freq_Set(conved_chan, freq_u16);
		TLE7242_CS1_HIGH();
	}
	else if(chan_u8 == PO5 || chan_u8 == PO6 || chan_u8 == PO7 || chan_u8 == PO8)
	{
		TLE7242_CS2_LOW();
		conved_chan = chan_u8 - 5;
		TLE_Channel_Mode_Config(conved_chan, 0);//恒流模式
		TLE_Channel_Constant_Current_Set(conved_chan, conved_current);
		TLE_Channel_KP_KI_Set(conved_chan, kp, ki);	
		TLE_Channel_Pwm_Freq_Set(conved_chan, freq_u16);
		TLE7242_CS2_HIGH();		
	}
	else if(chan_u8 == PO9 || chan_u8 == PO10 || chan_u8 == PO11 || chan_u8 == PO12)
	{
		TLE7242_CS3_LOW();
		conved_chan = chan_u8 - 9;
		TLE_Channel_Mode_Config(conved_chan, 0);//恒流模式
		TLE_Channel_Constant_Current_Set(conved_chan, conved_current);
		TLE_Channel_KP_KI_Set(conved_chan, kp, ki);	
		TLE_Channel_Pwm_Freq_Set(conved_chan, freq_u16);
		TLE7242_CS3_HIGH();		
	}

}


void API_PO_Mode_Config(uint8_t chan_u8, uint16_t mode_u8)
{
	uint8_t conved_chan = 0;

	if(chan_u8 == PO1 || chan_u8 == PO2 || chan_u8 == PO3 || chan_u8 == PO4)
	{
		TLE7242_CS1_LOW();
		conved_chan = chan_u8 -1;
		TLE_Channel_Mode_Config(conved_chan, mode_u8);
		TLE7242_CS1_HIGH();
	}
	else if(chan_u8 == PO5 || chan_u8 == PO6 || chan_u8 == PO7 || chan_u8 == PO8)
	{
		TLE7242_CS2_LOW();
		conved_chan = chan_u8 - 5;
		TLE_Channel_Mode_Config(conved_chan, mode_u8);
		TLE7242_CS2_HIGH();
	}
	else if(chan_u8 == PO9 || chan_u8 == PO10 || chan_u8 == PO11 || chan_u8 == PO12)
	{
		TLE7242_CS3_LOW();
		conved_chan = chan_u8 - 9;
		TLE_Channel_Mode_Config(conved_chan, mode_u8);
		TLE7242_CS3_HIGH();
	}

}



void API_Dither_Par_Config(uint8_t chan_u8, uint8_t dither_enable, uint16_t dither_freq, uint8_t dither_amp)
{
	uint16_t conved_dither_amp;
	uint8_t dither_steps;
	uint8_t conved_chan = 0;

	dither_steps = (uint8_t)((30 * 1000000) / (dither_freq * 4));
	conved_dither_amp = ((1 << 15) * Rsense * dither_amp) / (2 * 320  * dither_steps);

	if(chan_u8 == PO1 || chan_u8 == PO2 || chan_u8 == PO3 || chan_u8 == PO4)
	{
		conved_chan = chan_u8 - 1;

		TLE7242_CS1_LOW();
		TLE_Channel_Dither_Enable(conved_chan, dither_enable, conved_dither_amp);
		TLE_Channel_Dither_Freq_Set(conved_chan, dither_freq);
		TLE7242_CS1_HIGH();
	}
	else if(chan_u8 == PO5 || chan_u8 == PO6 || chan_u8 == PO7 || chan_u8 == PO8)
	{
		conved_chan = chan_u8 - 5;

		TLE7242_CS2_LOW();
		TLE_Channel_Dither_Enable(conved_chan, dither_enable, conved_dither_amp);
		TLE_Channel_Dither_Freq_Set(conved_chan, dither_freq);
		TLE7242_CS2_HIGH();
	}
	else if(chan_u8 == PO9 || chan_u8 == PO10 || chan_u8 == PO11 || chan_u8 == PO12)
	{
		conved_chan = chan_u8 - 9;

		TLE7242_CS3_LOW();
		TLE_Channel_Dither_Enable(conved_chan, dither_enable, conved_dither_amp);
		TLE_Channel_Dither_Freq_Set(conved_chan, dither_freq);
		TLE7242_CS3_HIGH();
	}
}

uint16_t API_DynamicCurrent_Read(uint8_t chan_u8)
{
	uint8_t conved_chan;
	uint16_t current_value = 0;

	
	if(chan_u8 == PO1 || chan_u8 == PO2 || chan_u8 == PO3 || chan_u8 == PO4)
	{
		conved_chan = chan_u8 - 1;

		TLE7242_CS1_LOW();
		current_value = TLE_Channel_Current_Read(conved_chan);
		TLE7242_CS1_HIGH();
	}
	else if(chan_u8 == PO5 || chan_u8 == PO6 || chan_u8 == PO7 || chan_u8 == PO8)
	{
		conved_chan = chan_u8 - 5;

		TLE7242_CS2_LOW();
		current_value = TLE_Channel_Current_Read(conved_chan);
		TLE7242_CS2_HIGH();
	}
	else if(chan_u8 == PO9 || chan_u8 == PO10 || chan_u8 == PO11 || chan_u8 == PO12)
	{
		conved_chan = chan_u8 - 9;

		TLE7242_CS3_LOW();
		current_value = TLE_Channel_Current_Read(conved_chan);
		TLE7242_CS3_HIGH();
	}

	return current_value;
}

void API_Power_Switch_Set(uint8_t chan_u8, uint8_t on_off_u8)
{
	uint8_t conved_chan;

	if(chan_u8 == PO1 || chan_u8 == PO2 || chan_u8 == PO3 || chan_u8 == PO4)
	{
		conved_chan = chan_u8 - 1;

		TLE7242_CS1_LOW();
		TLE_Channel_Mode_Config(conved_chan, 1);//首先设置成On/Off模式
		TLE_Channel_OnOff_Operate(conved_chan, on_off_u8);		
		TLE7242_CS1_HIGH();
	}
	else if(chan_u8 == PO5 || chan_u8 == PO6 || chan_u8 == PO7 || chan_u8 == PO8)
	{
		conved_chan = chan_u8 - 5;

		TLE7242_CS2_LOW();
		TLE_Channel_Mode_Config(conved_chan, 1);
		TLE_Channel_OnOff_Operate(conved_chan, on_off_u8);
		TLE7242_CS2_HIGH();
	}
	else if(chan_u8 == PO9 || chan_u8 == PO10 || chan_u8 == PO11 || chan_u8 == PO12)
	{
		conved_chan = chan_u8 - 9;

		TLE7242_CS3_LOW();
		TLE_Channel_Mode_Config(conved_chan, 1);
		TLE_Channel_OnOff_Operate(conved_chan, on_off_u8);
		TLE7242_CS3_HIGH();		
	}
}

uint16_t API_Duty_Feedback_Read(uint8_t chan_u8)
{
	uint32_t duty_cycle = 0;
	uint8_t actual_duty = 0;
	uint8_t conved_chan = 0;
	extern uint16_t N_MACRO;

	if(chan_u8 == PO1 || chan_u8 == PO2 || chan_u8 == PO3 || chan_u8 == PO4)
	{
		conved_chan = chan_u8 - 1;
		TLE7242_CS1_LOW();
		duty_cycle = TLE_Channel_Duty_Read(conved_chan);	
		actual_duty = 100 * (duty_cycle / (32 * N_MACRO));
		TLE7242_CS1_HIGH();
	}
	else if(chan_u8 == PO5 || chan_u8 == PO6 || chan_u8 == PO7 || chan_u8 == PO8)
	{
		conved_chan = chan_u8 - 5;
		TLE7242_CS2_LOW();
		duty_cycle = TLE_Channel_Duty_Read(conved_chan);	
		actual_duty = 100 * (duty_cycle / (32 * N_MACRO));
		TLE7242_CS2_HIGH();
	}
	else if(chan_u8 == PO9 || chan_u8 == PO10 || chan_u8 == PO11 || chan_u8 == PO12)
	{
		conved_chan = chan_u8 - 9;
		TLE7242_CS3_LOW();
		duty_cycle = TLE_Channel_Duty_Read(conved_chan);	
		actual_duty = 100 * (duty_cycle / (32 * N_MACRO));
		TLE7242_CS3_HIGH();
	}

	return actual_duty;
	
}

uint32_t bsp_Diag_Reset_Fault_PO(uint8_t chan_u8)
{
	uint32_t Diagnostic_info = 0;
	uint8_t conved_chan;

	if(chan_u8 == PO1 || chan_u8 == PO2 || chan_u8 == PO3 || chan_u8 == PO4)
	{
		conved_chan = chan_u8 - 1;
		TLE7242_CS1_LOW();
		Diagnostic_info = TLE_Channel_Diagnostic_Read(conved_chan);
		TLE7242_CS1_HIGH();
	}
	else if(chan_u8 == PO5 || chan_u8 == PO6 || chan_u8 == PO7 || chan_u8 == PO8)
	{
		conved_chan = chan_u8 - 5;
		TLE7242_CS2_LOW();
		Diagnostic_info = TLE_Channel_Diagnostic_Read(conved_chan);
		TLE7242_CS2_HIGH();
	}
	else if(chan_u8 == PO9 || chan_u8 == PO10 || chan_u8 == PO11 || chan_u8 == PO12)
	{
		conved_chan = chan_u8 - 9;
		TLE7242_CS3_LOW();
		Diagnostic_info = TLE_Channel_Diagnostic_Read(conved_chan);
		TLE7242_CS3_HIGH();
	}

	
	if(Diagnostic_info & (1 << 5) || Diagnostic_info & (1 << 11) || Diagnostic_info & (1 << 17) || Diagnostic_info & (1 << 23))//Short to Battery Fault
	{
		return 1;
	}

	if(Diagnostic_info & (1 << 7) || Diagnostic_info & (1 << 13) || Diagnostic_info & (1 << 19) || Diagnostic_info & (1 << 25))//short to ground
	{
		return 2;
	}

	if(Diagnostic_info & (1 << 2) || Diagnostic_info & (1 << 8) || Diagnostic_info & (1 << 14) || Diagnostic_info & (1 << 20)
		|| Diagnostic_info & (1 << 3) || Diagnostic_info & (1 << 9) || Diagnostic_info & (1 << 15) || Diagnostic_info & (1 << 21))
	{
		return 3;
	}

	return 0;
}

void bsp_Diag_PO_Detect_Protect(void)
{
	uint32_t result = 0;
	
	result = bsp_Diag_Reset_Fault_PO(PO1);
	if(result == 0)
	{
		return;
	}
	else if(result == 1)
	{
		
	}
	else if(result == 2)
	{
		
	}
}


