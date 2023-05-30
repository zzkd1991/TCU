#ifndef __FLASH_H__
#define __FLASH_H__

#include "ccp.h"

#ifdef __cplusplus
extern "C" {
#endif


void flashEraseBlock(CCP_MTABYTEPTR a);

void flashByteWrite(CCP_MTABYTEPTR a, CCP_BYTE data);


#ifdef __cplusplus
}
#endif

#endif




