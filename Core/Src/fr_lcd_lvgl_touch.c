/*
 * lcd_lvgl.c
 *
 *  Created on: Sep 19, 2022
 *      Author: Filip
 */


#include "fr_lcd_lvgl_touch.h"
#include "../../lv_conf.h"
#include "../../lvgl/lvgl.h"
#include "fr_lcd.h"
#include "string.h"

#define MAX(a,b) ((a)>(b) ? (a):(b))

uint16_t DISP_fb[DISP_HOR_RES*30]; // LCD FRAME Buffer for 2 ROW
lv_disp_drv_t disp_drv;

static lv_color_t disp_buf1[DISP_HOR_RES * 30];
static lv_color_t disp_buf2[DISP_HOR_RES * 30];

static volatile uint32_t t_saved = 0;

// Monitor time taken to render a frame
void monitor_cb(lv_disp_drv_t * d, uint32_t t, uint32_t p)
{
	t_saved = t;
}

// Initializes the Display.
void display_init(int rotation)
{
    static lv_disp_draw_buf_t buf;

    // Initialize the display draw buffer.
    // The buffer size is set to horizontal resolution times 30.
    lv_disp_draw_buf_init(&buf, disp_buf1, disp_buf2, DISP_HOR_RES * 30);

    // Initialize the display driver.
    lv_disp_drv_init(&disp_drv);

    // Set the driver's buffer to the one we initialized.
    disp_drv.draw_buf = &buf;

    // Set the flush callback function which sends the buffer's contents to the display.
    disp_drv.flush_cb = tft_flush;

    // Set the monitor callback function which logs drawing performance.
    disp_drv.monitor_cb = monitor_cb;

    // Set the display resolution.
    disp_drv.hor_res = DISP_HOR_RES;
    disp_drv.ver_res = DISP_VER_RES;

    // Set the display rotation.
    disp_drv.rotated = rotation;

    // Set the color format.
    disp_drv.color_format = LV_COLOR_FORMAT_NATIVE_REVERSE;

    // Register the driver to LittlevGL.
    lv_disp_drv_register(&disp_drv);
}

// Function to update a specific area of the display with new pixel data.
/**
 * Flush a color buffer
 * @param x1 left coordinate of the rectangle
 * @param x2 right coordinate of the rectangle
 * @param y1 top coordinate of the rectangle
 * @param y2 bottom coordinate of the rectangle
 * @param color_p pointer to an array of colors
 */
static void tft_flush(lv_disp_drv_t * drv, const lv_area_t * area, lv_color_t * color_p)
{
    // Check if the given area is out of screen bounds, if so return immediately.
    if(area->x2 < 0 || area->y2 < 0 || area->x1 > DISP_HOR_RES - 1 || area->y1 > DISP_VER_RES - 1) {
        return;
    }

    // Index variable for tracking the position in the framebuffer.
    int index = 0;

    // Iterate over the given area's pixels.
    for(int y = area->y1; y <= area->y2; y++) {
        for(int x = area->x1; x <= area->x2; x++) {
            // Set the framebuffer's corresponding pixel color.
            DISP_fb[index] = color_p->full;

            // Increment the color and index pointers.
            color_p++;
            index++;
        }
    }

    // Copy the updated area from the framebuffer to the LCD.
    lcd_copy(area->x1, area->x2, area->y1, area->y2, DISP_fb);
}

