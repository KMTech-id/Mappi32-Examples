//include telegram ct bot
#include <WiFi.h>
#include "CTBot.h"

CTBot myBot;
TBMessage msg ;
#define TOKEN_TELEGRAM "XXXXXXXXXX:xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" //token from telegram (botFather)
const int id = xxxxxxxxxx; // Id telegram (idBot)


//define sensor
#define BATAS_MAX 22 //maximum speed to send warning to telegram (km/h)
String isipesan = "Wind speed"; //message from telegram for view the wind speed
String Status;
boolean Start;
int windspeed;
String windspeedStr;
String rssiStr;

//include mqtt ubidots
#include <PubSubClient.h>

#define WIFISSID "SSID" // Nama Hotspot / WiFi
#define PASSWORD "PASSWORD" // Password WiFi
#define TOKEN "XXXX-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"   // Ubidots TOKEN
#define MQTT_CLIENT_NAME "anemometer" //mqtt client name

#define VARIABLE_LABEL_1 "rssi" // Nama Variabel 1
#define VARIABLE_LABEL_2 "windspeed" // Nama Variabel 2
#define DEVICE_LABEL "anemometer" // Device Label

char mqttBroker[]  = "industrial.api.ubidots.com";
char payload[200];
char topic[150];

char str_val_1[10];
char str_val_2[10];

//include LORA
#include <math.h>
#include <SPI.h>
#include <LoRa.h>

//define the pins used by the transceiver module
#define ss 15
#define rst 0
#define dio0 27

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

  myBot.wifiConnect(WIFISSID, PASSWORD);
  myBot.setTelegramToken(TOKEN_TELEGRAM);

  Serial.println("LoRa Receiver");

  //setup LoRa transceiver module
  SPI.begin(14, 12, 13, 15);
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
      windspeed = LoRaData.toInt();
      windspeedStr = String(windspeed);

      //test connection telegram
      if (myBot.testConnection())
      {
        Serial.println("------------------------------\nTest koneksi Telegram berhasil\n");
        if (windspeed > BATAS_MAX )
        {
          Status = "Bahaya! Angin Kencang\r\n";
          Serial.print("Windspeed = " + windspeedStr + "\tStatus = " + Status);
          sendUbidots();
        }
        if (windspeed <= BATAS_MAX )
        {
          Status = "Aman\r\n";
          Serial.print("Windspeed = " + windspeedStr + "\tStatus = " + Status);
          sendUbidots();
          Start = 0;
        }

        //message
        if (myBot.getNewMessage(msg))
        {
          Serial.println("Masuk : " + msg.text);
          String Pesan = msg.text;

          if (Pesan == isipesan && Start == 0)
          {
            myBot.sendMessage(id, Status + windspeedStr + " km/h");
            Serial.println("Terkirim ke telegram");
            sendUbidots();
          }
          else if ( Pesan == isipesan && windspeed > BATAS_MAX )
          {
            myBot.sendMessage(id, Status + windspeedStr + " km/h");
            Serial.println("Terkirim ke telegram");
            sendUbidots();
          }
        }

        if (windspeed > BATAS_MAX  && Start == 0)
        {
          myBot.sendMessage(id, Status + windspeedStr + " km/h");
          Serial.print("Peringatan Terkirim ke telegram\n");
          sendUbidots();
          Start = 1;
        }
      }

      else
      {
        Serial.println("xxxxxxxxxxxxxxxxxxxxxxxxxxx\nTest koneksi Telegram gagal :(\nxxxxxxxxxxxxxxxxxxxxxxxxxxx");
      }
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
  windspeedStr.toCharArray(str_val_2, windspeedStr.length() + 1);
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
