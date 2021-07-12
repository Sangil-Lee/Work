#!/usr/local/bin/python3.8
import sys
import os
import glob

pvfiles = []

for file in glob.glob1("pv/OM12","*.pv"):
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
    f = open('pv/OM12/'+str(file),'r')
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
char cv7401_valveName[40] = "SCL31-CDL01:EBx01-CV7401";{nl}\
char cv7402_valveName[40] = "SCL31-CDL01:EBx01-CV7402";{nl}\
char eh7401_valveName[40] = "SCL31-CDL01:EBx01-EH7401";{nl}\
char lt7401_valveName[40] = "SCL31-CDL01:EBx01-LT7401";{nl}\
{nl}\
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
int		logic_proc = 1;{nl}\
assign	logic_proc to "";{nl}\
{nl}\
char	valvename[60];{nl}\
float	valve_val;{nl}\
assign  valve_val to "";{nl}\
{nl}\
char	valve_evalname[60];{nl}\
float	valve_eval;{nl}\
assign	valve_eval to "";{nl}\
{nl}\
int first = 0;{nl}\
{nl}\
ss ssCDOM12{nl}\
{open}{nl}\
	state stopCDOM12{nl}\
	{open}{nl}\
		when(efTestAndClear(efStop) && ss_start == 1){nl}\
		{open}{nl}\
			index = 0;{nl}\
			pvPut(index, SYNC);{nl}\
		{close}state EBx_CV7401{nl}\
	{close}{nl}\
{nl}\
	state EBx_CV7401{nl}\
	{open}{nl}\
		when(first == 0){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC",	cv7401_valveName);{nl}\
			sprintf(valvename, "%s:Valve",			cv7401_valveName);{nl}\
			printf("%s, %s\\n", logicname, valvename);{nl}\
{nl}\
			pvAssign(valve_val, valvename);{nl}\
			pvMonitor(valve_val);{nl}\
			pvAssign(logic_proc, logicname);{nl}\
			pvMonitor(logic_proc);{nl}\
{nl}\
			first = 1;{nl}\
		{close} state EBx_CV7401{nl}\
{nl}\
		when(delay(cdDelay) && valve_val < 50){nl}\
		{open}{nl}\
			logic_proc = 1;{nl}\
			pvPut(logic_proc, SYNC);{nl}\
			printf("EBx CV7401 Processing...\\n");{nl}\
		{close} state EBx_CV7401{nl}\
{nl}\
		when(delay(cdDelay) && valve_val >= 50){nl}\
		{open}{nl}\
			first = 0;{nl}\
{nl}\
			pvStopMonitor(valve_val);{nl}\
			pvStopMonitor(logic_proc);{nl}\
			pvAssign(valve_val,  "");{nl}\
			pvAssign(logic_proc, "");{nl}\
{nl}\
			valvename[0] = {charnull};{nl}\
			logicname[0] = {charnull};{nl}\
{nl}\
			printf("EBx Transition CV7402.\\n");{nl}\
		{close} state EBx_CV7402{nl}\
{nl}\
	{close}{nl}\
{nl}\
	state EBx_CV7402{nl}\
	{open}{nl}\
		when(first == 0){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC",	cv7402_valveName);{nl}\
			sprintf(valvename, "%s:Valve",			cv7402_valveName);{nl}\
			printf("%s, %s\\n", logicname, valvename);{nl}\
{nl}\
			pvAssign(valve_val, valvename);{nl}\
			pvMonitor(valve_val);{nl}\
			pvAssign(logic_proc, logicname);{nl}\
			pvMonitor(logic_proc);{nl}\
{nl}\
			first = 1;{nl}\
		{close} state EBx_CV7402{nl}\
{nl}\
		when(delay(cdDelay) && valve_val > 0){nl}\
		{open}{nl}\
			logic_proc = 1;{nl}\
			pvPut(logic_proc, SYNC);{nl}\
			printf("EBx CV7402 Processing...\\n");{nl}\
		{close} state EBx_CV7402{nl}\
{nl}\
		when(delay(cdDelay) && valve_val <= 0){nl}\
		{open}{nl}\
			first = 0;{nl}\
{nl}\
			pvStopMonitor(valve_val);{nl}\
			pvStopMonitor(logic_proc);{nl}\
			pvAssign(valve_val,  "");{nl}\
			pvAssign(logic_proc, "");{nl}\
{nl}\
			valvename[0] = {charnull};{nl}\
			logicname[0] = {charnull};{nl}\
{nl}\
			printf("EBx Transition EH7401.\\n");{nl}\
		{close} state EBx_EH7401{nl}\
{nl}\
	{close}{nl}\
{nl}\
	state EBx_EH7401{nl}\
	{open}{nl}\
		when(first == 0){nl}\
		{open}{nl}\
			sprintf(logicname,		"%s:CDLogic.PROC",	eh7401_valveName);{nl}\
			sprintf(valvename,		"%s:Valve",			eh7401_valveName);{nl}\
			sprintf(valve_evalname, "%s:Level",			lt7401_valveName);{nl}\
			printf("%s, %s, %s\\n", logicname, valvename, valve_evalname);{nl}\
{nl}\
			pvAssign(valve_val,  valvename);{nl}\
			pvAssign(logic_proc, logicname);{nl}\
			pvAssign(valve_eval, valve_evalname);{nl}\
{nl}\
			pvMonitor(valve_val);{nl}\
			pvMonitor(logic_proc);{nl}\
			pvMonitor(valve_eval);{nl}\
{nl}\
			first = 1;{nl}\
		{close} state EBx_EH7401{nl}\
{nl}\
		when(delay(cdDelay) && valve_val < 10 && valve_eval >= 50){nl}\
		{open}{nl}\
			logic_proc = 1;{nl}\
			pvPut(logic_proc, SYNC);{nl}\
			printf("EBx EH7401 Processing...\\n");{nl}\
		{close} state EBx_EH7401{nl}\
{nl}\
		when(delay(cdDelay) && valve_val >= 10 && valve_eval < 50){nl}\
		{open}{nl}\
			first = 0;{nl}\
{nl}\
			pvStopMonitor(valve_val);{nl}\
			pvStopMonitor(logic_proc);{nl}\
			pvStopMonitor(valve_eval);{nl}\
			pvAssign(valve_val,  "");{nl}\
			pvAssign(valve_eval,  "");{nl}\
			pvAssign(logic_proc, "");{nl}\
{nl}\
			valvename[0] = {charnull};{nl}\
			logicname[0] = {charnull};{nl}\
{nl}\
{nl}\
			//PLC:PID, SCL31-CDL01:EBx01-CV7401:PID = Turn On(1){nl}\
{nl}\
			printf("EBx Transition Stop.\\n");{nl}\
		{close} state PhaseCond{nl}\
	{close}{nl}\
{nl}\
	state PhaseCond{nl}\
	{open}{nl}\
		when(TRUE){nl}\
		{open}{nl}\
		{close} state stopCDOM12{nl}\
	{close}{nl}\
{nl}\
{close}{nl}\
'
seq.write(Text)
seq.close()
