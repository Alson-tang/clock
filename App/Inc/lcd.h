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

/**
  *********************************************************************************************************
  *   函 数 名: lcd_show_qrcode
  *   功能说明: 在屏幕上显示放大后的二维码
  *   形    参: width 二维码宽度
  *   形    参: p_data 二维码点阵
  *   形    参: expand 扩大倍数
  *   说    明：当 expand 设置为 2 时，二维码显示的一个像素点将扩展为 4 个像素点显示，即行用 2 个像素点，列用 2 个
  *            像素点；当 expand 设置为 3 时，二维码显示的一个像素点将扩展为 9 个像素点显示，即行用 3 个像素点，列
  *            用 3 个像素点；以此类推
  *   返 回 值: 无
  *********************************************************************************************************
  */
void lcd_show_qrcode(int width, uint8_t *p_data, uint8_t expand);

uint16_t lcd_get_back_color(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* screen.h */
