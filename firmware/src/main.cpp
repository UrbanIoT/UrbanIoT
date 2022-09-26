#include <iostream>
#include <memory>
#include <vector>
#include <Arduino.h>
#include <FreeRTOS.h>
#include <semphr.h>
#include <FreeRtosMutex.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <ArduinoLog.h>
#include <LogService.h>
#include <TelemetryData.h>
#include <GpsData.h>
#include <LoRaTelemetryDataService.h>
#include <LoRaCommunicationService.h>
#include <UvLightSensor.h>
#include <LocationSensor.h>
#include <DustDetectionSensor.h>
#include <Si1145UvLightSensor.h>
#include <Air530GpsLocationSensor.h>
// #include <BME680EnvironmentalSensor.h>
#include <SHT40EnvironmentalSensor.h>
#include <HM3301DustDetectionSensor.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <gps_img.h>
#include <ui.h>
#include <LogService.h>

using namespace MobileEnvironmentalMonitoring::Models;
using namespace MobileEnvironmentalMonitoring::Communication;
using namespace MobileEnvironmentalMonitoring::Sensor;
using namespace MobileEnvironmentalMonitoring::Service;
using namespace MobileEnvironmentalMonitoring::Utils;

// #define LVGL_TICK_PERIOD 5

// UI
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); /* TFT instance */

// lv_indev_t *my_indev;

// Mutex
SemaphoreHandle_t _soft_serial_mutex = xSemaphoreCreateMutex();
SemaphoreHandle_t _telemetry_data_mutex = xSemaphoreCreateMutex();
std::unique_ptr<FreeRtosMutex> _communication_mutex = std::make_unique<FreeRtosMutex>();

// Interfaces
std::shared_ptr<SoftwareSerial> _uart0;
std::shared_ptr<SoftwareSerial> _uart1;
std::shared_ptr<TwoWire> _i2c0;
// std::shared_ptr<TwoWire> _i2c1;

// Models
std::shared_ptr<TelemetryData> _telemetry_data;

// Services
std::unique_ptr<CommunicationService> _communication_service;
std::unique_ptr<LoRaTelemetryDataService> _lora_telemetry_data_service;
std::shared_ptr<LogService> _log;

// Sensors
std::unique_ptr<LocationSensor> _location_sensor;
std::unique_ptr<UvLightSensor> _uv_light_sensor;
std::unique_ptr<EnvironmentalSensor> _environmental_sensor;
std::unique_ptr<DustDetectionSensor> _dust_detection_sensor;

// Variables
bool is_connected = false;
bool got_gps_fix = false;
bool got_new_data = false;
bool sending_data = false;

// Task handles
TaskHandle_t handle_sensor_update;
TaskHandle_t handle_check_connection;
TaskHandle_t handle_location_update;
TaskHandle_t handle_send_sensor_data;
TaskHandle_t handle_ui_update;
TaskHandle_t handle_ui_update_infos;
TaskHandle_t handle_monitor;

// UI variables
enum UiState
{
  Init,
  InitGpsRdy,
  InitConnectionRdy,
  Measurement
};

UiState current_ui_state = Init;

void listen_uart0()
{
  auto success_start = _uart0->listen();

  Log.verboseln("Uart0 start: %d", success_start);
  Log.verboseln("Uart0 active: %d\tUart1 active: %d", _uart0->isListening(), _uart1->isListening());
}

void listen_uart1()
{
  auto success_start = _uart1->listen();

  Log.verboseln("Uart1 start: %d", success_start);
  Log.verboseln("Uart0 active: %d\tUart1 active: %d", _uart0->isListening(), _uart1->isListening());
}

/**
 *  @brief Gets the updated values from the sensors and saves it to `_telemetry_data`
 *  @param pvParameters Unused parameter from the FreeRTOS task handle
 */
void sensor_update([[maybe_unused]] void *pvParameters)
{

  while (true)
  {
    if (xSemaphoreTake(_telemetry_data_mutex, portMAX_DELAY) == pdFALSE)
    {
      Log.errorln("Can't access telemetry data mutex!");
      return;
    }

    Log.verboseln("Sensor update...");

    // Location
    auto gps_data = _location_sensor->get_location();
    got_gps_fix = gps_data->got_fix;

    if (!got_gps_fix)
    {
      Log.infoln("GPS waiting for fix!");
    }
    else
    {
      _telemetry_data->set_lat(gps_data->lat);
      _telemetry_data->set_lng(gps_data->lng);
      _telemetry_data->set_alt(gps_data->alt);
      _telemetry_data->set_speed(gps_data->speed);
      _telemetry_data->set_satellites(gps_data->satellites);
      _telemetry_data->set_hdop(gps_data->hdop);
      Log.infoln("Got fix: %s", gps_data->got_fix ? "true" : "false");
      Log.infoln("Location: %F,%F", gps_data->lat, gps_data->lng);
      Log.infoln("Altitude: %d", gps_data->alt);
      Log.infoln("Course: %d", gps_data->course);
      Log.infoln("Speed: %F km/h", 1.852 * gps_data->speed / 100.0);
      Log.infoln("Satellites: %d", gps_data->satellites);
      Log.infoln("Hdop: %d", gps_data->hdop);
      Log.infoln("Date: %d/%d/%d", gps_data->date / 10000, (gps_data->date / 100) % 100, gps_data->date % 100 + 2000);
      Log.infoln("Time: %d:%d:%d:%d", gps_data->time / 1000000, (gps_data->time / 10000) % 100, (gps_data->time / 100) % 100, gps_data->time % 100);
    }

    // Temperature, Humidity, Pressure, Gas
    auto environmental_data = _environmental_sensor->get_data();
    _telemetry_data->set_temperature(environmental_data->temperature);
    _telemetry_data->set_humidity(environmental_data->humidity);
    _telemetry_data->set_pressure(environmental_data->pressure);
    _telemetry_data->set_gas(environmental_data->gas);

    Log.infoln("Temperature: %F °C", environmental_data->temperature);
    Log.infoln("Humidity: %F %%", environmental_data->humidity);
    Log.infoln("Pressure: %d KPa", environmental_data->pressure);
    Log.infoln("Gas: %d Kohms", environmental_data->gas);

    // Dust
    auto dust = _dust_detection_sensor->get_data();
    _telemetry_data->set_pm1_0_cf1(dust->pm1_0_cf1);
    _telemetry_data->set_pm2_5_cf1(dust->pm2_5_cf1);
    _telemetry_data->set_pm10_0_cf1(dust->pm10_0_cf1);

    Log.infoln("PM1.0 concentration(CF=1,Standard particulate matter,unit:ug/m3): %d", dust->pm1_0_cf1);
    Log.infoln("PM2.5 concentration(CF=1,Standard particulate matter,unit:ug/m3): %d", dust->pm2_5_cf1);
    Log.infoln("PM10.0 concentration(CF=1,Standard particulate matter,unit:ug/m3): %d", dust->pm10_0_cf1);

    // UV light
    auto uv_light_val = _uv_light_sensor->read_uv();
    _telemetry_data->set_uv_light(uv_light_val);

    Log.infoln("UV index: %F", uv_light_val / 100.0);

    got_new_data = true;

    // Release mutex to access telemetry data
    xSemaphoreGive(_telemetry_data_mutex);

    delay(1000);
  }
  vTaskDelete(NULL);
}

/**
 *  @brief Checks the connection of the CommunicationService and reconnects if isn't connected
 *  @param pvParameters Unused parameter from the FreeRTOS task handle
 */
void check_connection([[maybe_unused]] void *pvParameters)
{
  while (true)
  {

    // Get mutex to access SoftSerial
    if (xSemaphoreTake(_soft_serial_mutex, portMAX_DELAY) == pdFALSE)
    {
      Log.errorln("Can't access soft serial mutex!");
      return;
    }

    listen_uart0();

    is_connected = _communication_service->is_connected();
    if (!is_connected)
    {
      Log.infoln("Not connected! Reconnecting...");
      if (!_communication_service->connect())
      {
        Log.errorln("Can't connect!");
      }
    }
    else
    {
      Log.verboseln("Is connected, nothing to do!");
    }

    listen_uart1();

    // Release mutex to access SoftSerial
    xSemaphoreGive(_soft_serial_mutex);

    delay(5000);
  }
  vTaskDelete(NULL);
}

/**
 *  @brief Sends data via the communication service
 *  @param pvParameters Unused parameter from the FreeRTOS task handle
 */
void send_sensor_data([[maybe_unused]] void *pvParameters)
{
  while (true)
  {
    if (got_new_data && got_gps_fix && _communication_service->is_connected())
    // if (got_new_data && _communication_service->is_connected())
    {
      Log.infoln("Sending data...");

      // Get mutex to access SoftSerial
      if (xSemaphoreTake(_soft_serial_mutex, portMAX_DELAY) == pdFALSE)
      {
        Log.errorln("Can't access soft serial mutex!");
        return;
      }

      // Get mutex to access telemetry data
      if (xSemaphoreTake(_telemetry_data_mutex, portMAX_DELAY) == pdFALSE)
      {
        Log.errorln("Can't access telemetry data mutex!");
        return;
      }

      listen_uart0();

      _communication_service->send_data(_telemetry_data);
      got_new_data = false;

      listen_uart1();

      // Release mutex to access telemetry data
      xSemaphoreGive(_telemetry_data_mutex);

      // Release mutex to access SoftSerial
      xSemaphoreGive(_soft_serial_mutex);
    }
    else
    {
      Log.infoln("Didn't send data! No new data, gps fix or not connected!");
    }

    delay(15000);
  }
  vTaskDelete(NULL);
}

/**
 *  @brief Runs the LocationSensor's run-method to update the location
 *  @param pvParameters Unused parameter from the FreeRTOS task handle
 */
void location_update([[maybe_unused]] void *pvParameters)
{
  while (true)
  {
    Log.verboseln("Location update...");
    // Get mutex to access SoftSerial
    if (xSemaphoreTake(_soft_serial_mutex, portMAX_DELAY) == pdFALSE)
    {
      Log.errorln("Can't access soft serial mutex!");
      return;
    }

    _location_sensor->run();

    // Release mutex to access SoftSerial
    xSemaphoreGive(_soft_serial_mutex);

    delay(500);
  }
  vTaskDelete(NULL);
}

/* Display flushing */
void disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint16_t c;

  uint32_t w = (area->x2 - area->x1 + 1);
  uint32_t h = (area->y2 - area->y1 + 1);

  tft.startWrite();                            /* Start new TFT transaction */
  tft.setAddrWindow(area->x1, area->y1, w, h); /* set the working window */
  for (int y = area->y1; y <= area->y2; y++)
  {
    for (int x = area->x1; x <= area->x2; x++)
    {
      c = color_p->full;
      tft.writeColor(c, 1);
      color_p++;
    }
  }
  tft.endWrite();            /* terminate TFT transaction */
  lv_disp_flush_ready(disp); /* tell lvgl that flushing is done */
}

void ui_update([[maybe_unused]] void *pvParameters)
{
  auto last_update = xTaskGetTickCount() / portTICK_PERIOD_MS;
  auto delta_ms = 0ul;

  while (true)
  {
    auto cur_update = xTaskGetTickCount() / portTICK_PERIOD_MS;
    delta_ms = cur_update - last_update;
    last_update = cur_update;
    lv_tick_inc(delta_ms);
    lv_timer_handler();
    delay(5);
  }
  vTaskDelete(NULL);
}

void ui_update_infos([[maybe_unused]] void *pvParameters)
{
  // Load init screen
  lv_disp_load_scr(ui_InitScreen);

  // Set defaults
  lv_obj_set_style_arc_opa(ui_SpinnerInitSensor, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(ui_SpinnerInitSensor, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_opa(ui_LabelInitSensorStatus, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_arc_opa(ui_SpinnerConnection, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(ui_SpinnerConnection, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_opa(ui_LabelConnectionStatus, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_arc_opa(ui_SpinnerGpsFix, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(ui_SpinnerGpsFix, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_opa(ui_LabelGpsFixStatus, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  // Sensors loaded
  lv_label_set_text(ui_LabelInitSensorStatus, "OK!");
  lv_obj_set_style_text_color(ui_LabelInitSensorStatus, lv_color_hex(0x24C178), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(ui_SpinnerInitSensor, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_arc_opa(ui_SpinnerInitSensor, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_opa(ui_LabelInitSensorStatus, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  while (true)
  {
    if (current_ui_state == Init || current_ui_state == InitConnectionRdy || current_ui_state == InitGpsRdy)
    {
      if (is_connected && current_ui_state != InitConnectionRdy)
      {
        lv_label_set_text(ui_LabelConnectionStatus, "OK!");
        lv_obj_set_style_text_color(ui_LabelConnectionStatus, lv_color_hex(0x24C178), LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_arc_opa(ui_SpinnerConnection, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_arc_opa(ui_SpinnerConnection, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_opa(ui_LabelConnectionStatus, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        current_ui_state = InitConnectionRdy;
      }

      if (got_gps_fix && current_ui_state != InitGpsRdy)
      {
        lv_label_set_text(ui_LabelGpsFixStatus, "OK!");
        lv_obj_set_style_text_color(ui_LabelGpsFixStatus, lv_color_hex(0x24C178), LV_PART_MAIN | LV_STATE_DEFAULT);

        lv_obj_set_style_arc_opa(ui_SpinnerGpsFix, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
        lv_obj_set_style_arc_opa(ui_SpinnerGpsFix, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_opa(ui_LabelGpsFixStatus, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

        current_ui_state = InitGpsRdy;
      }

      if (is_connected && got_gps_fix)
      {
        current_ui_state = Measurement;
        lv_disp_load_scr(ui_Measurement);
      }
    }

    if (current_ui_state == Measurement && got_new_data)
    {
      char ui_buffer[50];
      sprintf(ui_buffer, "%.2f °C", _telemetry_data->get_temperature());
      lv_label_set_text(ui_LabelTemp, ui_buffer);

      sprintf(ui_buffer, "%.2f %%", _telemetry_data->get_humidity());
      lv_label_set_text(ui_LabelHum, ui_buffer);

      sprintf(ui_buffer, "%.2f UV", _telemetry_data->get_uv_light() / 100.0);
      lv_label_set_text(ui_LabelUv, ui_buffer);

      sprintf(ui_buffer, "PM1.0: %d\nPM2.5: %d\nPM10.0: %d", _telemetry_data->get_pm1_0_cf1(), _telemetry_data->get_pm2_5_cf1(), _telemetry_data->get_pm10_0_cf1());
      lv_label_set_text(ui_LabelDust, ui_buffer);

      sprintf(ui_buffer, "Lat: %.6f\nLng: %.6f\nAlt: %.2f m", _telemetry_data->get_lat(), _telemetry_data->get_lng(), _telemetry_data->get_alt() / 100.0);
      lv_label_set_text(ui_LabelLocation, ui_buffer);

      sprintf(ui_buffer, "%d Sats\n%.2f km/h\n%d Hdop", _telemetry_data->get_satellites(), 1.852 * _telemetry_data->get_speed() / 100.0, _telemetry_data->get_hdop());
      lv_label_set_text(ui_LabelSat, ui_buffer);
    }

    delay(100);
  }
  vTaskDelete(NULL);
}

void taskMonitor([[maybe_unused]] void *pvParameters)
{
  int measurement;

  Log.verboseln("Task Monitor: Started");

  while (true)
  {
    Log.verboseln("");
    Log.verboseln("******************************");
    Log.verboseln("[Stacks Free Bytes Remaining] ");

    measurement = uxTaskGetStackHighWaterMark(handle_sensor_update);
    Log.verboseln("Sensor update task: %d", measurement);

    measurement = uxTaskGetStackHighWaterMark(handle_check_connection);
    Log.verboseln("Check connection task: %d", measurement);

    measurement = uxTaskGetStackHighWaterMark(handle_send_sensor_data);
    Log.verboseln("Send sensor data: %d", measurement);

    measurement = uxTaskGetStackHighWaterMark(handle_location_update);
    Log.verboseln("Location update task: %d", measurement);

    measurement = uxTaskGetStackHighWaterMark(handle_ui_update);
    Log.verboseln("UI update task: %d", measurement);

    measurement = uxTaskGetStackHighWaterMark(handle_ui_update_infos);
    Log.verboseln("UI update infos task: %d", measurement);

    measurement = uxTaskGetStackHighWaterMark(handle_monitor);
    Log.verboseln("Monitor task: %d", measurement);

    Log.verboseln("******************************");
    Log.verboseln("");

    delay(10000); // print every 10 seconds
  }

  Log.verboseln("Task Monitor: Deleting");
  vTaskDelete(NULL);
}

void init_display()
{
  lv_init();

#if LV_USE_LOG != 0
  /* register print function for debugging */
  lv_log_register_print_cb([](const char *buf) -> void
                           { Log.info(buf); });
#endif

  tft.begin();        /* TFT init */
  tft.setRotation(3); /* Landscape orientation */

  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

  /*Initialize the display*/
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = disp_flush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);
}

void setup()
{
  SerialUSB.begin(115200);

  // Set the led the rtos will blink when we have a fatal rtos error
  // RTOS also Needs to know if high/low is the state that turns on the led.
  // Error Blink Codes:
  //    3 blinks - Fatal Rtos Error, something bad happened. Think really hard about what you just changed.
  //    2 blinks - Malloc Failed, Happens when you couldn't create a rtos object.
  //               Probably ran out of heap.
  //    1 blink  - Stack overflow, Task needs more bytes defined for its stack!
  //               Use the taskMonitor thread to help gauge how much more you need
  vSetErrorLed(LED_BUILTIN, LOW);

  Log.begin(LOG_LEVEL_VERBOSE, &SerialUSB);

  std::vector<std::shared_ptr<Print>> log_outputs{{std::shared_ptr<Print>(&SerialUSB)}};
  _log = std::make_shared<LogService>(LogLevel::DEBUG, log_outputs);

  init_display();
  // ui_draw(NULL);
  ui_init();

  _uart0 = std::make_shared<SoftwareSerial>(D0, D1);
  _uart1 = std::make_shared<SoftwareSerial>(D2, D3);
  _i2c0 = std::make_shared<TwoWire>(Wire);
  // _i2c1 = std::shared_ptr<TwoWire>(Wire1);

  _telemetry_data = std::make_shared<TelemetryData>();

  Log.infoln("Starting...");

  pinMode(LED_BUILTIN, OUTPUT);

  Log.infoln("Setup communication service...");
  _uart0->begin(9600);
  _lora_telemetry_data_service = std::make_unique<LoRaTelemetryDataService>();
  _communication_service = std::make_unique<LoRaCommunicationService>(std::move(_communication_mutex), _uart0, std::move(_lora_telemetry_data_service), LORA_DEV_EUI, LORA_APP_EUI, LORA_APP_KEY);

  Log.infoln("Setup location sensor...");
  _uart1->begin(9600);
  _location_sensor = std::make_unique<Air530GpsLocationSensor>(_uart1);

  Log.infoln("Setup uv light sensor...");
  _uv_light_sensor = std::make_unique<Si1145UvLightSensor>(_i2c0);

  Log.infoln("Setup environmental sensor...");
  _environmental_sensor = std::make_unique<SHT40EnvironmentalSensor>(_i2c0);

  Log.infoln("Setup dust detection sensor...");
  _dust_detection_sensor = std::make_unique<HM3301DustDetectionSensor>(_i2c0);

  // Create all tasks
  xTaskCreate(sensor_update, "Sensor update task", 512, NULL, tskIDLE_PRIORITY + 2, &handle_sensor_update);
  xTaskCreate(check_connection, "Check connection task", 256, NULL, tskIDLE_PRIORITY + 5, &handle_check_connection);
  xTaskCreate(location_update, "Location update task", 256, NULL, tskIDLE_PRIORITY + 3, &handle_location_update);
  xTaskCreate(send_sensor_data, "Send sensor data", 256, NULL, tskIDLE_PRIORITY + 6, &handle_send_sensor_data);
  xTaskCreate(ui_update, "UI update task", 512, NULL, tskIDLE_PRIORITY + 3, &handle_ui_update);
  xTaskCreate(ui_update_infos, "UI update infos task", 512, NULL, tskIDLE_PRIORITY + 3, &handle_ui_update_infos);
  xTaskCreate(taskMonitor, "Monitor task", 128, NULL, tskIDLE_PRIORITY + 1, &handle_monitor);

  // Start the RTOS, this function will never return and will schedule the tasks.
  vTaskStartScheduler();
}

void loop()
{
  // Don't write any code here. It will not execute, because of the vTaskStartScheduler() in the setup()!
}