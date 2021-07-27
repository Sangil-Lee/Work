import sys

filename = str(sys.argv[1])
dtype = str(sys.argv[2]).upper()

pvlist = []
f = open(filename, 'r')
for line in f:
    if(line[0] == '#'): continue
    pvlist.append(line.rstrip('\n'))
f.close()

seqfilename   = "snc"+filename.rsplit('.', 1)[0]+"WF"

seqExt = ".stt"
dbdExt = ".dbd"
templExt = ".template"
subExt = ".sub"

seq   = open('../src/'+seqfilename+seqExt, "w")
dbd   = open('../src/'+seqfilename+dbdExt, "w")
templ   = open('../Db/'+seqfilename+templExt, "w")
sub   = open('../Db/'+seqfilename+subExt, "w")

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
if(dtype == 'AO' or dtype == 'AI'): datatype = 'DOUBLE'
elif(dtype == 'LONGIN' or dtype == 'LONGOUT'): datatype = 'int'
elif(dtype == 'BI' or dtype == 'BO'): datatype = 'bool'

sncText = f'{nl}\
record({type}, "{prefix}{signal}"){nl}\
{open}{nl}'
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

print('Successfully Generated File:', templfile,

print("Successfully Generated File: " + seqfilename+seqExt)
