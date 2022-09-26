#include <Arduino.h>
#include <unity.h>
#include <ArduinoFake.h>
#include <memory>
#include <LoRaCommunicationService.h>
#include <Mutex.h>
#include <LoRaTelemetryDataService.h>
#include <stdio.h>
#include <vector>
#include <string>

using namespace fakeit;
using namespace MobileEnvironmentalMonitoring::Service;
using namespace MobileEnvironmentalMonitoring::Models;
using namespace MobileEnvironmentalMonitoring::Communication;
using namespace MobileEnvironmentalMonitoring::Utils;

Mock<Mutex> mockMutex;
auto mockMutexShrdPtr = std::shared_ptr<Mutex>(&mockMutex.get(), [](auto) {});

Stream *mockStream = ArduinoFakeMock(Stream);
auto mockStreamShrdPtr = std::shared_ptr<Stream>(mockStream, [](auto) {});

Mock<LoRaTelemetryDataService> mockLoRaTelemetryDataService;
auto mockLoRaTelemetryDataServiceShrdPtr = std::shared_ptr<LoRaTelemetryDataService>(&mockLoRaTelemetryDataService.get(), [](auto) {});

// Helpers
int atCalled = 0;
int modeCalled = 0;
int drCalled = 0;
int chCalled = 0;
int idDevEuiCalled = 0;
int idAppEuiCalled = 0;
int classCalled = 0;
int portCalled = 0;
int joinCalled = 0;
int msgCalled = 0;

void setUp(void)
{
    atCalled = 0;
    modeCalled = 0;
    drCalled = 0;
    chCalled = 0;
    idDevEuiCalled = 0;
    idAppEuiCalled = 0;
    classCalled = 0;
    portCalled = 0;
    joinCalled = 0;
    msgCalled = 0;

    ArduinoFakeReset();
    // We also need to fake 'millis()'
    When(Method(ArduinoFake(), millis)).AlwaysReturn(0);
    ArduinoFakeInstance(Stream);

    When(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *))).AlwaysReturn();
    // print get a const char*, but is gone, when we want to check if the method is called with parameter, so we check, when calling with our own method
    When(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT\r\n")))
        .AlwaysDo([](const char *cmd) -> size_t
                  {
        atCalled++;
        return 0; });
    When(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+MODE=LWOTAA\r\n")))
        .AlwaysDo([](const char *cmd) -> size_t
                  {
        modeCalled++;
        return 0; });
    When(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+DR=EU868\r\n")))
        .AlwaysDo([](const char *cmd) -> size_t
                  {
        drCalled++;
        return 0; });
    When(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+CH=NUM,0-2\r\n")))
        .AlwaysDo([](const char *cmd) -> size_t
                  {
        chCalled++;
        return 0; });
    When(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+ID=DevEui,\"deveui\"\r\n")))
        .AlwaysDo([](const char *cmd) -> size_t
                  {
        idDevEuiCalled++;
        return 0; });
    When(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+ID=AppEui,\"appeui\"\r\n")))
        .AlwaysDo([](const char *cmd) -> size_t
                  {
        idAppEuiCalled++;
        return 0; });
    When(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+CLASS=A\r\n")))
        .AlwaysDo([](const char *cmd) -> size_t
                  {
        classCalled++;
        return 0; });
    When(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+PORT=8\r\n")))
        .AlwaysDo([](const char *cmd) -> size_t
                  {
        portCalled++;
        return 0; });
    When(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+JOIN\r\n")))
        .AlwaysDo([](const char *cmd) -> size_t
                  {
        joinCalled++;
        return 0; });
    When(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+MSGHEX=\"000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d\"\r\n")))
        .AlwaysDo([](const char *cmd) -> size_t
                  {
        msgCalled++;
        return 0; });

    mockMutex.Reset();

    When(Method(mockMutex, take)).AlwaysReturn();
    When(Method(mockMutex, give)).AlwaysReturn();

    mockLoRaTelemetryDataService.Reset();

    When(Method(mockLoRaTelemetryDataService, transform))
        .AlwaysDo([](std::shared_ptr<MobileEnvironmentalMonitoring::Models::TelemetryData> telemetry_data, std::vector<uint8_t> &data)
                  {
        data.reserve(30);
        data.clear();
        data[0] = 0x00;
        data[1] = 0x01;
        data[2] = 0x02;
        data[3] = 0x03;
        data[4] = 0x04;
        data[5] = 0x05;
        data[6] = 0x06;
        data[7] = 0x07;
        data[8] = 0x08;
        data[9] = 0x09;
        data[10] = 0x0a;
        data[11] = 0x0b;
        data[12] = 0x0c;
        data[13] = 0x0d;
        data[14] = 0x0e;
        data[15] = 0x0f;
        data[16] = 0x10;
        data[17] = 0x11;
        data[18] = 0x12;
        data[19] = 0x13;
        data[20] = 0x14;
        data[21] = 0x15;
        data[22] = 0x16;
        data[23] = 0x17;
        data[24] = 0x18;
        data[25] = 0x19;
        data[26] = 0x1a;
        data[27] = 0x1b;
        data[28] = 0x1c;
        data[29] = 0x1d; });
}

void setupCtor(void)
{
    When(Method(ArduinoFake(Stream), available))
        .Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 0); // +AT: OK\r\n
    When(Method(ArduinoFake(Stream), read))
        .Return('+', 'A', 'T', ':', ' ', 'O', 'K', '\r', '\n'); // +AT: OK\r\n
}

void setupConnect(void)
{
    When(Method(ArduinoFake(Stream), available))
        .Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                    // +AT: OK\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                    // +AT: OK\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                               // +MODE: LWOTAA\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                        // +DR: EU868\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                              // +CH: NUM\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                     // +ID: DevEui\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                     // +ID: AppEui\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                           // +CLASS: A\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                              // +PORT: 8\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                         // +JOIN: Starting\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                               // +JOIN: NORMAL\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, // +JOIN: NetID 000024 DevAddr\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0                                                                                      // +JOIN: Done\r\n
        );
    When(Method(ArduinoFake(Stream), read))
        .Return('+', 'A', 'T', ':', ' ', 'O', 'K', '\r', '\n',                                                                                                                                                                 // +AT: OK\r\n
                '+', 'A', 'T', ':', ' ', 'O', 'K', '\r', '\n',                                                                                                                                                                 // +AT: OK\r\n
                '+', 'M', 'O', 'D', 'E', ':', ' ', 'L', 'W', 'O', 'T', 'A', 'A', '\r', '\n',                                                                                                                                   // +MODE: LWOTAA\r\n
                '+', 'D', 'R', ':', ' ', 'E', 'U', '8', '6', '8', '\r', '\n',                                                                                                                                                  // +DR: EU868\r\n
                '+', 'C', 'H', ':', ' ', 'N', 'U', 'M', '\r', '\n',                                                                                                                                                            // +CH: NUM\r\n
                '+', 'I', 'D', ':', ' ', 'D', 'e', 'v', 'E', 'u', 'i', '\r', '\n',                                                                                                                                             // +ID: DevEui\r\n
                '+', 'I', 'D', ':', ' ', 'A', 'p', 'p', 'E', 'u', 'i', '\r', '\n',                                                                                                                                             // +ID: AppEui\r\n
                '+', 'C', 'L', 'A', 'S', 'S', ':', ' ', 'A', '\r', '\n',                                                                                                                                                       // +CLASS: A\r\n
                '+', 'P', 'O', 'R', 'T', ':', ' ', '8', '\r', '\n',                                                                                                                                                            // +PORT: 8\r\n
                '+', 'J', 'O', 'I', 'N', ':', ' ', 'S', 't', 'a', 'r', 't', 'i', 'n', 'g', '\r', '\n',                                                                                                                         // +JOIN: Starting\r\n
                '+', 'J', 'O', 'I', 'N', ':', ' ', 'N', 'O', 'R', 'M', 'A', 'L', '\r', '\n',                                                                                                                                   // +JOIN: NORMAL\r\n
                '+', 'J', 'O', 'I', 'N', ':', ' ', 'N', 'e', 't', 'I', 'D', ' ', '0', '0', '0', '0', '2', '4', ' ', 'D', 'e', 'v', 'A', 'd', 'd', 'r', ' ', '4', '8', ':', '0', '0', ':', '0', '0', ':', '0', '1', '\r', '\n', // +JOIN: NetID 000024 DevAddr 48:00:00:01\r\n
                '+', 'J', 'O', 'I', 'N', ':', ' ', 'D', 'o', 'n', 'e', '\r', '\n'                                                                                                                                              // +JOIN: Done\r\n
        );
}

void setupSendData(void)
{
    When(Method(ArduinoFake(Stream), available))
        .Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                                               // +AT: OK\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                                               // +AT: OK\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                          // +MODE: LWOTAA\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                                   // +DR: EU868\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                                         // +CH: NUM\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                                // +ID: DevEui\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                                // +ID: AppEui\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                                      // +CLASS: A\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                                         // +PORT: 8\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                    // +JOIN: Starting\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                          // +JOIN: NORMAL\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                            // +JOIN: NetID 000024 DevAddr\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                                // +JOIN: Done\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                                       // +MSGHEX: Start\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                                                                              // +MSGHEX: FPENDING\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, // +MSGHEX: PORT: 1; RX: "48656C6C6F20576F726C6421"\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,                                        // +MSGHEX: RXWIN1, RSSI -79, SNR 13.0\r\n
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0                                                                                                           // +MSGHEX: Done\r\n
        );
    When(Method(ArduinoFake(Stream), read))
        .Return('+', 'A', 'T', ':', ' ', 'O', 'K', '\r', '\n',                                                                                                                                                                                                              // +AT: OK\r\n
                '+', 'A', 'T', ':', ' ', 'O', 'K', '\r', '\n',                                                                                                                                                                                                              // +AT: OK\r\n
                '+', 'M', 'O', 'D', 'E', ':', ' ', 'L', 'W', 'O', 'T', 'A', 'A', '\r', '\n',                                                                                                                                                                                // +MODE: LWOTAA\r\n
                '+', 'D', 'R', ':', ' ', 'E', 'U', '8', '6', '8', '\r', '\n',                                                                                                                                                                                               // +DR: EU868\r\n
                '+', 'C', 'H', ':', ' ', 'N', 'U', 'M', '\r', '\n',                                                                                                                                                                                                         // +CH: NUM\r\n
                '+', 'I', 'D', ':', ' ', 'D', 'e', 'v', 'E', 'u', 'i', '\r', '\n',                                                                                                                                                                                          // +ID: DevEui\r\n
                '+', 'I', 'D', ':', ' ', 'A', 'p', 'p', 'E', 'u', 'i', '\r', '\n',                                                                                                                                                                                          // +ID: AppEui\r\n
                '+', 'C', 'L', 'A', 'S', 'S', ':', ' ', 'A', '\r', '\n',                                                                                                                                                                                                    // +CLASS: A\r\n
                '+', 'P', 'O', 'R', 'T', ':', ' ', '8', '\r', '\n',                                                                                                                                                                                                         // +PORT: 8\r\n
                '+', 'J', 'O', 'I', 'N', ':', ' ', 'S', 't', 'a', 'r', 't', 'i', 'n', 'g', '\r', '\n',                                                                                                                                                                      // +JOIN: Starting\r\n
                '+', 'J', 'O', 'I', 'N', ':', ' ', 'N', 'O', 'R', 'M', 'A', 'L', '\r', '\n',                                                                                                                                                                                // +JOIN: NORMAL\r\n
                '+', 'J', 'O', 'I', 'N', ':', ' ', 'N', 'e', 't', 'I', 'D', ' ', '0', '0', '0', '0', '2', '4', ' ', 'D', 'e', 'v', 'A', 'd', 'd', 'r', ' ', '4', '8', ':', '0', '0', ':', '0', '0', ':', '0', '1', '\r', '\n',                                              // +JOIN: NetID 000024 DevAddr 48:00:00:01\r\n
                '+', 'J', 'O', 'I', 'N', ':', ' ', 'D', 'o', 'n', 'e', '\r', '\n',                                                                                                                                                                                          // +JOIN: Done\r\n
                '+', 'M', 'S', 'G', 'H', 'E', 'X', ':', ' ', 'S', 't', 'a', 'r', 't', '\r', '\n',                                                                                                                                                                           // +MSGHEX: Start\r\n
                '+', 'M', 'S', 'G', 'H', 'E', 'X', ':', ' ', 'F', 'P', 'E', 'N', 'D', 'I', 'N', 'G', '\r', '\n',                                                                                                                                                            // +MSGHEX: FPENDING\r\n
                '+', 'M', 'S', 'G', 'H', 'E', 'X', ':', ' ', 'P', 'O', 'R', 'T', ':', ' ', '1', ';', ' ', 'R', 'X', ':', ' ', '"', '4', '8', '6', '5', '6', 'C', '6', 'C', '6', 'F', '2', '0', '5', '7', '6', 'F', '7', '2', '6', 'C', '6', '4', '2', '1', '"', '\r', '\n', // +MSGHEX: PORT: 1; RX: "48656C6C6F20576F726C6421"\r\n
                '+', 'M', 'S', 'G', 'H', 'E', 'X', ':', ' ', 'R', 'X', 'W', 'I', 'N', '1', ',', ' ', 'R', 'S', 'S', 'I', ' ', '-', '7', '9', ',', ' ', 'S', 'N', 'R', ' ', '1', '3', '.', '0', '\r', '\n',                                                                  // +MSGHEX: RXWIN1, RSSI -79, SNR 13.0\r\n
                '+', 'M', 'S', 'G', 'H', 'E', 'X', ':', ' ', 'D', 'o', 'n', 'e', '\r', '\n'                                                                                                                                                                                 // +MSGHEX: Done\r\n
        );
}

void tearDown(void)
{
    // clean stuff up here
}

void test_should_test_communication_to_sensor_in_ctor(void)
{
    setupCtor();

    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");

    // Should call print("AT\r\n")
    // Verify(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT\r\n"))).Once();
    TEST_ASSERT_EQUAL_INT32(1, atCalled);

    // Should take and give mutex
    Verify(Method(mockMutex, take)).Once();
    Verify(Method(mockMutex, give)).Once();
}

void test_should_test_communication_to_sensor_when_calling_connect(void)
{
    setupConnect();

    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");

    sut.connect();

    // Verify(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT\r\n"))).Twice();
    TEST_ASSERT_EQUAL_INT32(atCalled, 2);
}

void test_should_return_false_when_test_communication_to_sensor_timeouts_when_calling_connect(void)
{
    When(Method(ArduinoFake(Stream), available))
        .Return(1, 1, 1, 1, 1, 1, 1, 1, 1, 0, // +AT: OK\r\n  for CTOR
                0);
    When(Method(ArduinoFake(Stream), read))
        .Return('+', 'A', 'T', ':', ' ', 'O', 'K', '\r', '\n' // +AT: OK\r\n for CTOR
        );

    When(Method(ArduinoFake(), millis)).Return(0, 1, 2, 102);

    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");

    auto result = sut.connect();

    TEST_ASSERT_FALSE(result);
}

void test_should_set_MODE_to_LWOTAA_on_connect()
{
    setupConnect();

    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");

    sut.connect();

    // Verify(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+MODE=LWOTAA\r\n"))).Once();
    TEST_ASSERT_EQUAL_INT32(1, modeCalled);
}

void test_should_set_DR_to_EU868_on_connect()
{
    setupConnect();

    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");

    sut.connect();

    // Verify(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+DR=868\r\n"))).Once();
    TEST_ASSERT_EQUAL_INT32(1, drCalled);
}

void test_should_set_CH_to_NUM_0_2_on_connect()
{
    setupConnect();

    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");

    sut.connect();

    // Verify(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+CH=NUM,0-2\r\n"))).Once();
    TEST_ASSERT_EQUAL_INT32(1, chCalled);
}

void test_should_set_DevEui_on_connect()
{
    setupConnect();

    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");

    sut.connect();

    // Verify(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+ID=DevEui,\"deveui\"\r\n"))).Once();
    TEST_ASSERT_EQUAL_INT32(1, idDevEuiCalled);
}

void test_should_set_AppEui_on_connect()
{
    setupConnect();

    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");

    sut.connect();

    // Verify(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+ID=AppEui,\"appeui\"\r\n"))).Once();
    TEST_ASSERT_EQUAL_INT32(1, idDevEuiCalled);
}

void test_should_set_class_to_a_on_connect()
{
    setupConnect();

    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");

    sut.connect();

    // Verify(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+CLASS=A\r\n"))).Once();
    TEST_ASSERT_EQUAL_INT32(1, classCalled);
}

void test_should_set_port_to_8_on_connect()
{
    setupConnect();

    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");

    sut.connect();

    // Verify(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+PORT=8\r\n"))).Once();
    TEST_ASSERT_EQUAL_INT32(1, portCalled);
}

void test_should_call_join_connect()
{
    setupConnect();

    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");

    sut.connect();

    // Verify(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+PORT=8\r\n"))).Once();
    TEST_ASSERT_EQUAL_INT32(1, joinCalled);
}

void test_send_data_should_return_false_when_not_connected()
{
    setupCtor();

    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");

    auto data = std::make_shared<MobileEnvironmentalMonitoring::Models::TelemetryData>();

    auto result = sut.send_data(data);

    TEST_ASSERT_FALSE(result);
}

void test_send_data_should_transform_the_data()
{
    setupSendData();
    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");
    sut.connect();

    auto data = std::make_shared<MobileEnvironmentalMonitoring::Models::TelemetryData>();

    auto result = sut.send_data(data);

    Verify(Method(mockLoRaTelemetryDataService, transform).Using(data, Any())).Once();
}

void test_send_data_should_send_correct_data()
{
    setupSendData();
    auto sut = LoRaCommunicationService(mockMutexShrdPtr, mockStreamShrdPtr, mockLoRaTelemetryDataServiceShrdPtr, "deveui", "appeui", "appkey");
    sut.connect();

    auto data = std::make_shared<MobileEnvironmentalMonitoring::Models::TelemetryData>();

    auto result = sut.send_data(data);

    // Verify(OverloadedMethod(ArduinoFake(Stream), print, size_t(const char *)).Using(StrEq("AT+MSGHEX=\"000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d\"\r\n"))).Once();
    TEST_ASSERT_EQUAL_INT32(1, msgCalled);
}

int main(int argc, char **argv)
{
    UNITY_BEGIN();

    // Ctor
    RUN_TEST(test_should_test_communication_to_sensor_in_ctor);

    // connect()
    RUN_TEST(test_should_test_communication_to_sensor_when_calling_connect);
    RUN_TEST(test_should_return_false_when_test_communication_to_sensor_timeouts_when_calling_connect);
    RUN_TEST(test_should_set_MODE_to_LWOTAA_on_connect);
    RUN_TEST(test_should_set_DR_to_EU868_on_connect);
    RUN_TEST(test_should_set_CH_to_NUM_0_2_on_connect);
    RUN_TEST(test_should_set_DevEui_on_connect);
    RUN_TEST(test_should_set_AppEui_on_connect);
    RUN_TEST(test_should_set_class_to_a_on_connect);
    RUN_TEST(test_should_set_port_to_8_on_connect);
    RUN_TEST(test_should_call_join_connect);

    // send_data(...)
    RUN_TEST(test_send_data_should_return_false_when_not_connected);
    RUN_TEST(test_send_data_should_transform_the_data);
    RUN_TEST(test_send_data_should_send_correct_data);

    return UNITY_END();
}