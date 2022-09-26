#pragma once

#include <memory>
#include <Arduino.h>
#include <GpsData.h>

namespace MobileEnvironmentalMonitoring::Sensor
{
    class LocationSensor
    {
    public:
         virtual void run() = 0;
         virtual std::shared_ptr<MobileEnvironmentalMonitoring::Models::GpsData>  get_location() = 0;
    };
}