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

#include <Eigen/Dense>
#include <unsupported/Eigen/FFT>

#define	OUTFILE 0

using namespace std;
using namespace Eigen;
static int fftwaveDebug = 0;

//
static long InitFFTWave(aSubRecord *pRec)
{
    return(0);
}

//
static long ProcFFTWave(aSubRecord *pRec)
{
	long status = 0;

	//INPA
	long nelm = pRec->noa;
	double *sinval = (double*)pRec->a;

#if 1
	//for FFT X-Y Waveform
	//OUTA Y FFT Amplitude
	double *pFFTWaveVal = (double*)pRec->vala;

	//OUTB X Frequency 
	double *pFFTFreqWaveVal = (double*)pRec->valb;
#endif
	double const Fs  = 2*100; // Nyquist [Hz]
	double const Ts  = 1./Fs; // [s] 

	Eigen::VectorXd time(nelm);
	Eigen::VectorXcd fvalues(nelm);
	Eigen::VectorXd freq(nelm);

	for(int i = 0; i < nelm; ++i){
		time(i) = i * Ts;
		fvalues(i) = sinval[i];
		fvalues(i) = fvalues(i)/(0.5*nelm);
		freq(i) = Fs * i / double(nelm);
	}

	Eigen::FFT<double> fft;
	Eigen::VectorXcd ffreq(nelm);
	fft.fwd(ffreq, fvalues);

	if(fftwaveDebug)
	{
		static int first = 1;
		if(first)
		{
			std::ofstream fftres("fft_result.txt");
			for(int i = 0; i < nelm; ++i){
				fftres << freq(i) << " " << std::abs(ffreq(i)) << "\n"; 
			}
			fftres.close();
			first++;
		};
	};

#if 1
	for(int i = 0; i < ffreq.size(); i++)
	{
		pFFTFreqWaveVal[i] = freq(i);
		pFFTWaveVal[i] = std::abs(ffreq(i));
	};
#endif
	return(status);
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitFFTWave);
epicsRegisterFunction(ProcFFTWave);
epicsExportAddress(int, fftwaveDebug);


