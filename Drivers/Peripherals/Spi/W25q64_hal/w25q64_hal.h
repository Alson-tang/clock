#ifndef __W25Q64_HAL_H__
#define __W25Q64_HAL_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "w25q64.h"

#define W25Q64_SPIx                             SPI1
#define W25Q64_SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define W25Q64_SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define W25Q64_SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define W25Q64_SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define W25Q64_SPIx_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()

#define W25Q64_SPIx_SCK_PIN                     GPIO_PIN_5
#define W25Q64_SPIx_SCK_GPIO_PORT               GPIOA
#define W25Q64_SPIx_MISO_PIN                    GPIO_PIN_6
#define W25Q64_SPIx_MISO_GPIO_PORT              GPIOA
#define W25Q64_SPIx_MOSI_PIN                    GPIO_PIN_7
#define W25Q64_SPIx_MOSI_GPIO_PORT              GPIOA
#define W25Q64_SPIx_CS_PIN                      GPIO_PIN_4
#define W25Q64_SPIx_CS_GPIO_PORT                GPIOA

void w25q64_hal_init(w25q64_cfg_t * w25q64_cfg);
void w25q64_hal_spi_map_init(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* w25q64_hal.h */
