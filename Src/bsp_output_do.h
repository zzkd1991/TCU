#ifndef BSP_OUTPUT_DO_H
#define BSP_OUTPUT_DO_H

#include "stm32f4xx.h"

enum {
	DOUT1 = 0,
	DOUT2,
	DOUT3,
	DOUT4,
	DOUT5,
	DOUT6,

	DOUT_MAX
};


uint8_t API_OUT_Do_Cfg(uint8_t channel_u8, uint8_t OutMode_u8);

uint8_t API_OUT_Do_Set(uint8_t channel_u8, uint8_t value);

#endif



