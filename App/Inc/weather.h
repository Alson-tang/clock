#ifndef __WEATHER_H__
#define __WEATHER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define WEATHER_THERMOMETR_COL                  (40)
#define WEATHER_THERMOMETR_ROW                  (40)
#define WEATHER_THERMOMETR_SHOW_COL             (16)
#define WEATHER_THERMOMETR_SHOW_ROW             (200)

#define WEATHER_TEMPERATURE_BAR_COL             (50)
#define WEATHER_TEMPERATURE_BAR_ROW             (4)
#define WEATHER_TEMPERATURE_BAR_SHOW_COL        (WEATHER_THERMOMETR_SHOW_COL + WEATHER_TEMPERATURE_BAR_COL)
#define WEATHER_TEMPERATURE_BAR_SHOW_ROW        (((WEATHER_THERMOMETR_ROW - WEATHER_TEMPERATURE_BAR_ROW) >> 1) + WEATHER_THERMOMETR_SHOW_ROW)

#define WEATHER_TEMPERATURE_VALUE_MIN           (-30)
#define WEATHER_TEMPERATURE_VALUE_MAX           (70)

#define WEATHER_IMAGE_COL                       (60)
#define WEATHER_IMAGE_ROW                       (60)
#define WEATHER_IMAGE_SHOW_COL                  (164)
#define WEATHER_IMAGE_SHOW_ROW                  (16)

#define WEATHER_THERMOMETR_IMAGE_SIZE            (WEATHER_THERMOMETR_COL * WEATHER_THERMOMETR_ROW)
#define WEATHER_TEMPERATURE_BAR_IMAGE_SIZE       (WEATHER_TEMPERATURE_BAR_COL * WEATHER_TEMPERATURE_BAR_ROW)
#define WEATHER_IMAGE_SIZE                       (WEATHER_IMAGE_COL * WEATHER_IMAGE_ROW)

#define WEATHER_TEXT                            (16)

/* 读取 FLASH 中天气图标需要用到的宏定义 */
#define WEATHER_IMAGE_START_ADDR_IN_FLASH       (4 * 1024 * 1024)
#define WEATHER_IMAGE_MAX_SIZE                  (8 * 1024)
#define WEATHER_IMAGE_READ_SIZE                 (1 * 1024)
#define WEATHER_IMAGE_HEADER_SIZE               (8)

typedef struct weather_info {
    char text[WEATHER_TEXT];
    int code;
    int temp;
} weather_info_t;

typedef enum weather_image {
    WEATHER_IMAGE_0_SUNNY,
    WEATHER_IMAGE_1_CLEAR,
    WEATHER_IMAGE_2_FAIR,
    WEATHER_IMAGE_3_FAIR,
    WEATHER_IMAGE_4_CLOUDY,
    WEATHER_IMAGE_5_PARTLY_CLOUDY,
    WEATHER_IMAGE_6_PARTLY_CLOUDY,
    WEATHER_IMAGE_7_MOSTLY_CLOUDY,
    WEATHER_IMAGE_8_MOSTLY_CLOUDY,
    WEATHER_IMAGE_9_OVERCAST,
    WEATHER_IMAGE_10_SHOWER,
    WEATHER_IMAGE_11_THUNDERSHOWER,
    WEATHER_IMAGE_12_THUNDERSHOWER_WITH_HAIL,
    WEATHER_IMAGE_13_LIGHT_RAIN,
    WEATHER_IMAGE_14_MODERATE_RAIN,
    WEATHER_IMAGE_15_HEAVY_RAIN,
    WEATHER_IMAGE_16_STORM,
    WEATHER_IMAGE_17_HEAVY_STORM,
    WEATHER_IMAGE_18_SEVERE_STORM,
    WEATHER_IMAGE_19_ICE_RAIN,
    WEATHER_IMAGE_20_SLEET,
    WEATHER_IMAGE_21_SNOW_FLURRY,
    WEATHER_IMAGE_22_LIGHT_SNOW,
    WEATHER_IMAGE_23_MODERATE_SNOW,
    WEATHER_IMAGE_24_HEAVY_SNOW,
    WEATHER_IMAGE_25_SNOWSTORM,
    WEATHER_IMAGE_26_DUST,
    WEATHER_IMAGE_27_SAND,
    WEATHER_IMAGE_28_DUSTSTORM,
    WEATHER_IMAGE_29_SANDSTORM,
    WEATHER_IMAGE_30_FOGGY,
    WEATHER_IMAGE_31_HAZE,
    WEATHER_IMAGE_32_WINDY,
    WEATHER_IMAGE_33_BLUSTERY,
    WEATHER_IMAGE_34_HURRICANE,
    WEATHER_IMAGE_35_TROPICAL_STORM,
    WEATHER_IMAGE_36_TORNADO,
    WEATHER_IMAGE_37_COLD,
    WEATHER_IMAGE_38_HOT,
    WEATHER_IMAGE_MAX,
} weather_image_e;

void weather_thermometr_show(void);
void weather_temperature_bar_show(int8_t temp, uint16_t color);
void weather_show(weather_image_e weather_image);
void weather_area_clear(void);
void weather_image_area_clear(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* weather.h */
