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
static int wfparamDebug = 0;

//
static long InitWFParamQWR(aSubRecord *pRec)
{
    return(0);
}

static long InitWFParamHWR(aSubRecord *pRec)
{
    return(0);
}

//
static long ProcWFParamHWR(aSubRecord *pRec)
{
	long status = 0;
	//E1 = 3 D1 = 2 F1 = 5 E2 = 3 D2 = 2 F2 = 5
	//H  = 2 I = 7 C = 5 K = 2 L = 4 N = 4 M = 8 J = 7
	//fx = sym.Eq(E*(X-D)**0.5*F**0.5-H*(I-C+X*K/L*(N/M)**0.25)**0.5*J**0.5, 0)
	//gx = E*(X-D)**0.5*F**0.5-H*(I-C+X*K/L*(N/M)**0.25)**0.5*J**0.5
#if 1
	double *d = (double*)pRec->a; double *e = (double*)pRec->b; double *f = (double*)pRec->c;
	double *c_old = (double*)pRec->d;
	double *h = (double*)pRec->e; double *i = (double*)pRec->f; double *j = (double*)pRec->g;
	double *k = (double*)pRec->h; double *l = (double*)pRec->i; double *m = (double*)pRec->j;
	double *n = (double*)pRec->k;

	DBADDR *pdbWFParamAddr   = (DBADDR*)(&pRec->outa)->value.pv_link.pvt;
	waveformRecord *pWFParam = (waveformRecord *)pdbWFParamAddr->precord;
	double *pWFParamVal      = (double*)pRec->vala;
	pWFParamVal[0] = d[0];
	pWFParamVal[1] = e[0];
	pWFParamVal[2] = f[0];
	pWFParamVal[3] = c_old[0];
	pWFParamVal[4] = h[0];
	pWFParamVal[5] = i[0];
	pWFParamVal[6] = j[0];
	pWFParamVal[7] = k[0];
	pWFParamVal[8] = l[0];
	pWFParamVal[8] = m[0];
	pWFParamVal[10] = n[0];
#else
#endif
	
	dbProcess((dbCommon*)pWFParam);
    return(0);
}
//
static long ProcWFParamQWR(aSubRecord *pRec)
{
	long status = 0;
#if 1
	//E1 = 3 D1 = 2 F1 = 5 E2 = 3 D2 = 2 F2 = 5
	//H  = 2 I = 7 C = 5 K = 2 L = 4 N = 4 M = 8 J = 7
	//fx = sym.Eq(E1*(X-D1)**0.5*F1**0.5+E2*(X-D2)**0.5*F2**0.5-H*(I-C+X*K/L*(N/M)**0.25)**0.5*J**0.5, 0)
	//gx = E1*(X-D1)**0.5*F1**0.5+E2*(X-D2)**0.5*F2**0.5-H*(I-C+X*K/L*(N/M)**0.25)**0.5*J**0.5

	double *d1 = (double*)pRec->a; double *e1 = (double*)pRec->b; double *f1 = (double*)pRec->c;
	double *d2 = (double*)pRec->d; double *e2 = (double*)pRec->e; double *f2 = (double*)pRec->f;
	double *c_old = (double*)pRec->g;
	double *h = (double*)pRec->h; double *i = (double*)pRec->i; double *j = (double*)pRec->j;
	double *k = (double*)pRec->k; double *l = (double*)pRec->l; double *m = (double*)pRec->m;
	double *n = (double*)pRec->n;

	DBADDR *pdbWFParamAddr   = (DBADDR*)(&pRec->outa)->value.pv_link.pvt;
	waveformRecord *pWFParam = (waveformRecord *)pdbWFParamAddr->precord;
	double *pWFParamVal      = (double*)pRec->vala;
	pWFParamVal[0] = e1[0];
	pWFParamVal[1] = d1[0];
	pWFParamVal[2] = f1[0];
	pWFParamVal[3] = e2[0];
	pWFParamVal[4] = d2[0];
	pWFParamVal[5] = f2[0];
	pWFParamVal[6] = c_old[0];
	pWFParamVal[7] = h[0];
	pWFParamVal[8] = i[0];
	pWFParamVal[8] = j[0];
	pWFParamVal[10] = k[0];
	pWFParamVal[11] = l[0];
	pWFParamVal[12] = m[0];
	pWFParamVal[13] = n[0];
	
	dbProcess((dbCommon*)pWFParam);

#else
	//INPA
	DBADDR *pdbWave = (DBADDR*)(&pRec->inpa)->value.pv_link.pvt;
	compressRecord *pWave = (compressRecord *)pdbWave->precord;

	long nelm = pdbWave->no_elements;
	double *sinval = (double*)pRec->a;

	//for FFT X-Y Waveform
	//OUTA Y FFT Amplitude
	DBADDR *pdbFFTDBAddr = (DBADDR*)(&pRec->outa)->value.pv_link.pvt;
	waveformRecord *pFFTWave = (waveformRecord *)pdbFFTDBAddr->precord;
	double *pFFTWaveVal = (double*)pRec->vala;

	//OUTB X Frequency 
	DBADDR *pdbFFTFreqAddr = (DBADDR*)(&pRec->outb)->value.pv_link.pvt;
	waveformRecord *pFFTFreqWave = (waveformRecord *)pdbFFTFreqAddr->precord;
	double *pFFTFreqWaveVal = (double*)pRec->valb;
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

	if(wfparamDebug)
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

	for(int i = 0; i < ffreq.size(); i++)
	{
		pFFTFreqWaveVal[i] = freq(i);
		pFFTWaveVal[i] = std::abs(ffreq(i));
	};
	dbProcess((dbCommon*)pFFTWave);
	dbProcess((dbCommon*)pFFTFreqWave);
#endif

	return(status);
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitWFParamQWR);
epicsRegisterFunction(ProcWFParamQWR);
epicsRegisterFunction(InitWFParamHWR);
epicsRegisterFunction(ProcWFParamHWR);
epicsExportAddress(int, wfparamDebug);

