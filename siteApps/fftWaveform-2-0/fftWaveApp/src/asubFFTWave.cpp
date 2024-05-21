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

//Eigen
#include <Eigen/Dense>
#include <unsupported/Eigen/FFT>

//using namespace std;
using namespace Eigen;
static int fftDebug = 0;
static int fftPyVerify = 0;

//
static long InitFFTWave(aSubRecord *pRec)
{
    return(0);
}

//
static long ProcFFTWave(aSubRecord *pRec)
{

	auto *rawwave = static_cast<double *>(pRec->a);
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	std::uniform_real_distribution<> dis(0.0, 6.28);

	auto *freqB = static_cast<float*>(pRec->b);
	auto *freqC = static_cast<float*>(pRec->c);
	auto *freqD = static_cast<float*>(pRec->d);

	auto *ampE = static_cast<float*>(pRec->e);
	auto *ampF = static_cast<float*>(pRec->f);
	auto *ampG = static_cast<float*>(pRec->g);

	auto *wave		= static_cast<double*>(pRec->vala);
	//std::vector<double> Wave(pRec->noa);

	int nelm = pRec->noa;
	//double const Fs  = 2*nelm;
	double const Fs  = nelm;
	double const Ts  = 1./Fs; // [s] 

	//float phase = dis(generator);
	float phase = 0.;

	//This is for Tes fftWave4.db
	for(size_t i = 0; i < pRec->noa; i++)
	{
		phase += Ts;
		if( phase > 2.*M_PI ) phase = 0.;

		if(!fftPyVerify)
		{
			rawwave[i] = ampE[0]*cos(2*M_PI*phase*freqB[0]) + ampF[0]*sin(2*M_PI*phase*freqC[0]) + ampG[0]*sin(2*M_PI*phase*freqD[0]);
			wave[i] = rawwave[i];
		}
		else
		{
			wave[i] = rawwave[i];
		};
	};

	auto *fftWave	  = static_cast<double*>(pRec->valb);
	auto *fftWaveFreq = static_cast<double*>(pRec->valc);

	try {
		Eigen::VectorXd time(nelm);
		Eigen::VectorXcd fvalues(nelm);
		Eigen::VectorXd freq(nelm);

		for(int i = 0; i < nelm; ++i){
			time(i) = i * Ts;
			fvalues(i) = wave[i];

			//fvalues(i) = rawwave[i];    //this is for fftwave4.db, test_4.py
			
			fvalues(i) = fvalues(i)/(0.5*nelm);
			freq(i) = Fs*i / double(nelm);
		}

		Eigen::FFT<double> fft;
		Eigen::VectorXcd ffreq(nelm);
		fft.fwd(ffreq, fvalues);

		if(fftDebug)
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
			fftWave[i] = freq(i);
			fftWaveFreq[i] = std::abs(ffreq(i));
		};

	} catch(std::exception &e) {
		std::cout <<  e.what() << std::endl;
	};

	return (0);
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitFFTWave);
epicsRegisterFunction(ProcFFTWave);
epicsExportAddress(int, fftDebug);
epicsExportAddress(int, fftPyVerify);


