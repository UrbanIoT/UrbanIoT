#pragma once

#include <FreeRTOS.h>
#include <lvgl.h>

class ButtonEvent
{
public:
  bool handled = true;
  lv_indev_state_t state = LV_INDEV_STATE_RELEASED;
};