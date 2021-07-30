import sys
import re

filename = str(sys.argv[1])
dtype = str(sys.argv[2]).lower()

pvlist = []
f = open(filename, 'r')
for line in f:
    if(line[0] == '#'): continue
    pvlist.append(line.rstrip('\n'))
f.close()

seqfilename   = "snc"+filename.rsplit('.', 1)[0]+"WF"
templfilename = 'templ'+filename.rsplit('.', 1)[0]
subfilename   = 'sub'+filename.rsplit('.', 1)[0]

seqExt = ".stt"
dbdExt = ".dbd"
templExt = ".template"
subExt = ".sub"

seq   = open('../src/'+seqfilename+seqExt, "w")
dbd   = open('../src/'+seqfilename+dbdExt, "w")
templ = open('../Db/'+templfilename+templExt, "w")
sub   = open('../Db/'+subfilename+subExt, "w")

########### Seqencer File Generation ###################
lenlist = len(pvlist)
nl = '\n'
tab = '\t'
open = '{'
close = '}'
charsize = 60
prefix = '$(SYS)$(SUBSYS)$(DEV)$(SUBDEV)'
signal = '$(SIGNAL)'
charnull = "'\\0'"
#########################################################
sncText = f'program {seqfilename} {nl}{nl}\
option +r; {nl}\
double wfList[{lenlist}]{nl}\
assign wfList to {open}{nl}' 
seq.write(sncText)

for orgidx, pvname in enumerate(pvlist):
    idx = orgidx + 1
    if(idx%5 == 0 and idx == lenlist):
        sncText = f'"{pvname}"{nl}'
    elif(idx%5 == 0 and idx != lenlist):
        sncText = f'"{pvname}",{nl}'
    elif(idx == lenlist):
        sncText = f'"{pvname}"{nl}'
    else:
        sncText = f'"{pvname}",'
    seq.write(sncText)
sncText = f'{close};{nl}{nl}\
monitor wfList;{nl}\
evflag evWave;{nl}\
sync wfList to evWave;{nl}\
ss make{seqfilename} {nl}\
{open}{nl}\
    state ini{nl}\
    {open}{nl}\
        when(TRUE){nl}\
        {open}{nl}\
            efSet(evWave);{nl}\
        {close}state MakeWaveform{nl}\
    {close}{nl}\
    state MakeWaveform{nl}\
    {open}{nl}\
        when(efTestAndClear(evWave)){nl}\
        {open}{nl}\
            int index = 0;{nl}\
            for(;index < {lenlist};index++){nl}\
                wfValue[index]=wfList[index];{nl}{nl}\
            pvPut (wfValue, SYNC);{nl}\
        {close}state MakeWaveform{nl}\
    {close}{nl}\
{close}'
seq.write(sncText)
seq.close()
##############################################################

datatype = ''
fieldlist = []
valuelist = ['\"\"', '\"\"', '\"\"', 'YES']
if(dtype == 'ao' or dtype == 'ai'): 
    datatype = 'DOUBLE'
elif(dtype == 'longin' or dtype == 'longout'): datatype = 'int'
elif(dtype == 'bi' or dtype == 'bo'): datatype = 'bool'

if(dtype == 'ai' or dtype == 'bi' or dtype == 'longin'):
    fieldlist = ['DESC', 'VAL', 'INP', 'PINI']
else: 
    fieldlist = ['DESC', 'VAL', 'OUT', 'PINI']

sncText = f'{nl}\
record({dtype}, "{signal}"){nl}\
{open}{nl}'
templ.write(sncText)

listcnt = len(fieldlist)
for idx, field in enumerate(fieldlist):
    sncText = f'\
    field({field}, {valuelist[idx]} ){nl}'
    templ.write(sncText)

sncText =f'{close}{nl}'
templ.write(sncText)
templ.close()
###################################################
########### subtitution File Generation ###################
sncText = f'file "db/{templfilename+templExt}" {open} pattern{nl}'
sub.write(sncText)
signal = re.split('[$()]',signal)
open  = '{'
comma =','
sncText = '{'+signal[2]+','
sncText = '{message: <{padcnt}}'.format(message = sncText, padcnt=40)
sub.write(sncText)

for idx, field in enumerate(fieldlist):
    idx = idx+1
    if(idx == len(fieldlist)):
        sncText = field
    else:
        sncText = field+','
        sncText = '{message: <{padcnt}}'.format(message = sncText, padcnt=20)
    sub.write(sncText)
sncText = '}\n'
sub.write(sncText)

for cnt in range(lenlist):
    sncText = '{'+pvlist[cnt]+','
    sncText = '{message: <{padcnt}}'.format(message = sncText, padcnt=40)
    sub.write(sncText)
    for idx, value in enumerate(valuelist):
        idx = idx+1
        if(value == "\"\""): value = ""
        if(idx == len(valuelist)):
            sncText = "\""+ value + "\""
        else:
            sncText = "\"" + value+'\",'
            sncText = '{message: <{padcnt}}'.format(message = sncText, padcnt=20)
        sub.write(sncText)
    sncText = '}\n'
    sub.write(sncText)

sncText = '}\n'
sub.write(sncText)
sub.close()

print('Successfully Generated File:', templfilename+templExt, seqfilename+seqExt)
