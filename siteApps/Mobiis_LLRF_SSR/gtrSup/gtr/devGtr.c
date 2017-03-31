/*devGtr.c */

/* Author:   Marty Kraimer */
/* Date:     17SEP2001     */

/*************************************************************************
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
* National Laboratory, and the Regents of the University of California, as
* Operator of Los Alamos National Laboratory. EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
*************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>

#include <epicsExport.h>
#include <errlog.h>
#include <dbStaticLib.h>
#include <callback.h>
#include <alarm.h>
#include <dbAccess.h>
#include <dbScan.h>
#include <recGbl.h>
#include <recSup.h>
#include <devSup.h>
#include <dbCommon.h>
#include <aiRecord.h>
#include <biRecord.h>
#include <longinRecord.h>
#include <mbbiRecord.h>
#include <aoRecord.h>
#include <boRecord.h>
#include <longoutRecord.h>
#include <mbboRecord.h>
#include <stringinRecord.h>
#include <stringoutRecord.h>
#include <waveformRecord.h>
#include <menuFtype.h>
#include <devLib.h>

#include "drvGtr.h"
#include "typedefined.h"


typedef struct devGtrChannels {
    int nchannels;
    gtrchannel *pachannel;
    gtrchannel **papgtrchannel;
    int hasWaveforms;
} devGtrChannels;

typedef struct devGtr {
    CALLBACK callback;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
    IOSCANPVT   ioscanpvt;
    int arm;
    devGtrChannels channels;
    devGtrChannels rawChannels;
} devGtr;

typedef struct dpvt{
    int      parm;
    devGtr *pdevGtr;
    /* The following are only used by waveform record */
    int      signal; /*only used by waveform*/
    int      isPdataBptr;
}dpvt;


static long get_ioint_info(int cmd, dbCommon *precord, IOSCANPVT *pvt);
typedef struct aidset {
	long	  number;
	DEVSUPFUN report;
	DEVSUPFUN init;
	DEVSUPFUN init_record;
	DEVSUPFUN get_ioint_info;
	DEVSUPFUN read;
	DEVSUPFUN special_linconv;
} aidset;
static long ai_init_record(dbCommon *precord);
static long ai_read(dbCommon *precord);
aidset devGtrAI
	= {6,0,0,ai_init_record,get_ioint_info,ai_read,0};
epicsExportAddress(dset,devGtrAI);

typedef struct aodset {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write;
    DEVSUPFUN special_linconv;    
} aodset;
static long ao_init_record(dbCommon *precord);
static long ao_write(dbCommon *precord);
aodset devGtrAO = {6,0,0,ao_init_record,get_ioint_info,ao_write, 0};
epicsExportAddress(dset,devGtrAO);

typedef struct bidset {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} bidset;
static long bi_init_record(dbCommon *precord);
static long bi_read(dbCommon *precord);
bidset devGtrBI = {5,0,0,bi_init_record,get_ioint_info,bi_read};
epicsExportAddress(dset,devGtrBI);

typedef struct bodset {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write;
} bodset;
static long bo_init_record(dbCommon *precord);
static long bo_write(dbCommon *precord);
bodset devGtrBO = {5,0,0,bo_init_record,get_ioint_info,bo_write};
epicsExportAddress(dset,devGtrBO);

typedef struct longoutdset {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write;
} longoutdset;
static long longout_init_record(dbCommon *precord);
static long longout_write(dbCommon *precord);
longoutdset devGtrLO = {5,0,0,longout_init_record,get_ioint_info,longout_write};
epicsExportAddress(dset,devGtrLO);

typedef struct mbbodset {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write;
} mbbodset;
static long mbbo_init_record(dbCommon *precord);
static long mbbo_write(dbCommon *precord);
mbbodset devGtrMBBO   = {5,0,0,mbbo_init_record,get_ioint_info,mbbo_write};
epicsExportAddress(dset,devGtrMBBO);

typedef struct stringindset {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} stringindset;
static long stringin_init_record(dbCommon *precord);
static long stringin_read(dbCommon *precord);
stringindset devGtrSI
    = {5,0,0,stringin_init_record,get_ioint_info,stringin_read};
epicsExportAddress(dset,devGtrSI);

typedef struct stringoutdset {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN write;
} stringoutdset;
static long stringout_init_record(dbCommon *precord);
static long stringout_write(dbCommon *precord);
stringoutdset devGtrSO
    = {5,0,0,stringout_init_record,get_ioint_info,stringout_write};
epicsExportAddress(dset,devGtrSO);

typedef struct waveformdset {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read;
} waveformdset;
static long waveform_init_record(dbCommon *precord);
static long waveform_read(dbCommon *precord);
waveformdset devGtrWF =
    {5,0,0,waveform_init_record,get_ioint_info,waveform_read};
epicsExportAddress(dset,devGtrWF);

static void myCallback(CALLBACK *pcallback)
{
    devGtr *pdevGtr = 0;
    gtrops *pgtrops;
    gtrStatus status;

    callbackGetUser(pdevGtr,pcallback);
    pgtrops = pdevGtr->pgtrops;
    if(pdevGtr->channels.hasWaveforms) {
        status = (*pgtrops->readMemory)(pdevGtr->gtrpvt,pdevGtr->channels.papgtrchannel);
        if(status!=gtrStatusOK)
            printf("devGtr: myCallback read failed\n");
    }
    if(pdevGtr->rawChannels.hasWaveforms) {
        status = (*pgtrops->readRawMemory)(pdevGtr->gtrpvt,pdevGtr->rawChannels.papgtrchannel);
        if(status!=gtrStatusOK)
            printf("devGtr: myCallback raw read failed\n");
    }
    scanIoRequest(pdevGtr->ioscanpvt);
}

static void interruptHandler(void *pvt)
{
    devGtr *pdevGtr = (devGtr *)pvt;

    callbackRequest(&pdevGtr->callback);
}

static long get_ioint_info(int cmd, dbCommon *precord, IOSCANPVT *pvt)
{
    dpvt *pdpvt;
    devGtr *pdevGtr;

    pdpvt = precord->dpvt;
    if(!pdpvt) return(-1);
    pdevGtr = pdpvt->pdevGtr;
    *pvt = pdevGtr->ioscanpvt;
    return(0);
}

static void
allocateChannels(devGtrChannels *pdevgtrchannels, int nchannels)
{
    int ind;

    pdevgtrchannels->nchannels = nchannels;
    if(pdevgtrchannels->nchannels != 0) {
        pdevgtrchannels->pachannel = calloc(pdevgtrchannels->nchannels,sizeof(gtrchannel));
        pdevgtrchannels->papgtrchannel = calloc(pdevgtrchannels->nchannels,sizeof(gtrchannel *));
        for(ind=0;ind<pdevgtrchannels->nchannels; ind++)
            pdevgtrchannels->papgtrchannel[ind] = &pdevgtrchannels->pachannel[ind];
    }
}

static dpvt *common_init_record(dbCommon *precord,DBLINK *plink,
    char **parmString,int nparmStrings)
{
    gtrPvt gtrpvt;
    gtrops *pgtrops;
    struct vmeio *pvmeio;
    devGtr *pdevGtr;
    dpvt   *pdpvt;
    int ind;
    char *parm = 0;

    if(plink->type!=VME_IO) {
        recGblRecordError(S_db_badField,(void *)precord, "devGtr NOT VME_IO");
        goto bad;
    }
    pvmeio = &(plink->value.vmeio);
    gtrpvt = gtrFind(pvmeio->card,&pgtrops);
    if(!gtrpvt) {
        recGblRecordError(S_db_badField,(void *)precord, "devGtr no card");
        goto bad;
    }
    (*pgtrops->lock)(gtrpvt);
    pdevGtr = (*pgtrops->getUser)(gtrpvt);
    if(!pdevGtr) {
        pdevGtr = dbCalloc(1,sizeof(devGtr));
        pdevGtr->gtrpvt = gtrpvt;
        pdevGtr->pgtrops = pgtrops;
#if 1
		allocateChannels(&pdevGtr->channels, 6);
		allocateChannels(&pdevGtr->rawChannels, 6);
#else
        allocateChannels(&pdevGtr->channels, (*pgtrops->numberChannels)(gtrpvt));
        allocateChannels(&pdevGtr->rawChannels, (*pgtrops->numberRawChannels)(gtrpvt));
#endif		
        (*pgtrops->setUser)(gtrpvt,pdevGtr);
        callbackSetCallback(myCallback,&pdevGtr->callback);
        callbackSetUser(pdevGtr,&pdevGtr->callback);
        callbackSetPriority(priorityLow,&pdevGtr->callback);
        (*pgtrops->registerHandler)(gtrpvt,interruptHandler,pdevGtr);
        scanIoInit(&pdevGtr->ioscanpvt);
        (*pgtrops->setUser)(gtrpvt,pdevGtr);
    }
    (*pgtrops->unlock)(gtrpvt);
    if(pvmeio->parm) parm = pvmeio->parm;
    if(!pvmeio->parm || !parm) {
        recGblRecordError(S_db_badField,(void *)precord, "devGtr no parm");
        goto bad;
    }
    if(strcmp(parm,"setDefault")==0) {
        ind = 0;
    }
    else {
        for(ind=0; ind<nparmStrings; ind++) {
            if(strcmp(parm,parmString[ind])==0) break;
        }
    }
    if(ind>=nparmStrings) {
        recGblRecordError(S_db_badField,(void *)precord, "devGtr bad parm");
        goto bad;
    }
    pdpvt = dbCalloc(1,sizeof(dpvt));
    pdpvt->pdevGtr = pdevGtr;
    pdpvt->parm = ind;
    precord->dpvt = pdpvt;
    return(pdpvt);
bad:
    precord->pact = 1;
    return(0);
}

static long ai_init_record(dbCommon *precord)
{
    aiRecord *paiRecord = (aiRecord *)precord;
    common_init_record(precord,&paiRecord->inp,aiParmString,NAIPARM);

    return(0);		
}

static long ai_read(dbCommon *precord)
{ 
    aiRecord *paiRecord = (aiRecord *)precord;
    devGtr *pdevGtr;
    dpvt *pdpvt;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
	struct vmeio *pvmeio;
    long status = 0;

	pdpvt = paiRecord->dpvt;
    if(!pdpvt) return(0);
    pdevGtr = pdpvt->pdevGtr;
    if(!pdevGtr) return(0);
    gtrpvt = pdevGtr->gtrpvt;
    pgtrops = pdevGtr->pgtrops;

	(*pgtrops->lock)(gtrpvt);
	
	pvmeio = &(paiRecord->inp.value.vmeio);
	status = (*pgtrops->aiFunction)(pdpvt->parm, gtrpvt, &paiRecord->val, pvmeio->signal);
	
	paiRecord->udf=isnan(paiRecord->val);
	(*pgtrops->unlock)(gtrpvt);

    return(2);
}

static long ao_init_record(dbCommon *precord)
{
	aoRecord *paoRecord = (aoRecord *)precord;
	dpvt *pdpvt;
	devGtr *pdevGtr;
	gtrPvt gtrpvt;
	gtrops *pgtrops;
	struct vmeio *pvmeio;
	long status = 0;

	common_init_record(precord,&paoRecord->out,aoParmString,NAOPARM);

	pdpvt = paoRecord->dpvt;
    if(!pdpvt) {
        status = S_dev_NoInit;
        recGblRecordError(status,(void *)paoRecord,
            "devGtr init_record failed");
        paoRecord->pact = 1;
        return(status);
    }

	pdevGtr = pdpvt->pdevGtr;
	gtrpvt = pdevGtr->gtrpvt;
	pgtrops = pdevGtr->pgtrops;

	pvmeio = &(paoRecord->out.value.vmeio);
	status = (*pgtrops->aoFunction)(pdpvt->parm, gtrpvt, paoRecord->val, pvmeio->signal);

	if(status!=gtrStatusOK) recGblSetSevr(paoRecord,STATE_ALARM,MINOR_ALARM);
	return(2);
}

static long ao_write(dbCommon *precord)
{
    aoRecord *paoRecord = (aoRecord *)precord;
    dpvt *pdpvt;
    devGtr *pdevGtr;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
	struct vmeio *pvmeio;
    long status = 0;

    pdpvt = paoRecord->dpvt;
    if(!pdpvt) {
        status = S_dev_NoInit;
        recGblRecordError(status,(void *)paoRecord,
            "devGtr init_record failed");
        paoRecord->pact = 1;
        return(status);
    }
    pdevGtr = pdpvt->pdevGtr;
    gtrpvt = pdevGtr->gtrpvt;
    pgtrops = pdevGtr->pgtrops;
    (*pgtrops->lock)(gtrpvt);

	pvmeio = &(paoRecord->out.value.vmeio);
	status = (*pgtrops->aoFunction)(pdpvt->parm ,gtrpvt, paoRecord->val, pvmeio->signal);
	
    (*pgtrops->unlock)(gtrpvt);
    if(status!=gtrStatusOK) recGblSetSevr(paoRecord,STATE_ALARM,MINOR_ALARM);
    return(0);
}

static long bi_init_record(dbCommon *precord)
{
	biRecord *pbiRecord = (biRecord *)precord;
	dpvt *pdpvt;
	devGtr *pdevGtr;
	gtrPvt gtrpvt;
	gtrops *pgtrops;
	struct vmeio *pvmeio;
	long status = 0;

	common_init_record(precord,&pbiRecord->inp,biParmString,NBIPARM);

	pdpvt = pbiRecord->dpvt;
	if(!pdpvt) {
		status = S_dev_NoInit;
		recGblRecordError(status, (void *)pbiRecord,
			"devGtr init_record failed");
		pbiRecord->pact = 1;
		return (status);
	}

	pdevGtr = pdpvt->pdevGtr;
	gtrpvt = pdevGtr->gtrpvt;
	pgtrops = pdevGtr->pgtrops;

	pvmeio = &(pbiRecord->inp.value.vmeio);

	status = (*pgtrops->biFunction)(pdpvt->parm, gtrpvt, &pbiRecord->val, pvmeio->signal);

	if(status!=gtrStatusOK) recGblSetSevr(pbiRecord, STATE_ALARM, MINOR_ALARM);
	return (2);
}

static long bi_read(dbCommon * precord)
{
	biRecord *pbiRecord = (biRecord *)precord;
	devGtr *pdevGtr;
	dpvt *pdpvt;
	gtrPvt gtrpvt;
	gtrops *pgtrops;
	struct vmeio *pvmeio;
	long status = 0;

	pdpvt = pbiRecord->dpvt;
	if(!pdpvt) return (0);
	pdevGtr = pdpvt->pdevGtr;
	if(!pdevGtr) return (0);
	gtrpvt = pdevGtr->gtrpvt;
	pgtrops = pdevGtr->pgtrops;

	(*pgtrops->lock)(gtrpvt);

	pvmeio = &(pbiRecord->inp.value.vmeio);
	status = (*pgtrops->biFunction)(pdpvt->parm, gtrpvt, &pbiRecord->val, pvmeio->signal);

	pbiRecord->udf=isnan(pbiRecord->val);
	(*pgtrops->unlock)(gtrpvt);
	if(status!=gtrStatusOK) recGblSetSevr(pbiRecord, STATE_ALARM, MINOR_ALARM);

	return(2);
}

static long bo_init_record(dbCommon *precord)
{
    boRecord *pboRecord = (boRecord *)precord;
    dpvt *pdpvt;
    devGtr *pdevGtr;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
    struct vmeio *pvmeio;
    long status = 0;

    common_init_record(precord,&pboRecord->out,boParmString,NBOPARM);

    pdpvt = pboRecord->dpvt;
    if(!pdpvt) {
        status = S_dev_NoInit;
        recGblRecordError(status,(void *)pboRecord,
            "devGtr init_record failed");
        pboRecord->pact = 1;
        return(status);
    }
    pdevGtr = pdpvt->pdevGtr;
    gtrpvt = pdevGtr->gtrpvt;
    pgtrops = pdevGtr->pgtrops;

	pvmeio = &(pboRecord->out.value.vmeio);
	status = (*pgtrops->boFunction)(pdpvt->parm ,gtrpvt, pboRecord->val, pvmeio->signal);

	if(status!=gtrStatusOK) recGblSetSevr(pboRecord,STATE_ALARM,MINOR_ALARM);
    return(2);
}

static long bo_write(dbCommon *precord)
{
    boRecord *pboRecord = (boRecord *)precord;
    dpvt *pdpvt;
    devGtr *pdevGtr;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
    struct vmeio *pvmeio;
    long status = 0;

    pdpvt = pboRecord->dpvt;
    if(!pdpvt) {
        status = S_dev_NoInit;
        recGblRecordError(status,(void *)pboRecord,
            "devGtr init_record failed");
        pboRecord->pact = 1;
        return(status);
    }
    pdevGtr = pdpvt->pdevGtr;
    gtrpvt = pdevGtr->gtrpvt;
    pgtrops = pdevGtr->pgtrops;

    (*pgtrops->lock)(gtrpvt);

	pvmeio = &(pboRecord->out.value.vmeio);
	status = (*pgtrops->boFunction)(pdpvt->parm ,gtrpvt, pboRecord->val, pvmeio->signal);

    (*pgtrops->unlock)(gtrpvt);
    if(status!=gtrStatusOK) recGblSetSevr(pboRecord,STATE_ALARM,MINOR_ALARM);
    return(0);
}

static long longout_init_record(dbCommon *precord)
{
    longoutRecord *plongoutRecord = (longoutRecord *)precord;
    dpvt *pdpvt;
    devGtr *pdevGtr;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
    long status = 0;

    common_init_record(precord,&plongoutRecord->out,longoutParmString,NLOPARM);

    pdpvt = plongoutRecord->dpvt;
    if(!pdpvt) {
        status = S_dev_NoInit;
        recGblRecordError(status,(void *)plongoutRecord,
            "devGtr init_record failed");
        plongoutRecord->pact = 1;
        return(status);
    }
    pdevGtr = pdpvt->pdevGtr;
    gtrpvt = pdevGtr->gtrpvt;
    pgtrops = pdevGtr->pgtrops;

	status = (*pgtrops->longoutFunction)(pdpvt->parm, gtrpvt, plongoutRecord->val);

	if(status!=gtrStatusOK) recGblSetSevr(plongoutRecord,STATE_ALARM,MINOR_ALARM);
    return(2);
}

static long longout_write(dbCommon *precord)
{
    longoutRecord *plongoutRecord = (longoutRecord *)precord;
    dpvt *pdpvt;
    devGtr *pdevGtr;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
    long status = 0;

    pdpvt = plongoutRecord->dpvt;
    if(!pdpvt) {
        status = S_dev_NoInit;
        recGblRecordError(status,(void *)plongoutRecord,
            "devGtr init_record failed");
        plongoutRecord->pact = 1;
        return(status);
    }
    pdevGtr = pdpvt->pdevGtr;
    gtrpvt = pdevGtr->gtrpvt;
    pgtrops = pdevGtr->pgtrops;
    (*pgtrops->lock)(gtrpvt);

	status = (*pgtrops->longoutFunction)(pdpvt->parm, gtrpvt, plongoutRecord->val);
    
    (*pgtrops->unlock)(gtrpvt);
    if(status!=gtrStatusOK) recGblSetSevr(plongoutRecord,STATE_ALARM,MINOR_ALARM);
    return(0);
}

#define ndefaultChoices 2
static char *defaultChoices[ndefaultChoices] = {
    "notSupported","notSupported"
};

static long mbbo_init_record(dbCommon *precord)
{
    mbboRecord *pmbboRecord = (mbboRecord *)precord;
    dpvt *pdpvt;
    devGtr *pdevGtr;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
    long status;

    common_init_record(precord,&pmbboRecord->out,mbboParmString,NMBBOPARM);
    pdpvt = pmbboRecord->dpvt;
    if(!pdpvt) return(2);
    pdevGtr = pdpvt->pdevGtr;
    gtrpvt = pdevGtr->gtrpvt;
    pgtrops = pdevGtr->pgtrops;

	status = (*pgtrops->mbboFunction)(pdpvt->parm,gtrpvt,pmbboRecord->val);
	
	if(status!=gtrStatusOK) recGblSetSevr(pmbboRecord,STATE_ALARM,MINOR_ALARM);
    return(2);
}

static long mbbo_write(dbCommon *precord)
{
    mbboRecord *pmbboRecord = (mbboRecord *)precord;
    dpvt *pdpvt;
    devGtr *pdevGtr;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
    long status = 0;

    pdpvt = pmbboRecord->dpvt;
    if(!pdpvt) {
        status = S_dev_NoInit;
        recGblRecordError(status,(void *)pmbboRecord,
            "devGtr init_record failed");
        pmbboRecord->pact = 1;
        return(status);
    }
    pdevGtr = pdpvt->pdevGtr;
    gtrpvt = pdevGtr->gtrpvt;
    pgtrops = pdevGtr->pgtrops;
    (*pgtrops->lock)(gtrpvt);

	status = (*pgtrops->mbboFunction)(pdpvt->parm,gtrpvt,pmbboRecord->val);
    
    (*pgtrops->unlock)(gtrpvt);
    if(status!=gtrStatusOK) recGblSetSevr(pmbboRecord,STATE_ALARM,MINOR_ALARM);
    return(0);
}

static long stringin_init_record(dbCommon *precord)
{
    stringinRecord *pstringinRecord = (stringinRecord *)precord;
    dpvt *pdpvt;
    devGtr *pdevGtr;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
    long status;

    common_init_record(precord,&pstringinRecord->inp,stringinParmString,NSIPARM);

	pdpvt = pstringinRecord->dpvt;
    if(!pdpvt) return(0);
    pdevGtr = pdpvt->pdevGtr;
    if(!pdevGtr) return(0);
    gtrpvt = pdevGtr->gtrpvt;
    pgtrops = pdevGtr->pgtrops;

	status = (*pgtrops->stringinFunction)(pdpvt->parm, gtrpvt, pstringinRecord->val, sizeof(pstringinRecord->val), 1);
	
}

static long stringin_read(dbCommon *precord)
{ 
    stringinRecord *pstringinRecord = (stringinRecord *)precord;
    dpvt *pdpvt;
    devGtr *pdevGtr;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
    long status;

    common_init_record(precord,&pstringinRecord->inp,stringinParmString,NSIPARM);

	pdpvt = pstringinRecord->dpvt;
    if(!pdpvt) return(0);
    pdevGtr = pdpvt->pdevGtr;
    if(!pdevGtr) return(0);
    gtrpvt = pdevGtr->gtrpvt;
    pgtrops = pdevGtr->pgtrops;

	(*pgtrops->lock)(gtrpvt);

	status = (*pgtrops->stringinFunction)(pdpvt->parm, gtrpvt, pstringinRecord->val, sizeof(pstringinRecord->val), 0);

    (*pgtrops->unlock)(gtrpvt);

	return(0);
}


static long stringout_init_record(dbCommon *precord)
{
    stringoutRecord *pstringoutRecord = (stringoutRecord *)precord;

    common_init_record(precord,&pstringoutRecord->out,stringoutParmString,NSOPARM);

    return (0);
}

static long stringout_write(dbCommon *precord)
{
    stringoutRecord *pstringoutRecord = (stringoutRecord *)precord;
    dpvt *pdpvt;
    devGtr *pdevGtr;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
    long status = 0;

    pdpvt = pstringoutRecord->dpvt;
    if(!pdpvt) return(2);
    pdevGtr = pdpvt->pdevGtr;
    gtrpvt = pdevGtr->gtrpvt;
    pgtrops = pdevGtr->pgtrops;
    (*pgtrops->lock)(gtrpvt);

	status = (*pgtrops->stringoutFunction)(pdpvt->parm,gtrpvt,pstringoutRecord->val);
    
    (*pgtrops->unlock)(gtrpvt);
    if(status!=gtrStatusOK) recGblSetSevr(pstringoutRecord,STATE_ALARM,MINOR_ALARM);
	return (0);
}

static long waveform_init_record(dbCommon *precord)
{
    waveformRecord *pwaveformRecord = (waveformRecord *)precord;
    devGtr *pdevGtr;
    dpvt *pdpvt;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
    long status = 0;
    struct vmeio *pvmeio;
    gtrchannel *pgtrchannel;
    devGtrChannels *pdevgtrchannels;
    int signal;
    int ftvl = pwaveformRecord->ftvl;

    pdpvt = common_init_record(precord,&pwaveformRecord->inp,
        waveformParmString,NWFPARM);

    if(!pdpvt) return(0);
    pdevGtr = pdpvt->pdevGtr;
    if(!pdevGtr) return(0);
    gtrpvt = pdevGtr->gtrpvt;
    pgtrops = pdevGtr->pgtrops;
    switch(pdpvt->parm) {
    case readData:     pdevgtrchannels=&pdevGtr->channels;     break;
    case readRawData:  pdevgtrchannels=&pdevGtr->rawChannels;  break;
    default:           return(S_db_badField);
    }
    switch(ftvl) {
    default:
        status = S_db_badField;
        recGblRecordError(status,(void *)precord,
            "FTVL must be SHORT FLOAT or DOUBLE");
        pwaveformRecord->pact = 1;
        return(status);
    
    case menuFtypeLONG: /* Limits must come from database */
            break;

	case menuFtypeUSHORT:
    case menuFtypeSHORT: {
        short rawLow,rawHigh;
#if 1
		rawLow = 0;
		rawHigh = 0xFFFF;
#else
        (*pgtrops->getLimits)(gtrpvt,&rawLow,&rawHigh);
#endif
        pwaveformRecord->hopr = rawHigh;
        pwaveformRecord->lopr = rawLow;
        }
        break;

    case menuFtypeFLOAT:
    case menuFtypeDOUBLE:
        pwaveformRecord->hopr = 1.0;
        pwaveformRecord->lopr = 0.0;
        break;
    }
    pvmeio = &(pwaveformRecord->inp.value.vmeio);
    signal = pvmeio->signal;
    if(signal<0 || signal>=pdevgtrchannels->nchannels) {
        status = S_db_badField;
        signal = 0;
        recGblRecordError(status,(void *)precord,
            "devGtr Illegal signal");
        pwaveformRecord->pact = 1;
        return(status);
    }
    pdpvt->signal = pvmeio->signal;
    pgtrchannel = &pdevgtrchannels->pachannel[pdpvt->signal];
    if(((ftvl==menuFtypeUSHORT)||(ftvl==menuFtypeSHORT)||(ftvl==menuFtypeLONG)) && !pgtrchannel->pdata) {
        pgtrchannel->pdata = pwaveformRecord->bptr;
        pgtrchannel->len = pwaveformRecord->nelm;
        pgtrchannel->ftvl = ftvl;
        pdpvt->isPdataBptr = 1;
    } else if(!pgtrchannel->pdata || pgtrchannel->len<pwaveformRecord->nelm) {
        if(pgtrchannel->pdata && !pdpvt->isPdataBptr) free(pgtrchannel->pdata);
        pdpvt->isPdataBptr = 0;
        pgtrchannel->pdata = dbCalloc(pwaveformRecord->nelm, sizeof(int16));
        pgtrchannel->len = pwaveformRecord->nelm;
        pgtrchannel->ftvl = menuFtypeSHORT;
    }
    precord->dpvt = pdpvt;
    pdevgtrchannels->hasWaveforms=1;

    return(0);
}

static long waveform_read(dbCommon *precord)
{
    waveformRecord *pwaveformRecord = (waveformRecord *)precord;
    dpvt *pdpvt;
    devGtr *pdevGtr;
    gtrPvt gtrpvt;
    gtrops *pgtrops;
    long status;
    int ndata;
    gtrchannel *pgtrchannel;
    devGtrChannels *pdevgtrchannels;

    pdpvt = pwaveformRecord->dpvt;
    if(!pdpvt) {
        status = S_dev_NoInit;
        recGblRecordError(status,(void *)precord,
            "devGtr init_record failed");
        precord->pact = 1;
        return(status);
    }
    pdevGtr = pdpvt->pdevGtr;
    gtrpvt = pdevGtr->gtrpvt;
    pgtrops = pdevGtr->pgtrops;
    switch(pdpvt->parm) {
    case readData:     pdevgtrchannels=&pdevGtr->channels;     break;
    case readRawData:  pdevgtrchannels=&pdevGtr->rawChannels;  break;
    default:           return(S_db_badField);
    }
    pgtrchannel = &pdevgtrchannels->pachannel[pdpvt->signal];
    ndata = pgtrchannel->ndata;
    if(ndata>pwaveformRecord->nelm) ndata = pwaveformRecord->nelm;
    if(ndata>0 ) {
        pwaveformRecord->nord = ndata;
    } else {
        recGblSetSevr(precord,STATE_ALARM,MINOR_ALARM);
        return(0);
    }
    if(pwaveformRecord->bptr==pgtrchannel->pdata) return(0);
    if((pwaveformRecord->ftvl == menuFtypeSHORT) || (pwaveformRecord->ftvl == menuFtypeUSHORT)) {
        memcpy(pwaveformRecord->bptr,pgtrchannel->pdata,ndata*sizeof(int16));
    } else if(pwaveformRecord->ftvl == menuFtypeLONG) {
        memcpy(pwaveformRecord->bptr,pgtrchannel->pdata,ndata*sizeof(long));
    } else {
        int16 rawLow,rawHigh;
        int16 *pfrom = pgtrchannel->pdata;
        int ind;
#if 1		
		rawLow = 0;
		rawHigh = 0xFFFF;		
#else		
        (*pgtrops->getLimits)(gtrpvt,&rawLow,&rawHigh);
#endif		
        if(pwaveformRecord->ftvl==menuFtypeFLOAT) {
            float *pto = (float *)pwaveformRecord->bptr;
            float low,high,diff;
            low = (float)rawLow; high = (float)rawHigh; diff = high - low;
            for(ind=0; ind<ndata; ind++)
                *pto++ = ((float)(*pfrom++) -low)/diff;
        } else if(pwaveformRecord->ftvl==menuFtypeDOUBLE) {
            double *pto = (double *)pwaveformRecord->bptr;
                double low,high,diff;
                low = (double)rawLow; high = (double)rawHigh; diff = high - low;
                for(ind=0; ind<ndata; ind++)
                    *pto++ = ((double)(*pfrom++) -low)/diff;
        } else {
            recGblRecordError(S_db_badField,(void *)precord,
                "devGtr FTVL must be SHORT or FLOAT or DOUBLE");
            pwaveformRecord->pact = 1;
        }
    }
	
    return(0);
}
