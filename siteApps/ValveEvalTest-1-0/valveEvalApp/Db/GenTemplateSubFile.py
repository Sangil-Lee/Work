#!/usr/local/bin/python3.8
import sys
import os
import glob
import re
from pathlib import Path

#python3.8 GenTemplateSubFile.py ao "OUTB,SCAN" "Setpt,1 second" "10,40,40" Valve.pv Example 0
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
record({type}, "{prefix}{signal}"){nl}\
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

sncText = f'{nl}\
    field(FLNK, "{prefix}{signal}FOut"){nl}\
    field(SCAN, "Passive"){nl}\
    field(PINI, "YES"){nl}\
    field(VAL, "0"){nl}\
'
templ.write(sncText)
sncText ='\n}\n'
templ.write(sncText)

sncText = f'{nl}\
record(bi, "{prefix}Start") {open}{nl}\
	field(SCAN, "Passive"){nl}\
	field(PINI, "YES"){nl}\
	field(VAL, "0"){nl}\
	field(ZNAM, "Stop"){nl}\
	field(ONAM, "Start"){nl}\
{close}{nl}\
record(dfanout, "{prefix}{signal}FOut") {open}{nl}\
    field(SCAN, "Passive"){nl}\
    field(OUTA, "{prefix}{signal}Comp PP"){nl}\
    field(OUTB, "{prefix}$(OUTB) CPP"){nl}\
    field(DOL,  "{prefix}{signal}"){nl}\
    field(OMSL, "closed_loop"){nl}\
{close}{nl}\
record(compress, "{prefix}{signal}Comp") {open}{nl}\
	field(SCAN, "Passive"){nl}\
	field(ALG,  "Circular Buffer"){nl}\
	field(NSAM, "6"){nl}\
	field(FLNK, "{prefix}{signal}Eval"){nl}\
	field(INP, "{prefix}{signal}"){nl}\
{close}{nl}\
record(acalcout, "{prefix}{signal}Eval") {open}{nl}\
	field(SCAN, "Passive"){nl}\
	field(NELM, "6"){nl}\
	field(CALC, "B:=AA[D-1,D-1];C:=AVG(AA);(A==0||B==0||D<6)?1:C==B?0:1"){nl}\
	field(INAA, "{prefix}{signal}Comp"){nl}\
	field(INPA, "{prefix}Start"){nl}\
	field(INPD, "{prefix}{signal}Comp.NUSE"){nl}\
	field(OOPT, "Every Time"){nl}\
	field(DOPT, "Use CALC"){nl}\
{close}{nl}\
record(calcout, "{prefix}{signal}CDLogic") {open}{nl}\
    field(SCAN, "Passive"){nl}\
    field(PINI, "NO"){nl}\
    field(INPA, "{prefix}{signal}"){nl}\
    field(FLNK, "{prefix}{signal}.PROC CPP"){nl}\
    field(OUT,  "{prefix}{signal}.VAL CPP"){nl}\
    field(CALC, "$(CALC)"){nl}\
    field(INPB, "$(INPB)"){nl}\
    field(INPC, "$(INPC)"){nl}\
    field(INPG, "0.5"){nl}\
    field(INPH, "1.3"){nl}\
    field(INPI, "$(INPI)"){nl}\
    field(INPJ, "$(INPJ)"){nl}\
    field(INPK, "$(INPK)"){nl}\
    field(INPL, "$(INPL)"){nl}\
    field(OOPT, "Every Time"){nl}\
    field(DOPT, "Use CALC"){nl}\
{close}{nl}\
'
templ.write(sncText)
templ.close()
###################################################
########### subtitution File Generation ###################
sncText = f'file "db/{templfile}" {open} pattern{nl}'
sub.write(sncText)
signal = re.split('[$()]',prefix+signal)
#sncText = '{'+signal[2]+',\t\t'
signal = ' '.join(signal).split()
#pvname = ','.join(str(e) for e in signal)
print(signal)
pvname = ''
for e in signal:
    e = '{message: <{padcnt}}'.format(message = e+',', padcnt=int(str(paddinglist[0])))
    pvname = pvname + e

#print(pvname)
open  = '{'
comma =','
#sncText = f'{open} {signal[2]}{comma: <20}'
#sncText = '{'+signal[2]+','.ljust(20)
#sncText = '{'+signal[2]+','
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
    #sncText = '{\"'+signal[2]+'_'+str(cnt)+'\",\t\t'
    #sncText = '{\"'+pvlist[cnt]+'\",\t\t'
    #sncText = f'{open}"{pvlist[cnt]}"{comma: <20}'
    pvnamelist = pvlist[cnt] 
    pvnamelist = re.split('[-:]', pvnamelist)
    print(pvnamelist)
    
    pvname =''
    for indx, e in enumerate(pvnamelist):
        if(signal[indx] == 'SUBSYS' or signal[indx] == 'SUBDEV'):
            e = '-' + e + ':'
        e = '{message: <{padcnt}}'.format(message = e+',', padcnt=int(str(paddinglist[0])))
        pvname = pvname + e

    sncText = '{'+pvname
    #sncText = '{\"'+pvlist[cnt]+'\",'
    #sncText = '{message: <{padcnt}}'.format(message = sncText, padcnt=int(str(paddinglist[0])))
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
