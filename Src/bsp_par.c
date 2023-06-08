#include "bsp_par.h"
#include "fm24cl64b_i2c.h"
#include "crc.h"
#include "par.h"


U16 API_PAR_Init(void)
{
	U32 status = 0;
	U32 Saved_Crc = 0;
	U32 Cal_Crc = 0;
	U16 par_val = 0;

	/* byte */
	status = I2C_Fram_BufferRead(PAR_BYTES, ParMain_BYTE_BASE, Max_BYTE_NUM);
	if(status != 0)
		return 0xff;
	Cal_Crc = calc_crc32(0, PAR_BYTES, Max_BYTE_NUM);
	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParMainCheckCode_Base, 4);
	if(status != 0)
		return 0xff;
	if(Cal_Crc != Saved_Crc)
	{
//		par_val |= 1;
		SET_PAR_STATUS_BIT(par_val, 0, 1)
	}

	status = I2C_Fram_BufferRead((uint8_t *)PAR_BYTES, ParBack_BYTE_BASE, Max_BYTE_NUM);
	if(status != 0)
		return 0xff;
	Cal_Crc = calc_crc32(0, PAR_WORDS, Max_BYTE_NUM);
	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParBackCheckCode_Base, 4);
	if(status != 0)
		return 0xff;
	if(Cal_Crc != Saved_Crc)
	{
//		par_val |= 1 << 1;
		SET_PAR_STATUS_BIT(par_val, 1, 1);
	}

	/*Word*/
	status = I2C_Fram_BufferRead((uint8_t *)PAR_WORDS, ParMain_WORD_BASE, Max_WORD_NUM * 2);
	if(status != 0)
		return 0xff;
	Cal_Crc = calc_crc32(0, PAR_WORDS, Max_WORD_NUM * 2);
	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParMainCheckCode_Base + 4, 4);
	if(status != 0)
		return 0xff;
	if(Cal_Crc != Saved_Crc)
	{
//		par_val |= 1 << 2;
		SET_PAR_STATUS_BIT(par_val, 2, 1);
	}

	status = I2C_Fram_BufferRead((uint8_t *)PAR_WORDS, ParBack_WORD_BASE, Max_WORD_NUM * 2);
	if(status != 0)
		return 0xff;
	Cal_Crc = calc_crc32(0, PAR_WORDS, Max_WORD_NUM * 2);
	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParBackCheckCode_Base + 4, 4);
	if(status != 0)
		return 0xff;
	if(Cal_Crc != Saved_Crc)
	{
//		par_val |= 1 << 3;
		SET_PAR_STATUS_BIT(par_val, 3, 1);
	}

	/*Real*/
	status = I2C_Fram_BufferRead((uint8_t *)PAR_REALS, ParMain_REAL_BASE, Max_REAL_NUM * 4);
	if(status != 0)
		return 0xff;
	Cal_Crc = calc_crc32(0, PAR_REALS, Max_REAL_NUM * 4);
	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParMainCheckCode_Base + 8, 4);
	if(status != 0)
		return 0xff;
	if(Cal_Crc != Saved_Crc)
	{
//		par_val |= 1 << 4;
		SET_PAR_STATUS_BIT(par_val, 4, 1);
	}

	status = I2C_Fram_BufferRead((uint8_t *)PAR_REALS, ParBack_REAL_BASE, Max_REAL_NUM * 4);
	if(status != 0)
		return 0xff;
	Cal_Crc = calc_crc32(0, PAR_REALS, Max_REAL_NUM * 4);
	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParBackCheckCode_Base + 8, 4);
	if(status != 0)
		return 0xff;
	if(Cal_Crc != Saved_Crc)
	{
//		par_val |= 1 << 5;
		SET_PAR_STATUS_BIT(par_val, 5, 1);
	}

	return par_val;
}


U8 API_PAR_Data_Init(U8 Par_type, U8 * buffer, U16 len)
{
	U16 result = 0;
	
	if(Par_type != PAR_TYPE_BYTE || Par_type != PAR_TYPE_WORD || Par_type != PAR_TYPE_DWORD)
		return PAR_TYPE_ERROR;
	
	if(buffer == NULL)
		return PAR_BUFF_ERROR;
	
	if(Par_type == PAR_TYPE_BYTE)
	{
		result = API_PAR_SaveBytes(0, buffer, len);
	}
	else if(Par_type == PAR_TYPE_WORD)
	{
		result = API_PAR_SaveWords(0, (U16 *)buffer, len);
	}
	else if(Par_type == PAR_TYPE_DWORD)	
	{
		result = API_PAR_SaveReals(0, (REAL32 *)buffer, len);
	}

	return result;
}



U16 API_PAR_SaveBytes(U16 Address, U8 *buffer, U16 len)
{
	U32 status = 0;
	U32 crc = 0;
	int i = 0;
	if(Address > 511)
		return PAR_INDEX_ERROR;
	if(buffer == NULL)
		return PAR_BUFF_ERROR;
	if(Address + len >= 512)
		return PAR_LEN_ERROR;


	for(i = 0; i < len; i++)
	{
		PAR_BYTES[i] = buffer[i];
	}

	crc = calc_crc32(0, buffer, len);

	//写主区
	status = I2C_Fram_BufferWrite((U8 *)&crc, ParMainCheckCode_Base, 4);
	if(status != 0)
		return PAR_STATUS_ERROR;
	status = I2C_Fram_BufferWrite(buffer, ParMain_BYTE_BASE + Address, len);
	if(status != 0)
		return PAR_STATUS_ERROR;

	//写备份区
	status = I2C_Fram_BufferWrite((U8 *)&crc, ParBackCheckCode_Base, 4);
	if(status != 0)
		return PAR_STATUS_ERROR;
	status = I2C_Fram_BufferWrite(buffer, ParBack_BYTE_BASE + Address, len);
	if(status != 0)
		return PAR_STATUS_ERROR;
	
	return PAR_WRITE_TRUE;
}

U16 API_PAR_SaveWords(uint16_t Address, uint16_t * buffer, uint16_t len)
{
	U32 status = 0;
	U32 crc = 0;
	int i = 0;
	if(Address > 511)
		return PAR_INDEX_ERROR;
	if(buffer == NULL)
		return PAR_BUFF_ERROR;
	if(Address + len >= 512)
		return PAR_LEN_ERROR;


	for(i = 0; i < len; i++)
	{
		PAR_WORDS[i] = buffer[i];
	}

	crc = calc_crc32(0, PAR_WORDS, Max_WORD_NUM * 2);

	/*写主区*/
	status = I2C_Fram_BufferWrite((U8 *)&crc, ParMainCheckCode_Base + 4, 4);
	if(status != 0)
		return PAR_STATUS_ERROR;
	status = I2C_Fram_BufferWrite((U8 *)buffer, ParMain_WORD_BASE + Address * 2,  len * 2);
	if(status != 0)
		return PAR_STATUS_ERROR;

	/*写备份区*/
	status = I2C_Fram_BufferWrite((U8 *)&crc, ParBackCheckCode_Base + 4, 4);
	if(status != 0)
		return PAR_STATUS_ERROR;
	status = I2C_Fram_BufferWrite((U8 *)buffer, ParBack_WORD_BASE + Address * 2, len * 2);
	if(status != 0)
		return PAR_STATUS_ERROR;
	
	return PAR_WRITE_TRUE;
}


U16 API_PAR_SaveReals(U16 Address, REAL32 * buffer, U16 len)
{
	U32 status = 0;
	U32 crc = 0;
	int i = 0;
	if(Address > 511)
		return PAR_INDEX_ERROR;
	if(buffer == NULL)
		return PAR_BUFF_ERROR;
	if(Address + len >= 512)
		return PAR_LEN_ERROR;

	for(i = 0; i < len; i++)
	{
		PAR_REALS[i] = buffer[i];		
	}

	crc = calc_crc32(0, buffer, Max_REAL_NUM * 4);

	/*写主区*/
	status = I2C_Fram_BufferWrite((U8 *)&crc, ParMainCheckCode_Base + 8, 4);
	if(status != 0)
		return PAR_STATUS_ERROR;
	status = I2C_Fram_BufferWrite((U8 *)buffer, ParMain_REAL_BASE + Address * 4,  len * 4);
	if(status != 0)
		return PAR_STATUS_ERROR;

	/*写备份区*/
	status = I2C_Fram_BufferWrite((U8 *)&crc, ParBackCheckCode_Base + 8, 4);
	if(status != 0)
		return PAR_STATUS_ERROR;
	status = I2C_Fram_BufferWrite((U8 *)buffer, ParBack_REAL_BASE + Address * 4, len * 4);
	if(status != 0)
		return PAR_STATUS_ERROR;

	return PAR_WRITE_TRUE;
}

U16 API_PAR_GetBytes(U16 Address, U8 * buffer, U16 len)
{
	//uint32_t Saved_Crc = 0;
	//uint32_t Cal_Crc = 0;
	//uint32_t status = 0;
	int i = 0;
	
	if(buffer == NULL)//缓冲区故障？
		return PAR_BUFF_ERROR;

	if(Address > 511)
		return PAR_INDEX_ERROR;

	if(Address + len >= 512)
		return PAR_LEN_ERROR;

#if 0
	/*先读主区*/
	status = I2C_Fram_BufferRead(buffer, ParMain_BYTE_BASE + Address, len);
	if(status != 0)
		return 1;
	Cal_Crc = calc_crc32(0, buffer, len);

	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParMainCheckCode_Base, 4);
	if(status != 0)
		return 1;
	if(Saved_Crc != Cal_Crc)//读取备份区
	{
		status = I2C_Fram_BufferRead(buffer, ParBack_BYTE_BASE + Address, len);
		if(status != 0)
			return 1;
		Cal_Crc = calc_crc32(0, buffer, len);
		status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParBackCheckCode_Base, 4);
		if(status != 0)
			return 1;
		if(Saved_Crc == Cal_Crc)
			return 0;
		else
			return 1;
	}
#else
	for(i = 0; i < len; i++)
	{
		buffer[i] = PAR_BYTES[i];
	}
	
#endif
	
	return 0;
}


U16 API_PAR_GetWords(U16 Address, U16 * buffer, U16 len)
{
	//uint32_t Saved_Crc = 0;
	//uint32_t Cal_Crc = 0;
	//uint32_t status = 0;
	int i = 0;
	
	if(buffer == NULL)//缓冲区故障？
		return PAR_BUFF_ERROR;

	if(Address > 511)
		return PAR_INDEX_ERROR;

	if(Address + len >= 512)
		return PAR_LEN_ERROR;

#if 0
	/*先读主区*/
	status = I2C_Fram_BufferRead((uint8_t *)buffer, ParMain_WORD_BASE + Address, len);
	if(status != 0)
		return 1;
	Cal_Crc = calc_crc32(0, buffer, len);

	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParMainCheckCode_Base + 4, 4);
	if(status != 0)
		return 1;
	if(Saved_Crc != Cal_Crc)//读取备份区
	{
		status = I2C_Fram_BufferRead((uint8_t *)buffer, ParBack_WORD_BASE + Address, len);
		if(status != 0)
			return 1;
		Cal_Crc = calc_crc32(0, buffer, len);
		status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParBackCheckCode_Base + 4, 4);
		if(status != 0)
			return 1;		
		if(Saved_Crc == Cal_Crc)
			return 0;
		else
			return 1;
	}
#else
	for(i = 0; i < len; i++)
	{
		buffer[i] = PAR_WORDS[i];
	}

#endif
	return 0;
}

U16 API_PAR_GetReals(U16 Address, REAL32 *buffer, U16 len)
{
	//uint32_t Saved_Crc = 0;
	//uint32_t Cal_Crc = 0;
	//uint32_t status = 0;
	int i = 0;
	
	if(buffer == NULL)//缓冲区故障？
		return PAR_BUFF_ERROR;

	if(Address > 511)
		return PAR_INDEX_ERROR;

	if(Address + len >= 512)
		return PAR_LEN_ERROR;

#if 0
	/*先读主区*/
	status = I2C_Fram_BufferRead((uint8_t *)buffer, ParMain_REAL_BASE + Address, len);
	if(status != 0)
		return 1;
	Cal_Crc = calc_crc32(0, buffer, len);

	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParMainCheckCode_Base + 8, 4);
	if(status != 0)
		return 1;
	if(Saved_Crc != Cal_Crc)//读取备份区
	{
		status = I2C_Fram_BufferRead((uint8_t *)buffer, ParBack_REAL_BASE + Address, len);
		if(status != 0)
			return 1;
		Cal_Crc = calc_crc32(0, buffer, len);
		status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParBackCheckCode_Base + 8, 4);
		if(status != 0)
			return 1;		
		if(Saved_Crc == Cal_Crc)
			return 0;
		else
			return 1;
	}
#else
	for(i = 0; i < len; i++)
	{
		buffer[i] = PAR_REALS[i];
	}

#endif
	return 0;
}



