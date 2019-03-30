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
	double dnst6501_(double *temp, double *press);
	double dnst6505_(double *temp, double *press);
	double tempk5301_(double *press);
	double dnstl5301_(double *press);
	double dnst5301_(double *press);
#else
	// Case: Fortran Subroutine
	void dnst6501_(float *a, float *b, float *c);
	void dnst6505_(float *a, float *b, float *c);
	void tempk5301_(float *a, float *b);
	void dnstl5301_(float *a, float *b);
	void dnst5301_(float *a, float *b);
#endif
}

 //Paramter A, B
static long Dnst6501Init (subRecord *precord)
{
	return 0;
};
 //Paramter A, B
static long Dnst6501Proc (subRecord *precord)
{
  //a: Temperature, b: Pressure, c: Result in Fortran
	double temp  = precord->a;
	double press = precord->b;

	//printf("Result:%f\n", dnst6501_(&temp,&press));
	//Fortran function
	precord->val = dnst6501_(&temp, &press);

	//Fortran Subroutine
	//dnst6501_(&a, &b, &c);
	//precord->val = c;
	
	return 0;
};

//Paramter A',E
static long Dnst6505Init (subRecord *precord)
{
	return 0;
};
//Paramter A',E
static long Dnst6505Proc (subRecord *precord)
{
	double temp  = precord->a;
	double press = precord->b;

	//printf("A:%f, B:%f, Multiply:%f\n", a, b, dnst6505_(&a,&b));
	precord->val = dnst6505_(&temp, &press);

	//Fortran Subroutine
	//dnst6505_(&a, &b, &c);
	//precord->val = c;

	return 0;
};

//Paramter I 
static long TempK5301Init (subRecord *precord)
{
	return 0;
};
//Paramter I 
static long TempK5301Proc (subRecord *precord)
{
	double press = precord->a;
	precord->val = tempk5301_(&press);
	return 0;
};

//Paramter I 
static long DnstL5301Init (subRecord *precord)
{
	return 0;
};
//Paramter I 
static long DnstL5301Proc (subRecord *precord)
{
	double press = precord->a;
	precord->val = dnstl5301_(&press);
	return 0;
};

//Paramter I 
static long Dnst5301Init (subRecord *precord)
{
	return 0;
};
//Paramter I 
static long Dnst5301Proc (subRecord *precord)
{
	double press = precord->a;
	precord->val = dnst5301_(&press);
	return 0;
};

epicsRegisterFunction(Dnst6501Init);
epicsRegisterFunction(Dnst6501Proc);
epicsRegisterFunction(Dnst6505Init);
epicsRegisterFunction(Dnst6505Proc);
epicsRegisterFunction(TempK5301Init);
epicsRegisterFunction(TempK5301Proc);
epicsRegisterFunction(DnstL5301Init);
epicsRegisterFunction(DnstL5301Proc);
epicsRegisterFunction(Dnst5301Init);
epicsRegisterFunction(Dnst5301Proc);
