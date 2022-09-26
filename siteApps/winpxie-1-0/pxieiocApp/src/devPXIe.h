#ifndef __devPXIe_h
#define __devPXIe_h

//#ifdef __cplusplus
//extern "C" {
//#endif

#include "alarm.h"
#include "dbCommon.h"
#include "dbScan.h"
#include "devSup.h"
#include "dbAccess.h"
#include "dbDefs.h"
#include "errlog.h"
#include "alarm.h"
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "dbEvent.h"
#include "epicsMath.h"
#include "epicsExport.h"


typedef long (*pxieIoFunction) (dbCommon*);

long initRecord(struct longinRecord* pRec);
long readPXI6514(struct longinRecord* pRec);

long initRecordW(struct longoutRecord* pRec);
long writePXI6514(struct longoutRecord* pRec);


//#ifdef __cplusplus
//}
//#endif

#endif