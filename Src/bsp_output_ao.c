#include "bsp_output_ao.h"
#include "dac.h"
#include <math.h>

extern uint16_t REAL_BUF[19];

uint16_t Dac_Set_Volt = 0;

void bsp_DAC_Init(void)
{
	dac_init(1);
}

void AO_outVoltage(uint16_t Voltage)
{
	Dac_Set_Volt = Voltage;
	dac_set_voltage(1, Voltage);
}


uint16_t API_GET_AO_Voltage(void)
{
	return REAL_BUF[9];
}

uint8_t AO_Diag_State_Get(void)
{
	if(abs(REAL_BUF[9] - Dac_Set_Volt) < Dac_Set_Volt * 0.05)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


