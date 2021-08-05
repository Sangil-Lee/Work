#!/usr/local/bin/python3.8
import sys
import os
import glob
import re
from pathlib import Path

#python3.8 GenTemplateSubFile.py ao "OUTB,SCAN" "Setpt,1 second" "10,40,40" Valve.pv Example 0
#python3.8 GenTemplateSubFile.py ao "CALC,INPB,INPC,INPI,INPJ,INPK,INPL,OUTB" "\"\",\"\",\"\",\"\",\"\",\"\",,Setpt" "10,10,10,10,10,10,10,10,40" Valve.pv Example 0
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

fieldlist = re.split('[,:]', fieldlist)
valuelist = re.split('[,:]', valuelist)
paddinglist = re.split('[,:]', paddinglist)

fieldlist   = ['\"\"' if value =='' else value for value in fieldlist]
valuelist   = ['\"\"' if value =='' else value for value in valuelist]
paddinglist = ['10' if value =='' else value for value in paddinglist]

print(fieldlist)

dirname, filename = os.path.split(pvlistfile)
templfile = filename.split('.',1)[0]
templfile = templfile +'.template'

pvfile = open(pvlistfile, 'r')
templ  = open('../Db/'+templfile, "w")

subfile = '../Db/'+subfile+'.sub'

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
##############################################################

#####################CDLogic Macro Field##########################
sncText = f'{nl}\
record({type}, "{prefix}{signal}"){nl}\
{open}'
templ.write(sncText)
for idx, value in enumerate(fieldlist):
    sncText = f'\
    field({fieldlist[idx]}, "$({fieldlist[idx]})"){nl}'
    templ.write(sncText)
sncText = f'{close}{nl}{nl}'
templ.write(sncText)
templ.close()
##################################################################
########### subtitution File Generation ###################
sncText = f'file "db/{templfile}" {open} pattern{nl}'
sub.write(sncText)
signal = re.split('[$()]',prefix+signal)
signal = ' '.join(signal).split()
#print(signal)
pvname = ''
for e in signal:
    e = '{message: <{padcnt}}'.format(message = e+',', padcnt=int(str(paddinglist[0])))
    pvname = pvname + e

#print(pvname)
open  = '{'
comma =','
sncText = '{'+pvname
#sncText = '{message: <{padcnt}}'.format(message = sncText, padcnt=int(str(paddinglist[0])))
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
    pvnamelist = pvlist[cnt] 
    pvnamelist = re.split('[-:]', pvnamelist)
    #print(pvnamelist)
    
    pvname =''
    for indx, e in enumerate(pvnamelist):
        if(signal[indx] == 'SUBSYS' or signal[indx] == 'SUBDEV'):
            e = '-' + e + ':'
        e = '{message: <{padcnt}}'.format(message = e+',', padcnt=int(str(paddinglist[0])))
        pvname = pvname + e

    sncText = '{'+pvname
    sub.write(sncText)
    for orgidx, value in enumerate(valuelist):
        idx = orgidx+1
        if(value == "\"\""): value = ""
        if(idx == len(valuelist)):
            if(fieldlist[orgidx].find('OUT') != -1 or fieldlist[orgidx] == 'FLNK'):
                pvnamelist.pop(len(pvnamelist)-1)
                pvname =''
                for indx, e in enumerate(pvnamelist):
                    if(signal[indx] == 'SUBSYS' or signal[indx] == 'SUBDEV'):
                        e = '-' + e + ':'
                    pvname = pvname + e
                sncText = "\""+ pvname + value + "\""
            else:
                sncText = "\""+ value + "\""
        else:
            if(fieldlist[orgidx].find('OUT') != -1 or fieldlist[orgidx] == 'FLNK'):
                pvnamelist.pop(len(pvnamelist)-1)
                pvname =''
                for indx, e in enumerate(pvnamelist):
                    if(signal[indx] == 'SUBSYS' or signal[indx] == 'SUBDEV'):
                        e = '-' + e + ':'
                    pvname = pvname + e
                sncText = "\""+ pvname + value + "\","
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
