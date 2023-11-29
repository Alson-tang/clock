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

void weather_thermometr_show(void);
void weather_temperature_bar_show(int8_t temp, uint16_t color);
void weather_show(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* weather.h */
