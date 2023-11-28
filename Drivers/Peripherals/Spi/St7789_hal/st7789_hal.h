#ifndef __ST7789_HAL_H__
#define __ST7789_HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "st7789.h"

#define ST7789_HAL_SPIx                             SPI2

#define ST7789_HAL_SPIx_SCK_PIN                     GPIO_PIN_13
#define ST7789_HAL_SPIx_SCK_GPIO_PORT               GPIOB
#define ST7789_HAL_SPIx_SDA_PIN                     GPIO_PIN_15
#define ST7789_HAL_SPIx_SDA_GPIO_PORT               GPIOB
#define ST7789_HAL_RES_PIN                          GPIO_PIN_14
#define ST7789_HAL_RES_GPIO_PORT                    GPIOB
#define ST7789_HAL_DC_PIN                           GPIO_PIN_12
#define ST7789_HAL_DC_GPIO_PORT                     GPIOB

#define ST7789_HAL_SPIx_CLK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()
#define ST7789_HAL_SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define ST7789_HAL_SPIx_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define ST7789_HAL_RES_GPIO_CLK_ENABLE()            __HAL_RCC_GPIOB_CLK_ENABLE()
#define ST7789_HAL_DC_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE()

#define ST7789_COL                                  (240)
#define ST7789_ROW                                  (240)
#define ST7789_RST_HOLD_TIME_MS                     (20)

void st7789_hal_init(st7789_cfg_t * st7789_cfg);
void st7789_hal_spi_map_init(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* st7789_hal.h */
