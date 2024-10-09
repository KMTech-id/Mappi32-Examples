    /* Automatic Scrolling with Gesture Sensor */
      // @diazkibidi // Raffi Diaz Pratama //  

#include <Wire.h>
#include "paj7620.h"

#define GES_REACTION_TIME 100 // Reduced from 300 to 100
#define GES_ENTRY_TIME 200    // Reduced from 600 to 200
#define GES_QUIT_TIME 300     // Reduced from 800 to 300

void setup() {
    Serial.begin(9600);
    Serial.println("\nPAJ7620U2 TEST DEMO: Recognize 9 gestures.");

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
                break;
            case GES_BACKWARD_FLAG:
                Serial.println("Backward");
                break;
            case GES_CLOCKWISE_FLAG:
                Serial.println("Clockwise");
                break;
            case GES_COUNT_CLOCKWISE_FLAG:
                Serial.println("Anti-clockwise");
                break;
            default:
                paj7620ReadReg(0x44, 1, &data1);
                if (data1 == GES_WAVE_FLAG) {
                    Serial.println("Wave");
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
    } else if (data == GES_BACKWARD_FLAG) {
        Serial.println("Backward");
    } else {
        if (strcmp(direction, "Right") == 0) {
            Serial.println("Right");
        } else if (strcmp(direction, "Left") == 0) {
            Serial.println("Left");
        } else if (strcmp(direction, "Up") == 0) {
            Serial.println("Up");
        } else if (strcmp(direction, "Down") == 0) {
            Serial.println("Down");
        }
    }
}
