#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Replace with your network credentials
char ssid[] = "hoshinova";
char pass[] = "Ichizora";

// Replace with your ThingSpeak API key
String apiKey = "URKH0R05Z9N21KEW";

int sensor_pin = 32; // Sensor Pin
int relay_pin = 5;   // Relay Pin

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display
WiFiClient client;

void sendSensorDataToThingSpeak(int sensorData) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://api.thingspeak.com/update?api_key=" + apiKey + "&field1=" + String(sensorData);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0) {
      Serial.printf("HTTP GET request sent, response code: %d\n", httpCode);
    } else {
      Serial.printf("Failed to send HTTP GET request, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(sensor_pin, INPUT);
  pinMode(relay_pin, OUTPUT);
  lcd.init();
  lcd.backlight(); // Turn on the backlight

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
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

  sendSensorDataToThingSpeak(sensor_data); // Send data to ThingSpeak
  delay(1000); // Adjust the delay as needed
}