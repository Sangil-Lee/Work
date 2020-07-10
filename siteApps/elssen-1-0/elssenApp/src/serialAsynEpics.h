#ifndef		_SERIAL_ASYN_EPICS_H
#define		_SERIAL_ASYN_EPICS_H

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
#include <tuple>
#include <memory>

//Windows using Qt
#if defined(WINDOWS_QT) && (WINDOWS_QT==1) 
#include <QSerialPort>
#else
//Linux
#include "SerialPort.h"
using namespace LibSerial;
#endif

#include "paramVal.h"
#include "asynParamType.h"
#include "ParamListInvalidIndex.h"
#include "ParamValWrongType.h"
#include "ParamValNotDefined.h"
#include "asynPortDriver.h"


using namespace std;

extern "C" {
}

class SerailAsynEpics : public asynPortDriver 
{
public:
	SerailAsynEpics(const char *portName, const char* filename, const char *serialPort);
	~SerailAsynEpics();

	void UserProcess();

protected:
	/** Values used for pasynUser->reason, and indexes into the parameter library. */

private:
	string fileName;
	const char *driverName;
    epicsEventId eventId_;
    int system_init_ok;
	string mSerialPort;

	//unsigned long bit test
	typedef bitset<32> OptionBit;
	OptionBit optbit;
	enum eMAP{MAP1=1, MAP2, MAP3, MAP4};

	struct RegMap{
		char			drvname[64];
		asynParamType	paramtype;
		char			drvlink[64];
		int				index;
	};

	//GCC Version > 4.3, unordered_map
	hash_map<string, RegMap> regmapfile;
	hash_map<int,	 RegMap> regmaptable;
	hash_map<string, RegMap>::const_iterator check_iter;

	typedef	vector<unsigned long> vecCode;
	hash_map<string, vecCode> vecCodeMap;

	void registerParamListFromFile(string filename);
	asynParamType getAsynParamType(const char *paramstring);
	asynStatus createParamNMap(RegMap &reg);
	int checkParam(const string drvname);

	int setParamValue(const string drvname, const string svalue);
	int setParamValue(const string drvname, const int ival);
	int setParamValue(const string drvname, const double dval);
	asynStatus getParamValue(const string drvname, int maxChars, char *value );
	asynStatus getParamValue(const string drvname, int &value);
	asynStatus getParamValue(const string drvname, double &value);

	asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
	asynStatus writeValue(const RegMap &regmap, const epicsInt32 value);
	int writeInt32Value(const RegMap &regmap, epicsInt32 value);
	int writeInt32Value(const RegMap &regmap);

	asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
	int writeFloat64Value(const RegMap &regmap);

	asynStatus writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual);
	asynStatus writeOctetValue(const RegMap &regmap, const char *value);
	asynStatus writeStringValue(const RegMap &regmap, const char *value);

	asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);
	asynStatus readStringValue(const RegMap &regmap, char *value);
	asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);
    asynStatus readFloat64(asynUser *pasynUser, epicsFloat64 *value);

	epicsInt32 readInt32Value(const RegMap &regmap);

	//New 
	asynStatus	readValue(const RegMap &regmap, epicsInt32 &value);
	asynStatus	readValue(const RegMap &regmap, epicsFloat64 &value);

	//WaveformPV Write
	//asynStatus writeFloat64Array(asynUser *pasynUser, epicsFloat64 *value, size_t nElements);
	//
	//WaveformPV Read
	asynStatus readFloat64Array(asynUser *pasynUser, epicsFloat64 *value, size_t nElements, size_t *nIn);


	//BO Record
    asynStatus writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask);
	
#if 1
	void getOptionBitset(OptionBit &optionbit, epicsUInt32 option, epicsUInt32 shift = 0, epicsUInt32 mask = 0);

	//Instead of bitset
	epicsUInt32 getRShiftMask(epicsUInt32 option, epicsUInt32 shift, epicsUInt32 mask);
	epicsUInt32 getLShiftMask(epicsUInt32 option, epicsUInt32 shift, epicsUInt32 mask);
	epicsUInt32 setBitOnOff(epicsUInt32 option, epicsUInt32 nbit, bool bit);
	epicsUInt32 setExclusiveBit(epicsUInt32 option, epicsUInt32 nbit);
	bool checkBit(epicsUInt32 option, epicsUInt32 nbit);
#endif
};

#endif
