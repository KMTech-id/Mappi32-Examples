// Bluetooth LED Control with Mappi32 //
// @yugoyokaii . Video KMTek Indonesia //

#include "BluetoothSerial.h"

BluetoothSerial SerialBT;  // Membuat instance dari kelas BluetoothSerial

int relay1 = 17;  // Menentukan pin untuk relay1

void setup() {
  SerialBT.begin("Something");  // Menginisialisasi Bluetooth dengan nama "Something"
  Serial.begin(115200);  // Menginisialisasi komunikasi serial dengan baud rate 115200

  pinMode(relay1, OUTPUT);  // Mengatur pin relay1 sebagai output
  digitalWrite(relay1, LOW);  // Awalnya, set relay1 ke kondisi LOW (mati)
}

void loop() {
  if (SerialBT.available() > 0) {  // Memeriksa apakah ada data yang masuk dari Bluetooth
    char command = SerialBT.read();  // Membaca perintah yang masuk
    if (command == '1') {
      digitalWrite(relay1, LOW);    // menyalakan relay1
    } else if (command == '2') {
      digitalWrite(relay1, HIGH);  // mematikan relay1
    }
  }
}
