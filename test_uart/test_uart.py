#!python3
#
# serial sender
#
import serial

port = serial.Serial('/dev/cu.usbserial-SL0l841x', 115200, timeout=10)

buf = ' ' * 1000
port.write(buf.encode())
rsp = port.readline()
print(rsp)
