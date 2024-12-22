#include "main.h"

json_status json_item_value_get(const char *p_value, const char *p_item, char *p_string, uint32_t len)
{
    json_status status = AT_STATUS_OK;
    cJSON *p_json = NULL;
    uint32_t item_str_len = 0;
    uint32_t copy_len = 0;

    p_json = cJSON_Parse(p_value);
    if (p_json == NULL) {
        status = AT_STATUS_ERROR;
    } else {
        cJSON *p_object_results = cJSON_GetObjectItem(p_json, "results");
        cJSON *p_array = cJSON_GetArrayItem(p_object_results, 0);
        cJSON *p_object_now = cJSON_GetObjectItem(p_array, "now");
        cJSON *p_object_item = cJSON_GetObjectItem(p_object_now, p_item);
        char *p = cJSON_GetStringValue(p_object_item);

        if (p == NULL) {
            status = AT_STATUS_ERROR;
        } else {
            item_str_len = strlen(p);
            copy_len = item_str_len <= (len - 1) ? item_str_len : (len - 1);
            memcpy((void*)p_string, (void*)p, copy_len);
            status = AT_STATUS_OK;
        }

        cJSON_Delete(p_json);
    }

    return status;
}
