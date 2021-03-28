import paho.mqtt.client as mqtt
import serial
import time
import json
import requests
seru = serial.Serial('/dev/ttyUSB0', 115200)
import datetime

datai = ""
datau = ""

while True:
 i = seru.read()
 datai += i
 i = i.encode('hex')
 datau += i
 group = datau[4:16]
 group = group.upper()
 ad = datau[16:28]
 ad = ad.upper()
 battery = datau[32:34]
 temp = datau[29:32]
 if len(datau)>32:
  a = int(temp,16)
  a = float(a)/100
  a = round(a,1)
  b = int(battery,16)
  now = datetime.datetime.now()
  now = now.strftime('%Y%m%d%H%M%S')
  datai = ""
  datau = ""
  j = {'observerId' : group,'sensor':[{'id':ad,'heat':a,'colDate':now,'battery':b}]}
  body = json.dumps(j)
  client = mqtt.Client()
  client.on_connect = on_connect
  client.on_disconnect = on_disconnect
  client.on_publish = on_publish
  client.connect('192.168.50.235', 1883)
  client.loop_start()
  client.publish('common', json.dumps({"success": "ok"}), 1)
  client.loop_stop()
  client.disconnect()
