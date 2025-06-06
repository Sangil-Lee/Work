#!/usr/local/bin/python3.8
import sys
import os
import glob

pvfiles = []

for file in glob.glob1("pv/OM04","*.pv"):
    glob.glob(file)
    pvfiles.append(file)

pyname = str(sys.argv[0])

pvlist = []
pvnamelist = {}

pyname = pyname.rsplit('.', 1)[0]
pyname = pyname.replace('Gen','snc')
seqExt = '.stt'
seqDBD = '.dbd'
seqfile    = pyname + seqExt
seqdbdfile = pyname + seqDBD
seq    = open(seqfile, 'w')
seqdbd = open(seqdbdfile, 'w')

for idx, file in enumerate(pvfiles):
    f = open('pv/OM04/'+str(file),'r')
    filename =str(file).rsplit('.', 1)[0]
    for line in f:
        pvname = line.rstrip('\n')
        if(pvname[0] =='#'): continue
        pvlist.append(pvname)
    f.close()
    pvnamelist[filename]=pvlist
    globals()[filename]=pvlist
    pvlist = []

print(pvfiles)
##############################################################
nl = '\n'
open = '{'
close = '}'
charsize = 60
prefix = '{SYS}{SUBSYS}{DEV}{SUBDEV}'
charnull = "'\\0'"
##############################################################

####Make Sequencer ####################
#########DBD##########
Text=f'registrar({pyname}Registrar){nl}'
seqdbd.write(Text)
seqdbd.close()
############################.stt###############################
####Make PVList from PV Files##########
Text=f'program {pyname}{nl}\
option +r; {nl}{nl}\
%%#include <math.h> {nl}{nl}\
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
        if(idx%5 == 0 and idx < idxsize):
            spvname += '",\n'
        elif (idx%5 == 0 and idx == idxsize):
            spvname += '"};\n\n'
        elif (idx%5 !=0 and idx < idxsize):
            spvname += '",'
        else:
            spvname += '"};\n\n'
        Text += (spvname)
    seq.write(Text)
####Assign Valuet##########

Text = f'{nl}\
int stopindex = 2;{nl}\
int stopindex_1_2 = 266;{nl}\
int  stopindex_3 = 66;{nl}\
int  stopindex_4 = 22;{nl}\
int  stopindex_5 = 33;{nl}\
int  stopindex_6 = 55;{nl}\
int  stopindex_7 = 44;{nl}\
int  stopindex_8 = 2;{nl}\
string steppv;{nl}\
assign steppv to "SCL3:CoolDown:StepPV";{nl}\
monitor steppv;{nl}\
{nl}\
int ss_start = 0;{nl}\
assign ss_start to "SCL3:CoolDown:Start";{nl}\
monitor ss_start;{nl}\
evflag	efStop;{nl}\
sync ss_start efStop;{nl}\
{nl}\
int index = 0;{nl}\
assign index to "SCL3:CoolDown:IndexPV";{nl}\
monitor index;{nl}\
{nl}\
float cdDelay;{nl}\
assign cdDelay to "SCL3:CoolDown:ScanTime";{nl}\
monitor cdDelay;{nl}\
{nl}\
int presz_start = 0;{nl}\
assign presz_start to "SCL3:CoolDown:PrezStart";{nl}\
monitor presz_start;{nl}\
{nl}\
float	valvewave[107];{nl}\
assign	valvewave to "SCL3:CoolDown:ValveWave";{nl}\
monitor valvewave;{nl}\
{nl}\
char	logicname[60];{nl}\
int		proc = 1;{nl}\
assign	proc to "";{nl}\
{nl}\
char	valvename[60];{nl}\
float	valveval;{nl}\
assign  valveval to "";{nl}\
{nl}\
char	valve_evalname[60];{nl}\
int		valve_eval;{nl}\
assign	valve_eval to "";{nl}\
{nl}\
evflag	efCDOM041_0;{nl}\
evflag	efCDOM041_20;{nl}\
evflag	efCDOM041_100;{nl}\
{nl}\
ss ssCDOM041{nl}\
{open}{nl}\
	state stopCDOM041{nl}\
	{open}{nl}\
		when(efTestAndClear(efStop) && ss_start == 1){nl}\
		{open}{nl}\
			index = 0;{nl}\
			stopindex = 2;{nl}\
			stopindex_1_2 = 266;{nl}\
			pvPut(index, SYNC);{nl}\
			efSet(efCDOM041_20);{nl}\
		{close}state CDOM041_Init{nl}\
	{close}{nl}\
{nl}\
	state CDOM041_Init{nl}\
	{open}{nl}\
		when(delay(cdDelay) && efTestAndClear(efCDOM041_20)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM041[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM041[index]);{nl}\
{nl}\
			printf("%s, %s\\n", logicname, valvename);{nl}\
{nl}\
			if(index < stopindex){nl}\
			{open}{nl}\
				strcpy(steppv, valvename);{nl}\
				pvPut(steppv, SYNC);{nl}\
{nl}\
				pvAssign(valveval, valvename);{nl}\
				pvMonitor(valveval);{nl}\
{nl}\
				pvAssign(proc, logicname);{nl}\
				pvMonitor(proc);{nl}\
			{close};{nl}\
{nl}\
		{close}state CDOM041_Proc_20{nl}\
{nl}\
		when(delay(cdDelay) && efTestAndClear(efCDOM041_100)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM041[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM041[index]);{nl}\
{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM043_1[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM043_1[index]);{nl}\
			printf("%s, %s\\n", logicname, valvename);{nl}\
{nl}\
			if(index < stopindex){nl}\
			{open}{nl}\
				strcpy(steppv, valvename);{nl}\
				pvPut(steppv, SYNC);{nl}\
{nl}\
				pvAssign(valveval, valvename);{nl}\
				pvMonitor(valveval);{nl}\
{nl}\
				pvAssign(proc, logicname);{nl}\
				pvMonitor(proc);{nl}\
			{close};{nl}\
{nl}\
		{close}state CDOM041_Proc_100{nl}\
{nl}\
		when(delay(cdDelay) && efTestAndClear(efCDOM041_0)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM043_1[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM043_1[index]);{nl}\
			printf("%s, %s\\n", logicname, valvename);{nl}\
{nl}\
			if(index < stopindex){nl}\
			{open}{nl}\
				strcpy(steppv, valvename);{nl}\
				pvPut(steppv, SYNC);{nl}\
{nl}\
				pvAssign(valveval, valvename);{nl}\
				pvMonitor(valveval);{nl}\
{nl}\
				pvAssign(proc, logicname);{nl}\
				pvMonitor(proc);{nl}\
			{close};{nl}\
		{close}state CDOM04_Proc_0{nl}\
	{close}{nl}\
{nl}\
	state CDOM041_Proc_20{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 20 && index < stopindex){nl}\
		{open}{nl}\
			printf("CDOM041_Proc_100\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM041_Proc_100{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 20 && index < stopindex){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
{nl}\
			efSet(efCDOM041_100);{nl}\
		{close}state CDOM041_Init{nl}\
{nl}\
		when(delay(cdDelay) && index >= stopindex){nl}\
		{open}{nl}\
			strcpy(steppv, "OM041 Done & OM032 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efCDOM041_100);{nl}\
{nl}\
		{close}state CDOM041_Init{nl}\
	{close}{nl}\
	state CDOM041_Proc_100{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 100 && index < stopindex){nl}\
		{open}{nl}\
			printf("CDOM041_Proc_100\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM041_Proc_100{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 100 && index < stopindex){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
{nl}\
			efSet(efCDOM041_0);{nl}\
		{close}state CDOM041_Init{nl}\
{nl}\
		when(delay(cdDelay) && index >= stopindex){nl}\
		{open}{nl}\
			strcpy(steppv, "OM041 Done & OM032 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efCDOM041_0);{nl}\
{nl}\
			ss_start = 2;{nl}\
			pvPut(ss_start, SYNC);{nl}\
{nl}\
		{close}state stopCDOM041{nl}\
	{close}{nl}\
{nl}\
	state CDOM04_Proc_0{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval > 0 && index < stopindex_1_2){nl}\
		{open}{nl}\
			printf("CDOM04_Proc_100\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM04_Proc_0{nl}\
{nl}\
		when(delay(cdDelay) && valveval <= 0 && index < stopindex_1_2){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
{nl}\
			efSet(efCDOM041_0);{nl}\
		{close}state CDOM041_Init{nl}\
{nl}\
		when(delay(cdDelay) && index >= stopindex_1_2){nl}\
		{open}{nl}\
			strcpy(steppv, "OM04 Done");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
			ss_start = 2;{nl}\
{nl}\
			pvPut(ss_start, SYNC);{nl}\
		{close}state stopCDOM041{nl}\
	{close}{nl}\
{close}{nl}\
'
seq.write(Text)
seq.close()
