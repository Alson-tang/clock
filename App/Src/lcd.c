#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "st7789_hal.h"
#include "st7789.h"
#include "font.h"
#include "num.h"
#include "led.h"

typedef struct
{
    uint8_t hour_col;
    uint8_t hour_row;
    uint8_t min_col;
    uint8_t min_row;
    uint8_t second_col;
    uint8_t second_row;
} clk_location_type;

static clk_location_type s_clk_location = { 0 };
static uint16_t s_back_color = COLOR_BLACK;

static void lcd_show_single_ascii(uint16_t color, char c, uint8_t width, uint8_t height)
{
    uint8_t data = 0;
    uint16_t index = 0;
    uint16_t bytes_per_col = 0;
    uint16_t pixel_color = COLOR_BLACK;
    uint16_t loop = 0;

    bytes_per_col = width >> 3;
    if (width & (0x07)) {
        bytes_per_col += 1;
    }

    if ((width == FONT_ASCII_GENERAL_COL) && (height == FONT_ASCII_GENERAL_ROW)) {
        index = c - ' ';

        for (uint16_t i = 0; i < height; i++) {
            for (uint16_t j = 0; j < bytes_per_col; j++) {
                data = asc2_1224[index][(i * bytes_per_col) + j];

                if (((j + 1) << 3) <= (uint16_t)width) {
                    loop = 8;
                } else {
                    loop = ((uint16_t)width - (j << 3));
                }

                for (uint16_t k = 0; k < loop; k++) {
                    if (data & (0x80 >> k)) {
                        pixel_color = color;
                    } else {
                        pixel_color = s_back_color;
                    }

                    st7789_set_color(pixel_color);
                }
            }
        }
    } else {
        index = c - '0';

        for (uint8_t i = 0; i < height; i++) {
            for (uint8_t j = 0; j < bytes_per_col; j++) {
                if ((width == FONT_CLOCK_NUM_HM_COL) && (height == FONT_CLOCK_NUM_HM_ROW)) {
                    data = asc2_4096[index][(i * bytes_per_col) + j];
                } else {
                    data = asc2_2448[index][(i * bytes_per_col) + j];
                }

                for (int8_t k = 7; k >= 0; k--) {
                    if (data & (0x01 << k)) {
                        pixel_color = color;
                    } else {
                        pixel_color = s_back_color;
                    }

                    st7789_set_color(pixel_color);
                }
            }
        }
    }
}

static void lcd_show_single_font(uint16_t color, uint8_t *utf8, uint8_t width, uint8_t height)
{
    uint8_t *buf = NULL;
    uint8_t data = 0;
    uint8_t bytes_per_font = 0;
    uint16_t gbk = 0;
    uint32_t buf_len = 0;
    uint16_t pixel_color = COLOR_BLACK;

    bytes_per_font = width >> 3;

    gbk = utf8_to_gbk(utf8);
    buf_len = ((uint32_t)(bytes_per_font * height));

    buf = (uint8_t *)malloc(buf_len);
    if (buf == NULL) {
        printf("Single font memory malloc failed\r\n");
        return;
    }

    get_font_dis_array(gbk, buf, buf_len);

    for (uint32_t i = 0; i < height; i++) {
        for (uint32_t j = 0; j < bytes_per_font; j++) {
            data = buf[(i * bytes_per_font) + j];
            for (int8_t k = 7; k >= 0; k--) {
                if (data & (0x01 << k)) {
                    pixel_color = color;
                } else {
                    pixel_color = s_back_color;
                }

                st7789_set_color(pixel_color);
            }
        }
    }

    if (buf) {
        free(buf);
        buf = NULL;
    }
}

void lcd_clear_full_screen(uint16_t color)
{
    st7789_screen_clear(color);
    s_back_color = color;
}

void lcd_show_ascii(uint16_t col, uint16_t row, uint16_t color, char *str, uint8_t width, uint8_t height)
{
    char *p = str;

    if (p == NULL) {
        return;
    }

    if ((col >= ST7789_COL) || (row >= ST7789_ROW)) {
        return;
    }

    if (ST7789_COL - col < width) {
        col = 0;
        row += height;
    }

    if (ST7789_ROW - row < height) {
        return;
    }

    while(p[0] != '\0') {
        st7789_set_window(col, row, col + width - 1, row + height - 1);
        lcd_show_single_ascii(color, p[0], width, height);

        p++;
        col += width;

        if (abs(ST7789_COL - col) < width) {
            col = 0;
            row += height;
        }

        if (abs(ST7789_ROW - row) < height) {
            return;
        }
    }
}

void lcd_show_font(uint16_t col, uint16_t row, uint16_t color, uint8_t *font, uint8_t width, uint8_t height)
{
    uint8_t *p = font;

    if (p == NULL) {
        return;
    }

    if ((col >= ST7789_COL) || (row >= ST7789_ROW)) {
        return;
    }

    if (ST7789_COL - col < width) {
        col = 0;
        row += height;
    }

    if (ST7789_ROW - row < height) {
        return;
    }

    while(p[0] != '\0') {
        st7789_set_window(col, row, col + width - 1, row + height - 1);
        lcd_show_single_font(color, p, width, height);

        p += 3;
        col += width;

        if (abs(ST7789_COL - col) < width) {
            col = 0;
            row += height;
        }

        if (abs(ST7789_ROW - row) < height) {
            return;
        }
    }
}

void lcd_show_bmp(uint8_t *raw, uint32_t len)
{
    uint16_t pixel_color = 0;

    for (uint32_t i = 0; i < len; i++) {
        pixel_color = (((uint16_t)(raw[(i << 1) + 1])) << 8) | ((uint16_t)(raw[i << 1]));
        st7789_set_color(pixel_color);
    }
}

void lcd_show_clock_single_num(uint16_t col, uint16_t row, uint16_t color, char num, uint8_t width, uint8_t height)
{
    if ((col >= ST7789_COL) || (row >= ST7789_ROW)) {
        return;
    }

    if (ST7789_COL - col < width) {
        col = 0;
        row += height;
    }

    if (ST7789_ROW - row < height) {
        return;
    }

    st7789_set_window(col, row, col + width - 1, row + height - 1);
    lcd_show_single_ascii(color, num, width, height);
}

void lcd_show_clock_num_h(uint16_t color, char *num)
{
    char value[2] = { '0', '0' };
    uint8_t col = s_clk_location.hour_col;
    uint8_t row = s_clk_location.hour_row;
    uint8_t width = FONT_CLOCK_NUM_HM_COL;
    uint8_t height = FONT_CLOCK_NUM_HM_ROW;

    if (num == NULL) {
        return;
    }

    if ((col >= ST7789_COL) || (row >= ST7789_ROW)) {
        return;
    }

    if (ST7789_COL - col < width) {
        col = 0;
        row += height;
    }

    if (ST7789_ROW - row < height) {
        return;
    }

    if (strlen(num) == 1) {
        memcpy(&value[1], num, 1);
    } else {
        memcpy(value, num, 2);
    }

    for (uint8_t i = 0; i < 2; i++) {
        st7789_set_window(col, row, col + width - 1, row + height - 1);
        lcd_show_single_ascii(color, value[i], width, height);

        col += width;

        if (abs(ST7789_COL - col) < width) {
            col = 0;
            row += height;
        }

        if (abs(ST7789_ROW - row) < height) {
            return;
        }
    }
}

void lcd_show_clock_num_m(uint16_t color, char *num)
{
    char value[2] = { '0', '0' };
    uint8_t col = s_clk_location.min_col;
    uint8_t row = s_clk_location.min_row;
    uint8_t width = FONT_CLOCK_NUM_HM_COL;
    uint8_t height = FONT_CLOCK_NUM_HM_ROW;

    if (num == NULL) {
        return;
    }

    if ((col >= ST7789_COL) || (row >= ST7789_ROW)) {
        return;
    }

    if (ST7789_COL - col < width) {
        col = 0;
        row += height;
    }

    if (ST7789_ROW - row < height) {
        return;
    }

    if (strlen(num) == 1) {
        memcpy(&value[1], num, 1);
    } else {
        memcpy(value, num, 2);
    }

    for (uint8_t i = 0; i < 2; i++) {
        st7789_set_window(col, row, col + width - 1, row + height - 1);
        lcd_show_single_ascii(color, value[i], width, height);

        col += width;

        if (abs(ST7789_COL - col) < width) {
            col = 0;
            row += height;
        }

        if (abs(ST7789_ROW - row) < height) {
            return;
        }
    }
}

void lcd_show_clock_num_s(uint16_t color, char *num)
{
    char value[2] = { '0', '0' };
    uint8_t col = s_clk_location.second_col;
    uint8_t row = s_clk_location.second_row;
    uint8_t width = FONT_CLOCK_NUM_S_COL;
    uint8_t height = FONT_CLOCK_NUM_S_ROW;

    if (num == NULL) {
        return;
    }

    if ((col >= ST7789_COL) || (row >= ST7789_ROW)) {
        return;
    }

    if (ST7789_COL - col < width) {
        col = 0;
        row += height;
    }

    if (ST7789_ROW - row < height) {
        return;
    }

    if (strlen(num) == 1) {
        memcpy(&value[1], num, 1);
    } else {
        memcpy(value, num, 2);
    }

    for (uint8_t i = 0; i < 2; i++) {
        st7789_set_window(col, row, col + width - 1, row + height - 1);
        lcd_show_single_ascii(color, value[i], width, height);

        col += width;

        if (abs(ST7789_COL - col) < width) {
            col = 0;
            row += height;
        }

        if (abs(ST7789_ROW - row) < height) {
            return;
        }
    }
}

void lcd_show_clock_num(char *hour, uint16_t hour_color, char *min, uint16_t min_color, char *second, uint16_t second_color, uint16_t col, uint16_t row)
{
    if ((hour == NULL) || (min == NULL) || (second == NULL)) {
        return;
    }

    memset((void *)&s_clk_location, 0, sizeof(clk_location_type));
    s_clk_location.hour_col = col;
    s_clk_location.hour_row = row;
    s_clk_location.min_col = col + 2 * FONT_CLOCK_NUM_HM_COL;
    s_clk_location.min_row = row;
    s_clk_location.second_col = col + 4 * FONT_CLOCK_NUM_HM_COL;
    s_clk_location.second_row = row + (FONT_CLOCK_NUM_HM_ROW >> 1) - 9;

    lcd_show_clock_num_h(hour_color, hour);
    lcd_show_clock_num_m(min_color, min);
    lcd_show_clock_num_s(second_color, second);
}

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
void lcd_show_qrcode(int width, uint8_t *p_data, uint8_t expand)
{
    uint16_t pixel_width = 0;
    uint16_t start_col = 0;
    uint16_t start_row = 0;
    uint16_t cur_col = 0;
    uint16_t cur_row = 0;
    uint16_t pixel_color = COLOR_BLACK;

    pixel_width = (uint16_t)(width * expand);

    if ((pixel_width > ST7789_COL) || (pixel_width > ST7789_ROW)) {
        return;
    } else {
        start_col = ((ST7789_COL - pixel_width) >> 1);
        start_row = ((ST7789_ROW - pixel_width) >> 1);

        cur_col = start_col;
        cur_row = start_row;

        for (uint16_t i = 0; i < width; i++) {
            for (uint16_t j = 0; j < width; j++) {
                if (p_data[i * width + j] & 0x01) {
                    pixel_color = COLOR_BLACK;
                } else {
                    pixel_color = COLOR_WHITE;
                }

                st7789_set_window(cur_col, cur_row, cur_col + expand - 1, cur_row + expand - 1);

                for (uint16_t k = 0; k < (expand * expand); k++) {
                    st7789_set_color(pixel_color);
                }

                cur_row += expand;
            }

            cur_col += expand;
            cur_row = start_row;
        }
    }
}

uint16_t lcd_get_back_color(void)
{
    return s_back_color;
}
