import serial
import time
import json
import requests
seru = serial.Serial('/dev/ttyUSB0', 115200)
import datetime

datai = ""
datau = ""
url = 'http://211.173.192.37:7890/api/bodyheat'
headers = {'Content-type':'application/json;charset=UTF-8',
                'serviceID': 'S00001',
                'equipId'  : 'GG000001',
                'modelId'  : '',
                'reqTypeCd': '12'}

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
  print(body)
  res = requests.post(url, headers = headers, data = body)
  print(res.json())
