#!/usr/local/bin/python3.8
import sys
import os
import glob
import re

argc = len(sys.argv)
if argc != 5:
    print('Usage:'+str(sys.argv[0])+' recordtype "Fieldlist" "ValueList" PVfile')
    raise SystemExit('EX)'+str(sys.argv[0])+' ao "OUT,FLNK,SCAN" "OUTLink, FLNKLink, 1 second" test')

type = str(sys.argv[1])
fieldlist = str(sys.argv[2])
valuelist = str(sys.argv[3])
pvlistfile= str(sys.argv[4])
templfile = pvlistfile.split('.',1)[0]
fieldlist = re.split('[,:]', fieldlist)
valuelist = re.split('[,:]', valuelist)

pvfile = open(pvlistfile, 'r')
templ  = open(templfile+'.template', "w")
sub    = open(templfile+'.sub', "w")

pvlist = []
for line in pvfile:
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
sncText = f'file "db/{templfile}.template" pattern {open}{nl}'
sub.write(sncText)
signal = re.split('[$()]',signal)
sncText = '{'+signal[2]+',\t\t'
sub.write(sncText)

for idx, field in enumerate(fieldlist):
    idx = idx+1
    if(idx == len(fieldlist)):
        sncText = field
    else:
        sncText = field+',\t'
    sub.write(sncText)
sncText = '}\n'
sub.write(sncText)

for cnt in range(int(count)):
    #sncText = '{\"'+signal[2]+'_'+str(cnt)+'\",\t\t'
    sncText = '{\"'+pvlist[cnt]+'\",\t\t'
    sub.write(sncText)
    for idx, value in enumerate(valuelist):
        idx = idx+1
        if(value == "\"\""): value = ""
        if(idx == len(valuelist)):
            sncText = "\""+ value + "\""
        else:
            sncText = "\"" + value+'\",\t'
        sub.write(sncText)
    sncText = '}\n'
    sub.write(sncText)

sncText = '}\n'
sub.write(sncText)
sub.close()

print('Successfully Generated File:', templfile+'.template', templfile+'.sub')
