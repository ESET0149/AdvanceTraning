import serial
import time
 
ser = serial.Serial('COM3', 115200, timeout=1)
time.sleep(2)  # wait for the serial connection to initialize

while True:
    cmd = input("Enter command ( on/off): ").strip().lower()
    if cmd in ['on', 'off']:
        ser.write((cmd + '\n').encode())
        time.sleep(0.1)  # give some time for the command to be sent
        if ser.in_waiting:
            print(ser.readline().decode(errors = 'ignore').strip())
    else:
        print("Invalid command. Please enter 'on' or 'off'.")
