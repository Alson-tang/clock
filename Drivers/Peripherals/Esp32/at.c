#include "main.h"

#define AT_CMD_BUF_INIT                                                 \
    do {                                                                \
        memset((void*)s_arr_cmd, 0, AT_CMD_BUF_MAX_LEN);                \
        memset((void*)&s_st_cur_at_cmd_cfg, 0, sizeof(at_cmd_cfg_t));   \
    } while (0);

#define AT_CUR_CMD_CFG_INIT(cmd, p)                                                             \
    do {                                                                                        \
        memcpy((void*)&s_st_cur_at_cmd_cfg, (void*)&s_arr_cmd_cfg[cmd], sizeof(at_cmd_cfg_t));  \
        s_arr_cmd_cfg[cmd].p_para = p;                                                          \
        s_st_cur_at_cmd_cfg.p_para = p;                                                         \
    } while (0);

extern at_status __cmd_cwstate_cb(const char *p, uint32_t len, void *p_para);
extern at_status __cmd_cipsntptime_cb(const char *p, uint32_t len, void *p_para);
extern at_status __cmd_httpcget_cb(const char *p, uint32_t len, void *p_para);

const static wifi_softap_info_t s_st_wifi_softap_info = {
    .ssid = SOFTAP_SSID_DEF,
    .password = SOFTAP_PASSWORD_DEF,
    .channel = SOFTAP_CHANNEL_DEF,
    .ecn = SOFTAP_ECN_DEF,
    .max_conn = SOFTAP_MAX_CONN_DEF
};
const static wifi_webserver_info_t s_st_wifi_webserver_info = {
    .server_port = WEBSERVER_SERVER_PORT,
    .timeout = WEBSERVER_CONN_TIMEOUT,
};

static uint8_t s_arr_cmd[AT_CMD_BUF_MAX_LEN] = { 0 };
static esp32_hardware_func_t *p_st_esp32_hardware_func = NULL;

static at_cmd_cfg_t s_arr_cmd_cfg[] = {
    { AT_CMD_AT,            "AT",                   "OK",                               1000,    NULL,    NULL },
    { AT_CMD_ATE0,          "ATE0",                 "OK",                               1000,    NULL,    NULL },
    { AT_CMD_CWSTATE,       "AT+CWSTATE?",          "OK",                               5000,    NULL,    __cmd_cwstate_cb },
    { AT_CMD_RESTORE,       "AT+RESTORE",           "ready",                            10000,   NULL,    NULL },
    { AT_CMD_CWMODE,        "AT+CWMODE=",           "OK",                               1000,    NULL,    NULL },
    { AT_CMD_CWSAP,         "AT+CWSAP=",            "OK",                               3000,    NULL,    NULL },
    { AT_CMD_CIPMUX,        "AT+CIPMUX=",           "OK",                               1000,    NULL,    NULL },
    { AT_CMD_WEBSERVER,     "AT+WEBSERVER=",        "OK",                               30000,   NULL,    NULL },
    { AT_CMD_CWSTARTSMART,  "AT+CWSTARTSMART=",     "smartconfig connected wifi",       60000,   NULL,    NULL },
    { AT_CMD_CIPSNTPCFG,    "AT+CIPSNTPCFG=1,8",    "OK",                               2000,    NULL,    NULL },
    { AT_CMD_CIPSNTPTIME,   "AT+CIPSNTPTIME?",      "OK",                               20000,   NULL,    __cmd_cipsntptime_cb },
    { AT_CMD_HTTPCGET,      "AT+HTTPCGET=",         "OK",                               10000,   NULL,    __cmd_httpcget_cb },
};
static at_cmd_cfg_t s_st_cur_at_cmd_cfg = { AT_CMD_AT, NULL, NULL, 0, NULL, NULL };

at_status __cmd_cwstate_cb(const char *p, uint32_t len, void *p_para)
{
    char *str = NULL;
    wifi_info_t *p_wifi_state = NULL;
    int state = 0;
    char ssid[SSID_MAX_LEN + 2] = { 0 };

    p_wifi_state = (wifi_info_t*)p_para;

    str = strstr(p, "+CWSTATE:");
    if (str == NULL) {
        return AT_STATUS_ERROR;
    } else {
        sscanf(str, "+CWSTATE:%d, %s\r\n", &state, &ssid[0]);

        p_wifi_state->state = (wifi_state_t)state;
        memcpy((void*)p_wifi_state->ssid, (void*)&ssid[1], strlen(ssid) - 2);

        return AT_STATUS_OK;
    }
}

/* +CIPSNTPTIME:Sun Nov 10 17:36:56 2024 */
at_status __cmd_cipsntptime_cb(const char *p, uint32_t len, void *p_para)
{
    char *str = NULL;
    char *index = NULL;
    sntp_time_t sntp_time = { 0 };

    str = strstr(p, "+CIPSNTPTIME:");
    if (str == NULL) {
        return AT_STATUS_ERROR;
    } else {
        /* 格式化字符串为 +CIPSNTPTIME:Sun,Nov,10,17:36:56,2024 形式 */
        index = strstr(str, " ");
        while (index != NULL) {
            *index = ',';
            index = strstr(str, " ");
        };

        sscanf(str,
            "+CIPSNTPTIME:%3s,%3s,%d,%d:%d:%d,%d\r\n",
            sntp_time.week,
            sntp_time.month,
            &sntp_time.day,
            &sntp_time.hour,
            &sntp_time.min,
            &sntp_time.sec,
            &sntp_time.year);

        memcpy((void*)p_para, (void*)&sntp_time, sizeof(sntp_time_t));

        return AT_STATUS_OK;
    }
}

/* +HTTPCGET:258,{"results":[{"location":{"id":"WTTE97PU94T9","name":"Wuxi","country":"CN","path":"Wuxi,Wuxi,Jiangsu,China","timezone":"Asia/Shanghai","timezone_offset":"+08:00"},"now":{"text":"Sunny","code":"0","temperature":"9"},"last_update":"2024-12-15T13:44:06+08:00"}]} */
at_status __cmd_httpcget_cb(const char *p, uint32_t len, void *p_para)
{
    cJSON *p_json = NULL;
    char *p_str = NULL;
    char *p_json_str = NULL;
    int str_len = 0;
    char text[16] = { 0 };
    char code[16] = { 0 };
    char temp[16] = { 0 };
    weather_info_t *p_weather_info = (weather_info_t *)p_para;

    sscanf(p, "+HTTPCGET:%d,", &str_len);
    if (str_len == 0) {
        return AT_STATUS_ERROR;
    } else {
        p_str = strstr(p, ",");
        p_json_str = (char*)malloc(str_len + 1);
        if (p_json_str == NULL) {
            return AT_STATUS_ERROR;
        } else {
            memset((void*)p_json_str, 0, str_len + 1);
            memcpy((void*)p_json_str, (void*)(p_str + 1), str_len);

            json_item_value_get(p_json_str, "text", text, 16);
            json_item_value_get(p_json_str, "code", code, 16);
            json_item_value_get(p_json_str, "temperature", temp, 16);

            memcpy((void*)p_weather_info->text, (void*)text, WEATHER_TEXT);
            p_weather_info->code = atoi(code);
            p_weather_info->temp = atoi(temp);

            free(p_json_str);
        }
    }
}

void at_init(esp32_hardware_func_t *p_func)
{
    p_st_esp32_hardware_func = p_func;
}

at_cmd_cfg_t* at_cmd_cfg_get(void)
{
    return &s_st_cur_at_cmd_cfg;
}

static at_status __at_cmd_send(at_cmd_e cmd, uint8_t *p_buf, void *p_para)
{
    ESP32_CLEAN_BITS;
    AT_CUR_CMD_CFG_INIT(cmd, p_para);
    p_st_esp32_hardware_func->write(p_buf, strlen((char*)p_buf), ESP32_CMD_TX_TIMEOUT);

    if (esp32_wait_response(s_arr_cmd_cfg[cmd].timeout) == ESP32_STATUS_OK) {
        return AT_STATUS_OK;
    } else {
        return AT_STATUS_TIMEOUT;
    }
}

at_status at_check(void)
{
    at_cmd_e cmd = AT_CMD_AT;

    AT_CMD_BUF_INIT;
    snprintf((char*)s_arr_cmd, AT_CMD_BUF_MAX_LEN, "%s\r\n", s_arr_cmd_cfg[cmd].p_request);

    return __at_cmd_send(cmd, s_arr_cmd, NULL);
}

at_status at_echo(void)
{
    at_cmd_e cmd = AT_CMD_ATE0;

    AT_CMD_BUF_INIT;
    snprintf((char*)s_arr_cmd, AT_CMD_BUF_MAX_LEN, "%s\r\n", s_arr_cmd_cfg[cmd].p_request);

    return __at_cmd_send(cmd, s_arr_cmd, NULL);
}

at_status at_wifi_info_get(wifi_info_t *p_st_wifi_state)
{
    at_cmd_e cmd = AT_CMD_CWSTATE;

    AT_CMD_BUF_INIT;
    snprintf((char*)s_arr_cmd, AT_CMD_BUF_MAX_LEN, "%s\r\n", s_arr_cmd_cfg[cmd].p_request);

    return __at_cmd_send(cmd, s_arr_cmd, p_st_wifi_state);
}

at_status at_restore(void)
{
    at_cmd_e cmd = AT_CMD_RESTORE;

    AT_CMD_BUF_INIT;
    snprintf((char*)s_arr_cmd, AT_CMD_BUF_MAX_LEN, "%s\r\n", s_arr_cmd_cfg[cmd].p_request);

    return __at_cmd_send(cmd, s_arr_cmd, NULL);
}

at_status at_mode_set(wifi_mode_t mode)
{
    at_cmd_e cmd = AT_CMD_CWMODE;

    AT_CMD_BUF_INIT;
    snprintf((char*)s_arr_cmd, AT_CMD_BUF_MAX_LEN, "%s%d\r\n", s_arr_cmd_cfg[cmd].p_request, mode);

    return __at_cmd_send(cmd, s_arr_cmd, NULL);
}

at_status at_softap_set(wifi_softap_info_t *p_st_wifi_softap_info)
{
    at_cmd_e cmd = AT_CMD_CWSAP;
    wifi_softap_info_t info = { 0 };

    if (p_st_wifi_softap_info == NULL) {
        memcpy((void*)&info, (void*)&s_st_wifi_softap_info, sizeof(wifi_softap_info_t));
    } else {
        memcpy((void*)&info, (void*)p_st_wifi_softap_info, sizeof(wifi_softap_info_t));
    }

    AT_CMD_BUF_INIT;
    snprintf((char*)s_arr_cmd,
            AT_CMD_BUF_MAX_LEN,
            "%s\"%s\",\"%s\",%d,%d,%d\r\n",
            s_arr_cmd_cfg[cmd].p_request,
            info.ssid,
            info.password,
            info.channel,
            info.ecn,
            info.max_conn);

    return __at_cmd_send(cmd, s_arr_cmd, NULL);
}

at_status at_cpimux_set(FunctionalState state)
{
    at_cmd_e cmd = AT_CMD_CIPMUX;

    AT_CMD_BUF_INIT;

    if (state == ENABLE) {
        snprintf((char*)s_arr_cmd, AT_CMD_BUF_MAX_LEN, "%s%d\r\n", s_arr_cmd_cfg[cmd].p_request, 1);
    } else {
        snprintf((char*)s_arr_cmd, AT_CMD_BUF_MAX_LEN, "%s%d\r\n", s_arr_cmd_cfg[cmd].p_request, 0);
    }

    return __at_cmd_send(cmd, s_arr_cmd, NULL);
}

at_status at_webserver_set(FunctionalState state, wifi_webserver_info_t *p_st_wifi_webserver_info)
{
    at_cmd_e cmd = AT_CMD_WEBSERVER;
    wifi_webserver_info_t info = { 0 };

    AT_CMD_BUF_INIT;

    if (state == ENABLE) {
        if (p_st_wifi_webserver_info == NULL) {
            memcpy((void*)&info, (void*)&s_st_wifi_webserver_info, sizeof(wifi_webserver_info_t));
        } else {
            memcpy((void*)&info, (void*)p_st_wifi_webserver_info, sizeof(wifi_webserver_info_t));
        }

        snprintf((char*)s_arr_cmd,
                AT_CMD_BUF_MAX_LEN,
                "%s%d,%d,%d\r\n",
                s_arr_cmd_cfg[cmd].p_request,
                1,
                info.server_port,
                info.timeout);
    } else {
        snprintf((char*)s_arr_cmd, AT_CMD_BUF_MAX_LEN, "%s%d\r\n", s_arr_cmd_cfg[cmd].p_request, 0);
    }

    return __at_cmd_send(cmd, s_arr_cmd, NULL);
}

/**
  *********************************************************************************************************
  * 函 数 名： at_smartconfig_set
  * 功能说明： SmartConfig 配置
  * 形    参： type：类型，目前仅支持 SMARTCONFIG_AIRKISS
  * 返 回 值： AT_STATUS_OK：执行成功
  *           AT_STATUS_ERROR：执行失败
  *           AT_STATUS_TIMEOUT：执行超时
  *********************************************************************************************************
  */
at_status at_smartconfig_set(smartconfig_type_t type)
{
    at_cmd_e cmd = AT_CMD_CWSTARTSMART;

    AT_CMD_BUF_INIT;

    if (type == SMARTCONFIG_AIRKISS) {
        snprintf((char*)s_arr_cmd, AT_CMD_BUF_MAX_LEN, "%s%d\r\n", s_arr_cmd_cfg[cmd].p_request, (int)SMARTCONFIG_AIRKISS);

        return __at_cmd_send(cmd, s_arr_cmd, NULL);
    } else {
        return AT_STATUS_OK;
    }
}

at_status at_sntp_set(void)
{
    at_cmd_e cmd = AT_CMD_CIPSNTPCFG;

    AT_CMD_BUF_INIT;

    snprintf((char*)s_arr_cmd, AT_CMD_BUF_MAX_LEN, "%s,\"%s\",\"%s\"\r\n", s_arr_cmd_cfg[cmd].p_request, SNTP_ADDR_1_DEF, SNTP_ADDR_2_DEF);

    return __at_cmd_send(cmd, s_arr_cmd, NULL);
}

at_status at_sntp_time_get(sntp_time_t *p_sntp_time)
{
    at_cmd_e cmd = AT_CMD_CIPSNTPTIME;

    AT_CMD_BUF_INIT;

    snprintf((char*)s_arr_cmd, AT_CMD_BUF_MAX_LEN, "%s\r\n", s_arr_cmd_cfg[cmd].p_request);

    return __at_cmd_send(cmd, s_arr_cmd, p_sntp_time);
}

at_status at_httpclient_get(char *p_key, char *p_location, weather_info_t *p_weather_info)
{
    at_cmd_e cmd = AT_CMD_HTTPCGET;
    uint8_t *p_url = NULL;

    AT_CMD_BUF_INIT;

    p_url = (uint8_t*)malloc(HTTP_WEATHER_SERVER_URL_MAX_LEN);
    if (p_url == NULL) {
        return AT_STATUS_ERROR;
    } else {
        snprintf((char*)p_url,
                HTTP_WEATHER_SERVER_URL_MAX_LEN,
                HTTP_WEATHER_SERVER_URL,
                p_key,
                p_location);

        snprintf((char*)s_arr_cmd,
                AT_CMD_BUF_MAX_LEN,
                "%s\"%s\"\r\n",
                s_arr_cmd_cfg[cmd].p_request,
                (char*)p_url);

        free(p_url);
    }

    return __at_cmd_send(cmd, s_arr_cmd, p_weather_info);
}
