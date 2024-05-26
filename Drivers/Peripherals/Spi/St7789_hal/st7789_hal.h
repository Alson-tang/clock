#ifndef __ST7789_HAL_H__
#define __ST7789_HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "st7789.h"

#define ST7789_HAL_SPIx                             SPI1

#define ST7789_HAL_SPIx_SCK_PIN                     GPIO_PIN_5
#define ST7789_HAL_SPIx_SCK_GPIO_PORT               GPIOA
#define ST7789_HAL_SPIx_SDA_PIN                     GPIO_PIN_7
#define ST7789_HAL_SPIx_SDA_GPIO_PORT               GPIOA
#define ST7789_HAL_RES_PIN                          GPIO_PIN_0
#define ST7789_HAL_RES_GPIO_PORT                    GPIOB
#define ST7789_HAL_DC_PIN                           GPIO_PIN_1
#define ST7789_HAL_DC_GPIO_PORT                     GPIOB

#define ST7789_HAL_SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define ST7789_HAL_SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define ST7789_HAL_SPIx_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define ST7789_HAL_RES_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define ST7789_HAL_DC_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()

#define ST7789_COL                                  (240)
#define ST7789_ROW                                  (240)
#define ST7789_RST_HOLD_TIME_MS                     (20)

void hal_st7789_init(st7789_cfg_t * st7789_cfg);
void hal_st7789_hardware_init_cb(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* st7789_hal.h */
