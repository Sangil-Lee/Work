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
#include "timingRegMap.h"

#define P_TSMODEString					"TIMING_MODE"
#define P_RunString						"SCOPE_RUN"	
#define P_UpdateTimeString				"SCOPE_UPDATE_TIME"
#define P_FirmwareString				"SCOPE_FIRMWARE"
#define P_SoftwareString				"SCOPE_SOFTWARE"
#define P_BuildTimeString				"BUILD_TIME"
#define P_BuildTimeRBVString			"BUILD_TIMERBV"
#define P_StartTimeString				"STARTTIME"	
#define P_IOCStartTimeString			"IOCSTARTTIME"

#define P_EVS_TIMEString				"EVS_TIME"
#define P_EVS_DAYString					"EVS_DAY"
#define P_EVS_SETString					"EVS_SET"

#define P_FRONT_A_String				"FAN_FRONT_A"
#define P_FRONT_B_String				"FAN_FRONT_B"

#define P_REAR_A_String				  	"FAN_REAR_A"
#define P_REAR_B_String				    "FAN_REAR_B"

#define P_MUX_Counter_A_String			"MULTIPLEXED_COUNTER_A"
#define P_MUX_Counter_B_String			"MULTIPLEXED_COUNTER_B"
#define P_MUX_Counter_C_String			"MULTIPLEXED_COUNTER_C"
#define P_MUX_Counter_D_String			"MULTIPLEXED_COUNTER_D"
#define P_MUX_Counter_E_String			"MULTIPLEXED_COUNTER_E"
#define P_MUX_Counter_F_String			"MULTIPLEXED_COUNTER_F"
#define P_MUX_Counter_G_String			"MULTIPLEXED_COUNTER_G"
#define P_MUX_Counter_H_String			"MULTIPLEXED_COUNTER_H"
#define P_MUX_Counter_I_String			"MULTIPLEXED_COUNTER_I"
#define P_MUX_Counter_J_String			"MULTIPLEXED_COUNTER_J"
#define P_MUX_Counter_K_String			"MULTIPLEXED_COUNTER_K"
#define P_MUX_Counter_L_String			"MULTIPLEXED_COUNTER_L"
#define P_MUX_Counter_M_String			"MULTIPLEXED_COUNTER_M"
#define P_MUX_Counter_N_String			"MULTIPLEXED_COUNTER_N"
#define P_SEQ_USER_Tri_String			"SEQUENCE_USER_TRIGGER"
#define P_PULSE_Gen_String				"PULSE_GEN_UPDATE"

#define P_PG_Width_1_String				"EVR_PG_WIDTH_1"
#define P_PG_Width_2_String				"EVR_PG_WIDTH_2"
#define P_PG_Width_3_String				"EVR_PG_WIDTH_3"
#define P_PG_Width_4_String				"EVR_PG_WIDTH_4"
#define P_PG_Width_5_String				"EVR_PG_WIDTH_5"
#define P_PG_Width_6_String				"EVR_PG_WIDTH_6"
#define P_PG_Width_7_String				"EVR_PG_WIDTH_7"
#define P_PG_Width_8_String				"EVR_PG_WIDTH_8"
#define P_PG_Width_9_String				"EVR_PG_WIDTH_9"
#define P_PG_Width_10_String			"EVR_PG_WIDTH_10"
#define P_PG_Width_11_String			"EVR_PG_WIDTH_11"
#define P_PG_Width_12_String			"EVR_PG_WIDTH_12"
#define P_PG_Width_13_String			"EVR_PG_WIDTH_13"
#define P_PG_Width_14_String			"EVR_PG_WIDTH_14"
#define P_PG_Width_15_String			"EVR_PG_WIDTH_15"
#define P_PG_Width_16_String			"EVR_PG_WIDTH_16"
#define P_PG_Width_17_String			"EVR_PG_WIDTH_17"
#define P_PG_Width_18_String			"EVR_PG_WIDTH_18"
#define P_PG_Width_19_String			"EVR_PG_WIDTH_19"
#define P_PG_Width_20_String			"EVR_PG_WIDTH_20"
#define P_PG_Width_21_String			"EVR_PG_WIDTH_21"
#define P_PG_Width_22_String			"EVR_PG_WIDTH_22"
#define P_PG_Width_23_String			"EVR_PG_WIDTH_23"
#define P_PG_Width_24_String			"EVR_PG_WIDTH_24"
#define P_PG_Width_25_String			"EVR_PG_WIDTH_25"
#define P_PG_Width_26_String			"EVR_PG_WIDTH_26"
#define P_PG_Width_27_String			"EVR_PG_WIDTH_27"
#define P_PG_Width_28_String			"EVR_PG_WIDTH_28"
#define P_PG_Width_29_String			"EVR_PG_WIDTH_29"
#define P_PG_Width_30_String			"EVR_PG_WIDTH_30"
#define P_PG_Width_31_String			"EVR_PG_WIDTH_31"
#define P_PG_Width_32_String			"EVR_PG_WIDTH_32"

#define P_PG_Delay_1_String				"EVR_PG_DELAY_1"
#define P_PG_Delay_2_String				"EVR_PG_DELAY_2"
#define P_PG_Delay_3_String				"EVR_PG_DELAY_3"
#define P_PG_Delay_4_String				"EVR_PG_DELAY_4"
#define P_PG_Delay_5_String				"EVR_PG_DELAY_5"
#define P_PG_Delay_6_String				"EVR_PG_DELAY_6"
#define P_PG_Delay_7_String				"EVR_PG_DELAY_7"
#define P_PG_Delay_8_String				"EVR_PG_DELAY_8"
#define P_PG_Delay_9_String				"EVR_PG_DELAY_9"
#define P_PG_Delay_10_String			"EVR_PG_DELAY_10"
#define P_PG_Delay_11_String			"EVR_PG_DELAY_11"
#define P_PG_Delay_12_String			"EVR_PG_DELAY_12"
#define P_PG_Delay_13_String			"EVR_PG_DELAY_13"
#define P_PG_Delay_14_String			"EVR_PG_DELAY_14"
#define P_PG_Delay_15_String			"EVR_PG_DELAY_15"
#define P_PG_Delay_16_String			"EVR_PG_DELAY_16"
#define P_PG_Delay_17_String			"EVR_PG_DELAY_17"
#define P_PG_Delay_18_String			"EVR_PG_DELAY_18"
#define P_PG_Delay_19_String			"EVR_PG_DELAY_19"
#define P_PG_Delay_20_String			"EVR_PG_DELAY_20"
#define P_PG_Delay_21_String			"EVR_PG_DELAY_21"
#define P_PG_Delay_22_String			"EVR_PG_DELAY_22"
#define P_PG_Delay_23_String			"EVR_PG_DELAY_23"
#define P_PG_Delay_24_String			"EVR_PG_DELAY_24"
#define P_PG_Delay_25_String			"EVR_PG_DELAY_25"
#define P_PG_Delay_26_String			"EVR_PG_DELAY_26"
#define P_PG_Delay_27_String			"EVR_PG_DELAY_27"
#define P_PG_Delay_28_String			"EVR_PG_DELAY_28"
#define P_PG_Delay_29_String			"EVR_PG_DELAY_29"
#define P_PG_Delay_30_String			"EVR_PG_DELAY_30"
#define P_PG_Delay_31_String			"EVR_PG_DELAY_31"
#define P_PG_Delay_32_String			"EVR_PG_DELAY_32"

#define P_PG_POL_1_String				"EVR_PG_POL_1"
#define P_PG_POL_2_String				"EVR_PG_POL_2"
#define P_PG_POL_3_String				"EVR_PG_POL_3"
#define P_PG_POL_4_String				"EVR_PG_POL_4"
#define P_PG_POL_5_String				"EVR_PG_POL_5"
#define P_PG_POL_6_String				"EVR_PG_POL_6"
#define P_PG_POL_7_String				"EVR_PG_POL_7"
#define P_PG_POL_8_String				"EVR_PG_POL_8"
#define P_PG_POL_9_String				"EVR_PG_POL_9"
#define P_PG_POL_10_String				"EVR_PG_POL_10"
#define P_PG_POL_11_String				"EVR_PG_POL_11"
#define P_PG_POL_12_String				"EVR_PG_POL_12"
#define P_PG_POL_13_String				"EVR_PG_POL_13"
#define P_PG_POL_14_String				"EVR_PG_POL_14"
#define P_PG_POL_15_String				"EVR_PG_POL_15"
#define P_PG_POL_16_String				"EVR_PG_POL_16"
#define P_PG_POL_17_String				"EVR_PG_POL_17"
#define P_PG_POL_18_String				"EVR_PG_POL_18"
#define P_PG_POL_19_String				"EVR_PG_POL_19"
#define P_PG_POL_20_String				"EVR_PG_POL_20"
#define P_PG_POL_21_String				"EVR_PG_POL_21"
#define P_PG_POL_22_String				"EVR_PG_POL_22"
#define P_PG_POL_23_String				"EVR_PG_POL_23"
#define P_PG_POL_24_String				"EVR_PG_POL_24"
#define P_PG_POL_25_String				"EVR_PG_POL_25"
#define P_PG_POL_26_String				"EVR_PG_POL_26"
#define P_PG_POL_27_String				"EVR_PG_POL_27"
#define P_PG_POL_28_String				"EVR_PG_POL_28"
#define P_PG_POL_29_String				"EVR_PG_POL_29"
#define P_PG_POL_30_String				"EVR_PG_POL_30"
#define P_PG_POL_31_String				"EVR_PG_POL_31"
#define P_PG_POL_32_String				"EVR_PG_POL_32"

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
	// User parameters
	int P_TSMode;
#define FIRST_SCOPE_COMMAND P_TSMode
	int P_Run;
	int P_UserAddressSelect;
	int P_ReadLatencyUs;
	int P_ChannelEn;
	int P_Firmware;
	int P_Software;
	int P_UpdateTime;
	int P_BuildTime;
	int P_BuildTimeRBV;
	int P_Starttime;
	int P_IOCStartTime;
	int P_EVS_Time;
	int P_EVS_Day;
	int P_EVS_Set;
	int P_Fan_Front_A;
	int P_Fan_Front_B;
	int P_Fan_Rear_A;
	int P_Fan_Rear_B;
	int P_MUX_Counter_A;
	int P_MUX_Counter_B;
	int P_MUX_Counter_C;
	int P_MUX_Counter_D;
	int P_MUX_Counter_E;
	int P_MUX_Counter_F;
	int P_MUX_Counter_G;
	int P_MUX_Counter_H;
	int P_MUX_Counter_I;
	int P_MUX_Counter_J;
	int P_MUX_Counter_K;
	int P_MUX_Counter_L;
	int P_MUX_Counter_M;
	int P_MUX_Counter_N;
	int P_SEQ_User_Trigger;
	int P_PG_Update;
	int P_PG_Width_1;
	int P_PG_Width_2;
	int P_PG_Width_3;
	int P_PG_Width_4;
	int P_PG_Width_5;
	int P_PG_Width_6;
	int P_PG_Width_7;
	int P_PG_Width_8;
	int P_PG_Width_9;
	int P_PG_Width_10;
	int P_PG_Width_11;
	int P_PG_Width_12;
	int P_PG_Width_13;
	int P_PG_Width_14;
	int P_PG_Width_15;
	int P_PG_Width_16;
	int P_PG_Width_17;
	int P_PG_Width_18;
	int P_PG_Width_19;
	int P_PG_Width_20;
	int P_PG_Width_21;
	int P_PG_Width_22;
	int P_PG_Width_23;
	int P_PG_Width_24;
	int P_PG_Width_25;
	int P_PG_Width_26;
	int P_PG_Width_27;
	int P_PG_Width_28;
	int P_PG_Width_29;
	int P_PG_Width_30;
	int P_PG_Width_31;
	int P_PG_Width_32;
	int P_PG_Delay_1;
	int P_PG_Delay_2;
	int P_PG_Delay_3;
	int P_PG_Delay_4;
	int P_PG_Delay_5;
	int P_PG_Delay_6;
	int P_PG_Delay_7;
	int P_PG_Delay_8;
	int P_PG_Delay_9;
	int P_PG_Delay_10;
	int P_PG_Delay_11;
	int P_PG_Delay_12;
	int P_PG_Delay_13;
	int P_PG_Delay_14;
	int P_PG_Delay_15;
	int P_PG_Delay_16;
	int P_PG_Delay_17;
	int P_PG_Delay_18;
	int P_PG_Delay_19;
	int P_PG_Delay_20;
	int P_PG_Delay_21;
	int P_PG_Delay_22;
	int P_PG_Delay_23;
	int P_PG_Delay_24;
	int P_PG_Delay_25;
	int P_PG_Delay_26;
	int P_PG_Delay_27;
	int P_PG_Delay_28;
	int P_PG_Delay_29;
	int P_PG_Delay_30;
	int P_PG_Delay_31;
	int P_PG_Delay_32;
	int P_PG_POL_1;
	int P_PG_POL_2;
	int P_PG_POL_3;
	int P_PG_POL_4;
	int P_PG_POL_5;
	int P_PG_POL_6;
	int P_PG_POL_7;
	int P_PG_POL_8;
	int P_PG_POL_9;
	int P_PG_POL_10;
	int P_PG_POL_11;
	int P_PG_POL_12;
	int P_PG_POL_13;
	int P_PG_POL_14;
	int P_PG_POL_15;
	int P_PG_POL_16;
	int P_PG_POL_17;
	int P_PG_POL_18;
	int P_PG_POL_19;
	int P_PG_POL_20;
	int P_PG_POL_21;
	int P_PG_POL_22;
	int P_PG_POL_23;
	int P_PG_POL_24;
	int P_PG_POL_25;
	int P_PG_POL_26;
	int P_PG_POL_27;
	int P_PG_POL_28;
	int P_PG_POL_29;
	int P_PG_POL_30;
	int P_PG_POL_31;
	int P_PG_POL_32;
	int P_LAST;
#define LAST_SCOPE_COMMAND P_LAST

private:
	string fileName;
	const char *driverName;
    epicsEventId eventId_;
    int scanRateHz;
    int system_init_ok;
    int tsmode;
	struct TimingRegmap *pregmap;

	struct regMap{
		char alias[64];
		int	 address;
	};
	regMap	regmap;

	//GCC Version < 4.3, __gnu_cxx::hash_map
	//__gnu_cxx::hash_map<string, regMap> regmapfile;
	//__gnu_cxx::hash_map<int, regMap> regmaptable;

	//GCC Version > 4.3, unordered_map
	hash_map<string, regMap> regmapfile;
	hash_map<int, regMap> regmaptable;

	int tsMode(const char* mode);
	void setTime();
	void setBuildTime();
	void registerParamListFromFile(string filename);
};

#define NUM_SCOPE_PARAMS (&LAST_SCOPE_COMMAND - &FIRST_SCOPE_COMMAND + 1)

#endif
