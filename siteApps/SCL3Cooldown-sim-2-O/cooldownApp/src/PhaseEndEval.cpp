#include <unistd.h>
#include <fstream>
#include <cstdio>
#include <string>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <cmath>
#include <stdlib.h>
#include <limits.h> 
#include <vector>
#include <regex>
#include <stdexcept>
#include <thread>
#include <csignal>
#include <time.h>

// #EPICS LIBS
#include <dbAccess.h>
#include <link.h>
#include <dbDefs.h>
#include <dbLock.h>
#include <dbAddr.h>
#include <dbCommon.h>
#include <epicsThread.h>
#include <epicsMessageQueue.h>
#include <registryFunction.h>
#include <recSup.h>
#include <waveformRecord.h>
#include <compressRecord.h>
#include <aSubRecord.h>
#include <calcoutRecord.h>
#include <epicsExport.h>
#include <iocsh.h>
#include <epicsTypes.h>
#include <epicsPrint.h>
#include <devLib.h>

#include "epicsStdlib.h"
#include "epicsStdio.h"
#include "epicsString.h"
#include "errlog.h"
#include "ellLib.h"
#include "epicsMutex.h"
#include "epicsStdioRedirect.h"
#include "dbBase.h"
#include "dbStaticLib.h"
#include "dbFldTypes.h"
#include "devSup.h"
#include "drvSup.h"
#include "special.h"
#include "db_field_log.h"
#include "dbAccessDefs.h"
#include "recGbl.h"
#include "dbEvent.h"
#include "callback.h"
#include "dbTest.h"


using namespace std;

static long InitTempEval(aSubRecord *pRec)
{

	return(0);
}


static long ProcTempEval(aSubRecord *pRec)
{
    DBENTRY dbentry;
    DBENTRY *pdbentry=&dbentry;
    long status;
	const char* filter;
	const char* filename;

    if (!pdbbase) {
        printf("No database loaded\n");
        return 0;
    }

    if (filter && (*filter == '\0')) filter = NULL;
    if (filename && (*filename == '\0')) filename = NULL;

	std::string subfilter = std::string("");

	if(filter != NULL)
		subfilter = std::string(filter);

	std::ofstream outfile;
	if(filename != NULL)
	{
		outfile.open(filename);
	};

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
				outfile << spvname << std::endl;
			};
            status = dbNextRecord(pdbentry);
        }
        status = dbNextRecordType(pdbentry);
    }

    dbFinishEntry(pdbentry);
	outfile.close();
    return 0;
}

epicsRegisterFunction(InitTempEval);
epicsRegisterFunction(ProcTempEval);

