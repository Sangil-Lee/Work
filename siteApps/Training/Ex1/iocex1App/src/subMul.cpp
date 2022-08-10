#include <stdio.h>

#include <registryFunction.h>
#include <subRecord.h>
#include <epicsExport.h>

static int subMulDebug = 1;

static long InitMul(subRecord *pRec)
{
	if(subMulDebug)
		printf("InputA:%f, InputB:%f\n", pRec->a, pRec->b);

	return(0);
}

static long ProcMul(subRecord *pRec)
{
	float inpa = pRec->a;
	float inpb = pRec->b;
	float result = inpa*inpb;

	if(subMulDebug)
	{
		printf("%f x %f = %f\n", inpa, inpb, result);
	}

	pRec->val = result;
	return(0);
}

epicsExportAddress(int, subMulDebug);
epicsRegisterFunction(InitMul);
epicsRegisterFunction(ProcMul);
