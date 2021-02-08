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
#include <stdexcept>
#include <thread>
#include <csignal>
#include <time.h>
#include <unistd.h>

long epicsShareAPI readBack(const char *precordTypename, const char *fields, const char* filter, const int sync)
{
    DBENTRY dbentry;
    DBENTRY *pdbentry=&dbentry;
    long status;
    int nfields = 0;
    int ifield;
    char *fieldnames = 0;
    char **papfields = 0;

    if (!pdbbase) {
        printf("No database loaded\n");
        return 0;
    }

    if (precordTypename &&
        ((*precordTypename == '\0') || !strcmp(precordTypename,"*")))
        precordTypename = NULL;

    if (fields && (*fields == '\0')) fields = NULL;
    if (filter && (*filter == '\0')) filter = NULL;

	std::string subfilter = std::string("");

	if(filter != NULL)
		subfilter = std::string(filter);

    if (fields) {
        char *pnext;

        fieldnames = epicsStrDup(fields);
        nfields = 1;
        pnext = fieldnames;
        while (*pnext && (pnext = strchr(pnext,' '))) {
            nfields++;
            while (*pnext == ' ') pnext++;
        }
        papfields = (char**)dbCalloc(nfields,sizeof(char *));
        pnext = fieldnames;
        for (ifield = 0; ifield < nfields; ifield++) {
            papfields[ifield] = pnext;
            if (ifield < nfields - 1) {
                pnext = strchr(pnext, ' ');
                *pnext++ = 0;
                while (*pnext == ' ') pnext++;
            }
        }
    }
    dbInitEntry(pdbbase, pdbentry);
    if (!precordTypename)
        status = dbFirstRecordType(pdbentry);
    else
        status = dbFindRecordType(pdbentry,precordTypename);
    if (status) {
        printf("No record type\n");
    }

    while (!status) {
        status = dbFirstRecord(pdbentry);
        while (!status) {
			std::string spvname = dbGetRecordName(pdbentry);
			if( std::string::npos != spvname.find(subfilter)) 
			{
				std::string substring= spvname.substr(spvname.length()-2, spvname.length()); 
				if( substring == subfilter )
					printf("ReadBackPV(%s)", spvname.c_str());
				else
					printf("%s", spvname.c_str());
					//printf("%s, %s", spvname.c_str(), substring.c_str());

				for (ifield = 0; ifield < nfields; ifield++) {
					char *pvalue;
					status = dbFindField(pdbentry, papfields[ifield]);
					if (status) {
						if (!strcmp(papfields[ifield], "recordType")) {
							pvalue = dbGetRecordTypeName(pdbentry);
						}
						else {
							printf(", ");
							continue;
						}
					}
					else {
						pvalue = dbGetString(pdbentry);
					};
					printf(", \"%s\"", pvalue ? pvalue : "");
					std::string writepv = spvname.substr(0, spvname.length()-subfilter.length()); 
					//printf("\nWrite PV(%s)", writepv.c_str()); 
					if(sync >= 1)
						dbpf(writepv.c_str(), pvalue);
				};
				printf("\n");
			};
            status = dbNextRecord(pdbentry);
        }
        if (precordTypename)
            break;

        status = dbNextRecordType(pdbentry);
    }
    if (nfields > 0) {
        free((void *)papfields);
        free((void *)fieldnames);
    }
    dbFinishEntry(pdbentry);
    return 0;
}

epicsRegisterFunction(readBack);

//static const iocshArg readBackArg0 = { "record type",iocshArgString};
//static const iocshArg readBackArg1 = { "fields",iocshArgString};
//static const iocshArg * const readBackArgs[] = {&dblArg0,&dblArg1};

static const iocshArg readBackArg0 = { "record type",iocshArgString};
static const iocshArg readBackArg1 = { "fields",iocshArgString};
static const iocshArg readBackArg2 = { "filter",iocshArgString};
static const iocshArg readBackArg3 = { "sync",iocshArgInt};
static const iocshArg * const readBackArgs[] = {&readBackArg0, &readBackArg1, &readBackArg2, &readBackArg3};
static const iocshFuncDef readBackFuncDef = {"readBack",4,readBackArgs};
static void readBackCallFunc(const iocshArgBuf *args)
{
    readBack(args[0].sval, args[1].sval, args[2].sval, args[3].ival);
}

/* Registration routine, runs at startup */
static void readBackRegister(void) {
    iocshRegister(&readBackFuncDef, readBackCallFunc);
}

epicsExportRegistrar(readBackRegister);
