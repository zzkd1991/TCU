/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "bsp_can.h"
#include "can_queue.h"
#include <string.h>
//#include "can_queue.h"

#if 1

extern void Error_Handler(void);

#define CAN1FIFO CAN_RX_FIFO0
#define CAN2FIFO CAN_RX_FIFO1
//CAN_TxHeaderTypeDef TxMeg;
//CAN_RxHeaderTypeDef RxMeg;

/* USER CODE END 0*/

uint32_t id1 = 0x0A;
uint32_t hal_can_tx_counter1 = 0;
uint32_t hal_can_tx_counter2 = 0;
uint32_t dict_tx_err_counter = 0;
uint32_t dict_tx_write_queue = 0;
uint32_t received_packet_num = 0;


CAN_HandleTypeDef hcan1;
CAN_HandleTypeDef hcan2;


#define CAN1_TX_IRQ		CAN1_TX_IRQn + 16
#define CAN1_RX_IRQ		CAN1_RX0_IRQn + 16

// pclk1 = sysclk(72M) / 4 = 36M
// baudrate = PCLK1(36M) / (SJW+BS1+BS2) / Prescaler

 const can_baudrate_config_t baudrate_config[] = {
 { 1000000,   4, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_5TQ },
 {	900000,   4, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_6TQ },
 {	800000,   5, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_5TQ },
 {	600000,   5, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_8TQ },
 {	500000,   8, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_5TQ },
 {	300000,  10, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_8TQ },
 {	250000,  16, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_5TQ },
 {	200000,  20, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_5TQ },
 {	125000,  32, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_5TQ },
 {	100000,  40, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_5TQ },
 {	 50000,  80, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_5TQ },
 {	 40000, 100, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_5TQ },
 {	 20000, 200, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_5TQ },
 {	 10000, 400, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_5TQ },
 {	  5000, 800, CAN_SJW_1TQ, CAN_BS1_3TQ, CAN_BS2_5TQ },
 };

void CAN_User_Init(CAN_HandleTypeDef* hcan) //???????
{
	CAN_FilterTypeDef sFilterConfig;
	HAL_StatusTypeDef HAL_Status;

#if 1
	//TxMeg.IDE = CAN_ID_STD;//CAN_ID_EXT;
	//TxMeg.RTR = CAN_RTR_DATA;

	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;//???????
	sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;

	sFilterConfig.FilterIdHigh = 0xff << 5;//0x60A
	sFilterConfig.FilterIdLow = 0x20A << 5;

	sFilterConfig.FilterMaskIdHigh = 0x30A << 5;
	sFilterConfig.FilterMaskIdLow = 0x40A << 5;

#else
	TxMeg.IDE = CAN_ID_STD;
	TxMeg.RTR = CAN_RTR_DATA;
	actual_nodeid = 0x600 + slaveNode;

	sFilterConfig.FilterBank = 0;
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;

	sFilterConfig.FilterIdHigh = ((((uint32_t)actual_nodeid << 21) | CAN_ID_STD | CAN_RTR_DATA) & 0xFFFF0000) >> 16;
	sFilterConfig.FilterIdLow = (((uint32_t)actual_nodeid << 21) | CAN_ID_STD | CAN_RTR_DATA) & 0xFFFF;

#if 0
	sFilterConfig.FilterMaskIdHigh = 0xFFFF;
	sFilterConfig.FilterMaskIdLow = 0xFFFF;

#else
	sFilterConfig.FilterMaskIdHigh = 0x1FF;//?????0x0A
	sFilterConfig.FilterMaskIdLow = 0xFFFF;//??????

#endif
	
#endif

	sFilterConfig.FilterFIFOAssignment = CAN1FIFO;//?????????FIFO0?

	sFilterConfig.FilterActivation = ENABLE;//?????
	sFilterConfig.SlaveStartFilterBank = 14;

	HAL_Status = HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
	
	HAL_Status = HAL_CAN_Start(hcan); //??CAN

	if(HAL_Status != HAL_OK)
	{
		printf("Start CAN error !\r\n");
	}

	HAL_Status = HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);//FIFO0 ????????
	if(HAL_Status != HAL_OK)
	{
		printf("ActivateNotification CAN error\r\n");
	}

	HAL_Status = HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO1_MSG_PENDING);//FIFO1 ????????
	if(HAL_Status != HAL_OK)
	{
		printf("ActivateNotification CAN error\r\n");
	}

	/*HAL_Status = HAL_CAN_ActivateNotification(&hcan1, CAN_IT_TX_MAILBOX_EMPTY);//???????
	if(HAL_Status != HAL_OK)
	{
		printf("ActivateNotification CAN error\r\n");
	}*/

}

int dev_txempty(CAN_HandleTypeDef *hcan)
{
	int freetxnum = 0;

	freetxnum = HAL_CAN_GetTxMailboxesFreeLevel(hcan);

	return (freetxnum == 0) ? 0 : 1;
}

uint8_t CANx_SendNormalData(CAN_HandleTypeDef *hcan, uint16_t ID, uint8_t *pData, uint8_t ide, uint8_t rtr, uint16_t Len, uint32_t *mailbox_num)
{
	HAL_StatusTypeDef HAL_RetVal;
	CAN_TxHeaderTypeDef TxMeg = {0};
	uint16_t SendTimes, SendCNT = 0;
	uint8_t FreeTxNum = 0;
	uint32_t tx_mailbox;
	//TxMeg.StdId = ID;
	//TxMeg.IDE = CAN_ID_STD;//CAN_ID_EXT;
	//TxMeg.RTR = CAN_RTR_DATA;
	if(rtr == 0)//数据帧
	{
		TxMeg.RTR = CAN_RTR_DATA;
	}
	else if(rtr == 1)
	{
		TxMeg.RTR = CAN_RTR_REMOTE;
	}

	if(ide == 0)//标准帧
	{
		TxMeg.StdId = ID;
		TxMeg.IDE = CAN_ID_STD;
	}
	else if(ide == 1)
	{
		TxMeg.ExtId = ID;
		TxMeg.IDE = CAN_ID_EXT;
	}
	
	if(!hcan || ! pData || !Len) return 1;
	SendTimes = Len / 8 + ( Len % 8 ? 1 : 0);
	FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(hcan);
	TxMeg.DLC = 8;
	while(SendTimes--){
		if(0 == SendTimes){
			if(Len % 8)
				TxMeg.DLC = Len % 8;
		}
#if 1	
		while(0 == FreeTxNum){
			FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(hcan);
		}
#endif
		HAL_RetVal = HAL_CAN_AddTxMessage(hcan, &TxMeg, pData + SendCNT, (uint32_t*)&tx_mailbox);
		if(HAL_RetVal != HAL_OK)
		{
			return 2;
		}
		SendCNT += 8;
	
	}

	return 0;
	
}

uint8_t CANx_SendNormalData_No_Wait(CAN_HandleTypeDef *hcan, uint16_t ID, uint8_t *pData, uint8_t ide, uint8_t rtr, uint16_t Len, uint32_t *mailbox_num)
{
	HAL_StatusTypeDef HAL_RetVal;
	CAN_TxHeaderTypeDef TxMeg = {0};
	uint16_t SendTimes, SendCNT = 0;
	uint8_t FreeTxNum = 0;
	uint32_t tx_mailbox;
	//TxMeg.StdId = ID;
	//TxMeg.IDE = CAN_ID_STD;//CAN_ID_EXT;
	//TxMeg.RTR = CAN_RTR_DATA;
	if(rtr == 0)//数据帧
	{
		TxMeg.RTR = CAN_RTR_DATA;
	}
	else if(rtr == 1)
	{
		TxMeg.RTR = CAN_RTR_REMOTE;
	}

	if(ide == 0)//标准帧
	{
		TxMeg.StdId = ID;
		TxMeg.IDE = CAN_ID_STD;
	}
	else if(ide == 1)
	{
		TxMeg.ExtId = ID;
		TxMeg.IDE = CAN_ID_EXT;
	}
	
	if(!hcan || ! pData || !Len) return 1;
	SendTimes = Len / 8 + ( Len % 8 ? 1 : 0);
	FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(hcan);
	TxMeg.DLC = 8;
	while(SendTimes--){
		if(0 == SendTimes){
			if(Len % 8)
				TxMeg.DLC = Len % 8;
		}
#if 0	
		while(0 == FreeTxNum){
			FreeTxNum = HAL_CAN_GetTxMailboxesFreeLevel(hcan);
		}
#else
		if(FreeTxNum == 0)
		{
			return 3;
		}
#endif
		HAL_RetVal = HAL_CAN_AddTxMessage(hcan, &TxMeg, pData + SendCNT, (uint32_t*)&tx_mailbox);
		if(HAL_RetVal != HAL_OK)
		{
			return 2;
		}
		SendCNT += 8;
	
	}
	return 0;
}

uint8_t Can_Send(CAN_PORT notused, Message *m, CAN_HandleTypeDef *hcan)
{
	uint16_t messageid;
	uint8_t result;
	uint32_t tx_mailbox;
#if 1
	messageid = m->cob_id;

	result = CANx_SendNormalData(hcan, messageid, m->data, m->ide, m->rtr, m->len, &tx_mailbox);

	return result;
#else
	result = InsertCanQueueTx(*m);
	if(result == 0)
	{
		Error_Handler();
	}	

	return result;
#endif
}

uint8_t CanInsert_SendQueue(CAN_PORT notused, Message *m)
{
	uint8_t result;
	result = InsertCanQueueTx(*m);
	if(result == 0)//发送队列满
	{
		while(IsFullCanQueueTx());
		//Error_Handler();
		result = InsertCanQueueTx(*m);
	}	

	return result;
}


uint8_t ST_CanGet_SendQueue(void)
{
	extern struct _CANQueue CANQueueTx;
	uint16_t head;
	uint8_t result;
	uint32_t tx_mailbox;
	Message TxMessage = {0};
	head = CANQueueTx.front;

	if(1 == GetCanQueueTx(head, &TxMessage))
	{
		head = (head + 1) % MAX_CAN_SIZE;
		SetHeadCanQueueTx(head);
		
		result = CANx_SendNormalData(&hcan1, TxMessage.cob_id, TxMessage.data, TxMessage.ide, TxMessage.rtr, TxMessage.len, &tx_mailbox);

		if(result != 0)
		{
			Error_Handler();
		}
	}
	else
	{
		return 1;
	}

	return result;
}



void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{

	hal_can_tx_counter1++;
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef * hcan)
{
	hal_can_tx_counter1++;
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	hal_can_tx_counter1++;
}


int Set_Can_Baud_Rate(int rate, CAN_HandleTypeDef *hcan)
{
	//int rate_array[9] = {10000, 20000, 50000, 100000, 125000, 250000, 500000, 800000, 1000000};
	//int prescaler = 0;
	int i = 0;
	
	for(i = 0; i < sizeof(baudrate_config) / sizeof(baudrate_config[0]); i++)
	{
		if(baudrate_config[i].bsp == rate * 1000)
		{
			break;
		}
	}

	if(i == (sizeof(baudrate_config) / sizeof(baudrate_config[0]) + 1))
	{
		printf("rate is not supported\n");
		return 1;
	}


	hcan->Init.Prescaler = baudrate_config[i].prescaler;
	hcan->Init.SyncJumpWidth = baudrate_config[i].SWJ;
	hcan->Init.TimeSeg1 = baudrate_config[i].BS1;
	hcan->Init.TimeSeg2 = baudrate_config[i].BS2;
	if(HAL_CAN_Init(hcan) != HAL_OK)
	{
		Error_Handler();
	}

	return 0;
}

void stm32can_rxinterrupt(CAN_HandleTypeDef *hcan, int Fifo_Num)
{
	HAL_StatusTypeDef HAL_RetVal;
	CAN_RxHeaderTypeDef RxMeg = {0};
	uint8_t Data[8];
	Message m;
	uint32_t i = 0;
	uint32_t len = 0;

	memset(&m, 0, sizeof(m));

	received_packet_num++;

	if(hcan == &hcan1){
		if(Fifo_Num == 0)
		{
			HAL_RetVal = HAL_CAN_GetRxMessage(&hcan1, CAN1FIFO, &RxMeg, Data);
		}
		else if(Fifo_Num == 1)
		{
			HAL_RetVal = HAL_CAN_GetRxMessage(&hcan1, CAN2FIFO, &RxMeg, Data);
		}
		else
		{
			return;
		}
		if(HAL_OK == HAL_RetVal)
		{
			if(RxMeg.IDE == CAN_ID_STD)
			{
				m.cob_id = RxMeg.StdId;
			}
			else
			{
				m.cob_id = RxMeg.ExtId;
			}

			if(RxMeg.RTR == CAN_RTR_REMOTE)
			{
				m.rtr = 1;
			}
			else
			{
				m.rtr = 0;
			}

			len = RxMeg.DLC;

			for(i = 0; i < len; i++)
			{
				m.data[i] = Data[i];
			}

			for(i = len; i < 8; i++)
			{
				memset((void*)&m.data[i], 0, sizeof(m.data[i]));
			}

			m.len = len;

			if(0 == InsertCanQueueRx(m))
			{
				Error_Handler();
			}
		}
	}
	else if(hcan == &hcan2)
	{
		if(Fifo_Num == 0)
		{
			HAL_RetVal = HAL_CAN_GetRxMessage(&hcan2, CAN1FIFO, &RxMeg, Data);
		}
		else if(Fifo_Num == 1)
		{
			HAL_RetVal = HAL_CAN_GetRxMessage(&hcan2, CAN2FIFO, &RxMeg, Data);
		}
		else
		{
			return;
		}
		if(HAL_OK == HAL_RetVal)
		{
			if(RxMeg.IDE == CAN_ID_STD)
			{
				m.cob_id = RxMeg.StdId;
			}
			else
			{
				m.cob_id = RxMeg.ExtId;
			}

			if(RxMeg.RTR == CAN_RTR_REMOTE)
			{
				m.rtr = 1;
			}
			else
			{
				m.rtr = 0;
			}

			len = RxMeg.DLC;

			for(i = 0; i < len; i++)
			{
				m.data[i] = Data[i];
			}

			for(i = len; i < 8; i++)
			{
				memset((void*)&m.data[i], 0, sizeof(m.data[i]));
			}

			m.len = len;

			if(0 == InsertCanQueueRx(m))
			{
				Error_Handler();
			}
		}
	}
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)//FIFO0 ??????
{
	stm32can_rxinterrupt(hcan, 0);
}

void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)//FIFO1 ??????
{
	stm32can_rxinterrupt(hcan, 1);
}



void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	//Error_Handler();
}



/* CAN1 init function */
void MX_CAN1_Init(void)
{
	/* USER CODE BEGIN CAN1_Init 0*/

	/* USER CODE END CAN1_Init 0 */

	/* USER CODE BEGIN CAN1_Init 1 */

	/* USER CODE END CAN1_Init 1 */
	hcan1.Instance = CAN1;
	hcan1.Init.Prescaler = 12;//500kbsp
	//hcan1.Init.Prescaler = 24;
	hcan1.Init.Mode = CAN_MODE_NORMAL;
	hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan1.Init.TimeSeg1 = CAN_BS1_3TQ;
	hcan1.Init.TimeSeg2 = CAN_BS2_3TQ;
	hcan1.Init.TimeTriggeredMode = DISABLE;
	hcan1.Init.AutoBusOff = ENABLE;
	hcan1.Init.AutoWakeUp = ENABLE;
	hcan1.Init.AutoRetransmission = DISABLE;
	hcan1.Init.ReceiveFifoLocked = DISABLE;
	hcan1.Init.TransmitFifoPriority = DISABLE;
	if(HAL_CAN_Init(&hcan1) != HAL_OK)
	{
		Error_Handler();
	}
}

/* CAN2 init function */
void MX_CAN2_Init(void)
{
	/* USER CODE BEGIN CAN2_Init 0*/

	/* USER CODE END CAN2_Init 0 */

	/* USER CODE BEGIN CAN2_Init 1 */

	/* USER CODE END CAN2_Init 1 */
	hcan2.Instance = CAN2;
	hcan2.Init.Prescaler = 12;//500kbsp
	//hcan1.Init.Prescaler = 24;
	hcan2.Init.Mode = CAN_MODE_NORMAL;
	hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan2.Init.TimeSeg1 = CAN_BS1_3TQ;
	hcan2.Init.TimeSeg2 = CAN_BS2_3TQ;
	hcan2.Init.TimeTriggeredMode = DISABLE;
	hcan2.Init.AutoBusOff = ENABLE;
	hcan2.Init.AutoWakeUp = ENABLE;
	hcan2.Init.AutoRetransmission = DISABLE;
	hcan2.Init.ReceiveFifoLocked = DISABLE;
	hcan2.Init.TransmitFifoPriority = DISABLE;
	if(HAL_CAN_Init(&hcan2) != HAL_OK)
	{
		Error_Handler();
	}
}

static uint32_t HAL_RCC_CAN1_CLK_ENABLED=0;


void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(hcan->Instance==CAN1)
	{
	/* USER CODE BEGIN CAN1_MspInit 0 */
	
	/* USER CODE END CAN1_MspInit 0 */
	  /* Peripheral clock enable */
	  HAL_RCC_CAN1_CLK_ENABLED++;
	  if(HAL_RCC_CAN1_CLK_ENABLED==1){
		__HAL_RCC_CAN1_CLK_ENABLE();
	  }
	
	  __HAL_RCC_GPIOB_CLK_ENABLE();
	  /**CAN1 GPIO Configuration
	  PB8	  ------> CAN1_RX
	  PB9	  ------> CAN1_TX
	  */
	  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	  /* CAN1 interrupt Init */
	  HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);
	  HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);
	  HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
	/* USER CODE BEGIN CAN1_MspInit 1 */
	
	/* USER CODE END CAN1_MspInit 1 */
	}
	else if(hcan->Instance==CAN2)
	{
	/* USER CODE BEGIN CAN2_MspInit 0 */
	
	/* USER CODE END CAN2_MspInit 0 */
	  /* Peripheral clock enable */
	  __HAL_RCC_CAN2_CLK_ENABLE();
	  HAL_RCC_CAN1_CLK_ENABLED++;
	  if(HAL_RCC_CAN1_CLK_ENABLED==1){
		__HAL_RCC_CAN1_CLK_ENABLE();
	  }
	
	  __HAL_RCC_GPIOB_CLK_ENABLE();
	  /**CAN2 GPIO Configuration
	  PB5	  ------> CAN2_RX
	  PB6	  ------> CAN2_TX
	  */
	  GPIO_InitStruct.Pin = GPIO_PIN_5|GPIO_PIN_6;
	  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	  GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
	  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	  /* CAN2 interrupt Init */
	  HAL_NVIC_SetPriority(CAN2_TX_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);
	  HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
	  HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
	/* USER CODE BEGIN CAN2_MspInit 1 */
	
	/* USER CODE END CAN2_MspInit 1 */
	}

}


void HAL_CAN_MspDeInit(CAN_HandleTypeDef *hcan)
{
	if(hcan->Instance==CAN1)
	{
	/* USER CODE BEGIN CAN1_MspDeInit 0 */
	
	/* USER CODE END CAN1_MspDeInit 0 */
	  /* Peripheral clock disable */
	  HAL_RCC_CAN1_CLK_ENABLED--;
	  if(HAL_RCC_CAN1_CLK_ENABLED==0){
		__HAL_RCC_CAN1_CLK_DISABLE();
	  }
	
	  /**CAN1 GPIO Configuration
	  PB8	  ------> CAN1_RX
	  PB9	  ------> CAN1_TX
	  */
	  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);
	
	  /* CAN1 interrupt DeInit */
	  HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
	  HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
	  HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
	/* USER CODE BEGIN CAN1_MspDeInit 1 */
	
	/* USER CODE END CAN1_MspDeInit 1 */
	}
	else if(hcan->Instance==CAN2)
	{
	/* USER CODE BEGIN CAN2_MspDeInit 0 */
	
	/* USER CODE END CAN2_MspDeInit 0 */
	  /* Peripheral clock disable */
	  __HAL_RCC_CAN2_CLK_DISABLE();
	  HAL_RCC_CAN1_CLK_ENABLED--;
	  if(HAL_RCC_CAN1_CLK_ENABLED==0){
		__HAL_RCC_CAN1_CLK_DISABLE();
	  }
	
	  /**CAN2 GPIO Configuration
	  PB5	  ------> CAN2_RX
	  PB6	  ------> CAN2_TX
	  */
	  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_5|GPIO_PIN_6);
	
	  /* CAN2 interrupt DeInit */
	  HAL_NVIC_DisableIRQ(CAN2_TX_IRQn);
	  HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
	  HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
	/* USER CODE BEGIN CAN2_MspDeInit 1 */
	
	/* USER CODE END CAN2_MspDeInit 1 */
	}

}

void CAN_Hardware_Config(void)
{
	MX_CAN1_Init();

	MX_CAN2_Init();

	CAN_User_Init(&hcan1);

	CAN_User_Init(&hcan2);
}

void CAN_Config(uint16_t can1_baud, uint16_t can2_baud)
{
	Set_Can_Baud_Rate(can1_baud, &hcan1);

	Set_Can_Baud_Rate(can2_baud, &hcan2);
}


signed short CAN1_ReceiveObj(uint32_t ID, uint8_t *Len, uint8_t *Data)
{
	extern struct _CANQueue CANQueueRx;
	uint16_t head;
	Message RxMessage;
	
	head = CANQueueRx.front;


	if(1 == GetCanQueueRx(head, &RxMessage))
	{
		head = (head + 1) % MAX_CAN_SIZE;
		SetHeadCanQueueRx(head);

		if(RxMessage.cob_id == ID)
		{
			*Len = RxMessage.len;
			memcpy(Data, RxMessage.data, RxMessage.len);
		}
		return 0;
	}
	else
	{
		return -1;
		//printf("CAN queue is empty\r\n");
	}	
}


uint8_t Can1_Tx_Msg(uint32_t id, uint8_t ide, uint8_t rtr, uint8_t len, uint8_t *dat)
{
	uint8_t result = 0;
	uint32_t tx_mailbox;
	
	result = CANx_SendNormalData_No_Wait(&hcan1, id, dat, ide, rtr, len, &tx_mailbox);

	if(result == 0)
	{
		return tx_mailbox;
	}

	else if(result == 3)
	{
		return 255;
	}

	else
	{
		Error_Handler();
		return 3;
	}
	
}


void CAN1_WriteData(uint32_t msgID, uint8_t bBytes[], int8_t iNoBytes, uint8_t ext, uint8_t mode, uint16_t cycle_ms)
{
	extern __IO uint32_t uwTick;
	Message m = {0};
	int i = 0;
	//uint8_t result;

	m.cob_id = msgID;
	m.ide = ext;
	m.rtr = 0;
	m.len = iNoBytes;
	for(i = 0; i < iNoBytes; i++)
	{
		m.data[i] = bBytes[i];
	}

	if(mode == 0)
	{
		return;
	}
	else if(mode == 1)//周期发送
	{
		if(uwTick % cycle_ms == 0)
		{
			Can_Send((CAN_PORT)NULL, &m, &hcan1);	
		}
	}
	else if(mode == 2)//数据变化发送
	{

		
	}
}


#endif


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


