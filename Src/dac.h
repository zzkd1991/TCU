#ifndef __DAC_H
#define __DAC_H

void dac_init(uint8_t outx);	//DAC通道1初始化
void dac_set_voltage(uint8_t outx, uint16_t vol);//设置通道1/2输出电压

#endif


