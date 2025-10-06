//receiver

#include <WiFi.h>
//include mqtt ubidots
#include <PubSubClient.h>

#define WIFISSID "SSID" // Nama Hotspot / WiFi
#define PASSWORD "Password" // Password WiFi
#define TOKEN "XXXX-xxxxxxxxxxxxxxxxxxxxxxxxxxxxx"   // Ubidots TOKEN
#define MQTT_CLIENT_NAME "xyz" //mqtt client name / Device name/ custom
#define DEVICE_LABEL "xyz" // Api Label / Device Label in Ubidots

#define VARIABLE_LABEL_1 "data" // Nama Variabel 1
#define VARIABLE_LABEL_2 "rssi" // Nama Variabel 2

//define receiver sensor

String dt;
String rssiStr;

char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[200];
char topic[150];

char str_val_1[10];
char str_val_2[10];

//include LORA
#include <SPI.h>
#include <LoRa.h>

/*silakan pilih board yang digunakan*/
//#define mappi32
#define esp32

#ifdef esp32
#define ss 5
#define rst 14
#define dio0 2
#endif

#ifdef mappi32
#define ss 15
#define rst 0
#define dio0 27
#endif

WiFiClient ubidots;
PubSubClient client(ubidots);

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
  Serial.begin(115200);
  Serial.println("Memulai telegram bot. koneksi ke wifi");
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

  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
#ifdef mappi32
  SPI.begin(14, 12, 13, 15);
#endif
  LoRa.setPins(ss, rst, dio0);

  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(921E6))//melakukan perulangan sampai Lora terkoneksi
  {
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
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    //read packet
    while (LoRa.available())
    {
      String LoRaData = LoRa.readString();
      dt = String(LoRaData);
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
  //  dt.toCharArray(str_val_2, dt.length() + 1);
  //  rssiStr.toCharArray(str_val_1, rssiStr.length() + 1);

  float dtfl = dt.toFloat();
  float rssifl = rssiStr.toFloat();

  dtostrf(dtfl, 4, 2, str_val_1);
  dtostrf(rssifl, 4, 2, str_val_2);

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
