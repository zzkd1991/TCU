#include "can_mcp2515.h"
#include "main.h"
#include "spi.h"
//#include "delay.h"

/* CAN 波特率选择 */
static uint16_t s_Baud = 500;
// static uint16_t s_Baud = 250; 
/* static uint16_t s_Baud = 500; */

/* MCP2515 外部晶振为 8M */
static uint32_t s_Osc = 8000;

/* mcp2515 控制指令 */
typedef enum
{
    RESET_CMD = 0xc0,           
    READ_CMD = 0x03,
    READ_RX_CMD	= 0x90,
    WRITE_CMD = 0x02,
    LOAD_TX_CMD = 0x40,
    RTS_CMD = 0x80,
    RTS_BUFF0_CMD = 0x81,
    RTS_BUFF1_CMD = 0x82,
    RTS_BUFF2_CMD = 0x84,
    READ_STATUS_CMD	= 0xA0,
    RX_STATUS_CMD = 0xB0,
    BIT_MODIFY_CMD = 0x05,
}_Cmd;

/* mcp2515 寄存器地址 */
typedef enum
{
    REG_RXF0SIDH = 0x00,
    REG_RXF0SIDL = 0x01,
    REG_RXF0EID8 = 0x02,
    REG_RXF0EID0 = 0x03,
    REG_RXF1SIDH = 0x04,
    REG_RXF1SIDL = 0x05,
    REG_RXF1EID8 = 0x06,
    REG_RXF1EID0 = 0x07,
    REG_RXF2SIDH = 0x08,
    REG_RXF2SIDL = 0x09,
    REG_RXF2EID8 = 0x0a,
    REG_RXF2EID0 = 0x0b,
    REG_BFPCTRL = 0x0c,
    REG_TXRTSCTRL = 0x0d,
    REG_CANSTAT = 0x0e,
    REG_CANCTRL = 0x0f,

    REG_RXF3SIDH = 0x10,
    REG_RXF3SIDL = 0x11,
    REG_RXF3EID8 = 0x12,
    REG_RXF3EID0 = 0x13,
    REG_RXF4SIDH = 0x14,
    REG_RXF4SIDL = 0x15,
    REG_RXF4EID8 = 0x16,
    REG_RXF4EID0 = 0x17,
    REG_RXF5SIDH = 0x18,
    REG_RXF5SIDL = 0x19,
    REG_RXF5EID8 = 0x1a,
    REG_RXF5EID0 = 0x1b,
    REG_TEC = 0x1c,
    REG_REC = 0x1d,

    REG_RXM0SIDH = 0x20,
    REG_RXM0SIDL = 0x21,
    REG_RXM0EID8 = 0x22,
    REG_RXM0EID0 = 0x23,
    REG_RXM1SIDH = 0x24,
    REG_RXM1SIDL = 0x25,
    REG_RXM1EID8 = 0x26,
    REG_RXM1EID0 = 0x27,
    REG_CNF3 = 0x28,
    REG_CNF2 = 0x29,
    REG_CNF1 = 0x2a,
    REG_CANINTE = 0x2b,//寄存器地址， The CANINTE register contains the individual interrupt enable bits for each interrupt source.
			CANINTE_MERRE =0x80,
			CANINTE_WAKIE =0x40,
			CANINTE_ERRIE =0x20,
			CANINTE_TX2IE =0x10,
			CANINTE_TX1IE =0x08,
			CANINTE_TX0IE =0x04,
			CANINTE_RX1IE =0x02,
			CANINTE_RX0IE =0x01,
    REG_CANINTF = 0x2c,// The CANINT Fregister contains the corresponding interrupt flag bit foreach interrupt source. 
			CANINTF_WAKIF =0x40,
			CANINTF_ERRIF =0x20,
			CANINTF_TX2IF =0x10,
			CANINTF_TX1IF =0x08,
			CANINTF_TX0IF =0x04,
			CANINTF_RX1IF =0x02,
			CANINTF_RX0IF =0x01,
			CANINTF_RX =(CANINTF_RX0IF | CANINTF_RX1IF),
			CANINTF_TX =(CANINTF_TX2IF | CANINTF_TX1IF | CANINTF_TX0IF),
		  CANINTF_ERR =(CANINTF_ERRIF), 
		
    REG_EFLG = 0x2d,

    REG_TXB0CTRL = 0x30,
    REG_TXB0SIDH = 0x31,
    REG_TXB0SIDL = 0x32,
    REG_TXB0EID8 = 0x33,
    REG_TXB0EID0 = 0x34,
    REG_TXB0DLC = 0x35,
    REG_TXB0D0 = 0x36,
    REG_TXB0D1 = 0x37,
    REG_TXB0D2 = 0x38,
    REG_TXB0D3 = 0x39,
    REG_TXB0D4 = 0x3a,
    REG_TXB0D5 = 0x3b,
    REG_TXB0D6 = 0x3c,
    REG_TXB0D7 = 0x3d,

    REG_TXB1CTRL = 0x40,
    REG_TXB1SIDH = 0x41,
    REG_TXB1SIDL = 0x42,
    REG_TXB1EID8 = 0x43,
    REG_TXB1EID0 = 0x44,
    REG_TXB1DLC = 0x45,
    REG_TXB1D0 = 0x46,
    REG_TXB1D1 = 0x47,
    REG_TXB1D2 = 0x48,
    REG_TXB1D3 = 0x49,
    REG_TXB1D4 = 0x4a,
    REG_TXB1D5 = 0x4b,
    REG_TXB1D6 = 0x4c,
    REG_TXB1D7 = 0x4d,

    REG_TXB2CTRL = 0x50,
    REG_TXB2SIDH = 0x51,
    REG_TXB2SIDL = 0x52,
    REG_TXB2EID8 = 0x53,
    REG_TXB2EID0 = 0x54,
    REG_TXB2DLC = 0x55,
    REG_TXB2D0 = 0x56,
    REG_TXB2D1 = 0x57,
    REG_TXB2D2 = 0x58,
    REG_TXB2D3 = 0x59,
    REG_TXB2D4 = 0x5a,
    REG_TXB2D5 = 0x5b,
    REG_TXB2D6 = 0x5c,
    REG_TXB2D7 = 0x5d,

    REG_RXB0CTRL = 0x60,
    REG_RXB0SIDH = 0x61,
    REG_RXB0SIDL = 0x62,
    REG_RXB0EID8 = 0x63,
    REG_RXB0EID0 = 0x64,
    REG_RXB0DLC = 0x65,
    REG_RXB0D0 = 0x66,
    REG_RXB0D1 = 0x67,
    REG_RXB0D2 = 0x68,
    REG_RXB0D3 = 0x69,
    REG_RXB0D4 = 0x6a,
    REG_RXB0D5 = 0x6b,
    REG_RXB0D6 = 0x6c,
    REG_RXB0D7 = 0x6d,

    REG_RXB1CTRL = 0x70,
    REG_RXB1SIDH = 0x71,
    REG_RXB1SIDL = 0x72,
    REG_RXB1EID8 = 0x73,
    REG_RXB1EID0 = 0x74,
    REG_RXB1DLC = 0x75,
    REG_RXB1D0 = 0x76,
    REG_RXB1D1 = 0x77,
    REG_RXB1D2 = 0x78,
    REG_RXB1D3 = 0x79,
    REG_RXB1D4 = 0x7a,
    REG_RXB1D5 = 0x7b,
    REG_RXB1D6 = 0x7c,
    REG_RXB1D7 = 0x7d,
}_RegAddr;

/* 寄存器 位 */
typedef enum
{
    REG_BIT0 = (1<<0),
    REG_BIT1 = (1<<1),
    REG_BIT2 = (1<<2),
    REG_BIT3 = (1<<3),
    REG_BIT4 = (1<<4),
    REG_BIT5 = (1<<5),
    REG_BIT6 = (1<<6),
    REG_BIT7 = (1<<7),
    
    BIT_SET = 1,
    BIT_RESET = 0,
}_Bit;

SPI_HandleTypeDef SPI1_Handler;  //SPI1句柄

void Spi1_Init(void)
{
	
		GPIO_InitTypeDef GPIO_Initure;
    

    __HAL_RCC_SPI1_CLK_ENABLE();        //使能SPI1时钟
	
	
    __HAL_RCC_GPIOB_CLK_ENABLE();       //使能GPIOB时钟
    //PB3,4,5
    GPIO_Initure.Pin=GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;              //复用推挽输出
    GPIO_Initure.Pull=GPIO_PULLDOWN;                 //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;             //快速            
    GPIO_Initure.Alternate=GPIO_AF5_SPI1;           //复用为SPI1
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	

    SPI1_Handler.Instance=SPI1;                         //SPI1
    SPI1_Handler.Init.Mode=SPI_MODE_MASTER;             //设置SPI工作模式，设置为主模式
    SPI1_Handler.Init.Direction=SPI_DIRECTION_2LINES;   //设置SPI单向或者双向的数据模式:SPI设置为双线模式
    SPI1_Handler.Init.DataSize=SPI_DATASIZE_8BIT;       //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI1_Handler.Init.CLKPolarity=SPI_POLARITY_LOW ;    //串行同步时钟的空闲状态为高电平
    SPI1_Handler.Init.CLKPhase=SPI_PHASE_1EDGE;         //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
    SPI1_Handler.Init.NSS=SPI_NSS_SOFT;                 //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI1_Handler.Init.BaudRatePrescaler=SPI_BAUDRATEPRESCALER_32;//定义波特率预分频的值:波特率预分频值为256
    SPI1_Handler.Init.FirstBit=SPI_FIRSTBIT_MSB;        //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI1_Handler.Init.TIMode=SPI_TIMODE_DISABLE;        //关闭TI模式
    SPI1_Handler.Init.CRCCalculation=SPI_CRCCALCULATION_DISABLE;//关闭硬件CRC校验
    SPI1_Handler.Init.CRCPolynomial=7;                  //CRC值计算的多项式
    HAL_SPI_Init(&SPI1_Handler);//初始化
    
		HAL_NVIC_SetPriority(SPI1_IRQn ,1,0);//定义SPI中断优先级
		HAL_NVIC_EnableIRQ(SPI1_IRQn);//使能SPI中断
		
    __HAL_SPI_ENABLE(&SPI1_Handler);                    //使能SPI1
	
    //Spi_SendRecData(0Xff);                           //启动传输
}



//static bool Spi1_Init(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;
//    SPI_InitTypeDef  SPI_InitStructure;

//    /* Peripheral Clock Enable -------------------------------------------------*/
//    /* Enable the SPI clock */
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

//    /* Enable GPIO clocks */
//    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA, ENABLE);

//    /* SPI GPIO Configuration --------------------------------------------------*/
//    /* Connect SPI pins to AF5 */  
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1);
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1);
//    GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1);

//    /* SPI  SCK/MISO/MOSI  */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);

//    /* SPI  CS */
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);

//    /* SPI configuration -------------------------------------------------------*/
////    SPI_I2S_DeInit(SPI1);
//    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
//    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
//    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
//    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
//    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
//    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//    SPI_InitStructure.SPI_CRCPolynomial = 7;
//    SPI_Init(SPI1,&SPI_InitStructure);

//    /* Configure the SPI interrupt priority */
//    NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);
//    
//    
//    
//    SPI_Cmd(SPI1,ENABLE);
//    
//    return true;
//}

/* MCP2515 CS 片选 设置 */
static void MCP2515_CS(uint8_t status)
{
    if( status )
        //GPIO_SetBits(GPIOA,GPIO_Pin_4);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
    else
        //GPIO_ResetBits(GPIOA,GPIO_Pin_4);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);
}

/*****************************************************
function    : static uint8_t Spi_SendRecData(uint8_t data)
description : SPi 发送8 位数据 同时接收 8 位数据
input       : data - 发送数据 
return      : fasle - 失败
              true  - 成功
******************************************************/
static uint8_t Spi_SendRecData(uint8_t data)
{
//    uint32_t i =0; 
//    while ( (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET) &&(i++<0xffff) );
//    SPI_I2S_SendData(SPI1, data);
//    
//    i =0;
//    while ( (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET) &&(i++<0xffff) );
//  
//    return SPI_I2S_ReceiveData(SPI1);  
	
	    uint8_t Rxdata;
    HAL_SPI_TransmitReceive(&hspi3,&data,&Rxdata,1, 1000);       
 	return Rxdata; 
	
}

/*******************************************************
function    : static bool MCP2515_WriteReg(uint8_t reg,uint8_t data)
description : 写mcp2515 寄存器 
input       : reg - 寄存器地址
              data - 寄存器数据
return      : true  - 成功
********************************************************/
static bool MCP2515_WriteReg(uint8_t reg,uint8_t data)
{
    
    MCP2515_CS(0);
    
    Spi_SendRecData(WRITE_CMD);
    
    Spi_SendRecData(reg);
    
    Spi_SendRecData(data);
    
    MCP2515_CS(1);
    
    return true;
}

/*******************************************************
function    : static void MCP2515_Load_TX_Buff(uint8_t reg)
description : 写mcp2515 TX 寄存器 ID/data
input       : reg - 寄存器地址
              data - 寄存器数据
return      : true  - 成功
********************************************************/
static void MCP2515_Load_TX_Buff(uint8_t reg)
{
    MCP2515_CS(0);
    
    Spi_SendRecData(LOAD_TX_CMD);
       
    Spi_SendRecData(0xff);
    
    MCP2515_CS(1);

}

/*****************************************************
function    : static bool MCP2515_RTS(uint8_t BuffNum)
description : 请求发送 
input       : BuffNum - 请求发送BUFF 编号 (0-2)
return      : void
*******************************************************/
static void MCP2515_RTS(uint8_t BuffNum)
{
    MCP2515_CS(0);
    
    if( 0 == BuffNum)
        Spi_SendRecData(RTS_BUFF0_CMD);
    else if( 1 == BuffNum)
        Spi_SendRecData(RTS_BUFF1_CMD);
    else
        Spi_SendRecData(RTS_BUFF2_CMD);
    
    MCP2515_CS(1);
    
}

/*******************************************************
function    : static uint8_t MCP2515_ReadReg(uint8_t reg)
description : 读mcp2515 寄存器 
input       : reg - 寄存器地址
return      : 返回寄存器数据
********************************************************/
static uint8_t MCP2515_ReadReg(uint8_t reg)
{
    uint8_t ret = 0;
    
    MCP2515_CS(0);
    
    Spi_SendRecData(READ_CMD);
   
    Spi_SendRecData(reg);
    
    ret = Spi_SendRecData(0xff);    /* 0xff无关数据 spi 时序使用 */
    
    MCP2515_CS(1);
    
    return ret;
}


/*******************************************************
function    : static uint8_t MCP2515_ReadStaReg(void)
description : 读mcp2515 状态寄存器 
input       : reg - 寄存器地址
return      : 返回寄存器数据
********************************************************/
static uint8_t MCP2515_ReadStaReg(void)
{
    uint8_t ret = 0;
    
    MCP2515_CS(0);
    
    Spi_SendRecData(READ_STATUS_CMD); 
    
    ret = Spi_SendRecData(0xff);    /* 0xff无关数据 spi 时序使用 */
    
    MCP2515_CS(1);
    
    return ret;
}

/*****************************************************************************
function    : static bool MCP2515_BitModify(uint8_t reg,uint8_t BitSel,uint8_t BitSta)
description : 寄存器 位修改 
input       : reg - 寄存器地址
              BitSel - 选择修改位
              BitSta - SET_BIT 置位
                     - RESET_BIT 清零
return      : true - 成功
******************************************************************************/
static bool MCP2515_BitModify(uint8_t reg,uint8_t BitSel,uint8_t BitSta)
{ 
    MCP2515_CS(0);
    
    Spi_SendRecData(BIT_MODIFY_CMD);
    
    Spi_SendRecData(reg);
    
    Spi_SendRecData(BitSel);    /* mask */
    
    if(BitSta)                  /* data */
        Spi_SendRecData(BitSel);
    else
        Spi_SendRecData(~BitSel);
    
    MCP2515_CS(1);
    
    return true;
}

/*************************************************************************
function    :static bool MCP2515_WriteReg_Seq(uint8_t reg,uint8_t *data,uint8_t length)
description :写寄存器序列
input       : reg - 寄存器地址
              *data - 数据序列
              length - 序列长度 
return      : true - 成功
***************************************************************************/
static bool MCP2515_WriteReg_Seq(uint8_t reg,uint8_t *data,uint8_t length)
{
    uint8_t i=0;
    if( NULL == data )return false;
    MCP2515_CS(0);
    
    Spi_SendRecData(WRITE_CMD);
    
    Spi_SendRecData(reg);
    
    for(i=0;i<length;i++)
        Spi_SendRecData(*data++);
    
    MCP2515_CS(1);
    
    return true;
}


/*************************************************************************
function    :static bool MCP2515_ReadReg_Seq(uint8_t reg,uint8_t *data,uint8_t length)
description : 读寄存器序列
input       : reg - 寄存器地址
              *data - 存放数据指针
              length - 序列长度 
return      : true - 成功
***************************************************************************/
static bool MCP2515_ReadReg_Seq(uint8_t reg,uint8_t *data,uint8_t length)
{
    uint8_t i=0;
    if( NULL == data) return false;
    MCP2515_CS(0);
    
    Spi_SendRecData(READ_CMD);
    
    Spi_SendRecData(reg);
    
    for(i=0;i<length;i++)
        *data++ = Spi_SendRecData(0xff);
    
    MCP2515_CS(1);
    
    return true;
}

/*****************************************************
function    : static bool MCP2515_Reset(void)
description : 复位 MCP2515
input       : void
return      : true - 成功
*******************************************************/
static bool MCP2515_Reset(void)
{
    MCP2515_CS(0);
    
    Spi_SendRecData(RESET_CMD);
    
    MCP2515_CS(1);
    
    return true;
}


/******************************************************
function    : static void MCP2515_Can_ConfigBaud(void)
description : 设置CAN 通信波特率 
input       : void
return      : void 
********************************************************/
static void MCP2515_Can_ConfigBaud(void)
{
   /* - - - - - - - --- - - - - - - -- - - - - - - - - - 
    f = 1/t 
    t = T(syscseg) + T(proseg) + T(ps1) + T(ps2)
     
    T(syscseg)  : 1Tq           : SJW            
    T(proseg)   : 1Tq - 8Tq     : 
    T(ps1)      : 1Tq - 8Tq
    T(ps2)      : 2Tq - 8Tq
    
    Tq = (2*(BRP+1))/Fosc
    
    Init_Value: (f = 125k )
       T(syscseg) = 1 * Tq;
       T(proseg)  = 2 * Tq;
       T(ps1)     = 3 * Tq;
       T(ps2)     = 2 * Tq;
    - - - - - - -  - -  - -  - -  - -  - - - - - - - - - -*/
    uint8_t cnf1=0,cnf2=0,cnf3=0;
    
    cnf1 = s_Osc/s_Baud/16 - 1 ;        /* SJW : 00  */
    cnf2 = 0x90;                        /* Tpro = (PRSEG0+1)Tq  Tps1 =(PHSEG1+1)Tq  */
    cnf3 = 0x02;                        /* Tps2 = (PHSEG2+1)Tq */
    
    /* send message  config */
    MCP2515_WriteReg(REG_CNF1,cnf1);
    MCP2515_WriteReg(REG_CNF2,cnf2);
    MCP2515_WriteReg(REG_CNF3,cnf3);
         
    /* receive message config */ 
    MCP2515_WriteReg(REG_RXB0CTRL,0x60);    /* 接收所有满足 MASK与Filter 的 报文 */
    MCP2515_WriteReg(REG_RXM0SIDH,0x00);    /* MASK 00 接收所有报文 */
    MCP2515_WriteReg(REG_RXM0SIDL,0x00);
    MCP2515_WriteReg(REG_RXM0EID8,0x00);
    MCP2515_WriteReg(REG_RXM0EID0,0x00);
}

/**********************************************************************
function    : static void MCP2515_Can_ConfigMsg(_MCP2515_CAN_MSG *msg)
description : 配置报文信息
input       : *msg - 报文信息
return      : void 
***********************************************************************/
static void MCP2515_Can_ConfigMsg(_MCP2515_CAN_MSG *msg)
{
    uint8_t sidh =0,sidl=0,eid8=0,eid0=0,dlc=0;
    
    if( msg->IDE )              /* 拓展帧 */
    {
        sidl = (uint8_t)(msg->ExtId>>16) | 0x08;
        eid8 = (uint8_t)(msg->ExtId>>8);
        eid0 = (uint8_t)(msg->ExtId);
    }
    else                        /* 数据帧 */
    {
        sidl = (uint8_t)(msg->StdId<<5);
        sidh = (uint8_t)(msg->StdId>>3);
    }
    
    if( msg->RTR )              /* 远程帧 */
        dlc = msg->DLC | 0x40;
    else                        /* 数据帧 */
        dlc = msg->DLC;
    
    MCP2515_BitModify(REG_TXB0CTRL,REG_BIT1|REG_BIT0,BIT_SET);        /* 设置BUFF0 为最高发送优先级 */
    MCP2515_WriteReg(REG_TXB1CTRL,0x02);     /* 设置BUFF1为 次优先级 */
    MCP2515_WriteReg(REG_TXB2CTRL,0x00);     /* 设置BUFF2为 最低优先级 */
    
    MCP2515_WriteReg(REG_TXB0SIDH,sidh);
    MCP2515_WriteReg(REG_TXB0SIDL,sidl);
    MCP2515_WriteReg(REG_TXB0EID8,eid8);
    MCP2515_WriteReg(REG_TXB0EID0,eid0);
    MCP2515_WriteReg(REG_TXB0DLC,dlc);
    MCP2515_WriteReg_Seq(REG_TXB0D0,msg->Data,msg->DLC);    /* 数据序列 */
}


/*******************************************************************
function    : static void MCP2515_Can_GetMsg(_MCP2515_CAN_MSG *msg)
description : 获取接收报文内容
input       : 存入报文到 数据结构 _MCP2515_CAN_MSG 中
return      : void
*********************************************************************/
static void MCP2515_Can_GetMsg(_MCP2515_CAN_MSG *msg)
{
    uint32_t sidh =0,sidl=0,eid8=0,eid0=0,dlc=0;
    
    if( NULL == msg) return;
    
    sidh = MCP2515_ReadReg(REG_RXB0SIDH);
    sidl = MCP2515_ReadReg(REG_RXB0SIDL);
    eid8 = MCP2515_ReadReg(REG_RXB0EID8);
    eid0 = MCP2515_ReadReg(REG_RXB0EID0);
    dlc = MCP2515_ReadReg(REG_RXB0DLC);

    msg->IDE = (sidl>>3)&0x0001;      /* 标准帧 or 拓展帧 */
    msg->RTR = (sidl>>4)&0x0001;      /* 数据帧 or 远程帧 */
    
    if( msg->IDE )
        msg->ExtId = (((sidl&0x03)<<16)|(eid8<<8)|eid0);
    else
        msg->StdId = ((sidh<<3) | (sidl>>5));
    
    msg->DLC = dlc&0x0f;
    
    MCP2515_ReadReg_Seq(REG_RXB0D0,msg->Data,msg->DLC);
}



/******************************************************
function    : static bool MCP2515_Can_Init(void)
description : MCP2515 CAN 通信初始化 
input       : void
output      : true - 成功 
******************************************************/
static bool MCP2515_Can_Init(void)
{   
    uint8_t sta;
    /* 进入 config 模式 */
    MCP2515_Reset();  
    MCP2515_BitModify(REG_CANCTRL,REG_BIT7,BIT_SET);
    /* 设置 CAN 波特率/接收报文  */
    MCP2515_Can_ConfigBaud();
	
		MCP2515_WriteReg(REG_CANINTE,0x01);   /* 使能 INT 管脚 ，使能buffter0中断*/
    
    MCP2515_WriteReg(REG_TXRTSCTRL,0x00);   /* 失能 RTS 管脚 */
    
    MCP2515_WriteReg(REG_BFPCTRL,0x00);     /* 失能 RX 管脚 */
     
    MCP2515_WriteReg(REG_CANINTF,0x00);  /* 清楚所有中断标记 */
         
    /* 进入正常工作模式 */
    MCP2515_BitModify(REG_CANCTRL,REG_BIT7|REG_BIT6|REG_BIT5,BIT_RESET);
 
    return true; 
}



/******************************************************
function    : bool MCP2515_Init(void)
description :
input       :
return      :
*******************************************************/
bool MCP2515_Init(void)
{
    Spi1_Init();
    
    MCP2515_Can_Init();
    
    return true;
}



/**************************************************************
function    : bool MCP2515_Can_Send_Msg(_MCP2515_CAN_MSG *msg)
description : MCP2515 can报文发送 
input       : *msg  报文数据 
retutn      : true - 成功
              false - 失败
****************************************************************/
bool MCP2515_Can_Send_Msg(_MCP2515_CAN_MSG *msg)
{
    uint32_t i=0;
    while( (MCP2515_ReadStaReg() & 0x04) == 0x04);  /* 等待BUFF0 空 */

    MCP2515_Can_ConfigMsg(msg);     /* 装填报文进入 BUFF0 中 */
     
    MCP2515_RTS(0);     /* 请求发送 BUFF0 */

    while( ((MCP2515_ReadReg(REG_TXB0CTRL) & 0x08) == 0x08 )&&(i++<0xffff) );    /* 等待发送完成 */
  
    if( i>0xffff)
        return false;
    
    return true;
}




/************************************************************
function    : _MCP2515_CAN_MSG *MCP_Can_Recv_Msg(void)
description : MCP2515 接收CAN 报文 
input       : void
return      : _MCP2515_CAN_MSG 数据内容
**************************************************************/
_MCP2515_CAN_MSG *MCP_Can_Recv_Msg(void)
{
    uint8_t STA;
    static _MCP2515_CAN_MSG msg;
    
    while( (MCP2515_ReadReg(REG_CANINTF) &  0x01) == 0x01)/* 等待BUFF0报文接收完成 */
    {
        MCP2515_Can_GetMsg(&msg);
        MCP2515_BitModify(REG_CANINTF,REG_BIT0,BIT_RESET);  /* 清除BUFF0接收 标志位 */
			
    }
    
    STA = MCP2515_ReadReg(REG_CANINTF);
    
    return &msg;
}







_MCP2515_CAN_MSG testmsg;
_MCP2515_CAN_MSG *testrecmsg;
void Test_Mcp2515_Can(void)
{
	 int i;
    testmsg.RTR = 0;
    testmsg.IDE = 1;
    testmsg.ExtId = 0x123;
    testmsg.StdId = 0x123;
    testmsg.DLC = 8;
    for(i=0;i<8;i++)
        testmsg.Data[i] = i+1;
    
    MCP2515_Can_Send_Msg(&testmsg);
    
    //testrecmsg = MCP_Can_Recv_Msg();//接收can信息在exit.c的外部中断中实现
}


