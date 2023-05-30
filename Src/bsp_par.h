#ifndef BSP_PAR_H
#define BSP_PAR_H

/*
共8192（0x2000）个字节,8*8192=64KB
BYTES:512*1 
WORDS:512*2
REALS:512*4
SIZE = 512+1024+2048 =3584
Sum = 3584*2 = 7168
*/
#define ParMain_BYTE_BASE		0
#define ParMain_WORD_BASE		0x200
#define ParMain_REAL_BASE		0x600

#define ParBack_BYTE_BASE		0xE00
#define ParBack_WORD_BASE		0x1000
#define ParBack_REAL_BASE		0x1400

#define Max_BYTE_NUM           512
#define Max_WORD_NUM           512
#define Max_REAL_NUM           512

#define ParMainCheckCode_Base	0x1C00      //主存储区校验起始地址2*3
#define ParBackCheckCode_Base	0x1C06      //备份区校验起始地址2*3

#define SYS_PRODUCT_BASE            0x1E00      //系统/产品信息存储地址    
#define SYS_PRODUCT_BACKUP          0x1F00      //系统/产品信息存储地址


uint16_t Par_Init_State;
uint8_t PAR_BYTES[Max_BYTE_NUM];
uint16_t PAR_WORDS[Max_WORD_NUM];
float PAR_REALS[Max_REAL_NUM];


void bsp_PAR_Init(void);
uint16_t API_PAR_Init(void);
uint8_t API_PAR_Data_Init(uint8_t Par_type, const uint8_t buffer,  uint16_t len);

uint16_t API_PAR_SaveBytes(uint16_t Address, const uint8_t buffer, uint16_t len);
uint16_t API_PAR_SaveWords(uint16_t Address, const uint16_t buffer, uint16_t len);
uint16_t API_PAR_SaveDwords(uint16_t Address, const uint32_t buffer, uint16_t len);

uint16_t API_PAR_GetBytes(uint16_t Address, uint8_t *buffer, uint16_t len);
uint16_t API_PAR_GetWords(uint16_t Address, uint16_t *buffer, uint16_t len);
uint16_t API_PAR_GetDwords(uint16_t Address, uint32_t *buffer, uint16_t len);
//uint16_t API_PAR_GetReals(uint16_t Address, real *buffer, uint16_t len);


#endif

