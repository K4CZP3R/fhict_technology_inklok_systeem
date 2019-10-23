import serial
from time import sleep

class KspSerial:
    def __init__(self, port, bdrate):
        self.port = port
        self.bdrate = bdrate
        self.ser = serial.Serial()
        self.ser.baudrate = bdrate
        self.ser.port = str(port)

        self.START_MAGIC = "<".encode("ASCII")
        self.END_MAGIC = ">".encode("ASCII")
    def open_port(self):
        try:
            self.ser.open()
            return self.ser.is_open
        except:
            return self.ser.is_open
    def send_message(self, content):
        if not self.ser.is_open:
            return False
        
        self.ser.write(self.START_MAGIC)
        self.ser.write(str(content).encode("ASCII"))
        self.ser.write(self.END_MAGIC)
    def read_message(self):
        if not self.ser.is_open:
            return False
        
        start_magic_found = False
        end_magic_found = False
        recv_buf = ""
        while(self.ser.in_waiting > 0 and not end_magic_found):
            current_byte = self.ser.read(1)
            #print("Received: {}".format(current_byte))
            if not start_magic_found:
                if current_byte == self.START_MAGIC:
                    print("Start magic found!")
                    start_magic_found = True
            else:
                if current_byte == self.END_MAGIC:
                    print("End magic found!")
                    end_magic_found = True
                else:
                    recv_buf+=current_byte.decode("ASCII")
        return recv_buf
    