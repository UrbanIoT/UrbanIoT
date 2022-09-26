/* Delete existing table */
IF  EXISTS (SELECT *
FROM sys.objects
WHERE object_id = OBJECT_ID(N'[dbo].[telemetry]') AND type in (N'U'))
DROP TABLE [dbo].[telemetry]
GO

/* Create Telemetry table */
CREATE TABLE [dbo].[telemetry]
(
    [deviceid] varchar(50) NOT NULL,
    [timestamp] datetime NOT NULL,
    [json] varchar(max) NOT NULL,
    /* Sensor configuration */
    [sensor_uv]  AS CONVERT(bit,json_value([json],'$.uplink_message.decoded_payload.sensors.uv')) PERSISTED,
    [sensor_temp]  AS CONVERT(bit,json_value([json],'$.uplink_message.decoded_payload.sensors.temp')) PERSISTED,
    [sensor_pressure]  AS CONVERT(bit,json_value([json],'$.uplink_message.decoded_payload.sensors.pressure')) PERSISTED,
    [sensor_humidity]  AS CONVERT(bit,json_value([json],'$.uplink_message.decoded_payload.sensors.humidity')) PERSISTED,
    [sensor_iaq]  AS CONVERT(bit,json_value([json],'$.uplink_message.decoded_payload.sensors.iaq')) PERSISTED,
    [sensor_dust]  AS CONVERT(bit,json_value([json],'$.uplink_message.decoded_payload.sensors.dust')) PERSISTED,
    /* GPS data */
    [location] AS  geography::Point(CONVERT(float,json_value([json],'$.uplink_message.decoded_payload.latitude')), CONVERT(float,json_value([json],'$.uplink_message.decoded_payload.longitude')), 4326) PERSISTED,
    -- in cm
    [alt]  AS CONVERT(float,json_value([json],'$.uplink_message.decoded_payload.altitude')) PERSISTED,
    [hdop]  AS CONVERT(float,json_value([json],'$.uplink_message.decoded_payload.hdop')) PERSISTED,
    [sats]  AS CONVERT(tinyint,json_value([json],'$.uplink_message.decoded_payload.sats')) PERSISTED,
    -- in km/h
    [speed]  AS CONVERT(float,json_value([json],'$.uplink_message.decoded_payload.speed')) PERSISTED,
    /* Sensor data */
    [uv_index]  AS CONVERT(float,json_value([json],'$.uplink_message.decoded_payload.uv_index')) PERSISTED,
    [temp]  AS CONVERT(float,json_value([json],'$.uplink_message.decoded_payload.temperature')) PERSISTED,
    [pressure]  AS CONVERT(float,json_value([json],'$.uplink_message.decoded_payload.pressure')) PERSISTED,
    [hum]  AS CONVERT(float,json_value([json],'$.uplink_message.decoded_payload.humidity')) PERSISTED,
    [iaq]  AS CONVERT(float,json_value([json],'$.uplink_message.decoded_payload.iaq')) PERSISTED,
    [pm1_0_cf1]  AS CONVERT(float,json_value([json],'$.uplink_message.decoded_payload.pm1_0_cf1')) PERSISTED,
    [pm2_5_cf1]  AS CONVERT(float,json_value([json],'$.uplink_message.decoded_payload.pm2_5_cf1')) PERSISTED,
    [pm10_0_cf1]  AS CONVERT(float,json_value([json],'$.uplink_message.decoded_payload.pm10_0_cf1')) PERSISTED,
) ON [PRIMARY] TEXTIMAGE_ON [PRIMARY] WITH (DATA_COMPRESSION = PAGE)
GO

/* Create clustered index on timestamp asc */
CREATE CLUSTERED INDEX [clTelemetry] ON [dbo].[telemetry]
(
	[timestamp] ASC
)
WITH (STATISTICS_NORECOMPUTE = OFF, DROP_EXISTING = OFF, ONLINE = OFF, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF, DATA_COMPRESSION = PAGE) ON [PRIMARY]
GO

/* Create non clustered index on deviceid asc, include the property columns */
CREATE NONCLUSTERED INDEX [ncclTelemetry] ON [dbo].[telemetry]
(
	[deviceid] ASC
)
INCLUDE([sensor_uv], [sensor_temp], [sensor_pressure], [sensor_humidity], [sensor_iaq], [sensor_dust], [location], [alt], [hdop], [sats], [speed], [uv_index], [temp], [pressure], [hum], [iaq], [pm1_0_cf1], [pm2_5_cf1], [pm10_0_cf1]) WITH (STATISTICS_NORECOMPUTE = OFF, DROP_EXISTING = OFF, ONLINE = OFF, OPTIMIZE_FOR_SEQUENTIAL_KEY = OFF) ON [PRIMARY]
GO


/* Create time series view */
-- IF NOT EXISTS (SELECT * FROM sys.views WHERE object_id = OBJECT_ID(N'[dbo].[vTimeSeriesBuckets]'))
-- EXEC dbo.sp_executesql @statement = N'
-- CREATE VIEW [dbo].[vTimeSeriesBuckets]
-- AS
-- SELECT 
-- 	DATEADD(MINUTE, DATEDIFF(MINUTE, ''2000'', timestamp) / 10 * 10, ''2000'') as timeslot,
-- 	deviceid,
-- 	AVGCONVERT(decimal(4,2),temp)) as temp,
--     AVGCONVERT(decimal(4,2),hum)) as hum,
-- FROM 
-- 	dbo.telemetry 
-- WHERE 
-- 	timestamp > DATEADD(HOUR, -12, getdate())
-- GROUP BY DATEADD(MINUTE, DATEDIFF(MINUTE, ''2000'', timestamp) / 10 * 10, ''2000''), deviceid
-- ' 
-- GO