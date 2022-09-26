#include "SHT40EnvironmentalSensor.h"
#include <ArduinoLog.h>

MobileEnvironmentalMonitoring::Sensor::SHT40EnvironmentalSensor::SHT40EnvironmentalSensor(const std::shared_ptr<TwoWire> i2c)
    : _i2c(i2c)
{
    _environmental_data = std::make_shared<MobileEnvironmentalMonitoring::Models::EnvironmentalData>();
    sht4x.begin(*i2c.get());

    error = sht4x.serialNumber(serialNumber);
    if (error)
    {
        errorToString(error, errorMessage, 256);
        Log.errorln("Error trying to execute serialNumber(): %s", errorMessage);
    }
    else
    {
        Log.verboseln("Serial Number: %d", serialNumber);
    }
}

std::shared_ptr<MobileEnvironmentalMonitoring::Models::EnvironmentalData> MobileEnvironmentalMonitoring::Sensor::SHT40EnvironmentalSensor::get_data(void)
{
    error = sht4x.measureHighPrecision(temp, hum);
    _environmental_data->temperature = temp;
    _environmental_data->humidity = hum;

    if (error)
    {
        errorToString(error, errorMessage, 256);
        Log.errorln("Error trying to execute measureHighPrecision(): %s", errorMessage);
    }

    return _environmental_data;
}