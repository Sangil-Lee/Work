#ifndef		_TIMING_ASYN_EPICS_H
#define		_TIMING_ASYN_EPICS_H

#include <iostream>
#include <fstream>
#include <ext/hash_map>

#include "inc/com_def.h"
#include "ts2regmap.h"
#include "ts2ip.h"
#include "ts2iic.h"
#include "ts2api.h"

#include "asynPortDriver.h"
#include "timingRegMap.h"

#define P_TSMODEString							"TIMING_MODE"										/* asynString,   r/w */
#define P_RunString									"SCOPE_RUN"											/* asynInt32,    r/w */
#define P_UpdateTimeString					"SCOPE_UPDATE_TIME"							/* asynFloat64,  r/w */
#define P_FirmwareString						"SCOPE_FIRMWARE"								/* asynInt32,    r/w */
#define P_SoftwareString						"SCOPE_SOFTWARE"								/* asynFloat64,  r/w */
#define P_BuildTimeString						"BUILD_TIME"										/* asynString,   r/w */
#define P_BuildTimeRBVString				"BUILD_TIMERBV"									/* asynString,   r/w */
#define P_StartTimeString						"STARTTIME"											/* asynString,   r/w */
#define P_IOCStartTimeString				"IOCSTARTTIME"									/* asynString,   r/w */

#define P_EVS_TIMEString				"EVS_TIME"													/* asynLongout,   r/w */
#define P_EVS_DAYString				  "EVS_DAY"														/* asynLongout,   r/w */
#define P_EVS_SETString				  "EVS_SET"														/* asynLongout,   r/w */

#define P_FRONT_A_String				"FAN_FRONT_A"												/* asynInt32,   r */
#define P_FRONT_B_String				"FAN_FRONT_B"												/* asynInt32,   r */

#define P_REAR_A_String				  	"FAN_REAR_A"												/* asynInt32,   r */
#define P_REAR_B_String				    "FAN_REAR_B"												/* asynInt32,   r */

#define P_MUX_Couter_A_String			"MULTIPLEXED_COUNTER_A"							/* asynInt32,   w */
#define P_MUX_Couter_B_String			"MULTIPLEXED_COUNTER_B"							/* asynInt32,   w */
#define P_MUX_Couter_C_String			"MULTIPLEXED_COUNTER_C"							/* asynInt32,   w */
#define P_MUX_Couter_D_String			"MULTIPLEXED_COUNTER_D"							/* asynInt32,   w */
#define P_MUX_Couter_E_String			"MULTIPLEXED_COUNTER_E"							/* asynInt32,   w */
#define P_MUX_Couter_F_String			"MULTIPLEXED_COUNTER_F"							/* asynInt32,   w */
#define P_MUX_Couter_G_String			"MULTIPLEXED_COUNTER_G"							/* asynInt32,   w */
#define P_MUX_Couter_H_String			"MULTIPLEXED_COUNTER_H"							/* asynInt32,   w */
#define P_MUX_Couter_I_String			"MULTIPLEXED_COUNTER_I"							/* asynInt32,   w */
#define P_MUX_Couter_J_String			"MULTIPLEXED_COUNTER_J"							/* asynInt32,   w */
#define P_MUX_Couter_K_String			"MULTIPLEXED_COUNTER_K"							/* asynInt32,   w */
#define P_MUX_Couter_L_String			"MULTIPLEXED_COUNTER_L"							/* asynInt32,   w */
#define P_MUX_Couter_M_String			"MULTIPLEXED_COUNTER_M"							/* asynInt32,   w */
#define P_MUX_Couter_N_String			"MULTIPLEXED_COUNTER_N"							/* asynInt32,   w */
#define P_SEQ_USER_Tri_String			"SEQUENCE_USER_TRIGGER"							/* asynInt32,   w */
#define P_PULSE_Gen_String				"PULSE_GEN_UPDATE"									/* asynInt32,   w */

#define P_PG_Width_1_String				"EVR_PG_WIDTH_1"										/* asynInt32,   w */
#define P_PG_Width_2_String				"EVR_PG_WIDTH_2"										/* asynInt32,   w */
#define P_PG_Width_3_String				"EVR_PG_WIDTH_3"										/* asynInt32,   w */
#define P_PG_Width_4_String				"EVR_PG_WIDTH_4"										/* asynInt32,   w */
#define P_PG_Width_5_String				"EVR_PG_WIDTH_5"										/* asynInt32,   w */
#define P_PG_Width_6_String				"EVR_PG_WIDTH_6"										/* asynInt32,   w */
#define P_PG_Width_7_String				"EVR_PG_WIDTH_7"										/* asynInt32,   w */
#define P_PG_Width_8_String				"EVR_PG_WIDTH_8"										/* asynInt32,   w */
#define P_PG_Width_9_String				"EVR_PG_WIDTH_9"										/* asynInt32,   w */
#define P_PG_Width_10_String			"EVR_PG_WIDTH_10"										/* asynInt32,   w */
#define P_PG_Width_11_String			"EVR_PG_WIDTH_11"										/* asynInt32,   w */
#define P_PG_Width_12_String			"EVR_PG_WIDTH_12"										/* asynInt32,   w */
#define P_PG_Width_13_String			"EVR_PG_WIDTH_13"										/* asynInt32,   w */
#define P_PG_Width_14_String			"EVR_PG_WIDTH_14"										/* asynInt32,   w */
#define P_PG_Width_15_String			"EVR_PG_WIDTH_15"										/* asynInt32,   w */
#define P_PG_Width_16_String			"EVR_PG_WIDTH_16"										/* asynInt32,   w */
#define P_PG_Width_17_String			"EVR_PG_WIDTH_17"										/* asynInt32,   w */
#define P_PG_Width_18_String			"EVR_PG_WIDTH_18"										/* asynInt32,   w */
#define P_PG_Width_19_String			"EVR_PG_WIDTH_19"										/* asynInt32,   w */
#define P_PG_Width_20_String			"EVR_PG_WIDTH_20"										/* asynInt32,   w */
#define P_PG_Width_21_String			"EVR_PG_WIDTH_21"										/* asynInt32,   w */
#define P_PG_Width_22_String			"EVR_PG_WIDTH_22"										/* asynInt32,   w */
#define P_PG_Width_23_String			"EVR_PG_WIDTH_23"										/* asynInt32,   w */
#define P_PG_Width_24_String			"EVR_PG_WIDTH_24"										/* asynInt32,   w */
#define P_PG_Width_25_String			"EVR_PG_WIDTH_25"										/* asynInt32,   w */
#define P_PG_Width_26_String			"EVR_PG_WIDTH_26"										/* asynInt32,   w */
#define P_PG_Width_27_String			"EVR_PG_WIDTH_27"										/* asynInt32,   w */
#define P_PG_Width_28_String			"EVR_PG_WIDTH_28"										/* asynInt32,   w */
#define P_PG_Width_29_String			"EVR_PG_WIDTH_29"										/* asynInt32,   w */
#define P_PG_Width_30_String			"EVR_PG_WIDTH_30"										/* asynInt32,   w */
#define P_PG_Width_31_String			"EVR_PG_WIDTH_31"										/* asynInt32,   w */
#define P_PG_Width_32_String			"EVR_PG_WIDTH_32"										/* asynInt32,   w */

#define P_PG_Delay_1_String				"EVR_PG_DELAY_1"										/* asynInt32,   w */
#define P_PG_Delay_2_String				"EVR_PG_DELAY_2"										/* asynInt32,   w */
#define P_PG_Delay_3_String				"EVR_PG_DELAY_3"										/* asynInt32,   w */
#define P_PG_Delay_4_String				"EVR_PG_DELAY_4"										/* asynInt32,   w */
#define P_PG_Delay_5_String				"EVR_PG_DELAY_5"										/* asynInt32,   w */
#define P_PG_Delay_6_String				"EVR_PG_DELAY_6"										/* asynInt32,   w */
#define P_PG_Delay_7_String				"EVR_PG_DELAY_7"										/* asynInt32,   w */
#define P_PG_Delay_8_String				"EVR_PG_DELAY_8"										/* asynInt32,   w */
#define P_PG_Delay_9_String				"EVR_PG_DELAY_9"										/* asynInt32,   w */
#define P_PG_Delay_10_String			"EVR_PG_DELAY_10"										/* asynInt32,   w */
#define P_PG_Delay_11_String			"EVR_PG_DELAY_11"										/* asynInt32,   w */
#define P_PG_Delay_12_String			"EVR_PG_DELAY_12"										/* asynInt32,   w */
#define P_PG_Delay_13_String			"EVR_PG_DELAY_13"										/* asynInt32,   w */
#define P_PG_Delay_14_String			"EVR_PG_DELAY_14"										/* asynInt32,   w */
#define P_PG_Delay_15_String			"EVR_PG_DELAY_15"										/* asynInt32,   w */
#define P_PG_Delay_16_String			"EVR_PG_DELAY_16"										/* asynInt32,   w */
#define P_PG_Delay_17_String			"EVR_PG_DELAY_17"										/* asynInt32,   w */
#define P_PG_Delay_18_String			"EVR_PG_DELAY_18"										/* asynInt32,   w */
#define P_PG_Delay_19_String			"EVR_PG_DELAY_19"										/* asynInt32,   w */
#define P_PG_Delay_20_String			"EVR_PG_DELAY_20"										/* asynInt32,   w */
#define P_PG_Delay_21_String			"EVR_PG_DELAY_21"										/* asynInt32,   w */
#define P_PG_Delay_22_String			"EVR_PG_DELAY_22"										/* asynInt32,   w */
#define P_PG_Delay_23_String			"EVR_PG_DELAY_23"										/* asynInt32,   w */
#define P_PG_Delay_24_String			"EVR_PG_DELAY_24"										/* asynInt32,   w */
#define P_PG_Delay_25_String			"EVR_PG_DELAY_25"										/* asynInt32,   w */
#define P_PG_Delay_26_String			"EVR_PG_DELAY_26"										/* asynInt32,   w */
#define P_PG_Delay_27_String			"EVR_PG_DELAY_27"										/* asynInt32,   w */
#define P_PG_Delay_28_String			"EVR_PG_DELAY_28"										/* asynInt32,   w */
#define P_PG_Delay_29_String			"EVR_PG_DELAY_29"										/* asynInt32,   w */
#define P_PG_Delay_30_String			"EVR_PG_DELAY_30"										/* asynInt32,   w */
#define P_PG_Delay_31_String			"EVR_PG_DELAY_31"										/* asynInt32,   w */
#define P_PG_Delay_32_String			"EVR_PG_DELAY_32"										/* asynInt32,   w */

#define P_PG_POL_1_String					"EVR_PG_POL_1"										/* asynInt32,   w */
#define P_PG_POL_2_String					"EVR_PG_POL_2"										/* asynInt32,   w */
#define P_PG_POL_3_String					"EVR_PG_POL_3"										/* asynInt32,   w */
#define P_PG_POL_4_String					"EVR_PG_POL_4"										/* asynInt32,   w */
#define P_PG_POL_5_String					"EVR_PG_POL_5"										/* asynInt32,   w */
#define P_PG_POL_6_String					"EVR_PG_POL_6"										/* asynInt32,   w */
#define P_PG_POL_7_String					"EVR_PG_POL_7"										/* asynInt32,   w */
#define P_PG_POL_8_String					"EVR_PG_POL_8"										/* asynInt32,   w */
#define P_PG_POL_9_String					"EVR_PG_POL_9"										/* asynInt32,   w */
#define P_PG_POL_10_String			"EVR_PG_POL_10"										/* asynInt32,   w */
#define P_PG_POL_11_String			"EVR_PG_POL_11"										/* asynInt32,   w */
#define P_PG_POL_12_String			"EVR_PG_POL_12"										/* asynInt32,   w */
#define P_PG_POL_13_String			"EVR_PG_POL_13"										/* asynInt32,   w */
#define P_PG_POL_14_String			"EVR_PG_POL_14"										/* asynInt32,   w */
#define P_PG_POL_15_String			"EVR_PG_POL_15"										/* asynInt32,   w */
#define P_PG_POL_16_String			"EVR_PG_POL_16"										/* asynInt32,   w */
#define P_PG_POL_17_String			"EVR_PG_POL_17"										/* asynInt32,   w */
#define P_PG_POL_18_String			"EVR_PG_POL_18"										/* asynInt32,   w */
#define P_PG_POL_19_String			"EVR_PG_POL_19"										/* asynInt32,   w */
#define P_PG_POL_20_String			"EVR_PG_POL_20"										/* asynInt32,   w */
#define P_PG_POL_21_String			"EVR_PG_POL_21"										/* asynInt32,   w */
#define P_PG_POL_22_String			"EVR_PG_POL_22"										/* asynInt32,   w */
#define P_PG_POL_23_String			"EVR_PG_POL_23"										/* asynInt32,   w */
#define P_PG_POL_24_String			"EVR_PG_POL_24"										/* asynInt32,   w */
#define P_PG_POL_25_String			"EVR_PG_POL_25"										/* asynInt32,   w */
#define P_PG_POL_26_String			"EVR_PG_POL_26"										/* asynInt32,   w */
#define P_PG_POL_27_String			"EVR_PG_POL_27"										/* asynInt32,   w */
#define P_PG_POL_28_String			"EVR_PG_POL_28"										/* asynInt32,   w */
#define P_PG_POL_29_String			"EVR_PG_POL_29"										/* asynInt32,   w */
#define P_PG_POL_30_String			"EVR_PG_POL_30"										/* asynInt32,   w */
#define P_PG_POL_31_String			"EVR_PG_POL_31"										/* asynInt32,   w */
#define P_PG_POL_32_String			"EVR_PG_POL_32"										/* asynInt32,   w */

extern "C" {
}

using namespace std;

#if 1
namespace __gnu_cxx {
	template <>
		struct hash<std::string> {
			size_t operator() (const std::string& x) const {
				return hash<const char*>()(x.c_str());
			}
		};
};
#endif

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
	int P_MUX_Couter_A;
	int P_MUX_Couter_B;
	int P_MUX_Couter_C;
	int P_MUX_Couter_D;
	int P_MUX_Couter_E;
	int P_MUX_Couter_F;
	int P_MUX_Couter_G;
	int P_MUX_Couter_H;
	int P_MUX_Couter_I;
	int P_MUX_Couter_J;
	int P_MUX_Couter_K;
	int P_MUX_Couter_L;
	int P_MUX_Couter_M;
	int P_MUX_Couter_N;
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

	//__gnu_cxx::hash_map<string, string> regmaptable;
	__gnu_cxx::hash_map<string, struct TimingRegmap> regmaptable;
	int tsMode(const char* mode);
	void setTime();
	void setBuildTime();
	void registerParamListFromFile(string filename);
	//		int ts2ip_rd(int fd, unsigned int offset, unsigned int *rdData);

};

#define NUM_SCOPE_PARAMS (&LAST_SCOPE_COMMAND - &FIRST_SCOPE_COMMAND + 1)

#endif
