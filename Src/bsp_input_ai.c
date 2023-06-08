#include "bsp_input_ai.h"
#include "temp_pres_drv.h"

#define ADC_PORT_NUM_TOTAL 	20

uint16_t REAL_BUF[ADC_PORT_NUM_TOTAL];
uint8_t AI_Diagnose_State[6]={0};//AI1-AI6的通道状态
uint16_t AI_Diag_Threshold_High[6]={3932,3932,3932,3932,3932,3932,};//AI1-AI6的通道高阈值电压
uint16_t AI_Diag_Threshold_Low[6]={328,328,328,328,328,328};//AI1-AI6的通道高阈值电压
uint16_t AI_Diag_Hysteresis_High[6]={20,20,20,20,20,20};
uint16_t AI_Diag_Hysteresis_Low[6]={20,20,20,20,20,20};

uint8_t Flag_vcc_short[6]={0};
uint8_t Flag_gnd_short[6]={0};

extern uint16_t ADC_AVG[6];
extern uint16_t _ADC_AVG[10];
extern uint16_t ADC_FINAL[20];

void ADC_Smooth(void)
{
	int i;
	
	for(i = 0; i < 8; i++)
	{
		//REAL_BUF[i] = (uint16_t)(3.3f * (ADC_AVG[i] / 4096.0f) * 1.51f * 1000);
		ADC_FINAL[i]=_ADC_AVG[i];
	}
	ADC_FINAL[12]=_ADC_AVG[8];
	ADC_FINAL[13]=_ADC_AVG[9];
	ADC_FINAL[13]=_ADC_AVG[9];
	ADC_FINAL[8]= ADC_AVG[0];
	ADC_FINAL[9]=_ADC_AVG[1];
	ADC_FINAL[10]=_ADC_AVG[2];
	ADC_FINAL[11]=_ADC_AVG[3];
	ADC_FINAL[15]=_ADC_AVG[4];
	ADC_FINAL[19]=_ADC_AVG[5];
	
	
	REAL_BUF[0]=((4983*ADC_FINAL[0])/4095);//AI1
	REAL_BUF[1]=((4983*ADC_FINAL[1])/4095);//AI2
	REAL_BUF[2]=((4983*ADC_FINAL[2])/4095);//AI3
	REAL_BUF[3]=((4983*ADC_FINAL[3])/4095);//AI4
	REAL_BUF[4]=((4983*ADC_FINAL[4])/4095);//AI5
	REAL_BUF[5]=((4983*ADC_FINAL[5])/4095);//AI6

	REAL_BUF[6]=((2000*1.21*ADC_FINAL[6])/(20475-1.21*ADC_FINAL[6]));//RI1 待电阻实际测试
	REAL_BUF[7]=((2000*1.21*ADC_FINAL[7])/(20475-1.21*ADC_FINAL[7]));//RI2 待电阻实际测试
	REAL_BUF[8]=((2000*1.21*ADC_FINAL[8])/(20475-1.21*ADC_FINAL[8]));//RI3 待电阻实际测试
	
	REAL_BUF[9]=((4983*ADC_FINAL[9])/4095);//AO1
  REAL_BUF[10]=((3630*ADC_FINAL[10])/4095);//VREF_ADIN_3.3
	REAL_BUF[11]=((3630*ADC_FINAL[11])/4095);//VREF_3.3
	REAL_BUF[12]=((36300*ADC_FINAL[12])/4095);//BATS_M
	REAL_BUF[13]=((13200*ADC_FINAL[13])/4095);//12V_ADIN
	REAL_BUF[15]=((5544*ADC_FINAL[15])/4095);//5V_detector
	REAL_BUF[19]=((3630*ADC_FINAL[19])/4095);//3V_detector
	
}

void AI_Diag_Threshold_Set(uint8_t chan_u8,uint8_t diag_u8,uint16_t threshold_u16)
{
     if (diag_u8==1)//配置对电源短路阈值
		{	
			AI_Diag_Threshold_High[chan_u8]=threshold_u16;
		}
	 
	   if (diag_u8==0)//配置对地短路阈值
		{	
			AI_Diag_Threshold_Low[chan_u8]=threshold_u16;
		}
}

void AI_Diag_Hysteresis_Set(uint8_t chan_u8,uint8_t diag_u8,uint16_t threshold_u16)
{
		 if (diag_u8==1)//配置对电源短路滞环电压
		{	
			AI_Diag_Hysteresis_High[chan_u8]=threshold_u16;
		}
	 
	   if (diag_u8==0)//配置对地短路滞环电压
		{	
			AI_Diag_Hysteresis_Low[chan_u8]=threshold_u16;
		}
}




void AI_Diagnose_State_Get(void)
{
	uint8_t i;
	for(i=0;i<7;i++)
	{
		
		if( ADC_FINAL[i]>AI_Diag_Threshold_High[i])
		{AI_Diagnose_State[i]= 1;//对电源短路 
    Flag_vcc_short[i]=1;}		
	
	  if( ADC_FINAL[i]<AI_Diag_Threshold_Low[i])
		{AI_Diagnose_State[i]= 2;//对地短路  
		Flag_gnd_short[i]=1;}		

		
//			if(Flag_gnd_short[i]==1)
//		{
			if(ADC_FINAL[i]>(AI_Diag_Threshold_Low[i]+AI_Diag_Hysteresis_Low[i]))
		     {
					 AI_Diagnose_State[i]= 0;
				   Flag_gnd_short[i]=0; 
				 }//正常状态			
//		 }	
		
//			if(Flag_vcc_short[i]==1)
//		{
			if(ADC_FINAL[i]<(AI_Diag_Threshold_High[i]-AI_Diag_Hysteresis_High[i]))
		     {
					 AI_Diagnose_State[i]= 0;
				   Flag_vcc_short[i]=0; 
				 }//正常状态			
//		 }				 	 				 
	}
}	
