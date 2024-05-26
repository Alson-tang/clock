#include "spi_device.h"
#include "w25q64_hal.h"
#include "w25q64.h"
#include "st7789_hal.h"
#include "st7789.h"

static st7789_cfg_t st7789_cfg = { 0 };

void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPIx) {
        hal_st7789_hardware_init_cb();
    } else {
        hal_w25q64_hardware_init_cb();
    }
}

void spi_device_init(spi_device_type device)
{
    switch (device) {
        case SPI_DEVICE_FLASH_W25Q64: {
            w25q64_cfg_t *p_w25q64_cfg = NULL;

            p_w25q64_cfg = hal_w25q64_init();
            w25q64_init(p_w25q64_cfg);

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
