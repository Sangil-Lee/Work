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
pvnamelist = []

for idx, file in enumerate(pvfiles):
    f = open('pv/'+str(file),'r')
    filename =str(file).rsplit('.', 1)[0]
    pvnamelist.append(filename)
    for line in f:
        pvlist.append(line.rstrip('\n'))
    f.close()
    globals()[pvnamelist[idx]] = pvlist
    pvlist = []

pvnamelist.sort()
print(pvnamelist)
#print(OM14_XV7301_7201)
#print(OM14_CV7401)

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
    spvname = '"'+str(pvname)
    #print(spvname,idx)
    if(idx%5 == 0 and idx == 0):
        spvname += '",'
    elif(idx%5 == 0 and idx != 0):
        spvname += '",\n'
    elif (idx%5 !=0 and idx < idxsize-1):
        spvname += '",'
    elif (idx%5 !=0 and idx == idxsize-1):
        spvname += '"}'
    Text += (spvname)
seq.write(Text)

idxsize = len(OM14_CV7401)
Text =f'{nl}{nl}char OM14_CV7401[{idxsize}][{charsize}] = {open}{nl}'
for idx, pvname in enumerate(OM14_CV7401):
    spvname = '"'+str(pvname)
    #print(spvname,idx)
    if(idx%5 == 0 and idx == 0):
        spvname += '",'
    elif(idx%5 == 0 and idx != 0):
        spvname += '",\n'
    elif (idx%5 !=0 and idx < idxsize-1):
        spvname += '",'
    elif (idx%5 !=0 and idx == idxsize-1):
        spvname += '"}'
    Text += (spvname)
seq.write(Text)

idxsize = len(OM14_CV8401)
Text =f'{nl}{nl}char OM14_CV8401[{idxsize}][{charsize}] = {open}{nl}'
for idx, pvname in enumerate(OM14_CV8401):
    spvname = '"'+str(pvname)
    #print(spvname,idx)
    if(idx%5 == 0 and idx == 0):
        spvname += '",'
    elif(idx%5 == 0 and idx != 0):
        spvname += '",\n'
    elif (idx%5 !=0 and idx < idxsize-1):
        spvname += '",'
    elif (idx%5 !=0 and idx == idxsize-1):
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