#include "spi.h"
#ifndef _SPI_FLASH_H
#define _SPI_FLASH_H

#define SPI_FLASH_PAGE_SIZE (0x1000U)
#define SPI_FLASH_SIZE (0x00200000U)
#define SPI_FLASH_PAGE_NUM (512)

#define SPI_FLASH_MFG_ID 0xbf41
#define SPI_FLASH_PAGE_ERASE_WAIT (25) 
#define SPI_FLASH_CHIP_ERASE_WAIT (100)

void spi_flash_init(void);
void spi_flash_read(uint32_t addr, uint8_t *buf, uint16_t len);
uint8_t spi_flash_write(uint32_t addr, uint8_t *buf, uint16_t len);
uint8_t spi_flash_erase_page(uint32_t addr, uint16_t num, uint32_t wait);
uint8_t spi_flash_erase_chip(uint32_t wait);
uint8_t spi_flash_ready(void);
uint8_t spi_flash_wait(uint32_t ms);
uint8_t spi_flash_rdsr(void);
uint16_t spi_flash_rdid(void);
void spi_flash_wren(void);
void spi_flash_wrdi(void);
#endif
