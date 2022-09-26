#pragma once

#include <iostream>
#include <memory>
#include <Arduino.h>
#include <LocationSensor.h>
#include <GpsData.h>
#include "TinyGPS++.h"

namespace MobileEnvironmentalMonitoring::Sensor
{
    class Air530GpsLocationSensor : public LocationSensor
    {
    public:
        Air530GpsLocationSensor(std::shared_ptr<Stream> stream);
        ~Air530GpsLocationSensor();
        void run() override;
        std::shared_ptr<MobileEnvironmentalMonitoring::Models::GpsData> get_location() override;

    private:
        std::shared_ptr<Stream> _stream;

        TinyGPSPlus _gps;
        std::shared_ptr<MobileEnvironmentalMonitoring::Models::GpsData> _gps_data;
    };
}