#ifndef __IWDG_H
#define	__IWDG_H

#include "stm32f4xx.h"

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif

void IWDG_Config(uint8_t prv, uint16_t rlv);
void IWDG_Feed(void);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif


#endif








