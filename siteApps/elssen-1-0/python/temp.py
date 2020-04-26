import serial
import time
import json
import requests
import random
seru = serial.Serial('/dev/ttyUSB0',115200)
import datetime
from epics import caput

datau = ''

def temp(t):
    t = int(t,16)
    t = float(t)/100
#t = round(t,1)
    return t

url = 'http://elssen.cafe24.com/SmartTemp/temp_upload.php'
headers = {"Content-type" : "application/x-www-form-urlencoded"}


pv1 = ''
pv2 = ''
while True:
 i = seru.read()
 i = i.encode('hex')
 datau += i
 s1 = 'eace'
 if s1 in datau:
   s2 = datau.find(s1)
   if len(datau[s2:s2+28])>27:
     a = datau[s2+8]
     b = datau[s2+9]
     c = datau[s2+10]
     d = datau[s2+11]
     e = a+'{0:x}'.format(int(b)+int(c))+d
     ad = int(e,16)
     t1 = datau[s2+12:s2+16]
     t2 = datau[s2+16:s2+20]
     t3 = datau[s2+20:s2+24]
     t4 = datau[s2+24:s2+28]
     t1 = temp(t1)
     t2 = temp(t2)
     t3 = temp(t3)
     t4 = temp(t4)
     now = datetime.datetime.now()
     now = now.strftime('%Y-%m-%d %H:%M:%S')
     datau = ''
     pv1 = str(ad)+':'+'T1_Temp'
     pv2 = str(ad)+':'+'T2_Temp'
     rannum = random.uniform(0.01, 0.0009)
     caput(pv1, t1+rannum)
     caput(pv2, t2+rannum)
     j = {'serial':ad,'t1_temp':t1,'t2_temp':t2,'t3_temp':t3,'t4_temp':t4,'today':now}
     body = json.dumps(j)
     print(j)
     res = requests.post(url,data = j)
     print(res.text)

