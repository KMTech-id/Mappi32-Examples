#define BLYNK_TEMPLATE_ID "*"
#define BLYNK_TEMPLATE_NAME "*"
#define BLYNK_AUTH_TOKEN "*"
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "*";
char pass[] = "*";

const int sensor_pin = 33; // Sensor Pin
const int relay_pin = 13;   // Relay Pin

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display
BlynkTimer timer;

void sendSensorDataToBlynk() {
  int sensor_data = analogRead(sensor_pin);
  Blynk.virtualWrite(V1, sensor_data); // Send sensor data to Blynk app
}

void setup() {
  Serial.begin(9600);
  pinMode(sensor_pin, INPUT);
  pinMode(relay_pin, OUTPUT);
  Blynk.begin(auth, ssid, pass);
  lcd.init();
  lcd.backlight(); // Turn on the backlight

  timer.setInterval(1000L, sendSensorDataToBlynk); // Send data to Blynk every second
}

void loop() {
  Blynk.run();
  timer.run();

  int sensor_data = analogRead(sensor_pin);
  Serial.print("Sensor_data: ");
  Serial.print(sensor_data);
  Serial.print("\t | ");

  lcd.setCursor(0, 0); // Set cursor to the first row
  lcd.print("Kelembaban: ");
  lcd.print(sensor_data);

  lcd.setCursor(0, 1); // Set cursor to the second row
  if (sensor_data > 3000) {
    Serial.println("No moisture, Soil is dry");
    lcd.print("Tanah Kering    ");
    digitalWrite(relay_pin, LOW);
  } else if (sensor_data >= 2300 && sensor_data <= 2500) {
    Serial.println("There is some moisture, Soil is medium");
    lcd.print("Tanah rada basah");
    digitalWrite(relay_pin, HIGH);
  } else if (sensor_data < 2200) {
    Serial.println("Soil is wet");
    lcd.print("Tanah basah     ");
    digitalWrite(relay_pin, HIGH);
  }

  delay(1000); // Adjust the delay as needed
}
