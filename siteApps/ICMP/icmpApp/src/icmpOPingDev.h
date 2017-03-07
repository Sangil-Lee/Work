#ifndef __ICMP_OPING_H
#define __ICMP_OPING_H

#include <vector>
#include <stdlib.h>
#include <oping.h>
    
#include "alarm.h"
#include "cvtTable.h"
#include "dbDefs.h"
#include "dbAccess.h" 
#include "recGbl.h"
#include "recSup.h"
#include "devSup.h"
#include "link.h"
#include "menuScan.h"
#include "epicsExport.h"
#include "shareLib.h"
#include "dbCommon.h"
#include "dbScan.h"
#include "callback.h"
#include "cantProceed.h"
#include "epicsStdio.h"
#include "epicsMutex.h"
#include "epicsEvent.h"
#include "epicsThread.h"
#include "iocsh.h"
#include "icmpRecord.h"
    
using namespace std;
        
#ifdef __cplusplus
extern "C" {
#endif

long init_record (icmpRecord *precord);
long icmp_oping(icmpRecord* precord);

epicsShareFunc int icmpOPingInit(const char *list0, const char *list1, const char* list2, const char* list3, const char* list4);

typedef struct devicmpOPingSoft{
	long        number;
	DEVSUPFUN   report;
	DEVSUPFUN   init;
	DEVSUPFUN   init_record;
	DEVSUPFUN   get_ioint_info;
	//long  (*get_ioint_info) (int, dbCommon*, IOSCANPVT*);
	DEVSUPFUN   icmp_oping;
}devicmpOPingSoft;




#ifdef __cplusplus
}
#endif

#endif // __ICMP_OPING_H

