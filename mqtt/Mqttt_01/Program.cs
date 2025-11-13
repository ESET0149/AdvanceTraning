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

            var mqttClient = factory.CreateManagedMqttClient();

            var client_option = new MqttClientOptionsBuilder()

            .WithTcpServer("Localhost", 1883)
            .WithCleanSession(false)

            .Build();

            var managed_options = new ManagedMqttClientOptionsBuilder()

            .WithClientOptions(client_option)


            .Build();

            mqttClient.ApplicationMessageReceivedAsync += async e =>
            {
                Console.WriteLine($"Received: {e.ApplicationMessage.Topic} - {e.ApplicationMessage.ConvertPayloadToString()}");
            };


            await mqttClient.StartAsync(managed_options);
            await mqttClient.SubscribeAsync(new[]
            {
                new MqttTopicFilterBuilder()
                    .WithTopic("test/temperature")
                    .WithQualityOfServiceLevel(MQTTnet.Protocol.MqttQualityOfServiceLevel.AtMostOnce)
                    .Build(),

            });
            while (true);
        }
    }
}
                
                

