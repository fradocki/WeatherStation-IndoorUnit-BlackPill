
#include "fr_lv_drv_touch.h"

uint16_t Xread, Yread;

// Function to read touch events from an XPT2046 touch controller using LittlevGL (lvgl) library.
void lv_xpt_2064_read(lv_indev_drv_t * drv, lv_indev_data_t*data)
{
    // If a touch event is detected (TouchState == XPT2046_TOUCHED)
    if(TouchState==XPT2046_TOUCHED)
    {
        // Loop over each sample in the touchtable array
        for(int i=0; i<MAXSAMPLE;i++){
            // Accumulate the x and y coordinates from the touch event
            Xread += touchtable[i].x;
            Yread += touchtable[i].y;
        }

        // Set the state of the input device to "PRESSED"
        data->state = LV_INDEV_STATE_PRESSED;

        // Set the x and y coordinates to the average values
        data->point.x = Xread/MAXSAMPLE;
        data->point.y = Yread/MAXSAMPLE;

        // Reset Xread and Yread for the next set of readings
        Xread=0;
        Yread=0;
    }
    else {
        // If no touch event is detected, set the state of the input device to "RELEASED"
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

