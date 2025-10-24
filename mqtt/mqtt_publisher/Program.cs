using MQTTnet;

using MQTTnet.Client;

using MQTTnet.Extensions.ManagedClient;

using System.Text;

using System.Threading.Tasks;

namespace mqtt

{

    internal class Program

    {

        static async Task Main(string[] args)

        {

            var factory = new MqttFactory();

            var client = factory.CreateManagedMqttClient();

            var client_option = new MqttClientOptionsBuilder()

            .WithTcpServer("Localhost", 1883)
            .WithCleanSession(false)

            .Build();

            var managed_options = new ManagedMqttClientOptionsBuilder()

            .WithClientOptions(client_option)
            

            .Build();

            client.StartAsync(managed_options);
            client.ConnectedAsync += async e =>
            {
                Console.WriteLine("Connection done");
            };
            client.DisconnectedAsync += async e =>
            {
                Console.WriteLine("Client Disconnected");
            };


          //  await client.StartAsync(managed_options);

            while (true)

            {

                string payload = GenerateRandomTemperature();

                var message = new MqttApplicationMessageBuilder()

                    .WithPayload(payload)

                    .WithTopic("test/temperature")

                    .WithQualityOfServiceLevel(MQTTnet.Protocol.MqttQualityOfServiceLevel.AtLeastOnce)

                    .WithRetainFlag(true)

                    .Build();

                await client.EnqueueAsync(message);

                await Task.Delay(5000);
                //await client.StopAsync();

            }

        }

        static string GenerateRandomTemperature()

        {

            Random random = new Random();

            double temperature = random.NextDouble() * 10 + 30; // Random value between 30.0 and 40.0

            return $"{temperature:F1} Degree";

        }

    }

}
