#ifndef __ESP32_WIFI_H__
#define __ESP32_WIFI_H__

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_SSID_MAX_LEN                           (32)
#define WIFI_PASSWORD_MAX_LEN                       (64)

typedef enum esp32_cmd {
    ESP32_CMD_AT,
    ESP32_CMD_WIFI_MODE_GET,
    ESP32_CMD_WIFI_MODE_STA_SET,
    ESP32_CMD_WIFI_MODE_SOFTAP_SET,
    ESP32_CMD_WIFI_SMARTCONFIG_START,
    ESP32_CMD_WIFI_SMARTCONFIG_STOP,
    ESP32_CMD_WIFI_SNTP_CFG,
    ESP32_CMD_WIFI_SNTP_TIME,
    ESP32_CMD_WIFI_STATE,
    ESP32_CMD_MAX,
} esp32_cmd_e;

typedef enum esp32_wifi_mode {
    ESP32_WIFI_DISABLE,
    ESP32_WIFI_STATION,
    ESP32_WIFI_SOFTAP,
    ESP32_WIFI_STATION_AND_SOFTAP,
} esp32_wifi_mode_e;

typedef enum esp32_wifi_smartconfig_type {
    ESP32_WIFI_SMARTCONFIG_ESP_TOUCH,
    ESP32_WIFI_AIRKISS,
    ESP32_WIFI_ESP_TOUCH_AIRKISS,
    ESP32_WIFI_ESP_TOUCH_V2,
} esp32_wifi_smartconfig_type_e;

typedef enum esp32_wifi_connect_state {
    ESP32_WIFI_NOT_CONNECTED,
    ESP32_WIFI_CONNECTED,
} esp32_wifi_connect_state_e;

typedef enum esp32_wifi_state {
    ESP32_WIFI_STATE_NOT_STARTED_CONN,
    ESP32_WIFI_STATE_CONNECTED_NO_IPV4,
    ESP32_WIFI_STATE_CONNECTED_IPV4,
    ESP32_WIFI_STATE_CONNECTING,
    ESP32_WIFI_STATE_DISCONNECTED,
} esp32_wifi_state_e;

typedef struct esp32_wifi_cmd_cfg {
    esp32_cmd_e cmd;
    char *p_request;
    char *p_response;
    uint32_t retry_nums;
    uint32_t timeout;
    cmd_cb cb;
} esp32_wifi_cmd_cfg_t;

#pragma pack(1)
typedef struct esp32_wifi_sntp_time {
    uint8_t hour;
    uint8_t min;
    uint8_t second;
} esp32_wifi_sntp_time_t;
#pragma pack()

typedef esp32_wifi_cmd_cfg_t esp32_wifi_run_cmd_cfg_t;

esp32_status esp32_wifi_check(void);
esp32_status esp32_wifi_mode_get(esp32_wifi_mode_e *p_mode);
esp32_status esp32_wifi_mode_set(esp32_wifi_mode_e mode);
esp32_status esp32_wifi_smartconfig_start(void);
esp32_status esp32_wifi_smartconfig_stop(void);
esp32_status esp32_wifi_sntp_set(void);
esp32_status esp32_wifi_sntp_time(esp32_wifi_sntp_time_t *p_sntp);
esp32_status esp32_wifi_state_get(esp32_wifi_state_e *p_state, char *p_ssid, uint16_t ssid_len);
esp32_wifi_run_cmd_cfg_t* esp32_wifi_run_cmd_cfg_get(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* esp32_wifi.h */
