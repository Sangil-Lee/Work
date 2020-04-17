#ifndef  IMITECH_ADDRIVER_H
#define  IMITECH_ADDRIVER_H

#define GCC_VERSION (__GNUC__ * 10000 \
    + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

#if GCC_VERSION >= 40300
#include <tr1/unordered_map>
#define hash_map std::tr1::unordered_map
#else
#include <ext/hash_map>
#define hash_map __gnu_cxx::hash_map
namespace __gnu_cxx {
	template <>
		struct hash<std::string> {
			size_t operator() (const std::string& x) const {
				return hash<const char*>()(x.c_str());
			}
		};
};
#endif

#include <stddef.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include <ellLib.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <cantProceed.h>
#include <osiSock.h>
#include <iocsh.h>
#include <epicsExit.h>
#include <epicsExport.h>

#include "paramVal.h"
#include "asynParamType.h"
#include "ParamListInvalidIndex.h"
#include "ParamValWrongType.h"
#include "ParamValNotDefined.h"
#include "asynPortDriver.h"

#include "ADDriver.h"
#include "NeptuneAPI.h"

extern "C" {
}

using namespace std;

class ADIMITechDriver : public ADDriver
{
public:
	ADIMITechDriver(const char *portName, const char *cameraId, int maxBuffers, size_t maxMemory,
                     int priority, int stackSize, int maxPvAPIFrames);

	~ADIMITechDriver();

	void UserImageGrabTask();

private:
	struct RegMap{
		char			drvname[64];
		asynParamType	paramtype;
		char			drvlink[64];
		int				index;
	};

	hash_map<string, RegMap> regmapfile;
	hash_map<int,	 RegMap> regmaptable;
	hash_map<string, RegMap>::const_iterator check_iter;

	typedef	vector<unsigned long> vecCode;
	hash_map<string, vecCode> vecCodeMap;

	char *mCamID;


	//Member Functions.
	void registerParamListFromFile(string &filename);
	asynParamType getAsynParamType(const char *paramstring);
	asynStatus createParamNMap(RegMap &reg);

};

#endif
