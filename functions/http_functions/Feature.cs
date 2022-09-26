using System;

namespace UrbanIoT.HttpFunction;

public class FeatureGeometry
{
    public string Type { get; set; }
    public double[] Coordinates { get; set; }
}

public class FeatureProperties
{
    public string DeviceId { get; set; }
    public DateTime Timestamp { get; set; }
    public bool Uv_Sensor { get; set; }
    public bool Temp_Sensor { get; set; }
    public bool Pressure_Sensor { get; set; }
    public bool Humidity_Sensor { get; set; }
    public bool Iaq_Sensor { get; set; }
    public bool Dust_Sensor { get; set; }
    public double Alt { get; set; }
    public double Hdop { get; set; }
    public int Sats { get; set; }
    public double Speed { get; set; }
    public double UvIndex { get; set; }
    public double Temp { get; set; }
    public double Pressure { get; set; }
    public double Hum { get; set; }
    public double Iaq { get; set; }
    public double Pm_1_0_Cf1 { get; set; }
    public double Pm_2_5_Cf1 { get; set; }
    public double Pm_10_0_Cf1 { get; set; }
}

public class Feature
{
    public string Type { get; set; }
    public FeatureGeometry Geometry { get; set; }
    public FeatureProperties Properties { get; set; }
}