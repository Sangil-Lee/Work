#include <iostream>
#include <algorithm>
#include <aSubRecord.h>
#include <epicsExport.h>
#include <registryFunction.h>


using namespace std;

static long InitHexaStr2Dec(aSubRecord *precord)
{
    return(0);
}

static long ProcHexaStr2Dec(aSubRecord *precord)
{
	long status = 0;

	const char* hexstring = (const char*)precord->a;
	//std::cout << hexstring << std::endl;
	if(hexstring == nullptr) {
	   return -1;
	}

	int number = (int)strtol(hexstring, NULL, 16);
	long *uval = (long*)precord->vala;
	uval[0] = number;

    return (status);
}

/* Register these symbols for use by IOC code: */
epicsRegisterFunction(InitHexaStr2Dec);
epicsRegisterFunction(ProcHexaStr2Dec);
