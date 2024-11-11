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
extern void application_task(void *pvParameters);

QueueHandle_t g_queue_network_handle = NULL;
QueueHandle_t g_queue_state_handle = NULL;
EventGroupHandle_t g_event_group_handle = NULL;
SemaphoreHandle_t g_semaphore_handle = NULL;

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
    /* Log 打印互斥信号量 */
    g_semaphore_handle = xSemaphoreCreateMutex();
    if (g_semaphore_handle == NULL) {
        Error_Handler();
    }

    /* 创建 ESP32 接收处理任务中使用的队列 handle */
    g_queue_network_handle = xQueueCreate(APPLICATION_NETWORK_QUEUE_SIZE, sizeof(message_t));
    if (g_queue_network_handle == NULL) {
        Error_Handler();
    }

    /* 时钟应用任务中使用的队列 handle */
    g_queue_state_handle = xQueueCreate(APPLICATION_QUEUE_SIZE, sizeof(event_e));
    if (g_queue_state_handle == NULL) {
        Error_Handler();
    }

    g_event_group_handle = xEventGroupCreate();
    if (g_event_group_handle == NULL) {
        Error_Handler();
    }
}

static void hardware_init(void)
{
    clk_t clk = { 0 };

    /* 串口初始化 */
    uart_init();

    /* 外部 FLASH(W25Q64) 初始化 */
    spi_device_init(SPI_DEVICE_FLASH_W25Q128);
    if (FLASH_ID == Flash_ReadFlashID()) {
       LOG_DEBUG("External FLASH init success");
    } else {
       LOG_DEBUG("External FLASH init failed");
    }

    /* TFTLCD 初始化 */
    spi_device_init(SPI_DEVICE_TFTLCD_ST7789);
    lcd_clear_full_screen(COLOR_BLACK);

    /* TFTLCD 时间默认显示 */
    CLK_SET_DEF(clk);
    clk_set(&clk);

    /* TFTLCD 城市默认显示 */
    lcd_show_font(LCD_SET_CITY_COL_DEF,
                  LCD_SET_CITY_ROW_DEF,
                  COLOR_WHITE,
                  LCD_SET_CITY_STR_DEF,
                  FONT_CHINESE_CHAR_GENERAL_COL,
                  FONT_CHINESE_CHAR_GENERAL_ROW);

    /* TFTLCD 天气默认显示 */
    lcd_show_font(LCD_SET_WEATHER_COL_DEF,
                  LCD_SET_WEATHER_ROW_DEF,
                  COLOR_WHITE,
                  LCD_SET_WEATHER_STR_DEF,
                  FONT_CHINESE_CHAR_GENERAL_COL,
                  FONT_CHINESE_CHAR_GENERAL_ROW);
    /* TFTLCD 日期默认显示 */
    lcd_show_ascii(LCD_SET_DATE_COL_DEF,
                  LCD_SET_DATE_ROW_DEF,
                  COLOR_WHITE,
                  LCD_SET_DATE_STR_DEF,
                  FONT_ASCII_GENERAL_COL, FONT_ASCII_GENERAL_ROW);

    /* TFTLCD 太空人显示 */
    astronaut_show();

    /* TFTLCD 天气显示 */
    weather_show();

    /* TFTLCD 温度计和温度显示 */
    weather_thermometr_show();
    weather_temperature_bar_show(17, COLOR_YELLOW);

    /* 时间定时器初始化 */
    update_init();

    /* 应用层状态机初始化 */
    fsm_init();

    /* ESP32-C3 硬件初始化 */
    esp32_hardware_init();
    esp32_init(esp32_hardware_fun_get());
    at_init(esp32_hardware_fun_get());

    /* 时间定时器启动 */
    update_start();
}

void start_init_task(void *pvParameters)
{
    /* FREERTOS 中用到的队列、事件组初始化 */
    app_init();

    /* 硬件初始化 */
    hardware_init();

    /* 创建 ESP32 接收处理任务 */
    xTaskCreate(network_rx_task, "network_rx_task", 256, NULL, 3, NULL);

    /* 创建时钟应用任务 */
    xTaskCreate(application_task, "application_task", 256, NULL, 5, NULL);

    /* 延时，目的是让其余高优先级任务运行 */
    vTaskDelay(100 / portTICK_PERIOD_MS);

    /* 向时钟应用任务 application_task 发出事件 EVENT_ESP32_POWER_ON */
    fsm_event_send(EVENT_ESP32_POWER_ON);

    /* 当前任务 start_init_task 已完成使命，退出，资源回收 */
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
