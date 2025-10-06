#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi LCD I2C (ubah alamat jika perlu)
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define MQ2_PIN 32  // Pin sensor gas (gunakan pin ADC ESP32)
#define BUZZER_PIN 33 // Pin buzzer

void setup() {
    Serial.begin(115200);
    lcd.init(); 
    lcd.backlight();
    
    pinMode(MQ2_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    
    lcd.setCursor(0, 0);
    lcd.print("Gas Detector");
    lcd.setCursor(0, 1);
    lcd.print("Initializing...");
    delay(2000);
    lcd.clear();
}

void loop() {
    int gasValue = analogRead(MQ2_PIN);
    Serial.print("Jumlah Gas: ");
    Serial.println(gasValue);

    lcd.setCursor(0, 0);
    lcd.print("Gas Level: ");
    lcd.print(gasValue);

    if (gasValue > 1300) {  // Sesuaikan threshold sesuai kebutuhan
        lcd.setCursor(0, 1);
        lcd.print("TERDETEKSI GAS!");
        digitalWrite(BUZZER_PIN , HIGH);
        
        
      
    } else {
        lcd.setCursor(0, 1);
        lcd.print("Aman            ");
        digitalWrite(BUZZER_PIN , LOW);
    }

    delay(1000);
}
