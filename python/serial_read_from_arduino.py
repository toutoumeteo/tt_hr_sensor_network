# Use this along with arduino skech :
# ../arduino_sketch/serial_send_to_pi/serial_send_to_pi.ino

import serial
import datetime
import os

now=datetime.datetime.now()

ordi='mac'
#ordi='pi'

if 'mac' in ordi:
   # Mac
   ser = serial.Serial('/dev/tty.usbmodem3d11',9600)   
   file_name="%d%d%d%d.html" % (now.year,now.month,now.day,now.minute)
   file_name_index='index.html'
else:
   # pi
   ser = serial.Serial('/dev/ttyACM0',9600)
   #file_name="/var/www/data/%d%d%d%d.html" % (now.year,now.month,now.day,now.minute)
   file_name='/var/www/index.html'
   file_name_index='/var/www/index.html'

mois=('janvier','fevrier','mars','avril','mai','juin','jullet','aout','septembre','octobre','novembre','decembre')
jour=('Lundi','Mardi','Mercredi','Jeudi','Vendredi','Samedi','Dimanche')

file=open(file_name,'w')

file.write('<HTML><HEAD><TITLE>iM&eacute;t&eacute;o.ca temperature maison</TITLE></HEAD><BODY BACKGROUND="http://toutoumeteo.homelinux.net/html/images/fond/rose_texture4.jpg"><IMG SRC="http://toutoumeteo.homelinux.net/html/images/fond/logo_imeteo_bande3.jpg\n" width="100%"><H1>')

file.write("%s %s %s %d, %d h %d\n<BR>\n" % (jour[now.weekday()],now.day,mois[now.month],now.year,now.hour,now.minute))

lire =  True
while lire:
   x = ser.readline()
   print x
   if 'DEBUT' in x:
      x = ser.readline()
      print x
      file.write(x)
      file.write("<BR>")
      x = ser.readline()
      print x
      file.write(x)
      file.close
      #os.system("/bin/cp %s %s" % (file_name,file_name_index))
      lire = False

#

