#ifndef __UPDATE_FONT_LIB_H__
#define __UPDATE_FONT_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stm32f1xx_hal.h"

typedef enum
{
   FONT_LIB_TYPE_GBK,
} font_lib_type;

bool update_font_lib(font_lib_type font_type);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* update_font_lib.h */
