#pragma once

#include <Arduino.h>

namespace MobileEnvironmentalMonitoring::Sensor
{
    class UvLightSensor {
    public:
        virtual uint16_t read_uv() = 0;
    };
}