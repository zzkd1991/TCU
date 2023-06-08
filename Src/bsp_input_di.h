#ifndef BSP_INPUT_DI_H
#define BSP_INPUT_DI_H

#include "stm32f4xx.h"

enum {
	DIN1 = 0,
	DIN2,
	DIN3,
	DIN4,
	DIN5,
	DIN6,
	DIN7,
	DIN8,
	DIN9,
	DIN10,
	DIN11,
	DIN12,
	DIN13,
	DIN14,
	DIN15,
	DIN16,
	DIN17,
	DIN18,
	DIN19,
	DIN20,
	DIN21,
	DIN_MAX
};

void Bsp_Di_Init(void);
void Reset_DI_Chara(uint8_t channel_u8, uint8_t signalType_u8);
void DI_Screen(void);
uint8_t Api_Di_Get(uint8_t channel_u8);
uint8_t API_DI_Cfg(uint8_t channel_u8, uint8_t signalType_u8);

#endif

