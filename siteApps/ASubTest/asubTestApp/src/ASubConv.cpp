#include <cstring>
#include <string>
#include <iostream>
#include <unordered_map>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <dbAccess.h>
#include <link.h>
#include <dbDefs.h>
#include <dbLock.h>
#include <dbAddr.h>
#include <epicsThread.h>
#include <epicsMessageQueue.h>
#include <registryFunction.h>
#include <recSup.h>
#include <aSubRecord.h>
#include <epicsExport.h>
#include <waveformRecord.h>

#define hash_map std::unordered_map

using namespace std;

hash_map<double, string> opmode2str;

static long InitOMConv(aSubRecord *prec)
{
	opmode2str[0.1] = string("Cool-down Stoped and Ready");
	opmode2str[0.3] = string("Purging & Purification");
	opmode2str[1.0] = string("Linac Ready");
	return 0;
}

static long ProcOMConv(aSubRecord *prec)
{
	double *aval = (double*)prec->a;
	char   *outwf = (char*)prec->vala;

	string stromstr;
	try {
		stromstr = opmode2str.at(aval[0]);
	} catch ( const std::out_of_range & oor)
	{
		cout << oor.what() << endl;
		stromstr = string("Not Defined");
	}

#if 0
	if (aval[0] == 0.1) strcpy(outwf, "Cool-down Stoped and Ready");
	else if (aval[0] == 0.3) strcpy(outwf, "Purging & Purification");
	else if (aval[0] == 1.0) strcpy(outwf, "Linac Ready");
#endif
	strcpy(outwf, stromstr.c_str());

	printf("%f, %s\n", aval[0], stromstr.c_str());

	return 0;
}

epicsRegisterFunction(InitOMConv);
epicsRegisterFunction(ProcOMConv);
