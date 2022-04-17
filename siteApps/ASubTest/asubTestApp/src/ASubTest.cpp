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

static long InitProc(aSubRecord *prec)
{
	return 0;
}

static long ASubProc(aSubRecord *prec)
{
	double *aval = (double*)prec->a;
	double *outwf = (double*)prec->vala;

	long noa = prec->noa;
	for ( int i = 0; i < noa; i++ )
	{
		//printf("INPA: %f,", aval[i]);
		//User Code....
		outwf[i] = aval[i];
	}

	return 0;
}

epicsRegisterFunction(InitProc);
epicsRegisterFunction(ASubProc);
