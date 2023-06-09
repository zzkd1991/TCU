#ifndef __CAN_MCP2515_H
#define __CAN_MCP2515_H


//#include "sys.h"
#include <stdint.h>
#include <stdbool.h>

#define NULL 0
#ifdef __cplusplus
extern "C" {
#endif

/* MCP2515 CAN  �������ݽṹ */
typedef struct
{
    uint32_t StdId;      /* ��׼֡ ID */
    uint32_t ExtId;      /* ��չ֡ ID */
    uint32_t  IDE;        /* 0 : ��׼֡  1����չ֡ */
    uint8_t  RTR;        /* 0 ������֡  1��Զ��֡ */
    uint8_t  DLC;        /* ���ݳ��� */
    uint8_t  Data[8];    /* �������� */
}_MCP2515_CAN_MSG;    
    
    
bool MCP2515_Init(void);
bool MCP2515_Can_Send_Msg(_MCP2515_CAN_MSG *msg);  
_MCP2515_CAN_MSG *MCP_Can_Recv_Msg(void);

void Test_Mcp2515_Can(void);
    
#ifdef __cplusplus
}
#endif


#endif



