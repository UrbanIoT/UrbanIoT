#include "HM3301DustDetectionSensor.h"

#ifdef SEEED_WIO_TERMINAL
MobileEnvironmentalMonitoring::Sensor::HM3301DustDetectionSensor::HM3301DustDetectionSensor(std::shared_ptr<TwoWire> i2c)
#elif PICO
MobileEnvironmentalMonitoring::Sensor::HM3301DustDetectionSensor::HM3301DustDetectionSensor(std::shared_ptr<HardwareI2C> i2c)
#endif
{
    this->_i2c = i2c;
    _environmental_data = std::make_shared<MobileEnvironmentalMonitoring::Models::EnvironmentalData>();

    this->_i2c->begin();
    set_iic_addr(DEFAULT_IIC_ADDR);
    IIC_SEND_CMD(SELECT_COMM_CMD);
}

std::shared_ptr<MobileEnvironmentalMonitoring::Models::EnvironmentalData> MobileEnvironmentalMonitoring::Sensor::HM3301DustDetectionSensor::get_data()
{
    uint8_t data[30];
    uint8_t data_len = 29;

    uint32_t time_out_count = 0;
    HM330XErrorCode ret = NO_ERROR;
    this->_i2c->requestFrom(0x40, 29);
    while (data_len != this->_i2c->available())
    {
        // time_out_count++;
        // if (time_out_count > 10) {
        //     return ERROR_COMM;
        // }
        delay(1);
    }
    for (int i = 0; i < data_len; i++)
    {
        data[i] = this->_i2c->read();
    }

    parse_result_value(data);

    auto sensornum = (uint16_t)data[1 * 2] << 8 | data[1 * 2 + 1];
    this->_environmental_data->pm1_0_cf1 = (uint16_t)data[2 * 2] << 8 | data[2 * 2 + 1];
    this->_environmental_data->pm1_0_cf1 = (uint16_t)data[3 * 2] << 8 | data[3 * 2 + 1];
    this->_environmental_data->pm1_0_cf1 = (uint16_t)data[4 * 2] << 8 | data[4 * 2 + 1];

    this->_environmental_data->pm1_0_ae = (uint16_t)data[5 * 2] << 8 | data[5 * 2 + 1];
    this->_environmental_data->pm1_0_ae = (uint16_t)data[6 * 2] << 8 | data[6 * 2 + 1];
    this->_environmental_data->pm1_0_ae = (uint16_t)data[7 * 2] << 8 | data[7 * 2 + 1];

    return this->_environmental_data;
}

HM330XErrorCode MobileEnvironmentalMonitoring::Sensor::HM3301DustDetectionSensor::parse_result_value(uint8_t *data)
{
    if (NULL == data)
    {
        return ERROR_PARAM;
    }
    // for (int i = 0; i < 28; i++) {
    //     SERIAL_OUTPUT.print(data[i], HEX);
    //     SERIAL_OUTPUT.print("  ");
    //     if ((0 == (i) % 5) || (0 == i)) {
    //         SERIAL_OUTPUT.println("");
    //     }
    // }
    uint8_t sum = 0;
    for (int i = 0; i < 28; i++)
    {
        sum += data[i];
    }
    if (sum != data[28])
    {
        return ERROR_OTHERS;
    }
    // SERIAL_OUTPUT.println("");
    return NO_ERROR;
}

/**
    @brief I2C write byte
    @param reg :Register address of operation object
    @param byte :The byte to be wrote.
    @return result of operation,non-zero if failed.
*/
HM330XErrorCode MobileEnvironmentalMonitoring::Sensor::HM3301DustDetectionSensor::IIC_write_byte(uint8_t reg, uint8_t byte)
{
    int ret = 0;
    this->_i2c->beginTransmission(_IIC_ADDR);
    this->_i2c->write(reg);
    this->_i2c->write(byte);
    ret = this->_i2c->endTransmission();
    if (!ret)
    {
        return NO_ERROR;
    }
    else
    {
        return ERROR_COMM;
    }
}

/**
    @brief I2C read byte
    @param reg: Register address of operation object
    @param byte: The byte to be read in.
    @return result of operation,non-zero if failed.
*/
HM330XErrorCode MobileEnvironmentalMonitoring::Sensor::HM3301DustDetectionSensor::IIC_read_byte(uint8_t reg, uint8_t *byte)
{
    uint32_t time_out_count = 0;
    this->_i2c->beginTransmission(_IIC_ADDR);
    this->_i2c->write(reg);
    this->_i2c->endTransmission(false);

    this->_i2c->requestFrom(_IIC_ADDR, (uint8_t)1);
    while (1 != this->_i2c->available())
    {
        time_out_count++;
        if (time_out_count > 10)
        {
            return ERROR_COMM;
        }
        delay(1);
    }
    *byte = this->_i2c->read();
    return NO_ERROR;
}

/**
    @brief change the I2C address from default.
    @param IIC_ADDR: I2C address to be set
*/
void MobileEnvironmentalMonitoring::Sensor::HM3301DustDetectionSensor::set_iic_addr(uint8_t IIC_ADDR)
{
    _IIC_ADDR = IIC_ADDR;
}

/**
    @brief I2C read 16bit value
    @param reg: Register address of operation object
    @param byte: The 16bit value to be read in.
    @return result of operation,non-zero if failed.
*/
HM330XErrorCode MobileEnvironmentalMonitoring::Sensor::HM3301DustDetectionSensor::IIC_read_16bit(uint8_t start_reg, uint16_t *value)
{
    uint32_t time_out_count = 0;
    uint8_t val = 0;
    *value = 0;
    this->_i2c->beginTransmission(_IIC_ADDR);
    this->_i2c->write(start_reg);
    this->_i2c->endTransmission(false);

    this->_i2c->requestFrom(_IIC_ADDR, sizeof(uint16_t));
    while (sizeof(uint16_t) != this->_i2c->available())
    {
        time_out_count++;
        if (time_out_count > 10)
        {
            return ERROR_COMM;
        }
        delay(1);
    }
    val = this->_i2c->read();
    *value |= (uint16_t)val << 8;
    val = this->_i2c->read();
    *value |= val;
    return NO_ERROR;
}

/**
    @brief I2C write 16bit value
    @param reg: Register address of operation object
    @param value: The 16bit value to be wrote .
    @return result of operation,non-zero if failed.
*/
HM330XErrorCode MobileEnvironmentalMonitoring::Sensor::HM3301DustDetectionSensor::IIC_write_16bit(uint8_t reg, uint16_t value)
{
    int ret = 0;
    this->_i2c->beginTransmission(_IIC_ADDR);
    this->_i2c->write(reg);

    this->_i2c->write((uint8_t)(value >> 8));
    this->_i2c->write((uint8_t)value);
    ret = this->_i2c->endTransmission();
    if (!ret)
    {
        return NO_ERROR;
    }
    else
    {
        return ERROR_COMM;
    }
}

/**
    @brief I2C read some bytes
    @param reg: Register address of operation object
    @param data: The buf  to be read in.
    @param data_len: The length of buf need to read in.
    @return result of operation,non-zero if failed.
*/
HM330XErrorCode MobileEnvironmentalMonitoring::Sensor::HM3301DustDetectionSensor::IIC_read_bytes(uint8_t start_reg, uint8_t *data, uint32_t data_len)
{
    HM330XErrorCode ret = NO_ERROR;
    uint32_t time_out_count = 0;
    this->_i2c->beginTransmission(_IIC_ADDR);
    this->_i2c->write(start_reg);
    this->_i2c->endTransmission(false);

    this->_i2c->requestFrom(_IIC_ADDR, data_len);
    while (data_len != this->_i2c->available())
    {
        time_out_count++;
        if (time_out_count > 10)
        {
            return ERROR_COMM;
        }
        delay(1);
    }

    for (int i = 0; i < data_len; i++)
    {
        data[i] = this->_i2c->read();
    }
    return ret;
}

HM330XErrorCode MobileEnvironmentalMonitoring::Sensor::HM3301DustDetectionSensor::IIC_SEND_CMD(uint8_t CMD)
{
    this->_i2c->beginTransmission(_IIC_ADDR);
    this->_i2c->write(CMD);
    byte ret = this->_i2c->endTransmission();
    if (ret == 0)
    {
        return NO_ERROR;
    }
    else
    {
        return ERROR_COMM;
    }
}
