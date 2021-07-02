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
vdb   = open("../Db/"+ vdbfile, "w")
seq   = open("snc"+seqfile, "w")

list_0=[pvname,       "Passive", "(A==0)?0:(B>=C&&E)?1:0"]
list_1=[pvname+"Cnt", "Passive", "(B==0||C==0)?0:A+1"]

nl = '\n'
open = '{'
close = '}'
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
  field(PINI, "YES") {nl} \
{close}{nl}' 

vdb.write(Text)
vdb.close()
###################################################
condAnd = '&&'
seq_list = ["snc"+pvname,"ev"+pvname.lower(),pvname.lower()+"Val"]
########### Sequence(.stt) Generation ###################
Text = f'program  {seq_list[0]} {nl}\
{nl}{nl}\
option +r {nl}\
{nl}{nl}\
int {seq_list[2]}; {nl}\
assign {seq_list[2]} to "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV){list_0[0]}";{nl}{nl}\
monitor {seq_list[2]}; {nl}\
evflag  {seq_list[1]};{nl}\
sync {seq_list[2]} {seq_list[1]};{nl}{nl}\
ss make{pvname} {nl}\
{open} {nl}\
    state init {nl}\
    {open} {nl}\
        when(TRUE) {nl}\
        {open} {nl}\
            efSet({seq_list[1]}); {nl}\
        {close}state Proc{pvname} {nl}\
    {close} {nl}\
    state Proc{pvname} {nl}\
    {open} {nl}\
        when(delay(){condAnd}efTestAndClear({seq_list[1]})) {nl}\
        {open} {nl}\
        {close}state Proc{pvname} {nl}\
    {close} {nl}\
{close} {nl}\
'
seq.write(Text)
seq.close()
###################################################

print("Successfully Generated File: " )
