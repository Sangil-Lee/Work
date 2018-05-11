#include <cstdio>
#include <iostream>
#include <epicsExport.h>
#include <registryFunction.h>
#include <subRecord.h>
#include <aiRecord.h>
#include <calcRecord.h>

using namespace std;

extern "C" {
	float dnst6501_(float *a, float *b);
	float dnst6505_(float *a, float *b);
	float tempk5301_(float *a);
	float dnstl5301_(float *a);
	float dnst5301_(float *a);
}


//Paramter A,B
static long Dnst6501Init (subRecord *precord)
{
	return 0;
};
//Paramter A,B
static long Dnst6501Proc (subRecord *precord)
{
	float a = precord->a;
	float b = precord->b;

	//printf("A:%f, B:%f, Multiply:%f\n", a, b, dnst6501_(&a,&b));

	precord->val = dnst6501_(&a, &b);
	
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
	float a = precord->a;
	float b = precord->b;

	//printf("A:%f, B:%f, Multiply:%f\n", a, b, dnst6505_(&a,&b));
	precord->val = dnst6505_(&a, &b);
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
	float a = precord->a;
	precord->val = tempk5301_(&a);
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
	float a = precord->a;
	precord->val = dnstl5301_(&a);
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
	float a = precord->a;
	precord->val = dnst5301_(&a);
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
