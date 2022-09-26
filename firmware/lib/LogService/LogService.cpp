#include "LogService.h"

MobileEnvironmentalMonitoring::Service::LogService::LogService(const MobileEnvironmentalMonitoring::Service::LogLevel log_level, std::vector<std::shared_ptr<Print>> &outputs) : _log_level(log_level), _outputs(outputs)
{
}

void MobileEnvironmentalMonitoring::Service::LogService::fatal(const std::string &msg, ...)
{
    va_list vl;
    va_start(vl, msg);
    // #ifndef DISABLE_LOGGING
    this->log(LogLevel::FATAL, msg, vl);
    // #endif
    va_end(vl);
}

void MobileEnvironmentalMonitoring::Service::LogService::error(const std::string &msg, ...)
{
    va_list vl;
    va_start(vl, msg);
    // #ifndef DISABLE_LOGGING
    this->log(LogLevel::ERROR, msg, vl);
    // #endif
    va_end(vl);
}

void MobileEnvironmentalMonitoring::Service::LogService::warn(const std::string &msg, ...)
{
    va_list vl;
    va_start(vl, msg);
    // #ifndef DISABLE_LOGGING
    this->log(LogLevel::WARN, msg, vl);
    // #endif
    va_end(vl);
}

void MobileEnvironmentalMonitoring::Service::LogService::info(const std::string &msg, ...)
{
    va_list vl;
    va_start(vl, msg);
    // #ifndef DISABLE_LOGGING
    this->log(LogLevel::INFO, msg, vl);
    // #endif
    va_end(vl);
}

void MobileEnvironmentalMonitoring::Service::LogService::debug(const std::string &msg, ...)
{
    va_list vl;
    va_start(vl, msg);
    // #ifndef DISABLE_LOGGING
    this->log(LogLevel::DEBUG, msg, vl);
    // #endif
    va_end(vl);
}

void MobileEnvironmentalMonitoring::Service::LogService::log(MobileEnvironmentalMonitoring::Service::LogLevel log_level, const std::string &msg, ...)
{
    va_list vl;
    va_start(vl, msg);

    SerialUSB.printf(msg.c_str(), vl);
    SerialUSB.print("\r\n");

    for (auto output : _outputs)
    {

        if (log_level < _log_level)
        {
            return;
        }

        // output->printf("[%c] ", this->_levels[static_cast<uint8_t>(log_level) + 1]);
        output->printf("[%c] ", this->_levels[static_cast<uint8_t>(0) + 1]);
        output->printf(msg.c_str(), vl);
        output->print("\r\n");
    }
    va_end(vl);
}