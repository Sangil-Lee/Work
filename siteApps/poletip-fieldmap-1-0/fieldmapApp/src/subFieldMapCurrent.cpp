#include <iostream>
#include <fstream>
#include <map>
#include <utility>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <cmath>

#include <dbAccess.h>
#include <link.h>
#include <dbDefs.h>
#include <dbLock.h>
#include <dbAddr.h>
#include <epicsThread.h>
#include <epicsMessageQueue.h>
#include <registryFunction.h>
#include <recSup.h>
#include <subRecord.h>
#include <epicsExport.h>

using namespace std;

//static int gb_Debug = 0;
//epicsExportAddress(int, gb_Debug); /*iocsh export variable*/

double getRandom(double min, double max)
{
		double before = rand() % (int)max + (int)min;
		double after = (double)rand() / RAND_MAX;
		double result = before + after;
		if (result < min || result > max) {
				result = getRandom(min, max);
		}
		return result;
}

class FieldMapCurrent {
public:
	FieldMapCurrent(const string filename):mfilename(filename) {
		printf("FieldMapCurrent Constructor\n");
		loadData();
	};

	~FieldMapCurrent() {
		printf("~FieldMapCurrent Destructor\n");
	};

	double GetClosestKey(double pvvalue);
	//Variable
private:
	string mfilename;
	ifstream dataFile;
	//Map<Field Gradient, Current>
	map<double, double> m;
	string strToken;

	//Function
private:
	int loadData();
};

int FieldMapCurrent::loadData()
{
		string strToken;
		double gradient = 0.0;
		double current = 0.0;

		dataFile.open(mfilename.c_str());
		if(dataFile.fail())
		{
				cout <<"File Not Found" << endl;
				return(-1);
		};

		while(!dataFile.eof())
		{
				getline(dataFile, strToken);
				if(strToken[0] == '#' || strToken.empty() == true) continue;

				char str[strToken.size()];
				strcpy (str, strToken.c_str());

				char *pch = 0;
				if(!(pch = strtok (str," \t,"))) continue;
				//printf("Field Gradient: %s\n", pch);
				gradient = strtod(pch, NULL);

				while (pch != NULL)
				{
						if(!(pch = strtok (NULL," \t,"))) continue;
						//printf("Current: %s\n", pch);
						current = strtod(pch, NULL);
						m.insert(std::pair<double,double>(gradient, current));
				};
		}

		return(0);
}

double FieldMapCurrent::GetClosestKey(double pv_value)
{
		//std::pair<double, double> range( floor(10.0 * value)/10.0, floor(1000.0*value)/1000.0);
		//std::map<double, double>::iterator low = m.lower_bound( range.first );
		//std::map<double, double>::iterator  upper = m.upper_bound( range.second );
		std::map<double, double>::iterator low = m.lower_bound(pv_value);
		//std::cout << low->first << ' ' << low->second << std::endl;
		return (low->second);
}

#if 0
static FieldMapCurrent fieldmapcurrentHWR("../../db/bptHWRHI");
static FieldMapCurrent fieldmapcurrentQWRO("../../db/bptQWROLDHI");
static FieldMapCurrent fieldmapcurrentQWRN("../../db/bptQWRNEWHI");
#endif

static long FieldMapCurrentInit(subRecord *psubrec)
{
	return 0;
}

static long FieldMapCurrentProc(subRecord *psubrec)
{
	if(psubrec == NULL) return -1;

	psubrec->val = fieldmapcurrentHWR.GetClosestKey(psubrec->a);
	return 0;
}

static long FieldMapCurrentQWROInit(subRecord *psubrec)
{
	return 0;
}

static long FieldMapCurrentQWROProc(subRecord *psubrec)
{
	if(psubrec == NULL) return -1;

	psubrec->val = fieldmapcurrentQWRO.GetClosestKey(psubrec->a);
	return 0;
}
static long FieldMapCurrentQWRNInit(subRecord *psubrec)
{
	return 0;
}

static long FieldMapCurrentQWRNProc(subRecord *psubrec)
{
	if(psubrec == NULL) return -1;

	psubrec->val = fieldmapcurrentQWRN.GetClosestKey(psubrec->a);
	return 0;
}

epicsRegisterFunction(FieldMapCurrentInit);
epicsRegisterFunction(FieldMapCurrentProc);
epicsRegisterFunction(FieldMapCurrentQWROInit);
epicsRegisterFunction(FieldMapCurrentQWROProc);
epicsRegisterFunction(FieldMapCurrentQWRNInit);
epicsRegisterFunction(FieldMapCurrentQWRNProc);
