#ifndef BSP_OUTPUT_DOCC_H
#define BSP_OUTPUT_DOCC_H

#include "main.h"
#include "tle7242.h"

typedef enum
{
	PO1 = 0,
	PO2,
	PO3,
	PO4,
	PO5,
	PO6,
	PO7,
	PO8,
	PO9,
	PO10,
	PO11,	
	PO12,	
	
	PO_MAX,
}ENUM_PWM_OUT;

void bsp_Docc_PowerOn_Init(void);
void API_PO_Mode_Config(uint8_t chan_u8,uint16_t mode_u8);
void API_ConstantCurrent_Drive(uint8_t chan_u8,uint16_t current_u16,uint16_t freq_u16,float kp_f,float ki_f);
uint16_t API_DynamicCurrent_Read(uint8_t chan_u8);
void API_Dither_Par_Config(uint8_t chan_u8,uint8_t dither_enable,uint16_t dither_freq,uint8_t dither_amp);
void API_Power_Switch_Set(uint8_t chan_u8,uint8_t on_off_u8);
uint8_t API_Diagnostics_Fault_Read(uint8_t chan_u8);
uint16_t API_Duty_Feedback_Read(uint8_t chan_u8);
void API_Manufacturer_Info_Get(uint8_t chan_u8);




#endif


