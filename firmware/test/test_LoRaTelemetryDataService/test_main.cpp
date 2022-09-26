#include <Arduino.h>
#include <unity.h>
#include <memory>
#include <vector>
#include <TelemetryData.h>
#include <LoRaTelemetryDataService.h>

using namespace fakeit;
using namespace MobileEnvironmentalMonitoring::Service;
using namespace MobileEnvironmentalMonitoring::Models;

auto telemetryData = std::make_shared<TelemetryData>();
std::vector<uint8_t> dataVector;
auto sut = LoRaTelemetryDataService();

void setUp(void)
{
    ArduinoFakeReset();
}

void tearDown(void)
{
    dataVector.clear();
}

void test_sensor_bytes_for_uv_index_sensor(void)
{
    sut.transform(telemetryData, dataVector);
    TEST_ASSERT_EQUAL(1, (dataVector[0] >> 0) & 1);
}

void test_sensor_bytes_for_temperature_sensor(void)
{
    sut.transform(telemetryData, dataVector);
    TEST_ASSERT_EQUAL(1, (dataVector[0] >> 1) & 1);
}

void test_sensor_bytes_for_pressure_sensor(void)
{
    sut.transform(telemetryData, dataVector);
    TEST_ASSERT_EQUAL(0, (dataVector[0] >> 2) & 1);
}

void test_sensor_bytes_for_humidity_sensor(void)
{
    sut.transform(telemetryData, dataVector);
    TEST_ASSERT_EQUAL(1, (dataVector[0] >> 3) & 1);
}

void test_sensor_bytes_for_iaq_sensor(void)
{
    sut.transform(telemetryData, dataVector);
    TEST_ASSERT_EQUAL(0, (dataVector[0] >> 4) & 1);
}

void test_sensor_bytes_for_dust_particle_sensor(void)
{
    sut.transform(telemetryData, dataVector);
    TEST_ASSERT_EQUAL(1, (dataVector[0] >> 5) & 1);
}

void test_sensor_bytes_rest_not_defined(void)
{
    sut.transform(telemetryData, dataVector);
    TEST_ASSERT_EQUAL(0, (dataVector[0] >> 6) & 1);
    TEST_ASSERT_EQUAL(0, (dataVector[0] >> 7) & 1);
    TEST_ASSERT_EQUAL(0, dataVector[1]);
    TEST_ASSERT_EQUAL(0, dataVector[2]);
}

void test_sensor_battery(void)
{
    sut.transform(telemetryData, dataVector);
    TEST_ASSERT_EQUAL(0, dataVector[3]);
}

void test_sensor_latitude(void)
{
    telemetryData->set_lat(1.23);

    sut.transform(telemetryData, dataVector);

    auto result = ((dataVector[4] << 16) | (dataVector[5] << 8) | (dataVector[6] << 0)) / 16777215.0 * 180 - 90;
    TEST_ASSERT_EQUAL(1.23, result);
}

void test_sensor_longitude(void)
{
    telemetryData->set_lng(2.34);

    sut.transform(telemetryData, dataVector);

    auto result = ((dataVector[7] << 16) | (dataVector[8] << 8) | (dataVector[9] << 0)) / 16777215.0 * 360 - 180;
    TEST_ASSERT_EQUAL(2.34, result);
}

void test_sensor_altitude(void)
{
    telemetryData->set_alt(123456);

    sut.transform(telemetryData, dataVector);

    auto result = dataVector[10] << 8 | dataVector[11];
    TEST_ASSERT_EQUAL(1234, result);
}

void test_sensor_hdop(void)
{
    telemetryData->set_hdop(200);

    sut.transform(telemetryData, dataVector);

    TEST_ASSERT_EQUAL(20, dataVector[12]);
}

void test_sensor_sats(void)
{
    telemetryData->set_satellites(3);

    sut.transform(telemetryData, dataVector);

    TEST_ASSERT_EQUAL(3, dataVector[13]);
}

void test_sensor_speed(void)
{
    telemetryData->set_speed(2000);

    sut.transform(telemetryData, dataVector);

    // 20 knots is ~37km/h
    TEST_ASSERT_EQUAL(37, dataVector[14] << 8 | dataVector[15]);
}

void test_sensor_uv_index(void)
{
    telemetryData->set_uv_light(8*100);

    sut.transform(telemetryData, dataVector);

    TEST_ASSERT_EQUAL(800, dataVector[16] << 8 | dataVector[17]);
}

void test_sensor_temperature(void)
{
    telemetryData->set_temperature(-12.6f);

    sut.transform(telemetryData, dataVector);

    TEST_ASSERT_EQUAL(-12, static_cast<int8_t>(dataVector[18]));
}

void test_sensor_pressure(void)
{
    telemetryData->set_pressure(1013);

    sut.transform(telemetryData, dataVector);

    TEST_ASSERT_EQUAL(1013, dataVector[19] << 8 | dataVector[20]);
}

void test_sensor_humidity(void)
{
    telemetryData->set_humidity(12.3f);

    sut.transform(telemetryData, dataVector);

    TEST_ASSERT_EQUAL(12, dataVector[21]);
}

void test_sensor_gas(void)
{
    telemetryData->set_gas(1234);

    sut.transform(telemetryData, dataVector);

    TEST_ASSERT_EQUAL(1234, dataVector[22] << 8 | dataVector[23]);
}

void test_sensor_pm1_0_cf1(void)
{
    telemetryData->set_pm1_0_cf1(1122);

    sut.transform(telemetryData, dataVector);

    TEST_ASSERT_EQUAL(1122, dataVector[24] << 8 | dataVector[25]);
}

void test_sensor_pm2_5_cf1(void)
{
    telemetryData->set_pm2_5_cf1(1002);

    sut.transform(telemetryData, dataVector);

    TEST_ASSERT_EQUAL(1002, dataVector[26] << 8 | dataVector[27]);
}

void test_sensor_pm10_0_cf1(void)
{
    telemetryData->set_pm10_0_cf1(1330);

    sut.transform(telemetryData, dataVector);

    TEST_ASSERT_EQUAL(1330, dataVector[28] << 8 | dataVector[29]);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    RUN_TEST(test_sensor_bytes_for_uv_index_sensor);
    RUN_TEST(test_sensor_bytes_for_temperature_sensor);
    RUN_TEST(test_sensor_bytes_for_pressure_sensor);
    RUN_TEST(test_sensor_bytes_for_humidity_sensor);
    RUN_TEST(test_sensor_bytes_for_iaq_sensor);
    RUN_TEST(test_sensor_bytes_for_dust_particle_sensor);
    RUN_TEST(test_sensor_bytes_rest_not_defined);

    RUN_TEST(test_sensor_battery);
    RUN_TEST(test_sensor_latitude);
    RUN_TEST(test_sensor_longitude);
    RUN_TEST(test_sensor_altitude);
    RUN_TEST(test_sensor_hdop);
    RUN_TEST(test_sensor_sats);
    RUN_TEST(test_sensor_speed);
    RUN_TEST(test_sensor_uv_index);
    RUN_TEST(test_sensor_temperature);
    RUN_TEST(test_sensor_pressure);
    RUN_TEST(test_sensor_humidity);
    RUN_TEST(test_sensor_gas);
    RUN_TEST(test_sensor_pm1_0_cf1);
    RUN_TEST(test_sensor_pm2_5_cf1);
    RUN_TEST(test_sensor_pm10_0_cf1);

    UNITY_END();
}