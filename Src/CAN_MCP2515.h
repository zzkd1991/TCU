#ifndef __CAN_MCP2515_H
#define __CAN_MCP2515_H


//#include "sys.h"
#include <stdint.h>
#include <stdbool.h>

#define NULL 0
#ifdef __cplusplus
extern "C" {
#endif

/* MCP2515 CAN  报文数据结构 */
typedef struct
{
    uint32_t StdId;      /* 标准帧 ID */
    uint32_t ExtId;      /* 拓展帧 ID */
    uint32_t  IDE;        /* 0 : 标准帧  1：拓展帧 */
    uint8_t  RTR;        /* 0 ：数据帧  1：远程帧 */
    uint8_t  DLC;        /* 数据长度 */
    uint8_t  Data[8];    /* 数据内容 */
}_MCP2515_CAN_MSG;    
    
    
bool MCP2515_Init(void);
bool MCP2515_Can_Send_Msg(_MCP2515_CAN_MSG *msg);  
_MCP2515_CAN_MSG *MCP_Can_Recv_Msg(void);

void Test_Mcp2515_Can(void);
    
#ifdef __cplusplus
}
#endif


#endif



