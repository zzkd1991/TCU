#ifndef _CRC_H
#define _CRC_H

#include <stdint.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

uint32_t calc_crc32(uint32_t origin, const void *buf, int32_t len);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif

#endif

