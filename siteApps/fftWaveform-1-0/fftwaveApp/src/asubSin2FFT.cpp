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
static int sin2fftDebug = 0;

//
static long InitSin2FFT(aSubRecord *pRec)
{
    return(0);
}

//
static long ProcSin2FFT(aSubRecord *pRec)
{
	long status = 0;

	//INPA
	DBADDR *pdbWave = (DBADDR*)(&pRec->inpa)->value.pv_link.pvt;
	compressRecord *pWave = (compressRecord *)pdbWave->precord;

	long nelm = pdbWave->no_elements;
	double *sinval = (double*)pRec->a;

#if 1
	//for FFT X-Y Waveform
	//OUTA Y FFT Amplitude
	DBADDR *pdbFFTDBAddr = (DBADDR*)(&pRec->outa)->value.pv_link.pvt;
	waveformRecord *pFFTWave = (waveformRecord *)pdbFFTDBAddr->precord;
	double *pFFTWaveVal = (double*)pRec->vala;

	//OUTB X Frequency 
	DBADDR *pdbFFTFreqAddr = (DBADDR*)(&pRec->outb)->value.pv_link.pvt;
	waveformRecord *pFFTFreqWave = (waveformRecord *)pdbFFTFreqAddr->precord;
	double *pFFTFreqWaveVal = (double*)pRec->valb;
#endif
	double const Fs  = 2*100; // Nyquist [Hz]
	double const Ts  = 1./Fs; // [s] 

	Eigen::VectorXd time(nelm);
	Eigen::VectorXcd f_values(nelm);
	Eigen::VectorXd freq(nelm);

	for(int i = 0; i < nelm; ++i){
		time(i) = i * Ts;
		f_values(i) = sinval[i];
		f_values(i) = f_values(i)/(0.5*nelm);
		freq(i) = Fs * i / double(nelm);
	}

	Eigen::FFT<double> fft;
	Eigen::VectorXcd f_freq(nelm);
	fft.fwd(f_freq, f_values);

	if(sin2fftDebug)
	{
		static int first = 1;
		if(first)
		{
			std::ofstream fftres("fft_result.txt");
			for(int i = 0; i < nelm; ++i){
				fftres << freq(i) << " " << std::abs(f_freq(i)) << "\n"; 
			}
			fftres.close();
			first++;
		};
	};

#if 1
	for(int i = 0; i < f_freq.size(); i++)
	{
		pFFTFreqWaveVal[i] = freq(i);
		pFFTWaveVal[i] = std::abs(f_freq(i));
	};
#endif

	dbProcess((dbCommon*)pFFTWave);
	dbProcess((dbCommon*)pFFTFreqWave);
	return(status);

#if 0 
unsigned const N = 10000;  // Sample Count
//double const Fs  = 32;    //  [Hz]
double const Fs  = 2*100;    // Nyquist [Hz]
double const Ts  = 1./Fs; // [s] 


const double f0  = 5;     // [Hz]
const double f1  = 10;     // [Hz]
const double f2  = 50;     // [Hz]
const double f3  = 60;     // [Hz]

using namespace std;

//std::complex<double> f(std::complex<double> const & t){
double f(double const & t){
    //return (std::sin(2*M_PI*f0*t));
	//
	//Add Amplitude
    //return (std::sin(2*M_PI*f0*t)*3.0);
	//
	//Add Other wave
    //return 2.0*cos(2*M_PI*2*t) + 3.0*sin(2*M_PI*4*t) + 4.0*sin(2*M_PI*6*t);
    return 3.0*sin(2*M_PI*f0*t) + 5.0*sin(2*M_PI*f1*t) + 4.0*cos(2*M_PI*f2*t) + 2.0*cos(2*M_PI*f3*t);
}

int main(){
    std::ofstream xrec("xrec.txt");
    std::ofstream sin("sin.txt");

    //Eigen::VectorXcd time(N);
    Eigen::VectorXd time(N);
    //Eigen::VectorXcd f_values(N);
    Eigen::VectorXd f_values(N);
    Eigen::VectorXd freq(N);

    for(int u = 0; u < N; ++u){
        time(u) = u * Ts;
        //f_values(u) = f(time(u)); // Y_Val need real scale
        f_values(u) = f(time(u)); // Count
        //sin << u << " " << f_values(u).real() << "\n"; 
        sin << u << " " << f_values(u) << "\n"; 
        f_values(u) = f_values(u)/(0.5*N);
        freq(u) = Fs * u / double(N);
    }

    Eigen::FFT<double> fft;
    Eigen::VectorXcd f_freq(N);
    fft.fwd(f_freq, f_values);

    //for(int u = 0; u < N; ++u){
	//Remove Nyquist Freq.
    //for(int u = 0; u < N/2; ++u){
    for(int u = 0; u < N; ++u){
        xrec << freq(u) << " " << std::abs(f_freq(u)) << "\n"; 
    }

	xrec.close();
	sin.close();
}
#endif

//GNU Plot Script
//set key off
//set grid
//set output "figure.png"
//set xlabel "Frequency [Hz]"
//plot [-1:100] [-1:10] "xrec.txt" with impulses, "xrec.txt" with points pt 4

}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitSin2FFT);
epicsRegisterFunction(ProcSin2FFT);
epicsExportAddress(int, sin2fftDebug);


