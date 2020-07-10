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
#include <epicsExport.h>

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

static long InitSubStr2Bit(subRecord *precord)
{
    return(0);
}

static long ProcSubStr2Bit(subRecord *precord)
{
#if 1
	//DBADDR *pdbAddrA = dbGetPdbAddrFromLink(&precord->inpa);
	//DBADDR *pdbAddrA =  (struct dbAddr*)(&precord->inpa)->value.pv_link.pvt;
	DBADDR *pdbAddrA =  (DBADDR *)(&precord->inpa)->value.pv_link.pvt;
	//char* sval = (char*)pdbAddrA->pfield;

	string strval((char*)pdbAddrA->pfield);
	reverse(strval.begin(), strval.end());

	//bitset<32> strbit(sval);
	bitset<32> strbit((char*)strval.c_str());
	precord->val = strbit.to_ullong();

#else
	DBADDR *pdbAddrA = dbGetPdbAddrFromLink(&precord->inpa);
	DBADDR *pdbAddrB = dbGetPdbAddrFromLink(&precord->inpb);

	float *pfieldLinkA =  (float*)pdbAddrA->pfield;
	float *pfieldLinkB =  (float*)pdbAddrB->pfield;

	long no_elements = pdbAddrA->no_elements;
	int index = 0;

	waveformRecord *precordLinkA = (waveformRecord *)pdbAddrA->precord;
	waveformRecord *precordLinkB = (waveformRecord *)pdbAddrB->precord;

	float con_inpA = (float)precord->c;
	float con_inpB = (float)precord->d;
	float con_inpDelta = (float)precord->e;

	for(index = 0; index < no_elements;index++)
	{
		pfieldLinkA[index] =  cos(con_inpA*M_PI/(float)no_elements * (float)index);
		pfieldLinkB[index] =  cos(con_inpB*M_PI/(float)no_elements * (float)index + con_inpDelta);
	};

	precordLinkA->nord = no_elements;
	precordLinkB->nord = no_elements;

	dbProcess(precordLinkA);
	dbProcess(precordLinkB);
#endif

    return(0);
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitSubStr2Bit);
epicsRegisterFunction(ProcSubStr2Bit);
