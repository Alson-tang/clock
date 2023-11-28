#include "update.h"
#include "clk.h"
#include "astronaut.h"

typedef struct {
    uint32_t clk_update_time;
    uint32_t astronaut_time;
    uint32_t time;
} update_config_type;

TIM_HandleTypeDef g_update_tim_handle = { 0 };
static update_config_type s_update_cfg = { 0 };

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    s_update_cfg.time += 1;

    astronaut_update();

    if (s_update_cfg.time >= s_update_cfg.clk_update_time) {
        clk_update();
        s_update_cfg.time = 0;
    }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
}

static void update_timer_init(void)
{
    TIM_ClockConfigTypeDef clk_cfg = { 0 };

    g_update_tim_handle.Instance = TIM2;
    g_update_tim_handle.Init.Prescaler = 1000 - 1;
    g_update_tim_handle.Init.CounterMode = TIM_COUNTERMODE_UP;
    g_update_tim_handle.Init.Period = 7200 - 1;
    g_update_tim_handle.Init.ClockDivision = 0;
    g_update_tim_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;

    HAL_TIM_Base_Init(&g_update_tim_handle);

    clk_cfg.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
    HAL_TIM_ConfigClockSource(&g_update_tim_handle, &clk_cfg);
}

void update_init(uint32_t clk_update_time, uint32_t astronaut_time)
{
    update_timer_init();

    memset(&s_update_cfg, 0, sizeof(update_config_type));
    s_update_cfg.clk_update_time = clk_update_time;
    s_update_cfg.astronaut_time = astronaut_time;
    s_update_cfg.time = 0;
}

void update_start(void)
{
    HAL_TIM_Base_Start_IT(&g_update_tim_handle);
}
