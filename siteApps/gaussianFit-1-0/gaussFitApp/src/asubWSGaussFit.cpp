#include <unistd.h>
#include <fstream>
#include <cstdio>
#include <string>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <cmath>

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


#include "Gaussian1dFitService.h"
#include "HeuristicGauss1dFitter.h"
#include "LBTAGauss1dFitter.h"
#include "LMGaussFitter1d.h"

#define	OUTFILE 0

using namespace std;

typedef std::vector < double > VD;

static int gaussDebug = 0;

class GaussFit
{
	public:
		GaussFit();
		~GaussFit();

		Gaussian1dFitService::InputParametersG1dFit m_gfInput;
		Gaussian1dFitService::ResultsG1dFit m_gfResult;


		long MakeGenData(aSubRecord *pRec);
		long GaussFitting(aSubRecord *pRec);
		long ResetGaussWave(const int wavesize, const int gauss, const int poly, const float noise, const float sigma);


	private:
		inline double fact( int i )
		{
			if ( i < 2 ) return 1;
			else 
				return double (i) * fact( i - 1 );
		}

		VD makeData( int n, int nGauss, int poly, float rnd, float sigma, compressRecord *pRec );
		long genData(compressRecord *pRec, waveformRecord *pWave, double *waveval);
		void fitGaussian(waveformRecord *pRec, double *waveval);
		void doFitting( Gaussian1dFitService::InputParametersG1dFit input, waveformRecord *pRec, double *waveval );
		//void fitting( Gaussian1dFitService::InputParametersG1dFit input );

		int m_wavesize;
		int m_gauss;
		int m_poly;
		float m_noise;
		float m_sigma;

};

GaussFit::GaussFit():m_wavesize(100),m_gauss(1),m_poly(0),m_noise(0.3),m_sigma(0.6)
{
}

GaussFit::~GaussFit()
{
}

long GaussFit::MakeGenData(aSubRecord *pRec)
{
	DBADDR *pdbAddrX = (DBADDR*)(&pRec->inpa)->value.pv_link.pvt;
	DBADDR *pdbAddrY = (DBADDR*)(&pRec->inpb)->value.pv_link.pvt;
	DBADDR *pdbAddrD = (DBADDR*)(&pRec->inpc)->value.pv_link.pvt;

	if(pdbAddrX == NULL || pdbAddrY == NULL || pdbAddrD == NULL)
	{
		cout << "Input Record Error!!" << endl;
		return(-1);
	}


	compressRecord *pCompX = (compressRecord *)pdbAddrX->precord;
	compressRecord *pCompY = (compressRecord *)pdbAddrY->precord;
	compressRecord *pCompD = (compressRecord *)pdbAddrD->precord;

	if(pCompX == NULL || pCompY == NULL || pCompD == NULL)
	{
		cout << "Input Compress Record Error!!" << endl;
		return(-1);
	}

	DBADDR *pdbAddrOX = (DBADDR*)(&pRec->outa)->value.pv_link.pvt;
	DBADDR *pdbAddrOY = (DBADDR*)(&pRec->outb)->value.pv_link.pvt;
	DBADDR *pdbAddrOD = (DBADDR*)(&pRec->outc)->value.pv_link.pvt;

	if(pdbAddrOX == NULL || pdbAddrOY == NULL || pdbAddrOD == NULL)
	{
		cout << "Output Record Error!!" << endl;
		return(-1);
	}

	waveformRecord *pWaveX = (waveformRecord *)pdbAddrOX->precord;
	waveformRecord *pWaveY = (waveformRecord *)pdbAddrOY->precord;
	waveformRecord *pWaveD = (waveformRecord *)pdbAddrOD->precord;

	if(pWaveX == NULL || pWaveY == NULL || pWaveD == NULL)
	{
		cout << "Output Waveform Record Error!!" << endl;
		return(-1);
	}

	double *wave_a = (double*)pRec->vala;
	double *wave_b = (double*)pRec->valb;
	double *wave_c = (double*)pRec->valc;

	long status = 0;
	for(int i = 0; i < 3; i++)
	{
		switch(i)
		{
			case 0:
			default:
					genData(pCompX, pWaveX, wave_a);
				break;
			case 1:
					genData(pCompY, pWaveY, wave_b);
				break;
			case 2:
					genData(pCompD, pWaveD, wave_c);
				break;
		};
	};

#if 0
	double *wave = (double*)pRec->vala;
	long nelm = pdbAddr->no_elements;
	
    for (int i=0; i < nelm; i++) {
        wave[i] = m_gfInput.data[i];
	};

	pWave->nord = nelm;
	dbProcess((dbCommon*)pWave);

	DBADDR *pdbAddr = (DBADDR*)(&pRec->outb)->value.pv_link.pvt;
	waveformRecord *pWave = (waveformRecord *)pdbAddr->precord;
	double *aval = (double*)pRec->a;

	if(pdbAddr == NULL || aval == NULL || pWave == NULL) return(-1);

	double *wave = (double*)pRec->valb;
	long nelm = pdbAddr->no_elements;

    int inputSize = m_gfInput.data.size();
    int outputSize = inputSize * 2;
    for ( int i = 0,j=0 ; i < outputSize ; ++i ) 
	{
        double x = double (i) / outputSize * inputSize;
        double y = Optimization::evalNGauss1dBkg( x, m_gfResult.input.nGaussians, m_gfResult.input.poly, m_gfResult.params );
		if(i%2 != 0) continue;

		wave[j++] = y;
	};

	if(gaussDebug)
		cout << "Center: " << m_gfResult.params[0] <<", Amplitude: " << m_gfResult.params[1] << ", FWHM: " << m_gfResult.params[2] << endl;

	pWave->nord = nelm;
	dbProcess((dbCommon*)pWave);
#endif
	return status;
}

long GaussFit::GaussFitting(aSubRecord *pRec)
{
	DBADDR *pdbAddr = (DBADDR*)(&pRec->outb)->value.pv_link.pvt;
	waveformRecord *pWave = (waveformRecord *)pdbAddr->precord;
	double *aval = (double*)pRec->a;

	if(pdbAddr == NULL || aval == NULL || pWave == NULL) return(-1);

	double *wave = (double*)pRec->valb;
	long nelm = pdbAddr->no_elements;

    int inputSize = m_gfInput.data.size();
    int outputSize = inputSize * 2;
    for ( int i = 0,j=0 ; i < outputSize ; ++i ) 
	{
        double x = double (i) / outputSize * inputSize;
        double y = Optimization::evalNGauss1dBkg( x, m_gfResult.input.nGaussians, m_gfResult.input.poly, m_gfResult.params );
		if(i%2 != 0) continue;

		wave[j++] = y;
	};

	if(gaussDebug)
		cout << "Center: " << m_gfResult.params[0] <<", Amplitude: " << m_gfResult.params[1] << ", FWHM: " << m_gfResult.params[2] << endl;

	pWave->nord = nelm;
	dbProcess((dbCommon*)pWave);

	return 0;
}
long GaussFit::ResetGaussWave(const int wavesize, const int gauss, const int poly, const float noise, const float sigma)
{
	m_wavesize = wavesize;
	m_gauss = gauss;
	m_poly = poly;
	m_noise = noise;
	m_sigma = sigma;

	return 0;
}

long GaussFit::genData(compressRecord *pRec, waveformRecord *pWave, double *waveval)
{
    auto dataSize = m_wavesize;
    auto nGauss = m_gauss;
    auto nPoly = m_poly;
    float noise = m_noise;
    float sigma = m_sigma;

    m_gfInput.data = makeData( dataSize, nGauss, nPoly, noise, sigma, pRec );
    m_gfInput.isNull = false;
    m_gfInput.stamp = 1;
    m_gfInput.left = 0;
    m_gfInput.right = m_gfInput.data.size() - 1;
    m_gfInput.randomHeuristicsEnabled = true;

	fitGaussian(pWave, waveval);

	return 0;
}

VD GaussFit::makeData( int n, int nGauss, int poly, float rnd, float sigma, compressRecord *pRec )
{
    VD params;
	double *wave = (double*)pRec->val;
	int size = pRec->nsam;
	double maxval = *max_element(wave, wave+size);
    for ( int i = 0 ; i < nGauss ; ++i ) 
	{
        Optimization::Gauss1dNiceParams nice;

        // 1: 1/2
        double w = double (n) / ( nGauss + 1.0 );
        nice.center = ( i + 1 ) * w;
        nice.amplitude = maxval;
       // nice.amplitude = 2 - drand48() * rnd;
        nice.fwhm = w * sigma;
#if 0
        nice.center += ( drand48() - 0.5 ) * rnd * nice.fwhm;
        nice.amplitude += ( drand48() - 0.5 ) * rnd;
        nice.fwhm += ( drand48() - 0.5 ) * rnd * w * 0.1;
#endif

//        center = n/2; ampl = 10; var = -1;
        auto ugly = nice.convertToUgly();
        params.push_back( ugly[0] );
        params.push_back( ugly[1] );
        params.push_back( ugly[2] );
    }

#if OUTFILE
	ofstream ofile("genData.csv");
#endif

    VD result( n );
    for ( int i = 0 ; i < n ; ++i ) {
        result[i] = Optimization::evalNGauss1dBkg( i, nGauss, poly, params );
        result[i] += wave[i];
       // result[i] += ( drand48() - 0.5 ) * rnd;

#if OUTFILE
		cout << result[i] << ',';
		ofile << result[i] << endl;
#endif
    }
#if OUTFILE
	cout << endl;
	ofile.close();
#endif

    return result;
}

void GaussFit::fitGaussian(waveformRecord *pRec, double *waveval)
{
    m_gfInput.nGaussians = 1;
    m_gfInput.poly = 0;
    m_gfInput.randomHeuristicsEnabled = false;

	doFitting(m_gfInput, pRec, waveval);
}
// throws an exception if the interrupt flag is set
void checkForInterrupts()
{
#if 0
    if ( interruptFlag_ == true ) {
        throw Interrupt();
    }
#endif
}

#if 0
void GaussFit::doFitting( Gaussian1dFitService::InputParametersG1dFit input )
{
    try {
        fitting( input );
    }
#if 0
    catch ( Interrupt & ) {
        // std::cerr << "*** Worker interrupted ***\n";
        //emit interrupt();
    }
#endif
    catch ( std::runtime_error & e ) {
        cout << "Worker runtime-exception: " << e.what() << endl;
    }
    catch ( std::exception & e ) {
        cout << "Worker std-exception: " << e.what() << endl;
    }
    catch ( std::string & s ) {
        cout << "Worker exception: " << s.c_str() << endl;
    }
    catch ( char * s ) {
        cout << "Worker exception: " << s << endl;
    }
    catch ( ... ) {
        cout << "Worker uncaught exception..." << endl;
    }
} 
#endif

void GaussFit::doFitting( Gaussian1dFitService::InputParametersG1dFit input, waveformRecord *pRec, double *waveval )
{
    // prepare results (partial)
	Gaussian1dFitService::ResultsG1dFit res;
    res.input = input;
    res.status_ = Gaussian1dFitService::ResultsG1dFit::Partial;
    res.params.resize( res.input.nGaussians * 3 + res.input.poly, 0.0 );

    // handle null input (early exit)
    if ( input.isNull || input.nGaussians + input.poly <= 0 ) {
        res.status_ = Gaussian1dFitService::ResultsG1dFit::Complete;
        res.diffSq = - 1;
        return;
    }

    // should we do all fitters?
    // heuristic fitter only if initial guess not present
    bool doHeuristic = ( int ( input.initGuess.size() ) != input.nGaussians * 3 + input.poly );

    // threshold accepting only if
    //   - multiple gaussians
    //   - or 1 gaussian and at least 1 polynomial term
    //   - it is enabled by the user
    //
    bool doLBTA = input.nGaussians > 1 || ( input.nGaussians == 1 && input.poly > 0 );
    if ( ! input.randomHeuristicsEnabled ) {
        doLBTA = false;
    }


    // levenberg/marquardt if there is at least 1 gaussian
    bool doLM = input.nGaussians > 0;

    bool firstProgressNeeded = true;
    int iteration = 0;

    // set up the data source for the various gaussian fitters
    Optimization::Gaussian1DFitting::FitterInput dataInterface( input.data );
    dataInterface.x1 = input.left;
    dataInterface.x2 = input.right;
    dataInterface.nGaussians = input.nGaussians;
    dataInterface.nPolyTerms = input.poly;
    dataInterface.precomputeRangeMinMax();
    dataInterface.ranges.resize( dataInterface.numParams() );
    double range12 = dataInterface.rangeMax - dataInterface.rangeMin;

    for ( int i = 0 ; i < dataInterface.nGaussians ; i++ ) {
        // center
        dataInterface.ranges[i * 3 + 0].set( input.left, input.right );

        // amplitude
        dataInterface.ranges[i * 3 + 1].set( dataInterface.rangeMin - 0.1 * range12,
                                             dataInterface.rangeMax + 0.1 * range12 );
        //cout << "Data range:" << dataInterface.rangeMin << "," << dataInterface.rangeMax << endl;

        // dataInterface.ranges[i*3+1].set( -10, 10);
        // variance controlling term
        dataInterface.ranges[i * 3 + 2].set(-1.0/(2 * 0.25), -1.0/(2*(dataInterface.x2 - dataInterface.x1)*
                                               (dataInterface.x2 - dataInterface.x1)));
    };

    // ----------------------------------------------------------------------
    // run the heuristic fitter
    // ----------------------------------------------------------------------
    if ( doHeuristic ) {
        Optimization::Gaussian1DFitting::HeuristicFitter hFitter( dataInterface );

        // start fitting
        iteration = 0;
        while ( true ) {
            iteration++;
            checkForInterrupts();
            bool fitDone = hFitter.iterate();

            // report progress:
            //   - after 100ms if it's the first time
            //   - after that every 1000ms
            bool reportProgress = firstProgressNeeded; 
            bool copyResults = fitDone || reportProgress;

            if ( copyResults ) {
                res.params = hFitter.getResults();
                res.diffSq = dataInterface.calculateDiffSq( res.params );
                res.rms = sqrt( res.diffSq / dataInterface.data.size() );
                if ( int ( res.params.size() ) != input.nGaussians * 3 + input.poly ) {
                    throw std::runtime_error(
                              "Gaussian1dFitService::Worker - mismatch in result size" );
                }
            }

            if ( reportProgress ) {
                firstProgressNeeded = false;
                res.status_ = res.Partial;
				char tmp[64];
				sprintf(tmp, "Heuristics #%d\n", iteration);
                res.info = string(tmp);
                checkForInterrupts();
            }

            if ( fitDone ) {
                break;
            }
        }
        //cout << "Heuristic fitter produced diffSq = " << res.diffSq << endl;
    }
    else {
        // if caller provided initial guess, use that instead
        res.params = input.initGuess;
        res.diffSq = dataInterface.calculateDiffSq( res.params );
        res.rms = sqrt( res.diffSq / dataInterface.data.size() );
        res.status_ = res.Partial;
        res.info = string("Initial guess");;
        cout << "Initial guess has diffSq = " << res.diffSq << endl;
    }

    // ----------------------------------------------------------------------
    // setup the LBTA fitter
    // ----------------------------------------------------------------------
    if ( doLBTA ) {
        Optimization::Gaussian1DFitting::LBTAFitter taFitter( dataInterface );
        taFitter.setInitialParams( res.params );

        // start fitting
        iteration = 0;
        while ( true ) {
            iteration++;
            checkForInterrupts();

            //  dbg(1) << "Calling taFitter.iterate()\n";
            bool taDone = taFitter.iterate();

            // report progress:
            //   - after 100ms if it's the first time
            //   - after that every 1000ms
            bool reportProgress = firstProgressNeeded; 
            bool copyResults = taDone || reportProgress;
            if ( copyResults ) {
                res.params = taFitter.getResults();
                res.diffSq = dataInterface.calculateDiffSq( res.params );
                res.rms = sqrt( res.diffSq / dataInterface.data.size() );
                if ( int ( res.params.size() ) != input.nGaussians * 3 + input.poly ) {
                    throw std::runtime_error(
                              "Gaussian1dFitService::Worker - mismatch in result size" );
                }
            }

            if ( reportProgress ) {
                firstProgressNeeded = false;
                res.status_ = res.Partial;
				char tmp[64];
				sprintf(tmp, "TA #%d\n", iteration);
                res.info = string(tmp);
                //res.info = QString( "TA #%1" ).arg( iteration );
                checkForInterrupts();
            }

            if ( taDone ) {
                break;
            }
        }
        //cout << "LBTA fitter produced diffSq = " << res.diffSq << endl;
    }

    if ( doLM ) {
        // ----------------------------------------------------------------------
        // setup up the lev-mar fitter
        // ----------------------------------------------------------------------
        Optimization::Gaussian1DFitting::LMFitter lmfitter( dataInterface );
        lmfitter.setInitialParams( res.params );

        // start fitting
        iteration = 0;
        while ( true ) 
		{
            iteration++;
            checkForInterrupts();
            bool lmDone = lmfitter.iterate();

            // report progress:
            //   - after 100ms if it's the first time
            //   - after that every 1000ms
            bool reportProgress = firstProgressNeeded; 
            bool copyResults = lmDone || reportProgress;
            if ( copyResults ) {
                res.params = lmfitter.getResults();
                res.diffSq = dataInterface.calculateDiffSq( res.params );
#if 0
				cout << "Copy Results" << endl;
				for (size_t i = 0; i < res.input.data.size();i++)
				{
					cout << res.input.data[i] <<",";
				};
#endif

                // TODO: fix up RMS to account for NANs
                res.rms = sqrt( res.diffSq / dataInterface.data.size() );
                if ( int ( res.params.size() ) != input.nGaussians * 3 + input.poly ) {
                    cout << "Gaussian1dFitService::Worker - mismatch in result size" << endl;
                }
            }

#if 1
            if ( reportProgress ) {
                firstProgressNeeded = false;
                res.status_ = res.Partial;
                res.info = string("LevMar Itertion...");
                checkForInterrupts();
            }
#endif

            if ( lmDone ) {
                break;
            }
        };
        //cout << "LevMar iteration = " << iteration << endl;
        //cout << "LevMar fitter produced diffSq = " << res.diffSq << endl;
    };

    res.status_ = res.Complete;
    res.info = "All done.";

	m_gfResult = res;


    int inputSize = m_gfInput.data.size();
    int outputSize = inputSize * 2;
    for ( int i = 0,j=0 ; i < outputSize ; ++i ) 
	{
        double x = double (i) / outputSize * inputSize;
        double y = Optimization::evalNGauss1dBkg( x, m_gfResult.input.nGaussians, m_gfResult.input.poly, m_gfResult.params );
		if(i%2 != 0) continue;

		waveval[j++] = y;
	};

	if(gaussDebug)
		cout << "Center: " << m_gfResult.params[0] <<", Amplitude: " << m_gfResult.params[1] << ", FWHM: " << m_gfResult.params[2] << endl;

	pRec->nord = inputSize;
	dbProcess((dbCommon*)pRec);
    checkForInterrupts();
}

static GaussFit gaussfit;

static long InitGaussFit(aSubRecord *pRec)
{
    return(0);
}

static long ProcGaussFit(aSubRecord *pRec)
{
	long status = 0;
	status = gaussfit.MakeGenData(pRec);
	if(status < 0) 
	{
		cout << "Make GenData Error" << endl;
		return (-1);
	};

    return(0);
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitGaussFit);
epicsRegisterFunction(ProcGaussFit);

long epicsShareAPI resetGaussWave(const int wavesize, const int gauss, const int poly, const float noise, const float sigma)
{
	long status = gaussfit.ResetGaussWave(wavesize, gauss, poly, noise, sigma);
    return (status);
}

static const iocshArg resetGaussWaveArg0 = { "wave size",	iocshArgInt};
static const iocshArg resetGaussWaveArg1 = { "gauss",		iocshArgInt};
static const iocshArg resetGaussWaveArg2 = { "poly",		iocshArgInt};
static const iocshArg resetGaussWaveArg3 = { "noise",		iocshArgDouble};
static const iocshArg resetGaussWaveArg4 = { "sigma",		iocshArgDouble};

static const iocshArg * const resetGaussWaveArgs[] = {&resetGaussWaveArg0, &resetGaussWaveArg1, &resetGaussWaveArg2, &resetGaussWaveArg3, &resetGaussWaveArg4};

static const iocshFuncDef resetGaussWaveFuncDef = {"resetGaussWave",5,resetGaussWaveArgs};

//epics>help resetGaussWave
static void resetGaussWaveCallFunc(const iocshArgBuf *args)
{
    //printf("Ag.1:%s(%p), Ag.2:%d(%p), Ag.3:%d(%p), Ag.4:%d(%p)\n",args[0].sval,&(args[0].sval), args[1].ival,&(args[1].ival), args[2].ival,&(args[2].ival), args[3].ival, &(args[3].ival));
	int wavesize	= args[0].ival;
	if(wavesize > 10000)
	{
		printf("Waveform Size too big(%d)\n", wavesize);
		return;
	}
	int gauss		= (abs(args[1].ival) > 0)?1:0;
	int poly		= (abs(args[2].ival) > 0)?1:0;
	float noise		= fabs(args[3].dval);
	float sigma		= fabs(args[4].dval);

    resetGaussWave(wavesize, gauss, poly, noise, sigma);
}

static void resetGaussWaveRegister(void)
{
	iocshRegister(&resetGaussWaveFuncDef, resetGaussWaveCallFunc);
}

epicsExportAddress(int, gaussDebug);
epicsRegisterFunction(resetGaussWave);
epicsExportRegistrar(resetGaussWaveRegister);
