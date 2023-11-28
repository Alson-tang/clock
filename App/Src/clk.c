#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lcd.h"
#include "st7789.h"
#include "clk.h"

#define CHECK_TIME(num, min, max)    \
    do {    \
        if (!((num >= min) && (num <= max))) {    \
            printf("parameter %d is not a valid value\r\n", num);    \
            return;    \
        }    \
    } while(0)

#define CHECK_HOUR(hour)        CHECK_TIME(hour, 0, 23)
#define CHECK_MIN(min)          CHECK_TIME(min, 0, 59)
#define CHECK_SECOND(second)    CHECK_TIME(second, 0, 59)

typedef struct
{
    uint8_t hour;
    uint8_t min;
    uint8_t second;
    uint16_t color;
} clk_time_type;

static clk_time_type s_clk_time = { 0 };

void clk_update(void)
{
    char h[2] = { 0 };
    char m[2] = { 0 };
    char s[2] = { 0 };
    bool min_update = false;
    bool hour_update = false;

    s_clk_time.second += 1;

    if (s_clk_time.second == 60) {
        s_clk_time.min += 1;
        s_clk_time.second = 0;
        min_update = true;
    }
    sprintf(s, "%d", s_clk_time.second);
    lcd_show_clock_num_s(s_clk_time.color, s);

    if (min_update) {
        if (s_clk_time.min == 60) {
            s_clk_time.hour += 1;
            s_clk_time.min = 0;
            hour_update = true;
        }
        sprintf(m, "%d", s_clk_time.min);
        lcd_show_clock_num_m(s_clk_time.color, m);
    }

    if (hour_update) {
        if (s_clk_time.hour == 24) {
            s_clk_time.hour = 0;
        }
        sprintf(h, "%d", s_clk_time.hour);
        lcd_show_clock_num_h(s_clk_time.color, h);
    }
}

void clk_set(int8_t hour, int8_t min, int8_t second, uint8_t col, uint8_t row, uint16_t color)
{
    char h[2] = { 0 };
    char m[2] = { 0 };
    char s[2] = { 0 };

    CHECK_HOUR(hour);
    CHECK_MIN(min);
    CHECK_SECOND(second);

    memset((void *)&s_clk_time, 0, sizeof(clk_time_type));
    s_clk_time.hour = hour;
    s_clk_time.min = min;
    s_clk_time.second = second;
    s_clk_time.color = color;

    sprintf(h, "%d", hour);
    sprintf(m, "%d", min);
    sprintf(s, "%d", second);

    lcd_show_clock_num(h, m, s, col, row, color);
}
