#ifndef BSP_OUTPUT_AO_H
#define BSP_OUTPUT_AO_H

#include "main.h"


enum {
	AOUT1 = 0,

	AOUT_MAX
};


enum {
	PIOUT1 = 0,

	PIOUT_MAX
};

void bsp_DAC_Init(void);
void AO_outVoltage(uint16_t Voltage);

uint16_t API_GET_AO_Voltage(void);
uint8_t AO_Diag_State_Get(void);

#endif



