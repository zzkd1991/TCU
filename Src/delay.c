#include "delay.h"

__IO float usDelayBase;

void PY_usDelayTest(void)
{
	__IO uint32_t firstms, secondms;
	__IO uint32_t counter = 0;

	firstms = HAL_GetTick() + 1;
	secondms = firstms + 1;

  	while(uwTick != firstms);
	
	while(uwTick != secondms) counter++;

	usDelayBase = ((float)counter)/1000;

}

void PY_Delay_us_t(uint32_t Delay)
{
	__IO uint32_t delayReg;
	__IO uint32_t usNum = (uint32_t)(Delay * usDelayBase);

	delayReg = 0;
	while(delayReg != usNum) delayReg++;
}

void PY_usDelayOptimize(void)
{
	__IO uint32_t firstms, secondms;
	__IO float coe = 1.0;

	firstms = HAL_GetTick();
	PY_Delay_us_t(1000000);
	secondms = HAL_GetTick();

	coe = ((float)1000) / (secondms - firstms);
	usDelayBase = coe * usDelayBase;
}

void delay_us(uint32_t delay)
{
	PY_Delay_us_t(delay);	
}

void delay_ms(uint16_t ms)
{
	HAL_Delay(ms);
}

