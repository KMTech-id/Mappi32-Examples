#include <PubSubClient.h>
#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
//define the pins used by the transceiver module
#define ss 15
#define rst 0
#define dio0 27

String tempStr;
String rssiStr;

WiFiClient ubidots;
PubSubClient client(ubidots);

#define WIFISSID "SSID" // Nama Hotspot / WiFi
#define PASSWORD "PASSWORD" // Password WiFi
#define TOKEN "XXXX-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"   // Ubidots TOKEN
#define MQTT_CLIENT_NAME "xyz" //mqtt client name / Device name/ custom
#define DEVICE_LABEL "XYZ" // Api Label / Device Label in Ubidots

#define VARIABLE_LABEL_1 "rssi" // Nama Variabel 1
#define VARIABLE_LABEL_2 "temperature" // Nama Variabel 2

char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[200];
char topic[150];

char str_val_1[10];
char str_val_2[10];



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
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
      Serial.println("Connected To Ubidots");
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
  //initialize Serial Monitor
  Serial.begin(115200);
  WiFi.begin(WIFISSID, PASSWORD);

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

  pinMode(5, OUTPUT);
  while (!Serial);
  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  SPI.begin(14, 12, 13, 15);
  LoRa.setPins(ss, rst, dio0);

  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(921E6)) {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
    while (LoRa.available()) {
      String tempStrs = LoRa.readString();
      tempStr = tempStrs.toInt();
      Serial.print(tempStr);
      Serial.print("°C");
      Serial.print("\n");
      digitalWrite(5, HIGH);
      delay(100);
      digitalWrite(5, LOW);
      delay(100);
      digitalWrite(5, HIGH);
      delay(100);
      digitalWrite(5, LOW);
      sendUbidots();
    }
  }
}

void sendUbidots() {
  if (!client.connected()) {
    reconnect();
  }

  //    read rssi / signal
  Serial.print("RSSI: ");
  rssiStr = LoRa.packetRssi();
  Serial.print(rssiStr);
  Serial.print(" dBm"); Serial.print("\n");

  //Send to Ubidots
  tempStr.toCharArray(str_val_2, tempStr.length() + 1);
  rssiStr.toCharArray(str_val_1, rssiStr.length() + 1);
  
  sprintf(topic, "%s", ""); // Cleans the topic content
  sprintf(topic, "%s%s", "/v1.6/devices/", DEVICE_LABEL);

  sprintf(payload, "%s", ""); // Cleans the payload content
  sprintf(payload, "{\"%s\":", VARIABLE_LABEL_1); // Adds the variable label
  sprintf(payload, "%s {\"value\": %s}", payload, str_val_1); // Adds the value
  sprintf(payload, "%s, \"%s\":", payload, VARIABLE_LABEL_2); // Adds the variable label
  sprintf(payload, "%s {\"value\": %s}", payload, str_val_2); // Adds the value
  sprintf(payload, "%s}", payload); // Closes the dictionary brackets

  Serial.println("Data terkirim ke Ubidots\n------------------------------\n");
  client.publish(topic, payload);
  client.loop();
  delay(1000);

}
