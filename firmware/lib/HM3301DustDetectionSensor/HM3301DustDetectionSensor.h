#pragma once

#include <memory>
#include <Arduino.h>
#include <Wire.h>
#include <DustDetectionSensor.h>
#include "HM330XErrorCode.h"

// See https://github.com/Seeed-Studio/Seeed_PM2_5_sensor_HM3301

namespace MobileEnvironmentalMonitoring::Sensor
{
#define DEFAULT_IIC_ADDR 0x40
#define SELECT_COMM_CMD 0X88

    class HM3301DustDetectionSensor : public DustDetectionSensor
    {
    public:
#ifdef SEEED_WIO_TERMINAL
        HM3301DustDetectionSensor(std::shared_ptr<TwoWire> i2c);
#elif PICO
        HM3301DustDetectionSensor(std::shared_ptr<HardwareI2C> i2c);
#endif
        std::shared_ptr<MobileEnvironmentalMonitoring::Models::EnvironmentalData> get_data() override;

    private:
        std::shared_ptr<MobileEnvironmentalMonitoring::Models::EnvironmentalData> _environmental_data;
#ifdef SEEED_WIO_TERMINAL
        std::shared_ptr<TwoWire> _i2c;
#elif PICO
        std::shared_ptr<HardwareI2C> _i2c;
#endif

        uint8_t _IIC_ADDR;

        HM330XErrorCode parse_result_value(uint8_t *data);

        HM330XErrorCode IIC_write_byte(uint8_t reg, uint8_t byte);
        HM330XErrorCode IIC_read_byte(uint8_t reg, uint8_t *byte);
        void set_iic_addr(uint8_t IIC_ADDR);
        HM330XErrorCode IIC_read_16bit(uint8_t start_reg, uint16_t *value);
        HM330XErrorCode IIC_write_16bit(uint8_t reg, uint16_t value);
        HM330XErrorCode IIC_read_bytes(uint8_t start_reg, uint8_t *data, uint32_t data_len);
        HM330XErrorCode IIC_SEND_CMD(uint8_t CMD);
    };
}