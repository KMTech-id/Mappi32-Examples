#include <SPI.h>
#include <MFRC522.h>
#include <ESP32Servo.h>
#include <LiquidCrystal_I2C.h>
#define SS_PIN 5
#define RST_PIN 0
#define SERVO_PIN 14 // Pin connected to servo
#define IR_SENSOR_PIN 26
#define IR_SENSOR_PIN_1 25
#define IR_SENSOR_PIN_2 13
#define IR_SENSOR_PIN_3 16
#define IR_SENSOR_PIN_4 17
#define MAX_SLOTS 4 // Maximum number of available slots
LiquidCrystal_I2C lcd(0x27, 20, 4);
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance
Servo myServo; // Define servo name
int filledSlots = 0; // Variable to keep track of filled slots
bool RFID_detected = false; // Flag to indicate RFID detection
int previousAvailableSpots = MAX_SLOTS; // Variable to keep track of previous available spots
bool allSensorsActivated = false;
void setup() {
  Serial.begin(9600); // Initiate a serial communication
  SPI.begin(); // Initiate SPI bus
  mfrc522.PCD_Init(); // Initiate MFRC522
  myServo.attach(SERVO_PIN); // Servo pin
  myServo.write(0); // Servo start position
  pinMode(IR_SENSOR_PIN, INPUT); // Initialize IR sensor pin
  pinMode(IR_SENSOR_PIN_1, INPUT);
  pinMode(IR_SENSOR_PIN_2, INPUT);
  pinMode(IR_SENSOR_PIN_3, INPUT);
  pinMode(IR_SENSOR_PIN_4, INPUT);
  Serial.println("Put your card to the reader...");
  Serial.println();
  lcd.init(); // Initialize the LCD
  lcd.backlight(); // Turn on the backlight
  lcd.setCursor(0, 0); // Set cursor to the first line
  lcd.print("Available slots:");
}

void loop() {
  detectRFID();
  detectIRSensor();
  slotIRSensor();
}

void detectRFID() {
  // Look for new cards
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    // Show UID on serial monitor
    Serial.println("Authorized access");
    Serial.println();
    RFID_detected = true;

    if (filledSlots < MAX_SLOTS) {
      filledSlots++; // Increment filled slots
      myServo.write(90);
      delay(5000); // Keep the servo open for 5 seconds
      myServo.write(0);
    } else {
      Serial.println("Parking lot full. Cannot grant access.");
      lcd.setCursor(0, 1);
      lcd.print("Parking lot full. Cannot grant access");
      delay(3000);
      lcd.print("                                     ");
    }

    mfrc522.PICC_HaltA(); // Halt PICC
    mfrc522.PCD_StopCrypto1(); // Stop encryption on PCD
  }
    if (digitalRead(IR_SENSOR_PIN_1) == LOW &&
      digitalRead(IR_SENSOR_PIN_2) == LOW &&
      digitalRead(IR_SENSOR_PIN_3) == LOW &&
      digitalRead(IR_SENSOR_PIN_4) == LOW) {
    allSensorsActivated = true;
  } else {
    allSensorsActivated = false;
  }
  if (allSensorsActivated) {
    myServo.write(0);
  }
  // Display quota when it changes
  int availableSpots = MAX_SLOTS - filledSlots;
  if (availableSpots != previousAvailableSpots) {
    Serial.print("Available spots: ");
    Serial.println(availableSpots);
    lcd.setCursor(0, 1);
    lcd.print(availableSpots);
    previousAvailableSpots = availableSpots;
  }

  // Reset flags
  RFID_detected = false;
}
void slotIRSensor() {
  lcd.setCursor(0, 2);
  lcd.print("Slot Terpakai :");
  if (digitalRead(IR_SENSOR_PIN_1) == LOW) {
    lcd.setCursor(1, 3);
    lcd.print("1");
  } else {
    lcd.setCursor(1, 3);
    lcd.print(" ");
  }
  if (digitalRead(IR_SENSOR_PIN_2) == LOW) {
    lcd.setCursor(2, 3);
    lcd.print("2");
  } else {
    lcd.setCursor(2, 3);
    lcd.print(" ");
  }
  if (digitalRead(IR_SENSOR_PIN_3) == LOW) {
    lcd.setCursor(3, 3);
    lcd.print("3");
  } else {
    lcd.setCursor(3, 3);
    lcd.print(" ");
  }
  if (digitalRead(IR_SENSOR_PIN_4) == LOW) {
    lcd.setCursor(4, 3);
    lcd.print("4");
  } else {
    lcd.setCursor(4, 3);
    lcd.print(" ");
  }
}
void detectIRSensor() {
  // Check if IR sensor detects a car leaving
  if (digitalRead(IR_SENSOR_PIN) == LOW) {
    if (filledSlots > 0) {
      filledSlots--; // Decrement filled slots if a car leaves
      myServo.write(90);
      delay(5000);
      Serial.println("Car left. Slot freed.");
      myServo.write(0);
    }
  }
}
