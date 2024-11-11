#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "lcd.h"
#include "st7789.h"
#include "clk.h"

#define CHECK_TIME(num, min, max)                                       \
    do {                                                                \
        if (!((num >= min) && (num <= max))) {                          \
            printf("parameter %d is not a valid value\r\n", num);       \
            return;                                                     \
        }                                                               \
    } while(0)

#define CHECK_HOUR(hour)                        CHECK_TIME(hour, 0, 23)
#define CHECK_MIN(min)                          CHECK_TIME(min, 0, 59)
#define CHECK_SECOND(second)                    CHECK_TIME(second, 0, 59)

static clk_t s_st_clk = { 0 };

void clk_update(void)
{
    char h[2] = { 0 };
    char m[2] = { 0 };
    char s[2] = { 0 };
    bool min_update = false;
    bool hour_update = false;

    s_st_clk.clk_time.clk_second += 1;

    if (s_st_clk.clk_time.clk_second == 60) {
        s_st_clk.clk_time.clk_min += 1;
        s_st_clk.clk_time.clk_second = 0;
        min_update = true;
    }
    sprintf(s, "%d", s_st_clk.clk_time.clk_second);
    lcd_show_clock_num_s(s_st_clk.clk_color.clk_second_color, s);

    if (min_update) {
        if (s_st_clk.clk_time.clk_min == 60) {
            s_st_clk.clk_time.clk_hour += 1;
            s_st_clk.clk_time.clk_min = 0;
            hour_update = true;
        }
        sprintf(m, "%d", s_st_clk.clk_time.clk_min);
        lcd_show_clock_num_m(s_st_clk.clk_color.clk_min_color, m);
    }

    if (hour_update) {
        if (s_st_clk.clk_time.clk_hour == 24) {
            s_st_clk.clk_time.clk_hour = 0;
        }
        sprintf(h, "%d", s_st_clk.clk_time.clk_hour);
        lcd_show_clock_num_h(s_st_clk.clk_color.clk_hour_color, h);
    }
}

void clk_set(clk_t *p_clk)
{
    char h[2] = { 0 };
    char m[2] = { 0 };
    char s[2] = { 0 };

    CHECK_HOUR(p_clk->clk_time.clk_hour);
    CHECK_MIN(p_clk->clk_time.clk_min);
    CHECK_SECOND(p_clk->clk_time.clk_second);

    memset((void *)&s_st_clk, 0, sizeof(clk_t));
    memcpy((void *)&s_st_clk, (void *)p_clk, sizeof(clk_t));

    sprintf(h, "%d", s_st_clk.clk_time.clk_hour);
    sprintf(m, "%d", s_st_clk.clk_time.clk_min);
    sprintf(s, "%d", s_st_clk.clk_time.clk_second);

    lcd_show_clock_num(h,
                    s_st_clk.clk_color.clk_hour_color,
                    m,
                    s_st_clk.clk_color.clk_min_color,
                    s,
                    s_st_clk.clk_color.clk_second_color,
                    s_st_clk.clk_location.col,
                    s_st_clk.clk_location.row);
}

void clk_get(clk_t *p_clk)
{
    memcpy((void*)p_clk, (void*)&s_st_clk, sizeof(clk_t));
}

uint8_t clk_month_convert(char *month)
{
    uint8_t month_num = 1;

    if (strstr(month, "Jan") != NULL) {
        month_num = 1;
    }

    if (strstr(month, "Feb") != NULL) {
        month_num = 2;
    }

    if (strstr(month, "Mar") != NULL) {
        month_num = 3;
    }

    if (strstr(month, "Apr") != NULL) {
        month_num = 4;
    }

    if (strstr(month, "May") != NULL) {
        month_num = 5;
    }

    if (strstr(month, "Jun") != NULL) {
        month_num = 6;
    }

    if (strstr(month, "Jul") != NULL) {
        month_num = 7;
    }

    if (strstr(month, "Aug") != NULL) {
        month_num = 8;
    }

    if (strstr(month, "Sep") != NULL) {
        month_num = 9;
    }

    if (strstr(month, "Oct") != NULL) {
        month_num = 10;
    }

    if (strstr(month, "Nov") != NULL) {
        month_num = 11;
    }

    if (strstr(month, "Dec") != NULL) {
        month_num = 12;
    }

    return month_num;
}
