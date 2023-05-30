#include "bsp_input_ai.h"
#include "temp_pres_drv.h"

#define ADC_PORT_NUM_TOTAL 	19

uint16_t REAL_BUF[ADC_PORT_NUM_TOTAL];
extern uint16_t ADC_AVG[9];
extern uint16_t _ADC_AVG[10];

void ADC_Smooth(void)
{
	int i;
	
	for(i = 0; i < 9; i++)
	{
		REAL_BUF[i] = (uint16_t)(3.3f * (ADC_AVG[i] / 4096.0f) * 1.51f * 1000);
	}

	for(i = 0; i < 10; i++)
	{
		REAL_BUF[i + 9] = (uint16_t)(3.3f * (_ADC_AVG[i] / 4096.0f) * 1.51f * 1000);
	}
}
