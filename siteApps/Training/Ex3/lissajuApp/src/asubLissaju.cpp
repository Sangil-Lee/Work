#include <cstdio>
#include <cstdlib>
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
#include <aSubRecord.h>
#include <epicsExport.h>
#include <waveformRecord.h>

using namespace std;

static long InitLissaju(aSubRecord *pRec)
{
	return 0;
}

static long ProcLissaju(aSubRecord *pRec)
{
	double *paramA = (double*)pRec->a;
	double *paramB = (double*)pRec->b;
	double *delta = (double*)pRec->c;

	double *lissajuX = (double *)pRec->vala;
	double *lissajuY = (double *)pRec->valb;

	int no_elements = pRec->nova;

	for(int index = 0; index < no_elements;index++)
	{
		lissajuX[index] =  cos(paramA[0]*M_PI/(float)no_elements * (float)index);
		lissajuY[index] =  cos(paramB[0]*M_PI/(float)no_elements * (float)index + delta[0]);
	};

	return 0;
}

epicsRegisterFunction(InitLissaju);
epicsRegisterFunction(ProcLissaju);
