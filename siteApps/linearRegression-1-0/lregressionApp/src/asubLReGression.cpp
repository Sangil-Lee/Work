#include <unistd.h>
#include <cstdio>
#include <string>
#include <fstream>
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

#include <Eigen/Dense>

#define	OUTFILE 0

using namespace std;
using namespace Eigen;
//static int lregressDebug = 0;
//
VectorXd polynomial(VectorXd xvals, VectorXd yvals, int order)
{
	assert(xvals.size() == yvals.size());
	assert(order >= 1 && order <= xvals.size()-1 );

	MatrixXd A(xvals.size(), order + 1);

	for(int i = 0; i < xvals.size(); i++) {
		A(i, 0) = 1.0;
	};

	for(int j = 0; j < xvals.size(); j++) {
		for(int i = 0; i < order; i++) {
			A(j, i+1) = A(j,i)*xvals(j);
		};
	};

	auto Q = A.householderQr();
	auto coeff = Q.solve(yvals);

	return coeff;
}

double polynomial_calc(VectorXd coeffs, double xval)
{
	double result = 0.0;
	for(int i = 0; i < coeffs.size(); i++)
		result += coeffs(i) * pow(xval, i);

	return result;
}

static long InitLReGression(aSubRecord *pRec)
{
    return(0);
}

static long ProcLReGression(aSubRecord *pRec)
{
	long status = 0;

	double *aval_deg = (double*)pRec->a;
	DBADDR *pdbTempWave = (DBADDR*)(&pRec->inpb)->value.pv_link.pvt;

	//waveformRecord *pTempWave = (waveformRecord *)pdbTempWave->precord;
	
	double *pTempWaveVal = (double*)pRec->b;
	long nelm = pdbTempWave->no_elements;

	DBADDR *pdbLRDBAddr = (DBADDR*)(&pRec->outa)->value.pv_link.pvt;
	waveformRecord *pLRWave = (waveformRecord *)pdbLRDBAddr->precord;
	double *pLRWaveVal = (double*)pRec->valb;


	VectorXd xvals(nelm), yvals(nelm);
	for (int i=0;i<nelm;i++) 
	{
		xvals(i) = i;
		yvals(i) = pTempWaveVal[i];
	};

	auto coeffs = polynomial(xvals, yvals, aval_deg[0]); 

	cout << "Poly-Coeff: " << coeffs << endl << endl;

	for(int i = 0; i < yvals.size(); i++)
	{
		pLRWaveVal[i] = polynomial_calc(coeffs, xvals(i));
	};

	dbProcess((dbCommon*)pLRWave);
	return(status);

#if 0
	int degree, N;
	cout.precision(4);                        //set precision
	cout.setf(ios::fixed);
	cout<<"\nEnter the no. of data pairs to be entered:\n";        //To find the size of arrays that will store x,y, and z values
	cin>>N;

	VectorXd xvals(N), yvals(N);
	//double x[N],y[N];
	cout<<"\nEnter the x-axis values:\n";                //Input x-values
	for (int i=0;i<N;i++)
		cin>>xvals[i];

	cout<<"\nEnter the y-axis values:\n";                //Input y-values
	for (int i=0;i<N;i++)
		cin>>yvals[i];

	cout<<"\nWhat degree of Polynomial do you want to use for the fit?\n";
	cin>>degree;                                // n is the degree of Polynomial 


	// TODO: use `polyfit` to fit a third order polynomial to the (x, y)
	//auto coeffs = polyfit(xvals,yvals,3); 
	//
	// polynomial coefficients.
	auto coeffs = polynomial(xvals, yvals, degree); 

	cout << "Coeff: " << coeffs << endl;
	cout << endl;
	//cout << "Coeff: " << coeffs << ", Coeff[0]:" << coeffs(0) << ", Coeff[1]:" << coeffs(1) << ", Coeff[2]:" << coeffs(2) << endl;

	for(int i = 0; i < yvals.size(); i++)
	{
		double val = polynomial_calc(coeffs, xvals(i));
		printf("Y-Val[%d](%f) - Poly_Y-Val[%d](%f) = %f\n", i, i, yvals(i), val,  yvals(i) - val);
	};
#endif


#if 0
	DBADDR *pdbAddr1 = (DBADDR*)(&pRec->outa)->value.pv_link.pvt;
	DBADDR *pdbAddr2 = (DBADDR*)(&pRec->outb)->value.pv_link.pvt;
	DBADDR *pdbAddr3 = (DBADDR*)(&pRec->outc)->value.pv_link.pvt;

	waveformRecord *pWave1 = (waveformRecord *)pdbAddr1->precord;
	waveformRecord *pWave2 = (waveformRecord *)pdbAddr2->precord;
	waveformRecord *pWave3 = (waveformRecord *)pdbAddr3->precord;
	double *aval = (double*)pRec->a;
	double *bval = (double*)pRec->b;
	double *cval = (double*)pRec->c;


	if(pdbAddr1 == NULL || pWave1 == NULL) return(-1);
	if(pdbAddr2 == NULL || pWave2 == NULL) return(-1);
	if(pdbAddr3 == NULL || pWave3 == NULL) return(-1);

	double *wave1 = (double*)pRec->vala;
	double *wave2 = (double*)pRec->valb;
	double *wave3 = (double*)pRec->valc;
	long nelm = pdbAddr1->no_elements;

	//random number
	srand(time(nullptr));
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<> dis(0.1, 0.5);

	std::time_t datetime = std::time(nullptr);

	double randomval = 0.0;
    for (int i=0; i < nelm; i++) {
		randomval = dis(gen);
        wave1[i] = randomval*aval[0];
        wave2[i] = randomval*bval[0];
        wave3[i] = randomval*cval[0];
	};
	pWave1->nord = pWave2->nord = pWave3->nord = nelm;

	dbProcess((dbCommon*)pWave1);
	dbProcess((dbCommon*)pWave2);
	dbProcess((dbCommon*)pWave3);
#endif
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitLReGression);
epicsRegisterFunction(ProcLReGression);
