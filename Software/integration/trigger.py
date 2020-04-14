import serial


class ArduinoTrigger():
    def __init__(self, serial_port, baud_rate=9600):
        self.ser = serial.Serial(serial_port, baud_rate)
        print(self.ser.name)

    def publish(self, string):
        self.ser.open()
        self.ser.write(string.encode())
        self.ser.close()
