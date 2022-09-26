#pragma once

#include <Arduino.h>
#include <EnvironmentalData.h>
#include <EnvironmentalSensor.h>
#include <Wire.h>
#include "bme680.h"

// See https://github.com/Seeed-Studio/Seeed_BME680

namespace MobileEnvironmentalMonitoring::Sensor
{
    class BME680EnvironmentalSensor : public EnvironmentalSensor
    {
    public:
#ifdef SEEED_WIO_TERMINAL
        BME680EnvironmentalSensor(std::shared_ptr<TwoWire> i2c);
#elif PICO
        BME680EnvironmentalSensor(std::shared_ptr<HardwareI2C> i2c);
#endif
        std::shared_ptr<MobileEnvironmentalMonitoring::Models::EnvironmentalData> get_data(void) override;

    private:
        struct bme680_dev sensor_param; /**< Official LIB structure.*/

        std::shared_ptr<MobileEnvironmentalMonitoring::Models::EnvironmentalData> _environmental_data;
#ifdef SEEED_WIO_TERMINAL
        std::shared_ptr<TwoWire> _i2c;
#elif PICO
        std::shared_ptr<HardwareI2C> _i2c;
#endif

        int8_t _read_sensor_data(void);
        int8_t _iic_read(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
        int8_t _iic_write(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
    };
}