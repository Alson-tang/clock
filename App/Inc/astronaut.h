#ifndef __ASTRONAUT_H__
#define __ASTRONAUT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include "st7789.h"

#define ASTRONAUT_COL                   (64)
#define ASTRONAUT_ROW                   (41)
#define ASTRONAUT_SHOW_COL              (176)
#define ASTRONAUT_SHOW_ROW              (184)

#define ASTRONAUT_IMAGE_SIZE            (ASTRONAUT_COL * ASTRONAUT_ROW)

#define ASTRONAUT_FLASH_START_ADDR      (0x200000)

#define ASTRONAUT_TOTAL_FRAME           (10)
#define ASTRONAUT_FRAME_DEFAULT         (0)
#define ASTRONAUT_FRAME_SIZE            (ASTRONAUT_COL * ASTRONAUT_ROW * 2)

#define ASTRONAUT_FRAME_DELAY_MS        (100)

#define ASTRONAUT_COLOR_DEFAULT         (COLOR_WHITE)

void astronaut_show(void);
void astronaut_update(void);

#ifdef __cplusplus
}
#endif /* cplusplus */

#endif /* astronaut.h */
