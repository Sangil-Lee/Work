import sys
import re

#python3.8 GenWaveformPV.py Setpt_list.txt ai WaveValue
argc = len(sys.argv)
#print(argc)
if argc != 4:
    print('Usage:python3.8 '+str(sys.argv[0])+' PVlistFile Datatype(ai/bi/longin,ao/bo/longout) WaveformName ')
    raise SystemExit('EX)python3.8 '+str(sys.argv[0])+' Setpt_list.txt ai WaveValue')

filename = str(sys.argv[1])
dtype = str(sys.argv[2]).lower()
wfname = str(sys.argv[3])

datatype = ''
if(dtype == 'ao' or dtype == 'ai'): 
    datatype = 'double'
elif(dtype == 'longin' or dtype == 'longout'): datatype = 'int'
elif(dtype == 'bi' or dtype == 'bo'): datatype = 'bool'

fieldlist = []
valuelist = ['\"\"', '\"\"', '\"\"', 'YES']
if(dtype == 'ai' or dtype == 'bi' or dtype == 'longin'):
    fieldlist = ['DESC', 'VAL', 'INP', 'PINI']
else: 
    fieldlist = ['DESC', 'VAL', 'OUT', 'PINI']

pvlist = []
f = open(filename, 'r')
for line in f:
    if(line[0] == '#'): continue
    pvlist.append(line.rstrip('\n'))
f.close()

seqfilename   = "snc"+filename.rsplit('.', 1)[0]+"WF"
templfilename = 'templ'+filename.rsplit('.', 1)[0]
subfilename   = 'sub'+filename.rsplit('.', 1)[0]
wffilename   = 'wf'+filename.rsplit('.', 1)[0]

seqExt = ".stt"
dbdExt = ".dbd"
templExt = ".template"
subExt = ".sub"
vdbExt = ".vdb"

seq   = open('../src/'+seqfilename+seqExt, "w")
dbd   = open('../src/'+seqfilename+dbdExt, "w")
templ = open('../Db/'+templfilename+templExt, "w")
sub   = open('../Db/'+subfilename+subExt, "w")
vdb   = open('../Db/'+wffilename+vdbExt, "w")
#########################################################
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
########### Waveform vdb Generation ###################
sncText = f'\
record(waveform, "{prefix}{wfname}"){nl}\
{open}{nl}\
    field(DESC, "Debug Waveform") {nl}\
    field(NELM, "{lenlist}") {nl}\
    field(FTVL, "{datatype.upper()}") {nl}\
{close}{nl}'
vdb.write(sncText)
vdb.close()
#########################################################
########### Seqencer DBD Generation ###################
sncText = "registrar("+seqfilename+"Registrar)\n"
dbd.write(sncText)
dbd.close()
########### Seqencer File Generation ###################
sncText = f'program {seqfilename} {nl}{nl}\
option +r; {nl}\
{datatype} wfList[{lenlist}];{nl}\
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
prefix = prefix.replace('$(','{')
prefix = prefix.replace(')','}')
sncText = f'{close};{nl}{nl}\
monitor wfList;{nl}\
evflag evWave;{nl}\
sync wfList to evWave;{nl}\
{datatype}  wfValue[{lenlist}];{nl}\
assign  wfValue to "{prefix}{wfname}";{nl}\
monitor wfValue; {nl}\
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
