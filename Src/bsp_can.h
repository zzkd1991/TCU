#ifndef __CAN_H
#define	__CAN_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif
/* Includes---------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern CAN_HandleTypeDef hcan1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#define CAN_SEND_OK		0

#define CAN_SEND_ERR	1


typedef void * CAN_PORT;

typedef void * CAN_HANDLE;

typedef struct {
	uint32_t cob_id;	/**< message's ID*/
	uint8_t rtr;		/** < remote transmission request. (0 if not rtr message, 1 if rtr message )*/
	uint8_t len;		/**< messgae's length (0 to 8) */
	uint8_t data[8];	/**< message's datas */
	uint8_t msgType;
}Message;

/* configuration table */
typedef struct {
	uint32_t bsp;				
	uint32_t prescaler;		/*1~1023*/	
	uint32_t SWJ;		    /* resynchronization jump width */		
	uint32_t BS2;			/* time segment 2*/
	uint32_t BS1;			/* time segment 1*/
}can_baudrate_config_t;


void MX_CAN1_Init(void);

/* USER CODE BEGIN Prototypes */
void CAN_User_Init(CAN_HandleTypeDef* hcan );

void CAN1_TX_HwiCreate(void);

void CAN1_RX_HwiCreate(void);

void Can_Config(void);

/*debug*/

#define CAN_DEBUG_ON         1
#define CAN_DEBUG_ARRAY_ON   1
#define CAN_DEBUG_FUNC_ON    1


#define CAN_SEND_OK			0
#define CAN_SEND_ERR		1

#ifndef getSDOcs
#define getSDOcs(byte)	(byte >> 5)
#endif
   
// Log define
#define CAN_INFO(fmt,arg...)           printf("<<-CAN-INFO->> "fmt"\n",##arg)
#define CAN_ERROR(fmt,arg...)          printf("<<-CAN-ERROR->> "fmt"\n",##arg)
#define CAN_DEBUG(fmt,arg...)          do{\
                                         if(CAN_DEBUG_ON)\
                                         printf("<<-CAN-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                       }while(0)

#define CAN_DEBUG_ARRAY(array, num)    do{\
                                         int32_t i;\
                                         uint8_t* a = array;\
                                         if(CAN_DEBUG_ARRAY_ON)\
                                         {\
                                            printf("<<-CAN-DEBUG-ARRAY->>\n");\
                                            for (i = 0; i < (num); i++)\
                                            {\
                                                printf("%02x   ", (a)[i]);\
                                                if ((i + 1 ) %10 == 0)\
                                                {\
                                                    printf("\n");\
                                                }\
                                            }\
                                            printf("\n");\
                                        }\
                                       }while(0)

#define CAN_DEBUG_FUNC()               do{\
                                         if(CAN_DEBUG_FUNC_ON)\
                                         printf("<<-CAN-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)


static void Can_Gpio_Config(void);
static void Can_NVIC_Config(void);
static void Can_Mode_Config(void);
static void Can_Filter_Config(uint32_t MessageId );
void Can_Config(void);																	 
void Init_RxMes(void);
int Set_Can_Baud_Rate(int rate, CAN_HandleTypeDef *hcan);
int Can_Message_Dispatch(Message *m);
uint8_t Can_Send(CAN_PORT notused, Message *m, CAN_HandleTypeDef *hcan);
uint8_t CanInsert_SendQueue(CAN_PORT notused, Message *m);
uint8_t CanGet_SendQueue(void);	


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif
																			 
#endif








