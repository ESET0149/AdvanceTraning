using MQTTnet;

using MQTTnet.Client;

using System.Text.Json;
 
public class ObisCode

{

    public int A { get; set; }

    public int B { get; set; }

    public int C { get; set; }

    public int D { get; set; }

    public int E { get; set; }

    public int F { get; set; }

}
 
class Program

{

    static async Task Main()

    {

        var factory = new MqttFactory();

        var client = factory.CreateMqttClient();
 
        var options = new MqttClientOptionsBuilder()

            .WithTcpServer("172.16.103.22", 1883)

            .WithClientId("csharp-client")

            .Build();
 
        await client.ConnectAsync(options);

        Console.WriteLine("Connected to MQTT broker");
 
        client.ApplicationMessageReceivedAsync += e =>

        {

            var message = System.Text.Encoding.UTF8.GetString(e.ApplicationMessage.PayloadSegment);

            return Task.CompletedTask;

        };
 
        await client.SubscribeAsync("meter/response");
 
        var obisCodes = new[] {

            new ObisCode { A = 1, B = 1, C = 1, D = 1, E = 1, F=1 },

            new ObisCode { A = 1, B = 1, C = 1, D = 1, E = 1, F = 2 },

            new ObisCode { A = 1, B = 1, C = 1, D = 1, E = 1, F = 3 }

        };
 
        Console.WriteLine("Sending OBIS code requests:");
 
        foreach (var obis in obisCodes)

        {

            var request = new { ObisCode = obis };

            var json = JsonSerializer.Serialize(request);
 
            var message = new MqttApplicationMessageBuilder()

                .WithTopic("meter/request")

                .WithPayload(json)

                .Build();
 
            await client.PublishAsync(message);

            Console.WriteLine("Sent: " + obis.A + "." + obis.B + "." + obis.C + "." + obis.D + "." + obis.E + "." + obis.F);
 
            await Task.Delay(3000);

        }
 
        Console.WriteLine("Press any key to exit...");

        Console.ReadKey();

    }

}
 