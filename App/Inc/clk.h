#ifndef __CLK_H__
#define __CLK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/* 时钟默认显示设置 */
#define CLK_SET_HOUR_DEF                            (12)
#define CLK_SET_MIN_DEF                             (0)
#define CLK_SET_SEC_DEF                             (0)
#define CLK_SET_LOCATION_COL_DEF                    (16)
#define CLK_SET_LOCATION_ROW_DEF                    (104)
#define CLK_SET_COLOR_HOUR_DEF                      COLOR_GREEN
#define CLK_SET_COLOR_MIN_DEF                       COLOR_BLUE
#define CLK_SET_COLOR_SEC_DEF                       COLOR_YELLOW

#define CLK_SET_DEF(clk)                                            \
    do {                                                            \
        clk.clk_time.clk_hour           = CLK_SET_HOUR_DEF;         \
        clk.clk_time.clk_min            = CLK_SET_MIN_DEF;          \
        clk.clk_time.clk_second         = CLK_SET_SEC_DEF;          \
        clk.clk_location.col            = CLK_SET_LOCATION_COL_DEF; \
        clk.clk_location.row            = CLK_SET_LOCATION_ROW_DEF; \
        clk.clk_color.clk_hour_color    = CLK_SET_COLOR_HOUR_DEF;   \
        clk.clk_color.clk_min_color     = CLK_SET_COLOR_MIN_DEF;    \
        clk.clk_color.clk_second_color  = CLK_SET_COLOR_SEC_DEF;    \
    } while(0);

typedef struct clk_time {
    uint8_t clk_hour;
    uint8_t clk_min;
    uint8_t clk_second;
} clk_time_t;

typedef struct clk_location {
    uint16_t col;
    uint16_t row;
} clk_location_t;

typedef struct clk_color {
    uint16_t clk_hour_color;
    uint16_t clk_min_color;
    uint16_t clk_second_color;
} clk_color_t;

typedef struct clk {
    clk_time_t clk_time;
    clk_location_t clk_location;
    clk_color_t clk_color;
} clk_t;

void clk_update(void);
void clk_set(clk_t *p_clk);
void clk_get(clk_t *p_clk);
uint8_t clk_month_convert(char *month);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* clk.h */
