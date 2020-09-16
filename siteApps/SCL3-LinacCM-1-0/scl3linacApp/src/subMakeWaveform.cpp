#include <aSubRecord.h>
#include <epicsExport.h>

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
#include <epicsExport.h>

#include <cstdio>
#include <string>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <cmath>

#define dbGetPdbAddrFromLink(PLNK) \
    ( ( (PLNK)->type != DB_LINK ) \
      ? 0 \
      : ( ( (struct dbAddr *)( (PLNK)->value.pv_link.pvt) ) ) )


using namespace std;

static long makeLinear(aSubRecord *pRec)
{
	//Logic Verified
#if 1
	DBADDR *pdbAddr = (DBADDR*)(&pRec->outa)->value.pv_link.pvt;
	waveformRecord *pWave = (waveformRecord *)pdbAddr->precord;
	double *aval = (double*)pRec->a;

	if(pdbAddr == NULL || aval == NULL || pWave == NULL) return(-1);

	double *wave = (double*)pRec->vala;
	long nelm = pdbAddr->no_elements;

    double x[nelm], y[nelm];
    double xsum=0,x2sum=0,ysum=0,xysum=0;     //variables for sums/sigma of xi,yi,xi^2,xiyi etc

    for (int i=0;i<nelm;i++) 
	{
		x[i] = i+1;
		y[i] = aval[i];

		//printf("(X, Y) = (%f, %f)\n", x[i], y[i]);
	};

    for (int i=0;i < nelm; i++) {
        xsum  += x[i];            //calculate sigma(xi)
        ysum  += y[i];			//calculate sigma(yi)
        x2sum += pow(x[i],2);     //calculate sigma(x^2i)
        xysum += x[i]*y[i];		//calculate sigma(xi*yi)
    };

	//calculate slope
    double a = (nelm*xysum-xsum*ysum)/(nelm*x2sum-xsum*xsum);

	//calculate intercept
    double b = (x2sum*ysum-xsum*xysum)/(nelm*x2sum-xsum*xsum);

	//cout << "No.E: " << nelm <<", y= ax + b" <<",a=" << a << ", b=" << b << endl;

	//an array to store the new fitted values of y    
    for (int i=0; i < nelm; i++) {
		//to calculate y(fitted) at given x points
        wave[i] = a*x[i]+b; 
	};

	pWave->nord = nelm;
	dbProcess((dbCommon*)pWave);
#endif
	return (0);
}

static long makePolynomial(aSubRecord *pRec, const int order /*n-th order */)
{
#if 1
	DBADDR *pdbAddr = (DBADDR*)(&pRec->outb)->value.pv_link.pvt;
	waveformRecord *pWave = (waveformRecord *)pdbAddr->precord;
	double *aval = (double*)pRec->a;

	if(pdbAddr == NULL || aval == NULL || pWave == NULL) return(-1);

	double *wave = (double*)pRec->valb;
	long nelm = pdbAddr->no_elements;

    int i,j,k;
	int n = order;

    double x[nelm],y[nelm];
    for (i=0;i<nelm;i++) 
	{
		x[i] = i+1;
		y[i] = aval[i];
	};

    double X[2*n+1];                        //Array that will store the values of sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    for (i=0;i<2*n+1;i++)
    {
        X[i]=0;
        for (j=0;j<nelm;j++)
            X[i]=X[i]+pow(x[j],i);        //consecutive positions of the array will store N,sigma(xi),sigma(xi^2),sigma(xi^3)....sigma(xi^2n)
    };

    double B[n+1][n+2],a[n+1];            //B is the Normal matrix(augmented) that will store the equations, 'a' is for value of the final coefficients
    for (i=0;i<=n;i++)
	{
        for (j=0;j<=n;j++)
            B[i][j]=X[i+j];            //Build the Normal matrix by storing the corresponding coefficients at the right positions except the last column of the matrix
	};

    double Y[n+1];                    //Array to store the values of sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
    for (i=0;i<n+1;i++)
    {    
        Y[i]=0;
        for (j=0;j<nelm;j++)
        Y[i]=Y[i]+pow(x[j],i)*y[j];        //consecutive positions will store sigma(yi),sigma(xi*yi),sigma(xi^2*yi)...sigma(xi^n*yi)
    };

    for (i=0;i<=n;i++)
	{
        B[i][n+1]=Y[i];                //load the values of Y as the last column of B(Normal Matrix but augmented)
	};

    n=n+1;                //n is made n+1 because the Gaussian Elimination part below was for n equations, but here n is the degree of polynomial and for n degree we get n+1 equations

    for (i=0;i<n;i++)                    //From now Gaussian Elimination starts(can be ignored) to solve the set of linear equations (Pivotisation)
	{
        for (k=i+1;k<n;k++)
		{
            if (B[i][i]<B[k][i])
			{
                for (j=0;j<=n;j++)
                {
                    double temp=B[i][j];
                    B[i][j]=B[k][j];
                    B[k][j]=temp;
                };
			};
		};
	};

    for (i=0;i<n-1;i++)            //loop to perform the gauss elimination
	{
		for (k=i+1;k<n;k++)
		{
			double t=B[k][i]/B[i][i];
			for (j=0;j<=n;j++)
				B[k][j]=B[k][j]-t*B[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
		};
	};

    for (i=n-1;i>=0;i--)                //back-substitution
    {                        //x is an array whose values correspond to the values of x,y,z..
        a[i]=B[i][n];                //make the variable to be calculated equal to the rhs of the last equation
        for (j=0;j<n;j++)
		{
            if (j!=i)            //then subtract all the lhs values except the coefficient of the variable whose value is being calculated
			{
                //a[i]=a[i]-(B[i][j]*a[j]);
                a[i] -= B[i][j]*a[j];
			};
		};
        a[i]=a[i]/B[i][i];            //now finally divide the rhs by the coefficient of the variable to be calculated
    };

#if 0
	double x2 =0.0, x1 = 0.0, x0= 0.0;
	for (i=0;i<n;i++)
	{
		cout<<"x^"<<i<<"="<<a[i]<<endl;            // Print the values of x^0,x^1,x^2,x^3,....    
		switch(i)
		{
			case 0: 
				x0 = a[i];
				break;
			case 1: 
				x1 = a[i];
				break;
			case 2: 
				x2 = a[i];
				break;
		};
	};
#endif

    for (int idx=0; idx < nelm; idx++) 
	{
		double solve = 0.;
		for (i=0;i<n;i++)
		{
			//cout<<"x^"<<i<<"="<<a[i]<<endl;            // Print the values of x^0,x^1,x^2,x^3,....    
			solve += a[i+1]*x[idx];
		};
		solve += a[0];
		//solve = x2*x[idx] + x1*x[idx] + x0;
		wave[idx] = solve;
	};
     
	pWave->nord = nelm;
	dbProcess((dbCommon*)pWave);

#endif

	return (0);
}

static long InitMakeWave(aSubRecord *pRec)
{
    return(0);
}

//static long CalcASUB2(aSubRecord *p_asubrec)
static long ProcMakeWave(aSubRecord *pRec)
{
	long status = 0;
	status = makeLinear(pRec);
	if(status < 0) 
	{
		cout << "Make Linear Waveform Error" << endl;
		return (-1);
	};
	status = makePolynomial(pRec, 2);

	if(status < 0) 
	{
		cout << "Make Linear Waveform Error" << endl;
		return (-1);
	};
    return(0);
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitMakeWave);
epicsRegisterFunction(ProcMakeWave);
