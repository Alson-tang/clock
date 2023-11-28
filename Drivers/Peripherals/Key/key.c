#include "key.h"

static key_status key_state = KEY_RELEASE;

void key_gpio_init(void)
{
	KEY_GPIO_CLK_ENABLE();

	GPIO_InitTypeDef key_gpio = {
		.Pin = KEY_GPIO_PIN,
		.Mode = GPIO_MODE_IT_FALLING,
		.Pull = GPIO_PULLUP,
		.Speed = GPIO_SPEED_FREQ_HIGH,
	};
	HAL_GPIO_Init(KEY_GPIO_PORT, &key_gpio);

	HAL_NVIC_SetPriority(EXTI0_IRQn, KEY_IT_PREEMPT_PRIORITY, KEY_IT_SUB_PRIORITY);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

key_status key_status_get(void)
{
	return key_state;
}

void key_status_set(key_status status)
{
	key_state = status;
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == KEY_GPIO_PIN) {
		key_state = KEY_PRESSED;
	} else {
		key_state = KEY_RELEASE;
	}
}

