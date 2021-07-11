#!/usr/local/bin/python3.8
import sys
import os
import glob

pvfiles = []

for file in glob.glob1("pv/OM031","*.pv"):
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
    f = open('pv/OM031/'+str(file),'r')
    filename =str(file).rsplit('.', 1)[0]
    for line in f:
        pvname = line.rstrip('\n')
        if(pvname[0] =='#'): continue
        pvlist.append(pvname)
    f.close()
    pvnamelist[filename]=pvlist
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
        if(idx%5 == 0):
            spvname += '",\n'
        elif (idx != idxsize):
            spvname += '",'
        else:
            spvname += '"};\n\n'
        Text += (spvname)
    seq.write(Text)
####Assign Valuet##########

Text =f'{nl}\
int stopindex = {idxsize};{nl}\
string steppv;{nl}\
assign steppv to "{prefix}StepPV";{nl}\
monitor steppv;{nl}\
{nl}\
int start = 0;{nl}\
assign start to "{prefix}Start";{nl}\
monitor start;{nl}\
evflag	efStop;{nl}\
sync start efStop;{nl}\
{nl}\
int index = 0;{nl}\
assign index to "{prefix}IndexPV";{nl}\
monitor index;{nl}\
{nl}\
float om031Val;{nl}\
assign om031Val to "{prefix}Value";{nl}\
monitor om031Val;{nl}\
{nl}\
float cdDelay;{nl}\
assign cdDelay to "{prefix}ScanTime";{nl}\
monitor cdDelay;{nl}\
{nl}\
float	valvewave[107];{nl}\
assign	valvewave to "{prefix}ValveWave";{nl}\
monitor valvewave;{nl}\
{nl}\
//float	valveval[107];{nl}\
char	logicname[60];{nl}\
int proc = 1;{nl}\
assign proc to "";{nl}\
{nl}\
char	valvename[60];{nl}\
float	valveval;{nl}\
assign  valveval to "";{nl}\
{nl}\
evflag	efNext;{nl}\
{nl}\
evflag	efCDOM031_1;{nl}\
evflag	efCDOM031_5;{nl}\
evflag	efCDOM031_100;{nl}\
'
seq.write(Text)

Text = f'{nl}\
ss ssCDOM031{nl}\
{open}{nl}\
	state stopCDOM131{nl}\
	{open}{nl}\
		when(efTestAndClear(efStop) && start == 1){nl}\
		{open}{nl}{nl}\
			index = 0;{nl}\
			pvPut(index, SYNC);{nl}\
			efSet(efCDOM031_1);{nl}\
		{close}state CDOM031_Init{nl}\
	{close}{nl}\
{nl}\
	state CDOM031_Init{nl}\
	{open}{nl}\
		when(delay(cdDelay)&&efTestAndClear(efCDOM031_100)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM031[index]);{nl}\
			printf("%s\\n", logicname);{nl}\
			sprintf(valvename, "%s:Valve", OM031[index]);{nl}\
			printf("%s\\n", valvename);{nl}\
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
			{close}{nl}\
			{nl}\
		{close}state CDOM031_Proc_100{nl}\
		{nl}\
		when(delay(cdDelay)&&efTestAndClear(efCDOM031_1)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM031[index]);{nl}\
			printf("%s\\n", logicname);{nl}\
			sprintf(valvename, "%s:Valve", OM031[index]);{nl}\
			printf("%s\\n", valvename);{nl}\
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
			{close}{nl}\
			{nl}\
		{close}state CDOM031_Proc_1{nl}\
		{nl}\
		when(delay(cdDelay)&&efTestAndClear(efCDOM031_5)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM031[index]);{nl}\
			printf("%s\\n", logicname);{nl}\
			sprintf(valvename, "%s:Valve", OM031[index]);{nl}\
			printf("%s\\n", valvename);{nl}\
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
		{close}state CDOM031_Proc_5{nl}\
	{close}{nl}\
{nl}\
	state CDOM031_Proc_100{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 100 && index < stopindex){nl}\
		{open}{nl}\
			printf("CDOM031_Proc_100\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM031_Proc_100{nl}\
		{nl}\
		when(delay(cdDelay) && valveval == 100 && index != 6 && index < stopindex){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
			{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
			{nl}\
			if(strstr(OM031[index], "XV") != NULL ) {open}{nl}\
				efSet(efCDOM031_1);{nl}\
			{close} else if(index == 6){open}{nl}\
				efSet(efCDOM031_5);{nl}\
			{close} else {open}{nl}\
				efSet(efCDOM031_100);{nl}\
			{close};{nl}\
		{close}state CDOM031_Init{nl}\
		{nl}\
		when(delay(cdDelay) && index >= stopindex){nl}\
		{open}{nl}\
			strcpy(steppv, "CDOM031 Completed");{nl}\
			pvPut(steppv, SYNC);{nl}\
			{nl}\
			efSet(efStop);{nl}\
			start = 0;{nl}\
			pvPut(start, SYNC);{nl}\
			{nl}\
		{close}state stopCDOM131{nl}\
	{close}{nl}\
{nl}\
	state CDOM031_Proc_1{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 1 && index < stopindex){nl}\
		{open}{nl}\
			printf("CDOM031_Proc_1\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM031_Proc_1{nl}\
		{nl}\
		when(delay(cdDelay)&& valveval == 1 && index < stopindex){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
			{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
			{nl}\
			if(strstr(OM031[index], "XV") != NULL ) {open}{nl}\
				efSet(efCDOM031_1);{nl}\
			{close} else if(index == 6){open}{nl}\
				efSet(efCDOM031_5);{nl}\
			{close} else {open}{nl}\
				efSet(efCDOM031_100);{nl}\
			{close};{nl}\
			{nl}\
		{close}state CDOM031_Init{nl}\
		{nl}\
		when(delay(cdDelay) && index >= stopindex){nl}\
		{open}{nl}\
			strcpy(steppv, "CDOM031 Completed");{nl}\
			pvPut(steppv, SYNC);{nl}\
			{nl}\
			efSet(efStop);{nl}\
			start = 0;{nl}\
			pvPut(start, SYNC);{nl}\
			{nl}\
		{close}state stopCDOM131{nl}\
	{close}{nl}\
{nl}\
	state CDOM031_Proc_5{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 5 && index < stopindex){nl}\
		{open}{nl}\
			printf("CDOM031_Proc_5\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM031_Proc_5{nl}\
		{nl}\
		when(delay(cdDelay) && valveval == 5 && index < stopindex){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
			{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
			{nl}\
			if(strstr(OM031[index], "XV") != NULL ) {open}{nl}\
				efSet(efCDOM031_1);{nl}\
			{close} else if(index == 6){open}{nl}\
				efSet(efCDOM031_5);{nl}\
			{close} else {open}{nl}\
				efSet(efCDOM031_100);{nl}\
			{close};{nl}\
			{nl}\
		{close}state CDOM031_Init{nl}\
		{nl}\
		when(delay(cdDelay) && index >= stopindex){nl}\
		{open}{nl}\
			strcpy(steppv, "CDOM031 Completed");{nl}\
			pvPut(steppv, SYNC);{nl}\
			{nl}\
			pvPut(start, SYNC);{nl}\
			efSet(efStop);{nl}\
			start = 0;{nl}\
			{nl}\
		{close}state stopCDOM131{nl}\
	{close}{nl}\
{close}{nl}\
'
seq.write(Text)
seq.close()
