#include <unistd.h>
#include <cstdio>
#include <string>
#include <fstream>
#include <iomanip>
#include <bitset>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cmath>
#include <complex>
#include <iostream>
#include <numeric>

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

#define hash_map std::unordered_map

using namespace std;
static int gbintDebug = 1;

hash_map<int, int> hashDelay;

//
static long InitGBInterlock(aSubRecord *pRec)
{
	double *outval = (double*)pRec->vala;
	for(int i = 0;i < 11; i++)
	{
		hashDelay[i] = 0;
		outval[i]=1;
	}
    return(0);
}

//
static long ProcGBInterlock(aSubRecord *pRec)
{
	long status = 0;

	/* INPA
	 * SCL3-ALL:IntWF-PT7503:PressR (Waveform, 11)
	 */
	
	/*INPB
	 *SCL3-ALL:IntWF-XV7301:Eval (1 or 0)
	 */
	/*INPC
	 *SCL3-ALL:IntWF-XV7201:Eval (1 or 0)
	 */

	/*INPD
	 * Contant :20.5
	 */
	/*INPE
	 * Cryo-CDL00:TBx01-PT7305:PresR
	 */
	/*INPF
	 * Cryo-CDL00:TBx01-XV7381:CloseStat
	 */


	//OUTA
	//SCL3-ALL:IntWF-XV7502:GBIntWF(Waveform, 11)

	int nelm = (int)pRec->noa;
	double *inpval = (double*)pRec->a;
	double *outval = (double*)pRec->vala;
	long *gintlock = (long*)pRec->valb;

	unsigned short *b = (unsigned short*)pRec->b;
	unsigned short *c = (unsigned short*)pRec->c;
	float *d = (float*)pRec->d;
	double *e = (double*)pRec->e;
	unsigned short *f = (unsigned short*)pRec->f;

	//int bc = (int)((int*)pRec->b[0]) && (int)(((int*)pRec->c[0]);
	
	if(gbintDebug)
	{
		//printf("D:(%f), E:(%f)\n", d[0], e[0]);
		cout <<"B:"<<b[0] <<", C:"<<c[0]<<", D:"<<d[0]<<",E:"<<e[0] <<", F:"<<f[0]<< endl;
	};

	for(int i = 0; i < nelm; i++)
	{
		double inpa = (double)inpval[i];
		bool test = (inpa < d[0] || inpa < e[0] )? false : b[0]&&c[0]&&f[0] ? true : false;

		if(test == true) {
			hashDelay[i] += 1;
		}
		else
		{
			hashDelay[i] = 0;
		}


		if(hashDelay[i] >= 5) {
			outval[i] = 1;
			hashDelay[i]=5;
			gintlock[0] = 1;
		}
#if 0
		//Interlock Release Manual
		else
			outval[i] = 0;
#endif

#if 0
		//Manual Open Status
		test = (inpa < e[0])? false : b[0]&&c[0]&&f[0]? true : false;
		if(test == true) {
			outvalb[i] = 1;
		}
		else{
			outvalb[i] = 0;
		}
#endif
	}

#if 0
	//accumulate(hashDelay.begin(), hashDelay.end(), 0);
	if(gbintDebug)
		cout << "Sum: " << accumulate(hashDelay.begin(), hashDelay.end(), 0, [](const size_t previous, decltype(*hashDelay.begin()) p) { return previous+p.second; }) << endl;
	 if(accumulate(hashDelay.begin(), hashDelay.end(), 0, [](const size_t previous, decltype(*hashDelay.begin()) p) { return previous+p.second; }) == 0 )
			gintlock[0] = 0;
#endif


	if(gbintDebug)
	{
		for(size_t i = 0; i < hashDelay.size(); i++)
			cout << hashDelay[i];

		cout << endl;
	};

	return(status);

}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitGBInterlock);
epicsRegisterFunction(ProcGBInterlock);
epicsExportAddress(int, gbintDebug);

