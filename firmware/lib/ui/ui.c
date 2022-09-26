// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.0.5
// LVGL VERSION: 8.2
// PROJECT: MEM

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_InitScreen;
lv_obj_t * ui_LabelVersion;
lv_obj_t * ui_LabelInitSensors;
lv_obj_t * ui_SpinnerInitSensor;
lv_obj_t * ui_LabelInitSensorStatus;
lv_obj_t * ui_InitGpsFix;
lv_obj_t * ui_SpinnerGpsFix;
lv_obj_t * ui_LabelGpsFixStatus;
lv_obj_t * ui_InitConnection;
lv_obj_t * ui_SpinnerConnection;
lv_obj_t * ui_LabelConnectionStatus;
lv_obj_t * ui_ImageLogo;
lv_obj_t * ui_Measurement;
lv_obj_t * ui_ImageLogo1;
lv_obj_t * ui_ImageTemp;
lv_obj_t * ui_ImageHum;
lv_obj_t * ui_ImageUv;
lv_obj_t * ui_ImageDust;
lv_obj_t * ui_ImageLocation;
lv_obj_t * ui_ImageGpsConnection;
lv_obj_t * ui_ImageWanConnection;
lv_obj_t * ui_ImagaSat;
lv_obj_t * ui_LabelTemp;
lv_obj_t * ui_LabelHum;
lv_obj_t * ui_LabelUv;
lv_obj_t * ui_LabelDust;
lv_obj_t * ui_LabelLocation;
lv_obj_t * ui_LabelSat;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=0
    #error "#error LV_COLOR_16_SWAP should be 0 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

// FUNCTION
void HideAnimation_Animation(lv_obj_t * TargetObject, int delay)
{

    //
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 500);
    lv_anim_set_user_data(&PropertyAnimation_0, TargetObject);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 255, 0);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_in);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_start(&PropertyAnimation_0);

}

// FUNCTION
void ShowAnimation_Animation(lv_obj_t * TargetObject, int delay)
{

    //
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 500);
    lv_anim_set_user_data(&PropertyAnimation_0, TargetObject);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 0, 255);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_in);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_opacity);
    lv_anim_start(&PropertyAnimation_0);

}

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////
void ui_InitScreen_screen_init(void)
{

    // ui_InitScreen

    ui_InitScreen = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_InitScreen, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_bg_color(ui_InitScreen, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_InitScreen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_LabelVersion

    ui_LabelVersion = lv_label_create(ui_InitScreen);

    lv_obj_set_width(ui_LabelVersion, 240);
    lv_obj_set_height(ui_LabelVersion, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_LabelVersion, 0);
    lv_obj_set_y(ui_LabelVersion, 50);

    lv_obj_set_align(ui_LabelVersion, LV_ALIGN_CENTER);

    lv_label_set_long_mode(ui_LabelVersion, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(ui_LabelVersion, " v0.0.1");

    lv_obj_set_style_text_align(ui_LabelVersion, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelVersion, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_LabelInitSensors

    ui_LabelInitSensors = lv_label_create(ui_InitScreen);

    lv_obj_set_width(ui_LabelInitSensors, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelInitSensors, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_LabelInitSensors, 0);
    lv_obj_set_y(ui_LabelInitSensors, 70);

    lv_obj_set_align(ui_LabelInitSensors, LV_ALIGN_CENTER);

    lv_label_set_text(ui_LabelInitSensors, "Init sensors");

    lv_obj_set_style_text_font(ui_LabelInitSensors, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_SpinnerInitSensor

    ui_SpinnerInitSensor = lv_spinner_create(ui_InitScreen, 1000, 90);

    lv_obj_set_width(ui_SpinnerInitSensor, 10);
    lv_obj_set_height(ui_SpinnerInitSensor, 10);

    lv_obj_set_x(ui_SpinnerInitSensor, 36);
    lv_obj_set_y(ui_SpinnerInitSensor, 70);

    lv_obj_set_align(ui_SpinnerInitSensor, LV_ALIGN_CENTER);

    lv_obj_clear_flag(ui_SpinnerInitSensor, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_style_arc_color(ui_SpinnerInitSensor, lv_color_hex(0xA3A3A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_SpinnerInitSensor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_SpinnerInitSensor, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_arc_color(ui_SpinnerInitSensor, lv_color_hex(0x77AE94), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_SpinnerInitSensor, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_SpinnerInitSensor, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // ui_LabelInitSensorStatus

    ui_LabelInitSensorStatus = lv_label_create(ui_InitScreen);

    lv_obj_set_width(ui_LabelInitSensorStatus, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelInitSensorStatus, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_LabelInitSensorStatus, 41);
    lv_obj_set_y(ui_LabelInitSensorStatus, 70);

    lv_obj_set_align(ui_LabelInitSensorStatus, LV_ALIGN_CENTER);

    lv_label_set_text(ui_LabelInitSensorStatus, "OK!");

    lv_obj_set_style_text_color(ui_LabelInitSensorStatus, lv_color_hex(0x24C178), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelInitSensorStatus, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelInitSensorStatus, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_LabelInitSensorStatus, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_InitGpsFix

    ui_InitGpsFix = lv_label_create(ui_InitScreen);

    lv_obj_set_width(ui_InitGpsFix, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_InitGpsFix, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_InitGpsFix, 0);
    lv_obj_set_y(ui_InitGpsFix, 85);

    lv_obj_set_align(ui_InitGpsFix, LV_ALIGN_CENTER);

    lv_label_set_text(ui_InitGpsFix, "Get GPS fix");

    lv_obj_set_style_text_font(ui_InitGpsFix, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_SpinnerGpsFix

    ui_SpinnerGpsFix = lv_spinner_create(ui_InitScreen, 1000, 90);

    lv_obj_set_width(ui_SpinnerGpsFix, 10);
    lv_obj_set_height(ui_SpinnerGpsFix, 10);

    lv_obj_set_x(ui_SpinnerGpsFix, 36);
    lv_obj_set_y(ui_SpinnerGpsFix, 85);

    lv_obj_set_align(ui_SpinnerGpsFix, LV_ALIGN_CENTER);

    lv_obj_clear_flag(ui_SpinnerGpsFix, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_style_arc_color(ui_SpinnerGpsFix, lv_color_hex(0xA3A3A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_SpinnerGpsFix, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_SpinnerGpsFix, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_arc_color(ui_SpinnerGpsFix, lv_color_hex(0x77AE94), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_SpinnerGpsFix, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_SpinnerGpsFix, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // ui_LabelGpsFixStatus

    ui_LabelGpsFixStatus = lv_label_create(ui_InitScreen);

    lv_obj_set_width(ui_LabelGpsFixStatus, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelGpsFixStatus, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_LabelGpsFixStatus, 41);
    lv_obj_set_y(ui_LabelGpsFixStatus, 85);

    lv_obj_set_align(ui_LabelGpsFixStatus, LV_ALIGN_CENTER);

    lv_label_set_text(ui_LabelGpsFixStatus, "OK!");

    lv_obj_set_style_text_color(ui_LabelGpsFixStatus, lv_color_hex(0x24C178), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelGpsFixStatus, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelGpsFixStatus, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_LabelGpsFixStatus, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_InitConnection

    ui_InitConnection = lv_label_create(ui_InitScreen);

    lv_obj_set_width(ui_InitConnection, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_InitConnection, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_InitConnection, 0);
    lv_obj_set_y(ui_InitConnection, 100);

    lv_obj_set_align(ui_InitConnection, LV_ALIGN_CENTER);

    lv_label_set_text(ui_InitConnection, "Establishing connection");

    lv_obj_set_style_text_font(ui_InitConnection, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_SpinnerConnection

    ui_SpinnerConnection = lv_spinner_create(ui_InitScreen, 1000, 90);

    lv_obj_set_width(ui_SpinnerConnection, 10);
    lv_obj_set_height(ui_SpinnerConnection, 10);

    lv_obj_set_x(ui_SpinnerConnection, 70);
    lv_obj_set_y(ui_SpinnerConnection, 100);

    lv_obj_set_align(ui_SpinnerConnection, LV_ALIGN_CENTER);

    lv_obj_clear_flag(ui_SpinnerConnection, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_style_arc_color(ui_SpinnerConnection, lv_color_hex(0xA3A3A3), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_SpinnerConnection, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_SpinnerConnection, 1, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_arc_color(ui_SpinnerConnection, lv_color_hex(0x77AE94), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_opa(ui_SpinnerConnection, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_arc_width(ui_SpinnerConnection, 2, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    // ui_LabelConnectionStatus

    ui_LabelConnectionStatus = lv_label_create(ui_InitScreen);

    lv_obj_set_width(ui_LabelConnectionStatus, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelConnectionStatus, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_LabelConnectionStatus, 76);
    lv_obj_set_y(ui_LabelConnectionStatus, 100);

    lv_obj_set_align(ui_LabelConnectionStatus, LV_ALIGN_CENTER);

    lv_label_set_text(ui_LabelConnectionStatus, "OK!");

    lv_obj_set_style_text_color(ui_LabelConnectionStatus, lv_color_hex(0x24C178), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_LabelConnectionStatus, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_LabelConnectionStatus, &lv_font_montserrat_10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_opa(ui_LabelConnectionStatus, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_ImageLogo

    ui_ImageLogo = lv_img_create(ui_InitScreen);
    lv_img_set_src(ui_ImageLogo, &ui_img_urbaniot_logo_png);

    lv_obj_set_width(ui_ImageLogo, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_ImageLogo, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_ImageLogo, 0);
    lv_obj_set_y(ui_ImageLogo, 0);

    lv_obj_set_align(ui_ImageLogo, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_ImageLogo, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ImageLogo, LV_OBJ_FLAG_SCROLLABLE);

}
void ui_Measurement_screen_init(void)
{

    // ui_Measurement

    ui_Measurement = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_Measurement, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_style_bg_color(ui_Measurement, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Measurement, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_ImageLogo1

    ui_ImageLogo1 = lv_img_create(ui_Measurement);
    lv_img_set_src(ui_ImageLogo1, &ui_img_urbaniot_logo_png);

    lv_obj_set_width(ui_ImageLogo1, 75);
    lv_obj_set_height(ui_ImageLogo1, 14);

    lv_obj_set_x(ui_ImageLogo1, 5);
    lv_obj_set_y(ui_ImageLogo1, 5);

    lv_obj_add_flag(ui_ImageLogo1, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ImageLogo1, LV_OBJ_FLAG_SCROLLABLE);

    // ui_ImageTemp

    ui_ImageTemp = lv_img_create(ui_Measurement);
    lv_img_set_src(ui_ImageTemp, &ui_img_1031106234);

    lv_obj_set_width(ui_ImageTemp, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_ImageTemp, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_ImageTemp, -130);
    lv_obj_set_y(ui_ImageTemp, -66);

    lv_obj_set_align(ui_ImageTemp, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_ImageTemp, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ImageTemp, LV_OBJ_FLAG_SCROLLABLE);

    // ui_ImageHum

    ui_ImageHum = lv_img_create(ui_Measurement);
    lv_img_set_src(ui_ImageHum, &ui_img_1121461496);

    lv_obj_set_width(ui_ImageHum, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_ImageHum, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_ImageHum, -128);
    lv_obj_set_y(ui_ImageHum, -20);

    lv_obj_set_align(ui_ImageHum, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_ImageHum, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ImageHum, LV_OBJ_FLAG_SCROLLABLE);

    // ui_ImageUv

    ui_ImageUv = lv_img_create(ui_Measurement);
    lv_img_set_src(ui_ImageUv, &ui_img_1209724868);

    lv_obj_set_width(ui_ImageUv, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_ImageUv, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_ImageUv, -129);
    lv_obj_set_y(ui_ImageUv, 27);

    lv_obj_set_align(ui_ImageUv, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_ImageUv, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ImageUv, LV_OBJ_FLAG_SCROLLABLE);

    // ui_ImageDust

    ui_ImageDust = lv_img_create(ui_Measurement);
    lv_img_set_src(ui_ImageDust, &ui_img_825150755);

    lv_obj_set_width(ui_ImageDust, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_ImageDust, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_ImageDust, -122);
    lv_obj_set_y(ui_ImageDust, 80);

    lv_obj_set_align(ui_ImageDust, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_ImageDust, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ImageDust, LV_OBJ_FLAG_SCROLLABLE);

    // ui_ImageLocation

    ui_ImageLocation = lv_img_create(ui_Measurement);
    lv_img_set_src(ui_ImageLocation, &ui_img_585350205);

    lv_obj_set_width(ui_ImageLocation, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_ImageLocation, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_ImageLocation, 0);
    lv_obj_set_y(ui_ImageLocation, -41);

    lv_obj_set_align(ui_ImageLocation, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_ImageLocation, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ImageLocation, LV_OBJ_FLAG_SCROLLABLE);

    // ui_ImageGpsConnection

    ui_ImageGpsConnection = lv_img_create(ui_Measurement);
    lv_img_set_src(ui_ImageGpsConnection, &ui_img_compass_png);

    lv_obj_set_width(ui_ImageGpsConnection, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_ImageGpsConnection, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_ImageGpsConnection, 147);
    lv_obj_set_y(ui_ImageGpsConnection, -107);

    lv_obj_set_align(ui_ImageGpsConnection, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_ImageGpsConnection, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ImageGpsConnection, LV_OBJ_FLAG_SCROLLABLE);

    // ui_ImageWanConnection

    ui_ImageWanConnection = lv_img_create(ui_Measurement);
    lv_img_set_src(ui_ImageWanConnection, &ui_img_1335595423);

    lv_obj_set_width(ui_ImageWanConnection, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_ImageWanConnection, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_ImageWanConnection, 126);
    lv_obj_set_y(ui_ImageWanConnection, -107);

    lv_obj_set_align(ui_ImageWanConnection, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_ImageWanConnection, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ImageWanConnection, LV_OBJ_FLAG_SCROLLABLE);

    // ui_ImagaSat

    ui_ImagaSat = lv_img_create(ui_Measurement);
    lv_img_set_src(ui_ImagaSat, &ui_img_1681582422);

    lv_obj_set_width(ui_ImagaSat, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_ImagaSat, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_ImagaSat, 0);
    lv_obj_set_y(ui_ImagaSat, 49);

    lv_obj_set_align(ui_ImagaSat, LV_ALIGN_CENTER);

    lv_obj_add_flag(ui_ImagaSat, LV_OBJ_FLAG_ADV_HITTEST);
    lv_obj_clear_flag(ui_ImagaSat, LV_OBJ_FLAG_SCROLLABLE);

    // ui_LabelTemp

    ui_LabelTemp = lv_label_create(ui_Measurement);

    lv_obj_set_width(ui_LabelTemp, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelTemp, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_LabelTemp, -80);
    lv_obj_set_y(ui_LabelTemp, -65);

    lv_obj_set_align(ui_LabelTemp, LV_ALIGN_CENTER);

    lv_label_set_text(ui_LabelTemp, "22.4 °C");

    lv_obj_set_style_text_font(ui_LabelTemp, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_LabelHum

    ui_LabelHum = lv_label_create(ui_Measurement);

    lv_obj_set_width(ui_LabelHum, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelHum, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_LabelHum, -82);
    lv_obj_set_y(ui_LabelHum, -21);

    lv_obj_set_align(ui_LabelHum, LV_ALIGN_CENTER);

    lv_label_set_text(ui_LabelHum, "56.7 %");

    lv_obj_set_style_text_font(ui_LabelHum, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_LabelUv

    ui_LabelUv = lv_label_create(ui_Measurement);

    lv_obj_set_width(ui_LabelUv, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelUv, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_LabelUv, -82);
    lv_obj_set_y(ui_LabelUv, 31);

    lv_obj_set_align(ui_LabelUv, LV_ALIGN_CENTER);

    lv_label_set_text(ui_LabelUv, "3");

    lv_obj_set_style_text_font(ui_LabelUv, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_LabelDust

    ui_LabelDust = lv_label_create(ui_Measurement);

    lv_obj_set_width(ui_LabelDust, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelDust, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_LabelDust, -72);
    lv_obj_set_y(ui_LabelDust, 80);

    lv_obj_set_align(ui_LabelDust, LV_ALIGN_CENTER);

    lv_label_set_text(ui_LabelDust, "PM1.0: 5\nPM2.5: 5\nPM10: 5");

    // ui_LabelLocation

    ui_LabelLocation = lv_label_create(ui_Measurement);

    lv_obj_set_width(ui_LabelLocation, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelLocation, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_LabelLocation, 81);
    lv_obj_set_y(ui_LabelLocation, -40);

    lv_obj_set_align(ui_LabelLocation, LV_ALIGN_CENTER);

    lv_label_set_text(ui_LabelLocation, "Lat: 49.233538\nLng: 6.992772\nAlt: 123.3");

    lv_obj_set_style_text_font(ui_LabelLocation, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

    // ui_LabelSat

    ui_LabelSat = lv_label_create(ui_Measurement);

    lv_obj_set_width(ui_LabelSat, LV_SIZE_CONTENT);
    lv_obj_set_height(ui_LabelSat, LV_SIZE_CONTENT);

    lv_obj_set_x(ui_LabelSat, 56);
    lv_obj_set_y(ui_LabelSat, 50);

    lv_obj_set_align(ui_LabelSat, LV_ALIGN_CENTER);

    lv_label_set_text(ui_LabelSat, "7 Sats\n15 km/h\n5 Hdop");

    lv_obj_set_style_text_font(ui_LabelSat, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

}

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_InitScreen_screen_init();
    ui_Measurement_screen_init();
    lv_disp_load_scr(ui_InitScreen);
}

