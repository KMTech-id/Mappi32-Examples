#include <math.h>
#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <PubSubClient.h> //include mqtt ubidots

String LoRaData;
String dt[10];
int i;
String temperature;
String humidity;
boolean parsing = false;
String rssiStr;

/*silakan pilih board yang digunakan*/
#define mappi32
//#define esp32

//define the pins used by the transceiver module
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


#define WIFISSID "KMTek" // Nama Hotspot / WiFi
#define PASSWORD "KMTek123" // Password WiFi
#define TOKEN "BBFF-r9tentbfpWpZvhdVi6iirhmMDVCBv3"   // Ubidots TOKEN
#define MQTT_CLIENT_NAME "monitoring-gudang-beras" //mqtt client name / Device name/ custom
#define DEVICE_LABEL "monitoring-gudang-beras" // Api Label / Device Label in Ubidots

#define VARIABLE_LABEL_1 "Temperature" // Nama Variabel 1
#define VARIABLE_LABEL_2 "Humidity" // Nama Variabel 2

char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[200];
char topic[150];

char str_val_1[10];
char str_val_2[10];

WiFiClient ubidots;
PubSubClient client(ubidots);

void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
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
  pinMode(5, OUTPUT);

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

  while (!Serial);
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
    // read packet
    while (LoRa.available()) {
      LoRaData = LoRa.readString();

      parsingData();

      humidity = dt[0];
      temperature = dt[1];


      digitalWrite(5, HIGH);
      delay(100);
      digitalWrite(5, LOW);
      delay(100);
      digitalWrite(5, HIGH);
      delay(300);
      digitalWrite(5, LOW);
      delay(100);
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
  temperature.toCharArray(str_val_1, temperature.length() + 1);
  humidity.toCharArray(str_val_2, humidity.length() + 1);

  Serial.println("Temperature : " + temperature);
  Serial.println("Humidity : " + humidity);

  //    char temperaturefl = temperature.toChar();
  //    char humidityfl = humidity.toChar();

  //  dtostrf(suhufl, 4, 2, str_val_1);
  //  dtostrf(rssifl, 4, 2, str_val_2);

  //  dtostrf(humidity, 4, 2, str_val_1);
  //  dtostrf(temperature, 4, 2, str_val_2);

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

void parsingData()
{
  int j = 0;
  //kirim data yang telah diterima sebelumnya
  Serial.print("data masuk : ");
  Serial.print(LoRaData);
  Serial.print("\n");
  //inisialisasi variabel, (reset isi variabel)
  dt[j] = "";
  //proses parsing data
  for (i = 0; i < LoRaData.length(); i++)
  {
    //pengecekan tiap karakter dengan karakter (#) dan (,)
    if (LoRaData[i] == ',')
    {
      //increment variabel j, digunakan untuk merubah index array penampung
      j++;
      dt[j] = ""; //inisialisasi variabel array dt[j]
    }
    else
    {
      //proses tampung data saat pengecekan karakter selesai.
      dt[j] = dt[j] + LoRaData[i];
    }
  }
}
