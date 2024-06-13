#ifndef __ESP32_HARDWARE_H__
#define __ESP32_HARDWARE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define ESP32_HARDWARE_PIN                      GPIO_PIN_3
#define ESP32_HARDWARE_PORT                     GPIOC
#define ESP32_HARDWARE_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()

void esp32_hardware_init(void);
void esp32_hardware_power_on(void);
void esp32_hardware_power_off(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* esp32_hardware.h */
