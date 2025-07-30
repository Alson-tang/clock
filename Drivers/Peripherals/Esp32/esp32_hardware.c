#include "main.h"

void esp32_hardware_init(void)
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

void esp32_hardware_power_on(void)
{
    HAL_GPIO_WritePin(ESP32_HARDWARE_PORT, ESP32_HARDWARE_PIN, GPIO_PIN_SET);
}

void esp32_hardware_power_off(void)
{
    HAL_GPIO_WritePin(ESP32_HARDWARE_PORT, ESP32_HARDWARE_PIN, GPIO_PIN_RESET);
}
