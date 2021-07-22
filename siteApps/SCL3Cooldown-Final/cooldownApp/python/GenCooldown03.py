#!/usr/local/bin/python3.8
import sys
import os
import glob

pvfiles = []

for file in glob.glob1("pv/OM03","*.pv"):
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
seq    = open('../src/'+seqfile, 'w')
seqdbd = open('../src/'+seqdbdfile, 'w')

for idx, file in enumerate(pvfiles):
    f = open('pv/OM03/'+str(file),'r')
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
option +r; {nl}\
option +c; {nl}\
option -a; {nl}{nl}\
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
//int stopindex = 107;{nl}\
int stopindex = {len(OM031)};{nl}\
int stopindex_1_2 = {len(OM036_1_2)};{nl}\
int stopindex_3 = {len(OM036_3)};{nl}\
int stopindex_4 = {len(OM036_4)};{nl}\
int stopindex_5 = {len(OM036_5)};{nl}\
int stopindex_6_7 = {len(OM036_6_7)}; //33+55{nl}{nl}\
//int stopindex_6_7 = 88; //33+55{nl}{nl}\
string steppv;{nl}\
assign steppv to "{prefix}StepPV";{nl}\
monitor steppv;{nl}\
{nl}\
int ss_start = 0;{nl}\
assign ss_start to "{prefix}Start";{nl}\
monitor ss_start;{nl}\
evflag	efStop;{nl}\
sync ss_start  to efStop;{nl}\
{nl}\
int index = 0;{nl}\
assign index to "{prefix}Index";{nl}\
monitor index;{nl}\
{nl}\
float cdDelay;{nl}\
assign cdDelay to "{prefix}StepDly";{nl}\
monitor cdDelay;{nl}\
{nl}\
int presz_start = 0;{nl}\
assign presz_start to "{prefix}PresStart";{nl}\
monitor presz_start;{nl}\
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
evflag	efCDOM031_1;{nl}\
evflag	efCDOM031_5;{nl}\
evflag	efCDOM031_100;{nl}\
{nl}\
evflag	efCDOM032_1;{nl}\
evflag	efCDOM032_100;{nl}\
{nl}\
evflag	efCDOM034_1;{nl}\
evflag	efCDOM034_50;{nl}\
{nl}\
evflag	efCDOM035;{nl}\
{nl}\
evflag	efCDOM036_1_2;{nl}\
evflag	efCDOM036_3;{nl}\
evflag	efCDOM036_4;{nl}\
evflag	efCDOM036_5;{nl}\
evflag	efCDOM036_6_7;{nl}\
evflag	efCDOM036_7;{nl}\
'
seq.write(Text)

Text =f'{nl}\
ss ssCDOM031{nl}\
{open}{nl}\
	state init{nl}\
	{open}{nl}\
		when(TRUE){nl}\
		{open}{nl}\
		  efSet(efStop);{nl}\
		  printf("Started Cooldown Logic Processig...\\n");{nl}\
		{close}state stopCDOM031{nl}\
	{close}{nl}\
{nl}\
	state stopCDOM031{nl}\
	{open}{nl}\
		//when(efTestAndClear(efStop) && ss_start == 1){nl}\
		when(ss_start == 1){nl}\
		{open}{nl}\
			index = 0;{nl}\
			//stopindex = 107;{nl}\
			stopindex = {len(OM031)};{nl}\
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
			sprintf(valvename, "%s:Valve", OM031[index]);{nl}\
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
		{close}state CDOM031_Proc_100{nl}\
{nl}\
		when(delay(cdDelay)&&efTestAndClear(efCDOM031_1)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM031[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM031[index]);{nl}\
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
		{close}state CDOM031_Proc_1{nl}\
{nl}\
		when(delay(cdDelay)&&efTestAndClear(efCDOM031_5)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM031[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM031[index]);{nl}\
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
			strcpy(steppv, "OM031 Done & OM032 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
			ss_start = 2;{nl}\
{nl}\
			pvPut(ss_start, SYNC);{nl}\
{nl}\
		{close}state stopCDOM031{nl}\
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
			strcpy(steppv, "OM031 Done & OM032 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
			ss_start = 2;{nl}\
{nl}\
			pvPut(ss_start, SYNC);{nl}\
{nl}\
		{close}state stopCDOM031{nl}\
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
			strcpy(steppv, "OM031 Done & OM032 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
{nl}\
			ss_start = 2;{nl}\
			pvPut(ss_start, SYNC);{nl}\
{nl}\
		{close}state stopCDOM031{nl}\
	{close}{nl}\
{nl}\
{close}{nl}\
{nl}\
ss ssCDOM032{nl}\
{open}{nl}\
	state stopCDOM032{nl}\
	{open}{nl}\
		when(efTestAndClear(efStop) && ss_start == 2){nl}\
		{open}{nl}\
			index = 0;{nl}\
			//stopindex = 11;{nl}\
			stopindex = {len(OM032)};{nl}\
			pvPut(index, SYNC);{nl}\
			efSet(efCDOM032_1);{nl}\
		{close}state CDOM032_Init{nl}\
	{close}{nl}\
{nl}\
	state CDOM032_Init{nl}\
	{open}{nl}\
		when(delay(cdDelay)&&efTestAndClear(efCDOM032_1)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM032[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM032[index]);{nl}\
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
		{close}state CDOM032_Proc_1{nl}\
{nl}\
		when(delay(cdDelay)&&efTestAndClear(efCDOM032_100)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM032[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM032[index]);{nl}\
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
		{close}state CDOM032_Proc_100{nl}\
{nl}\
	{close}{nl}\
{nl}\
	state CDOM032_Proc_1{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 1 && index < stopindex){nl}\
		{open}{nl}\
			printf("CDOM032_Proc_1\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM032_Proc_1{nl}\
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
			if(strstr(OM032[index], "XV") != NULL ) {open}{nl}\
				efSet(efCDOM032_1);{nl}\
			{close} else {open}{nl}\
				efSet(efCDOM032_100);{nl}\
			{close};{nl}\
{nl}\
		{close}state CDOM032_Init{nl}\
{nl}\
		when(delay(cdDelay) && index >= stopindex){nl}\
		{open}{nl}\
			strcpy(steppv, "OM032 Done & OM033 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
			ss_start = 3;{nl}\
			pvPut(ss_start, SYNC);{nl}\
{nl}\
		{close}state stopCDOM032{nl}\
	{close}{nl}\
{nl}\
	state CDOM032_Proc_100{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 100 && index < stopindex){nl}\
		{open}{nl}\
			printf("CDOM032_Proc_100\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM032_Proc_100{nl}\
{nl}\
		when(delay(cdDelay) && valveval == 100 && index < stopindex){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
{nl}\
{nl}\
			if(strstr(OM032[index], "XV") != NULL ) {open}{nl}\
				efSet(efCDOM032_1);{nl}\
			{close} else {open}{nl}\
				efSet(efCDOM032_100);{nl}\
			{close};{nl}\
		{close}state CDOM032_Init{nl}\
{nl}\
		when(delay(cdDelay) && index >= stopindex){nl}\
		{open}{nl}\
			strcpy(steppv, "OM032 Done & OM033 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
			ss_start = 3;{nl}\
			pvPut(ss_start, SYNC);{nl}\
{nl}\
		{close}state stopCDOM032{nl}\
	{close}{nl}\
{close}{nl}\
{nl}\
ss ssCDOM034{nl}\
{open}{nl}\
	state stopCDOM034{nl}\
	{open}{nl}\
		when(efTestAndClear(efStop) && ss_start == 3 && presz_start == 1){nl}\
		{open}{nl}\
			index = 0;{nl}\
			//stopindex = 11;{nl}\
			stopindex = {len(OM033)};{nl}\
			pvPut(index, SYNC);{nl}\
			efSet(efCDOM034_1);{nl}\
		{close}state CDOM034_Init{nl}\
	{close}{nl}\
{nl}\
	state CDOM034_Init{nl}\
	{open}{nl}\
		when(delay(cdDelay)&&efTestAndClear(efCDOM034_1)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM033[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM033[index]);{nl}\
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
		{close}state CDOM034_Proc_1{nl}\
{nl}\
		when(delay(cdDelay) && efTestAndClear(efCDOM034_50)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM033[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM033[index]);{nl}\
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
		{close}state CDOM034_Proc_50{nl}\
{nl}\
	{close}{nl}\
{nl}\
	state CDOM034_Proc_1{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 1 && index < stopindex){nl}\
		{open}{nl}\
			printf("CDOM034_Proc_1\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM034_Proc_1{nl}\
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
			efSet(efCDOM034_50);{nl}\
{nl}\
		{close}state CDOM034_Init{nl}\
{nl}\
		when(delay(cdDelay) && index >= stopindex){nl}\
		{open}{nl}\
			strcpy(steppv, "OM032 Done & OM033 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
			ss_start = 4;{nl}\
			pvPut(ss_start, SYNC);{nl}\
{nl}\
		{close}state stopCDOM034{nl}\
	{close}{nl}\
{nl}\
	state CDOM034_Proc_50{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 50 && index < stopindex){nl}\
		{open}{nl}\
			printf("CDOM034_Proc_50\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM034_Proc_50{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 50 && index < stopindex){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
{nl}\
		{close}state CDOM034_Init{nl}\
{nl}\
		when(delay(cdDelay) && index >= stopindex){nl}\
		{open}{nl}\
			strcpy(steppv, "OM034 Done & OM035 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
			ss_start = 4;{nl}\
			pvPut(ss_start, SYNC);{nl}\
{nl}\
		{close}state stopCDOM034{nl}\
	{close}{nl}\
{close}{nl}\
{nl}\
ss ssCDOM035{nl}\
{open}{nl}\
	state stopCDOM035{nl}\
	{open}{nl}\
		when(efTestAndClear(efStop) && ss_start == 4){nl}\
		{open}{nl}\
			index = 0;{nl}\
			//stopindex = 2;{nl}\
			stopindex = {len(OM035)};{nl}\
			pvPut(index, SYNC);{nl}\
			efSet(efCDOM035);{nl}\
		{close}state CDOM035_Init{nl}\
	{close}{nl}\
{nl}\
	state CDOM035_Init{nl}\
	{open}{nl}\
		when(delay(cdDelay)&&efTestAndClear(efCDOM035)){nl}\
		{open}{nl}\
			sprintf(logicname,		"%s:CDLogic.PROC", OM033[index]);{nl}\
			sprintf(valvename,      "%s:Valve",        OM033[index]);{nl}\
			sprintf(valve_evalname, "%s:ValveEval",    OM033[index]);{nl}\
			printf("%s, %s, %s\\n", logicname, valvename, valve_evalname);{nl}\
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
{nl}\
				pvAssign(valve_eval, valve_evalname);{nl}\
				pvMonitor(valve_eval);{nl}\
			{close};{nl}\
{nl}\
		{close}state CDOM035_Proc{nl}\
{nl}\
	{close}{nl}\
{nl}\
	state CDOM035_Proc{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valve_eval >=1  && index < stopindex){nl}\
		{open}{nl}\
			printf("CDOM035_Proc\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM035_Proc{nl}\
{nl}\
		when(delay(cdDelay) && valve_eval == 0 && index < stopindex){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
{nl}\
		{close}state CDOM035_Init{nl}\
{nl}\
		when(delay(cdDelay) && index >= stopindex){nl}\
		{open}{nl}\
			strcpy(steppv, "OM035 Done & OM036 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
			ss_start = 5;{nl}\
			pvPut(ss_start, SYNC);{nl}\
{nl}\
		{close}state stopCDOM035{nl}\
	{close}{nl}\
{close}{nl}\
{nl}\
ss ssCDOM036{nl}\
{open}{nl}\
	state stopCDOM036{nl}\
	{open}{nl}\
		when(efTestAndClear(efStop) && ss_start == 5){nl}\
		{open}{nl}\
			index = 0;{nl}\
			//stopindex_1_2 = 121;{nl}\
			stopindex_1_2 = {len(OM036_1_2)};{nl}\
			stopindex_3 = {len(OM036_3)};{nl}\
			stopindex_4 = {len(OM036_4)};{nl}\
			stopindex_5 = {len(OM036_5)};{nl}\
			stopindex_6_7 = {len(OM036_6_7)}; //33+55{nl}{nl}\
			//stopindex_6_7 = 88; //33+55{nl}\
{nl}\
			pvPut(index, SYNC);{nl}\
			efSet(efCDOM036_1_2);{nl}\
		{close}state CDOM036_Init{nl}\
	{close}{nl}\
{nl}\
	state CDOM036_Init{nl}\
	{open}{nl}\
		when(delay(cdDelay) && efTestAndClear(efCDOM036_1_2)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM036_1_2[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM036_1_2[index]);{nl}\
			printf("%s, %s\\n", logicname, valvename);{nl}\
{nl}\
			if(index < stopindex_1_2){nl}\
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
		{close}state CDOM036_1_2_Proc{nl}\
{nl}\
		when(delay(cdDelay) && efTestAndClear(efCDOM036_3)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM036_3[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM036_3[index]);{nl}\
			printf("%s, %s\\n", logicname, valvename);{nl}\
{nl}\
			if(index < stopindex_3){nl}\
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
		{close}state CDOM036_3_Proc{nl}\
{nl}\
		when(delay(cdDelay) && efTestAndClear(efCDOM036_4)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM036_4[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM036_4[index]);{nl}\
			printf("%s, %s\\n", logicname, valvename);{nl}\
{nl}\
			if(index < stopindex_4){nl}\
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
		{close}state CDOM036_4_Proc{nl}\
{nl}\
		when(delay(cdDelay) && efTestAndClear(efCDOM036_5)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM036_5[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM036_5[index]);{nl}\
			printf("%s, %s\\n", logicname, valvename);{nl}\
{nl}\
			if(index < stopindex_5){nl}\
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
		{close}state CDOM036_5_Proc{nl}\
{nl}\
		when(delay(cdDelay) && efTestAndClear(efCDOM036_6_7)){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM036_6_7[index]);{nl}\
			sprintf(valvename, "%s:Valve", OM036_6_7[index]);{nl}\
			printf("%s, %s\\n", logicname, valvename);{nl}\
{nl}\
			if(index < stopindex_6_7){nl}\
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
		{close}state CDOM036_6_7_Proc{nl}\
	{close}{nl}\
{nl}\
	state CDOM036_1_2_Proc{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 100 && index < stopindex_1_2){nl}\
		{open}{nl}\
			printf("CDOM036_1_2_Proc\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM036_1_2_Proc{nl}\
{nl}\
		when(delay(cdDelay) && valveval == 100 && index < stopindex_1_2){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
{nl}\
			efSet(efCDOM036_1_2);{nl}\
		{close}state CDOM036_Init{nl}\
{nl}\
		when(delay(cdDelay) && index >= stopindex_1_2){nl}\
		{open}{nl}\
			strcpy(steppv, "OM036_1_2 Done & OM036_3 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
{nl}\
			ss_start = 6 ;{nl}\
			pvPut(ss_start, SYNC);{nl}\
{nl}\
		{close}state stopCDOM036{nl}\
	{close}{nl}\
{nl}\
	state CDOM036_3_Proc{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 5 && index < stopindex_3){nl}\
		{open}{nl}\
			printf("CDOM036_3_Proc\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM036_3_Proc{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 5 && index < stopindex_3){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
{nl}\
			efSet(efCDOM036_3);{nl}\
		{close}state CDOM036_Init{nl}\
{nl}\
		when(delay(cdDelay) && index >= stopindex_3){nl}\
		{open}{nl}\
			strcpy(steppv, "OM036_3 Done & OM036_4 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
{nl}\
			ss_start = 6 ;{nl}\
			pvPut(ss_start, SYNC);{nl}\
{nl}\
		{close}state stopCDOM036{nl}\
	{close}{nl}\
{nl}\
	state CDOM036_4_Proc{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 50 && index < stopindex_4){nl}\
		{open}{nl}\
			printf("CDOM036_4_Proc\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM036_4_Proc{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 50 && index < stopindex_4){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
{nl}\
			efSet(efCDOM036_4);{nl}\
		{close}state CDOM036_Init{nl}\
{nl}\
		when(delay(cdDelay) && index >= stopindex_4){nl}\
		{open}{nl}\
			strcpy(steppv, "OM036_4 Done & OM036_5 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
{nl}\
			ss_start = 6 ;{nl}\
			pvPut(ss_start, SYNC);{nl}\
{nl}\
		{close}state stopCDOM036{nl}\
	{close}{nl}\
{nl}\
	state CDOM036_5_Proc{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 100 && index < stopindex_5){nl}\
		{open}{nl}\
			printf("CDOM036_5_Proc\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM036_5_Proc{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 100 && index < stopindex_5){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
{nl}\
			efSet(efCDOM036_5);{nl}\
		{close}state CDOM036_Init{nl}\
{nl}\
		when(delay(cdDelay) && index >= stopindex_5){nl}\
		{open}{nl}\
			strcpy(steppv, "OM036_5 Done & OM036_6_7 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
{nl}\
			ss_start = 6 ;{nl}\
			pvPut(ss_start, SYNC);{nl}\
{nl}\
		{close}state stopCDOM036{nl}\
	{close}{nl}\
{nl}\
	state CDOM036_6_7_Proc{nl}\
	{open}{nl}\
		when(delay(cdDelay) && valveval < 50 && index < stopindex_6_7){nl}\
		{open}{nl}\
			printf("CDOM036_6_7_Proc\\n");{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
		{close}state CDOM036_6_7_Proc{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 50 && index < stopindex_6_7){nl}\
		{open}{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvAssign(valveval, "");{nl}\
			pvAssign(proc, "");{nl}\
{nl}\
			index++;{nl}\
			pvPut(index, SYNC);{nl}\
{nl}\
			efSet(efCDOM036_6_7);{nl}\
		{close}state CDOM036_Init{nl}\
{nl}\
		when(delay(cdDelay) && index >= stopindex_6_7){nl}\
		{open}{nl}\
			strcpy(steppv, "OM036_6 Done & OM036_7 Start");{nl}\
			pvPut(steppv, SYNC);{nl}\
{nl}\
			efSet(efStop);{nl}\
{nl}\
			ss_start = 6 ;{nl}\
			pvPut(ss_start, SYNC);{nl}\
		{close}state stopCDOM036{nl}\
	{close}{nl}\
{close}{nl}\
'
seq.write(Text)
seq.close()
