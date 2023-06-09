#include "exti.h"
#include "main.h"
#include "can_mcp2515.h"
//////////////////////////////////////////////////////////////////////////////////	 
							  
////////////////////////////////////////////////////////////////////////////////// 	

//外部中断初始化

extern _MCP2515_CAN_MSG *testrecmsg;

void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();               //开启GPIOE时钟
    //__HAL_RCC_GPIOE_CLK_ENABLE();               //开启GPIOE时钟
    
    GPIO_Initure.Pin=GPIO_PIN_2;                //PE2
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;      //下降沿触发
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    

    
    //中断线2-PE2
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,0);       //抢占优先级为2，子优先级为0,中断线为2
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //使能中断线15-10

}


//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  
    switch(GPIO_Pin)
    {

        case GPIO_PIN_2:
				testrecmsg = MCP_Can_Recv_Msg();//MCP2515 CAN接收报文
        break;
				default:break;

    }
}
