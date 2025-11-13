#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
 
// library code embedded directly
#define MAX_OBJECTS 3
 
typedef struct {
    int a, b, c, d, e, f;
} ObisCode;
 
typedef struct {
    ObisCode* code;
    float data;
} CosemObject;
 
CosemObject* objects = NULL;
 
void make_obis_code(ObisCode* code, int a, int b, int c, int d, int e, int f) {
    if (code == NULL) return;
    code->a = a;
    code->b = b;
    code->c = c;
    code->d = d;
    code->e = e;
    code->f = f;
}
 
void make_cosem_object(CosemObject* object, ObisCode* code, float data) {
    if (object == NULL || code == NULL) return;
    object->code = code;
    object->data = data;
}
 
int compare_obis_code(const ObisCode* a, const ObisCode* b) {
    if (a == NULL || b == NULL) return 0;
    return (a->a == b->a && a->b == b->b && a->c == b->c &&
            a->d == b->d && a->e == b->e && a->f == b->f);
}
 
void init_meter() {
    objects = (CosemObject*)malloc(sizeof(CosemObject) * MAX_OBJECTS);
    if (!objects) return;
 
    for (int i = 0; i < MAX_OBJECTS; i++) {
        ObisCode* code = (ObisCode*)malloc(sizeof(ObisCode));
        if (!code) return;
        make_obis_code(code, 1, 1, 1, 1, 1, i + 1);
        make_cosem_object(&objects[i], code, (float)(i + 1) * 10.5f);
    }
}
 
void get_cosem_object(CosemObject* result, ObisCode* code) {
    if (objects == NULL || result == NULL || code == NULL) return;
 
    for (int i = 0; i < MAX_OBJECTS; i++) {
        if (compare_obis_code(objects[i].code, code)) {
            result->code = objects[i].code;
            result->data = objects[i].data;
            return;
        }
    }
    result->code = NULL;
    result->data = 0.0f;
}
// End of library code
 
const char* ssid = "Esya-Training";
const char* password = "P@$$w0rd@123";
const char* mqtt_server = "172.16.103.22";
 
WiFiClient espClient;
PubSubClient client(espClient);
 
void setup_wifi() {
  delay(10);
  Serial.begin(115200);
 
  Serial.println("Connecting to WiFi: " + String(ssid));
 
  WiFi.begin(ssid, password);
 
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    attempts++;
    if (attempts > 20) {
      Serial.println("Failed to connect to WiFi");
      return;
    }
  }
 
  Serial.println("WiFi connected");
  Serial.println("IP address: " + WiFi.localIP().toString());
}
 
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received OBIS request: ");
 
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println(message);
 
  try {
    DynamicJsonDocument doc(256);
    DeserializationError error = deserializeJson(doc, message);
   
    if (error) {
      Serial.println("JSON parse failed: " + String(error.c_str()));
      return;
    }
   
    int a = doc["ObisCode"]["A"];
    int b = doc["ObisCode"]["B"];
    int c = doc["ObisCode"]["C"];
    int d = doc["ObisCode"]["D"];
    int e = doc["ObisCode"]["E"];
    int f = doc["ObisCode"]["F"];
   
    Serial.print("Parsed OBIS code: ");
    Serial.print(a); Serial.print(".");
    Serial.print(b); Serial.print(".");
    Serial.print(c); Serial.print(".");
    Serial.print(d); Serial.print(".");
    Serial.print(e); Serial.print(".");
    Serial.println(f);
   
    ObisCode requestedCode;
    make_obis_code(&requestedCode, a, b, c, d, e, f);
   
    CosemObject result;
    get_cosem_object(&result, &requestedCode);
   
    if (result.code != NULL) {
      Serial.println("OBIS code found in database");
     
      DynamicJsonDocument response(256);
      response["Code"]["A"] = result.code->a;
      response["Code"]["B"] = result.code->b;
      response["Code"]["C"] = result.code->c;
      response["Code"]["D"] = result.code->d;
      response["Code"]["E"] = result.code->e;
      response["Code"]["F"] = result.code->f;
      response["Value"] = result.data;
      response["DeviceId"] = "arduino_meter";
      response["Status"] = "success";
     
      String responseJson;
      serializeJson(response, responseJson);
     
      client.publish("meter/response", responseJson.c_str());
      Serial.print("Sent meter data: ");
      Serial.println(responseJson);
    } else {
      Serial.println("OBIS code not found in database");
    }
   
  } catch (...) {
    Serial.println("Error processing request");
  }
}
 
void reconnect() {
  int attempts = 0;
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
   
    if (client.connect("arduino_client")) {
      Serial.println("connected");
      client.subscribe("meter/request");
      Serial.println("Subscribed to: meter/request");
    } else {
      Serial.print("failed, retrying in 5 seconds");
      delay(5000);
      attempts++;
    }
  }
}
 
void setup() {
  setup_wifi();
 
  init_meter();
  Serial.println("Meter library initialized");
 
  // Serial.println("Available OBIS codes:");
  // for (int i = 0; i < MAX_OBJECTS; i++) {
  //   ObisCode* c = objects[i].code;
  //   Serial.print("  ");
  //   Serial.print(c->a); Serial.print(".");
  //   Serial.print(c->b); Serial.print(".");
  //   Serial.print(c->c); Serial.print(".");
  //   Serial.print(c->d); Serial.print(".");
  //   Serial.print(c->e); Serial.print(".");
  //   Serial.print(c->f);
  //   Serial.print(" -> ");
  //   Serial.println(objects[i].data);
  // }
 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
 
  Serial.println("System ready - Waiting for OBIS requests");
}
 
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
 