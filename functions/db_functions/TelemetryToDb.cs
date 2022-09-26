using System;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.Text;
using System.Threading.Tasks;
using Azure.Messaging.EventHubs;
using Microsoft.ApplicationInsights;
using Microsoft.ApplicationInsights.Extensibility;
using Microsoft.Azure.WebJobs;
using Microsoft.Data.SqlClient;
using Microsoft.Extensions.Logging;

namespace UrbanIoT.Functions
{
    public class TelemetryToDb
    {
        private const string EnvSqlDbConnectionString = "SQLDBConnectionString";

        private static readonly string SqlDbConnectionString =
            Environment
                .GetEnvironmentVariable(
                    EnvSqlDbConnectionString,
                    EnvironmentVariableTarget.Process);
        
        private readonly CustomTelemetry _customTelemetry;

        public TelemetryToDb(TelemetryConfiguration telemetryConfiguration)
        {
            var telemetryClient = new TelemetryClient(telemetryConfiguration);
            _customTelemetry = new CustomTelemetry(telemetryClient);
        
            if (string.IsNullOrEmpty(SqlDbConnectionString))
            {
                throw new ArgumentException("Azure SQL Database connection string is not valid.",
                    nameof(SqlDbConnectionString));
            }
        }

        [FunctionName("TelemetryToDb")]
        public async Task Run(
            [EventHubTrigger("telemetry", Connection = "EventHubConnectionAppSetting")] EventData[] messages, ExecutionContext context, ILogger log)
        {

            var ticksUtcNow = DateTimeOffset.UtcNow;

            _customTelemetry.TrackMetric(
                context,
                "IoTHubMessagesReceived",
                messages.Length);

            // Track whether messages are arriving at the function late.
            if (messages[0]?.SystemProperties["iothub-enqueuedtime"] is DateTime firstMsgEnqueuedTicksUtc)
            {
                _customTelemetry.TrackMetric(
                    context,
                    "IoTHubMessagesReceivedFreshnessMsec",
                    (ticksUtcNow - firstMsgEnqueuedTicksUtc)
                    .TotalMilliseconds);
            }

            log.LogInformation(
                "Starting load | Docs to bulk load {Length}", messages.Length);

            // Bulk load events
            var sqlDbTotalMilliseconds = await BulkLoadEvents(messages, log);

            _customTelemetry.TrackMetric(
                context,
                "IoTHubMessagesDropped",
                messages.Length);
            _customTelemetry.TrackMetric(
                context,
                "SqlDbDocumentsCreated",
                messages.Length);
            var latency = messages.Length > 0
                ? sqlDbTotalMilliseconds / messages.Length
                : 0;
            _customTelemetry.TrackMetric(
                context,
                "SqlDbLatencyMsec",
                latency);

            foreach (var message in messages)
            {
                log.LogInformation(
                    "C# function triggered to process a message: {Body}", Encoding.UTF8.GetString(message.Body.ToArray()));
                log.LogInformation("EnqueuedTimeUtc={EnqueuedTimeUtc}", message.SystemProperties["iothub-enqueuedtime"]);
            }
        }
        
        private static async Task<long> BulkLoadEvents(
                            IEnumerable<EventData> docsToUpsert,
                            ILogger log)
        {

            // Define data structure that will load events into database
            var dt = new DataTable();
            dt.Columns.Add("deviceid",typeof(string));
            dt.Columns.Add("timestamp",typeof(DateTime));
            dt.Columns.Add("json",typeof(string));

            var sqlDbLatency = new Stopwatch();
            // for each message read from IoTHub
            foreach (var message in docsToUpsert)
            {
                var text = Encoding.UTF8.GetString(message.Body.ToArray());
                // Create a new row
                var dr = dt.NewRow();

                dr["deviceid"]=message.SystemProperties["iothub-connection-device-id"];
                dr["timestamp"]=message.SystemProperties["iothub-enqueuedtime"];
                dr["json"]=text;

                dt.Rows.Add(dr);                  
            }    

            try
            {
                sqlDbLatency.Start();

                await using(var cnn = new SqlConnection(SqlDbConnectionString))
                {
                    cnn.Open();
                    var bc = new SqlBulkCopy(cnn);
                    bc.BatchSize=10000;
                    bc.DestinationTableName="telemetry";
                    await bc.WriteToServerAsync(dt);
                }

                sqlDbLatency.Stop();
            }
            catch (SqlException sqlEx)
            {
                log.LogInformation("Error processing message with err number {ErrorNr}. Exception was {SqlException}", sqlEx.Number, sqlEx.ToString());
            }
            catch(Exception e)
            {
                log.LogInformation("Error processing message. Exception was {Exception}", e);
            }

            return (long)sqlDbLatency
                .Elapsed
                .TotalMilliseconds;
        }
    }
}
