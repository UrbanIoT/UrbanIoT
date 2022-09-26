#include "LoRaCommunicationService.h"
#include <ArduinoLog.h>
#include <vector>

MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::LoRaCommunicationService(const std::shared_ptr<MobileEnvironmentalMonitoring::Utils::Mutex> mutex, const std::shared_ptr<Stream> stream, const std::shared_ptr<MobileEnvironmentalMonitoring::Service::LoRaTelemetryDataService> lora_telemetry_data_service, const char *dev_eui, const char *app_eui, const char *app_key)
    : _mutex(mutex),
      _stream(stream),
      _lora_telemetry_data_service(lora_telemetry_data_service),
      _dev_eui(dev_eui),
      _app_eui(app_eui),
      _app_key(app_key)
{
    this->_is_connected = false;

    bufferString.reserve(bufferSize);

    if (!this->test())
    {
        Log.errorln("Can't check LoRa module!");
        return;
    }
}

MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::~LoRaCommunicationService()
{
}

bool MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::connect()
{
    if (!this->test())
    {
        Log.errorln("Can't check LoRa module!");
        return false;
    }

    this->at_send_check_response(this->create_simple_parse_function("+MODE: LWOTAA"), 1000, "AT+MODE=%s", "LWOTAA");
    this->at_send_check_response(this->create_simple_parse_function("+DR: EU868"), 1000, "AT+DR=%s", "EU868");
    this->at_send_check_response(this->create_simple_parse_function("+CH: NUM"), 1000, "AT+CH=%s", "NUM,0-2");
    this->at_send_check_response(this->create_simple_parse_function("+ID: DevEui"), 1000, "AT+ID=DevEui,\"%s\"", this->_dev_eui);
    this->at_send_check_response(this->create_simple_parse_function("+ID: AppEui"), 1000, "AT+ID=AppEui,\"%s\"", this->_app_eui);
    this->at_send_check_response(this->create_simple_parse_function("+CLASS: A"), 1000, "AT+CLASS=%s", "A");
    this->at_send_check_response(this->create_simple_parse_function("+PORT: 8"), 1000, "AT+PORT=%s", "8");

    if (!this->at_send_check_response(std::bind(&MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::check_join, this, std::placeholders::_1), 15000, "AT+JOIN"))
    {
        Log.errorln("Can't join LoRa network!");
        this->_is_connected = false;
        return false;
    }

    Log.infoln("Joined LoRa network!");
    this->_is_connected = true;
    return true;
}

bool MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::disconnect()
{
    this->_is_connected = false;
    return true;
}

bool MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::is_connected()
{
    return this->_is_connected;
}

bool MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::send_data(std::shared_ptr<MobileEnvironmentalMonitoring::Models::TelemetryData> telemetry_data)
{
    if (!this->_is_connected)
        return false;

    std::vector<uint8_t> data(30);
    _lora_telemetry_data_service->transform(telemetry_data, data);

    auto data_string = this->string_format("AT+MSGHEX=\"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x\"",
                                          data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7], data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15], data[16], data[17], data[18], data[19], data[20], data[21], data[22], data[23], data[24], data[25], data[26], data[27], data[28], data[29]);
    auto result = this->at_send_check_response(std::bind(&MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::check_msghex, this, std::placeholders::_1), 15000, data_string);
    return true;
}

bool MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::test()
{
    return this->at_send_check_response(this->create_simple_parse_function("+AT: OK"), 100, "AT");
}

void MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::writeToBuffer(int data)
{
    // we don't write EOL to the buffer
    if ((char)data != 13 && (char)data != 10)
    {
        this->bufferString += (char)data;
        this->bufferPos++;
    }
}

void MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::clearBuffer()
{
    this->bufferString = "";
    this->termPos = 0;
    this->bufferPos = 0;
    this->tokenPos = 0;
}

template <typename... Args>
bool MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::at_send_check_response(const std::function<std::tuple<bool, bool>(const std::string &)> &parseFunc, const unsigned long timeout_ms, const std::string &p_cmd, Args... args)
{
    this->_mutex->take();

    char ch;
    std::string cmd = this->string_format(p_cmd, args...);
    cmd.append(this->terminator);
    this->_stream->print(cmd.c_str());
    Log.verboseln("Send command: %s", cmd.c_str());

    auto startMillis = millis();

    do
    {
        while (this->_stream->available() > 0)
        {
            ch = this->_stream->read();

            if (ch <= 0)
            {
                continue;
            }

            if (this->bufferPos < this->bufferSize)
            {
                this->writeToBuffer(ch);
            }
            else
            {
                this->clearBuffer();
                Log.errorln("Error! Buffer overflow!");
                this->_mutex->give();
                return false;
            }

            if (this->terminator[this->termPos] != ch)
            {
                this->termPos = 0;
                continue;
            }

            if (this->terminator[++this->termPos] == 0)
            {
                Log.verboseln(this->bufferString.c_str());

                bool end_parsing, success;
                Log.verboseln("Answer: %s", bufferString.c_str());
                std::tie(end_parsing, success) = parseFunc(bufferString);

                if (end_parsing)
                {
                    this->clearBuffer();
                    this->_mutex->give();
                    return success;
                }
            }
        }
    } while (millis() - startMillis < timeout_ms);

    Log.errorln("Timeout!");
    this->clearBuffer();
    this->_mutex->give();

    return false;
}

template <typename... Args>
std::string MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::string_format(const std::string &format, Args... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
    // if (size_s <= 0)
    // {
    //   throw std::runtime_error("Error during formatting.");
    // }
    auto size = static_cast<size_t>(size_s);
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

std::function<std::tuple<bool, bool>(std::string)> MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::create_simple_parse_function(const std::string &param)
{
    return [&param](const std::string &command) -> std::tuple<bool, bool>
    {
        if (strstr(command.c_str(), param.c_str()) != NULL)
        {
            return std::make_tuple(true, true);
        }
        return std::make_tuple(true, false);
    };
}

std::tuple<bool, bool> MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::check_join(const std::string &command)
{
    if (strstr(command.c_str(), "+JOIN: Done") != NULL)
    {
        return std::make_tuple(true, true);
    }
    else if (strstr(command.c_str(), "+JOIN: Network joined") != NULL)
    {
        return std::make_tuple(true, true);
    }
    else if (strstr(command.c_str(), "+JOIN: Join failed") != NULL)
    {
        return std::make_tuple(true, false);
    }
    else if (strstr(command.c_str(), "+JOIN: LoRaWAN modem is busy") != NULL)
    {
        return std::make_tuple(true, false);
    }

    return std::make_tuple(false, false);
}

std::tuple<bool, bool> MobileEnvironmentalMonitoring::Communication::LoRaCommunicationService::check_msghex(const std::string &command)
{
    auto last_command = command.substr(command.rfind('+'));

    if (last_command.compare("+MSGHEX: Done") == 0)
    {
        return std::make_tuple(true, true);
    }
    else if (last_command.compare("+MSGHEX: LoRaWAN modem is busy") == 0)
    {
        return std::make_tuple(true, false);
    }
    else if (last_command.compare("+MSGHEX: Please join network first") == 0)
    {
        return std::make_tuple(true, false);
    }
    else if (last_command.compare(0, 24, "+MSGHEX: No free channel") == 0)
    {
        return std::make_tuple(true, false);
    }
    else if (last_command.compare(0, 16, "+MSG: No band in") == 0)
    {
        return std::make_tuple(true, false);
    }
    else if (last_command.compare("+MSG: DR error") == 0)
    {
        return std::make_tuple(true, false);
    }
    else if (last_command.compare(0, 18, "+MSG: Length error") == 0)
    {
        return std::make_tuple(true, false);
    }

    return std::make_tuple(false, false);
}
