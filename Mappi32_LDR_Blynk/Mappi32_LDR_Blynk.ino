#define BLYNK_TEMPLATE_ID "TMPLzTWwsdWE"
#define BLYNK_DEVICE_NAME "HCSR04"
#define BLYNK_AUTH_TOKEN "x8acfK5xIdov8xxAACngAkueB2JQ4n9Y"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h> //pembacaan mappi ke sensor

#define triggerPin  26// set Trigger ke pin 26/IO26
#define echoPin     25 // set Echo ke pin 25/IO25
#define LED 23
int sensorPin = 32 ; //membaca input sensor LDR
int ldr = 0 ; //variabel penyimpanan nilai sensor


long duration;
int jarak;
int kondisi = 0 ;

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "Rebstud";   // Nama Hotspot / WiFi
char pass[] = "tokoyangbaik"; // Password WiFi

BlynkTimer timer;


void myTimerEvent()
{
  Blynk.virtualWrite(V5, millis() / 1000);
}

void setup () {

  Wire.begin();
  Serial.begin(112500);
  pinMode(triggerPin, OUTPUT); // set pin trigger sebagai output
  pinMode(echoPin, INPUT); // set pin echo sebagai input
  pinMode(LED , OUTPUT); //output untuk lampu

  Blynk.begin(auth, ssid, pass);
  Wire.begin();


}
void loop () {
  Blynk.run();
  timer.run();
  digitalWrite(triggerPin, LOW);   //Set triggerPin low
  delayMicroseconds(2);       // delay 2 micro second
  digitalWrite(triggerPin, HIGH);  // Set triggerPin high
  delayMicroseconds(10);      // triggerPin high selama 10 micro seconds
  digitalWrite(triggerPin, LOW);   // triggerPinPin low

  duration = pulseIn(echoPin, HIGH);   //Pembacaan Echo pin dan durasi
  jarak = duration * 0.034 / 2;   /// Menghitung jarak yg sebenarnya

  ldr = analogRead(sensorPin); //data nilai sensor akan dibaca dari data analog (A0)

  Serial.print("nilai ldr :");
  Serial.println(ldr);


  if (jarak != 0) {
    Serial.print("jarak :");
    Serial.print(jarak);
    Serial.println(" cm");

  }

  Blynk.virtualWrite(V0, jarak);
  Blynk.virtualWrite(V1, ldr);

  if (kondisi == 0 && ldr >= 1000) {
    digitalWrite(LED, HIGH);
    kondisi = 1;
  }
  else if (kondisi == 1 && ldr >= 1000) {
    digitalWrite(LED, HIGH);
    kondisi = 1;
  }
  else if (kondisi == 2 && ldr >= 1000) {
    digitalWrite(LED, HIGH);
    kondisi = 1;
  }

  if (kondisi == 0 && ldr < 1000) {
    digitalWrite(LED, LOW);
    kondisi = 2;
  }
  else if (kondisi == 1 && ldr < 1000) {
    digitalWrite(LED, LOW);
    kondisi = 2;
  }
  else if (kondisi == 2 && ldr < 1000) {
    digitalWrite(LED, LOW);
    kondisi = 2;
  }

  if (kondisi == 1 && jarak <= 3 ) {
    digitalWrite(LED, LOW);
    //    kondisi = 2;
  }

  else if (kondisi == 2 && jarak <= 3) {
    digitalWrite(LED, HIGH);
    //    kondisi = 4;
  }

  kondisi = 0;
  delay (500);

}
