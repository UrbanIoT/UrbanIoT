#pragma once

namespace MobileEnvironmentalMonitoring::Models
{
    class GpsData
    {
    public:
        bool got_fix;
        double lat; // Latitude in degrees (double)
        double lng; // Longitude in degrees (double)
        int32_t alt; // Raw altitude in centimeters (i32)
        uint32_t time; // Raw time in HHMMSSCC format (u32)
        uint32_t date; // Raw date in DDMMYY format (u32)
        int32_t speed; // Raw speed in 100ths of a knot (i32)
        int32_t course; // Raw course in 100ths of a degree (i32)
        uint32_t satellites; // Number of satellites in use (u32)
        int32_t hdop; // Horizontal Dim. of Precision (100ths-i32)
    };
}