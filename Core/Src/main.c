/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "cmsis_os.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "spi_device.h"
#include "w25q64.h"
#include "st7789.h"
#include "update_font_lib.h"
#include "font.h"
#include "lcd.h"
#include "clk.h"
#include "astronaut.h"
#include "weather.h"
#include "update.h"
#include "esp32_hardware.h"

osThreadId defaultTaskHandle;
osThreadId main_logic_handle = NULL;
osThreadId xmodem_handle = NULL;
osThreadId astronaut_logic_handle = NULL;

void SystemClock_Config(void);
void StartDefaultTask(void const * argument);
void main_logic_handle_func(void const * argument);
extern void xmodem_handle_func(void const * argument);
extern void astronaut_logic_handle_func(void const * argument);

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    uart_init();
    printf("Hello World\r\n");

    spi_device_init(SPI_DEVICE_FLASH_W25Q64);
    printf("SPI FLASH ID : 0x%08X\r\n", Flash_ReadFlashID());

    spi_device_init(SPI_DEVICE_TFTLCD_ST7789);

    lcd_clear_full_screen(COLOR_BLACK);

    clk_t clk = {
        .clk_time = {
            .clk_hour = 12,
            .clk_min = 34,
            .clk_second = 00,
        },

        .clk_location = {
            .col = 16,
            .row = 104,
        },

        .clk_color = {
            .clk_hour_color = COLOR_GREEN,
            .clk_min_color = COLOR_BLUE,
            .clk_second_color = COLOR_YELLOW,
        }
    };
    clk_set(&clk);

    lcd_show_font(16, 16, COLOR_WHITE, "正", FONT_CHINESE_CHAR_GENERAL_COL, FONT_CHINESE_CHAR_GENERAL_ROW);
    lcd_show_font(16, 16, COLOR_WHITE, "无锡", FONT_CHINESE_CHAR_GENERAL_COL, FONT_CHINESE_CHAR_GENERAL_ROW);
    lcd_show_font(16, 48, COLOR_WHITE, "晴", FONT_CHINESE_CHAR_GENERAL_COL, FONT_CHINESE_CHAR_GENERAL_ROW);
    lcd_show_ascii(16, 80, COLOR_WHITE, "23.11.29 Wed", FONT_ASCII_GENERAL_COL, FONT_ASCII_GENERAL_ROW);
    astronaut_show();
    weather_show();
    weather_thermometr_show();
    weather_temperature_bar_show(17, COLOR_YELLOW);
    update_init();

    esp32_hardware_init();
    esp32_hardware_power_off();

    // /* if (1) {
    //   update_font_lib(FONT_LIB_TYPE_GBK);
    // } */

    osThreadDef(defaultTask, StartDefaultTask, 0, 0, 128);
    defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

    osThreadDef(main_logic, main_logic_handle_func, 1, 0, 128);
    main_logic_handle = osThreadCreate(osThread(main_logic), NULL);

    // osThreadDef(xmodem, xmodem_handle_func, 2, 0, 128);
    // xmodem_handle = osThreadCreate(osThread(xmodem), NULL);

    // osThreadDef(astronaut_logic, astronaut_logic_handle_func, 7, 0, 128);
    // astronaut_logic_handle = osThreadCreate(osThread(astronaut_logic), NULL);

    osKernelStart();

    while (1) {
    }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType    = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState          = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue    = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState          = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState      = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource     = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL        = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB busses clocks 
    */
    RCC_ClkInitStruct.ClockType         = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                         |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource      = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider     = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider    = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider    = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_main_logic_handle */
/**
* @brief Function implementing the main_logic thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_main_logic_handle */
void main_logic_handle_func(void const * argument)
{
    /* for(;;)
    {
        if (key_status_get() == KEY_PRESSED) {
            osDelay(20);
            if (HAL_GPIO_ReadPin(KEY_GPIO_PORT, KEY_GPIO_PIN) == GPIO_PIN_RESET) {
                led_gpio_trigger();
                update_start();
                key_status_set(KEY_RELEASE);
            }
        }

        osDelay(1);
    } */

    update_start();

    for (;;) {
        osDelay(1000);
    }
}

void astronaut_logic_handle_func(void const * argument)
{
    for(;;) {
        astronaut_update();

        osDelay(ASTRONAUT_FRAME_DELAY_MS);
    }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
