/*
 * touch.h
 *
 *  Created on: Sep 28, 2022
 *      Author: Filip
 */

#ifndef INC_TOUCH_H_
#define INC_TOUCH_H_


#include <stdio.h>
#include <main.h>
#include <fr_lcd.h>
#include "fr_global.h"

#ifndef DEF_TOUCH2046_H
#define DEF_TOUCH2046_H

#define ROTACJA_XPT 1
#define WYS_XPT 320
#define SZER_XPT 240

#define RAMKA_X    0b10010000
#define RAMKA_Y    0b11010000

#define XPT_MAX 0x0fff


#define WDEN (-986149)
#define WA (-88318)
#define WB 599
#define WC 16044793
#define WD (-320)
#define WE (-64760)
#define WF 16144186



//void xpt2046_init(SPI_HandleTypeDef*);
extern touch touchtable[MAXSAMPLE];
extern volatile XPT2046_State TouchState;
extern volatile int touchsample;
extern volatile int touchsample1;
void xpt2046_read(SPI_HandleTypeDef*, volatile int);
void calibration(SPI_HandleTypeDef *_hspi);
void touch_handle(SPI_HandleTypeDef *_hspi);
void touch_irq(void);

#endif /* DEF_TOUCH2046_H */


#endif /* INC_TOUCH_H_ */
