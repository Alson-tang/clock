#include "led.h"

void led_gpio_init(void)
{
	LED_GPIO_CLK_ENABLE();

	GPIO_InitTypeDef led_gpio = {
		.Pin = LED_GPIO_PIN,
		.Mode = GPIO_MODE_OUTPUT_PP,
		.Pull = GPIO_PULLUP,
		.Speed = GPIO_SPEED_FREQ_HIGH,
	};
	HAL_GPIO_Init(LED_GPIO_PORT, &led_gpio);
}

void led_gpio_on(void)
{
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_RESET);
}

void led_gpio_of(void)
{
	HAL_GPIO_WritePin(LED_GPIO_PORT, LED_GPIO_PIN, GPIO_PIN_SET);
}

void led_gpio_trigger(void)
{
	if (HAL_GPIO_ReadPin(LED_GPIO_PORT, LED_GPIO_PIN) == GPIO_PIN_RESET) {
		led_gpio_of();
	} else {
		led_gpio_on();
	}
}

