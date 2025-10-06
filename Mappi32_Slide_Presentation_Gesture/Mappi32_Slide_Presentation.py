# KMTek
# Karya Merapi Teknologi
# Automatic Scrolling with Gesture Sensor
# @diazkibidi

import serial
import time
import pyautogui

# Sesuaikan port dan baud dengan milik anda
ser = serial.Serial('COM3', 9600, timeout=1)
time.sleep(2) 

# Delay setiap pembacaan
KEY_PRESS_DELAY = 0.5

def press_key(perintah):
    if perintah == "Right":
        pyautogui.press('right')  # Gerakan ke kanan, slide ke kanan
    elif perintah == "Left":
        pyautogui.press('left')  # Gerakan ke kanan, slide ke kiri

try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8', errors='ignore').rstrip()
            if line:
                print(f"Gesture detected: {line}")
                press_key(line)
                time.sleep(KEY_PRESS_DELAY)
except KeyboardInterrupt:
    print("Program stopped")
finally:
    ser.close()
