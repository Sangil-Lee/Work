//C++
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>

//EPICS
#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>

//
static long InitFFTWaveW3(aSubRecord *pRec)
{
    return(0);
}

//
static long ProcFFTWaveW3(aSubRecord *pRec)
{

	auto *rawwave = static_cast<double *>(pRec->a);

	auto *freqB = static_cast<float*>(pRec->b);
	auto *freqC = static_cast<float*>(pRec->c);
	auto *freqD = static_cast<float*>(pRec->d);

	auto *ampE = static_cast<float*>(pRec->e);
	auto *ampF = static_cast<float*>(pRec->f);
	auto *ampG = static_cast<float*>(pRec->g);

	auto *wave	= static_cast<double*>(pRec->vala);

	//int nelm = pRec->noa;
	size_t nelm = 1000;
	double const Fs  = nelm;
	double const Ts  = 1./Fs; // [s] 

	double phase = 0.;

#if 1
	for(size_t i = 0; i < nelm;i++)
	{
		phase += Ts;
		if( phase > 2.*M_PI ) phase = 0.;

        //data = amplitude1*times*np.cos(FreqA*2 * np.pi * np.arange(arrsize) / arrsize) + amplitude2*times*np.sin(FreqB * 2 * np.pi * np.arange(arrsize) / arrsize) + amplitude3*times*np.sin(FreqC * 2 * np.pi * np.arange(arrsize) / arrsize)
		rawwave[i] = ampE[0]*cos(2*M_PI*phase*freqB[0]) + ampF[0]*sin(2*M_PI*phase*freqC[0]) + ampG[0]*sin(2*M_PI*phase*freqD[0]);
		//wave[i] = Wave.at(i) = rawwave[i];
		wave[i] = rawwave[i];
	};
#else
	for(size_t i = 0; i < pRec->noa; i++)
	{
#if 1
		phase = i/nelm;
		rawwave[i] = ampE[0]*cos(2*M_PI*(double)i/nelm*freqB[0]) + 
					 ampF[0]*sin(2*M_PI*(double)i/nelm*freqC[0]) + 
					 ampG[0]*sin(2*M_PI*(double)i/nelm*freqD[0]);
#else
		phase += Ts;
		rawwave[i] = ampE[0]*cos(2*M_PI*phase*freqB[0]) + 
					 ampF[0]*sin(2*M_PI*phase*freqC[0]) + 
					 ampG[0]*sin(2*M_PI*phase*freqD[0]);
#endif
		wave[i] = rawwave[i];
	};
#endif

	return (0);
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitFFTWaveW3);
epicsRegisterFunction(ProcFFTWaveW3);

