#include "TelemetryData.h"

uint16_t MobileEnvironmentalMonitoring::Models::TelemetryData::get_uv_light()
{
    return this->_uv_light;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_uv_light(uint16_t uv_light)
{
    this->_uv_light = uv_light;
}

float MobileEnvironmentalMonitoring::Models::TelemetryData::get_temperature()
{
    return this->_temperature;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_temperature(float temperature)
{
    this->_temperature = temperature;
}

uint16_t MobileEnvironmentalMonitoring::Models::TelemetryData::get_pressure()
{
    return this->_pressure;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_pressure(uint16_t pressure)
{
    this->_pressure = pressure;
}

float MobileEnvironmentalMonitoring::Models::TelemetryData::get_humidity()
{
    return this->_humidity;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_humidity(float humidity)
{
    this->_humidity = humidity;
}

uint32_t MobileEnvironmentalMonitoring::Models::TelemetryData::get_gas()
{
    return this->_gas;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_gas(uint32_t gas)
{
    this->_gas = gas;
}

double MobileEnvironmentalMonitoring::Models::TelemetryData::get_lat()
{
    return this->_lat;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_lat(double lat)
{
    this->_lat = lat;
}

double MobileEnvironmentalMonitoring::Models::TelemetryData::get_lng()
{
    return this->_lng;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_lng(double lng)
{
    this->_lng = lng;
}

int32_t MobileEnvironmentalMonitoring::Models::TelemetryData::get_alt()
{
    return this->_alt;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_alt(int32_t alt)
{
    this->_alt = alt;
}

int32_t MobileEnvironmentalMonitoring::Models::TelemetryData::get_speed()
{
    return this->_speed;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_speed(int32_t speed)
{
    this->_speed = speed;
}

uint32_t MobileEnvironmentalMonitoring::Models::TelemetryData::get_satellites()
{
    return this->_satellites;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_satellites(uint32_t satellites)
{
    this->_satellites = satellites;
}

int32_t MobileEnvironmentalMonitoring::Models::TelemetryData::get_hdop()
{
    return this->_hdop;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_hdop(int32_t hdop)
{
    this->_hdop = hdop;
}

uint16_t MobileEnvironmentalMonitoring::Models::TelemetryData::get_pm1_0_cf1()
{
    return this->_pm1_0_cf1;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_pm1_0_cf1(uint16_t pm1_0_cf1)
{
    this->_pm1_0_cf1 = pm1_0_cf1;
}

uint16_t MobileEnvironmentalMonitoring::Models::TelemetryData::get_pm2_5_cf1()
{
    return this->_pm2_5_cf1;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_pm2_5_cf1(uint16_t pm2_5_cf1)
{
    this->_pm2_5_cf1 = pm2_5_cf1;
}

uint16_t MobileEnvironmentalMonitoring::Models::TelemetryData::get_pm10_0_cf1()
{
    return this->_pm10_0_cf1;
}

void MobileEnvironmentalMonitoring::Models::TelemetryData::set_pm10_0_cf1(uint16_t pm10_0_cf1)
{
    this->_pm10_0_cf1 = pm10_0_cf1;
}