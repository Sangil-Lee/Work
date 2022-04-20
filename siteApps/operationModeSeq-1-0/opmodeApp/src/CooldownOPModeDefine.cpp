#include <cstring>
#include <string>
#include <iostream>
#include <unordered_map>

#include <registryFunction.h>
#include <aSubRecord.h>
#include <stringoutRecord.h>
#include <epicsExport.h>

#define hash_map std::unordered_map

using namespace std;

hash_map<double, string> opmode2str;

static long InitOPMode(aSubRecord *prec)
{
	opmode2str[0.0] = string("C.D Stoped and Ready");
	opmode2str[0.3] = string("Purging & Purification");
	opmode2str[0.4] = string("Valve Positioning");
	opmode2str[0.5] = string("Valve Closing");
	opmode2str[0.6] = string("Stop Purging");
	opmode2str[1.0] = string("Linac Ready");
	opmode2str[1.1] = string("C.D 300K to 4.5K Wo. C.M");
	//opmode2str[1.2] = string("C.D 300K to 4.5K Ex. C.M W. EBx LHe Fil");
	opmode2str[1.2] = string("C.D 300K to 4.5K W. EBx LHe Fill Wo. C.M");
	opmode2str[1.3] = string("Standby 4.5K Ex. C.M");
	opmode2str[1.4] = string("C.M Cool Down 4.5K");
	opmode2str[1.5] = string("C.M LHe Filling");
	opmode2str[2.0] = string("Standby 4.5K");
	opmode2str[2.1] = string("C.M Cool Down 4.5K to 2.05K");
	opmode2str[3.0] = string("Standby 2.05K");
	opmode2str[4.0] = string("Normal Operation");
	opmode2str[5.1] = string("Warm-up 4.5K to 300K");
	opmode2str[5.2] = string("Empty Reservoir");
	opmode2str[5.3] = string("Warm-up 2.05 to 4.5K");
	return 0;
}

static long ProcOPMode(aSubRecord *prec)
{
	double *aval = (double*)prec->a;
	char   *outwf = (char*)prec->vala;

	if(aval == nullptr || outwf == nullptr) return(-1);

	string stromstr;
	try {
		stromstr = opmode2str.at(aval[0]);
	} catch ( const std::out_of_range & oor) {
		//cout << oor.what() << endl;
		stromstr = string("Not Defined");
	}

	//printf("%f, %s\n", aval[0], stromstr.c_str());
	strcpy(outwf, stromstr.c_str());

	return 0;
}

epicsRegisterFunction(InitOPMode);
epicsRegisterFunction(ProcOPMode);
