 
#include <WiFi.h>
#include <PubSubClient.h>
 
 
const char* ssid = "Esya-Training";
const char* password = "P@$$w0rd@123";
const char* mqtt_server = "89.233.104.140";
const int mqtt_port = 1883;
 
 
 
WiFiClient espClient;
PubSubClient client(espClient);
 
 
void setup_wifi(){
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
   Serial.println("\nWiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
}
 
void reconnect(){
while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect (clientID can be anything)
    if (client.connect("ESP32_Client")) {
      Serial.println("connected");
      // Optional: publish a hello message once connected
      client.publish("esp32/status", "ESP32 connected VINISH");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 2 seconds...");
      delay(2000);
    }
  }
}
 
void setup(){
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}
 
void loop(){
  if(!client.connected()){
    reconnect();
  }
  client.loop();
  int value = random(0, 100);
  char msg[50];
  sprintf(msg, "from esp: %d", value);
  client.publish("esp32/data", msg);
   Serial.print("Published: ");
  Serial.println(msg);
  delay(2000);
}
 