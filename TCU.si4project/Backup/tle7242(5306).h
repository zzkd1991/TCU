#ifndef __TLE7242_H__
#define __TLE7242_H__

#include <stdint.h>

#define TLE7242_ENABLE_PORT	GPIOB
#define TLE7242_ENABLE_PIN	GPIO_PIN_4
#define TLE7242_ENABLE_ENA()		HAL_GPIO_WritePin(TLE7242_ENABLE_PORT, TLE7242_ENABLE_PIN, GPIO_PIN_RESET)
#define TLE7242_ENABLE_DIS()		HAL_GPIO_WritePin(TLE7242_ENABLE_PORT, TLE7242_ENABLE_PIN, GPIO_PIN_SET)

#define TLE7242_RESET_PORT	GPIOB
#define TLE7242_RESET_PIN	GPIO_PIN_5
#define TLE7242_RESET_ENA()		HAL_GPIO_WritePin(TLE7242_RESET_PORT, TLE7242_RESET_PIN, GPIO_PIN_RESET)
#define TLE7242_RESET_DIS()		HAL_GPIO_WritePin(TLE7242_RESET_PORT, TLE7242_RESET_PIN, GPIO_PIN_SET)


//Message0
typedef union
{
	struct
	{
		uint32_t reserve0				:8;//bit0-bit7
		uint32_t Version_Number		:8;//bit8-bit15
		uint32_t Manuf_ID			:8;//bit16-bit23
		uint32_t reserve1			:2;//bit24-bit25
		uint32_t MSG_ID				:5;//bit26-bit30
		uint32_t RW					:1; //bit31
	}B;
	uint32_t U;
}tag_TLE_IC_Version;

//Message1
typedef union
{
	struct
	{
		uint32_t	PWM_Divider : 14;//bit0-bit13
		uint32_t 	reserve0	:10;//bit14-bit23
		uint32_t	CH			:2;//bit24-bit25
		uint32_t	MSG_ID		:5;//bit26-bit30
		uint32_t	RW			:1;//bit31
	}B;
	uint32_t U;
}tag_TLE_Main_Period;

//Message2
typedef union
{
	struct
	{
		uint32_t Phase_Sync_Offset	:5;//bit0-bit4
		uint32_t reserve0			:19;//bit5-bit23
		uint32_t CH					:2;//bit24-bit25
		uint32_t MSG_ID				:5;//bit26-bit30
		uint32_t RW					:1;//bit31
	}B;
	uint32_t U;
}tag_TLE_PWM_Offset;


//Message3
typedef union
{
	struct
	{
		uint32_t Current_Set_Point	:11;//bit0-bit10
		uint32_t Dither_Enable		:1;//bit11
		uint32_t Step_Size			:10;//bit12-bit21
		uint32_t ON_OFF				:1;//bit22
		uint32_t EN					:1;//bit23
		uint32_t CH					:2;//bit24-bit25
		uint32_t MSG_ID				:5;//bit26-bit30
		uint32_t RW					:1;//bit31
	}B;
	uint32_t U;
}tag_TLE_Current_Set;


//Message4
typedef union
{
	struct
	{
		uint32_t	Dither_Steps	:6;//bit0-bit5
		uint32_t	reserve0		:18;//bit6-bit23
		uint32_t	CH				:2;//bit24-bit30
		uint32_t 	MSG_ID			:5;//bit26-bit30
		uint32_t	RW				:1;//bit31
	}B;	
	uint32_t U;
}tag_Dither_Period;


//Message5
typedef union
{
	struct
	{
		uint32_t KI			:12;//bit0-bit11
		uint32_t KP			:12;//bit12-bit23
		uint32_t CH			:2;//bit24-bit25
		uint32_t MSG_ID		:5;//bit26-bit30
		uint32_t RW			:1;//bit31
	}B;
	uint32_t U;
}tag_KP_KI;

//Message6
typedef union
{
	struct
	{
		uint32_t Integrator_preload		:12;//bit0-bit11
		uint32_t Transient_Threshold	:11;//bit12-bit22
		uint32_t reserve0				:1;//bit23
		uint32_t CH						:2;//bit24-bit25
		uint32_t MSG_ID					:5;//bit26-bit30
		uint32_t RW						:1;//bit31
	}B;
	uint32_t U;
}tag_Dynamic_Threshold;

//Message7
typedef union
{
	struct
	{
		uint32_t	reserve0							:11;	//bit0-bit10
		uint32_t	AZ_Disable						:1;		//bit11
		uint32_t	DIAG_TMR							:2;		//bit12-bit13			
		uint32_t	FME							:1;		//bit14
		uint32_t	FMR							:1;		//bit15
		uint32_t	FM3							:1;		//bit16
		uint32_t	FM2							:1;		//bit17
		uint32_t	FM1							:1;		//bit18
		uint32_t	FM0							:1;		//bit19
		uint32_t	CM3							:1;		//bit20
		uint32_t	CM2							:1;		//bit21
		uint32_t	CM1							:1;		//bit22
		uint32_t	CM0							:1;		//bit23
		uint32_t	reserve1				:2;		//bit24-bit25
		uint32_t	MSG_ID					:5;		//bit26-bit30
		uint32_t	RW							:1;		//bit31			
	}B;
	uint32_t U;
}tag_Fault_Mask;

//Message8
typedef union
{
	struct
	{
		uint32_t	SB_RETRY3				:4;	//bit0-bit3
		uint32_t	SB3							:2;		//bit4-bit5
		uint32_t	SB_RETRY2				:4;		//bit6-bit9			
		uint32_t	SB2							:2;		//bit10-bit11	
		uint32_t	SB_RETRY1				:4;		//bit12-bit15
		uint32_t	SB1							:2;		//bit16-bit17
		uint32_t	SB_RETRY0				:4;		//bit18-bit21
		uint32_t	SB0							:2;		//bit22-bit23
		uint32_t	reserve0				:2;		//bit24-bit25
		uint32_t	MSG_ID					:5;		//bit26-bit30
		uint32_t	RW							:1;		//bit31			
	}B;
	uint32_t U;
}tag_Diagnostic_Cfg;

//Message9
typedef union
{
	struct
	{
		uint32_t	RBL							:1;		//bit0
		uint32_t	ENL							:1;		//bit1
		uint32_t	OL_ON3					:1;		//bit2		
		uint32_t	OL_OFF3					:1;		//bit3
		uint32_t	SB_TST3					:1;		//bit4
		uint32_t	SB3							:1;		//bit5
		uint32_t	OFF_TST3				:1;		//bit6
		uint32_t	SG3							:1;		//bit7
		uint32_t	OL_ON2					:1;		//bit8	
		uint32_t	OL_OFF2					:1;		//bit9
		uint32_t	SB_TST2					:1;		//bit10
		uint32_t	SB2							:1;		//bit11
		uint32_t	OFF_TST2				:1;		//bit12
		uint32_t	SG2							:1;		//bit13
		uint32_t	OL_ON1					:1;		//bit14		
		uint32_t	OL_OFF1					:1;		//bit15
		uint32_t	SB_TST1					:1;		//bit16
		uint32_t	SB1							:1;		//bit17
		uint32_t	OFF_TST1				:1;		//bit18
		uint32_t	SG1							:1;		//bit19
		uint32_t	OL_ON0					:1;		//bit20	
		uint32_t	OL_OFF0					:1;		//bit21
		uint32_t	SB_TST0					:1;		//bit22
		uint32_t	SB0							:1;		//bit23
		uint32_t	OFF_TST0				:1;		//bit24
		uint32_t	SG0							:1;		//bit25
		uint32_t	MSG_ID					:5;		//bit26-bit30
		uint32_t	RW							:1;		//bit31			
	}B;
	uint32_t U;
}tag_Diagnostic_Read;


//Message10
typedef union
{
	struct
	{
		uint32_t Current_Read		:14;//bit0-bit13
		uint32_t	reserve0		:10;//bit14-bit23
		uint32_t	CH				:2;//bit24-bit25
		uint32_t	MSG_ID			:5;//bit26-bit30
		uint32_t	RW				:1;//bit31
	}B;
	uint32_t U;
}tag_Current_Read;


//Message11
typedef union
{
	struct
	{
		uint32_t	Autozero_off		:6;	//bit0-bit5
		uint32_t	Autozero_on			:6;	//bit6-bit11
		uint32_t	AZoff						:1;		//bit12
		uint32_t	AZon						:1;		//bit13
		uint32_t	PSL							:1;		//bit14
		uint32_t	OVL							:1;		//bit15		
		uint32_t	reserve0				:8;	//bit16-bit23
		uint32_t	CH							:2;		//bit24-bit25
		uint32_t	MSG_ID					:5;		//bit26-bit30
		uint32_t	RW							:1;		//bit31			
	}B;
	uint32_t U;
}tag_Autozero_Read;

//Message12
typedef union
{
	struct
	{
		uint32_t	Duty_Cycle			:19;		//bit0-bit18
		uint32_t	reserve0				:5;		//bit19-bit23
		uint32_t	CH							:2;		//bit24-bit25
		uint32_t	MSG_ID					:5;		//bit26-bit30
		uint32_t	reserve1				:1;		//bit31				
	}B;
	uint32_t U;
}tag_DutyCycle_Read;


typedef  struct
{
	tag_TLE_IC_Version				IC_Version;
	tag_TLE_Main_Period       Main_Period;
	tag_TLE_PWM_Offset       	PWM_Offset;
	tag_TLE_Current_Set				Current_Set;
	tag_Dither_Period					Dither_Period;
	tag_KP_KI									KP_KI;
	tag_Dynamic_Threshold			Dynamic_Threshold;
	tag_Fault_Mask						Fault_Mask;
	tag_Diagnostic_Cfg				Diagnostic_Cfg;
	tag_Diagnostic_Read				Diagnostic_Read;
	tag_Current_Read					Current_Read;
	tag_Autozero_Read					Autozero_Read;
	tag_DutyCycle_Read				DutyCycle_Read;
	
}tag_TLE_Register;

typedef struct
{
	uint8_t 	record_mode;
	uint16_t	record_freq;
	uint16_t	record_PWM_Divider;
	uint16_t 	record_current;
	uint8_t 	record_dither_en;
	uint8_t 	record_dither_steps;
	uint16_t 	record_dither_freq;
	uint16_t 	record_dither_amp;
	uint8_t 	record_on_off;
	uint16_t 	record_kp_value;
	uint16_t 	record_ki_value;

}tag_TLE_CONFIG_RECORD;

typedef enum
{
	TLE_CHIP_1 = 0,
	TLE_CHIP_2,
	TLE_CHIP_3,
	TLE_CHIP_MAX,
}TLE7242_CHIP;

typedef enum
{
	TLE_MSG0_ID = 0,
	TLE_MSG1_ID,
	TLE_MSG2_ID,
	TLE_MSG3_ID,
	TLE_MSG4_ID,
	TLE_MSG5_ID,
	TLE_MSG6_ID,
	TLE_MSG7_ID,
	TLE_MSG8_ID,
	TLE_MSG9_ID,
	TLE_MSG10_ID,
	TLE_MSG11_ID,
	TLE_MSG12_ID,

	TLE_MSG_MAX,
}TLE7242_MSG;


typedef enum
{
	TLE_MODE_CONST_CURRENT = 0,
	TLE_MODE_ON_OFF,
	
	TLE_MODE_MAX,
}TLE7242_MODE;

extern void TLE_Write_Register_Data(uint8_t channel_u8,uint8_t mess_id_u8,uint32_t SPI_Data_u32);
extern uint32_t TLE_Read_Register_Data(uint8_t channel_u8,uint8_t mess_id_u8);
extern void TLE_Chip_Output_Enable(void);
extern void TLE_Chip_Output_Disable(void);
extern void TLE_Power_On_Init(void);


extern uint8_t TLE_Channel_Pwm_Freq_Set(uint8_t channel_u8,uint16_t freq_u16);//MSID1
extern uint8_t TLE_Channel_Time_Offset_Set(uint8_t channel_u8,uint16_t offset_u16);//MSID2
extern uint8_t TLE_Channel_Constant_Current_Set(uint8_t channel_u8,uint16_t current_u16);//MSID3
extern uint8_t TLE_Channel_Dither_Enable(uint8_t channel_u8,uint8_t dither_en,uint16_t dither_step);//MSID3
extern uint8_t TLE_Channel_OnOff_Operate(uint8_t channel_u8,uint8_t on_off_u8);//MSID3
extern uint8_t TLE_Channel_Dither_Freq_Set(uint8_t channel_u8,uint16_t dither_freq_u16);//MSID4
extern uint8_t TLE_Channel_KP_KI_Set(uint8_t channel_u8,uint16_t kp_u16,uint16_t ki_u16);//MSID5
extern uint8_t TLE_Channel_Dynamic_Threshold_Set(uint8_t channel_u8,uint16_t integrator_preload,uint16_t transient_threshold);//MSID6
extern uint8_t TLE_Channel_Mode_Config(uint8_t channel_u8,uint8_t mode_u8);//MSID7
extern uint8_t TLE_Channel_Diagnostic_Read(uint8_t channel_u8);//MSID9
extern uint16_t TLE_Channel_Current_Read(uint8_t channel_u8);//MSID10
extern uint32_t TLE_Channel_Autozero_Read(uint8_t channel_u8);//MSID11
extern uint16_t TLE_Channel_Duty_Read(uint8_t channel_u8);//MSID12
extern void TLE_ALL_Register_Period_Read(uint8_t channel_u8,uint16_t period_u16);

void TLE7242_GPIO_Init(void);
#endif

