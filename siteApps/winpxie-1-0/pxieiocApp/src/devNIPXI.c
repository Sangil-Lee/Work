#include <string.h>

#include "registryFunction.h"
#include "epicsExport.h"
#include "dbCommon.h"
#include "iocsh.h"
#include "menuConvert.h"
#include "NIDAQmx.h"

#include "longinRecord.h"
#include "longoutRecord.h"
#include "aoRecord.h"
#include "aiRecord.h"
#include "mbbiDirectRecord.h"
#include "mbboDirectRecord.h"
#include "calcoutRecord.h"
#include "devPXIe.h"

static int nidaqDebug = 3;
static TaskHandle  g_taskHandle;

static char gDevName[24];

struct {
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devPXI6514ReadMbbi = {
    5,
    NULL,
    NULL,
    initRecordMbbiDI,
    NULL,
    readPXI6514Mbbi
};

epicsExportAddress(dset,devPXI6514ReadMbbi);


long initRecordMbbiDI(struct mbbiDirectRecord *pRec)
{
    if(recGblInitConstantLink(&pRec->inp,DBF_ULONG,&pRec->val))
         pRec->udf = FALSE;
    return(0);
}

long readPXI6514Mbbi(struct mbbiDirectRecord *pRec)
{
    long 	status;
    //uInt32	data = 2147483647;      //0x7FFF,FFFF
    //uInt32	data = 4294967295;      //0xFFFF,FFFF
    uInt32	data = 0;
    int32	read = 0;

    status = dbGetLink(&(pRec->inp),DBF_ULONG, &(pRec->val),0,0);
    /*If return was succesful then set undefined false*/

    //status = DAQmxReadDigitalU32(g_taskHandle,1, 10.0, DAQmx_Val_GroupByChannel, &data, 1, &read, NULL);

    pRec->rval = data;

    if(nidaqDebug == 1)
        printf("Mbbi-Data:(0x%X), Read(%d)\n", pRec->val, read);


    if(!status) pRec->udf = FALSE;
    return(0);
}

struct {
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devPXI6514WriteMbbo = {
    5,
    NULL,
    NULL,
    initRecordMbboDO,
    NULL,
    writePXI6514Mbbo
};

epicsExportAddress(dset,devPXI6514WriteMbbo);


long initRecordMbboDO(struct mbboDirectRecord *pRec)
{
    if(recGblInitConstantLink(&pRec->out,DBF_ULONG,&pRec->rval))
         pRec->udf = FALSE;
    return(0);
}

long writePXI6514Mbbo(struct mbboDirectRecord *pRec)
{
    long 	status;
    status = dbGetLink(&(pRec->out),DBF_ULONG, &(pRec->rval),0,0);

    uInt32  data = (uInt32)pRec->rval;
    int32	written;

    /*********************************************/
    // DAQmx Write Code
    /*********************************************/
    status = DAQmxWriteDigitalU32(g_taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel,&data,&written,NULL);

    if(nidaqDebug == 2)
        printf("Data(0x%X), Written Data(%d)\n", data, written);

    if(!status) pRec->udf = FALSE;
    return(0);
}

struct {
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} devPXI6514Read = {
    5,
    NULL,
    NULL,
    initRecordDI,
    NULL,
    readPXI6514
};

epicsExportAddress(dset,devPXI6514Read);


long initRecordDI(struct longinRecord *pRec)
{
    if(recGblInitConstantLink(&pRec->inp,DBF_LONG,&pRec->val))
         pRec->udf = FALSE;
    return(0);
}

long readPXI6514(struct longinRecord *pRec)
{
    long 	status;
    uInt32	data;
    int32	read;

    status = dbGetLink(&(pRec->inp),DBF_LONG, &(pRec->val),0,0);
    /*If return was succesful then set undefined false*/

    //status = DAQmxReadDigitalU32(g_taskHandle,1, 10.0, DAQmx_Val_GroupByChannel, &data, 1, &read, NULL);

    if(nidaqDebug == 1)
        printf("Data:(0x%X), Read(%d)\n", data, read);

    pRec->val = data;

    if(!status) pRec->udf = FALSE;
    return(0);
}

struct {
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write;
} devPXI6514Write = {
    5,
    NULL,
    NULL,
    initRecordDO,
    NULL,
    writePXI6514
};

long initRecordDO(struct longoutRecord *pRec)
{
    if(recGblInitConstantLink(&pRec->out,DBF_LONG,&pRec->val))
         pRec->udf = FALSE;
    return(0);
}


long writePXI6514(struct longoutRecord *pRec)
{
    long status;

    status = dbGetLink(&(pRec->out),DBF_LONG, &(pRec->val),0,0);
    /*If return was succesful then set undefined false*/

    uInt32  data = (uInt32)pRec->val;
    int32	written;

    /*********************************************/
    // DAQmx Write Code
    /*********************************************/
    status = DAQmxWriteDigitalU32(g_taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel,&data,&written,NULL);

    if(nidaqDebug == 2)
        printf("Data(0x%X), Written Data(%d)\n", data, written);

    if(!status) pRec->udf = FALSE;
    return(0);
}

epicsExportAddress(dset,devPXI6514Write);

struct devPXI4322Write{
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write;
} devPXI4322Write = {
    5,
    NULL,
    NULL,
    initRecordPXIAO,
    NULL,
    writePXIAO
};

long initRecordPXIAO(struct aoRecord *pRec, int pass)
{
    if(recGblInitConstantLink(&pRec->out,DBF_DOUBLE,&pRec->val))
         pRec->udf = FALSE;
    return(0);
#if 0
    aoRecord *prec = (aoRecord *)pRec;
    struct devPXI4322Write *pdset;
    long status;

    if (pass == 0) return (0);

    if ((pdset = (struct devPXI4322Write *)(prec->dset)) == NULL)
    {
        recGblRecordError(S_dev_noDSET, prec, "ao: init_record");
        return (S_dev_noDSET);
    }

    /* must have write function defined */
    if ((pdset->number < 5) || (pdset->write == NULL))
    {
        recGblRecordError(S_dev_missingSup, pdset, "ao: init_record");
        return (S_dev_missingSup);
    }

    if (pdset->init_record)
    {
        if ((status = (*pdset->init_record)(prec)))
            return (status);
    }

    return (0);
#endif
}


long writePXIAO(struct aoRecord *pRec)
{
    long status;

    status = dbGetLink(&(pRec->out),DBF_DOUBLE, &(pRec->val),0,0);
    /*If return was succesful then set undefined false*/

    /*********************************************/
    // DAQmx Write Code
    /*********************************************/
    //Current Output
    int written = 0;
    float64  data = pRec->val;
    status = DAQmxWriteAnalogF64(g_taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel, &data,NULL,NULL);

    //Voltage Out
    //status = DAQmxWriteAnalogF64(g_taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel,&data,NULL,NULL);

    if(nidaqDebug == 3)
        printf("AO Output(%f), Written(%d)\n", data, written);

    if(!status) pRec->udf = FALSE;
    return(0);
}

epicsExportAddress(dset,devPXI4322Write);

struct devPXI4322AIWrite{
    long number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write;
} devPXI4322AIWrite = {
    5,
    NULL,
    NULL,
    initRecordAIPXIAO,
    NULL,
    writePXIAI
};

long initRecordAIPXIAO(struct aiRecord *pRec)
{
    //if(recGblInitConstantLink(&pRec->inp,DBF_DOUBLE,&pRec->val))
         //pRec->udf = FALSE;
    return(0);
}


long writePXIAI(struct aiRecord *pRec)
{
    long status;

    status = dbGetLink(&(pRec->inp),DBF_DOUBLE, &(pRec->val),0,0);
    /*If return was succesful then set undefined false*/

    /*********************************************/
    // DAQmx Write Code
    /*********************************************/
    //Current Output
    int written = 0;
    float64  data = pRec->val;
    status = DAQmxWriteAnalogF64(g_taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel, &data,NULL,NULL);

    //Voltage Out
    //status = DAQmxWriteAnalogF64(g_taskHandle,1,1,10.0,DAQmx_Val_GroupByChannel,&data,NULL,NULL);

    if(nidaqDebug == 3)
        printf("AO Output(%f), Written(%d)\n", data, written);

    if(!status) pRec->udf = FALSE;
    return(0);
}

epicsExportAddress(dset, devPXI4322AIWrite);



/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxSizeSnapshot The maximum  number of sample in one snapshot
  * \param[in] maxNbSnapshot The number of snapshot buffered
 */
//============================>
epicsShareFunc int nidaqDIConfigure(const char *portName, const char *deviceName)
{
    int status = DAQmxCreateTask("", &g_taskHandle);

    char devNames[60];
	status = DAQmxGetSysDevNames(devNames, sizeof(devNames));
	printf("Device Names: %s\n", devNames);
	strcpy(gDevName, deviceName);
	return status;
}

epicsShareFunc int nidaqDICreateChannel(const char* pxiDevSlotName, const char *channelName)
{
	char devChName[40];	
	sprintf(devChName, "%s/%s", pxiDevSlotName, channelName); 

    printf("DI Channel:(%s)\n", devChName);

	int status = DAQmxCreateDIChan(g_taskHandle, devChName, "", DAQmx_Val_ChanForAllLines);
	return(status);
}

epicsShareFunc int nidaqDOCreateChannel(const char* pxiDevSlotName, const char *channelName)
{
	char devChName[40];	
	sprintf(devChName, "%s/%s", pxiDevSlotName, channelName); 

    printf("DO Channel:(%s)\n", devChName);
	int status = DAQmxCreateDOChan(g_taskHandle, devChName, "", DAQmx_Val_ChanForAllLines);
	return(status);
}

epicsShareFunc int nidaqAOCreateChannel(const char* pxiDevSlotName, const char *channelName)
{
	char devChName[40];	
	sprintf(devChName, "%s/%s", pxiDevSlotName, channelName); 

    printf("SlotChan:%s\n", devChName);

    //Analog Output Voltage 
    //int status = DAQmxCreateAOVoltageChan(g_taskHandle,devChName,"",-10.0,10.0,DAQmx_Val_Volts,"");

    //Analog Output Ampere 
    int status = DAQmxCreateAOCurrentChan(g_taskHandle,devChName,"", 0.0, 0.02, DAQmx_Val_Amps, "");
	return(status);
}

/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName"          ,iocshArgString};
static const iocshArg initArg1 = { "device name"       ,iocshArgString};

static const iocshArg * const initArgs[] = {&initArg0, &initArg1};

static const iocshFuncDef initFuncDef = {"nidaqDIConfigure",2,initArgs};

static void initCallFunc(const iocshArgBuf *args)
{
    nidaqDIConfigure(args[0].sval, args[1].sval);
}

static const iocshArg diArg0 = { "pxiDevSlotName", iocshArgString};
static const iocshArg diArg1 = { "chportName", iocshArgString};
static const iocshArg * const diArgs[] = {&diArg0, &diArg1};
static const iocshFuncDef niDICreateFuncDef = {"nidaqDICreateChannel", 2, diArgs};

static void niDICreateFunc(const iocshArgBuf *args)
{
	nidaqDICreateChannel(args[0].sval, args[1].sval);
}

static const iocshArg doArg0 = { "pxiDevSlotName", iocshArgString};
static const iocshArg doArg1 = { "chportName", iocshArgString};
static const iocshArg * const doArgs[] = {&doArg0, &doArg1};
static const iocshFuncDef niDOCreateFuncDef = {"nidaqDOCreateChannel", 2, doArgs};

static void niDOCreateFunc(const iocshArgBuf *args)
{
	nidaqDOCreateChannel(args[0].sval, args[1].sval);
}

static const iocshArg aoArg0 = { "pxiDevSlotName", iocshArgString};
static const iocshArg aoArg1 = { "chportName", iocshArgString};
static const iocshArg * const aoArgs[] = {&aoArg0, &aoArg1};
static const iocshFuncDef niAOCreateFuncDef = {"nidaqAOCreateChannel", 2, aoArgs};

static void niAOCreateFunc(const iocshArgBuf *args)
{
	nidaqAOCreateChannel(args[0].sval, args[1].sval);
}

static const iocshFuncDef niTaskStartFuncDef = {"nidaqTaskStart", 0};

static void niTaskStartFunc()
{
	DAQmxStartTask(g_taskHandle);
}

void nidaqAsynEpicsRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
    iocshRegister(&niDICreateFuncDef,niDICreateFunc);
    iocshRegister(&niDOCreateFuncDef,niDOCreateFunc);
    iocshRegister(&niAOCreateFuncDef,niAOCreateFunc);
    iocshRegister(&niTaskStartFuncDef, niTaskStartFunc);
}

epicsExportAddress(int, nidaqDebug);
epicsExportRegistrar(nidaqAsynEpicsRegister);
