#include "spi_flash.h"

// byte progam wait
#define SPI_FLASH_BPRG_WAIT (1)

// cs 
#define SPI_FLASH_CS_PORT (GPIOA)
#define SPI_FLASH_CS_PIN (GPIO_PIN_4)
#define SPI_FLASH_CS_ENA() HAL_GPIO_WritePin(SPI_FLASH_CS_PORT, SPI_FLASH_CS_PIN, GPIO_PIN_RESET)
#define SPI_FLASH_CS_DIS() HAL_GPIO_WritePin(SPI_FLASH_CS_PORT, SPI_FLASH_CS_PIN, GPIO_PIN_SET)

// OP code
#define SPI_FLASH_OP_RD  (0x03)
#define SPI_FLASH_OP_SE (0x20)
#define SPI_FLASH_OP_BE (0x52)
#define SPI_FLASH_OP_BPRG  (0x02)
#define SPI_FLASH_OP_AAIWP (0xad)
#define SPI_FLASH_OP_CE  (0x60)
#define SPI_FLASH_OP_RDSR  (0x05)
#define SPI_FLASH_OP_EWSR  (0x50)
#define SPI_FLASH_OP_WRSR  (0x01)
#define SPI_FLASH_OP_WREN  (0x06)
#define SPI_FLASH_OP_WRDI  (0x04)
#define SPI_FLASH_OP_RDID  (0x90)

// SR bit
#define SPI_FLASH_SR_BUSY (0x01)
#define SPI_FLASH_RDID_RETRY (64)
//extern void reload_iwdg(void);
void fatal(char *p)
{
	printf("%s\r\n",p);

//  for(;;){
//    NVIC_SystemReset();
//  }
}

// init
void spi_flash_init(void)
{
  uint16_t i = 0;

  SPI_FLASH_CS_DIS();
  // wait until flash readable
  while((spi_flash_rdid()) != SPI_FLASH_MFG_ID){
    // try recover from wrong chip/bus state 
    spi_flash_wren();
    spi_flash_wrdi();
    //reload_iwdg();
    // reset system if retry fail
    if(i++ >= SPI_FLASH_RDID_RETRY){
      fatal("spi flash rdid fail");
    }
  }
}

/**
* read		Read data at the specified address.
*	@arg		addr	The flash address where you want to read data
*	@arg		*buf	Storage buf to read data 
*	@arg		len		Data length
*/
void spi_flash_read(uint32_t addr, uint8_t *buf, uint16_t len)
{
  uint8_t cmd[4];
  
  cmd[0] = SPI_FLASH_OP_RD;
  cmd[1] = addr >> 16;
  cmd[2] = addr >> 8;
  cmd[3] = addr & 0xff;
  
  SPI_FLASH_CS_ENA();
  spi2_trx(4, cmd, 0);
  spi2_trx(len, 0, buf);
  SPI_FLASH_CS_DIS();
}

// byte prog
uint8_t spi_flash_bprg(uint32_t addr, uint8_t data)
{
  uint8_t cmd[5];  
  uint8_t r = 1;
  
  // wr enble
  spi_flash_wren();
  // byte prog
  cmd[0] = SPI_FLASH_OP_BPRG;
  cmd[1] = addr >> 16;
  cmd[2] = addr >> 8;
  cmd[3] = addr & 0xff;
  cmd[4] = data;

  SPI_FLASH_CS_ENA();  
  spi2_trx(5, cmd, 0);
  SPI_FLASH_CS_DIS();
  // wait until ready
	r=spi_flash_wait(SPI_FLASH_BPRG_WAIT);
  if(!(r)){
    goto L_EXIT;
  }
  
L_EXIT:  
  // wr disable
  spi_flash_wrdi();
  
  return r;
}

/**
* write		Before writing data, ensure that there is no data in the current flash address.
*					If there is data in the current flash address, use function spi_flash_erase_page or
*					spi_flash_erase_chip first.
*	@arg		addr	The flash address where you want to write data
*	@arg		*buf	Data to write
*	@arg		len	Data length
*/
uint8_t spi_flash_write(uint32_t addr, uint8_t *buf, uint16_t len)
{
  uint8_t cmd[6];
  uint8_t r = 1;

  if(len <= 0) return r;

  // leading byte
  if(addr & 0x01){
		r = spi_flash_bprg(addr, buf[0]);
    if(!(r)){
      return r;
    }
    addr++;
    buf++;
    len--;
  }
  // tail byte
  if(len & 0x01){
    len--;
		r = spi_flash_bprg(addr+len, buf[len]);
    if(!(r)){
      return r;
    }
  }
  
  // wr enble
  spi_flash_wren();

  // aai program
  cmd[0] = SPI_FLASH_OP_AAIWP;
  cmd[1] = addr >> 16;
  cmd[2] = addr >> 8;
  cmd[3] = addr & 0xff;
  cmd[4] = buf[0];
  cmd[5] = buf[1];
  SPI_FLASH_CS_ENA();  
  spi2_trx(6, cmd, 0);
  SPI_FLASH_CS_DIS();
  // wait until ready
	r=spi_flash_wait(SPI_FLASH_BPRG_WAIT);
  if(!(r)){
    goto L_EXIT;
  }
  
  // progam bytes
  for(uint16_t i = 2; i < len; i+=2){
    cmd[0] = SPI_FLASH_OP_AAIWP;
    cmd[1] = buf[i];
    cmd[2] = buf[i+1];
    SPI_FLASH_CS_ENA();  
    spi2_trx(3, cmd, 0);
    SPI_FLASH_CS_DIS();
    // wait until ready
		r=spi_flash_wait(SPI_FLASH_BPRG_WAIT);
    if(!(r)){
      break;
    }
  }

L_EXIT:  
	// wr disable
	spi_flash_wrdi();
  
  return r;
}

// erase page
/**
* erase page	The data page of the specified address will be erased.
*							page begin address base is 0x1000U;
*							1 page data size is 0x1000U;
*	@arg		addr	The flash address where you want to write data
*	@arg		*buf	Data to write
*	@arg		len	Data length
*/
uint8_t spi_flash_erase_page(uint32_t addr, uint16_t num, uint32_t wait)
{
  uint32_t i, j;
  uint8_t cmd[4];
  uint8_t r = 0;

  // erase page
  for(i = 0, j = addr; i < num; i++, j += SPI_FLASH_PAGE_SIZE){
    // wr enable
    spi_flash_wren();
    // erage page    
    cmd[0] = SPI_FLASH_OP_SE;
    cmd[1] = j >> 16;
    cmd[2] = j >>  8;
    cmd[3] = j;
    SPI_FLASH_CS_ENA();  
    spi2_trx(4, cmd, 0);
    SPI_FLASH_CS_DIS();
    // wait until ready
		r=spi_flash_wait(wait);
    if(!(r)){
      break;
    }
    //reload_iwdg();
  }
  // wr disable
  spi_flash_wrdi();

  return r;
}


/**
* erase chip	The all data of flash will be erased.
*	@arg		addr	The flash address where you want to write data
*	@arg		*buf	Data to write
*	@arg		len	Data length
*/
uint8_t spi_flash_erase_chip(uint32_t wait)
{
  uint8_t cmd[1];

  // wr enable
  spi_flash_wren();
  
  // chip erase  
  cmd[0] = SPI_FLASH_OP_CE;
  SPI_FLASH_CS_ENA();  
  spi2_trx(1, cmd, 0);
  SPI_FLASH_CS_DIS();

  // wr disable
  spi_flash_wrdi();
  
  return spi_flash_wait(wait);
}

// check ready
uint8_t spi_flash_ready(void)
{
  if(spi_flash_rdsr() & SPI_FLASH_SR_BUSY){
    return 0;
  }else{
    return 1;
  }
}

// wait ready
uint8_t spi_flash_wait(uint32_t t)
{
	uint32_t ticks = (SystemCoreClock / 1000UL) * t;
  uint32_t lapse, now, last = SysTick->VAL;

  while (1) {
    now = SysTick->VAL;
    if(spi_flash_ready() == 1) return 1;
    if(now <= last) {
      lapse = last - now; 
    } else {
      lapse = last + SysTick->LOAD - now;
    }
    if (ticks <= lapse) 
      break;
    ticks -= lapse;
    last = now;
  }

  return 0;
}

// read sr
uint8_t spi_flash_rdsr(void)
{
  uint8_t cmd[1];
  
  cmd[0] = SPI_FLASH_OP_RDSR;
  SPI_FLASH_CS_ENA();  
  spi2_trx(1, cmd, 0);
  spi2_trx(1, 0, cmd);
  SPI_FLASH_CS_DIS();
  
  return cmd[0];
}

// read did
uint16_t spi_flash_rdid(void)
{
  uint8_t cmd[2];
  
  cmd[0] = SPI_FLASH_OP_RDID;
  cmd[1] = 0;
  SPI_FLASH_CS_ENA();  
  spi2_trx(2, cmd, 0);
  cmd[0] = 0; cmd[1] = 0;
  spi2_trx(2, cmd, 0);
  spi2_trx(2, 0, cmd);
  SPI_FLASH_CS_DIS();

	//printf("%02X%02X",cmd[0],cmd[1]);
  
  return ((uint16_t)cmd[0]<<8)|cmd[1];
}

// write ena
void spi_flash_wren(void)
{
  uint8_t cmd[2];

  // enable SR
  cmd[0] = SPI_FLASH_OP_EWSR;
  SPI_FLASH_CS_ENA();  
  spi2_trx(1, cmd, 0);
  SPI_FLASH_CS_DIS();
  
  // clear SR
  cmd[0] = SPI_FLASH_OP_WRSR;
  cmd[1] = 0x00;
  SPI_FLASH_CS_ENA();  
  spi2_trx(2, cmd, 0);
  SPI_FLASH_CS_DIS();

  // enable write
  cmd[0] = SPI_FLASH_OP_WREN;
  SPI_FLASH_CS_ENA();  
  spi2_trx(1, cmd, 0);
  SPI_FLASH_CS_DIS();
}

// write dis
void spi_flash_wrdi(void)
{
  uint8_t cmd[1];
  
  // wr disable
  cmd[0] = SPI_FLASH_OP_WRDI;
  SPI_FLASH_CS_ENA();  
  spi2_trx(1, cmd, 0);
  SPI_FLASH_CS_DIS();
}

