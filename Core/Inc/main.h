/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "stm32f1xx_hal.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"

#if HAVE_CONFIG_H
#include "config.h"
#include "qrencode.h"
#endif

#include "system.h"
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
#include "at.h"
#include "esp32.h"
#include "state.h"
#include "qrcode.h"

extern QueueHandle_t g_queue_network_handle;
extern EventGroupHandle_t g_event_group_handle;
extern SemaphoreHandle_t g_semaphore_handle;

void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
