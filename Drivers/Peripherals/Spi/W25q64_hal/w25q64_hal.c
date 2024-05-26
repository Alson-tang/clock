#include "w25q64_hal.h"

static SPI_HandleTypeDef s_w25q64_hal_spi_handle = { 0 };
static w25q64_cfg_t s_st_w25q64_cfg = { 0 };

static void hal_w25q64_write(uint8_t *buf, uint16_t size)
{
    HAL_SPI_Transmit(&s_w25q64_hal_spi_handle, buf, size, 20);
}

static void hal_w25q64_read(uint8_t *buf, uint16_t size)
{
    HAL_SPI_Receive(&s_w25q64_hal_spi_handle, buf, size, 20);
}

static uint8_t hal_w25q64_read_one_byte(uint8_t send_byte)
{
    uint8_t recv_byte = 0;

    HAL_SPI_TransmitReceive(&s_w25q64_hal_spi_handle, &send_byte, &recv_byte, 1, 20);

    return recv_byte;
}

static void hal_w25q64_cs_select(void)
{
    HAL_GPIO_WritePin(W25Q64_SPIx_CS_GPIO_PORT, W25Q64_SPIx_CS_PIN, GPIO_PIN_RESET);
}

static void hal_w25q64_cs_unselect(void)
{
    HAL_GPIO_WritePin(W25Q64_SPIx_CS_GPIO_PORT, W25Q64_SPIx_CS_PIN, GPIO_PIN_SET);
}

w25q64_cfg_t* hal_w25q64_init(void)
{
    s_w25q64_hal_spi_handle.Instance               = W25Q64_SPIx;
    s_w25q64_hal_spi_handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
    s_w25q64_hal_spi_handle.Init.Direction         = SPI_DIRECTION_2LINES;
    s_w25q64_hal_spi_handle.Init.CLKPhase          = SPI_PHASE_1EDGE;
    s_w25q64_hal_spi_handle.Init.CLKPolarity       = SPI_POLARITY_LOW;
    s_w25q64_hal_spi_handle.Init.DataSize          = SPI_DATASIZE_8BIT;
    s_w25q64_hal_spi_handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
    s_w25q64_hal_spi_handle.Init.TIMode            = SPI_TIMODE_DISABLE;
    s_w25q64_hal_spi_handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
    s_w25q64_hal_spi_handle.Init.CRCPolynomial     = 7;
    s_w25q64_hal_spi_handle.Init.NSS               = SPI_NSS_SOFT;
    s_w25q64_hal_spi_handle.Init.Mode              = SPI_MODE_MASTER;
    HAL_SPI_Init(&s_w25q64_hal_spi_handle);
    __HAL_SPI_ENABLE(&s_w25q64_hal_spi_handle);

    s_st_w25q64_cfg.write = hal_w25q64_write;
    s_st_w25q64_cfg.read = hal_w25q64_read;
    s_st_w25q64_cfg.read_one_byte = hal_w25q64_read_one_byte;
    s_st_w25q64_cfg.cs_select = hal_w25q64_cs_select;
    s_st_w25q64_cfg.cs_unselect = hal_w25q64_cs_unselect;

    return &s_st_w25q64_cfg;
}

void hal_w25q64_hardware_init_cb(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct = { 0 };

    W25Q64_SPIx_SCK_GPIO_CLK_ENABLE();
    W25Q64_SPIx_MISO_GPIO_CLK_ENABLE();
    W25Q64_SPIx_MOSI_GPIO_CLK_ENABLE();
    W25Q64_SPIx_CS_GPIO_CLK_ENABLE();

    W25Q64_SPIx_CLK_ENABLE();

    GPIO_InitStruct.Pin     = W25Q64_SPIx_SCK_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull    = GPIO_NOPULL;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(W25Q64_SPIx_SCK_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin     = W25Q64_SPIx_MOSI_PIN;
    HAL_GPIO_Init(W25Q64_SPIx_MOSI_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin     = W25Q64_SPIx_MISO_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_AF_INPUT;
    HAL_GPIO_Init(W25Q64_SPIx_MISO_GPIO_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.Pin     = W25Q64_SPIx_CS_PIN;
    GPIO_InitStruct.Mode    = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull    = GPIO_PULLUP;
    GPIO_InitStruct.Speed   = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(W25Q64_SPIx_CS_GPIO_PORT, &GPIO_InitStruct);
}
