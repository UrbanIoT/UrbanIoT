#include "LoRaTelemetryDataService.h"

void MobileEnvironmentalMonitoring::Service::LoRaTelemetryDataService::transform(std::shared_ptr<MobileEnvironmentalMonitoring::Models::TelemetryData> telemetry_data, std::vector<uint8_t> &data)
{
    /**
    | Bytes | Type        | Description                                        | Required |
    |-------|-------------|----------------------------------------------------|----------|
    | 1-3   | Sensors     | Define which sensors are in use and deliver values | Yes      |
    | 4     | Battery     | Battery level in %                                 | Yes      |
    | 5-7   | Latitude    | Latitude                                           | Yes      |
    | 8-10  | Longitude   | Longitude                                          | Yes      |
    | 11-12 | Altitude    | Altitude in meter                                  | Yes      |
    | 13    | HDOP        | Horizontal Dilution of Precision                   | Yes      |
    | 14    | Sats        | Number of satellites                               | Yes      |
    | 15-16 | Speed       | Speed in km/h                                      | Yes      |
    | 17-18 | UV index    | UV index                                           | No       |
    | 19    | Temperature | Temperature in °C                                  | No       |
    | 20-21 | Pressure    | Pressure in KPa                                    | No       |
    | 22    | Humidity    | Humidity in %                                      | No       |
    | 23-24 | IAQ         | Indoor air quality                                 | No       |
    | 25-26 | PM1.0 CF1   | Particle count of size of PM1.0                    | No       |
    | 27-28 | PM2.5 CF1   | Particle count of size of PM2.5                    | No       |
    | 29-30 | PM10.0 CF1  | Particle count of size of PM10.0                   | No       |
    */

    // Sensors
    /**
    | Byte | Bit | Sensor        |
    |------|-----|---------------|
    | 1    | 1   | UV index      |
    |      | 2   | Temperature   |
    |      | 3   | Pressure      |
    |      | 4   | Humidity      |
    |      | 5   | IAQ           |
    |      | 6   | Dust/particle |
    |      | 7   | not defined   |
    |      | 8   | not defined   |
    | 2    | 1-8 | not defined   |
    | 3    | 1-8 | not defined   |
    */

    data.clear();
    data.reserve(30);

    data[0] = 0x00;
    data[0] |= 1 << 0; // UV index sensor
    data[0] |= 1 << 1; // Temperature sensor
    data[0] |= 0 << 2; // Pressure sensor
    data[0] |= 1 << 3; // Humidity sensor
    data[0] |= 0 << 4; // IAQ sensor
    data[0] |= 1 << 5; // dust/particle sensor
    data[0] |= 0 << 6; // not defined
    data[0] |= 0 << 7; // not defined

    data[1] = 0x00; // not defined
    data[2] = 0x00; // not defined

    // Battery
    data[3] = 0x00;

    uint32_t LatitudeBinary = ((telemetry_data->get_lat() + 90) / 180) * 16777215;
    data[4] = (LatitudeBinary >> 16) & 0xFF;
    data[5] = (LatitudeBinary >> 8) & 0xFF;
    data[6] = LatitudeBinary & 0xFF;

    uint32_t LongitudeBinary = ((telemetry_data->get_lng() + 180) / 360) * 16777215;
    data[7] = (LongitudeBinary >> 16) & 0xFF;
    data[8] = (LongitudeBinary >> 8) & 0xFF;
    data[9] = LongitudeBinary & 0xFF;

    uint16_t altitudeGps = telemetry_data->get_alt() / 100;
    data[10] = (altitudeGps >> 8) & 0xFF;
    data[11] = altitudeGps & 0xFF;

    uint8_t hdopGps = telemetry_data->get_hdop() / 10;
    data[12] = hdopGps & 0xFF;

    uint8_t satsGps = telemetry_data->get_satellites();
    data[13] = satsGps & 0xFF;

    uint16_t speedKmhGps = 1.852 * telemetry_data->get_speed() / 100.0;
    data[14] = (speedKmhGps >> 8) & 0xFF;
    data[15] = speedKmhGps & 0xFF;

    uint16_t uv_index = telemetry_data->get_uv_light();
    data[16] = (uv_index >> 8) & 0xFF;
    data[17] = uv_index & 0xFF;

    int8_t temperature = static_cast<int8_t>(telemetry_data->get_temperature());
    data[18] = temperature & 0xFF;

    uint16_t pressure = telemetry_data->get_pressure();
    data[19] = (pressure >> 8) & 0xFF;
    data[20] = pressure & 0xFF;
    
    uint8_t humidity = static_cast<uint8_t>(telemetry_data->get_humidity());
    data[21] = humidity & 0xFF;

    uint16_t iaq = telemetry_data->get_gas();
    data[22] = (iaq >> 8) & 0xFF;
    data[23] = iaq & 0xFF;

    uint16_t pm1_0_cf1 = telemetry_data->get_pm1_0_cf1();
    data[24] = (pm1_0_cf1 >> 8) & 0xFF;
    data[25] = pm1_0_cf1 & 0xFF;

    uint16_t pm2_5_cf1 = telemetry_data->get_pm2_5_cf1();
    data[26] = (pm2_5_cf1 >> 8) & 0xFF;
    data[27] = pm2_5_cf1 & 0xFF;

    uint16_t pm10_0_cf1 = telemetry_data->get_pm10_0_cf1();
    data[28] = (pm10_0_cf1 >> 8) & 0xFF;
    data[29] = pm10_0_cf1 & 0xFF;
}