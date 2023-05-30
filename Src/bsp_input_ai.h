#ifndef BSP_INPUT_AI_H
#define BSP_INPUT_AI_H


enum {
	AIN1 = 0,  //0-5V,PF3
	AIN2,		//0-5V,PF4
	AIN3,		//0-5V,PF5
	AIN4,		//0-5V,PF6
	AIN5,		//0-5V,PF7
	AIN6,		//0-5V,PF8
	AIN7,		//0-10K,PF9
	AIN8,		//0-10K,PF10
	AIN9,		//0-10K,PC0
	AIN_MAX
};

enum {
	AIN_5V_1 = 0,	//0-5V,PF3
	AIN_5V_2,		//0-5V,PF4
	AIN_5V_3,		//0-5V,PF5
	AIN_5V_4,		//0-5V,PF6
	AIN_5V_5,		//0-5V,PF7
	AIN_5V_6,		//0-5V,PF8
	AIN_5V_MAX
};


void ADC_Smooth(void);


#endif



