#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define LDR_PIN 32     
#define LED 26   
const int lightThreshold = 3000;

// Initialize the LCD with I2C address 0x27 (commonly used). Adjust if needed.
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);

  // Initialize LCD
  lcd.init();
  lcd.backlight(); // Turn on the LCD backlight
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  // Configure pins
  pinMode(LDR_PIN, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  delay(2000); // Display initialization message for 2 seconds
  lcd.clear();
}

void loop() {
  int lightLevel = analogRead(LDR_PIN);
  Serial.print("Light Level: ");
  Serial.println(lightLevel);

  // Update LCD display with light level
  lcd.setCursor(0, 0);
  lcd.print("Light Level:");
  lcd.print(lightLevel);

  // Check light level and control the relay
  if (lightLevel > lightThreshold) {
    digitalWrite(LED, HIGH);
    Serial.println("Lamp ON");

    lcd.setCursor(0, 1);
    lcd.print("Lamp: ON ");
  } else {
    digitalWrite(LED, LOW);
    Serial.println("Lamp OFF");

    lcd.setCursor(0, 1);
    lcd.print("Lamp: OFF");
  }

  delay(1000);
}
