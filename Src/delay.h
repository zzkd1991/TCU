#ifndef DELAY_H
#define DELAY_H

#include "stm32f4xx.h"

void PY_usDelayTest(void);
void PY_Delay_us_t(uint32_t Delay);
void PY_usDelayOptimize(void);
void delay_us(uint32_t delay);
void delay_ms(uint16_t ms);


#endif

