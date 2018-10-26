#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <cstdlib>

#include <dbDefs.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <waveformRecord.h>
#include <stringinRecord.h>
#include <epicsExport.h>
#include <epicsThread.h>
#include <dbAccess.h>
#include <link.h>
#include <dbAddr.h>

using namespace std;

enum WAVERECORD {EV_SEQA_CFG = 1, EV_SEQA_TSTAMP, EV_SEQB_CFG, EV_SEQB_TSTAMP, EV_MAP_RAM};

static long initStrToValue(subRecord *precord)
{
	return (0);
}

static long procStrToValue(subRecord *precord)
{
	DBADDR *pdbAddrA = dbGetPdbAddrFromLink(&precord->inpa);
	DBADDR *pdbAddrB = dbGetPdbAddrFromLink(&precord->inpb);
	DBADDR *pdbAddrC = dbGetPdbAddrFromLink(&precord->inpc);
	DBADDR *pdbAddrD = dbGetPdbAddrFromLink(&precord->inpd);
	DBADDR *pdbAddrE = dbGetPdbAddrFromLink(&precord->inpe);

	double *pfieldLinkA = (double *)pdbAddrA->pfield;
	double *pfieldLinkB = (double *)pdbAddrB->pfield;
	double *pfieldLinkC = (double *)pdbAddrC->pfield;
	double *pfieldLinkD = (double *)pdbAddrD->pfield;
	double *pfieldLinkE = (double *)pdbAddrE->pfield;

	long initvalue = precord->f;
	long initcount = precord->g;
	long initinc   = precord->h;

	if(initvalue > 2047 ) return (-1);

	int record = precord->val;
#if 0
	unsigned long long lvalue = 0xFFF;
	unsigned long NOEL = lvalue & 0x7FF;
	unsigned long record = (lvalue>>11)&0xFF;
#endif

	//printf("Type(%ld, 0x%x), Elements(%ld, 0x%x)\n", record,record, NOEL, NOEL);

	waveformRecord *precordLinkA = (waveformRecord *)pdbAddrA->precord;
	waveformRecord *precordLinkB = (waveformRecord *)pdbAddrB->precord;
	waveformRecord *precordLinkC = (waveformRecord *)pdbAddrC->precord;
	waveformRecord *precordLinkD = (waveformRecord *)pdbAddrD->precord;
	waveformRecord *precordLinkE = (waveformRecord *)pdbAddrE->precord;

	switch(record)
	{
		case EV_SEQA_CFG:
			{
				for(int i = 0; i < initcount; i++)
				{
					pfieldLinkA[i] = (long)initvalue+(i+1)*initinc;
					if(pfieldLinkA[i]>2047) pfieldLinkA[i] = 2047; 
				}
				precordLinkA->nord = initcount;
				dbProcess((dbCommon*)precordLinkA);
			}
			break;
		case EV_SEQA_TSTAMP:
			{
				for(int i = 0; i < initcount; i++)
				{
					pfieldLinkB[i] = (long)initvalue+(i+1)*initinc;
					if(pfieldLinkB[i]>2047) pfieldLinkB[i] = 2047; 
				}
				precordLinkB->nord = initcount;
			}
			dbProcess((dbCommon*)precordLinkB);
			break;
		case EV_SEQB_CFG:
			{
				for(int i = 0; i < initcount; i++)
				{
					pfieldLinkC[i] = (long)initvalue+(i+1)*initinc;
					if(pfieldLinkC[i]>2047) pfieldLinkC[i] = 2047; 
				}
				precordLinkC->nord = initcount;
			}
			dbProcess((dbCommon*)precordLinkC);
			break;
		case EV_SEQB_TSTAMP:
			{
				for(int i = 0; i < initcount; i++)
				{
					pfieldLinkD[i] = (long)initvalue+(i+1)*initinc;
					if(pfieldLinkD[i]>2047) pfieldLinkD[i] = 2047; 
				}
				precordLinkD->nord = initcount;
			}
			dbProcess((dbCommon*)precordLinkD);
			break;
		case EV_MAP_RAM:
			{
				for(int i = 0; i < initcount; i++)
				{
					pfieldLinkE[i] = (long)initvalue+(i+1)*initinc;
					if(pfieldLinkE[i]>2047) pfieldLinkE[i] = 2047; 
				}
				precordLinkE->nord = initcount;
			}
			dbProcess((dbCommon*)precordLinkE);
			break;
		case 0:
		default:
#if 0
			for (int i = 0; i < initcount;i++)
			{
				pfieldLinkA[i] = (long)initvalue;
				pfieldLinkB[i] = (long)initvalue;
				pfieldLinkC[i] = (long)initvalue;
				pfieldLinkD[i] = (long)initvalue;
				pfieldLinkE[i] = (long)initvalue;
				precordLinkA->nord = initcount; precordLinkB->nord = initcount;
				precordLinkC->nord = initcount; precordLinkD->nord = initcount;
				precordLinkE->nord = initcount;
			};
			epicsThreadSleep(1);
			dbScanLock((dbCommon*)precordLinkA);
			dbProcess((dbCommon*)precordLinkA);
			dbScanUnlock((dbCommon*)precordLinkA);
			epicsThreadSleep(1);
			dbScanLock((dbCommon*)precordLinkB);
			dbProcess((dbCommon*)precordLinkB);
			dbScanUnlock((dbCommon*)precordLinkB);
			epicsThreadSleep(1);
			dbScanLock((dbCommon*)precordLinkC);
			dbProcess((dbCommon*)precordLinkC);
			dbScanUnlock((dbCommon*)precordLinkC);
			epicsThreadSleep(1);
			dbScanLock((dbCommon*)precordLinkD);
			dbProcess((dbCommon*)precordLinkD);
			dbScanUnlock((dbCommon*)precordLinkD);
			epicsThreadSleep(1);
			dbScanLock((dbCommon*)precordLinkE);
			dbProcess((dbCommon*)precordLinkE);
			dbScanUnlock((dbCommon*)precordLinkE);
#endif
			break;
	};
	return (0);
}

epicsRegisterFunction(initStrToValue);
epicsRegisterFunction(procStrToValue);
