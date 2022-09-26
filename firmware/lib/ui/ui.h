// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: MEM

#ifndef _MEM_UI_H
#define _MEM_UI_H

#ifdef __cplusplus
extern "C" {
#endif

#if __has_include("lvgl.h")
#include "lvgl.h"
#else
#include "lvgl/lvgl.h"
#endif

extern lv_obj_t * ui_InitScreen;
extern lv_obj_t * ui_LabelVersion;
extern lv_obj_t * ui_LabelInitSensors;
extern lv_obj_t * ui_SpinnerInitSensor;
extern lv_obj_t * ui_LabelInitSensorStatus;
extern lv_obj_t * ui_InitGpsFix;
extern lv_obj_t * ui_SpinnerGpsFix;
extern lv_obj_t * ui_LabelGpsFixStatus;
extern lv_obj_t * ui_InitConnection;
extern lv_obj_t * ui_SpinnerConnection;
extern lv_obj_t * ui_LabelConnectionStatus;
extern lv_obj_t * ui_ImageLogo;
extern lv_obj_t * ui_Measurement;
extern lv_obj_t * ui_ImageLogo1;
extern lv_obj_t * ui_ImageTemp;
extern lv_obj_t * ui_ImageHum;
extern lv_obj_t * ui_ImageUv;
extern lv_obj_t * ui_ImageDust;
extern lv_obj_t * ui_ImageLocation;
extern lv_obj_t * ui_ImageGpsConnection;
extern lv_obj_t * ui_ImageWanConnection;
extern lv_obj_t * ui_ImagaSat;
extern lv_obj_t * ui_LabelTemp;
extern lv_obj_t * ui_LabelHum;
extern lv_obj_t * ui_LabelUv;
extern lv_obj_t * ui_LabelDust;
extern lv_obj_t * ui_LabelLocation;
extern lv_obj_t * ui_LabelSat;


LV_IMG_DECLARE(ui_img_urbaniot_logo_png);    // assets\urbaniot_logo.png
LV_IMG_DECLARE(ui_img_1031106234);    // assets\003-thermometer.png
LV_IMG_DECLARE(ui_img_1121461496);    // assets\006-humidity.png
LV_IMG_DECLARE(ui_img_1209724868);    // assets\005-uv-index.png
LV_IMG_DECLARE(ui_img_825150755);    // assets\002-dust.png
LV_IMG_DECLARE(ui_img_585350205);    // assets\007-placeholder.png
LV_IMG_DECLARE(ui_img_compass_png);    // assets\compass.png
LV_IMG_DECLARE(ui_img_1335595423);    // assets\signal-alt-2.png
LV_IMG_DECLARE(ui_img_1681582422);    // assets\008-satellite.png

void HideAnimation_Animation(lv_obj_t * TargetObject, int delay);
void ShowAnimation_Animation(lv_obj_t * TargetObject, int delay);



void ui_init(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
