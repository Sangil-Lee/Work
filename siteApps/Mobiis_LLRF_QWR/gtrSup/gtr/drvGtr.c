/*drvGtr.c */

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

#include <epicsMutex.h>
#include <epicsAssert.h>
#include <epicsExport.h>
#include <ellLib.h>
#include <errlog.h>
#include <drvSup.h>

#include "drvGtr.h"

#define STATIC static

typedef struct gtrInfo {
    ELLNODE node;
    epicsMutexId  lock;
    int     card;
    const char *name;
    gtrops  *pgtrdrvops;
    gtrPvt  drvPvt;
    void    *userPvt;
} gtrInfo;

static ELLLIST gtrList;
static int gtrIsInited = 0;
    
static void gtrinitialize()
{
    if(gtrIsInited) return;
    gtrIsInited=1;
    ellInit(&gtrList);
}

STATIC void gtrinit(gtrPvt pvt)
{
    gtrInfo *pgtrInfo = (gtrInfo *)pvt;

    if(pgtrInfo->pgtrdrvops->init)
        (*pgtrInfo->pgtrdrvops->init)(pgtrInfo->drvPvt);
}

STATIC void gtrreport(gtrPvt pvt,int level)
{
    gtrInfo *pgtrInfo = (gtrInfo *)pvt;

    if(pgtrInfo->pgtrdrvops->report) {
        (*pgtrInfo->pgtrdrvops->report)(pgtrInfo->drvPvt,level);
    } else {
        printf("gtr card %d name %s\n", pgtrInfo->card,pgtrInfo->name);
    }
}

/* ai */
STATIC gtrStatus gtraiFunction(long parm, gtrPvt pvt, double *value, int index)
{
	gtrInfo *pgtrInfo = (gtrInfo *)pvt;

	if(pgtrInfo->pgtrdrvops->aiFunction) {
		return (*pgtrInfo->pgtrdrvops->aiFunction)(parm,pgtrInfo->drvPvt,value,index);
	} else {
		*value;
		return(gtrStatusError);
	}
}

/* ao */
STATIC gtrStatus gtraoFunction(long parm, gtrPvt pvt, double value, int index)
{
	gtrInfo *pgtrInfo = (gtrInfo *)pvt;

	if(pgtrInfo->pgtrdrvops->aoFunction) {
		return (*pgtrInfo->pgtrdrvops->aoFunction)(parm,pgtrInfo->drvPvt,value,index);
	} else {
		return(gtrStatusError);
	}
}

/*bi */
STATIC gtrStatus gtrbiFunction(long parm, gtrPvt pvt, int *value, int index)
{
	gtrInfo *pgtrInfo = (gtrInfo *)pvt;

	if(pgtrInfo->pgtrdrvops->biFunction) {
		return (*pgtrInfo->pgtrdrvops->biFunction)(parm,pgtrInfo->drvPvt,value,index);
	} else {
		*value;
		return(gtrStatusError);
	}
}

/* bo */
STATIC gtrStatus gtrboFunction(long parm, gtrPvt pvt, int value, int index)
{
	gtrInfo *pgtrInfo = (gtrInfo *)pvt;

	if(pgtrInfo->pgtrdrvops->boFunction) {
		return (*pgtrInfo->pgtrdrvops->boFunction)(parm,pgtrInfo->drvPvt,value,index);
	} else {
		return(gtrStatusError);
	}
}

/* longout */
STATIC gtrStatus gtrlongoutFunction(long parm, gtrPvt pvt, int value)
{
	gtrInfo *pgtrInfo = (gtrInfo *)pvt;

	if(pgtrInfo->pgtrdrvops->longoutFunction) {
		return (*pgtrInfo->pgtrdrvops->longoutFunction)(parm,pgtrInfo->drvPvt,value);
	} else {
		return(gtrStatusError);
	}
}

/* mbbo */
STATIC gtrStatus gtrmbboFunction(long parm, gtrPvt pvt, int value)
{
	gtrInfo *pgtrInfo = (gtrInfo *)pvt;

	if(pgtrInfo->pgtrdrvops->mbboFunction) {
		return (*pgtrInfo->pgtrdrvops->mbboFunction)(parm,pgtrInfo->drvPvt,value);
	} else {
		return(gtrStatusError);
	}
}

STATIC gtrStatus gtrregisterHandler(gtrPvt pvt,gtrhandler usrIH,void *handlerPvt)
{
    gtrInfo *pgtrInfo = (gtrInfo *)pvt;
    
    if(pgtrInfo->pgtrdrvops->registerHandler) {
        return (*pgtrInfo->pgtrdrvops->registerHandler)(
            pgtrInfo->drvPvt,usrIH,handlerPvt);
    } else {
        return(gtrStatusError);
    }
}

STATIC gtrStatus gtrreadMemory(gtrPvt pvt, gtrchannel **papgtrchannel)
{
    gtrInfo *pgtrInfo = (gtrInfo *)pvt;
    
    if(pgtrInfo->pgtrdrvops->readMemory) {
        return (*pgtrInfo->pgtrdrvops->readMemory)(pgtrInfo->drvPvt,papgtrchannel);
    } else {
        return(gtrStatusError);
    }
}

STATIC gtrStatus gtrreadRawMemory(gtrPvt pvt, gtrchannel **papgtrchannel)
{
    gtrInfo *pgtrInfo = (gtrInfo *)pvt;
    
    if(pgtrInfo->pgtrdrvops->readRawMemory) {
        return (*pgtrInfo->pgtrdrvops->readRawMemory)(pgtrInfo->drvPvt,papgtrchannel);
    } else {
        return(gtrStatusError);
    }
}


/* stringin */
STATIC gtrStatus gtrstringinFunction(long parm, gtrPvt pvt, char *pname, int maxchars, int initFlag)
{
	gtrInfo *pgtrInfo = (gtrInfo *)pvt;
    
    if(pgtrInfo->pgtrdrvops->stringinFunction) {
        return (*pgtrInfo->pgtrdrvops->stringinFunction)(parm,pgtrInfo->drvPvt,pname,maxchars,initFlag);
    } else {
        return(gtrStatusError);
    }
}

/* stringout */
STATIC gtrStatus gtrstringoutFunction(long parm, gtrPvt pvt, const char *pvalue)
{
	gtrInfo *pgtrInfo = (gtrInfo *)pvt;

	if(pgtrInfo->pgtrdrvops->stringoutFunction) {
		return (*pgtrInfo->pgtrdrvops->stringoutFunction)(parm,pgtrInfo->drvPvt,pvalue);
	} else {
		return (gtrStatusError);
	}
}


STATIC void gtrsetUser(gtrPvt pvt,void * userPvt)
{
    gtrInfo *pgtrInfo = (gtrInfo *)pvt;
    pgtrInfo->userPvt = userPvt;
}

STATIC void *gtrgetUser(gtrPvt pvt)
{
    gtrInfo *pgtrInfo = (gtrInfo *)pvt;
    return(pgtrInfo->userPvt);
}

STATIC void gtrlock(gtrPvt pvt)
{
    gtrInfo *pgtrInfo = (gtrInfo *)pvt;
    epicsMutexLock(pgtrInfo->lock);
}

STATIC void gtrunlock(gtrPvt pvt)
{
    gtrInfo *pgtrInfo = (gtrInfo *)pvt;
    epicsMutexUnlock(pgtrInfo->lock);
}

static gtrops ops = {
gtrinit,
gtrreport,
gtraiFunction,
gtraoFunction,
gtrbiFunction,
gtrboFunction,
gtrlongoutFunction,
gtrmbboFunction,
gtrregisterHandler,
gtrreadMemory,
gtrreadRawMemory,
gtrstringinFunction,
gtrstringoutFunction,
gtrsetUser,
gtrgetUser,
gtrlock,
gtrunlock
};

gtrPvt gtrFind(int card,gtrops **ppgtrops)
{
    gtrInfo  *pgtrInfo;

    if(!gtrIsInited) gtrinitialize();
    pgtrInfo = (gtrInfo *)ellFirst(&gtrList);
    while(pgtrInfo) {
        if(pgtrInfo->card == card) break;
        pgtrInfo = (gtrInfo *)ellNext(&pgtrInfo->node);
    }
    *ppgtrops = &ops;
    return(pgtrInfo);
}

void gtrRegisterDriver(int card,
    const char *name,gtrops *pgtrdrvops,gtrPvt drvPvt)
{
    gtrInfo *pgtrInfo;
    gtrops *pgtrops;

    if(!gtrIsInited) gtrinitialize();
    if(gtrFind(card,&pgtrops)) {
        printf("gtrRegisterDriver card is already registered\n");
        return;
    }
    pgtrInfo = calloc(1,sizeof(gtrInfo));
    if(!pgtrInfo) {
        printf("gtrConfig: calloc failed\n");
        return;
    }
    pgtrInfo->lock = epicsMutexCreate();
    if(!pgtrInfo->lock) {
        printf("gtrConfig: semMCreate failed\n");
        return;
    }
    pgtrInfo->card = card;
    pgtrInfo->name = name;
    pgtrInfo->pgtrdrvops = pgtrdrvops;
    pgtrInfo->drvPvt = drvPvt;
    pgtrInfo->userPvt = 0;
    ellAdd(&gtrList,&pgtrInfo->node);
}

STATIC long drvGtrReport(int level)
{
    gtrInfo  *pgtrInfo;
    pgtrInfo = (gtrInfo *)ellFirst(&gtrList);
    while(pgtrInfo) {
        gtrreport(pgtrInfo,level);
        pgtrInfo = (gtrInfo *)ellNext(&pgtrInfo->node);
    }
    return(0);
}

STATIC long drvGtrInit()
{
    gtrInfo  *pgtrInfo;

    if(!gtrIsInited) gtrinitialize();
    pgtrInfo = (gtrInfo *)ellFirst(&gtrList);
    while(pgtrInfo) {
        gtrinit(pgtrInfo);
        pgtrInfo = (gtrInfo *)ellNext(&pgtrInfo->node);
    }
    return(0);
}

struct drvet drvGtr = {
2,
drvGtrReport,
drvGtrInit
};
epicsExportAddress(drvet,drvGtr);
