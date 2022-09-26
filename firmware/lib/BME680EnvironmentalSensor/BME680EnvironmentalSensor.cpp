#include "BME680EnvironmentalSensor.h"
#include <ArduinoLog.h>

#ifdef SEEED_WIO_TERMINAL
MobileEnvironmentalMonitoring::Sensor::BME680EnvironmentalSensor::BME680EnvironmentalSensor(std::shared_ptr<TwoWire> i2c)
#elif PICO
MobileEnvironmentalMonitoring::Sensor::BME680EnvironmentalSensor::BME680EnvironmentalSensor(std::shared_ptr<HardwareI2C> i2c)
#endif
{
    this->_i2c = i2c;
    _environmental_data = std::make_shared<MobileEnvironmentalMonitoring::Models::EnvironmentalData>();
}

std::shared_ptr<MobileEnvironmentalMonitoring::Models::EnvironmentalData> MobileEnvironmentalMonitoring::Sensor::BME680EnvironmentalSensor::get_data(void)
{
}

/** @brief Getting four kinds of result value from the sensor.
    @param NONE.
    @return Result of function excution. The normal exit is only when it returns BME680_OK(0).
*/
int8_t MobileEnvironmentalMonitoring::Sensor::BME680EnvironmentalSensor::_read_sensor_data(void)
{

    struct bme680_field_data data;

    int8_t ret;

    sensor_param.power_mode = BME680_FORCED_MODE;

    uint16_t settings_sel;

    sensor_param.tph_sett.os_hum = BME680_OS_1X;
    sensor_param.tph_sett.os_pres = BME680_OS_16X;
    sensor_param.tph_sett.os_temp = BME680_OS_2X;

    sensor_param.gas_sett.run_gas = BME680_ENABLE_GAS_MEAS;
    // sensor_param.gas_sett.heatr_dur = 150;
    // sensor_param.gas_sett.heatr_temp = 320;
    sensor_param.gas_sett.heatr_dur = 100;
    sensor_param.gas_sett.heatr_temp = 300;

    settings_sel = BME680_OST_SEL | BME680_OSH_SEL | BME680_OSP_SEL | BME680_FILTER_SEL | BME680_GAS_SENSOR_SEL;

    /*Set sensor's registers*/
    if ((ret = bme680_set_sensor_settings(settings_sel, &sensor_param)))
    {
        Log.verboseln("bme680_set_sensor_settings() ==>ret value = %d", ret);
        return -1;
    }

    /*Set sensor's mode ,activate sensor*/
    if ((ret = bme680_set_sensor_mode(&sensor_param)))
    {
        Log.verboseln("bme680_set_sensor_mode() ==>ret value = %d", ret);
        return -2;
    }

    uint16_t meas_period;
    bme680_get_profile_dur(&meas_period, &sensor_param);

    delay(meas_period); /**<It is necessary to delay for a duration time */

    /*Get sensor's result value from registers*/
    if ((ret = bme680_get_sensor_data(&data, &sensor_param)))
    {
        Log.verboseln("bme680_get_sensor_data() ==>ret value = %d", ret);
        return -3;
    }

    this->_environmental_data->temperature = data.temperature;
    this->_environmental_data->humidity = data.humidity;
    this->_environmental_data->pressure = data.pressure;

    if (data.status & BME680_HEAT_STAB_MSK)
    {
        this->_environmental_data->gas = data.gas_resistance;
    }
    else
    {
        this->_environmental_data->gas = 0;
    }
    return BME680_OK;
}

/** @brief  IIC reading interface
    @param  dev_id   IIC device address
    @param  reg_addr The register address for operated.
    @param  reg_data Storing data read from  registers.
    @param  len      Length of data.
    @return The normal exit is only when it returns 0.
*/
int8_t MobileEnvironmentalMonitoring::Sensor::BME680EnvironmentalSensor::_iic_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int32_t i = 0;
    this->_i2c->beginTransmission((uint8_t)dev_id);
    this->_i2c->write((uint8_t)reg_addr);
    this->_i2c->endTransmission();
    if (len != this->_i2c->requestFrom((uint8_t)dev_id, (byte)len))
    {
        return 1;
    }
    for (i = 0; i < len; i++)
    {
        reg_data[i] = (uint8_t)this->_i2c->read();
    }
    return 0;
}

/** @brief  IIC wrting interface
    @param  dev_id   IIC device address
    @param  reg_addr The register address for operated.
    @param  reg_data The data need to be transmitted.
    @param  len      Length of data.
    @return The normal exit is only when it returns 0.
*/
int8_t MobileEnvironmentalMonitoring::Sensor::BME680EnvironmentalSensor::_iic_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    int32_t i = 0;
    this->_i2c->beginTransmission((uint8_t)dev_id);
    this->_i2c->write((uint8_t)reg_addr);

    for (i = 0; i < len; i++)
    {
        this->_i2c->write(reg_data[i]);
    }
    this->_i2c->endTransmission();
    return 0;
}