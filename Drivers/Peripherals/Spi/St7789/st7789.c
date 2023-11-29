#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "st7789.h"
#include "font.h"

#define ST7789_CMD_MEMORY_DATA_ACCESS_CONTROL                   (0x36)
#define ST7789_DATA_MEMORY_DATA_ACCESS_CONTROL                  (0x00)

#define ST7789_CMD_INTERFACE_PIXEL_FORMAT                       (0x3A)
#define ST7789_DATA_INTERFACE_PIXEL_FORMAT                      (0x05)

#define ST7789_CMD_PORCH_SETTING                                (0xB2)
#define ST7789_DATA_PORCH_SETTING_1ST                           (0x0C)
#define ST7789_DATA_PORCH_SETTING_2ST                           (0x0C)
#define ST7789_DATA_PORCH_SETTING_3ST                           (0x00)
#define ST7789_DATA_PORCH_SETTING_4ST                           (0x33)
#define ST7789_DATA_PORCH_SETTING_5ST                           (0x33)

#define ST7789_CMD_GATE_CONTROL                                 (0xB7)
#define ST7789_DATA_GATE_CONTROL                                (0x35)

#define ST7789_CMD_VCOM_SETTING                                 (0xBB)
#define ST7789_DATA_VCOM_SETTING                                (0x19)

#define ST7789_CMD_LCM_CONTROL                                  (0xC0)
#define ST7789_DATA_LCM_CONTROL                                 (0x2C)

#define ST7789_CMD_VDV_AND_VRH                                  (0xC2)
#define ST7789_DATA_VDV_AND_VRH                                 (0x01)

#define ST7789_CMD_VRH_SET                                      (0xC3)
#define ST7789_DATA_VRH_SET                                     (0x12)

#define ST7789_CMD_VDV_SET                                      (0xC4)
#define ST7789_DATA_VDV_SET                                     (0x20)

#define ST7789_CMD_FRAME_RATE_CONTROL_IN_NORMAL_MODE            (0xC6)
#define ST7789_DATA_FRAME_RATE_CONTROL_IN_NORMAL_MODE           (0x0F)

#define ST7789_CMD_POWER_CONTROL_1                              (0xD0)
#define ST7789_DATA_POWER_CONTROL_1_1ST                         (0xA4)
#define ST7789_DATA_POWER_CONTROL_1_2ST                         (0xA1)

#define ST7789_CMD_POSITIVE_VOLTAGE_GAMMA_CONTROL               (0xE0)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_1ST          (0xD0)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_2ST          (0x04)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_3ST          (0x0D)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_4ST          (0x11)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_5ST          (0x13)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_6ST          (0x2B)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_7ST          (0x3F)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_8ST          (0x54)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_9ST          (0x4C)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_10ST         (0x18)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_11ST         (0x0D)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_12ST         (0x0B)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_13ST         (0x1F)
#define ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_14ST         (0x23)

#define ST7789_CMD_NEGATIVE_VOLTAGE_GAMMA_CONTROL               (0xE1)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_1ST          (0xD0)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_2ST          (0x04)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_3ST          (0x0C)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_4ST          (0x11)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_5ST          (0x13)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_6ST          (0x2C)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_7ST          (0x3F)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_8ST          (0x44)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_9ST          (0x51)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_10ST         (0x2F)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_11ST         (0x1F)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_12ST         (0x1F)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_13ST         (0x20)
#define ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_14ST         (0x23)

#define ST7789_CMD_DISPLAY_INVERSION_ON                         (0x21)
#define ST7789_CMD_SLEEP_OUT                                    (0x11)
#define ST7789_CMD_DISPLAY_ON                                   (0x29)
#define ST7789_CMD_COLUMN_ADDRESS_SET                           (0x2A)
#define ST7789_CMD_ROW_ADDRESS_SET                              (0x2B)
#define ST7789_CMD_MEMORY_WRITE                                 (0x2C)

static st7789_cfg_t s_st7789_cfg = { 0 };

static void st7789_reset(void)
{
    s_st7789_cfg.rst_unselect();
    HAL_Delay(s_st7789_cfg.rst_hold_time_ms);
    s_st7789_cfg.rst_select();
    HAL_Delay(s_st7789_cfg.rst_hold_time_ms);
    s_st7789_cfg.rst_unselect();
    HAL_Delay(s_st7789_cfg.rst_hold_time_ms);
}

static void st7789_write_cmd(uint8_t cmd)
{
    s_st7789_cfg.cmd_select();
    s_st7789_cfg.write(&cmd, 1);
}

static void st7789_write_single_byte_data(uint8_t data)
{
    s_st7789_cfg.data_select();
    s_st7789_cfg.write(&data, 1);
}

void st7789_set_color(uint16_t color)
{
    uint8_t high = (uint8_t)((color >> 8) & 0xFF);
    uint8_t low = (uint8_t)(color & 0xFF);

    st7789_write_single_byte_data(high);
    st7789_write_single_byte_data(low);
}

bool st7789_set_window(uint16_t start_col, uint16_t start_row, uint16_t end_col, uint16_t end_row)
{
    uint8_t high = 0;
    uint8_t low = 0;

    if ((end_col >= s_st7789_cfg.column) || (end_row >= s_st7789_cfg.row)) {
        printf("The set coordinates exceed the screen size\r\n");
        return false;
    }

    if ((start_col > end_col) || (start_row > end_row)) {
        printf("Window coordinates invalid\r\n");
        return false;
    }

    st7789_write_cmd(ST7789_CMD_COLUMN_ADDRESS_SET);

    high = (uint8_t)((start_col >> 8) & 0xFF);
    low = (uint8_t)(start_col & 0xFF);
    st7789_write_single_byte_data(high);
    st7789_write_single_byte_data(low);

    high = (uint8_t)((end_col >> 8) & 0xFF);
    low = (uint8_t)(end_col & 0xFF);
    st7789_write_single_byte_data(high);
    st7789_write_single_byte_data(low);

    st7789_write_cmd(ST7789_CMD_ROW_ADDRESS_SET);

    high = (uint8_t)((start_row >> 8) & 0xFF);
    low = (uint8_t)(start_row & 0xFF);
    st7789_write_single_byte_data(high);
    st7789_write_single_byte_data(low);

    high = (uint8_t)((end_row >> 8) & 0xFF);
    low = (uint8_t)(end_row & 0xFF);
    st7789_write_single_byte_data(high);
    st7789_write_single_byte_data(low);

    st7789_write_cmd(ST7789_CMD_MEMORY_WRITE);

    return true;
}

void st7789_init(st7789_cfg_t * st7789_cfg)
{
    if (st7789_cfg == NULL) {
        return;
    }

    memset((void *)&s_st7789_cfg, 0, sizeof(st7789_cfg_t));
    memcpy((void *)&s_st7789_cfg, (void *)st7789_cfg, sizeof(st7789_cfg_t));

    st7789_reset();

    st7789_write_cmd(ST7789_CMD_MEMORY_DATA_ACCESS_CONTROL);
    st7789_write_single_byte_data(ST7789_DATA_MEMORY_DATA_ACCESS_CONTROL);

    st7789_write_cmd(ST7789_CMD_INTERFACE_PIXEL_FORMAT);
    st7789_write_single_byte_data(ST7789_DATA_INTERFACE_PIXEL_FORMAT);

    st7789_write_cmd(ST7789_CMD_PORCH_SETTING);
    st7789_write_single_byte_data(ST7789_DATA_PORCH_SETTING_1ST);
    st7789_write_single_byte_data(ST7789_DATA_PORCH_SETTING_2ST);
    st7789_write_single_byte_data(ST7789_DATA_PORCH_SETTING_3ST);
    st7789_write_single_byte_data(ST7789_DATA_PORCH_SETTING_4ST);
    st7789_write_single_byte_data(ST7789_DATA_PORCH_SETTING_5ST);

    st7789_write_cmd(ST7789_CMD_GATE_CONTROL);
    st7789_write_single_byte_data(ST7789_DATA_GATE_CONTROL);

    st7789_write_cmd(ST7789_CMD_VCOM_SETTING);
    st7789_write_single_byte_data(ST7789_DATA_VCOM_SETTING);

    st7789_write_cmd(ST7789_CMD_LCM_CONTROL);
    st7789_write_single_byte_data(ST7789_DATA_LCM_CONTROL);

    st7789_write_cmd(ST7789_CMD_VDV_AND_VRH);
    st7789_write_single_byte_data(ST7789_DATA_VDV_AND_VRH);

    st7789_write_cmd(ST7789_CMD_VRH_SET);
    st7789_write_single_byte_data(ST7789_DATA_VRH_SET);

    st7789_write_cmd(ST7789_CMD_VDV_SET);
    st7789_write_single_byte_data(ST7789_DATA_VDV_SET);

    st7789_write_cmd(ST7789_CMD_FRAME_RATE_CONTROL_IN_NORMAL_MODE);
    st7789_write_single_byte_data(ST7789_DATA_FRAME_RATE_CONTROL_IN_NORMAL_MODE);

    st7789_write_cmd(ST7789_CMD_POWER_CONTROL_1);
    st7789_write_single_byte_data(ST7789_DATA_POWER_CONTROL_1_1ST);
    st7789_write_single_byte_data(ST7789_DATA_POWER_CONTROL_1_2ST);

    st7789_write_cmd(ST7789_CMD_POSITIVE_VOLTAGE_GAMMA_CONTROL);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_1ST);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_2ST);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_3ST);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_4ST);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_5ST);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_6ST);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_7ST);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_8ST);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_9ST);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_10ST);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_11ST);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_12ST);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_13ST);
    st7789_write_single_byte_data(ST7789_DATA_POSITIVE_VOLTAGE_GAMMA_CONTROL_14ST);

    st7789_write_cmd(ST7789_CMD_NEGATIVE_VOLTAGE_GAMMA_CONTROL);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_1ST);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_2ST);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_3ST);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_4ST);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_5ST);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_6ST);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_7ST);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_8ST);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_9ST);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_10ST);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_11ST);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_12ST);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_13ST);
    st7789_write_single_byte_data(ST7789_DATA_NEGATIVE_VOLTAGE_GAMMA_CONTROL_14ST);

    st7789_write_cmd(ST7789_CMD_DISPLAY_INVERSION_ON);
    st7789_write_cmd(ST7789_CMD_SLEEP_OUT);
    st7789_write_cmd(ST7789_CMD_DISPLAY_ON);
}

void st7789_full_screen_clear(uint16_t color)
{
    st7789_set_window(0, 0, s_st7789_cfg.column - 1, s_st7789_cfg.row - 1);

    for(uint16_t col = 0; col < s_st7789_cfg.column; col++) {
        for (uint16_t row = 0; row < s_st7789_cfg.row; row++) {
            st7789_set_color(color);
        }
    }
}

void st7789_pixel_draw(uint16_t col, uint16_t row, uint16_t color)
{
    if (st7789_set_window(col, row, col, row) == false) {
        return;
    }

    st7789_set_color(color);
}
