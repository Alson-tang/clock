#include "main.h"

#define ESP32_CMD_BUF_INIT(p, size)             \
    do {                                        \
        if (p) {                                \
            memset((void*)p, 0, size);          \
        }                                       \
    } while(0)

static uint8_t s_arr_cmd[ESP32_CMD_BUF_MAX_LEN] = { 0 };
static uint8_t s_arr_rx_buf1[ESP32_RX_BUF_MAX_LEN] = { 0 };
static uint8_t s_arr_rx_buf2[ESP32_RX_BUF_MAX_LEN] = { 0 };

static esp32_cmd_cfg_t s_arr_cmd_cfg[] = {
    { ESP32_CMD_AT, "AT", "OK", 1000, NULL },
};

static esp32_rx_dma_buf_cfg_t s_st_rx_dma_buf_cfg = {
    ESP32_RX_DMA_BUF_1,
    s_arr_rx_buf1,
    s_arr_rx_buf2,
    ESP32_RX_BUF_MAX_LEN,
};

static esp32_info_t s_st_esp32_info = {
    s_arr_cmd_cfg,
    &s_st_rx_dma_buf_cfg,
};

UART_HandleTypeDef s_st_esp32_uart_handle = { 0 };
DMA_HandleTypeDef s_st_esp32_dma_handle = { 0 };

static void esp32_uart_init(void)
{
    s_st_esp32_uart_handle.Instance         = ESP32_UART;
    s_st_esp32_uart_handle.Init.BaudRate    = ESP32_UART_BAUDRATE;
    s_st_esp32_uart_handle.Init.WordLength  = UART_WORDLENGTH_8B;
    s_st_esp32_uart_handle.Init.StopBits    = UART_STOPBITS_1;
    s_st_esp32_uart_handle.Init.Parity      = UART_PARITY_NONE;
    s_st_esp32_uart_handle.Init.HwFlowCtl   = UART_HWCONTROL_NONE;
    s_st_esp32_uart_handle.Init.Mode        = UART_MODE_TX_RX;

    if(HAL_UART_DeInit(&s_st_esp32_uart_handle) != HAL_OK) {
        Error_Handler();
    }

    if(HAL_UART_Init(&s_st_esp32_uart_handle) != HAL_OK) {
        Error_Handler();
    }

    __HAL_UART_ENABLE_IT(&s_st_esp32_uart_handle, UART_IT_IDLE);
}

static void esp32_dma_init(void)
{
    __HAL_RCC_DMA1_CLK_ENABLE();                                                           /* DMA1 时钟使能 */

    s_st_esp32_dma_handle.Instance = DMA1_Channel6;                                        /* USART2_RX 使用的 DMA 通道为: DMA1_Channel6 */
    s_st_esp32_dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;                           /* 外设到存储器模式 */
    s_st_esp32_dma_handle.Init.PeriphInc = DMA_PINC_DISABLE;                               /* 外设非增量模式 */
    s_st_esp32_dma_handle.Init.MemInc = DMA_MINC_ENABLE;                                   /* 存储器增量模式 */
    s_st_esp32_dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;                  /* 外设数据长度:8位 */
    s_st_esp32_dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;                     /* 存储器数据长度:8位 */
    s_st_esp32_dma_handle.Init.Mode = DMA_NORMAL;                                          /* 外设流控模式 */
    s_st_esp32_dma_handle.Init.Priority = DMA_PRIORITY_LOW;                                /* 低优先级 */

    HAL_DMA_Init(&s_st_esp32_dma_handle);

    __HAL_LINKDMA(&s_st_esp32_uart_handle, hdmarx, s_st_esp32_dma_handle);                  /* 将 DMA 与 USART2 联系起来 */
    HAL_UART_Receive_DMA(&s_st_esp32_uart_handle,
                        s_st_rx_dma_buf_cfg.p_rx_buf1,
                        s_st_rx_dma_buf_cfg.buf_len);
}

void esp32_rx_dma_buf_switch(void)
{
    if (s_st_rx_dma_buf_cfg.buf_index == ESP32_RX_DMA_BUF_1) {
        HAL_UART_Receive_DMA(&s_st_esp32_uart_handle,
                            s_st_rx_dma_buf_cfg.p_rx_buf2,
                            s_st_rx_dma_buf_cfg.buf_len);
        s_st_rx_dma_buf_cfg.buf_index = ESP32_RX_DMA_BUF_2;
    } else {
        HAL_UART_Receive_DMA(&s_st_esp32_uart_handle,
                            s_st_rx_dma_buf_cfg.p_rx_buf1,
                            s_st_rx_dma_buf_cfg.buf_len);
        s_st_rx_dma_buf_cfg.buf_index = ESP32_RX_DMA_BUF_1;
    }
}

esp32_info_t *esp32_info_get(void)
{
    return &s_st_esp32_info;
}

void esp32_init(void)
{
    esp32_uart_init();
    esp32_dma_init();
}

void esp32_check(void)
{
    ESP32_CMD_BUF_INIT(s_arr_cmd, ESP32_CMD_BUF_MAX_LEN);

    snprintf((char*)s_arr_cmd, ESP32_CMD_BUF_MAX_LEN, "%s\r\n", s_arr_cmd_cfg[ESP32_CMD_AT].p_request);
    HAL_UART_Transmit(&s_st_esp32_uart_handle, s_arr_cmd, strlen((char*)s_arr_cmd), ESP32_CMD_TX_TIMEOUT);
}
