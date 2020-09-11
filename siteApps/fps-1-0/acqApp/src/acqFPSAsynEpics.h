#ifndef		_ACQ_FPS_ASYN_EPICS_H
#define		_ACQ_FPS_ASYN_EPICS_H

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

// #include <iostream>
// #include <fstream>

#include "intech_global.h"

// #include <cstdlib>
// #include <cstring>
// #include <bitset>
// #include <iocsh.h>
// #include <bitset>

// #include <epicsTypes.h>
// #include <epicsTime.h>
// #include <epicsThread.h>
// #include <epicsString.h>
// #include <epicsTimer.h>
// #include <epicsMutex.h>
// #include <epicsEvent.h>
// #include <registryFunction.h>
// #include <epicsExport.h>
// #include <dbCommon.h>

#include "asynPortDriver.h"

#include "acqFPSDriver.h"
// #include "intech_define.h"

extern "C" {
}


void asynProcess(void *drvPvt);
// void asynProcess2(void *drvPvt);

using namespace acq_fps;

class acqFPSAsynEpics : public asynPortDriver 
{
	public:
		acqFPSAsynEpics(const char *portName,const char* filename);
		virtual asynStatus readInt32(asynUser *pasynUser, epicsInt32 *value);
		virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
		virtual asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);

		void asynProcess(void);
	protected:
		/** Values used for pasynUser->reason, and indexes into the parameter library. */
		//virtual asynStatus readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason);
		/* 2018.10.16 thread error debug */
		// int P_Run;
		// int P_UpdateTime;

		/* 2018.11.13 */
		// int P_DRY_Waveform;
		// int P_WET_Waveform;

	private:
		/* 2018.11.13 */
		// epicsInt32 *DRY_pData_;
		// epicsInt32 *WET_pData_;	

		int Board_Name_env;
		PMS_Data_t Store_Data[50][10000]; // 16Byte * 100 * 10000 => 1sec PMS Data
		PMS_Time_t Interlock_Time_Data = {0,};
		char pmsdata_filename[60] = {0,};
		int pms_file_exist_status = 0;	// 1 : exist , 0 : not exist	PMS 파일 존재 유무

	private:
		const char *driverName;
		epicsEventId eventId_;
		acqFPSDriver *pacqFPSDriver;
		// bitset<40>	polbit;
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

		int devnode;
		string filename;


	private:
		//member methods/functions
		void registerParamListFromFile(string filename);
		void setIOCStartTime();
		asynParamType getAsynParamType(const char *paramstring);
		asynStatus createParamNMap(RegMap &reg);

		int setParamValue(const string drvname, const int ival);
		int setParamValue(const string drvname, const double dval);
		int setParamValue(const string drvname, const string svalue);
		int setParamValue(const string drvname, const epicsUInt32 value, epicsUInt32 mask);

		/* thread */
		asynStatus getParamValue(const string drvname, int maxChars, char *value );
		asynStatus getParamValue(const string drvname, int &value);
		asynStatus getParamValue(const string drvname, double &value);

		int checkParam(const string drvname);
		epicsInt32 readInt32Value(const RegMap &regmap);
		asynStatus readStringValue(const RegMap &regmap, char *value);

		int writeInt32Value(const RegMap &regmap);
		asynStatus writeUInt32Digital(asynUser *pasynUser, epicsUInt32 value, epicsUInt32 mask);
		asynStatus readUInt32Digital(asynUser *pasynUser, epicsUInt32 *value, epicsUInt32 mask);

		int StartFPS();
		void EventPMS();
		int Create_Log(char *FileName);
		
		void Get_PMS_FileName(char *str, int size = FILE_MAX_SIZE);
		void Get_NodeName(char *str);
		void CheckFile();
		void Get_Absolute_Path(char *filename, int size = FILE_MAX_SIZE);

		void file_wr(unsigned int Line_Num, PMS_Data_t acqNode, int *fd, int interlockIdx);
};


#endif
