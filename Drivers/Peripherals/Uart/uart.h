#ifndef __UART_H__
#define __UART_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#if 0
#define UPDATE_FONT_LIB_UART_NUM                        USART1
#define UPDATE_FONT_LIB_UART_TX                         GPIO_PIN_9
#define UPDATE_FONT_LIB_UART_RX                         GPIO_PIN_10
#define UPDATE_FONT_LIB_UART_PORT                       GPIOA
#define UPDATE_FONT_LIB_USARTx_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define UPDATE_FONT_LIB_USARTx_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define UPDATE_FONT_LIB_USARTx_CLK_ENABLE()             __HAL_RCC_USART1_CLK_ENABLE();

#define DEBUG_UART_NUM                                  USART2
#define DEBUG_UART_TX                                   GPIO_PIN_2
#define DEBUG_UART_RX                                   GPIO_PIN_3
#define DEBUG_UART_PORT                                 GPIOA
#define DEBUG_USARTx_RX_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USARTx_TX_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USARTx_CLK_ENABLE()                       __HAL_RCC_USART2_CLK_ENABLE();
#endif

#define UPDATE_FONT_LIB_UART_NUM                        USART2
#define UPDATE_FONT_LIB_UART_TX                         GPIO_PIN_2
#define UPDATE_FONT_LIB_UART_RX                         GPIO_PIN_3
#define UPDATE_FONT_LIB_UART_PORT                       GPIOA
#define UPDATE_FONT_LIB_USARTx_RX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define UPDATE_FONT_LIB_USARTx_TX_GPIO_CLK_ENABLE()     __HAL_RCC_GPIOA_CLK_ENABLE()
#define UPDATE_FONT_LIB_USARTx_CLK_ENABLE()             __HAL_RCC_USART2_CLK_ENABLE()

#define DEBUG_UART_NUM                                  USART1
#define DEBUG_UART_TX                                   GPIO_PIN_9
#define DEBUG_UART_RX                                   GPIO_PIN_10
#define DEBUG_UART_PORT                                 GPIOA
#define DEBUG_USARTx_RX_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USARTx_TX_GPIO_CLK_ENABLE()               __HAL_RCC_GPIOA_CLK_ENABLE()
#define DEBUG_USARTx_CLK_ENABLE()                       __HAL_RCC_USART1_CLK_ENABLE()

extern UART_HandleTypeDef g_update_font_lib_uart_handle;

void uart_init(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* uart.h */

