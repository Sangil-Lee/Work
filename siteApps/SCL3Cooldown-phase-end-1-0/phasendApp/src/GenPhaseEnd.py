########/usr/local/bin/python3.8
#!/usr/bin/python3

import sys

####Python3 Literal String using f'

argc = len(sys.argv)
if argc != 2:
    print('Usage:'+str(sys.argv[0])+' PhaseEndName')
    raise SystemExit('EX)'+str(sys.argv[0])+' PhaseEnd')

filename = str(sys.argv[1])
vdbExt = ".vdb"
seqExt = ".stt"
dbdExt = ".dbd"
pvname = filename
vdbfile = filename + vdbExt
seqfile = filename + seqExt
dbdfile = filename + dbdExt
vdb   = open("../Db/"+ vdbfile, "w")
seq   = open("snc"+seqfile, "w")
dbd   = open("snc"+dbdfile, "w")

###################################################
list_0=[pvname,       "Passive", "(A==0)?0:(B>=C&&E&&F)?1:0"]
list_1=[pvname+"Cnt", "Passive", "(B==0||C==0||D==0)?0:A+1"]
condAnd = '&&'
seq_list = ["snc"+pvname,"ev"+pvname.lower(),pvname.lower()+"Val"]
pv_list = ["StepDly"]
prefix = "{SYS}{SUBSYS}{DEV}{SUBDEV}"
nl = '\n'
open = '{'
close = '}'
proc = 'proc'
###################################################

########### Waveform DB File Generation ###################
## PhaseEnd record(acalcout, calc)
#Text = f'record(acalcout, "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV)PhaseEnd"){open}{nl} \
Text = f'record(acalcout, "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV){list_0[0]}"){open}{nl} \
  field(SCAN, "{list_0[1]}") {nl} \
  field(CALC, "{list_0[2]}"){nl} \
  field(VAL, "0") {nl} \
  field(INPA, "0") {nl} \
  field(INPB, "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV){list_1[0]}") {nl} \
  field(INPC, "$(COUNT)") {nl} \
  field(INPE, "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV)TempEvalWFMon.VAL") {nl} \
  field(INPF, "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV)PressEvalWFMon.VAL") {nl} \
  field(FLNK, "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV){list_1[0]}") {nl} \
  field(PINI, "YES") {nl} \
{close} {nl} \
 {nl} \
record(calc, "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV){list_1[0]}") {open} {nl} \
  field(SCAN, "{list_1[1]}") {nl} \
  field(CALC, "{list_1[2]}") {nl} \
  field(VAL, "0") {nl} \
  field(INPA, "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV){list_1[0]}") {nl} \
  field(INPB, "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV){list_0[0]}.A") {nl} \
  field(INPC, "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV)TempEvalWFMon"){nl} \
  field(INPD, "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV)PressEvalWFMon"){nl} \
  field(PINI, "YES") {nl} \
{close}{nl} \
 {nl} \
record(ai, "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV){pv_list[0]}") {open} {nl} \
  field(SCAN, "{list_1[1]}") {nl} \
  field(VAL, "5") {nl} \
  field(PINI, "YES") {nl} \
{close}{nl}' 

vdb.write(Text)
vdb.close()
###################################################
########### Sequence(.stt) Generation ###################
Text = f'program  {seq_list[0]} {nl}\
{nl}\
option +r; {nl}\
{nl}\
int {seq_list[2]}; {nl}\
assign {seq_list[2]} to "{prefix}{list_0[0]}";{nl}\
monitor {seq_list[2]}; {nl}\
//evflag  {seq_list[1]};{nl}\
//sync {seq_list[2]} {seq_list[1]};{nl}\
int {proc}=1;{nl}\
assign {proc} to "{prefix}{list_0[0]}.PROC"; {nl}\
monitor {proc};{nl}\
{nl}\
float {pv_list[0].lower()}Val; {nl}\
assign {pv_list[0].lower()}Val to "{prefix}{pv_list[0]}";{nl}\
monitor {pv_list[0].lower()}Val; {nl}{nl}\
ss make{pvname} {nl}\
{open} {nl}\
    state init {nl}\
    {open} {nl}\
        when(TRUE) {nl}\
        {open} {nl}\
            //efSet({seq_list[1]}); {nl}\
            //pvAssign({proc}, "{prefix}{list_0[0]}.PROC"); {nl}\
            //pvMonitor({proc}); {nl}\
        {close}state Proc{pvname} {nl}\
    {close} {nl}\
    state Proc{pvname} {nl}\
    {open} {nl}\
        //when(delay({pv_list[0].lower()}Val){condAnd}efTestAndClear({seq_list[1]})) {nl}\
        when(delay({pv_list[0].lower()}Val){condAnd}{seq_list[2]}==0) {nl}\
        {open} {nl}\
           {proc} = 1;{nl}\
           pvPut({proc},SYNC);{nl}\
        {close}state Proc{pvname} {nl}{nl}\
        when(delay({pv_list[0].lower()}Val){condAnd}{seq_list[2]}==1) {nl}\
        {open} {nl}\
           //Finish PhaseEnd{nl}\
           printf("Finished PhaseEnd\\n"); {nl}\
        {close}state Proc{pvname} {nl}\
    {close} {nl}\
{close} {nl}\
'
seq.write(Text)
seq.close()
###################################################
########### Sequence DBD(.dbd) Generation ###################

Text = f'registrar(snc{pvname}Registrar)'
dbd.write(Text)
dbd.close()
###################################################

print("Successfully Generated File:", "snc"+seqfile )
