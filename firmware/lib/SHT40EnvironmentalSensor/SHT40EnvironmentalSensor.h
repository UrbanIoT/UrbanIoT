#pragma once

#include <memory>
#include <Arduino.h>
#include <EnvironmentalData.h>
#include <EnvironmentalSensor.h>
#include <Wire.h>
#include <SensirionI2CSht4x.h>

namespace MobileEnvironmentalMonitoring::Sensor
{
    class SHT40EnvironmentalSensor : public EnvironmentalSensor
    {
    public:
        SHT40EnvironmentalSensor(const std::shared_ptr<TwoWire> i2c);

        std::shared_ptr<MobileEnvironmentalMonitoring::Models::EnvironmentalData> get_data(void) override;

    private:
        const std::shared_ptr<TwoWire> _i2c;
        std::shared_ptr<MobileEnvironmentalMonitoring::Models::EnvironmentalData> _environmental_data;
        SensirionI2CSht4x sht4x;

        uint32_t serialNumber;
        uint16_t error;
        char errorMessage[256];
        float temp, hum;
    };
}