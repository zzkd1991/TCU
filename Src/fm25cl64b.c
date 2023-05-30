#include "fm25cl64b.h"
#include "spi.h"
#include "stm32f4xx_hal_spi.h"

/*
Address范围0-8191（1FFF）
*/

extern SPI_HandleTypeDef hspi1;

void FM25CL_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

	__HAL_RCC_GPIOB_CLK_ENABLE();
	
	HAL_GPIO_WritePin(FM25CL_CS_PORT, FM25CL_CS_PIN, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(FM25CL_HOLD_PORT, FM25CL_HOLD_PIN, GPIO_PIN_SET);
	
	HAL_GPIO_WritePin(FM25CL_WP_PORT, FM25CL_WP_PIN, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = FM25CL_CS_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(FM25CL_CS_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = FM25CL_HOLD_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(FM25CL_HOLD_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = FM25CL_WP_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(FM25CL_WP_PORT, &GPIO_InitStruct);

}

static void delay_80us(void)
{
	int i;
	for(i = 0; i < 1000; i++)
	{

	}
	return;
}

uint8_t SPI_ReadWriteByte(uint8_t TxData)
{
	uint8_t Rxdata;
	
	spi2_trx(1, &TxData, NULL);

	spi2_trx(1, NULL, &Rxdata);
	
	return Rxdata;
}

void FramWriteByte(uint16_t address, uint8_t da)
{
	uint8_t temM, temL;
	
	temM = (uint8_t)((address&0xff00)>>8);
	temL = (uint8_t)((address&0x00ff));
	
	FM25CL64B_CS_0;
	SPI_ReadWriteByte(WREN);
	FM25CL64B_CS_1;
	
	delay_80us();
	
	FM25CL64B_CS_0;
	SPI_ReadWriteByte(WRITEE);
	SPI_ReadWriteByte(temM);
	SPI_ReadWriteByte(temL);
	SPI_ReadWriteByte(da);
	FM25CL64B_CS_1;
	delay_80us();
	
	FM25CL64B_CS_0;
	SPI_ReadWriteByte(WRDI);
	FM25CL64B_CS_1;
}

uint8_t FramReadByte(uint16_t address)
{
	uint8_t temp, temM, temL;
	
	temM = (uint8_t)((address&0xff00) >> 8);
	temL = (uint8_t)((address&0x00ff));
	
	FM25CL64B_CS_0;
	SPI_ReadWriteByte(READD);
	SPI_ReadWriteByte(temM);
	SPI_ReadWriteByte(temL);
	temp = SPI_ReadWriteByte(0xff);
	FM25CL64B_CS_1;
	
	return temp;
}


int8_t SPIFRAM_Read_Bytes(uint32_t addr, uint8_t *data, uint32_t len)
{
	uint32_t i = 0;
	for(i = 0; i < len; i++)
	{
		*(data + i) = FramReadByte(addr + i);
	}
	
	return 0;
}

int8_t SPIFRAM_Write_Bytes(uint32_t addr, uint8_t *data, uint32_t len)
{
	uint32_t i = 0;
	for(i = 0; i < len; i++)
	{
		FramWriteByte(addr + i, *(data + i));
	}

	return 0;
}




