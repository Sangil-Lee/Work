#include <cstdio>
#include <iostream>
#include <epicsExport.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <aiRecord.h>
#include <calcRecord.h>

using namespace std;

extern "C" {
	// Case: Fortran Function
	double dnst_enthalpy_(int *code, double *press, double *temp);
}

 //Paramter A, B
static long InitHePAKDensity(subRecord *precord)
{
	return 0;
};
 //Paramter A, B
static long ProcHePAKDensity(subRecord *precord)
{
  //a: Pressure, b: TempK, c: Result in Fortran
	double press = precord->a;
	double temp  = precord->b;

	//density code = 3, enthalpy code = 9
	int code = 3;

	//Fortran function
	precord->val = dnst_enthalpy_(&code, &press, &temp);

	return 0;
};
static long InitHePAKEnthalpy(subRecord *precord)
{
	return 0;
};
 //Paramter A, B
static long ProcHePAKEnthalpy(subRecord *precord)
{
  //a: Pressure, b: TempK, c: Result in Fortran
	double press = precord->a;
	double temp  = precord->b;

	//density code = 3, enthalpy code = 9
	int code = 9;

	//Fortran function
	precord->val = dnst_enthalpy_(&code, &press, &temp);

	return 0;
};
epicsRegisterFunction(InitHePAKDensity);
epicsRegisterFunction(ProcHePAKDensity);
epicsRegisterFunction(InitHePAKEnthalpy);
epicsRegisterFunction(ProcHePAKEnthalpy);
