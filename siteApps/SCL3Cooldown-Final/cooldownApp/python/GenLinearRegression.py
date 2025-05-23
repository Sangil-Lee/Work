#!/usr/local/bin/python3.8
import sys

argc = len(sys.argv)
if argc < 4 or argc > 5:
    print('Usage:'+str(sys.argv[0])+' PVList WaveformPVName Datatype(DOUBLE, UCHAR, USHORT, ...) OUTLINK')
    raise SystemExit('EX)'+str(sys.argv[0])+' (Temp.txt or Press.txt) (TempEvalWF or PressEvalWF) DOUBLE (and OUTLINK)')

prefix = "$(SYS)$(SUBSYS)$(DEV)$(SUBDEV)"
#Usage:$>python GenLinearRegression.py Temp.txt TempWF DOUBLE
filename = str(sys.argv[1])
wfname   = str(sys.argv[2])
datatype = str(sys.argv[3])
wf_fullname = prefix + wfname

if(argc == 5): outlink = str(sys.argv[4])
#print('Argument List', filename)
#print('Argument Count', len(sys.argv));
genout = 0

if (argc == 5 and outlink == "OUTLINK"): genout = 1

pvlist = []
f = open(filename, 'r')
for line in f:
    pvlist.append(line.rstrip('\n'))

dtype = "double"
if(datatype == "DOUBLE" or datatype=="FLOAT" ): dtype = "double"
elif(datatype == "SHORT" or datatype=="USHORT" ): dtype = "int"
elif(datatype == "CHAR" or datatype=="UCHAR" ): dtype = "unsigned char"
#print(pvlist)
#print(len(pvlist))

listlength =len(pvlist);

f.close()

seqfilename = "snc"+filename.rsplit('.', 1)[0]+"WF"
asubfilename = "aSub"+filename.rsplit('.', 1)[0]+"WF"
lregfilename = "LReg"+filename.rsplit('.', 1)[0]+"WF"
seqExt = ".stt"
dbdExt = ".dbd"
vdbExt = ".vdb"
templExt = ".template"
subExt = ".sub"
cppExt = ".cpp"

seq   = open(seqfilename+seqExt, "w")
dbd   = open(seqfilename+dbdExt, "w")
cpp   = open(asubfilename+cppExt, "w")
cppdbd= open(asubfilename+dbdExt, "w")
vdb   = open("../Db/LR"+seqfilename+vdbExt, "w")
#cppvdb= open("../Db/"+lregfilename+vdbExt, "w")
templ = open("../Db/"+seqfilename+templExt, "w")
sub = open("../Db/"+seqfilename+subExt, "w")

########### Seqencer File Generation ###################
sncText = "program " + seqfilename + "\n\noption +r; \n\n"
seq.write(sncText)

#sncText = "double wfList["+ str(listlength)+ "];\n"
sncText = dtype + " wfList["+ str(listlength)+ "];\n"
seq.write(sncText)

sncText = "assign wfList to {\n"
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
sncText = "\n};\nmonitor wfList;\nevflag evWave;\nsync wfList evWave;\n\n"
seq.write(sncText)

#sncText = "double wfValue["+ str(listlength)+ "];\n"
sncText = dtype + " wfValue["+ str(listlength)+ "];\n"
seq.write(sncText)

sncText = "assign wfValue to \"{SYS}{SUBSYS}{DEV}{SUBDEV}"+wfname+"\";\n"
seq.write(sncText)

sncText = "monitor wfValue;\nevflag evWaveIdx;\nsync wfValue evWaveIdx;\n\n"
seq.write(sncText)

#sncText = "double idxValue["+ str(listlength)+ "];\n"
sncText = dtype + " idxValue["+ str(listlength)+ "];\n"
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

sncText = "\t\t\tefSet(evWave);\n\t\t}state MakeWaveform\n\t}\n"
seq.write(sncText)

sncText = "\tstate MakeWaveform\n\t{\n"
seq.write(sncText)

sncText = "\t\twhen(efTestAndClear(evWave))\n\t\t{\n"
seq.write(sncText)

sncText = "\t\t\tint index = 0;\n"
seq.write(sncText)

sncText = "\t\t\tfor(;index < "+str(listlength)+ ";index++)\n"
seq.write(sncText)

sncText = "\t\t\t\twfValue[index]=wfList[index];\n"
seq.write(sncText)

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

sncText = "\t\t\tefSet(evWaveIdx);\n\t\t}state MakeWaveform_Idx\n\t}\n"
seq.write(sncText)

sncText = "\tstate MakeWaveform_Idx\n\t{\n"
seq.write(sncText)

sncText = "\t\twhen(efTestAndClear(evWaveIdx))\n\t\t{\n"
seq.write(sncText)

sncText = "\t\t\tint index = 0;\n"
seq.write(sncText)

sncText = "\t\t\tfor(;index < "+str(listlength)+ ";index++)\n\t\t\t{"
seq.write(sncText)

sncText = "\n\t\t\t\tidxValue[index]=wfValue[index];\n"
seq.write(sncText)

sncText = "\t\t\t\tpvPut(idxValue[index], SYNC);\n\t\t\t}"
seq.write(sncText)

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
nl = '\n'
open = '{'
close = '}'
########### Waveform DB File Generation ###################
nelm = str(listlength)
Text=f'{nl}\
record(acalcout, "{prefix}{wfname}Init") {open}{nl}\
{nl}\
 field(SCAN, "1 second"){nl}\
 field(NELM, "{nelm}"){nl}\
 field(INPB, "10"){nl}\
 #field(INAA, "{prefix}{wfname}.VAL"){nl}\
 field(OUT, "{prefix}{wfname} PP"){nl}\
 #field(CALC, "A:=1;UNTIL(AA:=AA+1;A=A+1;A<{nelm}");AA){nl}\
 #field(CALC, "AA:=(IX+1)*ARNDM;AA"){nl}\
 field(CALC, "AA:=(IX+1)+ARNDM*B;AA"){nl}\
 field(PINI, "YES"){nl}\
{close}{nl}\
record(waveform, {prefix}{wfname}) {open}{nl}\
  field(SCAN, "Passive"){nl}\
  field(NELM, "{nelm}"){nl}\
  field(FTVL, "{datatype}"){nl}\
  field(PINI, "YES"){nl}\
  field(PREC, "3"){nl}\
#field(FLNK, "{prefix}LReg{wfname}"){nl}\
  field(FLNK, "{prefix}{wfname}FOut"){nl}\
{close}{nl}\
record(fanout, {prefix}{wfname}FOut) {open}{nl}\
  field(SCAN, "Passive"){nl}\
  field(FLNK, "{prefix}LReg{wfname}"){nl}\
#field(LNK0, "{wfname}Mon"){nl}\
{close}{nl}\
record(aSub, {prefix}LReg{wfname}) {open}{nl}\
#field(SCAN, "1 second"){nl}\
  field(SCAN, "Passive"){nl}\
  field(PINI, "YES"){nl}\
  field(INAM, "Init{lregfilename}"){nl}\
  field(SNAM, "Proc{lregfilename}"){nl}\
  field(INPA, "1"){nl}\
  field(INPB, "{prefix}{wfname}"){nl}\
  field(NOB, "{nelm}"){nl}\
  field(NOVA, "{nelm}"){nl}\
  field(OUTA, "{prefix}LR{wfname} PP"){nl}\
{close}{nl}\
record(waveform, {prefix}LR{wfname}) {open}{nl}\
  field(SCAN, "Passive"){nl}\
  field(NELM, "{nelm}"){nl}\
  field(FTVL, "{datatype}"){nl}\
  field(FLNK, "{prefix}{wfname}Logic"){nl}\
{close}{nl}\
record(acalcout, {prefix}{wfname}Logic) {open}{nl}\
  field(INAA, "{prefix}LR{wfname}"){nl}\
  field(INBB, "{prefix}{wfname}"){nl}\
  field(NELM, "{nelm}"){nl}\
  field(CALC, "CC:=AA-BB;AMAX(ABS(CC))"){nl}\
  field(OOPT, "Every Time"){nl}\
  field(DOPT, "Use CALC"){nl}\
{close}{nl}\
record(ai, {prefix}RandMaxVal) {open}{nl}\
  field(SCAN, "Passive"){nl}\
  field(VAL, "5.0"){nl}\
{close}{nl}\
record(ai, {prefix}ScanVal) {open}{nl}\
  field(SCAN, "Passive"){nl}\
  field(VAL, "5.0"){nl}\
{close}{nl}\
'
vdb.write(Text)
vdb.close()
###################################################

########### Template File Generation ###################
sncText = "record(ao, \"$(SYS)$(SUBSYS)$(DEV)$(SUBDEV)$(SIGNAL)\")\n{\n"
templ.write(sncText)

if(genout == 1):
    sncText = "\tfield(OUT, \"$(OUT) CPP\")\n"
    templ.write(sncText)
    sncText = "\tfield(FLNK, \"$(OUT)\")\n"
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

if(genout ==1):
    sncText = "{SIGNAL,\t\t\t SCAN,\t\tOUT}\n"
    sub.write(sncText)
else:
    sncText = "{SIGNAL,\t\t\t SCAN}\n"
    sub.write(sncText)

for line in range(len(pvlist)):
    strline = str(line)
    if(genout == 1):
        sncText="{Index_"+strline+",\t\t\t \"Passive\",\t \"\"}\n"
    else:
        sncText="{Index_"+strline+",\t\t\t \"Passive\"}\n"

    sub.write(sncText)
sncText="}\n"
sub.write(sncText)
sub.close()
###################################################
########### asubRecord CPP File Generation ###################
Text=f'{nl}\
#include <iostream>{nl}\
#include <dbAccess.h>{nl}\
#include <registryFunction.h>{nl}\
#include <waveformRecord.h>{nl}\
#include <aSubRecord.h>{nl}\
#include <epicsExport.h>{nl}\
#include <Eigen/Dense>{nl}\
using namespace std;{nl}\
using namespace Eigen;{nl}\
static int lregressDebug = 0;{nl}\
{nl}\
VectorXd Polynomial(VectorXd xvals, VectorXd yvals, int order){nl}\
{open}{nl}\
    assert(xvals.size() == yvals.size());{nl}\
    assert(order >= 1 && order <= xvals.size()-1 );{nl}\
{nl}\
    MatrixXd A(xvals.size(), order + 1);{nl}\
{nl}\
    for(int i = 0; i < xvals.size(); i++) {open}{nl}\
        A(i, 0) = 1.0;{nl}\
    {close};{nl}\
    if(lregressDebug == 1) {nl}\
      cout << "A-Mat(unit): " <<  A << endl;{nl}\
{nl}\
    for(int j = 0; j < xvals.size(); j++) {open}{nl}\
        for(int i = 0; i < order; i++) {open}{nl}\
            A(j, i+1) = A(j,i)*xvals(j);{nl}\
        {close};{nl}\
    {close};{nl}\
{nl}\
    if(lregressDebug == 1){nl}\
      cout << "A-Mat: " <<  A << endl;{nl}\
{nl}\
    auto Q = A.householderQr();{nl}\
{nl}\
//  if(lregressDebug == 1){nl}\
//      cout << "Q: " <<  A << endl;{nl}\
{nl}\
    auto coeff = Q.solve(yvals);{nl}\
{nl}\
    return coeff;{nl}\
{close}{nl}\
'
cpp.write(Text)

Text=f'{nl}\
double polynomial_calc(VectorXd coeffs, double xval){nl}\
{open}{nl}\
    double result = 0.0;{nl}\
    for(int i = 0; i < coeffs.size(); i++){nl}\
        result += coeffs(i) * pow(xval, i);{nl}\
{nl}\
    return result;{nl}\
{close}{nl}\
'
cpp.write(Text)

Text=f'{nl}\
static long Init{lregfilename}(aSubRecord *pRec){nl}\
{open}{nl}\
    return(0);{nl}\
{close};{nl}\
static long Proc{lregfilename}(aSubRecord *pRec){nl}\
{open}{nl}\
    long status = 0;{nl}\
{nl}\
    double *aval_deg = (double*)pRec->a;{nl}\
    DBADDR *pdbTempWave = (DBADDR*)(&pRec->inpb)->value.pv_link.pvt;{nl}\
    waveformRecord *pTempWave = (waveformRecord *)pdbTempWave->precord;{nl}\
{nl}\
    //printf("Wave NORD:%d, NELM:%d \\n", pTempWave->nord, pdbTempWave->no_elements );{nl}\
    //if(pTempWave->nord != pdbTempWave->no_elements ) return(-1);{nl}\
    //double *pTempWaveVal = (double*)pdbTempWave->pfield;{nl}\
{nl}\
    double *pTempWaveVal = (double*)pRec->b;{nl}\
    long nelm = pdbTempWave->no_elements;{nl}\
{nl}\
#if 1{nl}\
    DBADDR *pdbLRDBAddr = (DBADDR*)(&pRec->outa)->value.pv_link.pvt;{nl}\
    waveformRecord *pLRWave = (waveformRecord *)pdbLRDBAddr->precord;{nl}\
    double *pLRWaveVal = (double*)pRec->vala;{nl}\
#endif{nl}\
{nl}\
    VectorXd xvals(nelm), yvals(nelm);{nl}\
    double dval;{nl}\
    for (int i=0;i<nelm;i++){nl}\
    {open}{nl}\
        xvals(i) = i;{nl}\
        dval = pTempWaveVal[i];{nl}\
        yvals(i) = dval;{nl}\
{nl}\
        if(lregressDebug == 2){nl}\
            printf("X(%d):%f, Y(%d):%f\\n", i, xvals(i), i, yvals(i));{nl}\
    {close};{nl}\
{nl}\
    auto coeffs = Polynomial(xvals, yvals, aval_deg[0]);{nl}\
{nl}\
    if(lregressDebug == 2){nl}\
        cout << "Poly-Coeff: " << coeffs << endl << endl;{nl}\
{nl}\
#if 1{nl}\
    for(int i = 0; i < yvals.size(); i++){nl}\
    {open}{nl}\
        pLRWaveVal[i] = polynomial_calc(coeffs, xvals(i));{nl}\
    {close};{nl}\
#endif{nl}\
{nl}\
    dbProcess((dbCommon*)pLRWave);{nl}\
    return(status);{nl}\
{close};{nl}\
{nl}\
epicsRegisterFunction(Init{lregfilename});{nl}\
epicsRegisterFunction(Proc{lregfilename});{nl}\
epicsExportAddress(int, lregressDebug);{nl}\
'
cpp.write(Text)
cpp.close()

Text = f'\
function(Init{lregfilename}){nl}\
function(Proc{lregfilename}){nl}\
variable(lregressDebug){nl}\
'
cppdbd.write(Text)
cppdbd.close()

###################################################
print("Successfully Generated File: " + seqfilename+seqExt +
","+seqfilename+dbdExt +"," +asubfilename+cppExt+ ","+asubfilename+dbdExt + 
","+"../Db/"+seqfilename+vdbExt + ","+"../Db/LR"+seqfilename+templExt+","+"../Db/"+seqfilename+subExt)