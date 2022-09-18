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
	double dnst_enthalpy_(int *code, double *press, double *temp);
#endif
}

 //Paramter A, B
static long DnstInit(subRecord *precord)
{
	return 0;
};
 //Paramter A, B
static long DnstProc(subRecord *precord)
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
static long InEnthalInit(subRecord *precord)
{
	return 0;
};
 //Paramter A, B
static long InEnthalProc(subRecord *precord)
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
static long OutEnthalInit(subRecord *precord)
{
	return 0;
};
 //Paramter A, B
static long OutEnthalProc(subRecord *precord)
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
epicsRegisterFunction(DnstInit);
epicsRegisterFunction(DnstProc);
epicsRegisterFunction(InEnthalInit);
epicsRegisterFunction(InEnthalProc);
epicsRegisterFunction(OutEnthalInit);
epicsRegisterFunction(OutEnthalProc);
