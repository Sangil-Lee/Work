#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <dbAccess.h>
#include <link.h>
#include <dbDefs.h>
#include <dbLock.h>
#include <dbAddr.h>
#include <epicsThread.h>
#include <epicsMessageQueue.h>
#include <registryFunction.h>
#include <recSup.h>
#include <aSubRecord.h>
#include <epicsExport.h>
#include <waveformRecord.h>


/*subroutine record*/
static long initASUB(aSubRecord *precord)
{

	return 0;
}

static long calcASUB(aSubRecord *p_asubrec)
{
	double *aval = (double *)p_asubrec->a;
	double *bval = (double *)p_asubrec->b;

	DBADDR *pdbAddr = (DBADDR*)dbGetPdbAddrFromLink(&p_asubrec->outa);
	waveformRecord *precordLinkA = (waveformRecord *)pdbAddr->precord;

	double  *wf = (double *)p_asubrec->vala;
	long noe = pdbAddr->no_elements;

	long noa = p_asubrec->noa;

	//long nob = p_asubrec->nob;
	//long* sin_phase = (long*)p_asubrec->c;
	//long* cos_phase = (long*)p_asubrec->d;

	int i = 0;
	for( i = 0; i < noe; i++)
	{
		wf[i] = (aval[i]&0x1<<12) | (bval[i] & 0x7ff);
		printf("Index(%d),A-Val(%d),B-Val(%d),Wave(%d)\n", i, aval[i], bval[i], wf[i]);
	};

	precordLinkA->nord = noe;
	dbProcess((dbCommon*)precordLinkA);

	return 0;
}

epicsRegisterFunction(initASUB);
epicsRegisterFunction(calcASUB);
