#ifndef __JSON_H__
#define __JSON_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/* 心知天气返回天气 JSON 示例 */
/*
{
    "results": [
        {
            "location": {
                "id": "WTTE97PU94T9",
                "name": "Wuxi",
                "country": "CN",
                "path": "Wuxi,Wuxi,Jiangsu,China",
                "timezone": "Asia/Shanghai",
                "timezone_offset": "+08:00"
            },
            "now": {
                "text": "Sunny",
                "code": "0",
                "temperature": "9"
            },
            "last_update": "2024-12-15T13:27:20+08:00"
        }
    ]
}
*/

#define JSON_STATUS_OK                         (0)
#define JSON_STATUS_ERROR                      (-1)

typedef int8_t json_status;

json_status json_item_value_get(const char *p_value, const char *p_item, char *p_string, uint32_t len);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* json.h */

