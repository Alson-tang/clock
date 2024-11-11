#ifndef __ESP32_HARDWARE_H__
#define __ESP32_HARDWARE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/* 电源控制管脚 */
#define ESP32_HARDWARE_PIN                      GPIO_PIN_3
#define ESP32_HARDWARE_PORT                     GPIOC
#define ESP32_HARDWARE_GPIO_CLK_ENABLE()        __HAL_RCC_GPIOC_CLK_ENABLE()

/* 物理通信接口，使用 USART2 */
#define ESP32_HARDWARE_UART                     (USART2)
#define ESP32_HARDWARE_UART_BAUDRATE            (115200)
#define ESP32_HARDWARE_UART_TX_PIN              (GPIO_PIN_2)
#define ESP32_HARDWARE_UART_RX_PIN              (GPIO_PIN_3)
#define ESP32_HARDWARE_UART_PIN_PORT            (GPIOA)
#define ESP32_HARDWARE_UART_CLK_ENABLE()        __HAL_RCC_USART2_CLK_ENABLE()
#define ESP32_HARDWARE_UART_TX_PIN_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define ESP32_HARDWARE_UART_RX_PIN_CLK_ENABLE() __HAL_RCC_GPIOA_CLK_ENABLE()
#define ESP32_HARDWARE_UART_IRQn                USART2_IRQn
#define ESP32_HARDWARE_UART_IRQHandler          USART2_IRQHandler

/* DMA 配置 */
#define ESP32_HARDWARE_DMA_CHANNEL              DMA1_Channel6

/* 上电下电 */
#define ESP32_HARDWARE_POWER_ON_DELAY_MS                                            (5000)
#define ESP32_HARDWARE_POWER_ON                                                     \
    do {                                                                            \
        esp32_hardware_power_on();                                                  \
        vTaskDelay(ESP32_HARDWARE_POWER_ON_DELAY_MS / portTICK_PERIOD_MS);          \
    } while (0)

#define ESP32_HARDWARE_POWER_OFF_DELAY_MS                                           (0)
#define ESP32_HARDWARE_POWER_OFF                                                    \
    do {                                                                            \
        esp32_hardware_power_off();                                                 \
        vTaskDelay(ESP32_HARDWARE_POWER_OFF_DELAY_MS / portTICK_PERIOD_MS);         \
    } while (0)

/* ESP32 硬件回调接口 */
typedef void (*write_cb)(const uint8_t *p_buf, uint16_t size, uint32_t timeout);
typedef void (*buf_switch_cb)(uint8_t *p_buf, uint16_t size);

typedef struct esp32_hardware_cfg {
    UART_HandleTypeDef  uart_handle;
    DMA_HandleTypeDef   dma_handle;
} esp32_hardware_cfg_t;

typedef struct esp32_hardware_func
{
    write_cb write;
    buf_switch_cb buf_switch;
} esp32_hardware_func_t;

esp32_hardware_cfg_t *esp32_hardware_cfg_get(void);
esp32_hardware_func_t *esp32_hardware_fun_get(void);
void esp32_hardware_init(void);
void esp32_hardware_power_on(void);
void esp32_hardware_power_off(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* esp32_hardware.h */
