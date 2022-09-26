#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <vector>
#include <memory>
#include <string>
#include <Arduino.h>

namespace MobileEnvironmentalMonitoring::Service
{
        enum class LogLevel : uint8_t
        {
                SILENT, // No output at all
                DEBUG,  // Everything: Fatal, Error, Warn, Infos and Debug
                INFO,   // Fatal, Error, Warn and Infos
                WARN,   // Fatal, Error and Warn
                ERROR,  // Fatal and Error
                FATAL   // Fatal
        };

        class LogService
        {
        public:
                LogService(const LogLevel log_level, std::vector<std::shared_ptr<Print>> &outputs);

                void fatal(const std::string &msg, ...);
                void error(const std::string &msg, ...);
                void warn(const std::string &msg, ...);
                void info(const std::string &msg, ...);
                void debug(const std::string &msg, ...);

                void log(LogLevel log_level, const std::string &msg, ...);

        private:
                const std::string _levels = "DIWEF";
                const LogLevel _log_level;
                std::vector<std::shared_ptr<Print>> &_outputs;
        };
}