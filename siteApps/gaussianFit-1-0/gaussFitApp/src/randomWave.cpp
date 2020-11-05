#include <aSubRecord.h>
#include <epicsExport.h>

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
#include <epicsExport.h>

#include <cstdio>
#include <string>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <bitset>
#include <numeric>
#include <valarray>
#include <unistd.h>
#include <random>
#include <ctime>

using namespace std;


static long makeRWaveform(aSubRecord *pRec)
{
	DBADDR *pdbAddr1 = (DBADDR*)(&pRec->outa)->value.pv_link.pvt;

	waveformRecord *pWave1 = (waveformRecord *)pdbAddr1->precord;
	double *aval = (double*)pRec->a;

	if(pdbAddr1 == NULL || pWave1 == NULL) return(-1);

	double *wave1 = (double*)pRec->vala;
	long nelm = pdbAddr1->no_elements;

	//random number
	srand(time(nullptr));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.01, 0.02);

	std::time_t datetime = std::time(nullptr);

	double randomval = 0.0;
    for (int i=0; i < nelm; i++) {
		//randomval = dis(gen);
		randomval = drand48();
        wave1[i] = randomval*aval[0];
	};

	pWave1->nord = nelm;
	dbProcess((dbCommon*)pWave1);

	return (0);
}


static long InitRWave(aSubRecord *pRec)
{
    return(0);
}

static long ProcRWave(aSubRecord *pRec)
{
	long status = 0;
	status = makeRWaveform(pRec);
	if(status < 0) 
	{
		cout << "Make Linear Waveform Error" << endl;
		return (-1);
	};

    return(0);
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitRWave);
epicsRegisterFunction(ProcRWave);
