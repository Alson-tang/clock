#include "update.h"
#include "clk.h"

TIM_HandleTypeDef g_update_tim_handle = { 0 };

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == (&g_update_tim_handle)) {
        clk_update();
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

/* The STM32F103xC/D/E series include up to 2 advanced-control timers (TIM1 / TIM8),
   up to 4 general-purpose timers (TIM2 / TIM3 / TIM4 / TIM5),
   2 basic timers (TIM6 / TIM7).
   I choose TIM2 as the timer for updating the clock */
static void update_timer_init(void)
{
    TIM_ClockConfigTypeDef clk_cfg = { 0 };

    g_update_tim_handle.Instance = TIM2;
    g_update_tim_handle.Init.Prescaler = 10000 - 1;
    g_update_tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_update_tim_handle.Init.Period = 7200 - 1;
    g_update_tim_handle.Init.ClockDivision = 0;
    g_update_tim_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    HAL_TIM_Base_Init(&g_update_tim_handle);
}

void update_init(void)
{
    update_timer_init();
}

void update_start(void)
{
    HAL_TIM_Base_Start_IT(&g_update_tim_handle);
}
