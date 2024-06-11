#ifndef __FONT_H__
#define __FONT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define FONT_ASCII_GENERAL_COL          (12)
#define FONT_ASCII_GENERAL_ROW          (24)
#define FONT_CHINESE_CHAR_GENERAL_COL   (24)
#define FONT_CHINESE_CHAR_GENERAL_ROW   (24)
#define FONT_CLOCK_NUM_HM_COL           (40)
#define FONT_CLOCK_NUM_HM_ROW           (96)
#define FONT_CLOCK_NUM_S_COL            (24)
#define FONT_CLOCK_NUM_S_ROW            (48)

uint16_t utf8_to_gbk(uint8_t *utf8);
void get_font_dis_array(uint16_t gbk, uint8_t *buf, uint32_t len);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* font.h */
