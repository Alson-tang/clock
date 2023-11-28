#include "st7789_hal.h"

static SPI_HandleTypeDef s_st7789_hal_spi_handle = { 0 };

static void st7789_hal_write(uint8_t *buf, uint16_t len)
{
    HAL_SPI_Transmit(&s_st7789_hal_spi_handle, buf, len, 1000);
}

static void st7789_hal_cmd_select(void)
{
    HAL_GPIO_WritePin(ST7789_HAL_DC_GPIO_PORT, ST7789_HAL_DC_PIN, GPIO_PIN_RESET);
}

static void st7789_hal_data_select(void)
{
    HAL_GPIO_WritePin(ST7789_HAL_DC_GPIO_PORT, ST7789_HAL_DC_PIN, GPIO_PIN_SET);
}

static void st7789_hal_rst_select(void)
{
    HAL_GPIO_WritePin(ST7789_HAL_RES_GPIO_PORT, ST7789_HAL_RES_PIN, GPIO_PIN_RESET);
}

static void st7789_hal_rst_unselect(void)
{
    HAL_GPIO_WritePin(ST7789_HAL_RES_GPIO_PORT, ST7789_HAL_RES_PIN, GPIO_PIN_SET);
}

void st7789_hal_init(st7789_cfg_t * st7789_cfg)
{
    s_st7789_hal_spi_handle.Instance               = ST7789_HAL_SPIx;
    s_st7789_hal_spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
    s_st7789_hal_spi_handle.Init.Direction         = SPI_DIRECTION_2LINES;
    s_st7789_hal_spi_handle.Init.CLKPhase          = SPI_PHASE_2EDGE;
    s_st7789_hal_spi_handle.Init.CLKPolarity       = SPI_POLARITY_HIGH;
    s_st7789_hal_spi_handle.Init.DataSize          = SPI_DATASIZE_8BIT;
    s_st7789_hal_spi_handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    s_st7789_hal_spi_handle.Init.TIMode            = SPI_TIMODE_DISABLE;
    s_st7789_hal_spi_handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    s_st7789_hal_spi_handle.Init.CRCPolynomial     = 7;
    s_st7789_hal_spi_handle.Init.NSS               = SPI_NSS_SOFT;
    s_st7789_hal_spi_handle.Init.Mode              = SPI_MODE_MASTER;
    HAL_SPI_Init(&s_st7789_hal_spi_handle);
    __HAL_SPI_ENABLE(&s_st7789_hal_spi_handle);

    st7789_cfg->write = st7789_hal_write;
    st7789_cfg->cmd_select = st7789_hal_cmd_select;
    st7789_cfg->data_select = st7789_hal_data_select;
    st7789_cfg->rst_select = st7789_hal_rst_select;
    st7789_cfg->rst_unselect = st7789_hal_rst_unselect;
}

void st7789_hal_spi_map_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    ST7789_HAL_SPIx_SCK_GPIO_CLK_ENABLE();
    ST7789_HAL_SPIx_SDA_GPIO_CLK_ENABLE();
    ST7789_HAL_RES_GPIO_CLK_ENABLE();
    ST7789_HAL_DC_GPIO_CLK_ENABLE();

    ST7789_HAL_SPIx_CLK_ENABLE();
    // __HAL_AFIO_REMAP_SPI1_ENABLE();
    // __HAL_AFIO_REMAP_SWJ_NOJTAG();

    GPIO_InitStruct.Pin     = ST7789_HAL_SPIx_SCK_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ST7789_HAL_SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin     = ST7789_HAL_SPIx_SDA_PIN;
    HAL_GPIO_Init(ST7789_HAL_SPIx_SDA_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin     = ST7789_HAL_RES_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_PULLUP;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(ST7789_HAL_RES_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin     = ST7789_HAL_DC_PIN;
    HAL_GPIO_Init(ST7789_HAL_DC_GPIO_PORT, &GPIO_InitStruct);
}
