#include "main.h"

#define ESP32_WIFI_CMD_RETRY_NUMS                                                       (10)

#define ESP32_WIFI_CMD_INIT()                                                           \
    do {                                                                                \
        memset((void*)s_arr_cmd, 0, ESP32_CMD_BUF_MAX_LEN);                             \
    } while (0);

#define ESP32_WIFI_RUN_CMD_INIT(p_dst_cmd, p_src_cmd)                                   \
    do {                                                                                \
        memcpy((void*)p_dst_cmd, (const void*)p_src_cmd, sizeof(esp32_wifi_cmd_cfg_t)); \
    } while (0);

typedef struct esp32_wifi_info {
    esp32_wifi_mode_e                   mode;                                   /* Wi-Fi 模式 */
    esp32_wifi_smartconfig_type_e       type;                                   /* Wi-Fi SmartConfig 类型 */
    char                                ssid[WIFI_SSID_MAX_LEN];                /* Wi-Fi SSID */
    char                                password[WIFI_PASSWORD_MAX_LEN];        /* Wi-Fi PASSWORD */
    esp32_wifi_connect_state_e          con_state;                              /* Wi-Fi 连接状态 */
} esp32_wifi_info_t;

typedef struct esp32_wifi_state_info {
    esp32_wifi_state_e                  state;                                  /* Wi-Fi 状态 */
    char                                ssid[WIFI_SSID_MAX_LEN];                /* Wi-Fi SSID */
} esp32_wifi_state_info_t;

static esp32_status esp32_cmd_wifi_mode_get_cb(const char *p_str, uint32_t len);
static esp32_status esp32_cmd_wifi_sntp_time_cb(const char *p_str, uint32_t len);
static esp32_status esp32_cmd_wifi_state_cb(const char *p_str, uint32_t len);

static uint8_t s_arr_cmd[ESP32_CMD_BUF_MAX_LEN] = { 0 };
static esp32_wifi_cmd_cfg_t s_arr_cmd_cfg[] = {
    {
        ESP32_CMD_AT,
        "AT",
        "OK",
        1000,
        ESP32_WIFI_CMD_RETRY_NUMS,
        NULL
    },
    {
        ESP32_CMD_WIFI_MODE_GET,
        "AT+CWMODE?",
        "OK",
        1000,
        ESP32_WIFI_CMD_RETRY_NUMS,
        esp32_cmd_wifi_mode_get_cb
    },
    {
        ESP32_CMD_WIFI_MODE_STA_SET,
        "AT+CWMODE=1",
        "OK",
        1000,
        ESP32_WIFI_CMD_RETRY_NUMS,
        NULL
    },
    {
        ESP32_CMD_WIFI_MODE_SOFTAP_SET,
        "AT+CWMODE=2",
        "OK",
        1000,
        ESP32_WIFI_CMD_RETRY_NUMS,
        NULL
    },
    {
        ESP32_CMD_WIFI_SMARTCONFIG_START,
        "AT+CWSTARTSMART",
        "OK",
        1000,
        ESP32_WIFI_CMD_RETRY_NUMS,
        NULL
    },
    {
        ESP32_CMD_WIFI_SMARTCONFIG_STOP,
        "AT+CWSTOPSMART",
        "OK",
        1000,
        ESP32_WIFI_CMD_RETRY_NUMS,
        NULL
    },
    {
        ESP32_CMD_WIFI_SNTP_CFG,
        "AT+CIPSNTPCFG=1,8,\"ntp.aliyun.com\"",
        "OK",
        2000,
        ESP32_WIFI_CMD_RETRY_NUMS,
        NULL
    },
    {
        ESP32_CMD_WIFI_SNTP_TIME,
        "AT+CIPSNTPTIME?",
        "OK",
        2000,
        ESP32_WIFI_CMD_RETRY_NUMS,
        esp32_cmd_wifi_sntp_time_cb,
    },
    {
        ESP32_CMD_WIFI_STATE,
        "AT+CWSTATE?",
        "OK",
        2000,
        ESP32_WIFI_CMD_RETRY_NUMS,
        esp32_cmd_wifi_state_cb,
    },
};
static esp32_wifi_cmd_cfg_t s_st_run_cmd_cfg = { 0 };
static esp32_wifi_info_t s_st_wifi_info = { 0 };
static esp32_wifi_sntp_time_t s_st_wifi_sntp_time = { 0 };
static esp32_wifi_state_info_t s_st_wifi_state = { 0 };

static esp32_status esp32_cmd_wifi_mode_get_cb(const char *p_str, uint32_t len)
{
    esp32_status state = ESP32_STATUS_OK;
    char *str = NULL;
    int32_t mode = 0;

    str = strstr(p_str, "+CWMODE:");
    if (str != NULL) {
        sscanf(str, "+CWMODE:%d", &mode);
        s_st_wifi_info.mode = (esp32_wifi_mode_e)mode;
        state = ESP32_STATUS_OK;
    } else {
        state = ESP32_STATUS_ERROR;
    }

    return ESP32_STATUS_OK;
}

static esp32_status esp32_cmd_wifi_sntp_time_cb(const char *p_str, uint32_t len)
{
    esp32_status state = ESP32_STATUS_OK;
    char *str = NULL;

    char week[4] = { 0 };
    char month[4] = { 0 };
    int32_t day = 0;
    int32_t hour = 0;
    int32_t min = 0;
    int32_t sec = 0;
    int32_t year = 0;

    str = strstr(p_str, "+CIPSNTPTIME:");
    if (str != NULL) {
        sscanf(str, "+CIPSNTPTIME:%s %s %d %d:%d:%d %d", week, month, &day, &hour, &min, &sec, &year);
        s_st_wifi_sntp_time.hour = (uint8_t)hour;
        s_st_wifi_sntp_time.min = (uint8_t)min;
        s_st_wifi_sntp_time.second = (uint8_t)sec;
        state = ESP32_STATUS_OK;
    } else {
        state = ESP32_STATUS_ERROR;
    }

    return state;
}

static esp32_status esp32_cmd_wifi_state_cb(const char *p_str, uint32_t len)
{
    esp32_status status = ESP32_STATUS_OK;
    char *str = NULL;
    int32_t state = 0;
    char ssid[WIFI_SSID_MAX_LEN] = { 0 };

    str = strstr(p_str, "+CWSTATE:");
    if (str != NULL) {
        sscanf(str, "+CWSTATE:%d, %s", &state, ssid);
        s_st_wifi_state.state = (esp32_wifi_mode_e)state;
        memcpy((void*)s_st_wifi_state.ssid, (void*)ssid, WIFI_SSID_MAX_LEN);
        status = ESP32_STATUS_OK;
    } else {
        status = ESP32_STATUS_ERROR;
    }

    return ESP32_STATUS_OK;
}

static void esp32_cmd_end_init(char *p_cmd_str, char *p_cmd_request)
{
    snprintf(p_cmd_str, ESP32_CMD_BUF_MAX_LEN, "%s\r\n", p_cmd_request);
}

static esp32_status esp32_cmd_response_handle(esp32_cmd_e cmd)
{
    EventBits_t bits = 0;
    TickType_t wait_ticks = 0;

    wait_ticks = s_arr_cmd_cfg[cmd].timeout / portTICK_PERIOD_MS;

    bits = xEventGroupWaitBits(g_event_group_handle,
                                APPLICATION_NETWORK_RECV_OK,
                                pdTRUE,
                                pdTRUE,
                                wait_ticks);
    if ((bits & APPLICATION_NETWORK_RECV_OK) != 0) {
        return ESP32_STATUS_OK;
    } else {
        return ESP32_STATUS_TIMEOUT;
    }
}

esp32_status esp32_wifi_check(void)
{
    esp32_cmd_e cmd = ESP32_CMD_AT;

    ESP32_WIFI_CMD_INIT();
    ESP32_WIFI_RUN_CMD_INIT(&s_st_run_cmd_cfg, &s_arr_cmd_cfg[cmd]);

    snprintf((char*)s_arr_cmd, ESP32_CMD_BUF_MAX_LEN, "%s\r\n", s_arr_cmd_cfg[cmd].p_request);
    HAL_UART_Transmit(&s_st_esp32_uart_handle, s_arr_cmd, strlen((char*)s_arr_cmd), ESP32_CMD_TX_TIMEOUT);

    return esp32_cmd_response_handle(cmd);
}

esp32_status esp32_wifi_mode_get(esp32_wifi_mode_e *p_mode)
{
    esp32_status state = ESP32_STATUS_OK;
    esp32_cmd_e cmd = ESP32_CMD_WIFI_MODE_GET;

    ESP32_WIFI_CMD_INIT();
    ESP32_WIFI_RUN_CMD_INIT(&s_st_run_cmd_cfg, &s_arr_cmd_cfg[cmd]);

    esp32_cmd_end_init((char*)s_arr_cmd, s_arr_cmd_cfg[cmd].p_request);
    HAL_UART_Transmit(&s_st_esp32_uart_handle, s_arr_cmd, strlen((char*)s_arr_cmd), ESP32_CMD_TX_TIMEOUT);

    state = esp32_cmd_response_handle(cmd);
    if (state == ESP32_STATUS_OK) {
        *p_mode = s_st_wifi_info.mode;
    } else {
        /* 这里只是为了匹配 if，什么也不做 */
    }

    return state;
}

esp32_status esp32_wifi_mode_set(esp32_wifi_mode_e mode)
{
    esp32_cmd_e cmd = ESP32_CMD_MAX;

    switch (mode) {
        case ESP32_WIFI_STATION: {
            cmd = ESP32_CMD_WIFI_MODE_STA_SET;
            break;
        }

        case ESP32_WIFI_SOFTAP: {
            cmd = ESP32_CMD_WIFI_MODE_SOFTAP_SET;
            break;
        }

        default:
            break;
    }

    ESP32_WIFI_CMD_INIT();
    ESP32_WIFI_RUN_CMD_INIT(&s_st_run_cmd_cfg, &s_arr_cmd_cfg[cmd]);

    esp32_cmd_end_init((char*)s_arr_cmd, s_arr_cmd_cfg[cmd].p_request);
    HAL_UART_Transmit(&s_st_esp32_uart_handle, s_arr_cmd, strlen((char*)s_arr_cmd), ESP32_CMD_TX_TIMEOUT);

    return esp32_cmd_response_handle(cmd);
}

esp32_status esp32_wifi_smartconfig_start(void)
{
    esp32_cmd_e cmd = ESP32_CMD_WIFI_SMARTCONFIG_START;

    ESP32_WIFI_CMD_INIT();
    ESP32_WIFI_RUN_CMD_INIT(&s_st_run_cmd_cfg, &s_arr_cmd_cfg[cmd]);

    snprintf((char*)s_arr_cmd, ESP32_CMD_BUF_MAX_LEN, "%s\r\n", s_arr_cmd_cfg[cmd].p_request);
    HAL_UART_Transmit(&s_st_esp32_uart_handle, s_arr_cmd, strlen((char*)s_arr_cmd), ESP32_CMD_TX_TIMEOUT);

    return esp32_cmd_response_handle(cmd);
}

esp32_status esp32_wifi_smartconfig_stop(void)
{

}

esp32_status esp32_wifi_sntp_set(void)
{
    esp32_cmd_e cmd = ESP32_CMD_WIFI_SNTP_CFG;

    ESP32_WIFI_CMD_INIT();
    ESP32_WIFI_RUN_CMD_INIT(&s_st_run_cmd_cfg, &s_arr_cmd_cfg[cmd]);

    snprintf((char*)s_arr_cmd, ESP32_CMD_BUF_MAX_LEN, "%s\r\n", s_arr_cmd_cfg[cmd].p_request);
    HAL_UART_Transmit(&s_st_esp32_uart_handle, s_arr_cmd, strlen((char*)s_arr_cmd), ESP32_CMD_TX_TIMEOUT);

    return esp32_cmd_response_handle(cmd);
}

esp32_status esp32_wifi_sntp_time(esp32_wifi_sntp_time_t *p_sntp)
{
    esp32_status state = ESP32_STATUS_OK;
    esp32_cmd_e cmd = ESP32_CMD_WIFI_SNTP_TIME;

    ESP32_WIFI_CMD_INIT();
    ESP32_WIFI_RUN_CMD_INIT(&s_st_run_cmd_cfg, &s_arr_cmd_cfg[cmd]);

    snprintf((char*)s_arr_cmd, ESP32_CMD_BUF_MAX_LEN, "%s\r\n", s_arr_cmd_cfg[cmd].p_request);
    HAL_UART_Transmit(&s_st_esp32_uart_handle, s_arr_cmd, strlen((char*)s_arr_cmd), ESP32_CMD_TX_TIMEOUT);

    state = esp32_cmd_response_handle(cmd);
    if (state == ESP32_STATUS_OK) {
        memcpy((void*)p_sntp, (void*)&s_st_wifi_sntp_time, sizeof(esp32_wifi_sntp_time_t));
    } else {
        /* 这里只是为了匹配 if，什么也不做 */
    }

    return state;
}

esp32_status esp32_wifi_state_get(esp32_wifi_state_e *p_state, char *p_ssid, uint16_t ssid_len)
{
    esp32_status state = ESP32_STATUS_OK;
    esp32_cmd_e cmd = ESP32_CMD_WIFI_STATE;

    ESP32_WIFI_CMD_INIT();
    ESP32_WIFI_RUN_CMD_INIT(&s_st_run_cmd_cfg, &s_arr_cmd_cfg[cmd]);

    snprintf((char*)s_arr_cmd, ESP32_CMD_BUF_MAX_LEN, "%s\r\n", s_arr_cmd_cfg[cmd].p_request);
    HAL_UART_Transmit(&s_st_esp32_uart_handle, s_arr_cmd, strlen((char*)s_arr_cmd), ESP32_CMD_TX_TIMEOUT);

    state = esp32_cmd_response_handle(cmd);
    if (state == ESP32_STATUS_OK) {
        *p_state = s_st_wifi_state.state;

        if (ssid_len < WIFI_SSID_MAX_LEN) {
            memcpy((void *)p_ssid, (void *)s_st_wifi_state.ssid, ssid_len);
        } else {
            memcpy((void *)p_ssid, (void *)s_st_wifi_state.ssid, WIFI_SSID_MAX_LEN);
        }
    } else {
        /* 这里只是为了匹配 if，什么也不做 */
    }

    return state;
}

esp32_wifi_run_cmd_cfg_t* esp32_wifi_run_cmd_cfg_get(void)
{
    return &s_st_run_cmd_cfg;
}
