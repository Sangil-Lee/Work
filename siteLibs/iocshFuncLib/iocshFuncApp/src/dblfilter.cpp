#include <stdlib.h>
#include <limits.h> 
#include <cstring>
#include <set>   
#include <string> 
#include <map>

// #EPICS LIBS
#define epicsTypesGLOBAL
#include <epicsTypes.h>
#include <epicsPrint.h>
#include <epicsExport.h>
#include <registryFunction.h>
#include <dbCommon.h>
#include <devLib.h>
#include <iocsh.h>

#include "epicsStdlib.h"
#include "epicsStdio.h"
#include "epicsString.h"
#include "dbDefs.h"
#include "errlog.h"
#include "ellLib.h"
#include "epicsMutex.h"
#include "epicsStdioRedirect.h"
#include "dbBase.h"
#include "dbStaticLib.h"
#include "link.h"
#include "dbFldTypes.h"
#include "recSup.h"
#include "devSup.h"
#include "drvSup.h"
#include "dbCommon.h"
#include "special.h"
#include "db_field_log.h"
#define epicsExportSharedSymbols
#include "dbAddr.h"
#include "dbLock.h"
#include "dbAccessDefs.h"
#include "recGbl.h"
#include "dbEvent.h"
#include "callback.h"
#include "dbTest.h"

#include <iostream>
#include <vector>
#include <regex>
#include <stdexcept>
#include <thread>
#include <csignal>
#include <time.h>
#include <unistd.h>

long epicsShareAPI dblFilter(const char* filter)
{
    DBENTRY dbentry;
    DBENTRY *pdbentry=&dbentry;
    long status;

    if (!pdbbase) {
        printf("No database loaded\n");
        return 0;
    }

    if (filter && (*filter == '\0')) filter = NULL;

	std::string subfilter = std::string("");

	if(filter != NULL)
		subfilter = std::string(filter);

    dbInitEntry(pdbbase, pdbentry);
	status = dbFirstRecordType(pdbentry);

    if (status) {
        printf("No record type\n");
    };

	//regex e("(.*)P2DT(.*)EH(.*)");
	std::regex r("\\s+");
	subfilter = std::regex_replace(subfilter, r, "(.*)");
	std::string sfilter = "(.*)"+subfilter+"(.*)";
	std::regex e(sfilter);

    while (!status) {
        status = dbFirstRecord(pdbentry);
        while (!status) {
			std::string spvname = dbGetRecordName(pdbentry);

			bool match = regex_match(spvname, e);
			if(match) {
				std::cout << spvname << std::endl;
			};
            status = dbNextRecord(pdbentry);
        }
        status = dbNextRecordType(pdbentry);
    }

    dbFinishEntry(pdbentry);
    return 0;
}

epicsRegisterFunction(dblFilter);

static const iocshArg dblFilterArg0 = { "filter",iocshArgString};
static const iocshArg * const dblFilterArgs[] = {&dblFilterArg0};
static const iocshFuncDef dblFilterFuncDef = {"dblFilter",1,dblFilterArgs};
static void dblFilterCallFunc(const iocshArgBuf *args)
{
    dblFilter(args[0].sval);
}

/* Registration routine, runs at startup */
static void dblFilterRegister(void) {
    iocshRegister(&dblFilterFuncDef, dblFilterCallFunc);
}

epicsExportRegistrar(dblFilterRegister);
