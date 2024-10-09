# KMTek
# Karya Merapi Teknologi
# Automatic Scrolling with Gesture Sensor
# @diazkibidi

import serial
import time
import pyautogui

# Ganti 'COM3' dengan port serial yang Anda gunakan
ser = serial.Serial('COM3', 9600, timeout=1)
time.sleep(2)  # Menunggu koneksi serial stabil

# Waktu delay antara setiap langkah scroll (dalam detik)
SCROLL_DELAY = 0.5

def kontrol_scroll(perintah):
    if perintah == "Right":
        pyautogui.scroll(500)  # Scroll ke bawah (Dapat disesuaikan)
    elif perintah == "Left":
        pyautogui.scroll(-500) # Scroll ke atas (Dapat disesuaikan)

try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8', errors='ignore').rstrip()
            if line:
                print(f"Gesture detected: {line}")
                kontrol_scroll(line)
                time.sleep(SCROLL_DELAY)  
except KeyboardInterrupt:
    print("Program dihentikan")
finally:
    ser.close()
