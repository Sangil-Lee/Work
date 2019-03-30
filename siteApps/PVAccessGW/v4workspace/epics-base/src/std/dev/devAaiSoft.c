/*************************************************************************\
* Copyright (c) 2008 UChicago Argonne LLC, as Operator of Argonne
*     National Laboratory.
* Copyright (c) 2002 The Regents of the University of California, as
*     Operator of Los Alamos National Laboratory.
* EPICS BASE is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
\*************************************************************************/

/* $Revision-Id$
 * devAaiSoft.c - Device Support Routines for soft Waveform Records
 *
 *      Original Author: Bob Dalesio
 *      Current Author:  Dirk Zimoch
 *      Date:            27-MAY-2010
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "dbConstLink.h"
#include "recGbl.h"
#include "devSup.h"
#include "cantProceed.h"
#include "menuYesNo.h"
#include "aaiRecord.h"
#include "epicsExport.h"

/* Create the dset for devAaiSoft */
static long init_record();
static long read_aai();

struct {
    long      number;
    DEVSUPFUN report;
    DEVSUPFUN init;
    DEVSUPFUN init_record;
    DEVSUPFUN get_ioint_info;
    DEVSUPFUN read_aai;
} devAaiSoft = {
    5,
    NULL,
    NULL,
    init_record,
    NULL,
    read_aai
};
epicsExportAddress(dset,devAaiSoft);

static long init_record(aaiRecord *prec)
{
    if (prec->inp.type == CONSTANT) {
        long nRequest = prec->nelm;
        long status;

        /* Allocate a buffer, record support hasn't done that yet */
        if (!prec->bptr) {
            prec->bptr = callocMustSucceed(nRequest, dbValueSize(prec->ftvl),
                "devAaiSoft: buffer calloc failed");
        }

        /* This is pass 0 so link hasn't been initialized either */
        dbConstInitLink(&prec->inp);

        status = dbLoadLinkArray(&prec->inp, prec->ftvl, prec->bptr, &nRequest);
        if (!status && nRequest > 0) {
            prec->nord = nRequest;
            prec->udf = FALSE;
        }
    }
    return 0;
}

static long read_aai(aaiRecord *prec)
{
    long nRequest = prec->nelm;
    long status = dbGetLink(prec->simm == menuYesNoYES ? &prec->siol :
        &prec->inp, prec->ftvl, prec->bptr, 0, &nRequest);

    if (!status && nRequest > 0) {
        prec->nord = nRequest;
        prec->udf = FALSE;

        if (dbLinkIsConstant(&prec->tsel) &&
            prec->tse == epicsTimeEventDeviceTime)
            dbGetTimeStamp(&prec->inp, &prec->time);
    }
    return status;
}
