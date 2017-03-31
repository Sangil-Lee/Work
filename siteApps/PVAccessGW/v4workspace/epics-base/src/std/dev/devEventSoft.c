/*************************************************************************\
* Copyright (c) 2008 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Revision-Id$
 *
 *      Author: Janet Anderson
 *      Date: 04-21-91
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "devSup.h"
#include "eventRecord.h"
#include "epicsExport.h"

/* Create the dset for devEventSoft */
static long init_record(eventRecord *prec);
static long read_event(eventRecord *prec);

struct {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read_event;
} devEventSoft = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    read_event
};
epicsExportAddress(dset, devEventSoft);

static long init_record(eventRecord *prec)
{
    if (recGblInitConstantLink(&prec->inp, DBF_STRING, &prec->val))
        prec->udf = FALSE;
    return 0;
}

static long read_event(eventRecord *prec)
{
    long status;
    char newEvent[MAX_STRING_SIZE];

    if (!dbLinkIsConstant(&prec->inp)) {
        status = dbGetLink(&prec->inp, DBR_STRING, newEvent, 0, 0);
        if (status) return status;
        if (strcmp(newEvent, prec->val) != 0) {
            strcpy(prec->val, newEvent);
            prec->epvt = eventNameToHandle(prec->val);
        }
    }
    prec->udf = FALSE;
    if (dbLinkIsConstant(&prec->tsel) &&
        prec->tse == epicsTimeEventDeviceTime)
        dbGetTimeStamp(&prec->inp, &prec->time);
    return 0;
}
