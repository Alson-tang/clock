#include "main.h"

static uint8_t s_arr_handle_buf[ESP32_RX_BUF_MAX_LEN] = { 0 };

void network_rx_task(void *pvParameters)
{
    esp32_info_t *p_esp32_info = NULL;
    esp32_wifi_run_cmd_cfg_t *p_esp32_wifi_run_cmd_cfg = NULL;
    uint8_t *p_src = NULL;
    uint8_t *p_dst = NULL;
    message_t message = { 0 };

    p_esp32_info = esp32_info_get();
    p_esp32_wifi_run_cmd_cfg = esp32_wifi_run_cmd_cfg_get();
    p_dst = s_arr_handle_buf;

    for(;;) {
        xQueueReceive(g_queue_network_handle, (void*)&message, portMAX_DELAY);

        if ((esp32_rx_dma_buf_index_e)message.buf_index == ESP32_RX_DMA_BUF_1) {
            p_src = p_esp32_info->p_rx_dma_buf_cfg->p_rx_buf1;
        } else {
            p_src = p_esp32_info->p_rx_dma_buf_cfg->p_rx_buf2;
        }

        memcpy((void*)p_dst, (const void*)p_src, (size_t)message.recv_len);

        printf("alson-%s, %d, len : %d, content : \r\n%s\r\n", __func__, __LINE__, message.recv_len, p_dst);


        if (strstr((const char*)p_dst, "ready")) {
        } else if (strstr((const char*)p_dst, "ready")) {

        } else if (strstr((const char*)p_dst, (const char*)(p_esp32_wifi_run_cmd_cfg->p_response)) != NULL) {
            if (p_esp32_wifi_run_cmd_cfg->cb != NULL) {
                p_esp32_wifi_run_cmd_cfg->cb((const char *)p_dst, message.recv_len);
            }
            xEventGroupSetBits(g_event_group_handle, APPLICATION_NETWORK_RECV_OK);
        }

        memset((void*)p_dst, 0, (size_t)message.recv_len);
    }
}
