 
#include <WiFi.h>
#include <HTTPClient.h>
const char* ssid = "Esya-Training";         // Your WiFi network SSID
const char* password = "P@$$w0rd@123"; // Your WiFi network password
const char* api_url = "http://jsonplaceholder.typicode.com/posts/1"; // Example API endpoint
void setup() {
  Serial.begin(115200);
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(api_url);
    int httpResponseCode = http.GET(); // Send the GET request
    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString(); // Get the response payload
      Serial.println(payload);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end(); // Close the connection
  } else {
    Serial.println("WiFi Disconnected");
  }
  delay(5000); // Wait 5 seconds before making the next request
}
 