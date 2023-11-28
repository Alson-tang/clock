#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "st7789_hal.h"
#include "st7789.h"
#include "font.h"
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

    if ((width == FONT_ASCII_GENERAL_COL) && (height == FONT_ASCII_GENERAL_ROW)) {
        index = (c - ' ') * 16;

        for (uint8_t i = 0; i < height; i++) {
            data = asc2_0816[index + i];

            for (int8_t y = width - 1; y >= 0; y--) {
                if (data & (0x01 << y)) {
                    pixel_color = color;
                } else {
                    pixel_color = s_back_color;
                }

                st7789_set_color(pixel_color);
            }
        }
    } else {
        index = c - '0';
        bytes_per_col = width >> 3;

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
    st7789_full_screen_clear(color);
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

void lcd_show_clock_num(char *hour, char *min, char *second, uint16_t col, uint16_t row, uint16_t color)
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

    lcd_show_clock_num_h(color, hour);
    lcd_show_clock_num_m(color, min);
    lcd_show_clock_num_s(color, second);
}

uint16_t lcd_get_back_color(void)
{
    return s_back_color;
}
