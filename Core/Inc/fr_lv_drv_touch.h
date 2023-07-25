/*
 * lv_drv.h
 *
 *  Created on: Jun 15, 2021
 *      Author: tktof
 */

#ifndef LV_DRV_H_
#define LV_DRV_H_

#include "../../lvgl/lvgl.h"
#include "fr_global.h"
#include "fr_touch.h"

void lv_xpt_2064_read(lv_indev_drv_t * drv, lv_indev_data_t*data);

#endif /* LV_DRV_H_ */
