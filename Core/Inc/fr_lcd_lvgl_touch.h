

#ifndef INC_LCD_LVGL_TOUCH_H_
#define INC_LCD_LVGL_TOUCH_H_

/*********************
 *      INCLUDES
 *********************/
#include <stdint.h>
#include "../../lvgl/lvgl.h"
#include "fr_touch.h"
#include <main.h>
#include "fr_global.h"

/*********************
 *      DEFINES
 *********************/
#define DISP_HOR_RES 320
#define DISP_VER_RES 240


/**********************
 * GLOBAL PROTOTYPES
 **********************/
void Display_init(int rotation);
static void tft_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p);

void DMA_Handler (void);
#endif /* INC_LCD_LVGL_TOUCH_H_ */
