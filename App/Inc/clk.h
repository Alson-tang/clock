#ifndef __CLK_H__
#define __CLK_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

void clk_set(int8_t hour,
                int8_t min,
                int8_t second,
                uint8_t col,
                uint8_t row,
                uint16_t color);

void clk_update(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* clk.h */
