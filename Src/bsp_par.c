#include "bsp_par.h"
#include "fm24cl64b_i2c.h"
#include "crc.h"

uint16_t byte_len;
uint16_t words_len;
uint16_t real_len;

#define PAR_TYPE_BYTE	1
#define PAR_TYPE_WORD	2
#define PAR_TYPE_DWORD	4

uint16_t API_PAR_Init(void)
{
	uint32_t status = 0;
	uint32_t Saved_Crc = 0;
	uint32_t Cal_Crc = 0;
	uint16_t par_val = 0;

	/* byte */
	status = I2C_Fram_BufferRead(PAR_BYTES, ParMain_BYTE_BASE, byte_len);
	if(status != 0)
		return 0xff;
	Cal_Crc = calc_crc32(0, PAR_BYTES, byte_len);
	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParMainCheckCode_Base, 4);
	if(status != 0)
		return 0xff;
	if(Cal_Crc != Saved_Crc)
	{
		par_val |= 1;
	}

	status = I2C_Fram_BufferRead((uint8_t *)PAR_WORDS, ParBack_BYTE_BASE, byte_len);
	if(status != 0)
		return 0xff;
	Cal_Crc = calc_crc32(0, PAR_WORDS, byte_len);
	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParBackCheckCode_Base, 4);
	if(status != 0)
		return 0xff;
	if(Cal_Crc != Saved_Crc)
	{
		par_val |= 1 << 1;
	}

	/*Word*/
	status = I2C_Fram_BufferRead((uint8_t *)PAR_WORDS, ParMain_WORD_BASE, words_len);
	if(status != 0)
		return 0xff;
	Cal_Crc = calc_crc32(0, PAR_WORDS, words_len);
	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParMainCheckCode_Base + 4, 4);
	if(status != 0)
		return 0xff;
	if(Cal_Crc != Saved_Crc)
	{
		par_val |= 1 << 2;
	}

	status = I2C_Fram_BufferRead((uint8_t *)PAR_WORDS, ParBack_WORD_BASE, words_len);
	if(status != 0)
		return 0xff;
	Cal_Crc = calc_crc32(0, PAR_WORDS, words_len);
	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParBackCheckCode_Base + 4, 4);
	if(status != 0)
		return 0xff;
	if(Cal_Crc != Saved_Crc)
	{
		par_val |= 1 << 3;
	}

	/*Real*/
	status = I2C_Fram_BufferRead((uint8_t *)PAR_REALS, ParMain_REAL_BASE, real_len);
	if(status != 0)
		return 0xff;
	Cal_Crc = calc_crc32(0, PAR_REALS, real_len);
	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParMainCheckCode_Base + 8, 4);
	if(status != 0)
		return 0xff;
	if(Cal_Crc != Saved_Crc)
	{
		par_val |= 1 << 4;
	}

	status = I2C_Fram_BufferRead((uint8_t *)PAR_REALS, ParBack_REAL_BASE, real_len);
	if(status != 0)
		return 0xff;
	Cal_Crc = calc_crc32(0, PAR_REALS, real_len);
	status = I2C_Fram_BufferRead((uint8_t *)&Saved_Crc, ParBackCheckCode_Base + 8, 4);
	if(status != 0)
		return 0xff;
	if(Cal_Crc != Saved_Crc)
	{
		par_val |= 1 << 5;
	}

	return par_val;
}

uint8_t API_PAR_Data_Init(uint8_t Par_type, uint8_t * buffer, uint16_t len)
{
	uint16_t result = 0;
	
	if(Par_type != PAR_TYPE_BYTE || Par_type != PAR_TYPE_WORD || Par_type != PAR_TYPE_DWORD)
		return 0xff;
	
	if(buffer == NULL)
		return 3;
	
	if(Par_type == PAR_TYPE_BYTE)
	{
		result = API_PAR_SaveBytes(0, buffer, len);
	}
	else if(Par_type == PAR_TYPE_WORD)
	{
		result = API_PAR_SaveWords(0, (uint16_t *)buffer, len);
	}
	else if(Par_type == PAR_TYPE_DWORD)	
	{
		result = API_PAR_SaveReals(0, (float *)buffer, len);
	}

	return result;
}



uint16_t API_PAR_SaveBytes(uint16_t Address, uint8_t *buffer, uint16_t len)
{
	uint32_t status = 0;
	uint32_t crc = 0;
	if(Address > 511)
		return 2;
	if(buffer == NULL)
		return 3;
	if(Address + len >= 512)
		return 4;

	
	crc = calc_crc32(0, buffer, len);

	/*写主区*/
	status = I2C_Fram_BufferWrite((uint8_t *)&crc, ParMainCheckCode_Base, 4);
	if(status != 0)
		return 5;
	status = I2C_Fram_BufferWrite(buffer, ParMain_BYTE_BASE + Address, len);
	if(status != 0)
		return 5;

	/*写备份区*/
	status = I2C_Fram_BufferWrite((uint8_t *)&crc, ParBackCheckCode_Base, 4);
	if(status != 0)
		return 5;
	status = I2C_Fram_BufferWrite(buffer, ParBack_BYTE_BASE + Address, len);
	if(status != 0)
		return 5;

	return 0;
}

uint16_t API_PAR_SaveWords(uint16_t Address, uint16_t * buffer, uint16_t len)
{
	uint32_t status = 0;
	uint32_t crc = 0;
	if(Address > 511)
		return 2;
	if(buffer == NULL)
		return 3;
	if(Address + len >= 512)
		return 4;

	crc = calc_crc32(0, buffer, len * 2);

	/*写主区*/
	status = I2C_Fram_BufferWrite((uint8_t *)&crc, ParMainCheckCode_Base + 4, 4);
	if(status != 0)
		return 5;
	status = I2C_Fram_BufferWrite((uint8_t *)buffer, ParMain_WORD_BASE + Address * 2,  len * 2);
	if(status != 0)
		return 5;

	/*写备份区*/
	status = I2C_Fram_BufferWrite((uint8_t *)&crc, ParBackCheckCode_Base + 4, 4);
	if(status != 0)
		return 5;
	status = I2C_Fram_BufferWrite((uint8_t *)buffer, ParBack_WORD_BASE + Address * 2, len * 2);
	if(status != 0)
		return 5;

	return 0;
}


uint16_t API_PAR_SaveReals(uint16_t Address, float * buffer, uint16_t len)
{
	uint32_t status = 0;
	uint32_t crc = 0;
	if(Address > 511)
		return 2;
	if(buffer == NULL)
		return 3;
	if(Address + len >= 512)
		return 4;

	crc = calc_crc32(0, buffer, len * 2);

	/*写主区*/
	status = I2C_Fram_BufferWrite((uint8_t *)&crc, ParMainCheckCode_Base + 8, 4);
	if(status != 0)
		return 5;
	status = I2C_Fram_BufferWrite((uint8_t *)buffer, ParMain_REAL_BASE + Address * 4,  len * 4);
	if(status != 0)
		return 5;

	/*写备份区*/
	status = I2C_Fram_BufferWrite((uint8_t *)&crc, ParBackCheckCode_Base + 8, 4);
	if(status != 0)
		return 5;
	status = I2C_Fram_BufferWrite((uint8_t *)buffer, ParBack_REAL_BASE + Address * 4, len * 4);
	if(status != 0)
		return 5;

	return 0;
}

uint16_t API_PAR_GetBytes(uint16_t Address, uint8_t * buffer, uint16_t len)
{
	uint32_t Saved_Crc = 0;
	uint32_t Cal_Crc = 0;
	uint32_t status = 0;
	
	if(buffer == NULL)//缓冲区故障？
		return 3;

	if(Address > 511)
		return 2;

	if(Address + len >= 512)
		return 4;

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
	
	return 0;
}


uint16_t API_PAR_GetWords(uint16_t Address, uint16_t * buffer, uint16_t len)
{
	uint32_t Saved_Crc = 0;
	uint32_t Cal_Crc = 0;
	uint32_t status = 0;
	
	if(buffer == NULL)//缓冲区故障？
		return 3;

	if(Address > 511)
		return 2;

	if(Address + len >= 512)
		return 4;

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
	
	return 0;
}

uint16_t API_PAR_GetReals(uint16_t Address, float *buffer, uint16_t len)
{
	uint32_t Saved_Crc = 0;
	uint32_t Cal_Crc = 0;
	uint32_t status = 0;
	
	if(buffer == NULL)//缓冲区故障？
		return 3;

	if(Address > 511)
		return 2;

	if(Address + len >= 512)
		return 4;

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
	
	return 0;
}



