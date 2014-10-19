# Use this to see the serial print from any arduino connected to USB
# NOTE : to make upload, this must not be running, otherwise an error
#        similar to the following will be happen:
#        /usr/share/arduino/Arduino.mk:496: recipe for target 'raw_upload' failed
#        make: *** [raw_upload] Error 1

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
