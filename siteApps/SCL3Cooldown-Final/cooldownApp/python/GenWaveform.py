import sys

if len(sys.argv) != 4:
    raise SystemExit('Usage:'+str(sys.argv[0])+' PVList WaveforPVName Datatype(DOUBLE, UCHAR, USHORT, ...)')

#Usage:$>python GenWaveform.py Temp.txt SCL3:Cooldown:DataWF
filename = str(sys.argv[1])
wfname   = str(sys.argv[2])
datatype = str(sys.argv[3])
print('Argument List', filename)
print('Argument Count', len(sys.argv));

pvlist = []
f = open(filename, 'r')
for line in f:
    pvlist.append(line.rstrip('\n'))

#print(pvlist)
#print(len(pvlist))

listlength =len(pvlist);

f.close()

seqfilename = "snc"+filename.rsplit('.', 1)[0]+"WF"
seqExt = ".stt"
dbdExt = ".dbd"
vdbExt = ".vdb"
templExt = ".template"
subExt = ".sub"

print(seqfilename)

seq   = open(seqfilename+seqExt, "w")
dbd   = open(seqfilename+dbdExt, "w")
vdb   = open("../Db/"+seqfilename+vdbExt, "w")
templ = open("../Db/"+seqfilename+templExt, "w")
sub = open("../Db/"+seqfilename+subExt, "w")

########### Seqencer File Generation ###################
sncText = "program " + seqfilename + "\n\noption +r; \n\n"
seq.write(sncText)

sncText = "double wflist["+ str(listlength)+ "];\n"
seq.write(sncText)

sncText = "assign wflist to {\n"
seq.write(sncText)

index = 1
for line in range(listlength):
    if(index%5==0):
        sncText="\""+pvlist[line]+"\",\n"
    elif(index != listlength):
        sncText="\""+pvlist[line]+"\","
    else:
        sncText="\""+pvlist[line]+"\""
    seq.write(sncText)
    index += 1
sncText = "\n};\nmonitor wflist;\nevflag evWave;\nsync wflist evWave;\n\n"
seq.write(sncText)

sncText = "double wfValue["+ str(listlength)+ "];\n"
seq.write(sncText)

sncText = "assign wfValue to \""+wfname+"\";\n"
seq.write(sncText)

sncText = "monitor wfValue;\nevflag evWaveIdx;\nsync wfValue evWaveIdx;\n\n"
seq.write(sncText)

sncText = "double idxValue["+ str(listlength)+ "];\n"
seq.write(sncText)

sncText = "assign idxValue to {\n"
seq.write(sncText)

index = 1
for line in range(listlength):
    if(index%5==0):
        sncText="\"{SYS}{SUBSYS}{DEV}{SUBDEV}Index_"+str(line)+"\",\n"
    elif(index != listlength):
        sncText="\"{SYS}{SUBSYS}{DEV}{SUBDEV}Index_"+str(line)+"\","
    else:
        sncText="\"{SYS}{SUBSYS}{DEV}{SUBDEV}Index_"+str(line)+"\""
    seq.write(sncText)
    index += 1

#sncText = "\n};\nmonitor idxValue;\nevflag evWaveIdx;\nsync idxValue evWaveIdx;\n\n"
sncText = "\n};\nmonitor idxValue;\n\n"
seq.write(sncText)

sncText = "%% #include <math.h>\n"
seq.write(sncText)

##PVList Values to Waveform 
sncText = "ss make"+seqfilename+"\n{\n"
seq.write(sncText)

sncText = "\tstate init\n\t{\n"
seq.write(sncText)

sncText = "\t\twhen(TRUE)\n\t\t{\n"
seq.write(sncText)

sncText = "\t\t\tefClear(evWave);\n\t\t}state MakeWaveform\n\t}\n"
seq.write(sncText)

sncText = "\tstate MakeWaveform\n\t{\n"
seq.write(sncText)

sncText = "\t\twhen(efTestAndClear(evWave))\n\t\t{\n"
seq.write(sncText)

index = 1
for line in range(len(pvlist)):
    strline = str(line)
    if(line==0):
        sncText="\t\t\twfValue["+strline+"] = wflist["+strline+"];"
    elif(index%5==0):
        sncText="\twfValue["+strline+"] = wflist["+strline+"];\n"
    elif(index%5==1):
        sncText="\t\t\twfValue["+strline+"] = wflist["+strline+"];"
    else:
        sncText="\twfValue["+strline+"] = wflist["+strline+"];"
    seq.write(sncText)
    index += 1

sncText = "\n\t\t\tpvPut (wfValue, SYNC);\n"
seq.write(sncText)

sncText = "\n\t\t}state MakeWaveform\n\t}\n"
seq.write(sncText)

sncText = "}\n"
seq.write(sncText)

##Waveform to Index Value
sncText = "ss make"+seqfilename+"toIdxValue\n{\n"
seq.write(sncText)

sncText = "\tstate init\n\t{\n"
seq.write(sncText)

sncText = "\t\twhen(TRUE)\n\t\t{\n"
seq.write(sncText)

sncText = "\t\t\tefClear(evWaveIdx);\n\t\t}state MakeWaveform_Idx\n\t}\n"
seq.write(sncText)

sncText = "\tstate MakeWaveform_Idx\n\t{\n"
seq.write(sncText)

sncText = "\t\twhen(efTestAndClear(evWaveIdx))\n\t\t{\n"
seq.write(sncText)

index = 1
for line in range(len(pvlist)):
    strline = str(line)
    sncText="\t\t\tidxValue["+strline+"] = wfValue["+strline+"];"
    seq.write(sncText)
    sncText = "\tpvPut(idxValue["+ strline +"], SYNC);\n"
    seq.write(sncText)
    index += 1

sncText = "\n\t\t}state MakeWaveform_Idx\n\t}\n"
seq.write(sncText)

sncText = "}\n"
seq.write(sncText)
seq.close()
########### Seqencer File Close ###################

########### DBD File Generation ###################
sncText = "registrar("+seqfilename+"Registrar)\n"
dbd.write(sncText)
dbd.close()
###################################################

########### Waveform DB File Generation ###################
sncText = "record(waveform, \""+wfname+"\")\n{\n"
vdb.write(sncText)
sncText = "\tfield(DTYP, \"Soft Channel\")\n"
vdb.write(sncText)

sncText = "\tfield(NELM, \""+str(listlength)+"\")\n"
vdb.write(sncText)

sncText = "\tfield(PREC, \"3\")\n"
vdb.write(sncText)

#sncText = "\tfield(FTVL, \"DOUBLE\")\n}\n"
sncText = "\tfield(FTVL, \""+datatype+"\")\n}\n"
vdb.write(sncText)
vdb.close()
###################################################

########### Template File Generation ###################
sncText = "record(ao, \"$(SYS)$(SUBSYS)$(DEV)$(SUBDEV)$(SIGNAL)\")\n{\n"
templ.write(sncText)

sncText = "\tfield(OUT, \"$(OUT)\")\n"
templ.write(sncText)

sncText = "\tfield(DTYP, \"Soft Channel\")\n"
templ.write(sncText)

sncText = "\tfield(PREC, \"3\")\n"
templ.write(sncText)

sncText = "\tfield(VAL, \"0\")\n}\n"
templ.write(sncText)
templ.close()
###################################################

########### subtitution File Generation ###################
strtemplfile = seqfilename+templExt
sncText = "file \"db/"+strtemplfile+"\"{ pattern \n"
sub.write(sncText)

sncText = "{SIGNAL,\t\t\t SCAN,\t\tOUT}\n"
sub.write(sncText)

for line in range(len(pvlist)):
    strline = str(line)
    sncText="{Index_"+strline+",\t\t\t \"Passive\",\t \"\"}\n"
    sub.write(sncText)

sncText="}\n"
sub.write(sncText)
sub.close()
###################################################
