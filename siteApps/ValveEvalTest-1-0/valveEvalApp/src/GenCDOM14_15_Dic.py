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
seqDBD = '.dbd'
seqfile    = pyname + seqExt
seqdbdfile = pyname + seqDBD
seq    = open(seqfile, 'w')
seqdbd = open(seqdbdfile, 'w')


for idx, file in enumerate(pvfiles):
    f = open('pv/'+str(file),'r')
    filename =str(file).rsplit('.', 1)[0]
    #pvnamelist.append(filename)
    #pvnamelist[idx]=filename
    for line in f:
        pvname = line.rstrip('\n')
        if(pvname[0] =='#'): continue
        pvlist.append(pvname)
    f.close()
    pvnamelist[filename]=pvlist
    #globals()[pvnamelist[idx]] = pvlist
    pvlist = []

print(pvfiles)
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
int cycle = 0; {nl}\
assign cycle to "{prefix}Cycle";{nl}\
monitor cycle;{nl}{nl}\
int index = 0;{nl}\
assign index to "{prefix}IndexPV";{nl}\
monitor index;{nl}{nl}\
float om1415Val;{nl}\
assign om1415Val to "{prefix}Value";{nl}\
monitor om1415Val;{nl}{nl}\
float cdDelay;{nl}\
assign cdDelay to "{prefix}ScanTime";{nl}\
monitor cdDelay;{nl}{nl}\
//Valve Control{nl}\
char    logicname[{charsize}];{nl}\
char    logicname_1[{charsize}];{nl}\
char    logicname_2[{charsize}];{nl}\
int     proc = 1;{nl}\
int     proc_1 = 1;{nl}\
int     proc_2 = 1;{nl}\
assign  proc to "";{nl}\
assign  proc_1 to "";{nl}\
assign  proc_2 to "";{nl}\
{nl}\
char    valvename[{charsize}];{nl}\
char    valvename_1[{charsize}];{nl}\
char    valvename_2[{charsize}];{nl}\
float   valveval;{nl}\
float   valveval_1;{nl}\
float   valveval_2;{nl}\
assign  valveval to "";{nl}\
assign  valveval_1 to "";{nl}\
assign  valveval_2 to "";{nl}\
{nl}\
char    valve_evalname[{charsize}];{nl}\
char    valve_evalname_1[{charsize}];{nl}\
char    logic_valve_evalname[{charsize}];{nl}\
char    logic_valve_evalname_1[{charsize}];{nl}\
char    logic_start[{charsize}];{nl}\
char    logic_start_1[{charsize}];{nl}\
int     valve_eval;{nl}\
int     valve_eval_1;{nl}\
int     valve_eval_proc;{nl}\
int     valve_eval_proc_1;{nl}\
int     logic_start_val;{nl}\
int     logic_start_val_1;{nl}\
{nl}\
assign  valve_eval to "";{nl}\
assign  valve_eval_1 to "";{nl}\
assign  valve_eval_proc to "";{nl}\
assign  valve_eval_proc_1 to "";{nl}\
assign  logic_start_val to "";{nl}\
assign  logic_start_val_1 to "";{nl}\
{nl}\
//PID Control{nl}\
char    pidname[{charsize}];{nl}\
char    pidname_1[{charsize}];{nl}\
char    pidname_2[{charsize}];{nl}\
char    pidname_3[{charsize}];{nl}\
char    levelname[{charsize}];{nl}\
char    levelname_1[{charsize}];{nl}\
char    pressname[{charsize}];{nl}\
char    pressname_1[{charsize}];{nl}\
int     pid_val;{nl}\
int     pid_val_1;{nl}\
int     pid_val_2;{nl}\
int     pid_val_3;{nl}\
int     level_val;{nl}\
int     level_val_1;{nl}\
int     press_val;{nl}\
int     press_val_1;{nl}\
assign  pid_val to "";{nl}\
assign  pid_val_1 to "";{nl}\
assign  pid_val_2 to "";{nl}\
assign  pid_val_3 to "";{nl}\
assign  level_val to "";{nl}\
assign  level_val_1 to "";{nl}\
assign  press_val to "";{nl}\
assign  press_val_1 to "";{nl}\
{nl}\
int first = 0;{nl}\
evflag  efCDOM14;{nl}\
evflag  efCDOM15QWR;{nl}\
evflag  efCDOM15HWR;{nl}\
{nl}\
'
seq.write(Text)

######################################State Set####################################
####State Set-ssCDOM14 ##########
Text =f'{nl}\
ss ssCDOM14{nl}\
{open}{nl}\
    state init{nl}\
    {open}{nl}\
        when(TRUE){nl}\
        {open}{nl}\
            cycle = 0;{nl}\
            index = 0;{nl}\
            pvPut(cycle, SYNC);{nl}\
            pvPut(index, SYNC);{nl}\
        {close}state XV7301_7201{nl}\
    {close}{nl}\
	state Stop{nl}\
	{open}{nl}\
		when(efTestAndClear(efCDOM14)) {nl}\
		{open}{nl}\
        {close}state XV7301_7201{nl}\
    {close}{nl}{nl}\
	//XV7301/XV7201 Regulation{nl}\
	state XV7301_7201{nl}\
	{open}{nl}\
		when(first == 0){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM14_XV7301_7201[cycle]);{nl}\
			printf("%s\\n", logicname);{nl}\
			sprintf(valvename, "%s:Valve", OM14_XV7301_7201[cycle]);{nl}\
			printf("%s\\n", valvename);{nl}\
{nl}\
			pvAssign(valveval, valvename);{nl}\
			pvMonitor(valveval);{nl}\
{nl}\
			pvAssign(proc, logicname);{nl}\
			pvMonitor(proc);{nl}\
{nl}\
			first = 1;{nl}\
		{close}state XV7301_7201{nl}\
{nl}\
		when(delay(cdDelay) && valveval < 1 ){nl}\
		{open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
			printf("XV7301 Processing...\\n");{nl}\
		{close}state XV7301_7201{nl}\
{nl}\
		when(delay(cdDelay) && valveval == 1 ){nl}\
		{open}{nl}\
			first = 0;{nl}\
{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
{nl}\
{nl}\
			printf("Transition CV7401.\\n");{nl}\
		{close}state CV7401{nl}\
    {close}{nl}\
{nl}\
	//CV7401 Regulation{nl}\
	state CV7401{nl}\
	{open}{nl}\
		when(first == 0){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM14_CV7401[cycle]);{nl}\
			printf("%s\\n", logicname);{nl}\
			sprintf(valvename, "%s:Valve", OM14_CV7401[cycle]);{nl}\
			printf("%s\\n", valvename);{nl}\
{nl}\
			pvAssign(valveval, valvename);{nl}\
			pvMonitor(valveval);{nl}\
{nl}\
			pvAssign(proc, logicname);{nl}\
			pvMonitor(proc);{nl}\
{nl}\
			first = 1;{nl}\
		{close}state CV7401{nl}\
{nl}\
		when(delay(cdDelay) && valveval < 100){nl}\
		{open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
			printf("CV7401 Processing...\\n");{nl}\
		{close}state CV7401{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 100){nl}\
		{open}{nl}\
			first = 0;{nl}\
			printf("Transition CV8401.\\n");{nl}\
			pvStopMonitor(valveval);{nl}\
		{close}state CV8401{nl}\
	{close}{nl}\
{nl}\
	//CV8401 Regulation{nl}\
	state CV8401{nl}\
	{open}{nl}\
		when(first == 0){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM14_CV8401[cycle]);{nl}\
			printf("%s\\n", logicname);{nl}\
			sprintf(valvename, "%s:Valve", OM14_CV8401[cycle]);{nl}\
			printf("%s\\n", valvename);{nl}\
{nl}\
			//sprintf(logicname_1, "%s:CDLogic.PROC", OM14_CV8401_1[cycle]);{nl}\
			//printf("%s\\n", logicname_1);{nl}\
			//sprintf(valvename_1, "%s:Valve", OM14_CV8401_1[cycle]);{nl}\
			//printf("%s\\n", valvename_1);{nl}\
{nl}\
			pvAssign(valveval, valvename);{nl}\
			pvMonitor(valveval);{nl}\
{nl}\
			pvAssign(proc, logicname);{nl}\
			pvMonitor(proc);{nl}\
{nl}\
			//pvAssign(valveval_1, valvename_1);{nl}\
			//pvMonitor(valveval_1);{nl}\
{nl}\
			//pvAssign(proc_1, logicname_1);{nl}\
			//pvMonitor(proc_1);{nl}\
{nl}\
			first = 1;{nl}\
		{close}state CV8401{nl}\
{nl}\
		//when(delay(cdDelay) && valveval < 100 && valveval_1 < 100){nl}\
		when(delay(cdDelay) && valveval < 100){nl}\
		{open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
{nl}\
			//proc_1 = 1;{nl}\
			//pvPut(proc_1, SYNC);{nl}\
{nl}\
			printf("CV8401 Processing...\\n");{nl}\
		{close}state CV8401{nl}\
{nl}\
		//when(delay(cdDelay) && valveval >= 100 && valveval_1 >= 100 && Cycle >= 12){nl}\
		when(delay(cdDelay) && valveval >= 100 && cycle >= 12){nl}\
		{open}{nl}\
			printf("CV8401 Complete!!\\n");{nl}\
			first = 0;{nl}\
		{close}state CV8402_5{nl}\
{nl}\
		//when(delay(cdDelay) && valveval >= 100 && valveval_1 >= 100 && Cycle < 12){nl}\
		when(delay(cdDelay) && valveval >= 100 && cycle < 12){nl}\
		{open}{nl}\
			printf("CV8401 Complete!!\\n");{nl}\
			first = 0;{nl}\
		{close}state CV7401_100{nl}\
	{close}{nl}\
{nl}\
	//CV8402_5 Regulation(Only HWR(A/B)/P2DT -> Cycle12 ~ 44){nl}\
	state CV8402_5{nl}\
	{open}{nl}\
		when(first == 0){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM14_CV8402[cycle]);{nl}\
			printf("%s\\n", logicname);{nl}\
			sprintf(valvename, "%s:Valve", OM14_CV8402[cycle]);{nl}\
			printf("%s\\n", valvename);{nl}\
{nl}\
			pvAssign(valveval, valvename);{nl}\
			pvMonitor(valveval);{nl}\
{nl}\
			pvAssign(proc, logicname);{nl}\
			pvMonitor(proc);{nl}\
{nl}\
			first = 1;{nl}\
		{close}state CV8402_5{nl}\
{nl}\
		when(delay(cdDelay) && valveval < 5){nl}\
		{open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
			printf("CV8402_5 Processing...\\n");{nl}\
		{close}state CV8402_5{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 5){nl}\
		{open}{nl}\
			first = 0;{nl}\
			printf("Transition CV8401.\\n");{nl}\
		{close}state CV7401_100{nl}\
	{close}{nl}\
{nl}\
	state CV7401_100{nl}\
	{open}{nl}\
		when(first == 0){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM14_CV7401[cycle]);{nl}\
			printf("%s\\n", logicname);{nl}\
			sprintf(valvename, "%s:Valve", OM14_CV7401[cycle]);{nl}\
			printf("%s\\n", valvename);{nl}\
{nl}\
			pvAssign(valveval, valvename);{nl}\
			pvMonitor(valveval);{nl}\
{nl}\
			pvAssign(proc, logicname);{nl}\
			pvMonitor(proc);{nl}\
{nl}\
			first = 1;{nl}\
		{close}state CV7401_100{nl}\
{nl}\
		when(delay(cdDelay) && valveval < 100){nl}\
		{open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
			printf("CV7401_100 Processing...\\n");{nl}\
		{close}state CV7401_100{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 100 && cycle < 12){nl}\
		{open}{nl}\
			first = 0;{nl}\
			printf("Transition CV8402_5_Regulate.\\n");{nl}\
		{close}state CV8402_5_regulate{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 100 && cycle >= 12){nl}\
		{open}{nl}\
			first = 0;{nl}\
			printf("Transition CV8403_Regulate.\\n");{nl}\
		{close}state CV8403_regulate{nl}\
	{close}{nl}\
{nl}\
	state CV8402_5_regulate{nl}\
	{open}{nl}\
		when(first == 0){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM14_CV8402[cycle]);{nl}\
			printf("%s\\n", logicname);{nl}\
			sprintf(valvename, "%s:Valve", OM14_CV8402[cycle]);{nl}\
			printf("%s\\n", valvename);{nl}\
{nl}\
			sprintf(valve_evalname, "%s:ValveEval", OM14_CV8402[cycle]);{nl}\
			printf("%s\\n", valve_evalname);{nl}\
{nl}\
			sprintf(logic_valve_evalname, "%s:ValveEval.PROC", OM14_CV8402[cycle]);{nl}\
			sprintf(logic_start, "%s:Start", OM14_CV8402[cycle]);{nl}\
{nl}\
			pvAssign(valveval, valvename);{nl}\
			pvMonitor(valveval);{nl}\
{nl}\
			pvAssign(proc, logicname);{nl}\
			pvMonitor(proc);{nl}\
{nl}\
			pvAssign(valve_eval, valve_evalname);{nl}\
			pvMonitor(valve_eval);{nl}\
{nl}\
			pvAssign(valve_eval_proc, logic_valve_evalname);{nl}\
			pvMonitor(valve_eval_proc);{nl}\
{nl}\
			pvAssign(logic_start_val, logic_start);{nl}\
			pvMonitor(logic_start_val);{nl}\
{nl}\
			first = 1;{nl}\
		{close}state CV8402_5_regulate{nl}\
{nl}\
		when(delay(cdDelay) && valve_eval >= 1){nl}\
		{open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
{nl}\
			valve_eval_proc = 1;{nl}\
			pvPut(valve_eval_proc, SYNC);{nl}\
{nl}\
			logic_start_val = 1;{nl}\
			pvPut(logic_start_val, SYNC);{nl}\
{nl}\
			printf("CV8402_5_regulate Processing...\\n");{nl}\
		{close}state CV8402_5_regulate{nl}\
{nl}\
		when(delay(cdDelay) && valve_eval == 0){nl}\
		{open}{nl}\
			first = 0;{nl}\
			printf("Transition PhaseEnd 14.\\n");{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvStopMonitor(valve_eval);{nl}\
			pvStopMonitor(valve_eval_proc);{nl}\
			pvStopMonitor(logic_start_val);{nl}\
		{close}state PhaseEnd14{nl}\
	{close}{nl}\
{nl}\
	state CV8403_regulate{nl}\
	{open}{nl}\
		when(first == 0){nl}\
		{open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM14_CV8403[cycle]);{nl}\
			printf("%s\\n", logicname);{nl}\
			sprintf(valvename, "%s:Valve", OM14_CV8403[cycle]);{nl}\
			printf("%s\\n", valvename);{nl}\
{nl}\
			sprintf(valve_evalname, "%s:ValveEval", OM14_CV8403[cycle]);{nl}\
			printf("%s\\n", valve_evalname);{nl}\
{nl}\
			sprintf(logic_valve_evalname, "%s:ValveEval.PROC", OM14_CV8403[cycle]);{nl}\
			sprintf(logic_start, "%s:Start", OM14_CV8403[cycle]);{nl}\
{nl}\
			pvAssign(valveval, valvename);{nl}\
			pvMonitor(valveval);{nl}\
{nl}\
			pvAssign(proc, logicname);{nl}\
			pvMonitor(proc);{nl}\
{nl}\
			pvAssign(valve_eval, valve_evalname);{nl}\
			pvMonitor(valve_eval);{nl}\
{nl}\
			pvAssign(valve_eval_proc, logic_valve_evalname);{nl}\
			pvMonitor(valve_eval_proc);{nl}\
{nl}\
			pvAssign(logic_start_val, logic_start);{nl}\
			pvMonitor(logic_start_val);{nl}\
{nl}\
			first = 1;{nl}\
		{close}state CV8403_regulate{nl}\
{nl}\
		when(delay(cdDelay) && valveval < 1){nl}\
		{open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
{nl}\
			valve_eval_proc = 1;{nl}\
			pvPut(valve_eval_proc, SYNC);{nl}\
{nl}\
			logic_start_val = 1;{nl}\
			pvPut(logic_start_val, SYNC);{nl}\
			printf("CV8403 Regulate Processing...\\n");{nl}\
		{close}state CV8403_regulate{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 1){nl}\
		{open}{nl}\
			first = 0;{nl}\
			printf("Transition PhaseEnd 14.\\n");{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			pvStopMonitor(valve_eval);{nl}\
			pvStopMonitor(valve_eval_proc);{nl}\
			pvStopMonitor(logic_start_val);{nl}\
		{close}state PhaseEnd14{nl}\
	{close}{nl}\
{nl}\
	state PhaseEnd14{nl}\
	{open}{nl}\
		when(first == 0){nl}\
		{open}{nl}\
			sprintf(valvename, "%s:Level", OM14_CV7401[cycle]);{nl}\
			printf("%s\\n", valvename);{nl}\
{nl}\
			pvAssign(valveval, valvename);{nl}\
			pvMonitor(valveval);{nl}\
{nl}\
			first = 1;{nl}\
		{close}state PhaseEnd14{nl}\
{nl}\
		when(delay(cdDelay) && valveval < 50){nl}\
		{open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
			printf("PhaseEnd 14 Processing...\\n");{nl}\
		{close}state PhaseEnd14{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 50){nl}\
		{open}{nl}\
			first = 0;{nl}\
			printf("Transition CV8401.\\n");{nl}\
			pvStopMonitor(valveval);{nl}\
{nl}\
			efSet(efCDOM15QWR);{nl}\
{nl}\
		{close}state Stop{nl}\
	{close}{nl}\
{close}{nl}\
'
####State Set-ssCDOM15 ##########
seq.write(Text)
Text =f'{nl}\
ss ssCDOM15QWR{nl}\
{open}{nl}\
	state init{nl}\
    {open}{nl}\
		when(TRUE){nl}\
                {open}{nl}\
                {close}state CV8402{nl}\
	{close}{nl}\
{nl}\
	state Stop{nl}\
    {open}{nl}\
		when(delay(cdDelay) && efTestAndClear(efCDOM15QWR)){nl}\
                {open}{nl}\
                {close}state CV8402{nl}\
	{close}{nl}\
{nl}\
	state CV8402{nl}\
    {open}{nl}\
		when(first == 0){nl}\
        {open}{nl}\
			sprintf(logicname,   "%s:CDLogic.PROC", OM15_CV8402[cycle]);{nl}\
			sprintf(logicname_1, "%s:CDLogic.PROC", OM15_CV8402_1[cycle]);{nl}\
			printf("%s, %s\\n", logicname, logicname_1);{nl}\
{nl}\
			pvAssign(proc, logicname);{nl}\
			pvMonitor(proc);{nl}\
			pvAssign(proc_1, logicname_1);{nl}\
			pvMonitor(proc_1);{nl}\
{nl}\
			sprintf(valvename,   "%s:Valve",        OM15_CV8402[cycle]);{nl}\
			sprintf(valvename_1, "%s:Valve",        OM15_CV8402_1[cycle]);{nl}\
			printf("%s, %s\\n", valvename, valvename_1);{nl}\
{nl}\
			pvAssign(valveval, valvename);{nl}\
			pvMonitor(valveval);{nl}\
			pvAssign(valveval_1, valvename_1);{nl}\
			pvMonitor(valveval_1);{nl}\
{nl}\
			sprintf(valve_evalname,   "%s:ValveEval", OM15_CV8402[cycle]);{nl}\
			sprintf(valve_evalname_1, "%s:ValveEval", OM15_CV8402_1[cycle]);{nl}\
			printf("%s, %s\\n", valve_evalname, valve_evalname_1);{nl}\
{nl}\
			pvAssign(valve_eval, valve_evalname);{nl}\
			pvMonitor(valve_eval);{nl}\
			pvAssign(valve_eval_1, valve_evalname_1);{nl}\
			pvMonitor(valve_eval_1);{nl}\
{nl}\
			sprintf(logic_valve_evalname,   "%s:ValveEval.PROC", OM15_CV8402[cycle]);{nl}\
			sprintf(logic_valve_evalname_1, "%s:ValveEval.PROC", OM15_CV8402_1[cycle]);{nl}\
{nl}\
			pvAssign(valve_eval_proc, logic_valve_evalname);{nl}\
			pvMonitor(valve_eval_proc);{nl}\
			pvAssign(valve_eval_proc_1, logic_valve_evalname_1);{nl}\
			pvMonitor(valve_eval_proc_1);{nl}\
{nl}\
			sprintf(logic_start,            "%s:Start",          OM15_CV8402[cycle]);{nl}\
			sprintf(logic_start_1,          "%s:Start",          OM15_CV8402_1[cycle]);{nl}\
{nl}\
			pvAssign(logic_start_val, logic_start);{nl}\
			pvMonitor(logic_start_val);{nl}\
			pvAssign(logic_start_val_1, logic_start_1);{nl}\
			pvMonitor(logic_start_val_1);{nl}\
{nl}\
			first = 1;{nl}\
        {close}state CV8402{nl}\
{nl}\
		when(delay(cdDelay) && valve_eval >= 1 && valve_eval_1 >= 1){nl}\
        {open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
			proc_1 = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
{nl}\
			valve_eval_proc = 1;{nl}\
			pvPut(valve_eval_proc, SYNC);{nl}\
			valve_eval_proc_1 = 1;{nl}\
			pvPut(valve_eval_proc_1, SYNC);{nl}\
{nl}\
			logic_start_val = 1;{nl}\
			pvPut(logic_start_val, SYNC);{nl}\
			logic_start_val_1 = 1;{nl}\
			pvPut(logic_start_val_1, SYNC);{nl}\
{nl}\
			printf("OM15 QWR CV8402 Processing...\\n");{nl}\
        {close}state CV8402{nl}\
{nl}\
		when(delay(cdDelay) && valve_eval == 0 && valve_eval_1 == 0){nl}\
        {open}{nl}\
			first = 0;{nl}\
			printf("OM15 QWR Transition CV8401.\\n");{nl}\
			pvStopMonitor(proc);{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(valve_eval);{nl}\
			pvStopMonitor(valve_eval_proc);{nl}\
			pvStopMonitor(logic_start_val);{nl}\
{nl}\
			pvStopMonitor(proc_1);{nl}\
			pvStopMonitor(valveval_1);{nl}\
			pvStopMonitor(valve_eval_1);{nl}\
			pvStopMonitor(valve_eval_proc_1);{nl}\
			pvStopMonitor(logic_start_val_1);{nl}\
        {close}state CV8401{nl}\
	{close}{nl}\
	{nl}\
	state CV8401{nl}\
    {open}{nl}\
		when(first == 0){nl}\
        {open}{nl}\
			sprintf(logicname,   "%s:CDLogic.PROC", OM15_CV8402[cycle]);{nl}\
			sprintf(logicname_1, "%s:CDLogic.PROC", OM15_CV8402_1[cycle]);{nl}\
			printf("%s, %s\\n", logicname, logicname_1);{nl}\
{nl}\
			pvAssign(proc, logicname);{nl}\
			pvMonitor(proc);{nl}\
			pvAssign(proc_1, logicname_1);{nl}\
			pvMonitor(proc_1);{nl}\
{nl}\
			sprintf(valvename,   "%s:Valve",        OM15_CV8402[cycle]);{nl}\
			sprintf(valvename_1, "%s:Valve",        OM15_CV8402_1[cycle]);{nl}\
			printf("%s, %s\\n", valvename, valvename_1);{nl}\
{nl}\
			pvAssign(valveval, valvename);{nl}\
			pvMonitor(valveval);{nl}\
			pvAssign(valveval_1, valvename_1);{nl}\
			pvMonitor(valveval_1);{nl}\
{nl}\
			sprintf(valve_evalname,   "%s:ValveEval", OM15_CV8402[cycle]);{nl}\
			sprintf(valve_evalname_1, "%s:ValveEval", OM15_CV8402_1[cycle]);{nl}\
			printf("%s, %s\\n", valve_evalname, valve_evalname_1);{nl}\
{nl}\
			pvAssign(valve_eval, valve_evalname);{nl}\
			pvMonitor(valve_eval);{nl}\
			pvAssign(valve_eval_1, valve_evalname_1);{nl}\
			pvMonitor(valve_eval_1);{nl}\
{nl}\
			sprintf(logic_valve_evalname,   "%s:ValveEval.PROC", OM15_CV8402[cycle]);{nl}\
			sprintf(logic_valve_evalname_1, "%s:ValveEval.PROC", OM15_CV8402_1[cycle]);{nl}\
{nl}\
			pvAssign(valve_eval_proc, logic_valve_evalname);{nl}\
			pvMonitor(valve_eval_proc);{nl}\
			pvAssign(valve_eval_proc_1, logic_valve_evalname_1);{nl}\
			pvMonitor(valve_eval_proc_1);{nl}\
{nl}\
			sprintf(logic_start,            "%s:Start",          OM15_CV8402[cycle]);{nl}\
			sprintf(logic_start_1,          "%s:Start",          OM15_CV8402_1[cycle]);{nl}\
{nl}\
			pvAssign(logic_start_val, logic_start);{nl}\
			pvMonitor(logic_start_val);{nl}\
			pvAssign(logic_start_val_1, logic_start_1);{nl}\
			pvMonitor(logic_start_val_1);{nl}\
{nl}\
			first = 1;{nl}\
        {close}state CV8401{nl}\
{nl}\
		when(delay(cdDelay) && valve_eval >= 1){nl}\
        {open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
			proc_1 = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
{nl}\
			valve_eval_proc = 1;{nl}\
			pvPut(valve_eval_proc, SYNC);{nl}\
			valve_eval_proc_1 = 1;{nl}\
			pvPut(valve_eval_proc_1, SYNC);{nl}\
{nl}\
			logic_start_val = 1;{nl}\
			pvPut(logic_start_val, SYNC);{nl}\
			logic_start_val_1 = 1;{nl}\
			pvPut(logic_start_val_1, SYNC);{nl}\
{nl}\
			printf("OM15 QWR CV8401 Processing...\\n");{nl}\
{nl}\
        {close}state CV8401{nl}\
{nl}\
		when(delay(cdDelay) && valve_eval == 0){nl}\
         {open}{nl}\
			first = 0;{nl}\
			printf("OM15 QWR Transition CV7301.\\n");{nl}\
			pvStopMonitor(proc);{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(valve_eval);{nl}\
			pvStopMonitor(valve_eval_proc);{nl}\
			pvStopMonitor(logic_start_val);{nl}\
{nl}\
			pvStopMonitor(proc_1);{nl}\
			pvStopMonitor(valveval_1);{nl}\
			pvStopMonitor(valve_eval_1);{nl}\
			pvStopMonitor(valve_eval_proc_1);{nl}\
			pvStopMonitor(logic_start_val_1);{nl}\
        {close}state CV7301_5{nl}\
	{close}{nl}\
{nl}\
	state CV7301_5{nl}\
    {open}{nl}\
		when(first == 0){nl}\
        {open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM15_CV7301[cycle]);{nl}\
			sprintf(valvename, "%s:Valve", OM15_CV7301[cycle]);{nl}\
			printf("%s, %s\\n", logicname, valvename);{nl}\
{nl}\
			pvAssign(valveval, valvename);{nl}\
			pvAssign(proc, logicname);{nl}\
{nl}\
			pvMonitor(valveval);{nl}\
			pvMonitor(proc);{nl}\
{nl}\
			first = 1;{nl}\
        {close}state CV7301_5{nl}\
{nl}\
		when(delay(cdDelay) && valveval < 5){nl}\
        {open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
			printf("OM15 QWR CV7301_5 Processing...\\n");{nl}\
        {close}state CV7301_5{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 5){nl}\
        {open}{nl}\
			first = 0;{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			printf("OM15 QWR Transition XV7301.\\n");{nl}\
        {close}state XV7301{nl}\
	{close}{nl}\
{nl}\
	state XV7301{nl}\
    {open}{nl}\
		when(first == 0){nl}\
        {open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM15_CV7301[cycle]);{nl}\
			sprintf(valvename, "%s:Valve",		  OM15_CV7301[cycle]);{nl}\
			printf("%s, %s\\n", logicname, valvename);{nl}\
{nl}\
			pvAssign(valveval, valvename);{nl}\
			pvAssign(proc, logicname);{nl}\
			pvMonitor(valveval);{nl}\
			pvMonitor(proc);{nl}\
{nl}\
			first = 1;{nl}\
        {close}state XV7301{nl}\
{nl}\
		when(delay(cdDelay) && valveval < 1 ){nl}\
        {open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
			printf("OM15 QWR XV7301 Processing...\\n");{nl}\
        {close}state XV7301{nl}\
{nl}\
		when(delay(cdDelay) && valveval == 1 ){nl}\
        {open}{nl}\
			first = 0;{nl}\
{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
{nl}\
			valvename[0] = {charnull};{nl}\
			logicname[0] = {charnull};{nl}\
{nl}\
			printf("OM15 QWR Transition CV7301.\\n");{nl}\
        {close}state CV7301_100{nl}\
	{close}{nl}\
{nl}\
	state CV7301_100{nl}\
    {open}{nl}\
		when(first == 0){nl}\
        {open}{nl}\
			sprintf(logicname, "%s:CDLogic.PROC", OM14_XV7301_7201[cycle]);{nl}\
			sprintf(valvename, "%s:Valve",		  OM14_XV7301_7201[cycle]);{nl}\
			printf("%s, %s\\n", logicname, valvename);{nl}\
{nl}\
			pvAssign(valveval, valvename);{nl}\
			pvAssign(proc, logicname);{nl}\
{nl}\
			pvMonitor(valveval);{nl}\
			pvMonitor(proc);{nl}\
{nl}\
			first = 1;{nl}\
        {close}state CV7301_100{nl}\
{nl}\
		when(delay(cdDelay) && valveval < 50){nl}\
        {open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
			printf("OM15 QWR CV7301_5 Processing...\\n");{nl}\
        {close}state CV7301_100{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 50){nl}\
        {open}{nl}\
			first = 0;{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(proc);{nl}\
			printf("OM15 QWR Transition XV7301.\\n");{nl}\
        {close}state PhaseEndQWR{nl}\
	{close}{nl}\
{nl}\
	state PhaseEndQWR{nl}\
    {open}{nl}\
		when(first == 0){nl}\
        {open}{nl}\
			//PLC PID Control PV{nl}\
			//OM15_CV8401;{nl}\
			//OM15_CV8401_1;{nl}\
			//OM15_CV7301;{nl}\
			sprintf(pidname,   "%s:PID", OM15_CV8401[cycle]);{nl}\
			sprintf(pidname_1, "%s:PID", OM15_CV8401_1[cycle]);{nl}\
			sprintf(pidname_2, "%s:PID", OM15_CV7301[cycle]);{nl}\
			printf("%s, %s, %s\\n", pidname, pidname_1, pidname_2);{nl}\
{nl}\
			pvAssign(pid_val,   pidname);{nl}\
			pvAssign(pid_val_1, pidname_1);{nl}\
			pvAssign(pid_val_2, pidname_2);{nl}\
			pvMonitor(pid_val);{nl}\
			pvMonitor(pid_val_1);{nl}\
			pvMonitor(pid_val_2);{nl}\
				{nl}\
			//Level >= 70%{nl}\
			//OM15_PhaseEnd;{nl}\
			//OM15_PhaseEnd_1;{nl}\
			sprintf(levelname,   "%s:Level", OM15_PhaseEnd[cycle]);{nl}\
			sprintf(levelname_1, "%s:Level", OM15_PhaseEnd_1[cycle]);{nl}\
			printf("%s, %s\\n", levelname, levelname_1);{nl}\
{nl}\
			pvAssign(level_val,   levelname);{nl}\
			pvAssign(level_val_1, levelname_1);{nl}\
			pvMonitor(level_val);{nl}\
			pvMonitor(level_val_1);{nl}\
{nl}\
			first = 1;{nl}\
        {close}state PhaseEndQWR{nl}\
{nl}\
		when(delay(cdDelay) && level_val < 70 && level_val_1 < 70){nl}\
        {open}{nl}\
			printf("OM15 QWR Phase End(QWR) Processing...\\n");{nl}\
        {close}state PhaseEndQWR{nl}\
{nl}\
		when(delay(cdDelay) && level_val >= 70 && level_val_1 >= 70){nl}\
        {open}{nl}\
			printf("OM15 QWR Phase End(QWR) Processing...\\n");{nl}\
			//PLC PID Control{nl}\
			pid_val = pid_val_1 = pid_val_2 = 1;{nl}\
			pvPut(pid_val, SYNC); pvPut(pid_val_1, SYNC); pvPut(pid_val_2, SYNC);{nl}\
        {close}state Stop{nl}\
	{close}{nl}\
{close}{nl}\
'
seq.write(Text)

Text =f'{nl}\
ss ssCDOM15HWRP2DT{nl}\
{open}{nl}\
	state init{nl}\
    {open}{nl}\
		when(TRUE){nl}\
        {open}{nl}\
        {close}state CV7201_7301{nl}\
    {close}{nl}\
{nl}\
	state Stop{nl}\
    {open}{nl}\
		when(delay(cdDelay) && efTestAndClear(efCDOM15HWR)){nl}\
        {open}{nl}\
        {close}state CV7201_7301{nl}\
    {close}{nl}\
{nl}\
	state CV7201_7301{nl}\
    {open}{nl}\
		when(first == 0){nl}\
        {open}{nl}\
			sprintf(logicname,   "%s:CDLogic.PROC", OM15_CV7201_HWR[cycle]);{nl}\
			sprintf(logicname_1, "%s:CDLogic.PROC", OM15_CV7301_HWR[cycle]);{nl}\
			printf("%s, %s\\n", logicname, logicname_1);{nl}\
{nl}\
			pvAssign(proc,   logicname); {nl}\
			pvAssign(proc_1, logicname_1);{nl}\
			pvMonitor(proc);			{nl}\
			pvMonitor(proc_1);{nl}\
{nl}\
			sprintf(valvename,   "%s:Valve",		  OM15_CV7201_HWR[cycle]);{nl}\
			sprintf(valvename_1, "%s:Valve",		  OM15_CV7301_HWR[cycle]);{nl}\
{nl}\
			pvAssign(valveval,   valvename); {nl}\
			pvAssign(valveval_1, valvename_1);{nl}\
			pvMonitor(valveval);			{nl}\
			pvMonitor(valveval_1);{nl}\
{nl}\
			first = 1;{nl}\
        {close}state CV7201_7301{nl}\
{nl}\
		when(delay(cdDelay) && valveval < 5 && valveval < 5){nl}\
        {open}{nl}\
			proc = proc_1 = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
			pvPut(proc_1, SYNC);{nl}\
			printf("OM15 HWR CV7201_7301 Processing...\\n");{nl}\
        {close}state CV7201_7301{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 5 && valveval >= 5){nl}\
        {open}{nl}\
			first = 0;{nl}\
			pvStopMonitor(proc);{nl}\
			pvStopMonitor(proc_1);{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(valveval_1);{nl}\
			printf("OM15 HWR Transition CV8303_8401_8402.\\n");{nl}\
{nl}\
        {close}state CV8403_8401_8402{nl}\
    {close}{nl}\
{nl}\
	state CV8403_8401_8402{nl}\
    {open}{nl}\
		when(first == 0){nl}\
        {open}{nl}\
			sprintf(logicname,   "%s:CDLogic.PROC", OM15_CV8403_HWR[cycle]);{nl}\
			sprintf(logicname_1, "%s:CDLogic.PROC", OM15_CV8401_HWR[cycle]);{nl}\
			sprintf(logicname_2, "%s:CDLogic.PROC", OM15_CV8402_HWR[cycle]);{nl}\
			printf("%s, %s, %s\\n", logicname, logicname_1, logicname_2);{nl}\
{nl}\
			pvAssign(proc,   logicname); {nl}\
			pvAssign(proc_1, logicname_1);{nl}\
			pvAssign(proc_2, logicname_1);{nl}\
			pvMonitor(proc);			{nl}\
			pvMonitor(proc_1);{nl}\
			pvMonitor(proc_2);{nl}\
{nl}\
			sprintf(valvename,   "%s:Valve", OM15_CV8403_HWR[cycle]);{nl}\
			sprintf(valvename_1, "%s:Valve", OM15_CV8401_HWR[cycle]);{nl}\
			sprintf(valvename_2, "%s:Valve", OM15_CV8402_HWR[cycle]);{nl}\
{nl}\
			pvAssign(valveval,   valvename); {nl}\
			pvAssign(valveval_1, valvename_1);{nl}\
			pvAssign(valveval_2, valvename_1);{nl}\
			pvMonitor(valveval);			{nl}\
			pvMonitor(valveval_1);{nl}\
			pvMonitor(valveval_2);{nl}\
{nl}\
			sprintf(valve_evalname,   "%s:ValveEval", OM15_CV8401_HWR[cycle]);{nl}\
			sprintf(valve_evalname_1, "%s:ValveEval", OM15_CV8402_HWR[cycle]);{nl}\
			printf("%s, %s\\n", valve_evalname, valve_evalname_1);{nl}\
{nl}\
			pvAssign(valve_eval,   valve_evalname);{nl}\
			pvAssign(valve_eval_1, valve_evalname_1);{nl}\
			pvMonitor(valve_eval);{nl}\
			pvMonitor(valve_eval_1);{nl}\
{nl}\
			sprintf(logic_start,       "%s:Start",   OM15_CV8401_HWR[cycle]);{nl}\
			sprintf(logic_start_1,     "%s:Start",   OM15_CV8402_HWR[cycle]);{nl}\
{nl}\
			pvAssign(logic_start_val,   logic_start);{nl}\
			pvAssign(logic_start_val_1, logic_start_1);{nl}\
			pvMonitor(logic_start_val);{nl}\
			pvMonitor(logic_start_val_1);{nl}\
{nl}\
			first = 1;{nl}\
        {close}state CV8403_8401_8402{nl}\
{nl}\
		when(delay(cdDelay) && valve_eval >= 1 && valve_eval_1 >= 1){nl}\
        {open}{nl}\
			proc = proc_1 = proc_2 = 1;{nl}\
			logic_start_val = logic_start_val_1 = 1;{nl}\
{nl}\
			pvPut(proc, SYNC);{nl}\
			pvPut(proc_1, SYNC);{nl}\
			pvPut(proc_2, SYNC);{nl}\
			pvPut(logic_start_val, SYNC);{nl}\
			pvPut(logic_start_val_1, SYNC);{nl}\
			printf("OM15 HWR CV8403_8401_8402 Processing...\\n");{nl}\
{nl}\
        {close}state CV8403_8401_8402{nl}\
{nl}\
		when(delay(cdDelay) && valve_eval == 0 && valve_eval_1 == 0){nl}\
        {open}{nl}\
			first = 0;{nl}\
			pvStopMonitor(proc);{nl}\
			pvStopMonitor(proc_1);{nl}\
			pvStopMonitor(proc_2);{nl}\
			pvStopMonitor(valveval);{nl}\
			pvStopMonitor(valveval_1);{nl}\
			pvStopMonitor(valveval_2);{nl}\
			pvStopMonitor(valve_eval);{nl}\
			pvStopMonitor(valve_eval_1);{nl}\
			pvStopMonitor(logic_start_val);{nl}\
			pvStopMonitor(logic_start_val_1);{nl}\
{nl}\
			pvAssign(proc,   ""); {nl}\
			pvAssign(proc_1, "");{nl}\
			pvAssign(proc_2, "");{nl}\
			pvAssign(valveval,   ""); {nl}\
			pvAssign(valveval_1, "");{nl}\
			pvAssign(valveval_2, "");{nl}\
			pvAssign(valve_eval,   "");{nl}\
			pvAssign(valve_eval_1, "");{nl}\
			pvAssign(logic_start_val,   "");{nl}\
			pvAssign(logic_start_val_1, "");{nl}\
			printf("OM15 HWR Transition CV7201_50_XV7201.\\n");{nl}\
                        {close}state CV7201_50_XV7201{nl}\
        {close}{nl}\
{nl}\
	state CV7201_50_XV7201{nl}\
    {open}{nl}\
		when(first == 0){nl}\
        {open}{nl}\
			sprintf(logicname,   "%s:CDLogic.PROC", OM15_CV7201_HWR[cycle]);{nl}\
			sprintf(logicname_1, "%s:CDLogic.PROC", OM15_XV7201_HWR[cycle]);{nl}\
			printf("%s, %s\\n", logicname, logicname_1);{nl}\
{nl}\
			pvAssign(proc,   logicname); {nl}\
			pvAssign(proc_1, logicname_1);{nl}\
			pvMonitor(proc);			{nl}\
			pvMonitor(proc_1);{nl}\
{nl}\
			sprintf(valvename,   "%s:Valve", OM15_CV7201_HWR[cycle]);{nl}\
			sprintf(valvename_1, "%s:Valve", OM15_XV7201_HWR[cycle]);{nl}\
{nl}\
			pvAssign(valveval,   valvename); {nl}\
			pvAssign(valveval_1, valvename_1);{nl}\
			pvMonitor(valveval);			{nl}\
			pvMonitor(valveval_1);{nl}\
{nl}\
			first = 1;{nl}\
        {close}state CV7201_50_XV7201{nl}\
{nl}\
		when(delay(cdDelay) && valveval < 50 && valveval_1 !=0 ){nl}\
        {open}{nl}\
			proc = proc_1 = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
			pvPut(proc_1, SYNC);{nl}\
			printf("OM15 HWR CV7201_50_XV7201 Processing...\\n");{nl}\
{nl}\
        {close}state CV7201_50_XV7201{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 50 && valveval_1 == 0 ){nl}\
        {open}{nl}\
			first = 0;{nl}\
			pvStopMonitor(proc);			{nl}\
			pvStopMonitor(proc_1);{nl}\
			pvStopMonitor(valveval);			{nl}\
			pvStopMonitor(valveval_1);{nl}\
			pvAssign(valveval,   ""); {nl}\
			pvAssign(valveval_1, "");{nl}\
			pvAssign(proc,   ""); {nl}\
			pvAssign(proc_1, "");{nl}\
{nl}\
			printf("OM15 HWR Transition CV7301_50.\\n");{nl}\
                        {close}state CV7301_50{nl}\
        {close}{nl}\
{nl}\
	state CV7301_50{nl}\
    {open}{nl}\
		when(first == 0){nl}\
        {open}{nl}\
			sprintf(logicname,   "%s:CDLogic.PROC", OM15_CV7301_HWR[cycle]);{nl}\
			printf("%s\\n", logicname);{nl}\
			pvAssign(proc,   logicname); {nl}\
			pvMonitor(proc);			{nl}\
{nl}\
			sprintf(valvename,   "%s:Valve", OM15_CV7301_HWR[cycle]);{nl}\
			pvAssign(valveval,   valvename); {nl}\
			pvMonitor(valveval);			{nl}\
{nl}\
			first = 1;{nl}\
        {close}state CV7301_50{nl}\
{nl}\
		when(delay(cdDelay) && valveval < 50 ){nl}\
        {open}{nl}\
			proc = 1;{nl}\
			pvPut(proc, SYNC);{nl}\
{nl}\
			printf("OM15 HWR CV7201_50_XV7201 Processing...\\n");{nl}\
        {close}state CV7301_50{nl}\
{nl}\
		when(delay(cdDelay) && valveval >= 50 ){nl}\
        {open}{nl}\
			pvStopMonitor(valveval);			{nl}\
			pvStopMonitor(proc);			{nl}\
			pvAssign(proc,    ""); {nl}\
			pvAssign(valveval,""); {nl}\
			first = 0;{nl}\
			printf("OM15 HWR Transition PhaseEndHWR.\\n");{nl}\
        {close}state PhaseEndHWR{nl}\
    {close}{nl}\
{nl}\
	state PhaseEndHWR{nl}\
    {open}{nl}\
		when(first == 0){nl}\
        {open}{nl}\
			first = 1;{nl}\
{nl}\
			sprintf(pidname,   "%s:PID", OM15_CV8401_HWR[cycle]);{nl}\
			sprintf(pidname_1, "%s:PID", OM15_CV8402_HWR[cycle]);{nl}\
			sprintf(pidname_2, "%s:PID", OM15_CV7301_HWR[cycle]);{nl}\
			sprintf(pidname_3, "%s:PID", OM15_CV7201_HWR[cycle]);{nl}\
			printf("%s, %s, %s, %s\\n", pidname, pidname_1, pidname_2, pidname_3);{nl}\
{nl}\
			pvAssign(pid_val,   pidname);{nl}\
			pvAssign(pid_val_1, pidname_1);{nl}\
			pvAssign(pid_val_2, pidname_2);{nl}\
			pvAssign(pid_val_3, pidname_3);{nl}\
			pvMonitor(pid_val);{nl}\
			pvMonitor(pid_val_1);{nl}\
			pvMonitor(pid_val_2);{nl}\
			pvMonitor(pid_val_3);{nl}\
				{nl}\
			//Level >= 70%{nl}\
			sprintf(levelname,   "%s:Level", OM15_LT8401_HWR[cycle]);{nl}\
			sprintf(levelname_1, "%s:Level", OM15_LT8201_HWR[cycle]);{nl}\
			printf("%s, %s\\n", levelname, levelname_1);{nl}\
{nl}\
			pvAssign(level_val,   levelname);{nl}\
			pvAssign(level_val_1, levelname_1);{nl}\
			pvMonitor(level_val);{nl}\
			pvMonitor(level_val_1);{nl}\
{nl}\
			sprintf(pressname,   "%s:Press", OM15_PT8301_HWR[cycle]);{nl}\
			sprintf(pressname_1, "%s:Press", OM15_PT8201_HWR[cycle]);{nl}\
{nl}\
			pvAssign(press_val,   pressname);{nl}\
			pvAssign(press_val_1, pressname_1);{nl}\
			pvMonitor(press_val);{nl}\
			pvMonitor(press_val_1);{nl}\
{nl}\
        {close}state PhaseEndHWR{nl}\
{nl}\
		when(delay(cdDelay) && level_val < 70 && level_val_1 < 70 && press_val > 2 && press_val_1 > 1.3){nl}\
        {open}{nl}\
        {close}state PhaseEndHWR{nl}\
{nl}\
		when(delay(cdDelay) && level_val >= 70 && level_val_1 >= 70 && press_val <= 2 && press_val_1 <= 1.3){nl}\
        {open}{nl}\
			//OM15 Stop & OM14 Start with cycle++{nl}\
			pid_val = pid_val_1 = pid_val_2 = pid_val_3 = 1;{nl}\
			pvPut(pid_val, SYNC);{nl}\
			pvPut(pid_val_1, SYNC);{nl}\
			pvPut(pid_val_2, SYNC);{nl}\
			pvPut(pid_val_3, SYNC);{nl}\
{nl}\
			cycle++;{nl}\
			pvPut(cycle, SYNC);{nl}\
{nl}\
{nl}\
			pvStopMonitor(pid_val);{nl}\
			pvStopMonitor(pid_val_1);{nl}\
			pvStopMonitor(pid_val_2);{nl}\
			pvStopMonitor(pid_val_3);{nl}\
			pvStopMonitor(level_val);{nl}\
			pvStopMonitor(level_val_1);{nl}\
			pvStopMonitor(press_val);{nl}\
			pvStopMonitor(press_val_1);{nl}\
{nl}\
			pvAssign(pid_val,   "");{nl}\
			pvAssign(pid_val_1, "");{nl}\
			pvAssign(pid_val_2, "");{nl}\
			pvAssign(pid_val_3, "");{nl}\
			pvAssign(level_val,   "");{nl}\
			pvAssign(level_val_1, "");{nl}\
			pvAssign(press_val,   "");{nl}\
			pvAssign(press_val_1, "");{nl}\
{nl}\
        {close}state Stop{nl}\
{nl}\
		when(delay(cdDelay)){nl}\
        {open}{nl}\
        {close}state OM15StateEnd{nl}\
    {close}{nl}\
{nl}\
	state OM15StateEnd {nl}\
    {open}{nl}\
		when(delay(cdDelay)){nl}\
        {open}{nl}\
        {close}state OM15StateEnd{nl}\
{nl}\
		when(delay(cdDelay)){nl}\
        {open}{nl}\
			//OM2 Standby 4.5K{nl}\
        {close}state Stop{nl}\
    {close}{nl}\
{nl}\
{close} {nl}\
'

seq.write(Text)
seq.close()

