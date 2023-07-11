#include "stm32f4xx.h"
#include "delay.h"

/***************************************************************/
/*挂在是SPI总线上的外设的个数和延迟时间,移植修改第一处*/
/***************************************************************/
#define SPI_NUM_ALL 3
#define SPI_DELAY_TIME 10
/***************************************************************/
 
/*SPI的SCK引脚电平设置，移植修改第二处*/
/***************************************************************/
#define SPI_SCK_SET_1       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET)
#define SPI_SCK_SET_0       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET)
/***************************************************************/
/**SPI2 GPIO Configuration
PC2 	------> SPI2_MISO
PB13	 ------> SPI2_SCK
PB15	 ------> SPI2_MOSI
*/
/*SPI的MOSI引脚电平设置,输出引脚，移植修改第三处*/
/***************************************************************/
#define SPI_MOSI_SET_1       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET)
#define SPI_MOSI_SET_0       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET)
/***************************************************************/
 
/*SPI的MISO引脚电平设置，输入引脚，移植修改第四处*/
/***************************************************************/
#define SPI_MISO_READ  HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)
/***************************************************************/
 
/*SPI的CS引脚电平设置,移植修改第五处*/
/***************************************************************/
#define SPI_CS1_SET_1       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)
#define SPI_CS1_SET_0       HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
 
#if SPI_NUM_ALL > 1
#define SPI_CS2_SET_1       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET)
#define SPI_CS2_SET_0       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET)
#endif
 
#if SPI_NUM_ALL > 2
#define SPI_CS3_SET_1       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET)
#define SPI_CS3_SET_0       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET)
#endif
 
#if SPI_NUM_ALL > 3
#define SPI_CS4_SET_1       GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define SPI_CS4_SET_0       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#endif
/***************************************************************/
 
 
/***************************************************************************/
/*SPI引脚初始化,移植修改第六处*/
/***************************************************************************/
void user_spi_init(void)
{
#if 1
		GPIO_InitTypeDef GPIO_InitStructure;
		/**SPI3 GPIO Configuration
		PC10	 ------> SPI3_SCK
		PC11	 ------> SPI3_MISO
		PC12	 ------> SPI3_MOSI
		*/
		//普通gpio模拟spi
		__GPIOC_CLK_ENABLE();
	
		GPIO_InitStructure.Pin = GPIO_PIN_10 | GPIO_PIN_12;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.Pull = GPIO_PULLUP;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);
	
		GPIO_InitStructure.Pin = GPIO_PIN_11;
		GPIO_InitStructure.Mode = GPIO_MODE_INPUT;//GPIO输入模式
		GPIO_InitStructure.Pull = GPIO_PULLDOWN;//管脚下拉
		HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);	
#endif

}
/***************************************************************************/

void user_delay_us(uint32_t delay)
{
	delay_us(delay);
}

 
/***************************************************************/
/*SPI的SCK引脚设置*/
/***************************************************************/
void spi_sck_set(unsigned char IOvalue)
{
  if(IOvalue != 0)
	{SPI_SCK_SET_1;user_delay_us(SPI_DELAY_TIME);}
	else{SPI_SCK_SET_0;user_delay_us(SPI_DELAY_TIME);}
}
/***************************************************************/
 
 
/***************************************************************/
/*SPI的MOSI引脚设置*/
/***************************************************************/
void spi_mosi_set(unsigned char IOvalue)
{
	if(IOvalue != 0){SPI_MOSI_SET_1;user_delay_us(SPI_DELAY_TIME);}
	else{SPI_MOSI_SET_0;user_delay_us(SPI_DELAY_TIME);}
}
/***************************************************************/
 
 
/***************************************************************/
/*SPI的MISO引脚设置*/
/***************************************************************/
unsigned char spi_miso_read(void)
{
  unsigned char bit_value;
  bit_value = SPI_MISO_READ;
	user_delay_us(SPI_DELAY_TIME);
	return bit_value;
}
/***************************************************************/
 
 
/***************************************************************/
/*SPI起始信号*/
/***************************************************************/
void user_spi_start(unsigned char spi_id)
{
  switch(spi_id)
	{
	  case 1: SPI_CS1_SET_0;user_delay_us(SPI_DELAY_TIME);break;
		
	#if SPI_NUM_ALL > 1
	  case 2: SPI_CS2_SET_0;user_delay_us(SPI_DELAY_TIME);break;
    #endif
 
    #if SPI_NUM_ALL > 2
		case 3: SPI_CS3_SET_0;user_delay_us(SPI_DELAY_TIME);break;
    #endif
 
    #if SPI_NUM_ALL > 3
		case 4: SPI_CS4_SET_0;user_delay_us(SPI_DELAY_TIME);break;
    #endif
	}
}
/***************************************************************/
 
 
/***************************************************************/
/*SPI停止信号*/
/***************************************************************/
void user_spi_stop(unsigned char spi_id)
{
  switch(spi_id)
	{
	  case 1: SPI_CS1_SET_1;user_delay_us(SPI_DELAY_TIME);break;
		
	#if SPI_NUM_ALL > 1
	  case 2: SPI_CS2_SET_1;user_delay_us(SPI_DELAY_TIME);break;
    #endif
 
    #if SPI_NUM_ALL > 2
	  case 3: SPI_CS3_SET_1;user_delay_us(SPI_DELAY_TIME);break;
    #endif
 
    #if SPI_NUM_ALL > 3
	  case 4: SPI_CS4_SET_1;user_delay_us(SPI_DELAY_TIME);break;
    #endif
	}
}
/***************************************************************/
 
 
/************************************************************************************************/
/*SPI移位寄存器收发一个字节数据*/
/************************************************************************************************/
unsigned char user_spi_swap_byte(unsigned char spi_mode,unsigned char send_byte)
{
  unsigned char i,receice_byte = 0x00;
	switch(spi_mode)
	{
		case 0:
			for(i = 0;i < 8;i++)
			{
				if((send_byte & (0x80 >> i)) != 0){spi_mosi_set(1);}else{spi_mosi_set(0);}
				spi_sck_set(1);
				if(spi_miso_read() != 0){receice_byte |= (0x80 >> i);}else{receice_byte &= (~(0x80 >> i));}
				spi_sck_set(0);
			};break;
		case 1:
			for(i = 0;i < 8;i++)
			{
				spi_sck_set(1);
				if((send_byte & (0x80 >> i)) != 0){spi_mosi_set(1);}else{spi_mosi_set(0);}
				spi_sck_set(0);
				if(spi_miso_read() != 0){receice_byte |= (0x80 >> i);}else{receice_byte &= (~(0x80 >> i));}
			};break;
		case 2:
			for(i = 0;i < 8;i++)
			{
				if((send_byte & (0x80 >> i)) != 0){spi_mosi_set(1);}else{spi_mosi_set(0);}
				spi_sck_set(0);
				if(spi_miso_read() != 0){receice_byte |= (0x80 >> i);}else{receice_byte &= (~(0x80 >> i));}
				spi_sck_set(1);
			};break;
		case 3:
			for(i = 0;i < 8;i++)
			{
				spi_sck_set(0);
				if((send_byte & (0x80 >> i)) != 0){spi_mosi_set(1);}else{spi_mosi_set(0);}
				spi_sck_set(1);
				if(spi_miso_read() != 0){receice_byte |= (0x80 >> i);}else{receice_byte &= (~(0x80 >> i));}
			};break;;
	}
	return receice_byte; 
}

uint8_t __SPI_Send(uint8_t write_data)
{
	uint8_t tp;
	tp = user_spi_swap_byte(3, write_data);

	return tp;
}

/*uint32_t SPI_Send(uint32_t data, uint8_t channel_id)
{
	int i;
	uint8_t write_data[4] = {0};
	uint8_t read_data[4] = {0};
	uint32_t ret_value = 0;
	write_data[0] = data >> 24;
	write_data[1] = (data >> 16) & 0xff;
	write_data[2] = (data >> 8) & 0xff;
	write_data[3] = data & 0xff;
	
	//for(i = 0; i < 4; i++)
	//{
	//	printf("write_data[i], %d, %x\r\n", i, write_data[i]);
	//}
	
	for(i = 0; i < 4; i++)
	{
		read_data[i] = user_spi_swap_byte(2, write_data[i]);		
	}

	ret_value = read_data[0] << 24 | read_data[1] << 16 | read_data[2] << 8 | read_data[3];
	
	return ret_value;
}*/



/***********************************************************************************************/





