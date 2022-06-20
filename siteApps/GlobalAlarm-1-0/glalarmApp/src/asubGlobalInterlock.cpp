#include <unistd.h>
#include <cstdio>
#include <string>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>

#include <dbAccess.h>
#include <link.h>
#include <dbDefs.h>
#include <dbLock.h>
#include <dbAddr.h>
#include <dbCommon.h>
#include <epicsThread.h>
#include <epicsMessageQueue.h>
#include <registryFunction.h>
#include <recSup.h>
#include <waveformRecord.h>
#include <compressRecord.h>
#include <aSubRecord.h>
#include <epicsExport.h>
#include <iocsh.h>

#define	OUTFILE 0

using namespace std;
static int gbintDebug = 0;

//
static long InitGBInterlock(aSubRecord *pRec)
{
    return(0);
}

//
static long ProcGBInterlock(aSubRecord *pRec)
{
	long status = 0;

	//INPA
	//INPB
	//INPC
	//INPD
	//INPE
	//INPF
	

	//OUTA

	long nelm = pRec->noa;
	double *sinval = (double*)pRec->a;

#if 1
	double *pFFTWaveVal = (double*)pRec->vala;
	double *pFFTFreqWaveVal = (double*)pRec->valb;
#endif
	if(gbintDebug)
	{
	};

#if 0
	for(int i = 0; i < ffreq.size(); i++)
	{
		pFFTFreqWaveVal[i] = freq(i);
		pFFTWaveVal[i] = std::abs(ffreq(i));
	};
#endif

	return(status);

}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitGBInterlock);
epicsRegisterFunction(ProcGBInterlock);
epicsExportAddress(int, gbintDebug);


