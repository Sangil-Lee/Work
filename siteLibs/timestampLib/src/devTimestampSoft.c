#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "alarm.h"
#include "callback.h"
#include "cvtTable.h"
#include "dbDefs.h"
#include "dbAccess.h"
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "link.h"
#include "dbCommon.h"

#include "timestampRecord.h"

#include "epicsExport.h"


LOCAL long devTimestampSoft_read_timestamp(timestampRecord *precord);

struct {
    long    number;
    DEVSUPFUN    report;
    DEVSUPFUN    init;
    DEVSUPFUN    init_record;
    DEVSUPFUN    get_ioint_info;
    DEVSUPFUN    read_timestamp;
} devTimestampSoft = {
    5,
    NULL,
    NULL,
    NULL,
    NULL,
    devTimestampSoft_read_timestamp
};

epicsExportAddress(dset, devTimestampSoft);


LOCAL long devTimestampSoft_read_timestamp(timestampRecord *precord)
{
    recGblGetTimeStamp(precord);

    return 0;
}

