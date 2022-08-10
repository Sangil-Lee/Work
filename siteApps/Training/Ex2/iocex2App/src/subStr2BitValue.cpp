#include <subRecord.h>
#include <epicsExport.h>

#include <dbAccess.h>
#include <link.h>
#include <dbDefs.h>
#include <dbLock.h>
#include <dbAddr.h>
#include <epicsThread.h>
#include <epicsMessageQueue.h>
#include <registryFunction.h>
#include <recSup.h>
#include <stringinRecord.h>
#include <aSubRecord.h>
#include <epicsExport.h>

#include <cstdio>
#include <string>
#include <iostream>
#include <bitset>
#include <algorithm>

using namespace std;

static int debugVar = 0;

static long InitSubStr2Bit(aSubRecord *pRec)
{
    return(0);
}

static long ProcSubStr2Bit(aSubRecord *pRec)
{
	char *inpA = (char*)pRec->a;
	string strval = string(inpA);
	//reverse(strval.begin(), strval.end());

	bitset<32> strbit((char*)strval.c_str());
	long *outval = (long*)pRec->vala;
	outval[0] = strbit.to_ulong();

	if(debugVar)
		cout << strbit.to_string() << endl;

    return(0);
}

/* Register these symbols for use by IOC code: */
epicsExportAddress(int, debugVar);
epicsRegisterFunction(InitSubStr2Bit);
epicsRegisterFunction(ProcSubStr2Bit);
