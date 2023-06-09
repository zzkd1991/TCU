#ifndef __I2C_EE_H
#define	__I2C_EE_H

#include "stm32f4xx.h"

//I2C2_SCL----PF1
//I2C2_SDA----PF0

#define FRAM_PAGESIZE 	   8			

#define I2C_OWN_ADDRESS7      0X0A   

#define I2Cx                             I2C1
#define I2Cx_CLK_ENABLE()                __HAL_RCC_I2C1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE() 

#define I2Cx_FORCE_RESET()               __HAL_RCC_I2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()             __HAL_RCC_I2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_8
#define I2Cx_SCL_GPIO_PORT              GPIOB
#define I2Cx_SCL_AF                     GPIO_AF4_I2C1
#define I2Cx_SDA_PIN                    GPIO_PIN_9
#define I2Cx_SDA_GPIO_PORT              GPIOB
#define I2Cx_SDA_AF                     GPIO_AF4_I2C1


#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

#define I2Cx_TIMEOUT_MAX                300
/* Maximum number of trials for HAL_I2C_IsDeviceReady() function */
#define FRAM_MAX_TRIALS               300

#define FRAM_DEBUG_ON         0

#define FRAM_INFO(fmt,arg...)           printf("<<-EEPROM-INFO->> "fmt"\n",##arg)
#define FRAM_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define FRAM_DEBUG(fmt,arg...)          do{\
                                          if(EEPROM_DEBUG_ON)\
                                          printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

/* 
 * fm24cl64b 64kb = 65536bit = 65536/8 B = 8192 B
 * 8192 pages of 8 bytes each
 * memory address rang 0000h~1FFFh
 * Device Address
 *|MSB                                        LSB|
 *|<--Slave ID--->|   |<-Device Select-->|
 * 1    0    1    0    A2      A1       A0     R/W
 * 1    0    1    0    0       0        0       0 = 0XA0
 * 1    0    1    0    0       0        0       1 = 0XA1 
 */

#define FRAM_ADDRESS        0xA0

void I2C_Fram_Init(void);
										  
uint32_t I2C_Fram_BufferWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t I2C_Fram_ByteWrite(uint8_t* pBuffer, uint16_t WriteAddr);
uint32_t I2C_Fram_PageWrite(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t NumByteToWrite);
uint32_t I2C_Fram_BufferRead(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t NumByteToRead);

extern I2C_HandleTypeDef  I2C_Handle;
extern void Error_Handler(void);
#endif /* __I2C_EE_H */


