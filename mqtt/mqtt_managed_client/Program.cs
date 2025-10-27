using MQTTnet;

using MQTTnet.Client;

using MQTTnet.Extensions.ManagedClient;

using System.Text;

using System.Threading.Tasks;

namespace ConsoleApp2

{

    internal class Program

    {

        private static readonly string logFilePath = "mqtt_client.log";

        static async Task Main(string[] args)

        {

            // Log application start

            LogEvent("APPLICATION_START", "MQTT Client application starting");

            var factory = new MqttFactory();

            var client = factory.CreateManagedMqttClient();

            // Create client options with will message configuration

            var client_option = new MqttClientOptionsBuilder()

                .WithClientId("Myhome")

                .WithTcpServer("Localhost", 1883)

                // Configure will message - this will be published by broker if client disconnects unexpectedly

                .WithWillTopic("test/status") // Topic where will message will be published

                .WithWillPayload("Client disconnected unexpectedly") // Will message content

                .WithWillQualityOfServiceLevel(MQTTnet.Protocol.MqttQualityOfServiceLevel.AtLeastOnce)

                .WithWillRetain(true)

                .Build();

            var managed_options = new ManagedMqttClientOptionsBuilder()

                .WithClientOptions(client_option)

                .Build();

            // Connection event handlers for better monitoring

            client.ConnectedAsync += async e =>

            {

                LogEvent("CONNECTION_ESTABLISHED", $"Connected to MQTT broker at Localhost:1883");

                // Publish connected status when successfully connected

                await PublishStatusMessage(client, "Client connected");

            };

            client.DisconnectedAsync += async e =>

            {

                LogEvent("CONNECTION_LOST", $"Disconnected from MQTT broker. Reason: {e.Reason}");

            };

            // Log connection attempt

            LogEvent("CONNECTION_ATTEMPT", "Attempting to connect to MQTT broker");

            try

            {

                await client.StartAsync(managed_options);

                LogEvent("CLIENT_STARTED", "Managed MQTT client started successfully");

            }

            catch (Exception ex)

            {

                LogEvent("CONNECTION_FAILED", $"Failed to connect to broker: {ex.Message}");

                return;

            }

            // Publish initial connected status

            await PublishStatusMessage(client, "Client connected and running");

            int messageCount = 0;

            while (true)

            {

                messageCount++;

                string payload = GenerateRandomTemperature();

                var message = new MqttApplicationMessageBuilder()

                    .WithPayload(payload)

                    .WithTopic("test/temperature")

                    .WithQualityOfServiceLevel(MQTTnet.Protocol.MqttQualityOfServiceLevel.AtMostOnce)

                    .WithRetainFlag(true)

                    .Build();

                try

                {

                    await client.EnqueueAsync(message);

                    LogEvent("MESSAGE_PUBLISHED", $"Message #{messageCount} published to test/temperature: {payload}");

                }

                catch (Exception ex)

                {

                    LogEvent("MESSAGE_PUBLISH_FAILED", $"Failed to publish message #{messageCount}: {ex.Message}");

                }

                await Task.Delay(3000);

            }

        }

        // Method to publish status messages

        static async Task PublishStatusMessage(IManagedMqttClient client, string status)

        {

            var statusMessage = new MqttApplicationMessageBuilder()

                .WithTopic("test/status")

                .WithPayload(status)

                .WithQualityOfServiceLevel(MQTTnet.Protocol.MqttQualityOfServiceLevel.AtLeastOnce)

                .WithRetainFlag(true) // Retain so new subscribers can see the latest status

                .Build();

            try

            {

                await client.EnqueueAsync(statusMessage);

                LogEvent("STATUS_UPDATE", $"Status message published: {status}");

            }

            catch (Exception ex)

            {

                LogEvent("STATUS_PUBLISH_FAILED", $"Failed to publish status: {ex.Message}");

            }

        }

        static string GenerateRandomTemperature()

        {

            Random random = new Random();

            double temperature = random.NextDouble() * 10 + 30; // Random value between 30.0 and 40.0

            return $"{temperature:F1} Degree";

        }

        // Logging method that writes one line per event

        static void LogEvent(string eventType, string message)

        {

            string logEntry = $"{DateTime.Now:yyyy-MM-dd HH:mm:ss.fff} | {eventType} | {message}";

            // Write to console

            Console.WriteLine(logEntry);

            // Write to log file

            try

            {

                using (StreamWriter sw = new StreamWriter(logFilePath, true))

                {

                    sw.WriteLine(logEntry);

                }

            }

            catch (Exception ex)

            {

                Console.WriteLine($"{DateTime.Now:yyyy-MM-dd HH:mm:ss.fff} | LOGGING_ERROR | Failed to write to log file: {ex.Message}");

            }

        }

    }

}
