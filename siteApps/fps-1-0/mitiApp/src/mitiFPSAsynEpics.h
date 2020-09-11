#ifndef		_MITI_FPS_ASYN_EPICS_H
#define		_MITI_FPS_ASYN_EPICS_H

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

#include "intech_global.h"

extern "C" {
}

void asynProcess(void *drvPvt);

using namespace miti_fps;

class mitiFPSAsynEpics : public asynPortDriver 
{
	public:
		mitiFPSAsynEpics(const char *portName,const char* filename);
		void asynProcess(void);
		virtual asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);
		virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
		virtual asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);

	private:
		const char *driverName;
		epicsEventId eventId_;
		mitiFPSDriver *pmitiFPSDriver;

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

		int devnode;
		string filename;

		int Board_Name_env = 0;
		unsigned int OLD_TEMP = 0;
	
	/* member methods/functions */
	private:
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

		int readInt32Value(const RegMap &regmap);
		int writeInt32Value(const RegMap &regmap);
		
		// asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);
		int   readValue(const RegMap &regmap, epicsInt32 &value);
		int   readValue(const RegMap &regmap, epicsFloat64 &value);
		int   readValue(const RegMap &regmap, char *value);

};

#endif
