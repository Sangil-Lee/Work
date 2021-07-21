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

for idx, file in enumerate(pvfiles):
    f = open('pv/'+str(file),'r')
    filename =str(file).rsplit('.', 1)[0]
    #pvnamelist.append(filename)
    pvnamelist[idx]=filename
    for line in f:
        pvlist.append(line.rstrip('\n'))
    f.close()
    globals()[pvnamelist[idx]] = pvlist
    pvlist = []

#pvnamelist.sort()
#print(pvnamelist.keys())
#print(pvnamelist.values())
#print(OM14_XV7301_7201)
#print(OM14_CV7401)

print(pvnamelist)
rev_dict = dict(map(reversed, pvnamelist.items()))
print(rev_dict)

#print(rev_dict['OM14_XV7301_7201'])

#testlist = 

pyname = pyname.rsplit('.', 1)[0]
pyname = pyname.replace('Gen','snc')
print(pyname)
seqExt = ".stt"
seqfile = pyname + seqExt
seq   = open(seqfile, "w")

##############################################################
nl = '\n'
open = '{'
close = '}'
arrcnt = 40
charsize = 60
##############################################################
seqText=f'program {pyname}{nl}\
option +r {nl}{nl}\
%% #include <math.h> {nl}{nl}\
'
seq.write(seqText)

idxsize = len(OM14_XV7301_7201)
Text =f'char OM14_XV7301_7201[{idxsize}][{charsize}] = {open}{nl}'
for idx, pvname in enumerate(OM14_XV7301_7201):
    idx = idx+1
    spvname = '"'+str(pvname)
    if(idx%5 == 0):
        spvname += '",\n'
    elif (idx != idxsize):
        spvname += '",'
    else:
        spvname += '"}'
    Text += (spvname)
seq.write(Text)

idxsize = len(OM14_CV7401)
Text =f'{nl}{nl}char OM14_CV7401[{idxsize}][{charsize}] = {open}{nl}'
for idx, pvname in enumerate(OM14_CV7401):
    idx = idx+1
    spvname = '"'+str(pvname)
    if(idx%5 == 0):
        spvname += '",\n'
    elif (idx != idxsize):
        spvname += '",'
    else:
        spvname += '"}'
    Text += (spvname)
seq.write(Text)

idxsize = len(OM14_CV8401)
Text =f'{nl}{nl}char OM14_CV8401[{idxsize}][{charsize}] = {open}{nl}'
for idx, pvname in enumerate(OM14_CV8401):
    idx = idx+1
    spvname = '"'+str(pvname)
    if(idx%5 == 0):
        spvname += '",\n'
    elif (idx != idxsize):
        spvname += '",'
    else:
        spvname += '"}'
    Text += (spvname)
seq.write(Text)

idxsize = len(OM14_CV8401_1)
Text =f'{nl}{nl}char OM14_CV8401_1[{idxsize}][{charsize}] = {open}{nl}'
for idx, pvname in enumerate(OM14_CV8401_1):
    idx = idx+1
    spvname = '"'+str(pvname)
    if(idx%5 == 0):
        spvname += '",\n'
    elif (idx != idxsize):
        spvname += '",'
    else:
        spvname += '"}'
    Text += (spvname)
seq.write(Text)

idxsize = len(OM14_CV8402)
Text =f'{nl}{nl}char OM14_CV8402[{idxsize}][{charsize}] = {open}{nl}'
for idx, pvname in enumerate(OM14_CV8402):
    idx = idx+1
    spvname = '"'+str(pvname)
    if(idx%5 == 0):
        spvname += '",\n'
    elif (idx != idxsize):
        spvname += '",'
    else:
        spvname += '"}'
    Text += (spvname)
seq.write(Text)

idxsize = len(OM14_CV8402_1)
Text =f'{nl}{nl}char OM14_CV8402_1[{idxsize}][{charsize}] = {open}{nl}'
for idx, pvname in enumerate(OM14_CV8402_1):
    idx = idx+1
    spvname = '"'+str(pvname)
    if(idx%5 == 0):
        spvname += '",\n'
    elif (idx != idxsize):
        spvname += '",'
    else:
        spvname += '"}'
    Text += (spvname)
seq.write(Text)

idxsize = len(OM14_CV8403)
Text =f'{nl}{nl}char OM14_CV8403[{idxsize}][{charsize}] = {open}{nl}'
for idx, pvname in enumerate(OM14_CV8403):
    idx = idx+1
    spvname = '"'+str(pvname)
    if(idx%5 == 0):
        spvname += '",\n'
    elif (idx != idxsize):
        spvname += '",'
    else:
        spvname += '"}'
    Text += (spvname)
seq.write(Text)

idxsize = len(OM14_PhaseEnd)
Text =f'{nl}{nl}char OM14_PhaseEnd[{idxsize}][{charsize}] = {open}{nl}'
for idx, pvname in enumerate(OM14_PhaseEnd):
    idx = idx+1
    spvname = '"'+str(pvname)
    if(idx%5 == 0):
        spvname += '",\n'
    elif (idx != idxsize):
        spvname += '",'
    else:
        spvname += '"}'
    Text += (spvname)
seq.write(Text)

idxsize = len(OM14_PhaseEnd_1)
Text =f'{nl}{nl}char OM14_PhaseEnd_1[{idxsize}][{charsize}] = {open}{nl}'
for idx, pvname in enumerate(OM14_PhaseEnd_1):
    idx = idx+1
    spvname = '"'+str(pvname)
    if(idx%5 == 0):
        spvname += '",\n'
    elif (idx != idxsize):
        spvname += '",'
    else:
        spvname += '"}'
    Text += (spvname)
seq.write(Text)

seq.close()

#Text=f''
#for line in f:
#
#    Text += line
#
#f.close()
#
#filename =filename.rsplit('.', 1)[0]
#seqExt = ".stt"
#seqfile = "snc"+filename + seqExt
#
#seq   = open(seqfile, "w")
#
#nl = '\n'
#open = '{'
#close = '}'
#
#seqText=f'program snc{filename}{nl}\
#option +r {nl}\
#%% #include <math.h> {nl}\
#'
#
#seq.write(seqText)
#seq.write(Text)
#seq.close()
#