#include "w25q64_hal.h"

static SPI_HandleTypeDef s_w25q64_hal_spi_handle = { 0 };

static void w25q64_hal_write(uint8_t *buf, uint16_t size)
{
    HAL_SPI_Transmit(&s_w25q64_hal_spi_handle, buf, size, 20);
}

static void w25q64_hal_read(uint8_t *buf, uint16_t size)
{
    HAL_SPI_Receive(&s_w25q64_hal_spi_handle, buf, size, 20);
}

static uint8_t w25q64_hal_read_one_byte(uint8_t send_byte)
{
    uint8_t recv_byte = 0;

    HAL_SPI_TransmitReceive(&s_w25q64_hal_spi_handle, &send_byte, &recv_byte, 1, 20);

    return recv_byte;
}

static void w25q64_hal_cs_select(void)
{
    HAL_GPIO_WritePin(W25Q64_SPIx_CS_GPIO_PORT, W25Q64_SPIx_CS_PIN, GPIO_PIN_RESET);
}

static void w25q64_hal_cs_unselect(void)
{
    HAL_GPIO_WritePin(W25Q64_SPIx_CS_GPIO_PORT, W25Q64_SPIx_CS_PIN, GPIO_PIN_SET);
}

void w25q64_hal_init(w25q64_cfg_t * w25q64_cfg)
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

    w25q64_cfg->write = w25q64_hal_write;
    w25q64_cfg->read = w25q64_hal_read;
    w25q64_cfg->read_one_byte = w25q64_hal_read_one_byte;
    w25q64_cfg->cs_select = w25q64_hal_cs_select;
    w25q64_cfg->cs_unselect = w25q64_hal_cs_unselect;
}

void w25q64_hal_spi_map_init(void)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    W25Q64_SPIx_SCK_GPIO_CLK_ENABLE();
    W25Q64_SPIx_MISO_GPIO_CLK_ENABLE();
    W25Q64_SPIx_MOSI_GPIO_CLK_ENABLE();
    W25Q64_SPIx_CS_GPIO_CLK_ENABLE();

    W25Q64_SPIx_CLK_ENABLE();
    // __HAL_AFIO_REMAP_SPI1_ENABLE();
    // __HAL_AFIO_REMAP_SWJ_NOJTAG(); 

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
