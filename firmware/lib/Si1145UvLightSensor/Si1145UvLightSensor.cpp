#include "Si1145UvLightSensor.h"
#include <ArduinoLog.h>

#ifdef SEEED_WIO_TERMINAL
MobileEnvironmentalMonitoring::Sensor::Si1145UvLightSensor::Si1145UvLightSensor(std::shared_ptr<TwoWire> i2c)
#elif PICO
MobileEnvironmentalMonitoring::Sensor::Si1145UvLightSensor::Si1145UvLightSensor(std::shared_ptr<HardwareI2C> i2c)
#endif
{
    this->_i2c = i2c;
    this->_i2c->begin();

    if (_read_byte(SI114X_PART_ID) != 0X45)
    {
        Log.errorln("No Si1145 sensor detected!");
        return;
    }

    // Reset
    this->write_byte(SI114X_MEAS_RATE0, 0);
    this->write_byte(SI114X_MEAS_RATE1, 0);
    this->write_byte(SI114X_IRQ_ENABLE, 0);
    this->write_byte(SI114X_IRQ_MODE1, 0);
    this->write_byte(SI114X_IRQ_MODE2, 0);
    this->write_byte(SI114X_INT_CFG, 0);
    this->write_byte(SI114X_IRQ_STATUS, 0xFF);

    this->write_byte(SI114X_COMMAND, SI114X_RESET);
    delay(10);
    this->write_byte(SI114X_HW_KEY, 0x17);
    delay(10);

    // Deinit
    // ENABLE UV reading
    // these reg must be set to the fixed value
    this->write_byte(SI114X_UCOEFF0, 0x29);
    this->write_byte(SI114X_UCOEFF1, 0x89);
    this->write_byte(SI114X_UCOEFF2, 0x02);
    this->write_byte(SI114X_UCOEFF3, 0x00);
    this->_write_param_data(SI114X_CHLIST, SI114X_CHLIST_ENUV | SI114X_CHLIST_ENALSIR | SI114X_CHLIST_ENALSVIS |
                                               SI114X_CHLIST_ENPS1);
    //
    // set LED1 CURRENT(22.4mA)(It is a normal value for many LED)
    //
    this->_write_param_data(SI114X_PS1_ADCMUX, SI114X_ADCMUX_LARGE_IR);
    this->write_byte(SI114X_PS_LED21, SI114X_LED_CURRENT_22MA);
    this->_write_param_data(SI114X_PSLED12_SELECT, SI114X_PSLED12_SELECT_PS1_LED1); //
    //
    // PS ADC SETTING
    //
    this->_write_param_data(SI114X_PS_ADC_GAIN, SI114X_ADC_GAIN_DIV1);
    this->_write_param_data(SI114X_PS_ADC_COUNTER, SI114X_ADC_COUNTER_511ADCCLK);
    this->_write_param_data(SI114X_PS_ADC_MISC, SI114X_ADC_MISC_HIGHRANGE | SI114X_ADC_MISC_ADC_RAWADC);
    //
    // VIS ADC SETTING
    //
    this->_write_param_data(SI114X_ALS_VIS_ADC_GAIN, SI114X_ADC_GAIN_DIV1);
    this->_write_param_data(SI114X_ALS_VIS_ADC_COUNTER, SI114X_ADC_COUNTER_511ADCCLK);
    this->_write_param_data(SI114X_ALS_VIS_ADC_MISC, SI114X_ADC_MISC_HIGHRANGE);
    //
    // IR ADC SETTING
    //
    this->_write_param_data(SI114X_ALS_IR_ADC_GAIN, SI114X_ADC_GAIN_DIV1);
    this->_write_param_data(SI114X_ALS_IR_ADC_COUNTER, SI114X_ADC_COUNTER_511ADCCLK);
    this->_write_param_data(SI114X_ALS_IR_ADC_MISC, SI114X_ADC_MISC_HIGHRANGE);
    //
    // interrupt enable
    //
    this->write_byte(SI114X_INT_CFG, SI114X_INT_CFG_INTOE);
    this->write_byte(SI114X_IRQ_ENABLE, SI114X_IRQEN_ALS);
    //
    // AUTO RUN
    //
    this->write_byte(SI114X_MEAS_RATE0, 0xFF);
    this->write_byte(SI114X_COMMAND, SI114X_PSALS_AUTO);
}

uint16_t MobileEnvironmentalMonitoring::Sensor::Si1145UvLightSensor::read_uv()
{
    return (this->_read_half_word(SI114X_AUX_DATA0_UVINDEX0));
}

void MobileEnvironmentalMonitoring::Sensor::Si1145UvLightSensor::write_byte(uint8_t reg, uint8_t value)
{
    this->_i2c->beginTransmission(SI114X_ADDR);
    this->_i2c->write(reg);
    this->_i2c->write(value);
    this->_i2c->endTransmission();
}

uint8_t MobileEnvironmentalMonitoring::Sensor::Si1145UvLightSensor::_read_byte(uint8_t reg)
{
    this->_i2c->beginTransmission(SI114X_ADDR);
    this->_i2c->write(reg);
    this->_i2c->endTransmission();
    this->_i2c->requestFrom(SI114X_ADDR, 1);
    return this->_i2c->read();
}

uint16_t MobileEnvironmentalMonitoring::Sensor::Si1145UvLightSensor::_read_half_word(uint8_t reg)
{
    uint16_t value;
    this->_i2c->beginTransmission(SI114X_ADDR);
    this->_i2c->write(reg);
    this->_i2c->endTransmission();
    this->_i2c->requestFrom(SI114X_ADDR, 2);
    value = this->_i2c->read();
    value |= (uint16_t)this->_i2c->read() << 8;
    return value;
}

uint8_t MobileEnvironmentalMonitoring::Sensor::Si1145UvLightSensor::_read_param_data(uint8_t reg)
{
    this->write_byte(SI114X_COMMAND, reg | SI114X_QUERY);
    return this->_read_byte(SI114X_RD);
}

uint8_t MobileEnvironmentalMonitoring::Sensor::Si1145UvLightSensor::_write_param_data(uint8_t reg, uint8_t value)
{
    // write Value into PARAMWR reg first
    this->write_byte(SI114X_WR, value);
    this->write_byte(SI114X_COMMAND, reg | SI114X_SET);
    // SI114X writes value out to PARAM_RD,read and confirm its right
    return this->_read_byte(SI114X_RD);
}