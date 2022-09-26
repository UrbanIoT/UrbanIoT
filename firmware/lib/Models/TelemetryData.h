#pragma once

#include <Arduino.h>

namespace MobileEnvironmentalMonitoring::Models
{
    class TelemetryData
    {
    public:
        uint16_t get_uv_light();
        void set_uv_light(uint16_t uv_light);

        float get_temperature();
        void set_temperature(float temperature);

        uint16_t get_pressure();
        void set_pressure(uint16_t pressure);

        float get_humidity();
        void set_humidity(float humidity);

        uint32_t get_gas();
        void set_gas(uint32_t gas);

        double get_lat();
        void set_lat(double lat);

        double get_lng();
        void set_lng(double lng);

        int32_t get_alt();
        void set_alt(int32_t alt);

        int32_t get_speed();
        void set_speed(int32_t speed);

        uint32_t get_satellites();
        void set_satellites(uint32_t satellites);

        int32_t get_hdop();
        void set_hdop(int32_t hdop);

        uint16_t get_pm1_0_cf1();
        void set_pm1_0_cf1(uint16_t pm1_0_cf1);

        uint16_t get_pm2_5_cf1();
        void set_pm2_5_cf1(uint16_t pm2_5_cf1);

        uint16_t get_pm10_0_cf1();
        void set_pm10_0_cf1(uint16_t pm10_0_cf1);

    private:
        uint16_t _uv_light;
        float _temperature;
        uint16_t _pressure;
        float _humidity;
        uint32_t _gas;
        double _lat;          // Latitude in degrees (double)
        double _lng;          // Longitude in degrees (double)
        int32_t _alt;         // Raw altitude in centimeters (i32)
        int32_t _speed;       // Raw speed in 100ths of a knot (i32)
        uint32_t _satellites; // Number of satellites in use (u32)
        int32_t _hdop;        // Horizontal Dim. of Precision (100ths-i32)
        uint16_t _pm1_0_cf1;
        uint16_t _pm2_5_cf1;
        uint16_t _pm10_0_cf1;
    };
}