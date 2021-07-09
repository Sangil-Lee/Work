#!/usr/local/bin/python3.8
import sys
import os
import glob

pvfiles = []

for file in glob.glob1("pv","*.pv"):
    glob.glob(file)
    pvfiles.append(file)

pyname = str(sys.argv[0])
pvlist = []
#pvnamelist = ['OM14_XV7301_7201','OM14_CV7401']
#pvnamelist = []
pvnamelist = {}

pyname = pyname.rsplit('.', 1)[0]
pyname = pyname.replace('Gen','snc')
seqExt = '.stt'
seqfile = pyname + seqExt
seq = open(seqfile, 'w')

for idx, file in enumerate(pvfiles):
    f = open('pv/'+str(file),'r')
    filename =str(file).rsplit('.', 1)[0]
    #pvnamelist.append(filename)
    #pvnamelist[idx]=filename
    for line in f:
        pvlist.append(line.rstrip('\n'))
    f.close()
    pvnamelist[filename]=pvlist
    #globals()[pvnamelist[idx]] = pvlist
    pvlist = []

#pvnamelist.sort()
#print(pvnamelist.keys())
#print(pvnamelist.values())
#print(pvnamelist.get('OM14_XV7301_7201'))
#print(pvnamelist.get('OM15_PhaseEnd'))
#print(pvnamelist.get(pvfiles[2].rsplit('.',1)[0]))
#print(OM14_CV7401)

##############################################################
nl = '\n'
open = '{'
close = '}'
arrcnt = 40
charsize = 60
##############################################################

Text=f'program {pyname}{nl}\
option +r {nl}{nl}\
%% #include <math.h> {nl}{nl}\
'
seq.write(Text)


for idx, file in enumerate(pvfiles):
    listname = pvfiles[idx].rsplit('.',1)[0]
    pvlist = pvnamelist.get(listname)
    idxsize = len(pvlist)
    Text =f'char {listname}[{idxsize}][{charsize}] = {open}{nl}'
    for idx, pvname in enumerate(pvlist):
        idx = idx+1
        spvname = '"'+str(pvname)
        if(idx%5 == 0):
            spvname += '",\n'
        elif (idx != idxsize):
            spvname += '",'
        else:
            spvname += '"}\n\n'
        Text += (spvname)
    seq.write(Text)


seq.close()

