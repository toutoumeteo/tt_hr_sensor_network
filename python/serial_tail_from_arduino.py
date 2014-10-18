# Use this along with arduino skech :
# ../arduino_sketch/serial_send_to_pi/serial_send_to_pi.ino

import serial
import datetime
import os

#ordi='mac'
ordi='pi'

if 'mac' in ordi:
   # Mac
   ser = serial.Serial('/dev/tty.usbmodem3d11',9600)   
else:
   # pi
   ser = serial.Serial('/dev/ttyACM0',9600)

lire =  True
while lire:
   x = ser.readline()
   print x
#

