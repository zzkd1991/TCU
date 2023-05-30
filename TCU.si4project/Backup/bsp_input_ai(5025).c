#include "bsp_input_ai.h"
#include "temp_pres_drv.h"

#define ADC_PORT_NUM_TOTAL 	9

uint16_t REAL_BUF[ADC_PORT_NUM_TOTAL];


void ADC_Smooth(void)
{
	int i;

	for(i = 0; i < 6; i++)
	{
		REAL_BUF[i] = (uint16_t)(3.3f * ((float)ADC_AVG[i] / 4096.0f) * 1.51 * 1000);
	}

	

}



