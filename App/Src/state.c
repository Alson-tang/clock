#include "main.h"

/* “动作” 映射表 */
static action_map_t s_arr_action_map[] = {
    { STATE_START,                  state_start_entry,                  state_start_running,                state_start_exti },
    { STATE_ESP32_POWER_ON,         state_esp32_power_on_entry,         state_esp32_power_on_running,       state_esp32_power_on_exti },
    { STATE_INIT,                   state_init_entry,                   state_init_running,                 state_init_exti },
    { STATE_WIFI_NOT_CONNECTED,     state_wifi_not_connected_entry,     state_wifi_not_connected_running,   state_wifi_not_connected_exti },
    { STATE_WIFI_CONNECTED,         state_wifi_connected_entry,         state_wifi_connected_running,       state_wifi_connected_exti },
    { STATE_WIFI_DISCONNECTED,      state_wifi_disconnected_entry,      state_wifi_disconnected_running,    state_wifi_disconnected_exti },
    { STATE_SYNC_TIME,              state_sync_time_entry,              state_sync_time_running,            state_sync_time_exti },
};

/* “事件” 和 “状态” 映射表 */
static event_map_t s_arr_event_map[] = {
    { EVENT_ESP32_POWER_ON,          STATE_ESP32_POWER_ON },
    { EVENT_INIT,                    STATE_INIT },
    { EVENT_WIFI_NOT_CONNECTED,      STATE_WIFI_NOT_CONNECTED },
    { EVENT_WIFI_CONNECTED,          STATE_WIFI_CONNECTED },
    { EVENT_WIFI_DISCONNECTED,       STATE_WIFI_DISCONNECTED },
    { EVENT_SYNC_TIME,               STATE_SYNC_TIME },
    { EVENT_RUNNING,                 STATE_RUNNING },
};

static fsm_t s_st_fsm = {
    STATE_START,
    STATE_START,
    NULL,
    NULL };

void state_start_entry(void)
{
    return;
}

void state_start_running(void)
{
    return;
}

void state_start_exti(void)
{
    return;
}

void state_esp32_power_on_entry(void)
{
    ESP32_HARDWARE_POWER_ON;
    LOG_DEBUG("after");

    return;
}

void state_esp32_power_on_running(void)
{
    return;
}

void state_esp32_power_on_exti(void)
{
    return;
}

void state_init_entry(void)
{
    wifi_info_t wifi_info = { WIFI_NOT_CONNECTED, 0 };
    event_e event = EVENT_INIT;

    at_check();
    // at_echo();

    if (at_wifi_info_get(&wifi_info) == AT_STATUS_OK) {
        printf("Wi-Fi state: %u\r\n", wifi_info.state);
        printf("SSID: %s\r\n", wifi_info.ssid);

        if (wifi_info.state == WIFI_NOT_CONNECTED) {
            event = EVENT_WIFI_NOT_CONNECTED;
            // xQueueSend(g_queue_state_handle, (void*)&event, portMAX_DELAY);
            fsm_event_send(event);
        }
    }

    return;
}

void state_init_running(void)
{
    return;
}

void state_init_exti(void)
{
    return;
}

void state_wifi_not_connected_entry(void)
{
    at_mode_set(WIFI_MODE_STATION);
    at_smartconfig_set(SMARTCONFIG_AIRKISS);

    return;
}

void state_wifi_not_connected_running(void)
{
    return;
}

void state_wifi_not_connected_exti(void)
{
    return;
}

void state_wifi_connected_entry(void)
{
    at_sntp_set();

    return;
}

void state_wifi_connected_running(void)
{
    return;
}

void state_wifi_connected_exti(void)
{
    return;
}


void state_wifi_disconnected_entry(void)
{
    return;
}

void state_wifi_disconnected_running(void)
{
    return;
}

void state_wifi_disconnected_exti(void)
{
    return;
}

void state_sync_time_entry(void)
{
    sntp_time_t sntp_time = { 0 };
    clk_t clk = { 0 };
    char date[SNTP_DATE_STR_LEN] = { 0 };

    if (at_sntp_time_get(&sntp_time) == AT_STATUS_OK) {
        clk_get(&clk);
        clk.clk_time.clk_hour = sntp_time.hour;
        clk.clk_time.clk_min = sntp_time.min;
        clk.clk_time.clk_second = sntp_time.sec;
        clk_set(&clk);

        snprintf(date,
                SNTP_DATE_STR_LEN,
                "%d.%d.%d %s",
                sntp_time.year,
                clk_month_convert(sntp_time.month),
                sntp_time.day,
                sntp_time.week);
        lcd_show_ascii(16, 80, COLOR_WHITE, date, FONT_ASCII_GENERAL_COL, FONT_ASCII_GENERAL_ROW);

        update_start();
    }

    return;
}

void state_sync_time_running(void)
{
    return;
}

void state_sync_time_exti(void)
{
    return;
}

void fsm_init(void)
{
    s_st_fsm.pre_state = STATE_START;
    s_st_fsm.cur_state = STATE_START;
    s_st_fsm.p_action = &s_arr_action_map[s_st_fsm.cur_state];
    s_st_fsm.p_event = &s_arr_event_map[s_st_fsm.cur_state];
}

void fsm_state_trans(event_e event)
{
    LOG_DEBUG("event is %u, state is %u\r\n", event, s_st_fsm.p_event[event].state);

    if (s_st_fsm.cur_state != s_st_fsm.p_event[event].state) {
        s_st_fsm.pre_state = s_st_fsm.cur_state;
        s_st_fsm.cur_state = s_st_fsm.p_event[event].state;

        LOG_DEBUG("pre is %u, cur is %u\r\n", s_st_fsm.pre_state, s_st_fsm.cur_state);

        s_st_fsm.p_action[s_st_fsm.pre_state].exti_state();
        s_st_fsm.p_action[s_st_fsm.cur_state].entry_state();
    } else {
        s_st_fsm.p_action[s_st_fsm.cur_state].running_state();
    }
}

void fsm_event_send(event_e event)
{
    xQueueSend(g_queue_state_handle, (void*)&event, portMAX_DELAY);
}
