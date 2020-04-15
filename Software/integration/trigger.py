import serial
import time


class ArduinoTrigger():
    def __init__(self, serial_port, baud_rate=9600):
        self.ser = serial.Serial(serial_port, baud_rate)
        time.sleep(2)

    def publish(self, string):
        self.ser.write(string.encode())
