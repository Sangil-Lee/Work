/* devRTP.c,v 0.1 2014/08/01 silee, RISP 
 *
 * devRTP
 *
 * EPICS Device Support,
 *
 * silee7103@ibs.re.kr
 */

/* System */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include <string.h>

/* Base */
#include <alarm.h>
#include <cvtTable.h>
#include <dbDefs.h>
#include <dbAccess.h>
#include <recGbl.h>
#include <recSup.h>
#include <devSup.h>
#include <devLib.h>
#include <link.h>
#include <errlog.h>
#include <dbScan.h>
#include <menuConvert.h>
#include <aiRecord.h>
#include <biRecord.h>
#include <mbbiRecord.h>
#include <mbbiDirectRecord.h>
#include <stringinRecord.h>
#include <waveformRecord.h>
#include <menuFtype.h>
#include <aoRecord.h>
#include <boRecord.h>
#include <mbboRecord.h>
#include <mbboDirectRecord.h>
#include "epicsExport.h"

/* Local */
#include "drvRTP.h"

typedef struct DEV_
{
	long        number;
	DEVSUPFUN   report;
	DEVSUPFUN   init;
	DEVSUPFUN   init_record;
	DEVSUPFUN   get_ioint_info;
	DEVSUPFUN   read_write;
	DEVSUPFUN   special_linconv;
} DSET;

static long init(void)
{
	return 0;
}

/* Common initialization for all record types */
static long init_record(dbCommon *rec, const DBLINK *link)
{
	DEVRTPData *pvt = calloc (sizeof (DEVRTPData), 1);
	if (! pvt)
	{
		errlogPrintf("devRTP (%s): cannot allocate DPVT\n", rec->name);
		return -1;
	}
	if (link->type != INST_IO)
	{
		errlogPrintf("devRTP (%s): INP is not INST_IO\n", rec->name);
		return -1;
	}
	scanIoInit(&pvt->ioscanpvt);
	rec->dpvt = pvt;

	return 0;
}

static long ai_init_record(aiRecord *rec)
{
	long status = init_record((dbCommon *)rec, &rec->inp);
	/* Make sure record processing routine does not perform any conversion*/
	if (rec->linr != menuConvertSLOPE)
		rec->linr = 0;
	return status;
}

/* device support routine get_ioint_info */
static long get_ioint_info(dbCommon *rec, IOSCANPVT *ppvt)
{
	DEVRTPData *pvt = (DEVRTPData *)rec->dpvt;
	if (pvt)
		*ppvt = pvt->ioscanpvt;
	return 0;
}


static long ai_read(aiRecord *rec)
{
    DEVRTPData *pvt = (DEVRTPData *)rec->dpvt;
	printf("DEVRTPData(%p)\n", pvt);
	long status = 0;
    return status;
}

DSET devAiRTP =
{
	6,
	NULL,
	init,
	ai_init_record,
	get_ioint_info,
	ai_read,
	NULL
};

#if 0
DSET devAoRTP =
{
	6,
	NULL,
	init,
	ao_init_record,
	NULL,
	ao_write,
	NULL
};

DSET devBiRTP =
{
	6,
	NULL,
	init,
	bi_init_record,
	get_ioint_info,
	bi_read,
	NULL
};

DSET devBoRTP =
{
	6,
	NULL,
	init,
	bo_init_record,
	NULL,
	bo_write,
	NULL
};


DSET devMbbiRTP =
{
	6,
	NULL,
	init,
	mbbi_init_record,
	get_ioint_info,
	mbbi_read,
	NULL
};

DSET devMbbiDirectRTP =
{
	6,
	NULL,
	init,
	mbbi_direct_init_record,
	get_ioint_info,
	mbbi_direct_read,
	NULL
};

DSET devSiRTP =
{
	5,
	NULL,
	init,
	si_init_record,
	get_ioint_info,
	si_read
};

DSET devWfRTP =
{
	5,
	NULL,
	init,
	wf_init_record,
	get_ioint_info,
	wf_read
};

DSET devMbboRTP =
{
	6,
	NULL,
	init,
	mbbo_init_record,
	NULL,
	mbbo_write,
	NULL
};

DSET devMbboDirectRTP =
{
	6,
	NULL,
	init,
	mbbo_direct_init_record,
	NULL,
	mbbo_direct_write,
	NULL
};
#endif

epicsExportAddress(dset,devAiRTP);
#if 0
epicsExportAddress(dset,devAoRTP);
epicsExportAddress(dset,devBiRTP);
epicsExportAddress(dset,devBoRTP);

epicsExportAddress(dset,devMbbiRTP);
epicsExportAddress(dset,devMbbiDirectRTP);
epicsExportAddress(dset,devSiRTP);
epicsExportAddress(dset,devWfRTP);
epicsExportAddress(dset,devMbboRTP);
epicsExportAddress(dset,devMbboDirectRTP);
#endif

