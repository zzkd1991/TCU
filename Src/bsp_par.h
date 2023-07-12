#ifndef BSP_PAR_H
#define BSP_PAR_H

#include "main.h"
#include "Par.h"

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

#define ParMainCheckCode_Base	0x1C00      //主存储区校验起始地址4*3
#define ParBackCheckCode_Base	0x1C0C      //备份区校验起始地址4*3

#define SYS_PRODUCT_BASE            0x1E00      //系统/产品信息存储地址    
#define SYS_PRODUCT_BACKUP          0x1F00      //系统/产品信息存储地址


extern U8 Par_Init_State;
extern U8 PAR_BYTES[Max_BYTE_NUM];
extern U16 PAR_WORDS[Max_WORD_NUM];
extern REAL32 PAR_REALS[Max_REAL_NUM];

typedef union
{
	float x;
	U8 s[4];
}f32_sep;



void bsp_PAR_Init(void);
U16 API_PAR_Init(void);
U8 API_PAR_Data_Init(U8 Par_type, U8 *buffer,  U16 len);

U16 API_PAR_SaveBytes(U16 Address, U8 *buffer, U16 len);
U16 API_PAR_SaveWords(U16 Address, U16 *buffer, U16 len);
U16 API_PAR_SaveReals(U16 Address, REAL32 *buffer, U16 len);


U16 API_PAR_GetBytes(U16 Address, U8 *buffer, U16 len);
U16 API_PAR_GetWords(U16 Address, U16 *buffer, U16 len);
U16 API_PAR_GetReals(U16 Address, REAL32 *buffer, U16 len);


#endif

