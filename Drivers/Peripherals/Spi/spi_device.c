#include "spi_device.h"
#include "w25q64_hal.h"
#include "w25q64.h"
#include "st7789_hal.h"
#include "st7789.h"

static w25q64_cfg_t s_w25q64_cfg = { 0 };
static st7789_cfg_t st7789_cfg = { 0 };

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    // if (hspi->Instance == SPIx) {
    //     w25q64_hal_spi_map_init();
    // } else {
        hal_st7789_hardware_init_cb();
    // }
}

void spi_device_init(spi_device_type device)
{
    switch (device)
    {
        case SPI_DEVICE_FLASH_W25Q64: {
            w25q64_hal_init(&s_w25q64_cfg);
            w25q64_init(&s_w25q64_cfg);
            break;
        }

        case SPI_DEVICE_TFTLCD_ST7789: {
            st7789_cfg.column = ST7789_COL;
            st7789_cfg.row = ST7789_ROW;
            st7789_cfg.rst_hold_time_ms = ST7789_RST_HOLD_TIME_MS;
            hal_st7789_init(&st7789_cfg);
            st7789_init(&st7789_cfg);
            st7789_screen_clear(COLOR_WHITE);
            break;
        }

        default:
            break;
    }
}
