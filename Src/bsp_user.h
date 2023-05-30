#ifndef __BSP_USE_H
#define __BSP_USE_H

#ifndef __cplusplus
#if __cplusplus
extern "C"
{
#endif
#endif

#include <stdint.h>

void bsp_application_task_loop(void);
void bsp_application_task_1ms(void);
void bsp_application_task_5ms(void);
void bsp_application_task_10ms(void);
void bsp_application_task_20ms(void);
void bsp_application_task_50ms(void);
void bsp_application_task_100ms(void);
void bsp_application_task_500ms(void);
void bsp_application_task_1000ms(void);
void bsp_application_task_2000ms(void);


#ifndef __cplusplus
#if __cplusplus
}
#endif
#endif

	
#endif


