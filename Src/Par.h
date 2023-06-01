#ifndef PAR_H
#define PAR_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef U8
	#define U8 unsigned char
#endif
#ifndef U16
	#define U16 unsigned short
#endif
#ifndef U32
	#define U32 unsigned int
#endif
#ifndef U64
	#define U64 unsigned long long
#endif
#ifndef S8
	#define S8 signed char
#endif
#ifndef S16
	#define S16 signed short
#endif
#ifndef S32
	#define S32 signed long
#endif
#ifndef S64
	#define S64 signed long long
#endif
#ifndef REAL32
	#define REAL32 float
#endif

#define SET_PAR_STATUS_BIT(VALUE, BIT, SET) (VALUE = (VALUE & (~(0x01 << BIT))) | ((SET & 0x01) << BIT))

#define PAR_ADDR               (0)//(10 * 1024)			//8K的FRAM，从0地址开始写入
#define CRCINIT                0xAA55

#define PAR_READ_COUNT         2
#define PAR_WRITE_COUNT        2

#define PAR_ZONE_MAIN          0
#define PAR_ZONE_BACK          1
  
#define PAR_TYPE_BYTE          1
#define PAR_TYPE_WORD          2
#define PAR_TYPE_DWORD         4

#define PAR_BYTE_MAIN_BIT      0
#define PAR_BYTE_BACK_BIT      1
#define PAR_WORD_MAIN_BIT      2
#define PAR_WORD_BACK_BIT      3
#define PAR_DWORD_MAIN_BIT     4
#define PAR_DWORD_BACK_BIT     5

#define PAR_WRITE_STATUS_ING   1
#define PAR_WRITE_STATUS_OVER  0

#define ONE_BY_ONE_WRITE


#define PAR_TRUE               0
#define PAR_FALSE              1
#define PAR_CHECK_TRUE         0
#define PAR_CHECK_FALSE        1
#define PAR_WRITE_TRUE         0
#define PAR_WRITE_FALSE        1


#define PAR_TYPE_ERROR         0xFF


#define PAR_INDEX_ERROR        2
#define PAR_BUFF_ERROR         3
#define PAR_LEN_ERROR          4
#define PAR_STATUS_ERROR       5
#define PAR_WRITING            6
#define PAR_ZONE_ERROR         7
#define PAR_WRITE_RESULT_ERROR 8
#define PAR_ADDR_ERROR         9
//#define PAR_DATA_SAME          10

#define PAR_WRITE_OKFLAG       0x00
#define PAR_WRITE_ERRORFLAG    0xAA

#define PAR_DATA_COUNT         512


#endif

