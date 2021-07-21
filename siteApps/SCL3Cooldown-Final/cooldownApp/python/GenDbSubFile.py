#!/usr/local/bin/python3.8
import sys
import os
import glob
import re
from pathlib import Path


argc = len(sys.argv)
if argc != 8:
    #print('Usage:'+str(sys.argv[0])+' recordtype "Fieldlist" "ValueList" PVfile SignalPad \
    #FieldPad')
    #raise SystemExit('EX)'+str(sys.argv[0])+' ao "OUT,FLNK,SCAN" "OUTLink, FLNKLink, \
    #1 second" test 30 20')
    print('Usage:'+str(sys.argv[0])+' recordtype "Fieldlist" "ValueList" "PaddingList" Pvfile.pv \
    Subfilename 0 or 1')
    raise SystemExit('EX)'+str(sys.argv[0])+' ao "OUT,FLNK,SCAN" "OUTLink, FLNKLink, \
    1 second" "30,20,20" test.pv test (0 or 1)[0:new_overwrite, 1:append]')

type        = str(sys.argv[1])
fieldlist   = str(sys.argv[2])
valuelist   = str(sys.argv[3])
paddinglist = str(sys.argv[4])
pvlistfile  = str(sys.argv[5])
subfile    = str(sys.argv[6])
new_append = int(sys.argv[7])
if(new_append < 0 or new_append > 1):
    raise SystemExit('Last argument 0 or 1(0:new_overwrite, 1:append)')

templfile = pvlistfile.split('.',1)[0]
templfile = templfile +'.template'

fieldlist = re.split('[,:]', fieldlist)
valuelist = re.split('[,:]', valuelist)
paddinglist = re.split('[,:]', paddinglist)

pvfile = open(pvlistfile, 'r')
templ  = open(templfile, "w")

subfile = subfile+'.sub'

if(Path(subfile).is_file() and new_append == 1):
    sub    = open(subfile, "a")
    sub.write('\n')
else:
    sub    = open(subfile, "w")

pvlist = []
for line in pvfile:
    line = line.rstrip('\n')
    if(line[0] == "#"): continue
    pvlist.append(line.rstrip('\n'))

count = len(pvlist)
#############################################################
nl = '\n'
tab = '\t'
open = '{'
close = '}'
charsize = 60
prefix = '$(SYS)$(SUBSYS)$(DEV)$(SUBDEV)'
signal = '$(SIGNAL)'
charnull = "'\\0'"
#signalpad = sys.argv[5]
#fieldpad  = sys.argv[6]
##############################################################

sncText = f'{nl}\
record({type}, "{prefix}$(SIGNAL)"){nl}\
{open}\
'
templ.write(sncText)

listcnt = len(fieldlist)
strvalue = valuelist[0]
#print(len(valuelist), strvalue)

if(strvalue == ""):
    for cnt in range(listcnt):
        if(cnt != 0):
          valuelist.append("\"\"")

for field in fieldlist:
    sncText = f'{nl}\
    field({field}, "$({field})")\
    '
    templ.write(sncText)

sncText ='\n}\n'
templ.write(sncText)
templ.close()
###################################################
########### subtitution File Generation ###################
sncText = f'file "db/{templfile}" {open} pattern{nl}'
sub.write(sncText)
signal = re.split('[$()]',signal)
#sncText = '{'+signal[2]+',\t\t'
open  = '{'
comma =','
#sncText = f'{open} {signal[2]}{comma: <20}'
#sncText = '{'+signal[2]+','.ljust(20)
sncText = '{'+signal[2]+','
sncText = '{message: <{padcnt}}'.format(message = sncText, padcnt=int(str(paddinglist[0])))
sub.write(sncText)

for idx, field in enumerate(fieldlist):
    idx = idx+1
    if(idx == len(fieldlist)):
        sncText = field
    else:
        sncText = field+','
        sncText = '{message: <{padcnt}}'.format(message = sncText, padcnt=int(str(paddinglist[idx])))
    sub.write(sncText)
sncText = '}\n'
sub.write(sncText)

for cnt in range(int(count)):
    #sncText = '{\"'+signal[2]+'_'+str(cnt)+'\",\t\t'
    #sncText = '{\"'+pvlist[cnt]+'\",\t\t'
    #sncText = f'{open}"{pvlist[cnt]}"{comma: <20}'
    sncText = '{\"'+pvlist[cnt]+'\",'
    sncText = '{message: <{padcnt}}'.format(message = sncText, padcnt=int(str(paddinglist[0])))
    sub.write(sncText)
    for idx, value in enumerate(valuelist):
        idx = idx+1
        if(value == "\"\""): value = ""
        if(idx == len(valuelist)):
            sncText = "\""+ value + "\""
        else:
            sncText = "\"" + value+'\",'
            sncText = '{message: <{padcnt}}'.format(message = sncText, padcnt=int(str(paddinglist[idx])))
        sub.write(sncText)
    sncText = '}\n'
    sub.write(sncText)

sncText = '}\n'
sub.write(sncText)
sub.close()

print('Successfully Generated File:', templfile, subfile)
