#include "bsp_iwdg.h"

IWDG_HandleTypeDef IWDG_Handle;

/*设置 IWDG超时时间
* Tout = prv/40 * rlv (s)

*/

void IWDG_Config(uint8_t prv, uint16_t rlv)
{
	IWDG_Handle.Instance = IWDG;

	IWDG_Handle.Init.Prescaler = prv;

	IWDG_Handle.Init.Reload = rlv;

	HAL_IWDG_Init(&IWDG_Handle);

	__HAL_IWDG_START(&IWDG_Handle);
}

void IWDG_Feed(void)
{
	HAL_IWDG_Refresh(&IWDG_Handle);
}

