#include <string.h>
#include "tle7242.h"
#include "spi.h"
#include "stm32f4xx_hal_spi.h"
#include "stm32f4xx.h"
/* Fclk范围为20-40  本历程中取30Mhz */
/* fpwm频率范围为50-4000Hz*/

extern SPI_HandleTypeDef hspi1;

extern uint8_t spi_trx(uint16_t len, uint8_t *w, uint8_t *r);

#define Rsense  10


#define TLE_INSTRUCTION_WRITE	0x80
#define TLE_INSTRUCTION_READ	0x00

uint16_t N_MACRO = 0;

void TLE7242_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

	TLE7242_ENABLE_GPIO_CLK_ENABLE();
	TLE7242_RESET_GPIO_CLK_ENABLE();
	TLE7242_FAULT_GPIO_CLK_ENABLE();
	
	GPIO_InitStruct.Pin = TLE7242_ENABLE_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(TLE7242_ENABLE_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = TLE7242_RESET_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(TLE7242_RESET_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = TLE7242_FAULT_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(TLE7242_FAULT_INT_GPIO_PORT, &GPIO_InitStruct);
	HAL_NVIC_SetPriority(TLE7242_FAULT_EXTI_IRQ, 0, 0);
	HAL_NVIC_EnableIRQ(TLE7242_FAULT_EXTI_IRQ);
	
}


void TLE_Write_Register_Data(uint8_t channel_u8,uint8_t mess_id_u8,uint32_t SPI_Data_u32)
{
	uint8_t spi_tx_buf[4];

	spi_tx_buf[3] = TLE_INSTRUCTION_WRITE | (mess_id_u8 << 2) | channel_u8;
	memcpy(spi_tx_buf, &SPI_Data_u32, 3);

	spi_trx(4, spi_tx_buf, NULL);
	
	return;
}
uint32_t TLE_Read_Register_Data(uint8_t channel_u8,uint8_t mess_id_u8)
{
	uint32_t result = 0;
	uint8_t spi_tx_buf[4];

	spi_tx_buf[3] = TLE_INSTRUCTION_READ | (mess_id_u8 << 2) | channel_u8;

	spi_trx(4, spi_tx_buf, NULL);

	spi_trx(4, NULL, (uint8_t *)&result);

	return result;
}
void TLE_Chip_Output_Enable(void)
{
	TLE7242_ENABLE_DIS();
	TLE7242_RESET_DIS();
	return;
}
void TLE_Chip_Output_Disable(void)
{
	TLE7242_ENABLE_ENA();
	TLE7242_RESET_ENA();	
	return;
}
void TLE_Power_On_Init(void)
{
	TLE7242_ENABLE_DIS();
	TLE7242_RESET_DIS();

	return;
}


uint8_t TLE_Channel_Pwm_Freq_Set(uint8_t channel_u8,uint16_t freq_u16)
{
	uint16_t N;
	
	N = (uint16_t )((30 * 1000000) / (32 * freq_u16)); 
	
	N_MACRO = N;
	
	TLE_Write_Register_Data(channel_u8, TLE_MSG1_ID, (uint32_t)N);
	
	return HAL_OK;
}
uint8_t TLE_Channel_Time_Offset_Set(uint8_t channel_u8,uint16_t offset_u16)
{
	TLE_Write_Register_Data(channel_u8, TLE_MSG2_ID, (uint32_t)offset_u16);
	
	return HAL_OK;
}
uint8_t TLE_Channel_Constant_Current_Set(uint8_t channel_u8,uint16_t current_u16)
{
	TLE_Write_Register_Data(channel_u8, TLE_MSG3_ID, (uint32_t)current_u16);
	
	return HAL_OK;
}
uint8_t TLE_Channel_Dither_Enable(uint8_t channel_u8,uint8_t dither_en,uint16_t dither_step)
{
	uint32_t temp;
	
	temp |= dither_en << 11;
	temp |= dither_step << 12;
	
	TLE_Write_Register_Data(channel_u8, TLE_MSG3_ID, temp);
	
	return HAL_OK;
}
uint8_t TLE_Channel_OnOff_Operate(uint8_t channel_u8,uint8_t on_off_u8)
{
	
	uint32_t temp;
	
	temp |= on_off_u8 << 21;
	
	TLE_Write_Register_Data(channel_u8, TLE_MSG3_ID, temp);
	
	return HAL_OK;
}
uint8_t TLE_Channel_Dither_Freq_Set(uint8_t channel_u8,uint16_t dither_freq_u16)
{
	uint8_t Dither_Steps = 0;
	
	Dither_Steps = (uint8_t)((30 * 1000000) / (dither_freq_u16 * 4));
	
	TLE_Write_Register_Data(channel_u8, TLE_MSG4_ID, Dither_Steps);
	
	return HAL_OK;
}
uint8_t TLE_Channel_KP_KI_Set(uint8_t channel_u8,uint16_t kp_u16,uint16_t ki_u16)
{
	
	tag_KP_KI message5 = {0};
	uint32_t temp = 0;
	
	temp |= (kp_u16 & 0xfff) << 11;
	temp |= ki_u16 & 0xfff;
	
	TLE_Write_Register_Data(channel_u8, TLE_MSG5_ID, temp);
	
	return HAL_OK;
}
uint8_t TLE_Channel_Dynamic_Threshold_Set(uint8_t channel_u8,uint16_t integrator_preload,uint16_t transient_threshold)
{
	uint32_t temp = 0;
	
	temp |= (integrator_preload & 0xfff);
	temp |= (transient_threshold & 0x7ff) << 12;
	
	TLE_Write_Register_Data(channel_u8, TLE_MSG6_ID, temp);
	
	return HAL_OK;
}
uint8_t TLE_Channel_Mode_Config(uint8_t channel_u8,uint8_t mode_u8)
{
	uint32_t temp = 0;
	
	
	return 0;	
}

void TLE_AutoZero_Enable(uint8_t channel_u8)
{
	uint32_t temp = 0;
	temp |= 1 << 10;

	TLE_Write_Register_Data(channel_u8, TLE_MSG7_ID, temp);
}



uint8_t TLE_Channel_Diagnostic_Read(uint8_t channel_u8)
{
	uint8_t fault_info = 0;
	uint32_t fault_info_temp = 0;
	
	fault_info_temp = TLE_Read_Register_Data(channel_u8, TLE_MSG7_ID);
	
	fault_info = (uint8_t)(fault_info_temp >>11) & 0xff;
	
	return fault_info;	
}


uint16_t TLE_Channel_Current_Read(uint8_t channel_u8)
{
	uint16_t current_read;
	uint16_t current_value;
	
	current_read = TLE_Read_Register_Data(channel_u8, TLE_MSG10_ID);
	
	current_value = (uint16_t)((current_read * (1 << 14)) * (230 / Rsense));
	
	return current_value;
}
uint32_t TLE_Channel_Autozero_Read(uint8_t channel_u8)
{
	uint32_t autozero_read;
	
	autozero_read = TLE_Read_Register_Data(channel_u8, TLE_MSG11_ID);
	
	return autozero_read;
}
uint16_t TLE_Channel_Duty_Read(uint8_t channel_u8)
{
	uint32_t return_value;
	uint32_t duty;
	
	return_value = TLE_Read_Register_Data(channel_u8, TLE_MSG12_ID);
	
	duty = (return_value & 0x7ffff); 
	
	return duty;
}
void TLE_ALL_Register_Period_Read(uint8_t channel_u8,uint16_t period_u16)
{
	return;
}

uint32_t TLE7242_Error_State_Read(void)
{
	uint32_t error_state = 0;

	error_state = TLE_Read_Register_Data(0, TLE_MSG9_ID);

	return error_state;
}


void API_ConstantCurrent_Drive(uint8_t chan_u8, uint16_t current_u16, uint16_t freq_u16,
								float kp_f, float ki_f)
{
	uint16_t kp;
	uint16_t ki;
	uint16_t conved_current;

	kp = (uint16_t)(kp_f * 4095);
	ki = (uint16_t)(ki_f * 4095);

	conved_current = (current_u16 * (1 << 11) * Rsense) / 320;
	
	
	TLE_Channel_Constant_Current_Set(chan_u8, conved_current);
	TLE_Channel_KP_KI_Set(chan_u8, kp, ki);
	
	TLE_Channel_Pwm_Freq_Set(chan_u8, freq_u16);
}


void API_Dither_Par_Config(uint8_t chan_u8, uint8_t dither_enable, uint16_t dither_freq, uint8_t dither_amp)
{
	uint16_t conved_dither_amp;
	uint8_t dither_steps;

	dither_steps = (uint8_t)((30 * 1000000) / (dither_freq * 4));

	conved_dither_amp = ((1 << 15) * Rsense) / (2 * 320  * dither_steps);

	TLE_Channel_Dither_Enable(chan_u8, dither_enable, conved_dither_amp);
	
	TLE_Channel_Dither_Freq_Set(chan_u8, dither_freq);
}

uint16_t API_DynamicCurrent_Read(uint8_t chan_u8)
{
	return TLE_Channel_Current_Read(chan_u8);
}

void API_Power_Switch_Set(uint8_t chan_u8, uint8_t on_off_u8)
{
	
}

uint16_t API_Duty_Feedback_Read(uint8_t chan_u8)
{
	uint32_t duty_cycle = 0;
	uint8_t actual_duty = 0;
	
	duty_cycle = TLE_Channel_Duty_Read(chan_u8);

	actual_duty = 100 * (duty_cycle / (32 * N_MACRO));

	return actual_duty;
	
}

uint32_t bsp_Diag_Reset_Fault_PO(uint8_t cahn_u8)
{
	
	return 0;
}

