# Use this along with arduino skech :
# ../arduino_sketch/serial_send_to_pi/serial_send_to_pi.ino

import serial
import datetime
import os

now=datetime.datetime.now()

#ordi='mac'
ordi='pi'

if 'mac' in ordi:
   # Mac
   ser = serial.Serial('/dev/tty.usbmodem3d11',9600)   
   file_name="%d%d%d%d.html" % (now.year,now.month,now.day,now.minute)
   file_name_index='index.html'
   remote=None
   log_file=None
else:
   # pi
   ser = serial.Serial('/dev/ttyACM0',9600)
   #file_name="/var/www/data/%d%d%d%d.html" % (now.year,now.month,now.day,now.minute)
   file_name=os.getcwd()+"/index.html_"+str(os.getpgid(0))
   file_name_index='/var/www/HenriAngers/index.html'
   remote="plante@toutoumeteo.homelinux.net"
   log_dir=os.environ['HOME']+'/data/tt_hr_sensor_network'

mois=('janvier','fevrier','mars','avril','mai','juin','jullet','aout','septembre','octobre','novembre','decembre')
jour=('Lundi','Mardi','Mercredi','Jeudi','Vendredi','Samedi','Dimanche')

print "Opening "+file_name
file=open(file_name,'w')

os.system("/bin/mkdir -p %s" % (log_dir))
#log_file=open(log_dir+"/log_file.txt",'a')

file.write('<HTML><HEAD><TITLE>iM&eacute;t&eacute;o.ca temperature maison</TITLE></HEAD><BODY BACKGROUND="http://toutoumeteo.homelinux.net/html/images/fond/rose_texture4.jpg"><IMG SRC="http://toutoumeteo.homelinux.net/html/images/fond/logo_imeteo_bande3.jpg\n" width="100%"><H1>')


file.write("%s %s %s %d, %d h %d\n<BR>\n" % (jour[now.weekday()],now.day,mois[now.month-1],now.year,now.hour,now.minute))

lire =  True
while lire:
   line = ser.readline()
   print line
   if 'DEBUT' in line:
      line = ser.readline()
      print line
      file.write(line)
      name = "%s/%s%d%02d%02d.txt" % (log_dir, (line.split(':')[0]).replace (" ", "_"), now.year, now.month, now.day)
      print name
      log_file=open(name,'a')
      log_file.write("%d%02d%02d%02d%02d, %s" % (now.year,now.month,now.day,now.hour,now.minute,line))
      log_file.close()
      file.write("<BR>")
      line = ser.readline()
      print line
      file.write(line)
      file.close()
      name = "%s/%s%d%02d%02d.txt" % (log_dir, (line.split(':')[0]).replace (" ", "_"), now.year, now.month, now.day)
      print name
      log_file=open(name,'a')
      log_file.write("%d%02d%02d%02d%02d, %s" % (now.year,now.month,now.day,now.hour,now.minute,line))
      log_file.close()
      #os.system("/bin/cp %s %s" % (file_name,file_name_index))
      print "Executing the os commande: /usr/bin/scp %s %s:%s" % (file_name,remote,file_name_index)
      os.system("/bin/ls -al %s" % (file_name))
      os.system("/usr/bin/scp %s %s:%s" % (file_name,remote,file_name_index))
      os.system("/bin/rm -f %s" % (file_name))
      lire = False
#

