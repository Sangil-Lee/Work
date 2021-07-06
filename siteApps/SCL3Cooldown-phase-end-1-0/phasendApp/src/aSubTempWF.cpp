
#include <iostream>
#include <dbAccess.h>
#include <registryFunction.h>
#include <waveformRecord.h>
#include <aSubRecord.h>
#include <epicsExport.h>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;
static int lregressDebug = 0;

VectorXd Polynomial(VectorXd xvals, VectorXd yvals, int order)
{
    assert(xvals.size() == yvals.size());
    assert(order >= 1 && order <= xvals.size()-1 );

    MatrixXd A(xvals.size(), order + 1);

    for(int i = 0; i < xvals.size(); i++) {
        A(i, 0) = 1.0;
    };
    if(lregressDebug == 1) 
      cout << "A-Mat(unit): " <<  A << endl;

    for(int j = 0; j < xvals.size(); j++) {
        for(int i = 0; i < order; i++) {
            A(j, i+1) = A(j,i)*xvals(j);
        };
    };

    if(lregressDebug == 1)
      cout << "A-Mat: " <<  A << endl;

    auto Q = A.householderQr();

//  if(lregressDebug == 1)
//      cout << "Q: " <<  A << endl;

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
};
static long ProcLReGression(aSubRecord *pRec)
{
    long status = 0;

    double *aval_deg = (double*)pRec->a;
    DBADDR *pdbTempWave = (DBADDR*)(&pRec->inpb)->value.pv_link.pvt;
    waveformRecord *pTempWave = (waveformRecord *)pdbTempWave->precord;

    //printf("Wave NORD:%d, NELM:%d \n", pTempWave->nord, pdbTempWave->no_elements );
    //if(pTempWave->nord != pdbTempWave->no_elements ) return(-1);
    //double *pTempWaveVal = (double*)pdbTempWave->pfield;

    double *pTempWaveVal = (double*)pRec->b;
    long nelm = pdbTempWave->no_elements;

#if 1
    DBADDR *pdbLRDBAddr = (DBADDR*)(&pRec->outa)->value.pv_link.pvt;
    waveformRecord *pLRWave = (waveformRecord *)pdbLRDBAddr->precord;
    double *pLRWaveVal = (double*)pRec->vala;
#endif

    VectorXd xvals(nelm), yvals(nelm);
    double dval;
    for (int i=0;i<nelm;i++)
    {
        xvals(i) = i;
        dval = pTempWaveVal[i];
        yvals(i) = dval;

        if(lregressDebug == 2)
            printf("X(%d):%f, Y(%d):%f\n", i, xvals(i), i, yvals(i));
    };

    auto coeffs = Polynomial(xvals, yvals, aval_deg[0]);

    if(lregressDebug == 2)
        cout << "Poly-Coeff: " << coeffs << endl << endl;

#if 1
    for(int i = 0; i < yvals.size(); i++)
    {
        pLRWaveVal[i] = polynomial_calc(coeffs, xvals(i));
    };
#endif

    dbProcess((dbCommon*)pLRWave);
    return(status);
};

epicsRegisterFunction(InitLReGression);
epicsRegisterFunction(ProcLReGression);
epicsExportAddress(int, lregressDebug);
