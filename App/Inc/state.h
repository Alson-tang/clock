#ifndef __STATE_H__
#define __STATE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

typedef void (*state_action)(void);

typedef enum event {
    EVENT_ESP32_POWER_ON,
    EVENT_INIT,
    EVENT_WIFI_NOT_CONNECTED,
    EVENT_WIFI_CONNECTED,
    EVENT_WIFI_DISCONNECTED,
    EVENT_SYNC_TIME,
    EVENT_WEATHER_GET,
    EVENT_RUNNING,
} event_e;

typedef enum state {
    STATE_START,
    STATE_ESP32_POWER_ON,
    STATE_INIT,
    STATE_WIFI_NOT_CONNECTED,
    STATE_WIFI_CONNECTED,
    STATE_WIFI_DISCONNECTED,
    STATE_SYNC_TIME,
    STATE_WEATHER_GET,
    STATE_RUNNING,
} state_e;

typedef struct action_map {
    state_e         state;
    state_action    entry_state;
    state_action    running_state;
    state_action    exti_state;
} action_map_t;

typedef struct event_map {
    event_e event;
    state_e state;
} event_map_t;

typedef struct fsm {
    state_e         pre_state;
    state_e         cur_state;
    action_map_t    *p_action;
    event_map_t     *p_event;
} fsm_t;

/* “动作” 表 */
void state_start_entry(void);
void state_start_running(void);
void state_start_exti(void);

void state_esp32_power_on_entry(void);
void state_esp32_power_on_running(void);
void state_esp32_power_on_exti(void);

void state_init_entry(void);
void state_init_running(void);
void state_init_exti(void);

void state_wifi_not_connected_entry(void);
void state_wifi_not_connected_running(void);
void state_wifi_not_connected_exti(void);

void state_wifi_connected_entry(void);
void state_wifi_connected_running(void);
void state_wifi_connected_exti(void);

void state_wifi_disconnected_entry(void);
void state_wifi_disconnected_running(void);
void state_wifi_disconnected_exti(void);

void state_sync_time_entry(void);
void state_sync_time_running(void);
void state_sync_time_exti(void);

void state_weather_get_entry(void);
void state_weather_get_running(void);
void state_weather_get_exti(void);

/* 队列 */
extern QueueHandle_t g_queue_state_handle;

void fsm_init(void);
void fsm_state_trans(event_e event);
void fsm_event_send(event_e event);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* state.h */
