#include <stdio.h>
#include "uart.h"

UART_HandleTypeDef g_update_font_lib_uart_handle = { 0 };
static UART_HandleTypeDef s_debug_uart_handle = { 0 };

void uart_init(void)
{

    g_update_font_lib_uart_handle.Instance        = UPDATE_FONT_LIB_UART_NUM;
    g_update_font_lib_uart_handle.Init.BaudRate   = 115200;
    g_update_font_lib_uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
    g_update_font_lib_uart_handle.Init.StopBits   = UART_STOPBITS_1;
    g_update_font_lib_uart_handle.Init.Parity     = UART_PARITY_NONE;
    g_update_font_lib_uart_handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    g_update_font_lib_uart_handle.Init.Mode       = UART_MODE_TX_RX;
    HAL_UART_Init(&g_update_font_lib_uart_handle);

    s_debug_uart_handle.Instance        = DEBUG_UART_NUM;
    s_debug_uart_handle.Init.BaudRate   = 115200;
    s_debug_uart_handle.Init.WordLength = UART_WORDLENGTH_8B;
    s_debug_uart_handle.Init.StopBits   = UART_STOPBITS_1;
    s_debug_uart_handle.Init.Parity     = UART_PARITY_NONE;
    s_debug_uart_handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    s_debug_uart_handle.Init.Mode       = UART_MODE_TX_RX;
    HAL_UART_Init(&s_debug_uart_handle);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    if (huart == &g_update_font_lib_uart_handle) {
      UPDATE_FONT_LIB_USARTx_TX_GPIO_CLK_ENABLE();
      UPDATE_FONT_LIB_USARTx_RX_GPIO_CLK_ENABLE();

      UPDATE_FONT_LIB_USARTx_CLK_ENABLE();

      GPIO_InitStruct.Pin       = UPDATE_FONT_LIB_UART_TX;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

      HAL_GPIO_Init(UPDATE_FONT_LIB_UART_PORT, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = UPDATE_FONT_LIB_UART_RX;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;

      HAL_GPIO_Init(UPDATE_FONT_LIB_UART_PORT, &GPIO_InitStruct);

    } else {
      DEBUG_USARTx_TX_GPIO_CLK_ENABLE();
      DEBUG_USARTx_RX_GPIO_CLK_ENABLE();

      DEBUG_USARTx_CLK_ENABLE();

      GPIO_InitStruct.Pin       = DEBUG_UART_TX;
      GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull      = GPIO_PULLUP;
      GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;

      HAL_GPIO_Init(DEBUG_UART_PORT, &GPIO_InitStruct);

      GPIO_InitStruct.Pin = DEBUG_UART_RX;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;

      HAL_GPIO_Init(DEBUG_UART_PORT, &GPIO_InitStruct);
    }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&s_debug_uart_handle, (uint8_t *)&ch, 1, 0xFFFF);

    return ch;
}
