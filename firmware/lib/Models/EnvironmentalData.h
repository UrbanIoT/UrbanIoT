#pragma once

namespace MobileEnvironmentalMonitoring::Models
{
    class EnvironmentalData
    {
    public:
        float temperature;
        uint32_t pressure;
        float humidity;
        uint32_t gas;
        uint16_t pm1_0_cf1;
        uint16_t pm2_5_cf1;
        uint16_t pm10_0_cf1;
        uint16_t pm1_0_ae;
        uint16_t pm2_5_ae;
        uint16_t pm10_0_ae;
    };
}