#include "exti.h"
#include "main.h"
#include "can_mcp2515.h"
//////////////////////////////////////////////////////////////////////////////////	 
							  
////////////////////////////////////////////////////////////////////////////////// 	

//�ⲿ�жϳ�ʼ��

extern _MCP2515_CAN_MSG *testrecmsg;

void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOE_CLK_ENABLE();               //����GPIOEʱ��
    //__HAL_RCC_GPIOE_CLK_ENABLE();               //����GPIOEʱ��
    
    GPIO_Initure.Pin=GPIO_PIN_2;                //PE2
    GPIO_Initure.Mode=GPIO_MODE_IT_FALLING;      //�½��ش���
    GPIO_Initure.Pull=GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE,&GPIO_Initure);
    

    
    //�ж���2-PE2
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,0);       //��ռ���ȼ�Ϊ2�������ȼ�Ϊ0,�ж���Ϊ2
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);             //ʹ���ж���15-10

}


//�жϷ����������Ҫ��������
//��HAL�������е��ⲿ�жϷ�����������ô˺���
//GPIO_Pin:�ж����ź�
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  
    switch(GPIO_Pin)
    {

        case GPIO_PIN_2:
				testrecmsg = MCP_Can_Recv_Msg();//MCP2515 CAN���ձ���
        break;
				default:break;

    }
}
