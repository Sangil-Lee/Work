#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include <dbAccess.h>
#include <link.h>
#include <dbDefs.h>
#include <dbLock.h>
#include <dbAddr.h>
#include <epicsThread.h>
#include <epicsMessageQueue.h>
#include <registryFunction.h>
#include <recSup.h>
#include <subRecord.h>
#include <biRecord.h>
#include <epicsExport.h>

//using namespace std;

static long InitXVSetPt(subRecord *psubrec)
{
	return 0;
}

static long ProcXVSetPt(subRecord *psubrec)
{
	if(psubrec == NULL) return -1;
	DBADDR *pdbAddrA =  (DBADDR *)(&psubrec->inpa)->value.pv_link.pvt;
	DBADDR *pdbAddrB =  (DBADDR *)(&psubrec->inpb)->value.pv_link.pvt;

	if(pdbAddrA == NULL || pdbAddrB == NULL) return -1;

	biRecord *precordLinkA = (biRecord *)pdbAddrA->precord;
	biRecord *precordLinkB = (biRecord *)pdbAddrB->precord;

	std::string nameA = precordLinkA->name;
	std::string nameB = precordLinkB->name;

	int *pfieldLinkA = (int*)pdbAddrA->pfield;
	int *pfieldLinkB = (int*)pdbAddrB->pfield;

	int subVal = psubrec->val;

	pfieldLinkA[0] = subVal; pfieldLinkB[0] = !subVal;

	printf("Name:[%s(%d), %s(%d)]\n", nameA.c_str(),pfieldLinkA[0], nameB.c_str(),pfieldLinkB[0]);

	dbProcess((dbCommon*)precordLinkA);
	dbProcess((dbCommon*)precordLinkB);

	return 0;
}


epicsRegisterFunction(InitXVSetPt);
epicsRegisterFunction(ProcXVSetPt);


