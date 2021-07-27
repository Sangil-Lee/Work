import sys

filename = str(sys.argv[1])

pvlist = []
f = open(filename, 'r')
for line in f:
    if(line[0] == '#'): continue
    pvlist.append(line.rstrip('\n'))
f.close()

seqfilename = "snc"+filename.rsplit('.', 1)[0]+"WF"
seqExt = ".stt"
seq   = open('../src/'+seqfilename+seqExt, "w")

########### Seqencer File Generation ###################
nl = '\n'
lenlist = len(pvlist)
open = '{'
close = '}'

sncText = f'program {seqfilename} {nl}{nl}\
option +r; {nl}\
double wfList[{lenlist}]{nl}\
assign wfList to {open}{nl}\
' 
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
print("Successfully Generated File: " + seqfilename+seqExt)
