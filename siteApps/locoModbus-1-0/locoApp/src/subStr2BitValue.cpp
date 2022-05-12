#include <aSubRecord.h>
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
#include <longinRecord.h>
#include <epicsExport.h>

#include <stdio.h>
#include <string.h>
#include <cstdio>
#include <string>
#include <iostream>
#include <bitset>
#include <algorithm>

#define dbGetPdbAddrFromLink(PLNK) \
    ( ( (PLNK)->type != DB_LINK ) \
      ? 0 \
      : ( ( (struct dbAddr *)( (PLNK)->value.pv_link.pvt) ) ) )

using namespace std;

static long InitSubStr2Bit(aSubRecord *precord)
{
    printf("aSub Initialize\n");
    return(0);
}

static long ProcSubStr2Bit(aSubRecord *precord)
{
	long status = 0;

	char* sval = (char*)precord->a;
	string strval((char*)sval);
	reverse(strval.begin(), strval.end());

	bitset<5> strbit((char*)strval.c_str());
	//precord->vala = (void*)strbit.to_ulong();

	long *uval = (long*)precord->vala;
	uval[0] = strbit.to_ulong();
	//
	//long uval = (long)precord->vala;
	//uval = strbit.to_ulong();
	//printf("strvalue(%s), longvalue:(%ld)\n", sval, uval[0] );
    return(status);
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitSubStr2Bit);
epicsRegisterFunction(ProcSubStr2Bit);
