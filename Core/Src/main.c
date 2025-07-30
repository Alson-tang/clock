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
#include "main.h"

extern void start_init_task(void *pvParameters);
extern void network_rx_task(void *pvParameters);

QueueHandle_t g_queue_network_handle = NULL;
EventGroupHandle_t g_event_group_handle = NULL;

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

static void app_init(void)
{
    g_queue_network_handle = xQueueCreate(APPLICATION_NETWORK_QUEUE_SIZE, sizeof(message_t));

    if (g_queue_network_handle == NULL) {
        Error_Handler();
    }

    g_event_group_handle = xEventGroupCreate();

    if (g_event_group_handle == NULL) {
        Error_Handler();
    }
}

void start_init_task(void *pvParameters)
{
    esp32_wifi_state_e wifi_state = ESP32_WIFI_STATE_NOT_STARTED_CONN;
    char ssid[WIFI_SSID_MAX_LEN] = { 0 };
    esp32_wifi_sntp_time_t sntp = { 0 };

    /* 硬件初始化 */
    uart_init();

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

    /* 应用中有关 FREERTOS 初始化 */
    app_init();

    /* 创建 ESP32 接收处理任务 */
    xTaskCreate(network_rx_task, "network_rx_task", 256, NULL, 1, NULL);

    esp32_hardware_init();
    esp32_init();
    esp32_hardware_power_on();

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    if (esp32_check() == ESP32_STATUS_OK) {
        printf("ESP32-C3 module online\r\n");
    } else {
        printf("ESP32-C3 module offline\r\n");
    }

    esp32_wifi_mode_e mode = ESP32_WIFI_DISABLE;
    if (esp32_wifi_mode_get(&mode) == ESP32_STATUS_OK) {
        printf("ESP32-C3 Wi-Fi mode is %d\r\n", mode);
    }

    if (esp32_wifi_state_get(&wifi_state, ssid, WIFI_SSID_MAX_LEN) == ESP32_STATUS_OK) {
        switch (wifi_state) {
            case ESP32_WIFI_STATE_NOT_STARTED_CONN: {
                esp32_wifi_mode_set(ESP32_WIFI_STATION);
                esp32_wifi_smartconfig_start();

                break;
            }

            case ESP32_WIFI_STATE_CONNECTED_IPV4: {
                printf("SSID: %s\r\n", ssid);
                esp32_wifi_sntp_set();
                vTaskDelay(3000 / portTICK_PERIOD_MS);
                if (esp32_wifi_sntp_time(&sntp) == ESP32_STATUS_OK) {
                    printf("SNTP: %u:%u:%u\r\n", sntp.hour, sntp.min, sntp.second);

                    clk.clk_time.clk_hour = sntp.hour;
                    clk.clk_time.clk_min = sntp.min;
                    clk.clk_time.clk_second = sntp.second;
                    clk_set(&clk);
                    update_start();
                }
 
                break;
            }

            default:
                break;
        }
    }

    vTaskDelete(NULL);
}

int main(void)
{
    HAL_Init();

    SystemClock_Config();

    // /* if (1) {
    //   update_font_lib(FONT_LIB_TYPE_GBK);
    // } */

    xTaskCreate(start_init_task, "start_init_task", 256, NULL, tskIDLE_PRIORITY, NULL);

    vTaskStartScheduler();

    while (1) {
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
