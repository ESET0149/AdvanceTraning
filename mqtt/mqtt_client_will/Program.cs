using MQTTnet;
using MQTTnet.Client;
using MQTTnet.Extensions.ManagedClient;
using System;
using System.Threading.Tasks;

namespace mqtt
{
    internal class Program
    {
        static async Task Main(string[] args)
        {
            var factory = new MqttFactory();
            var client = factory.CreateManagedMqttClient();

            var clientOptions = new MqttClientOptionsBuilder()
                .WithClientId("TempSensor1")
                .WithTcpServer("localhost", 1883)
                .WithCleanSession(true)
                .WithWillTopic("test/temperature")
                .WithWillPayload("Sensor Offline")
                .Build();

            var managedOptions = new ManagedMqttClientOptionsBuilder()
                .WithClientOptions(clientOptions)
                .Build();

            try
            {
                await client.StartAsync(managedOptions);
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }

            Console.WriteLine("Connected and running...");

            // Simulate temperature publishing
            var rand = new Random();
            while (true)
            {
                double temperature = rand.NextDouble() * 10 + 30;
                string payload = $"{temperature:F1} °C";

                var message = new MqttApplicationMessageBuilder()
                    .WithTopic("test/temperature")
                    .WithPayload(payload)
                    .WithQualityOfServiceLevel(MQTTnet.Protocol.MqttQualityOfServiceLevel.AtLeastOnce)
                    .WithRetainFlag(true)
                    .Build();

                await client.EnqueueAsync(message);

                Console.WriteLine($" Sent: {payload}");
                await Task.Delay(5000);
            }
        }
    } 
}
