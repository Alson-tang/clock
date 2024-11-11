#include "main.h"

extern esp32_status cmd_cwstate_cb(const char *p, uint32_t len);

// static uint8_t s_arr_cmd[ESP32_CMD_BUF_MAX_LEN] = { 0 };
static uint8_t s_arr_rx_buf1[ESP32_RX_BUF_MAX_LEN] = { 0 };
static uint8_t s_arr_rx_buf2[ESP32_RX_BUF_MAX_LEN] = { 0 };

static esp32_rx_dma_buf_cfg_t s_st_rx_dma_buf_cfg = {
    ESP32_RX_DMA_BUF_1,
    s_arr_rx_buf1,
    s_arr_rx_buf2,
    ESP32_RX_BUF_MAX_LEN,
};

static esp32_info_t s_st_esp32_info = {
    NULL,
    &s_st_rx_dma_buf_cfg,
};

static esp32_hardware_func_t *p_st_esp32_hardware_func = NULL;

esp32_status esp32_wait_response(uint32_t timeout)
{
    EventBits_t bits = 0;
    const TickType_t ticks = (timeout / portTICK_PERIOD_MS);

    bits = xEventGroupWaitBits(g_event_group_handle,
                            APPLICATION_NETWORK_RECV_OK,
                            pdTRUE,
                            pdTRUE,
                            ticks);

    if ((bits & APPLICATION_NETWORK_RECV_OK) != 0) {
        return ESP32_STATUS_OK;
    } else {
        return ESP32_STATUS_TIMEOUT;
    }
}

void esp32_rx_dma_buf_switch(void)
{
    if (s_st_rx_dma_buf_cfg.buf_index == ESP32_RX_DMA_BUF_1) {
        p_st_esp32_hardware_func->buf_switch(s_st_rx_dma_buf_cfg.p_rx_buf2,
                                            s_st_rx_dma_buf_cfg.buf_len);
        s_st_rx_dma_buf_cfg.buf_index = ESP32_RX_DMA_BUF_2;
    } else {
        p_st_esp32_hardware_func->buf_switch(s_st_rx_dma_buf_cfg.p_rx_buf1,
                                            s_st_rx_dma_buf_cfg.buf_len);
        s_st_rx_dma_buf_cfg.buf_index = ESP32_RX_DMA_BUF_1;
    }
}

esp32_info_t *esp32_info_get(void)
{
    at_cmd_cfg_t *p_cfg = NULL;

    p_cfg = at_cmd_cfg_get();
    s_st_esp32_info.p_cmd_cfg = p_cfg;

    return &s_st_esp32_info;
}

void esp32_init(esp32_hardware_func_t *p_func)
{
    p_st_esp32_hardware_func = p_func;
    xEventGroupClearBits(g_event_group_handle, APPLICATION_NETWORK_RECV_OK);
}
