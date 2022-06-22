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
static int gbintDebug = 0;

hash_map<int, int> hashDelay;

//
static long InitGBInterlock(aSubRecord *pRec)
{
	for(int i = 0;i < 11; i++)
		hashDelay[i] = 0;
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

	int nelm = (int)pRec->noa;
	double *inpval = (double*)pRec->a;
	unsigned char *outval = (unsigned char*)pRec->vala;

	float *inpd = (float*)pRec->d;

	char *b = (char*)pRec->b;
	char *c = (char*)pRec->c;
	double *e = (double*)pRec->e;
	char *f = (char*)pRec->f;

	//int bc = (int)((int*)pRec->b[0]) && (int)(((int*)pRec->c[0]);

	for(int i = 0; i < nelm; i++)
	{
		double inpa = (double)inpval[i];
		bool test = (inpa < inpd[0] && inpa < e[0] )? false : b[0]&&c[0]&&f[0] ? false : true;

		if(test)
			hashDelay[i] += 1;
		else
			hashDelay[i] = 0;

		if(hashDelay[i] > 3)
			outval[i] = 1;
		else
			outval[i] = 0;
	}




	double *sinval = (double*)pRec->a;

#if 1
	double *pFFTWaveVal = (double*)pRec->vala;
	double *pFFTFreqWaveVal = (double*)pRec->valb;
#endif
	if(gbintDebug)
	{
	};

#if 0
	for(int i = 0; i < ffreq.size(); i++)
	{
		pFFTFreqWaveVal[i] = freq(i);
		pFFTWaveVal[i] = std::abs(ffreq(i));
	};
#endif

	return(status);

}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitGBInterlock);
epicsRegisterFunction(ProcGBInterlock);
epicsExportAddress(int, gbintDebug);

