#ifndef __ST7789_H__
#define __ST7789_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include "stm32f1xx_hal.h"

#define COLOR_WHITE             (0xFFFF)
#define COLOR_BLACK             (0x0000)
#define COLOR_RED               (0xF800)
#define COLOR_GREEN             (0x07E0)
#define COLOR_BLUE              (0x001F)
#define COLOR_YELLOW            (0xFFE0)

typedef struct st7789_cfg
{
    uint16_t column;
    uint16_t row;
    uint8_t rst_hold_time_ms;
    void (*write)(uint8_t * buf, uint16_t len);
    void (*cmd_select)(void);
    void (*data_select)(void);
    void (*rst_select)(void);
    void (*rst_unselect)(void);
} st7789_cfg_t;

void st7789_init(st7789_cfg_t * st7789_cfg);
void st7789_set_color(uint16_t color);
bool st7789_set_window(uint16_t start_col, uint16_t start_row, uint16_t end_col, uint16_t end_row);
void st7789_full_screen_clear(uint16_t color);
void st7789_pixel_draw(uint16_t col, uint16_t row, uint16_t color);


void st7789_show_string(uint16_t col, uint16_t row, char *str);
void st7789_show_chinese_char(uint16_t col, uint16_t row, uint8_t *buf, uint32_t buf_len);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* st7789.h */
