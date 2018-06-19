#ifndef		_TIMING_ASYN_EPICS_H
#define		_TIMING_ASYN_EPICS_H

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

#include <iostream>
#include <fstream>

#include "inc/com_def.h"
#include "ts2regmap.h"
#include "ts2ip.h"
#include "ts2iic.h"
#include "ts2api.h"

#include "asynPortDriver.h"
#include "timingDriver.h"

extern "C" {
}

using namespace std;


class timingAsynEpics : public asynPortDriver 
{
public:
	timingAsynEpics(const char *portName, int maxSizeSnapshot, int maxSizeBufferMB, int clientMode, const char* filename, const char *deviceName);
	void userProcess();
	virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
	virtual asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
	virtual asynStatus writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual);
	virtual asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);

	virtual asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);
	int clientThreadMode;

protected:
	/** Values used for pasynUser->reason, and indexes into the parameter library. */

private:
	string fileName;
	const char *driverName;
    epicsEventId eventId_;
    int scanRateHz;
    int system_init_ok;
    int tsmode;

	timing::timingDriver *ptiming;
	bitset<40>	polbit;

	struct RegMap{
		char			drvname[64];
		int				address;
		asynParamType	paramtype;
		int				index;
	};

	int		evfile;
	//GCC Version > 4.3, unordered_map
	hash_map<string, RegMap> regmapfile;
	hash_map<int,	 RegMap> regmaptable;
	hash_map<string, RegMap>::const_iterator check_iter;

	int tsMode(const char* mode);
	void setIOCStartTime();

	int setParamValue(const string drvname, const string svalue);
	int setParamValue(const string drvname, const int ival);
	int setParamValue(const string drvname, const double dval);

	asynStatus getParamValue(const string drvname, int maxChars, char *value );
	asynStatus getParamValue(const string drvname, int &value);
	asynStatus getParamValue(const string drvname, double &value);

	void registerParamListFromFile(string filename);
	asynParamType getAsynParamType(const char *paramstring);
	asynStatus createParamNMap(RegMap &reg);
	epicsInt32 readInt32Value(const RegMap &regmap);
	int writeFloat64Value(const RegMap &regmap);
	int writeInt32Value(const RegMap &regmap);
	int writeOctetValue(const RegMap &regmap);
	asynStatus readStringValue(const RegMap &regmap, char *value);
	int checkParam(const string drvname);
};

#endif
