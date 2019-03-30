#ifndef		_DAQ_CONTROL_ASYN_EPICS_H
#define		_DAQ_CONTROL_ASYN_EPICS_H

#define GCC_VERSION (__GNUC__ * 10000 \
    + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)

// Hash_map Data Structure.
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
#include <bitset>

#include "asynPortDriver.h"
#include "daqControlDriver.h"

extern "C" {
}

class daqControlAsynEpics : public asynPortDriver 
{
public:
	daqControlAsynEpics(const char *portName, const char* filename, const int slotNum);
	void userProcess();

protected:
	/** Values used for pasynUser->reason, and indexes into the parameter library. */

private:
	string fileName;
	const char *driverName;
    epicsEventId eventId_;
    const bool sw_event;
    int tsmode;

	typedef bitset<32> OptionBit;
	OptionBit optbit;

	daqControl::daqControlDriver *pdaqControl;

	struct RegMap{
		char			drvname[64];	//DB ALIAS to index 
		int				address;		//HW Address
		asynParamType	paramtype;		//Paramter Data type
		unsigned long	option;			//BCD option
		char			drvlink[64];	//Reference drvname, hexa decimal string in regiter map file.
		int				index;			//createParam generate index automatically in order.
	};

	enum NUM_OPTION { OPTION0 = 0, OPTION1, OPTION2, OPTION3, OPTION4, OPTION5, OPTION6, OPTION7,
					OPTION8, OPTION9, OPTION10, OPTION11, OPTION12, OPTION13, OPTION14, OPTION15,
					OPTION16, OPTION17, OPTION18, OPTION19, OPTION20, OPTION21, OPTION22, OPTION23,
					OPTION24, OPTION25, OPTION26, OPTION27, OPTION28, OPTION29, OPTION30, OPTION31 };

	int		evfile;
	//GCC Version > 4.3, unordered_map
	hash_map<string, RegMap> regmapfile;
	hash_map<int,	 RegMap> regmaptable;
	hash_map<string, RegMap>::const_iterator check_iter;

	typedef	vector<unsigned long> vecCode;
	hash_map<string, vecCode> vecCodeMap;

	int tsMode(const char* mode);
	void registerParamListFromFile(string filename);
	asynParamType getAsynParamType(const char *paramstring);
	asynStatus createParamNMap(RegMap &reg);
	int checkParam(const string drvname);

	int setParamValue(const string drvname, const string svalue);
	int setParamValue(const string drvname, epicsInt32 value);
	int setParamValue(const string drvname, epicsFloat64 value);
	int setParamValue(const string drvname, epicsUInt32 value, epicsUInt32 mask);

	asynStatus getParamValue(const string drvname, int maxChars, char *value );
	asynStatus getParamValue(const string drvname, epicsInt32 &value);
	asynStatus getParamValue(const string drvname, epicsFloat64 &value);
	asynStatus getParamValue(const string drvname, epicsUInt32 &value, epicsUInt32 mask);


	asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
	asynStatus writeValue(const RegMap &regmap, const epicsInt32 value);
	int writeInt32Value(const RegMap &regmap, epicsInt32 value);
	int writeUInt32Value(const RegMap &regmap, epicsUInt32 value, epicsUInt32 mask);

	asynStatus writeFloat64(asynUser *pasynUser, epicsFloat64 value);
	int writeFloat64Value(const RegMap &regmap, epicsFloat64 value);

	asynStatus writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual);
	asynStatus writeOctetValue(const RegMap &regmap, const char *value);
	asynStatus writeStringValue(const RegMap &regmap, const char *value);

	asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);
	asynStatus readStringValue(const RegMap &regmap, char *value);
	asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);

	epicsInt32 readInt32Value(const RegMap &regmap);

	//New 
	asynStatus	readValue(const RegMap &regmap, epicsInt32 &value);
	asynStatus	readValue(const RegMap &regmap, epicsFloat64 &value);

	//WaveformPV Setup
	asynStatus writeFloat64Array(asynUser *pasynUser, epicsFloat64 *value, size_t nElements);


	//BO Record
    asynStatus writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask);

	void getOptionBitset(OptionBit &optionbit, epicsUInt32 option, epicsUInt32 shift = 0, epicsUInt32 mask = 0);
	//Instead of bitset
	epicsUInt32 getRShiftMask(epicsUInt32 option, epicsUInt32 shift, epicsUInt32 mask);
	epicsUInt32 getLShiftMask(epicsUInt32 option, epicsUInt32 shift, epicsUInt32 mask);
	epicsUInt32 setBitOnOff(epicsUInt32 option, epicsUInt32 nbit, bool bit);
	epicsUInt32 setExclusiveBit(epicsUInt32 option, epicsUInt32 nbit);
	bool checkBit(epicsUInt32 option, epicsUInt32 nbit);

};

#endif
