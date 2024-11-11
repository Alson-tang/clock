#ifndef __LCD_H__
#define __LCD_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/* TFTLCD 默认各信息显示位置 */
#define LCD_SET_CITY_COL_DEF                        (16)
#define LCD_SET_CITY_ROW_DEF                        (16)
#define LCD_SET_CITY_STR_DEF                        ("无锡")

#define LCD_SET_WEATHER_COL_DEF                     (16)
#define LCD_SET_WEATHER_ROW_DEF                     (48)
#define LCD_SET_WEATHER_STR_DEF                     ("晴")

#define LCD_SET_DATE_COL_DEF                        (16)
#define LCD_SET_DATE_ROW_DEF                        (80)
#define LCD_SET_DATE_STR_DEF                        ("2023.11.29 Wed")

void lcd_clear_full_screen(uint16_t color);

void lcd_show_ascii(uint16_t col,
                    uint16_t row,
                    uint16_t color,
                    char *str,
                    uint8_t width,
                    uint8_t height);

void lcd_show_font(uint16_t col,
                    uint16_t row,
                    uint16_t color,
                    uint8_t *font,
                    uint8_t width,
                    uint8_t height);

void lcd_show_bmp(uint8_t *raw, uint32_t len);

void lcd_show_clock_single_num(uint16_t col,
                                uint16_t row,
                                uint16_t color,
                                char num,
                                uint8_t width,
                                uint8_t height);

void lcd_show_clock_num_h(uint16_t color, char *num);

void lcd_show_clock_num_m(uint16_t color, char *num);

void lcd_show_clock_num_s(uint16_t color, char *num);

void lcd_show_clock_num(char *hour,
                        uint16_t hour_color,
                        char *min,
                        uint16_t min_color,
                        char *second,
                        uint16_t second_color,
                        uint16_t col,
                        uint16_t row);

uint16_t lcd_get_back_color(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* screen.h */
