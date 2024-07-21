#ifndef __ESP32_H__
#define __ESP32_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define ESP32_STATUS_OK                         (0)
#define ESP32_STATUS_ERROR                      (-1)

#define ESP32_UART                              (USART2)
#define ESP32_UART_BAUDRATE                     (115200)
#define ESP32_UART_TX_PIN                       (GPIO_PIN_2)
#define ESP32_UART_RX_PIN                       (GPIO_PIN_3)
#define ESP32_UART_PIN_PORT                     (GPIOA)
#define ESP32_UART_CLK_ENABLE()                 __HAL_RCC_USART2_CLK_ENABLE()
#define ESP32_UART_TX_PIN_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()
#define ESP32_UART_RX_PIN_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE()
#define ESP32_UART_IRQn                         USART2_IRQn
#define ESP32_UART_IRQHandler                   USART2_IRQHandler

#define ESP32_CMD_BUF_MAX_LEN                   (256)
#define ESP32_CMD_TX_TIMEOUT                    (1000)
#define ESP32_RX_BUF_MAX_LEN                    (1024)

typedef int8_t esp32_status;
typedef esp32_status (*cmd_cb)(void);

typedef enum esp32_cmd {
    ESP32_CMD_AT,
    ESP32_CMD_MAX,
} esp32_cmd_e;

typedef enum esp32_rx_dma_buf_index {
    ESP32_RX_DMA_BUF_1,
    ESP32_RX_DMA_BUF_2,
} esp32_rx_dma_buf_index_e;

typedef struct esp32_cmd_cfg {
    esp32_cmd_e cmd;
    char *p_request;
    char *p_response;
    uint32_t timeout;
    cmd_cb cb;
} esp32_cmd_cfg_t;

typedef struct esp32_rx_dma_buf_cfg {
    esp32_rx_dma_buf_index_e buf_index;
    uint8_t *p_rx_buf1;
    uint8_t *p_rx_buf2;
    uint32_t buf_len;
} esp32_rx_dma_buf_cfg_t;

typedef struct esp32_info {
    esp32_cmd_cfg_t *p_cmd_cfg;
    esp32_rx_dma_buf_cfg_t *p_rx_dma_buf_cfg;
} esp32_info_t;

extern UART_HandleTypeDef s_st_esp32_uart_handle;
extern DMA_HandleTypeDef s_st_esp32_dma_handle;

void esp32_rx_dma_buf_switch(void);
esp32_info_t *esp32_info_get(void);

void esp32_init(void);
void esp32_check(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* esp32.h */
