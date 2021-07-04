#ifndef		__ASYN_DRIVER_SUPPORT_H
#define		__ASYN_DRIVER_SUPPORT_H

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
#include <vector>

#include "asynPortDriver.h"

extern "C" {
}

using namespace std;

class asynDriverSupport : public asynPortDriver 
{
public:
	asynDriverSupport(const char *portName,const char* filename);

protected:
	/** Values used for pasynUser->reason, and indexes into the parameter library. */
	virtual asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);
    virtual asynStatus writeInt32Array(asynUser *pasynUser, epicsInt32 *value, size_t nElements);
    virtual asynStatus readInt32Array(asynUser *pasynUser, epicsInt32 *value, size_t nElements, size_t *nIn);
    virtual asynStatus readInt8Array(asynUser *pasynUser, epicsInt8 *value, size_t nElements, size_t *nIn);

private:
	const char *driverName;
    epicsEventId eventId_;

	bitset<40>	polbit;

	struct RegMap{
		char			drvname[64];
		int				address;
		asynParamType	paramtype;
		int				index;
	};

	//GCC Version > 4.3, unordered_map
	hash_map<string, RegMap> regmapfile;
	hash_map<int,	 RegMap> regmaptable;
	hash_map<string, RegMap>::const_iterator check_iter;

	typedef	vector<unsigned long> vecCode;
	hash_map<string, vecCode> vecCodeMap;

	//member methods/functions
	void registerParamListFromFile(string filename);
	void setIOCStartTime();
	asynParamType getAsynParamType(const char *paramstring);
	asynStatus createParamNMap(RegMap &reg);
	int setParamValue(const string drvname, const int ival);
	int setParamValue(const string drvname, const double dval);
	int setParamValue(const string drvname, const string svalue);
	asynStatus getParamValue(const string drvname, int maxChars, char *value );
	asynStatus getParamValue(const string drvname, int &value);
	asynStatus getParamValue(const string drvname, double &value);
	int checkParam(const string drvname);
	int writeValue(const RegMap & regmap);

	asynStatus  readInt32(asynUser *pasynUser, epicsInt32 *value);
	asynStatus	readValue(const RegMap &regmap, epicsInt32 &value);
	asynStatus	readValue(const RegMap &regmap, epicsFloat64 &value);
	asynStatus	readValue(const RegMap &regmap, char *value);
};

#endif
