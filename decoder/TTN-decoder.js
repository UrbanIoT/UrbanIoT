function decodeUplink(input) {
    var bytes = input.bytes;
    var decoded = {};

    // Sensor config
    var sensors = {};
    sensors.uv = ((bytes[0] >> 0) & 1) == 1;
    sensors.temp = ((bytes[0] >> 1) & 1) == 1;
    sensors.pressure = ((bytes[0] >> 2) & 1) == 1;
    sensors.humidity = ((bytes[0] >> 3) & 1) == 1;
    sensors.iaq = ((bytes[0] >> 4) & 1) == 1;
    sensors.dust = ((bytes[0] >> 5) & 1) == 1;

    decoded.sensors = sensors;

    decoded.battery = bytes[3];

    // GPS data
    decoded.latitude = ((bytes[4] << 16) >>> 0) + ((bytes[5] << 8) >>> 0) + bytes[5];
    decoded.latitude = (decoded.latitude / 16777215.0 * 180) - 90;

    decoded.longitude = ((bytes[7] << 16) >>> 0) + ((bytes[8] << 8) >>> 0) + bytes[8];
    decoded.longitude = (decoded.longitude / 16777215.0 * 360) - 180;

    var altValue = ((bytes[10] << 8) >>> 0) + bytes[11];
    var sign = bytes[10] & (1 << 7);
    if (sign) decoded.altitude = 0xFFFF0000 | altValue;
    else decoded.altitude = altValue;

    decoded.hdop = bytes[12] / 10.0;
    decoded.sats = bytes[13];

    // in km/h
    // decoded.speed = 1.852 * (((bytes[14] << 8) >>> 0) + bytes[15]) / 100.0;
    decoded.speed = ((bytes[14] << 8) >>> 0) + bytes[15];

    // Sensor data
    decoded.uv_index = (((bytes[16] << 8) >>> 0) + bytes[17]) / 100.0;
    decoded.temperature = bytes[18];
    decoded.pressure = ((bytes[19] << 8) >>> 0) + bytes[20];
    decoded.humidity = bytes[21];
    decoded.iaq = ((bytes[22] << 8) >>> 0) + bytes[23];
    decoded.pm1_0_cf1 = ((bytes[24] << 8) >>> 0) + bytes[25];
    decoded.pm2_5_cf1 = ((bytes[26] << 8) >>> 0) + bytes[27];
    decoded.pm10_0_cf1 = ((bytes[28] << 8) >>> 0) + bytes[29];

    return {
        data: decoded
    }
}