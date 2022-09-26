#pragma once

#include <memory>
#include <Arduino.h>
#include <EnvironmentalData.h>

namespace MobileEnvironmentalMonitoring::Sensor
{
    class EnvironmentalSensor
    {
    public:
        virtual std::shared_ptr<MobileEnvironmentalMonitoring::Models::EnvironmentalData> get_data() = 0;
    };
}