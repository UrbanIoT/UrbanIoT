using Microsoft.ApplicationInsights;
using Microsoft.ApplicationInsights.DataContracts;
using Microsoft.Azure.WebJobs;

namespace UrbanIoT.Functions;

public class CustomTelemetry
{
    private readonly TelemetryClient _telemetryClient;

    public CustomTelemetry(TelemetryClient telemetryClient)
    {
        _telemetryClient = telemetryClient;
    }

    // This correllates all telemetry with the current Function invocation
    private static void UpdateTelemetryContext(TelemetryContext context, ExecutionContext functionContext)
    {
        context.Operation.Id = functionContext.InvocationId.ToString();
        context.Operation.ParentId = functionContext.InvocationId.ToString();
        context.Operation.Name = functionContext.FunctionName;
    }

    public void TrackMetric(ExecutionContext context, string metricName, double metricValue)
    {
        var metric2 = new MetricTelemetry(metricName, metricValue);
        UpdateTelemetryContext(metric2.Context, context);
        _telemetryClient.TrackMetric(metric2);
    }
}