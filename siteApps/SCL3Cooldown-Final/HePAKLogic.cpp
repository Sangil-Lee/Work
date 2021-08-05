#include <cstdio>
#include <iostream>
#include <epicsExport.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <aiRecord.h>
#include <calcRecord.h>

using namespace std;

extern "C" {
#if 1
	// Case: Fortran Function
	double dnst_viscosity_(int *code, double *press, double *temp);
#endif
}

 //Paramter A, B
static long InitDnstVisco(subRecord *precord)
{
	return 0;
};

 //Paramter A, B
static long ProcDnstVisco(subRecord *precord)
{
  //a: Pressure, b: TempK, c: Result in Fortran
	double press = precord->a;
	double temp  = precord->b;

	//density code = 3, enthalpy code = 9
	int code = precord->c;

	//Fortran function
	precord->val = dnst_viscosity_(&code, &press, &temp);

	return 0;
};
epicsRegisterFunction(InitDnstVisco);
epicsRegisterFunction(ProcDnstVisco);
