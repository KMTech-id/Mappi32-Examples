// KMTek
// Karya Merapi Teknologi
// Automated Presentation Slides using Mappi32
// @diazkibidi

#include <Wire.h>
#include "paj7620.h"

#define GES_REACTION_TIME 100 
#define GES_ENTRY_TIME 200    
#define GES_QUIT_TIME 300     
#define BUZZER_PIN 18         

void setup() {
    Serial.begin(9600);
    Serial.println("\nPAJ7620U2 TEST DEMO: Recognize 9 gestures.");
    
    pinMode(BUZZER_PIN, OUTPUT);  

    uint8_t error = paj7620Init(); 
    if (error) {
        Serial.print("INIT ERROR, CODE:");
        Serial.println(error);
    } else {
        Serial.println("INIT OK");
    }
    Serial.println("Please input your gestures:\n");
}

void loop() {
    uint8_t data = 0, data1 = 0;

    uint8_t error = paj7620ReadReg(0x43, 1, &data); 
    if (!error) {
        switch (data) {
            case GES_RIGHT_FLAG:
                handleGesture(data, "Right");
                break;
            case GES_LEFT_FLAG:
                handleGesture(data, "Left");
                break;
            case GES_UP_FLAG:
                handleGesture(data, "Up");
                break;
            case GES_DOWN_FLAG:
                handleGesture(data, "Down");
                break;
            case GES_FORWARD_FLAG:
                Serial.println("Forward");
                soundBuzzer();
                break;
            case GES_BACKWARD_FLAG:
                Serial.println("Backward");
                soundBuzzer();
                break;
            case GES_CLOCKWISE_FLAG:
                Serial.println("Clockwise");
                soundBuzzer();
                break;
            case GES_COUNT_CLOCKWISE_FLAG:
                Serial.println("Anti-clockwise");
                soundBuzzer();
                break;
            default:
                paj7620ReadReg(0x44, 1, &data1);
                if (data1 == GES_WAVE_FLAG) {
                    Serial.println("Wave");
                    soundBuzzer();
                }
                break;
        }
    }
    delay(50); // Reduced from 100 to 50
}

void handleGesture(uint8_t &data, const char* direction) {
    delay(GES_ENTRY_TIME);
    paj7620ReadReg(0x43, 1, &data);
    if (data == GES_FORWARD_FLAG) {
        Serial.println("Forward");
        soundBuzzer();
    } else if (data == GES_BACKWARD_FLAG) {
        Serial.println("Backward");
        soundBuzzer();
    } else {
        if (strcmp(direction, "Right") == 0) {
            Serial.println("Right");
            soundBuzzer();
        } else if (strcmp(direction, "Left") == 0) {
            Serial.println("Left");
            soundBuzzer();
        } else if (strcmp(direction, "Up") == 0) {
            Serial.println("Up");
            soundBuzzer();
        } else if (strcmp(direction, "Down") == 0) {
            Serial.println("Down");
            soundBuzzer();
        }
    }
}

void soundBuzzer() {
    digitalWrite(BUZZER_PIN, HIGH); 
    delay(100);                      
    digitalWrite(BUZZER_PIN, LOW);  
}
