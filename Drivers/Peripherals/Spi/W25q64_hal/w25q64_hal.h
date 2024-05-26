#ifndef __W25Q64_HAL_H__
#define __W25Q64_HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "w25q64.h"

#define W25Q64_SPIx                             SPI2
#define W25Q64_SPIx_CLK_ENABLE()                __HAL_RCC_SPI2_CLK_ENABLE()
#define W25Q64_SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE()
#define W25Q64_SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define W25Q64_SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOB_CLK_ENABLE()
#define W25Q64_SPIx_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOB_CLK_ENABLE()

#define W25Q64_SPIx_SCK_PIN                     GPIO_PIN_13
#define W25Q64_SPIx_SCK_GPIO_PORT               GPIOB
#define W25Q64_SPIx_MISO_PIN                    GPIO_PIN_14
#define W25Q64_SPIx_MISO_GPIO_PORT              GPIOB
#define W25Q64_SPIx_MOSI_PIN                    GPIO_PIN_15
#define W25Q64_SPIx_MOSI_GPIO_PORT              GPIOB
#define W25Q64_SPIx_CS_PIN                      GPIO_PIN_12
#define W25Q64_SPIx_CS_GPIO_PORT                GPIOB

w25q64_cfg_t* hal_w25q64_init(void);
void hal_w25q64_hardware_init_cb(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* w25q64_hal.h */
