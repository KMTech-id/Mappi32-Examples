//Library dan Deklarasi Variabel
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPI.h>
#include <LoRa.h>

#define ss 15
#define rst 0
#define dio0 27

const char* ssid     = "Rebstud";
const char* password = "12345678";

String latString;
String lonString;

String receivedMessage = "";

//Ubidots
String deviceLabel = "gpstracking"; // nama device label kalian diubidots

WiFiClient client;

int status = WL_IDLE_STATUS;
char server[] = "http://things.iot.ubidots.com";
const int port = 80;
String data;
String token = "BBFF-rdSL7X5sK1N6CUtFNt8Q6RTyYgNE8K"; // di isi menggunakan Token Ubidoots Kalian

void setup() {
  Serial.begin(115200);
  while (!Serial);

  SPI.begin(14, 12, 13, 15);
  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(915E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  status = WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }
}

void loop() {
  if (LoRa.parsePacket()) {
// Menerima pesan dari tranceiver
    receivedMessage = "";
    while (LoRa.available()) {
      receivedMessage += (char)LoRa.read();
    }

    Serial.println("Pesan diterima: " + receivedMessage);
    int charasterisk = receivedMessage.indexOf('*');
    int charcomma = receivedMessage.indexOf(',');
    int charhash = receivedMessage.indexOf('#');

    //        Serial.println("posisi bintang: " + String(charasterisk));
    //        Serial.println("posisi koma: " + String(charcomma));

    latString = receivedMessage.substring(charasterisk + 1, charcomma);
    Serial.println("latitude: " + latString);
    lonString = receivedMessage.substring(charcomma + 1, charhash);
    Serial.println("longitude: " + lonString);
    sendData();
  }
}

// Mengirim Data ke Ubidots
void sendData() {

  // Pengecekan Koneksi
  if (!client.connect(server, port))
  {
    Serial.println("No connection"); 
    while (!client.connect(server, port))
    {
      Serial.print(".");
      delay(1000);
    }
    return;
  }
  else if (client.connect(server, port))
  {
    Serial.println("Connected");
    Serial.println();

    // Jika Koneksi Berhasil program mengirimkan Permintaan HTTP POST ke server
    client.print("POST /api/v1.6/devices/" + deviceLabel + " HTTP/1.1\r\n");
    client.print("X-Auth-Token: ");
    client.println(token);
    client.print("Content-Type: application/json\r\n");
    client.print("Host: things.ubidots.com\r\n");

    // Data dikirim dalam bentuk JSON Agar bisa diakses pada ubidots 
    String buff = "";
    buff += "{";
    buff += "\"position\": {";
    buff += "\"value\": 1, ";
    buff += "\"context\": {";
    buff += "\"lat\": \"" + latString + "\",";
    buff += "\"lng\": \""    + lonString +   "\"";
    buff += "}}}\r\n";

    int dataLength = buff.length() - 1;
    String dataLengthStr = String(dataLength);

     client.print("Content-Length: ");
    client.println(dataLengthStr);
    client.println();
    client.println(buff);

    //Setelah Permintaan terkirim  prorgam akan mengirim respon 
    Serial.print("POST /api/v1.6/devices/" + deviceLabel + " HTTP/1.1\r\n");
    Serial.print("X-Auth-Token: ");
    Serial.println(token);
    Serial.print("Content-Type: application/json\r\n");
    Serial.println("Host: things.ubidots.com\r\n");
    Serial.print("Content-Length: ");
    Serial.println(dataLengthStr);
    Serial.println();
    Serial.println(buff);

    while (!client.available());
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }

    client.stop();
    Serial.println("closing connection");
    Serial.println();
  }
}
