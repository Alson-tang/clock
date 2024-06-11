#ifndef __UPDATE_H__
#define __UPDATE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

extern TIM_HandleTypeDef g_update_tim_handle;

void update_init(void);
void update_start(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* update.h */
