#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

//inisialisasi LCD dengan alamat I2C 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

//konfigurasi HC-SR04
const int trigPin = 17; // Pin Trig
const int echoPin = 16; // Pin echo
const int naxDistance = 400; //Maximum jarak
NewPing sonar(trigPin, echoPin, maxDistance);

//variable untuk kalibrasi
const float speedOfSound = 0.0200; 
const float offset = -2.0; //koreksi offset untuk akurasi (disesuaikan untuk +1cm)
const float numRedings = 10; //jumlah pembacaan untuk rata rata


void setup(){
  //inisialisasi serial monitor
  Serial.begin(9600);

  //inisialisasi LCD
  lcd.init();
  lcd.backlight();
  lcd.print("Smart Ruler");
  delay(2000);
  lcd.clear(); 
}

void loop(){
  float totalDistance = 0;

  //ambil rata rata dari beberapa pembacaa
  for (int i = 0; i < numReadings; i++){
    float distance = sonar.ping_cm();
    if(distance > 0){
      totalDistance += distance;
    }
    delay(50);
  }

  float averageDistance = totalDistance / numReadings + offset + 1.0;

  Serial.print("Jarak");
  Serial.print(averageDistance);
  Serial.print(" cm");

  lcd.setCursor(0, 0);
  lcd.print("jarak: ");
  lcd.print(averageDistance);
  lcd.print(" c ");

  delay(500);
}