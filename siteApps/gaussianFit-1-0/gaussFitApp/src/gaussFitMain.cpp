/* gaussFitMain.cpp */
/* Author:  Marty Kraimer Date:    17MAR2000 */

#include <stddef.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include "epicsExit.h"
#include "epicsThread.h"
#include "iocsh.h"

#if 0
#include "Gaussian1dFitService.h"
#include "HeuristicGauss1dFitter.h"
#include "LBTAGauss1dFitter.h"
#include "LMGaussFitter1d.h"

using namespace std;

typedef std::vector < double > VD;
static Gaussian1dFitService::InputParametersG1dFit m_gfInput;
static Gaussian1dFitService::ResultsG1dFit m_gfResult;

static void fitGaussian();
static void doWork( Gaussian1dFitService::InputParametersG1dFit input);
static void onGoFromService( Gaussian1dFitService::InputParametersG1dFit input );

static double fact( int i )
{
    if ( i < 2 ) return 1;
    else 
        return double (i) * fact( i - 1 );
}

static VD makeData( int n, int nGauss, int poly, double rnd )
{
    VD params;
    for ( int i = 0 ; i < nGauss ; ++i ) {
        Optimization::Gauss1dNiceParams nice;

        // 1: 1/2
        // 2: 1/3 2/3
        // 3: 1/4 2/4 3/4
        double w = double (n) / ( nGauss + 1.0 );
        nice.center = ( i + 1 ) * w;
        nice.amplitude = 2 - drand48() * rnd;
        nice.fwhm = w * 0.8;
        nice.center += ( drand48() - 0.5 ) * rnd * nice.fwhm;
        nice.amplitude += ( drand48() - 0.5 ) * rnd;
        nice.fwhm += ( drand48() - 0.5 ) * rnd * w * 0.1;

//        center = n/2; ampl = 10; var = -1;
        auto ugly = nice.convertToUgly();
        params.push_back( ugly[0] );
        params.push_back( ugly[1] );
        params.push_back( ugly[2] );
    }
    for ( int i = 0 ; i < poly ; ++i ) {
        if( i % 2 == 1) {
            double ii = (i-1) / 2;
            params.push_back( pow(-1.0,ii) / fact(2*ii+1) / pow(n/8,2*ii+1));
        } else {
            int ii = i / 2;
            params.push_back( pow(-1.0,ii) / fact(2*ii) / pow(n/8,2*ii));
        }
    }

	ofstream ofile("gendData.csv");

    VD result( n );
    for ( int i = 0 ; i < n ; ++i ) {
        result[i] = Optimization::evalNGauss1dBkg( i, nGauss, poly, params );
        result[i] += ( drand48() - 0.5 ) * rnd;

		cout << result[i] << ',';
		ofile << result[i] << endl;
    }
	cout << endl;
	ofile.close();

    return result;
} // makeData

void genData()
{
    auto dataSize = 1000;
    auto nGauss = 1;
    auto nPoly = 0;
    double noise = 0.6;

    m_gfInput.data = makeData( dataSize, nGauss, nPoly, noise );
    m_gfInput.isNull = false;
    m_gfInput.stamp = 1;
    m_gfInput.left = 0;
    m_gfInput.right = m_gfInput.data.size() - 1;
    m_gfInput.randomHeuristicsEnabled = true;

	fitGaussian();
}

void fitGaussian()
{
    int inputSize = m_gfInput.data.size();
    int outputSize = inputSize * 2;
	ofstream offile("gaussData.csv");

    m_gfInput.nGaussians = 1;
    m_gfInput.poly = 0;
    m_gfInput.randomHeuristicsEnabled = false;

	onGoFromService(m_gfInput);

    for ( int i = 0 ; i < outputSize ; ++i ) {
        double x = double (i) / outputSize * inputSize;
        double y = Optimization::evalNGauss1dBkg( x, m_gfResult.input.nGaussians, m_gfResult.input.poly, m_gfResult.params );
		if(i%2==0)
			offile << y << endl;
    }
	offile.close();
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

void onGoFromService( Gaussian1dFitService::InputParametersG1dFit input )
{
    try {
        doWork( input );
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
    cout << "Exiting onGoFromService" << endl;
} // onGoFromService

void doWork( Gaussian1dFitService::InputParametersG1dFit input )
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
        cout << "Data range:" << dataInterface.rangeMin << "," << dataInterface.rangeMax << endl;

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
        cout << "Heuristic fitter produced diffSq = " << res.diffSq << endl;
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
        cout << "LBTA fitter produced diffSq = " << res.diffSq << endl;
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
            //bool reportProgress = progressTimer.elapsed() > 1000 || ( progressTimer.elapsed() > 100 && firstProgressNeeded );
            bool reportProgress = firstProgressNeeded; 
			//if (firstProgressNeeded) usleep(100);
			//else usleep(1000);
            //bool copyResults = lmDone || reportProgress;
            bool copyResults = lmDone;
			cout << "copyResuts: " << copyResults << endl;
            if ( copyResults ) {
				cout << "Copy Results" << endl;
                res.params = lmfitter.getResults();
                res.diffSq = dataInterface.calculateDiffSq( res.params );
				for (size_t i = 0; i < res.input.data.size();i++)
				{
					cout << res.input.data[i] <<",";
				};

                // TODO: fix up RMS to account for NANs
                res.rms = sqrt( res.diffSq / dataInterface.data.size() );
                if ( int ( res.params.size() ) != input.nGaussians * 3 + input.poly ) {
                    cout << "Gaussian1dFitService::Worker - mismatch in result size" << endl;
                }
            }



#if 1
            if ( reportProgress ) {
                firstProgressNeeded = false;
                //progressTimer.restart();
                res.status_ = res.Partial;
                res.info = string("LevMar Itertion...");
                checkForInterrupts();
            }
#endif

            if ( lmDone ) {
                break;
            }
        };
        cout << "LevMar iteration = " << iteration << endl;
        cout << "LevMar fitter produced diffSq = " << res.diffSq << endl;
    };

    res.status_ = res.Complete;
    res.info = "All done.";

	m_gfResult = res;
    checkForInterrupts();
} // Worker::doWork
#endif

int main(int argc,char *argv[])
{
    if(argc>=2) {    
        iocsh(argv[1]);
        epicsThreadSleep(.2);
    }
	//genData();
    iocsh(NULL);
    epicsExit(0);
    return(0);
}
