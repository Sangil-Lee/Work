#include <bitset>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <chrono>
#include <cmath>

#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>

using namespace std;

static long InitNoiseWave(aSubRecord *pRec)
{
    return(0);
}

static long ProcNoiseWave(aSubRecord *pRec)
{
	//cout << "Size: " << sizeB[0] << ", Freq: " << freqC[0] << endl;

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::default_random_engine generator(seed);

	auto *mean    = static_cast<double*>(pRec->a);
	auto *stddev  = static_cast<double*>(pRec->b);
	auto *outvala = static_cast<double*>(pRec->vala);

	try {
		std::normal_distribution<double> dist(mean[0], stddev[0]);

		//Adding Gaussian Noise
		for(size_t i = 0; i < pRec->nova; i++)
		{
			outvala[i] = dist(generator);
		};


	} catch(exception &e) {
		cout << e.what() << endl;
	}

	//std::copy(begin(wavedata), end(wavedata), std::ostream_iterator<double>(std::cout, " "));
	//std::cout << std::endl;

    return (0);
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitNoiseWave);
epicsRegisterFunction(ProcNoiseWave);

