#include <WiFi.h>
#include <PubSubClient.h>

#define WIFISSID "SSID"                           // Nama Hotspot / WiFi
#define PASSWORD "SSIDPASSWORD"                       // Password WiFi
#define TOKEN "XXXX-xxxxxxxxxxxxxxxxxxxxxxxx"   // Ubidots TOKEN
#define MQTT_CLIENT_NAME "sensor-uv"

#define VARIABLE_LABEL_1 "Uva" // Nama Variabel 1
#define VARIABLE_LABEL_2 "Uvb" // Nama Variabel 2
#define DEVICE_LABEL "sensor-uv" // Device Label

#include <SparkFun_VEML6075_Arduino_Library.h>
VEML6075 uv;


char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[700];
char topic[150];

char str_val_1[6];
char str_val_2[6];

WiFiClient ubidots;
PubSubClient client(ubidots);

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
  Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");

    // Attemp to connect
    if (client.connect(MQTT_CLIENT_NAME, TOKEN, "")) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 2 seconds");
      // Wait 2 seconds before retrying
      delay(2000);
    }
  }
}



void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFISSID, PASSWORD);
  
  // Sensor
  Wire.begin();

  if (!uv.begin())
  {
    Serial.println("Unable to communicate with VEML6075.");
    while (1)
      ;
  }

  //connecting to wifi
  Serial.println();
  Serial.print("Wait for WiFi...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  client.setServer(mqttBroker, 1883);
  client.setCallback(callback);
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  
  float Uva = (uv.a());
  float Uvb = (uv.b());
  Serial.println(String(uv.a()));
  Serial.println(String(uv.b()));

  dtostrf(Uva, 4, 2, str_val_1);
  dtostrf(Uvb, 4, 2, str_val_2);

  sprintf(topic, "%s", "");
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);

  sprintf(payload, "%s", ""); // Cleans the payload content
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL_1);  
  sprintf(payload, "%s {\"value\": %s}", payload, str_val_1);
  sprintf(payload, "%s, \"%s\":", payload, VARIABLE_LABEL_2);  
  sprintf(payload, "%s {\"value\": %s}", payload, str_val_2);
  sprintf(payload, "%s}", payload);
  Serial.println("Publishing data to Ubidots Cloud");
  client.publish(topic, payload);
  client.loop();
  delay(1000);
}
