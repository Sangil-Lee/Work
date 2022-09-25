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

class CVFlowMap
{

	public:
		CVFlowMap(const string filename):mfilename(filename)
	{
		printf("CVFlowMap Constructor\n");
		//m.get_allocator().allocate(100000);
		loadData();
	};

		~CVFlowMap()
		{
			printf("~CVFlowMap Destructor\n");
		};

		double GetClosestKey(double pvvalue);

		//Variable
	private:

		string mfilename;
		ifstream dataFile;
		map<double, double> m;
		string strToken;

		//Function
	private:
		int loadData();

};

int CVFlowMap::loadData()
{
	string strToken;
	double cvtrack = 0.0;
	double flowcoeff = 0.0;

	dataFile.open(mfilename.c_str());
	if(dataFile.fail())
	{
		cout <<"File Not Found" << endl;
		return(-1);
	};

	//static int index = 0;

	while(!dataFile.eof())
	{
		getline(dataFile, strToken);
		if(strToken[0] == '#' || strToken.empty() == true) continue;

		char str[strToken.size()];
		strcpy (str, strToken.c_str());

		char *pch = 0;
		if(!(pch = strtok (str," \t,"))) continue;

		cvtrack = strtod(pch, NULL);
		while (pch != NULL)
		{
			if(!(pch = strtok (NULL," \t,"))) continue;
			//printf("index(%d),cvtrack(%f), Kv(%f)\n", index++, cvtrack, flowcoeff);
			cout << endl;
			flowcoeff = strtod(pch, NULL);
		};
		m[cvtrack]=flowcoeff;
		//m.insert(std::pair<double,double>(cvtrack, flowcoeff));
	}

	return(0);
}

double CVFlowMap::GetClosestKey(double pv_value)
{
	std::map<double, double>::iterator low = m.lower_bound(pv_value);
	return (low->second);
}

static CVFlowMap flowcoeffmap("./cv_kvMap");

static long FlowCoeffMapInit(subRecord *psubrec)
{
	return 0;
}

static long FlowCoeffMapProc(subRecord *psubrec)
{
	if(psubrec == NULL) return -1;
	double dval = psubrec->a;

	if(dval > psubrec->hopr)
		dval = psubrec->hopr;
	else if(dval < psubrec->lopr) 
		dval = psubrec->lopr;

	psubrec->val = flowcoeffmap.GetClosestKey(dval);
	return 0;
}


epicsRegisterFunction(FlowCoeffMapInit);
epicsRegisterFunction(FlowCoeffMapProc);

