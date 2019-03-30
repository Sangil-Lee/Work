/*************************************************************************\
* Copyright (c) 2016 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Revision-Id$
 *
 *      Original Author: Janet Anderson
 *      Date: 09-23-91
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "devSup.h"
#include "int64inRecord.h"
#include "epicsExport.h"

/* Create the dset for devI64inSoft */
static long init_record(int64inRecord *prec);
static long read_int64in(int64inRecord *prec);

struct {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read_int64in;
} devI64inSoft = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    read_int64in
};
epicsExportAddress(dset, devI64inSoft);

static long init_record(int64inRecord *prec)
{
    /* INP must be CONSTANT, PV_LINK, DB_LINK or CA_LINK*/
    switch (prec->inp.type) {
    case CONSTANT:
        if (recGblInitConstantLink(&prec->inp, DBF_INT64, &prec->val))
            prec->udf = FALSE;
        break;
    case PV_LINK:
    case DB_LINK:
    case CA_LINK:
        break;
    default:
        recGblRecordError(S_db_badField, (void *)prec,
            "devI64inSoft (init_record) Illegal INP field");
        return S_db_badField;
    }
    return 0;
}

static long read_int64in(int64inRecord *prec)
{
    long status;

    status = dbGetLink(&prec->inp, DBR_INT64, &prec->val, 0, 0);
    if (!status &&
        prec->tsel.type == CONSTANT &&
        prec->tse == epicsTimeEventDeviceTime)
        dbGetTimeStamp(&prec->inp, &prec->time);
    return status;
}
