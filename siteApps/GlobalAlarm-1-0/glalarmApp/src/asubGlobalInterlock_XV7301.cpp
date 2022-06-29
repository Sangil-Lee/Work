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
static int gbintDebugXV7301 = 0;

static hash_map<int, int> hashdelay;

//
static long InitGBInterlockXV7301(aSubRecord *pRec)
{
	double *outval = (double*)pRec->vala;
	for(int i = 0;i < 11; i++)
	{
		hashdelay[i] = 0;
		outval[i]=1;
	}
    return(0);
}

//
static long ProcGBInterlockXV7301(aSubRecord *pRec)
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

	double *b = (double*)pRec->b;
	double *c = (double*)pRec->c;
	unsigned short *d = (unsigned short*)pRec->d;
	unsigned short *e = (unsigned short*)pRec->e;

	if(gbintDebugXV7301)
	{
		//printf("D:(%f), E:(%f)\n", d[0], e[0]);
		cout <<"B:"<<b[0] <<", C:"<<c[0]<<", D:"<<d[0]<<",E:"<<e[0]<< endl;
	};

#if 1
	for(int i = 0; i < nelm; i++)
	{
		double inpa = (double)inpval[i];
		bool test = (inpa < b[0] || inpa < c[0] )? false : d[0]&&e[0] ? true : false;

		if(test == true) {
			hashdelay[i] += 1;
		}
		else
		{
			hashdelay[i] = 0;
		}


		if(hashdelay[i] >= 5) {
			outval[i] = 1;
			hashdelay[i]=5;
			gintlock[0] = 1;
		}
	}
#endif

	if(gbintDebugXV7301)
	{
		for(size_t i = 0; i < hashdelay.size(); i++)
			cout << hashdelay[i];

		cout << endl;
	};

	return(status);

}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitGBInterlockXV7301);
epicsRegisterFunction(ProcGBInterlockXV7301);
epicsExportAddress(int, gbintDebugXV7301);

