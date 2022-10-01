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


long initRecordMbbiDI(struct mbbiDirectRecord* pRec);
long readPXI6514Mbbi(struct mbbiDirectRecord* pRec);

long initRecordMbboDO(struct mbboDirectRecord *pRec);
long writePXI6514Mbbo(struct mbboDirectRecord *pRec);

long initRecordDI(struct longinRecord* pRec);
long readPXI6514(struct longinRecord* pRec);

long initRecordDO(struct longoutRecord* pRec);
long writePXI6514(struct longoutRecord* pRec);

long initRecordPXIAO(struct aoRecord* pRec, int pass);
long writePXIAO(struct aoRecord* pRec);

long initRecordAIPXIAO(struct aiRecord* pRec);
long writePXIAI(struct aiRecord* pRec);


//#ifdef __cplusplus
//}
//#endif

#endif
