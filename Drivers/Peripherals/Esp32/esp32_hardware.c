#include "main.h"

static esp32_hardware_cfg_t s_st_esp32_hardware_cfg = { 0 };
static esp32_hardware_func_t s_st_esp32_hardware_func = { 0 };

static void __esp32_hardware_power_init(void)
{
    ESP32_HARDWARE_GPIO_CLK_ENABLE();

    GPIO_InitTypeDef led_gpio = {
        .Pin = ESP32_HARDWARE_PIN,
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_HIGH,
    };
    HAL_GPIO_Init(ESP32_HARDWARE_PORT, &led_gpio);

    esp32_hardware_power_off();
}

static void __esp32_hardware_communi_inter_init(UART_HandleTypeDef *p_uart_handle)
{
    p_uart_handle->Instance         = ESP32_HARDWARE_UART;
    p_uart_handle->Init.BaudRate    = ESP32_HARDWARE_UART_BAUDRATE;
    p_uart_handle->Init.WordLength  = UART_WORDLENGTH_8B;
    p_uart_handle->Init.StopBits    = UART_STOPBITS_1;
    p_uart_handle->Init.Parity      = UART_PARITY_NONE;
    p_uart_handle->Init.HwFlowCtl   = UART_HWCONTROL_NONE;
    p_uart_handle->Init.Mode        = UART_MODE_TX_RX;

    if(HAL_UART_DeInit(p_uart_handle) != HAL_OK) {
        Error_Handler();
    }

    if(HAL_UART_Init(p_uart_handle) != HAL_OK) {
        Error_Handler();
    }

    __HAL_UART_ENABLE_IT(p_uart_handle, UART_IT_IDLE);
}

static void __esp32_hardware_dma_init(UART_HandleTypeDef *p_uart_handle, DMA_HandleTypeDef *p_dma_handle)
{
    __HAL_RCC_DMA1_CLK_ENABLE();                                    /* DMA1 时钟使能 */

    p_dma_handle->Instance = ESP32_HARDWARE_DMA_CHANNEL;            /* USART2_RX 使用的 DMA 通道为: DMA1_Channel6 */
    p_dma_handle->Init.Direction = DMA_PERIPH_TO_MEMORY;            /* 外设到存储器模式 */
    p_dma_handle->Init.PeriphInc = DMA_PINC_DISABLE;                /* 外设非增量模式 */
    p_dma_handle->Init.MemInc = DMA_MINC_ENABLE;                    /* 存储器增量模式 */
    p_dma_handle->Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;   /* 外设数据长度:8位 */
    p_dma_handle->Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;      /* 存储器数据长度:8位 */
    p_dma_handle->Init.Mode = DMA_NORMAL;                           /* 外设流控模式 */
    p_dma_handle->Init.Priority = DMA_PRIORITY_LOW;                 /* 低优先级 */

    HAL_DMA_Init(p_dma_handle);

    __HAL_LINKDMA(p_uart_handle, hdmarx, *p_dma_handle);            /* 将 DMA 与 USART2 联系起来 */
}

static void __esp32_hardware_write(const uint8_t *p_buf, uint16_t size, uint32_t timeout)
{
    HAL_UART_Transmit(&s_st_esp32_hardware_cfg.uart_handle, p_buf, size, timeout);
}

static void __esp32_hardware_buf_switch(uint8_t *p_buf, uint16_t size)
{
    HAL_UART_Receive_DMA(&s_st_esp32_hardware_cfg.uart_handle, p_buf, size);
}

esp32_hardware_cfg_t *esp32_hardware_cfg_get(void)
{
    return &s_st_esp32_hardware_cfg;
}

esp32_hardware_func_t *esp32_hardware_fun_get(void)
{
    return &s_st_esp32_hardware_func;
}

void esp32_hardware_init(void)
{
    memset((void*)&s_st_esp32_hardware_cfg, 0, sizeof(esp32_hardware_cfg_t));
    memset((void*)&s_st_esp32_hardware_func, 0, sizeof(esp32_hardware_func_t));

    /* 初始化电源控制管脚 */
    __esp32_hardware_power_init();

    /* 初始化物理通信接口 */
    __esp32_hardware_communi_inter_init(&s_st_esp32_hardware_cfg.uart_handle);

    /* 初始化 DMA */
    __esp32_hardware_dma_init(&s_st_esp32_hardware_cfg.uart_handle, &s_st_esp32_hardware_cfg.dma_handle);

    /* 初始化 ESP32 硬件回调接口 */
    s_st_esp32_hardware_func.write       = __esp32_hardware_write;
    s_st_esp32_hardware_func.buf_switch  = __esp32_hardware_buf_switch;

    return;
}

void esp32_hardware_power_on(void)
{
    HAL_GPIO_WritePin(ESP32_HARDWARE_PORT, ESP32_HARDWARE_PIN, GPIO_PIN_SET);
}

void esp32_hardware_power_off(void)
{
    HAL_GPIO_WritePin(ESP32_HARDWARE_PORT, ESP32_HARDWARE_PIN, GPIO_PIN_RESET);
}
