#pragma once

#include <memory>
#include <vector>
#include "TelemetryData.h"

namespace MobileEnvironmentalMonitoring::Communication
{
    class CommunicationService
    {
    public:
        virtual bool connect() = 0;
        virtual bool disconnect() = 0;
        virtual bool send_data(std::shared_ptr<MobileEnvironmentalMonitoring::Models::TelemetryData> telemetry_data) = 0;
        virtual bool is_connected() = 0;
    };
}