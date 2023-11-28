#ifndef __LED_H__
#define __LED_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define LED_GPIO_PIN            GPIO_PIN_0
#define LED_GPIO_PORT           GPIOB
#define LED_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()

void led_gpio_init(void);
void led_gpio_on(void);
void led_gpio_of(void);
void led_gpio_trigger(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* led.h */

