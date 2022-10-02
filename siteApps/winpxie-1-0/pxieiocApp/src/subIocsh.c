
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <link.h>
#include <dbDefs.h>
#include <dbLock.h>
#include <epicsThread.h>
#include <epicsMessageQueue.h>
#include <registryFunction.h>
#include <recSup.h>
#include <subRecord.h>
#include <epicsExport.h>
#include <errlog.h>


static long InitIOCPause(subRecord *pRec)
{

    return(0);
}

static long ProcIOCPause(subRecord *pRec)
{

    if(pRec->pact) {
		pRec->pact = FALSE;
        return 0;
    }

    pRec->pact = TRUE;

    if(pRec->val==0)
    {
        printf("IOC Pause Called \n");
        iocPause();
    }
    else
    {
        printf("IOC ReRun Called \n");
        iocRun();
    }

    return(2);

}

epicsRegisterFunction(InitIOCPause);
epicsRegisterFunction(ProcIOCPause);