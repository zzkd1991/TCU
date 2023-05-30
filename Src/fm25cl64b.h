#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <stdint.h>

#define FM25CL_CS_PORT	GPIOB
#define FM25CL_CS_PIN	GPIO_PIN_4
#define FM25CL_CS_ENA()		HAL_GPIO_WritePin(FM25CL_CS_PORT, FM25CL_CS_PIN, GPIO_PIN_RESET)
#define FM25CL_CS_DIS()		HAL_GPIO_WritePin(FM25CL_CS_PORT, FM25CL_CS_PIN, GPIO_PIN_SET)

#define FM25CL_HOLD_PORT	GPIOB
#define FM25CL_HOLD_PIN	GPIO_PIN_5
#define FM25CL_HOLD_ENA()		HAL_GPIO_WritePin(FM25CL_HOLD_PORT, FM25CL_HOLD_PIN, GPIO_PIN_RESET)
#define FM25CL_HOLD_DIS()		HAL_GPIO_WritePin(FM25CL_HOLD_PORT, FM25CL_HOLD_PIN, GPIO_PIN_SET)

#define FM25CL_WP_PORT		GPIOB
#define FM25CL_WP_PIN			GPIO_PIN_6
#define FM25CL_WP_ENA()			HAL_GPIO_WritePin(FM25CL_WP_PORT, FM25CL_WP_PIN, GPIO_PIN_RESET)
#define FM25CL_WP_DIS()			HAL_GPIO_WritePin(FM25CL_WP_PORT, FM25CL_WP_PIN, GPIO_PIN_SET)

#define DUMMY		0xFF
#define DRIVER_MAX_ADDR	8192

//FM25CL64B
#define WREN		0x06
#define WRDI		0x04
#define RDSR		0x05
#define WRSR		0x01
#define READD		0x03
#define WRITEE		0x02

#define FM25CL64B_CS_0						
#define FM25CL64B_CS_1

int8_t SPIFRAM_Read_Bytes(uint32_t addr, uint8_t *data, uint32_t len);
int8_t SPIFRAM_Write_Bytes(uint32_t addr, uint8_t *data, uint32_t len);

int8_t FM25CL64B_READ(uint32_t RAddr, uint8_t *pBuf, uint32_t num);
int8_t FM25CL64B_WRITE(uint32_t WAddr, uint8_t *pBuf, uint32_t num);
void FM25CL_GPIO_Init(void);

#endif

