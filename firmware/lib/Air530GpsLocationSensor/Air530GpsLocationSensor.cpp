#include "Air530GpsLocationSensor.h"
#include <ArduinoLog.h>

MobileEnvironmentalMonitoring::Sensor::Air530GpsLocationSensor::Air530GpsLocationSensor(std::shared_ptr<Stream> uart)
{
    this->_stream = uart;
    this->_gps_data = std::make_shared<MobileEnvironmentalMonitoring::Models::GpsData>();
}

MobileEnvironmentalMonitoring::Sensor::Air530GpsLocationSensor::~Air530GpsLocationSensor()
{
}

void MobileEnvironmentalMonitoring::Sensor::Air530GpsLocationSensor::run()
{
    while (this->_stream->available() > 0)
    {
        if (this->_gps.encode(this->_stream->read()))
        {
            if (this->_gps.location.isValid() &&
                this->_gps.location.age() < 2000 &&
                this->_gps.hdop.isValid() &&
                this->_gps.hdop.value() <= 300 &&
                this->_gps.hdop.age() < 2000 &&
                this->_gps.altitude.isValid() &&
                this->_gps.altitude.age() < 2000)
            {
                this->_gps_data->got_fix = true;
            }
            else
            {
                this->_gps_data->got_fix = false;
            }

            if (this->_gps_data->got_fix && this->_gps.location.isUpdated())
            {
                this->_gps_data->lat = this->_gps.location.lat();
                this->_gps_data->lng = this->_gps.location.lng();
            }

            if (this->_gps.altitude.isValid() && this->_gps.altitude.isUpdated())
            {
                this->_gps_data->alt = this->_gps.altitude.value();
            }

            if (this->_gps.course.isValid() && this->_gps.course.isUpdated())
            {
                this->_gps_data->course = this->_gps.course.value();
            }

            if (this->_gps.hdop.isValid() && this->_gps.hdop.isUpdated())
            {
                this->_gps_data->hdop = this->_gps.hdop.value();
            }

            if (this->_gps.satellites.isValid() && this->_gps.satellites.isUpdated())
            {
                this->_gps_data->satellites = this->_gps.satellites.value();
            }

            if (this->_gps.speed.isValid() && this->_gps.speed.isUpdated())
            {
                this->_gps_data->speed = this->_gps.speed.value();
            }

            if (this->_gps.date.isValid() && this->_gps.date.isUpdated())
            {
                this->_gps_data->date = this->_gps.date.value();
            }

            if (this->_gps.time.isValid() && this->_gps.time.isUpdated())
            {
                this->_gps_data->time = this->_gps.time.value();
            }
        }
    }
}

std::shared_ptr<MobileEnvironmentalMonitoring::Models::GpsData> MobileEnvironmentalMonitoring::Sensor::Air530GpsLocationSensor::get_location()
{
    return this->_gps_data;
}