/*drvGtr.h */

/* Author:   Marty Kraimer */
/* Date:     17SEP2001     */

/*************************************************************************
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
* National Laboratory, and the Regents of the University of California, as
* Operator of Los Alamos National Laboratory. EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
*************************************************************************/

#ifndef drvGtrH
#define drvGtrH

#ifdef __cplusplus
extern "C" {
#endif

#include "typedefined.h"

typedef void *gtrPvt;
typedef void (*gtrhandler)(void *pvt);

typedef enum {gtrStatusOK=0,gtrStatusBusy=-1,gtrStatusError=-2} gtrStatus;

#define NAIPARM 13
typedef enum {
	temperature, power, phase, rmsAmplitude, rmsDiffPhase, rmsOrgPhase, dBmPower, stepAmp, stepPhase, outputDBM,amplitudeFBDummy,phaseFBDummy,outputDBMStepDummy
}aiParm;
static char *aiParmString[NAIPARM] =
{
	/*add auto step Amp,Phase : stepAmp, stepPhase : FPGA에서 UI로 부터 입력받은 stepSizeAmp, stepSizePhase를 1,2,3,4,5...등으로 저장해서 IOC로 넘긴 후 저장 되는 변수*/ 
	"temperature", "power", "phase", "rmsAmplitude", "rmsDiffPhase", "rmsOrgPhase", "dBmPower", "stepAmp", "stepPhase", "outputDBM","amplitudeFBDummy","phaseFBDummy","outputDBMStepDummy"
};

#define NAOPARM 19
typedef enum {
	/*add PID  : pKp, aKp, pKi, aKi, pKd, aKd , add auto step Amp,Phase : stepSizeAmp, stepSizePhase  */
	ampOffset, pacAmplitude, pacPhase, pacOffset, pKp, aKp, pKi, aKi, pKd, aKd, maxAmplitude, triggerOnTime, stepSizeAmp , stepSizePhase,  timeInterval, ampHighLimit, overPower, dBmOverPower
	, pulseLength
}aoParm;
static char *aoParmString[NAOPARM] =
{
	"ampOffset", "pacAmplitude", "pacPhase", "pacOffset", "pKp", "aKp", "pKi", "aKi", "pKd", "aKd",  "maxAmplitude", "triggerOnTime", "stepSizeAmp" , "stepSizePhase", "timeInterval", "ampHighLimit", "overPower", "dBmOverPower"
    , "pulseLength"
};

#define NBIPARM 4
typedef enum {
	powerState, currentMode, adcState, ampRange
}biParm;
static char *biParmString[NBIPARM] =
{
	"powerState", "currentMode", "adcState", "ampRange"
};

#define NBOPARM 13
typedef enum {
	phaseFB, amplitudeFB, audioMute, triggerSyncPol, TriggerOut, runMode, powerOn,reFresh, stepModAmp, stepModPhase, ampOn, FBCh, pacMode
}boParm;
static char *boParmString[NBOPARM] =
{
	/*Auto Step Mode On/Off : stepModAmp, stepModPhase*/
	"phaseFB", "amplitudeFB", "audioMute", "triggerSyncPol", "TriggerOut", "runMode", "powerOn","reFresh" , "stepModAmp", "stepModPhase", "ampOn", "FBCh", "pacMode"
};

#define NLOPARM 3
typedef enum {
	rmsSample, audioFreq, triggerFreq
}longoutParm;
static char *longoutParmString[NLOPARM] =
{
    "rmsSample", "audioFreq", "triggerFreq"
};

#define NMBBOPARM 2
typedef enum {
    triggerIn, clockSelect
}mbboParm;
static char *mbboParmString[NMBBOPARM] =
{
    "triggerIn", "clockSelect"
};

#define NSIPARM 3
typedef enum {
    name, curTime, upTime
}stringinParm;
static char *stringinParmString[NSIPARM] =
{
    "name", "curTime", "upTime"
};

#define NSOPARM 4
typedef enum {
    ipaddr, subnet, gateway, iocserver
}stringoutParm;
static char *stringoutParmString[NSOPARM] =
{
	"ipaddr", "subnet", "gateway", "iocserver"
};

#define NWFPARM 2
typedef enum {
    readData,readRawData
}waveformParm;
static char *waveformParmString[NWFPARM] =
{
    "readData","readRawData"
};

typedef struct gtrchannel {
    int len; /*size of pdata array*/
    int ndata; /*number of elements readMemory put into array*/
    int16 *pdata;
    int ftvl;
}gtrchannel;

typedef struct gtrops {
    void      (*init)(gtrPvt pvt);
    void      (*report)(gtrPvt pvt,int level);

	/* ai  */
	gtrStatus (*aiFunction)(long parm, gtrPvt pvt, double *value, int index);

	/* ao */
	gtrStatus (*aoFunction)(long parm, gtrPvt pvt, double value, int index);

	/* bi */
	gtrStatus (*biFunction)(long parm, gtrPvt pvt, int *value, int index);
	
	/* bo */
	gtrStatus (*boFunction)(long parm, gtrPvt pvt, int value, int index);

	/* longout */
	gtrStatus (*longoutFunction)(long parm, gtrPvt pvt, int value);

	/* mbbo */
	gtrStatus (*mbboFunction)(long parm, gtrPvt pvt, int value);

	gtrStatus (*registerHandler)(gtrPvt pvt,gtrhandler usrIH,void *handlerPvt);

    gtrStatus (*readMemory)(gtrPvt pvt, gtrchannel **papgtrchannel);
    gtrStatus (*readRawMemory)(gtrPvt pvt, gtrchannel **papgtrchannel);

	/* stringin */
	gtrStatus (*stringinFunction)(long parm, gtrPvt pvt, char *pname, int maxchars, int initFlag);

	/* stringout */
	gtrStatus (*stringoutFunction)(long parm, gtrPvt pvt, const char *pvalue);
	
    void      (*setUser)(gtrPvt pvt,void * userPvt);
    void      *(*getUser)(gtrPvt pvt);
    void      (*lock)(gtrPvt pvt);
    void      (*unlock)(gtrPvt pvt);
}gtrops;

gtrPvt gtrFind(int card,gtrops **ppgtrops);

void gtrRegisterDriver(int card,
    const char *name,gtrops *pgtrdrvops,gtrPvt drvPvt);

#ifdef __cplusplus
}
#endif

#endif /*drvGtrH*/
