#ifndef __SPI_DEVICE_H__
#define __SPI_DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

typedef enum
{
   SPI_DEVICE_FLASH_W25Q128,
   SPI_DEVICE_TFTLCD_ST7789,
   SPI_DEVICE_MAX
} spi_device_type;

void spi_device_init(spi_device_type device);

#define SPIx                             SPI1
#define SPIx_CLK_ENABLE()                __HAL_RCC_SPI1_CLK_ENABLE()
#define SPIx_SCK_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MISO_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_MOSI_GPIO_CLK_ENABLE()      __HAL_RCC_GPIOA_CLK_ENABLE()
#define SPIx_CS_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOA_CLK_ENABLE()

#define SPIx_SCK_PIN                     GPIO_PIN_5
#define SPIx_SCK_GPIO_PORT               GPIOA
#define SPIx_MISO_PIN                    GPIO_PIN_6
#define SPIx_MISO_GPIO_PORT              GPIOA
#define SPIx_MOSI_PIN                    GPIO_PIN_7
#define SPIx_MOSI_GPIO_PORT              GPIOA
#define SPIx_CS_PIN                      GPIO_PIN_4
#define SPIx_CS_GPIO_PORT                GPIOA

// void spi_init(void);
// void spi_write(uint8_t *buf, uint16_t size);
// uint8_t spi_read_one_byte(uint8_t send_byte);
// void spi_read(uint8_t *buf, uint16_t size);
// void spi_cs_select(void);
// void spi_cs_unselect(void);

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* spi_device.h */
