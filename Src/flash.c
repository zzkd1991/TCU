#include <stdio.h>
#include <stdint.h>
#include "flash.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"	{
#endif
#endif

extern void FramWriteByte(uint16_t address, uint8_t da);


void flashEraseBlock(CCP_MTABYTEPTR a)
{
	return;
}


void flashByteWrite(CCP_MTABYTEPTR a, CCP_BYTE data)
{
	FramWriteByte((uint16_t)a, (uint8_t)data);
	return;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


