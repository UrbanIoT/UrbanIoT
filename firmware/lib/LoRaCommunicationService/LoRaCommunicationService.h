#pragma once

#include "CommunicationService.h"
#include <stdio.h>
#include <stdarg.h>
#include <functional>
#include <Arduino.h>
#include <string>
#include <tuple>
#include <Mutex.h>
#include <LoRaTelemetryDataService.h>

namespace MobileEnvironmentalMonitoring::Communication
{
    class LoRaCommunicationService : public CommunicationService
    {
    public:
        LoRaCommunicationService(const std::shared_ptr<MobileEnvironmentalMonitoring::Utils::Mutex> mutex, const std::shared_ptr<Stream> stream, const std::shared_ptr<MobileEnvironmentalMonitoring::Service::LoRaTelemetryDataService> lora_telemetry_data_service, const char *dev_eui, const char *app_eui, const char *app_key);
        ~LoRaCommunicationService();
        bool connect() override;
        bool disconnect() override;
        bool is_connected() override;
        bool send_data(std::shared_ptr<MobileEnvironmentalMonitoring::Models::TelemetryData> telemetry_data) override;
        bool test();

    private:
        const std::shared_ptr<MobileEnvironmentalMonitoring::Utils::Mutex> _mutex;
        const std::shared_ptr<Stream> _stream;
        const std::shared_ptr<MobileEnvironmentalMonitoring::Service::LoRaTelemetryDataService> _lora_telemetry_data_service;

        const char *_dev_eui, *_app_eui, *_app_key;
        bool _is_connected;

        const std::string terminator = "\r\n";
        std::string bufferString = "";
        const uint16_t bufferSize = 255;
        uint16_t bufferPos = 0;
        uint16_t termPos = 0;
        uint16_t tokenPos = 0;

        template <typename... Args>
        bool at_send_check_response(const std::function<std::tuple<bool, bool>(const std::string &)> &parseFunc, const unsigned long timeout_ms, const std::string &p_cmd, Args... args);

        template <typename... Args>
        std::string string_format(const std::string &format, Args... args);

        void writeToBuffer(int data);
        void clearBuffer();

        std::function<std::tuple<bool, bool>(std::string)> create_simple_parse_function(const std::string &param);
        std::tuple<bool, bool> check_join(const std::string &command);
        std::tuple<bool, bool> check_msghex(const std::string &command);
    };
}