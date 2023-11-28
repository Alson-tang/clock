#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

typedef enum
{
  KEY_RELEASE = 0u,
  KEY_PRESSED
} key_status;

#define KEY_GPIO_PIN                GPIO_PIN_0
#define KEY_GPIO_PORT               GPIOA
#define KEY_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()

#define KEY_IT_PREEMPT_PRIORITY     (14)
#define KEY_IT_SUB_PRIORITY         (0)

void key_gpio_init(void);
key_status key_status_get(void);
void key_status_set(key_status status);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* key.h */


