#ifndef __CLK_H__
#define __CLK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

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

void clk_set(clk_t *p_clk);
void clk_update(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* clk.h */
