#ifndef __AT_H__
#define __AT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define AT_STATUS_OK                         (0)
#define AT_STATUS_ERROR                      (-1)
#define AT_STATUS_TIMEOUT                    (-2)

#define AT_CMD_BUF_MAX_LEN                  (256)

#define SSID_MAX_LEN                        (32)
#define PASSWORD_MAX_LEN                    (64)

/* 默认 SOFTAP 的设置 */
#define SOFTAP_SSID_DEF                     ("Clock")
#define SOFTAP_PASSWORD_DEF                 ("12345678")
#define SOFTAP_CHANNEL_DEF                  (11)
#define SOFTAP_ECN_DEF                      WIFI_ECN_WPA2_PSK
#define SOFTAP_MAX_CONN_DEF                 (5)

/* 默认 WEBSERVER 的设置 */
#define WEBSERVER_SERVER_PORT               (80)
#define WEBSERVER_CONN_TIMEOUT              (30)

/* 默认 SNTP 服务器地址 */
#define SNTP_ADDR_1_DEF                     "cn.ntp.org.cn"
#define SNTP_ADDR_2_DEF                     "ntp.sjtu.edu.cn"

/* 默认 SNTP 时间结构体设置 */
#define SNTP_WEEK_STR_LEN                   (4)
#define SNTP_MONTH_STR_LEN                  (4)
#define SNTP_DATE_STR_LEN                   (16)

/* 默认 HTTP 服务器设置，使用心知天气，key 要自己申请 */
// #define HTTP_WEATHER_SERVER_URL             "https://api.seniverse.com/v3/weather/now.json?key=%s&location=%s&language=zh-Hans&unit=c"
#define HTTP_WEATHER_SERVER_URL_MAX_LEN     (192)
#define HTTP_WEATHER_SERVER_URL             "http://httpbin.org/get"

typedef int8_t at_status;
typedef at_status (*cmd_cb)(const char *p, uint32_t len, void *p_para);

typedef enum at_cmd {
    AT_CMD_AT,
    AT_CMD_ATE0,
    AT_CMD_CWSTATE,
    AT_CMD_RESTORE,
    AT_CMD_CWMODE,
    AT_CMD_CWSAP,
    AT_CMD_CIPMUX,
    AT_CMD_WEBSERVER,
    AT_CMD_CWSTARTSMART,
    AT_CMD_CIPSNTPCFG,
    AT_CMD_CIPSNTPTIME,
    AT_CMD_HTTPCLIENT,
    AT_CMD_MAX,
} at_cmd_e;

typedef enum wifi_state {
    WIFI_NOT_CONNECTED,
    WIFI_NOT_GOT_IPV4,
    WIFI_GOT_IPV4,
    WIFI_CONNECTING,
    WIFI_DISCONNECTED,
} wifi_state_t;

typedef enum wifi_ecn {
    WIFI_ECN_OPEN,
    WIFI_ECN_WPA_PSK,
    WIFI_ECN_WPA2_PSK,
    WIFI_ECN_WPA_WPA2_PSK,
} wifi_ecn_t;

typedef enum wifi_mode {
    WIFI_MODE_NONE,
    WIFI_MODE_STATION,
    WIFI_MODE_SOFTAP,
    WIFI_MODE_STATION_SOFTAP,
} wifi_mode_t;

typedef enum smartconfig_type {
    SMARTCONFIG_ESP_TOUCH = 1,
    SMARTCONFIG_AIRKISS,
    SMARTCONFIG_ESP_TOUCH_AIRKISS,
    SMARTCONFIG_ESP_TOUCH_V2,
} smartconfig_type_t;

typedef struct at_cmd_cfg {
    at_cmd_e cmd;
    char *p_request;
    char *p_response;
    uint32_t timeout;
    void *p_para;
    cmd_cb cb;
} at_cmd_cfg_t;

typedef struct wifi_info {
    wifi_state_t state;
    char ssid[SSID_MAX_LEN];
} wifi_info_t;

typedef struct wifi_softap_info {
    char ssid[SSID_MAX_LEN];
    char password[PASSWORD_MAX_LEN];
    uint8_t channel;
    wifi_ecn_t ecn;
    uint8_t max_conn;
} wifi_softap_info_t;

typedef struct wifi_webserver_info {
    uint16_t server_port;
    uint16_t timeout;
} wifi_webserver_info_t;

typedef struct sntp_time {
    char week[SNTP_WEEK_STR_LEN];
    char month[SNTP_MONTH_STR_LEN];
    int day;
    int hour;
    int min;
    int sec;
    int year;
} sntp_time_t;

void at_init(esp32_hardware_func_t *p_func);
at_cmd_cfg_t* at_cmd_cfg_get(void);
at_status at_check(void);
at_status at_echo(void);
at_status at_wifi_info_get(wifi_info_t *p_st_wifi_state);
at_status at_restore(void);
at_status at_mode_set(wifi_mode_t mode);
at_status at_softap_set(wifi_softap_info_t *p_st_wifi_softap_info);
at_status at_cpimux_set(FunctionalState state);
at_status at_webserver_set(FunctionalState state, wifi_webserver_info_t *p_st_wifi_webserver_info);
at_status at_smartconfig_set(smartconfig_type_t type);
at_status at_sntp_set(void);
at_status at_sntp_time_get(sntp_time_t *p_sntp_time);
at_status at_httpclient_get(char *p_key, char *p_location);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* at.h */
