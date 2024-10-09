import serial
import time
import pyautogui

# Replace 'COM3' with the serial port you are using
ser = serial.Serial('COM3', 9600, timeout=1)
time.sleep(2)  # Wait for the serial connection to stabilize

# Time delay between each key press (in seconds)
KEY_PRESS_DELAY = 0.5

def press_key(perintah):
    if perintah == "Right":
        pyautogui.press('right')  # Press the right arrow key
    elif perintah == "Left":
        pyautogui.press('left')  # Press the left arrow key
    elif perintah == "Forward":
        pyautogui.press('up')  # Press the up arrow key (example action)
    elif perintah == "Backward":
        pyautogui.press('down')  # Press the down arrow key (example action)

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
