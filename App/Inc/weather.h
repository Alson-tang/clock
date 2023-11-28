#ifndef __WEATHER_H__
#define __WEATHER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

#define WEATHER_THERMOMETR_COL                  (40)
#define WEATHER_THERMOMETR_ROW                  (40)
#define WEATHER_THERMOMETR_SHOW_COL             (16)
#define WEATHER_THERMOMETR_SHOW_ROW             (184)

#define WEATHER_TEMPERATURE_BAR_COL             (88)
#define WEATHER_TEMPERATURE_BAR_ROW             (4)
#define WEATHER_TEMPERATURE_BAR_SHOW_COL        (56)
#define WEATHER_TEMPERATURE_BAR_SHOW_ROW        (202)

#define WEATHER_IMAGE_COL                       (60)
#define WEATHER_IMAGE_ROW                       (60)
#define WEATHER_IMAGE_SHOW_COL                  (164)
#define WEATHER_IMAGE_SHOW_ROW                  (16)

#define WEATHER_THERMOMETR_IMAGE_SIZE            (WEATHER_THERMOMETR_COL * WEATHER_THERMOMETR_ROW)
#define WEATHER_TEMPERATURE_BAR_IMAGE_SIZE       (WEATHER_TEMPERATURE_BAR_COL * WEATHER_TEMPERATURE_BAR_ROW)
#define WEATHER_IMAGE_SIZE                       (WEATHER_IMAGE_COL * WEATHER_IMAGE_ROW)

void weather_thermometr_show(void);
void weather_temperature_bar_show(uint8_t temp, uint16_t color);
void weather_show(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* weather.h */
