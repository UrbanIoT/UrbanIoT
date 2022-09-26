using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Extensions.Http;
using Microsoft.AspNetCore.Http;
using Microsoft.Data.SqlClient;
using Microsoft.Extensions.Logging;

namespace UrbanIoT.HttpFunction
{
    public class HttpTrigger
    {
        private const string Sql = "SELECT TOP (1000) [deviceid] ,[timestamp] ,[sensor_uv] ,[sensor_temp] ,[sensor_pressure] ,[sensor_humidity] ,[sensor_iaq] ,[sensor_dust] ,[location].Lat as Latitude ,[location].Long as Longitude ,[alt] ,[hdop] ,[sats] ,[speed] ,[uv_index] ,[temp] ,[pressure] ,[hum] ,[iaq] ,[pm1_0_cf1] ,[pm2_5_cf1] ,[pm10_0_cf1] FROM [dbo].[telemetry] WHERE [location].STWithin(GEOGRAPHY::STPolyFromText('POLYGON(({0} {1}, {2} {1}, {2} {3}, {0} {3}, {0} {1}))', 4326)) = 1 ORDER BY [timestamp] DESC;";

        private const string EnvSqlDbConnectionString = "SQLDBConnectionString";

        private static readonly string SqlDbConnectionString =
            Environment
                .GetEnvironmentVariable(
                    EnvSqlDbConnectionString,
                    EnvironmentVariableTarget.Process);

        public HttpTrigger()
        {
            if (string.IsNullOrEmpty(SqlDbConnectionString))
            {
                throw new ArgumentException("Azure SQL Database connection string is not valid.",
                    nameof(SqlDbConnectionString));
            }
        }

        [FunctionName("HttpTrigger")]
        public static async Task<IActionResult> Run(
            [HttpTrigger(AuthorizationLevel.Anonymous, "get", Route = "from/{latFrom:float}/{lngFrom:float}/to/{latTo:float}/{lngTo:float}")] HttpRequest req,
            float latFrom, float lngFrom, float latTo, float lngTo,
            ILogger log)
        {
            log.LogInformation($"C# HTTP trigger function processed a request: {latFrom}, {lngFrom}, {latTo}, {lngTo}");

            var sqlString = string.Format(Sql, latFrom, lngFrom, latTo, lngTo);

            var features = new List<Feature>();

            using (var conn = new SqlConnection(SqlDbConnectionString))
            {
                conn.Open();

                using (var cmd = new SqlCommand(sqlString, conn))
                {
                    using (var reader = await cmd.ExecuteReaderAsync())
                    {
                        while (await reader.ReadAsync())
                        {
                            features.Add(
                                new Feature
                                {
                                    Type = "Feature",
                                    Geometry = new FeatureGeometry
                                    {
                                        Type = "Point",
                                        Coordinates = new double[2] { reader.GetDouble(9), reader.GetDouble(8) },
                                    },
                                    Properties = new FeatureProperties
                                    {
                                        DeviceId = reader.GetString(0),
                                        Timestamp = reader.GetDateTime(1),
                                        Uv_Sensor = reader.GetBoolean(2),
                                        Temp_Sensor = reader.GetBoolean(3),
                                        Pressure_Sensor = reader.GetBoolean(4),
                                        Humidity_Sensor = reader.GetBoolean(5),
                                        Iaq_Sensor = reader.GetBoolean(6),
                                        Dust_Sensor = reader.GetBoolean(7),
                                        Alt = reader.GetDouble(10),
                                        Hdop = reader.GetDouble(11),
                                        Sats = reader.GetByte(12),
                                        Speed = reader.GetDouble(13),
                                        UvIndex = reader.GetDouble(14),
                                        Temp = reader.GetDouble(15),
                                        Pressure = reader.GetDouble(16),
                                        Hum = reader.GetDouble(17),
                                        Iaq = reader.GetDouble(18),
                                        Pm_1_0_Cf1 = reader.GetDouble(19),
                                        Pm_2_5_Cf1 = reader.GetDouble(20),
                                        Pm_10_0_Cf1 = reader.GetDouble(21),
                                    }
                                }
                                );
                        }
                    }
                }
            }

            return new OkObjectResult(new
            {
                type = "FeatureCollection",
                features = features
            });
        }
    }
}
