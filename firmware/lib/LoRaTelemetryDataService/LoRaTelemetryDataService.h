#pragma once

#include <memory>
#include <vector>
#include <Arduino.h>
#include <TelemetryData.h>

namespace MobileEnvironmentalMonitoring::Service
{
    class LoRaTelemetryDataService
    {
    public:
        virtual void transform(std::shared_ptr<MobileEnvironmentalMonitoring::Models::TelemetryData> telemetry_data, std::vector<uint8_t> &data);
    };
}