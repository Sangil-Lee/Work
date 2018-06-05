#include <cstdlib>
#include <cstring>
#include <bitset>
#include <epicsTypes.h>
#include <epicsTime.h>
#include <epicsThread.h>
#include <epicsString.h>
#include <epicsTimer.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <registryFunction.h>
#include <epicsExport.h>
#include <dbCommon.h>
#include <iocsh.h>

#include "timingAsynEpics.h"

#define FREQUENCY 1000       /* Frequency in Hz */
#define AMPLITUDE 1.0        /* Plus and minus peaks of sin wave */
#define NUM_DIVISIONS 10     /* Number of scope divisions in X and Y */
#define MIN_UPDATE_TIME 0.02 /* Minimum update time, to prevent CPU saturation */

static void userProcess(void *drvPvt);
extern struct s_ts2sys* gpSys;


TimingRegmap::TimingRegmap():firmware(0.1),software(3.14125)
{
}

timingAsynEpics::timingAsynEpics(const char *portName, int maxSizeSnapshot, int maxSizeBufferMB, int clientMode, const char* filename, const char *deviceName = "EVR")
   : asynPortDriver(portName, 
                    1,  /*maxAddr */ 
					/*0xC600, maxAddr*/
                    (int)NUM_SCOPE_PARAMS,
                    asynInt32Mask | asynFloat64Mask | asynInt16ArrayMask | asynFloat64ArrayMask | asynEnumMask | asynOctetMask |asynDrvUserMask, /* Interface mask */
                    asynInt32Mask | asynFloat64Mask | asynInt16ArrayMask | asynFloat64ArrayMask | asynEnumMask | asynOctetMask,  /* Interrupt mask */
                    1, /* asynFlags.  This driver does not block and it is not multi-device, so flag is 0 */
                    1, /* Autoconnect */
                    0, /* Default priority */
                    0 /* Default stack size*/
										),fileName(filename),driverName("timingAsynEpics"),system_init_ok(0) 
{
    asynStatus status;
    const char *functionName = "timingAsynEpics";

	tsmode = tsMode(deviceName);
	printf("TSMode(%d)\n", tsmode);


	//ts2api_init(tsmode);
	//ts2api_getInstance(gpSys);

    // Create database entries
    eventId_ = epicsEventCreate(epicsEventEmpty);  
    //pregmap = (struct TimingRegmap *)malloc(sizeof(TimingRegmap));
    //pregmap = (TimingRegmap *)calloc(1,sizeof(TimingRegmap));
    pregmap = new TimingRegmap();
	clientThreadMode = clientMode;

    // Snapshots  
	registerParamListFromFile(fileName);

    if (system_init_ok==1) 
		{
				status = (asynStatus)(epicsThreadCreate("timingAsynEpicsUserTask",
										epicsThreadPriorityMedium,
										epicsThreadGetStackSize(epicsThreadStackMedium),
										(EPICSTHREADFUNC)::userProcess,
										this) == NULL);
				if (status) {
						printf("%s:%s: epicsThreadCreate failure\n", driverName, functionName);
						return;
				}
		} else {
				status = asynError;
		};

		//printf("Number:%d, Last:(%d), First(%d)\n", NUM_SCOPE_PARAMS,&LAST_SCOPE_COMMAND, &FIRST_SCOPE_COMMAND);
}

void timingAsynEpics::registerParamListFromFile(string filename)
{
	ifstream file(filename.c_str());
	if(file.fail()) return;
	
	string strToken;
	while(!file.eof())
	{
		getline(file, strToken);
		if(strToken[0] == '#' || strToken.empty()==true) continue;

		char str[strToken.size()];
		strcpy (str, strToken.c_str());

		char *pch = 0;
		if(!(pch = strtok (str," \t,"))) continue;
		memset(&regmap,0, sizeof(regMap));

#if 1
		printf("ALIAS_Name(%s)\n", pch);
		strcpy(regmap.alias, pch);
		while (pch != NULL)
		{
			if(!(pch = strtok (NULL," \t,"))) continue;
			printf("Addr(%s)\n", pch);
			regmap.address = strtol(pch,NULL,16);
			if(!(pch = strtok (NULL," \t,"))) continue;
			printf("AsynParam(%s)\n", pch);
		};
#endif
		regmapfile[regmap.alias] = regmap;
	};	
	
	
#if 1
    // System list
    createParam(P_BuildTimeString,        asynParamOctet,		&P_BuildTime);
	regmaptable[P_BuildTime] = regmapfile[P_BuildTimeString];
	//regMap reg = regmaptable[P_BuildTime];
    createParam(P_StartTimeString,        asynParamOctet,		&P_Starttime);
	regmaptable[P_Starttime] = regmapfile[P_StartTimeString];
    createParam(P_IOCStartTimeString,     asynParamOctet,		&P_IOCStartTime);
	regmaptable[P_IOCStartTime] = regmapfile[P_IOCStartTimeString];
    createParam(P_EVS_TIMEString,  		  asynParamInt32,		&P_EVS_Time);
	regmaptable[P_EVS_Time] = regmapfile[P_EVS_TIMEString];
    createParam(P_EVS_DAYString,     	  asynParamInt32,		&P_EVS_Day);
	regmaptable[P_EVS_Day] = regmapfile[P_EVS_DAYString];
    createParam(P_EVS_SETString,     	  asynParamInt32,		&P_EVS_Set);
	regmaptable[P_EVS_Set] = regmapfile[P_EVS_SETString];
    createParam(P_FRONT_A_String, 		asynParamInt32,				&P_Fan_Front_A);
	regmaptable[P_Fan_Front_A] = regmapfile[P_FRONT_A_String];
    createParam(P_FRONT_B_String, 		asynParamInt32,				&P_Fan_Front_B);
	regmaptable[P_Fan_Front_B] = regmapfile[P_FRONT_B_String];
    createParam(P_REAR_A_String, 		asynParamInt32,				&P_Fan_Rear_A);
	regmaptable[P_Fan_Rear_A] = regmapfile[P_REAR_A_String];
    createParam(P_REAR_B_String, 		asynParamInt32,				&P_Fan_Rear_B);
	regmaptable[P_Fan_Rear_B] = regmapfile[P_REAR_B_String];
	createParam(P_MUX_Couter_A_String,	asynParamFloat64,		 	&P_MUX_Couter_A);
	regmaptable[P_MUX_Couter_A] = regmapfile[P_MUX_Couter_A_String];
	createParam(P_MUX_Couter_B_String,	asynParamFloat64,			&P_MUX_Couter_B);
	regmaptable[P_MUX_Couter_B] = regmapfile[P_MUX_Couter_B_String];
	createParam(P_MUX_Couter_C_String,	asynParamFloat64,			&P_MUX_Couter_C);
	regmaptable[P_MUX_Couter_C] = regmapfile[P_MUX_Couter_C_String];
	createParam(P_MUX_Couter_D_String,	asynParamFloat64,			&P_MUX_Couter_D);
	regmaptable[P_MUX_Couter_D] = regmapfile[P_MUX_Couter_D_String];
	createParam(P_MUX_Couter_E_String,	asynParamFloat64,			&P_MUX_Couter_E);
	regmaptable[P_MUX_Couter_E] = regmapfile[P_MUX_Couter_E_String];
	createParam(P_MUX_Couter_F_String,	asynParamFloat64,			&P_MUX_Couter_F);
	regmaptable[P_MUX_Couter_F] = regmapfile[P_MUX_Couter_F_String];
	createParam(P_MUX_Couter_G_String,	asynParamFloat64,			&P_MUX_Couter_G);
	regmaptable[P_MUX_Couter_G] = regmapfile[P_MUX_Couter_G_String];
	createParam(P_MUX_Couter_H_String,	asynParamFloat64,			&P_MUX_Couter_H);
	regmaptable[P_MUX_Couter_H] = regmapfile[P_MUX_Couter_H_String];
	createParam(P_MUX_Couter_I_String,	asynParamFloat64,			&P_MUX_Couter_I);
	regmaptable[P_MUX_Couter_I] = regmapfile[P_MUX_Couter_I_String];
	createParam(P_MUX_Couter_J_String,	asynParamFloat64,			&P_MUX_Couter_J);
	regmaptable[P_MUX_Couter_J] = regmapfile[P_MUX_Couter_J_String];
	createParam(P_MUX_Couter_K_String,	asynParamFloat64,			&P_MUX_Couter_K);
	regmaptable[P_MUX_Couter_K] = regmapfile[P_MUX_Couter_K_String];
	createParam(P_MUX_Couter_L_String,	asynParamFloat64,			&P_MUX_Couter_L);
	regmaptable[P_MUX_Couter_L] = regmapfile[P_MUX_Couter_L_String];
	createParam(P_MUX_Couter_M_String,	asynParamFloat64,			&P_MUX_Couter_M);
	regmaptable[P_MUX_Couter_M] = regmapfile[P_MUX_Couter_M_String];
	createParam(P_MUX_Couter_N_String,	asynParamFloat64,			&P_MUX_Couter_N);
	regmaptable[P_MUX_Couter_N] = regmapfile[P_MUX_Couter_N_String];
	createParam(P_SEQ_USER_Tri_String, 	asynParamInt32,				&P_SEQ_User_Trigger);
	regmaptable[P_SEQ_User_Trigger] = regmapfile[P_SEQ_USER_Tri_String];
	createParam(P_PULSE_Gen_String,		asynParamInt32,				&P_PG_Update);
	regmaptable[P_PG_Update] = regmapfile[P_PULSE_Gen_String];

	createParam(P_PG_Width_1_String,	asynParamInt32,			   &P_PG_Width_1);
	createParam(P_PG_Width_2_String,	asynParamInt32,			   &P_PG_Width_2);
	createParam(P_PG_Width_3_String,	asynParamInt32,			   &P_PG_Width_3);
	createParam(P_PG_Width_4_String,	asynParamInt32,			   &P_PG_Width_4);
	createParam(P_PG_Width_5_String,	asynParamInt32,			   &P_PG_Width_5);
	createParam(P_PG_Width_6_String,	asynParamInt32,			   &P_PG_Width_6);
	createParam(P_PG_Width_7_String,	asynParamInt32,			   &P_PG_Width_7);
	createParam(P_PG_Width_8_String,	asynParamInt32,			   &P_PG_Width_8);
	createParam(P_PG_Width_9_String,	asynParamInt32,			   &P_PG_Width_9);
	createParam(P_PG_Width_10_String,	asynParamInt32,			   &P_PG_Width_10);
	createParam(P_PG_Width_11_String,	asynParamInt32,			   &P_PG_Width_11);
	createParam(P_PG_Width_12_String,	asynParamInt32,			   &P_PG_Width_12);
	createParam(P_PG_Width_13_String,	asynParamInt32,			   &P_PG_Width_13);
	createParam(P_PG_Width_14_String,	asynParamInt32,			   &P_PG_Width_14);
	createParam(P_PG_Width_15_String,	asynParamInt32,			   &P_PG_Width_15);
	createParam(P_PG_Width_16_String,	asynParamInt32,			   &P_PG_Width_16);
	createParam(P_PG_Width_17_String,	asynParamInt32,			   &P_PG_Width_17);
	createParam(P_PG_Width_18_String,	asynParamInt32,			   &P_PG_Width_18);
	createParam(P_PG_Width_19_String,	asynParamInt32,			   &P_PG_Width_19);
	createParam(P_PG_Width_20_String,	asynParamInt32,			   &P_PG_Width_20);
	createParam(P_PG_Width_21_String,	asynParamInt32,			   &P_PG_Width_21);
	createParam(P_PG_Width_22_String,	asynParamInt32,			   &P_PG_Width_22);
	createParam(P_PG_Width_23_String,	asynParamInt32,			   &P_PG_Width_23);
	createParam(P_PG_Width_24_String,	asynParamInt32,			   &P_PG_Width_24);
	createParam(P_PG_Width_25_String,	asynParamInt32,			   &P_PG_Width_25);
	createParam(P_PG_Width_26_String,	asynParamInt32,			   &P_PG_Width_26);
	createParam(P_PG_Width_27_String,	asynParamInt32,			   &P_PG_Width_27);
	createParam(P_PG_Width_28_String,	asynParamInt32,			   &P_PG_Width_28);
	createParam(P_PG_Width_29_String,	asynParamInt32,			   &P_PG_Width_29);
	createParam(P_PG_Width_30_String,	asynParamInt32,			   &P_PG_Width_30);
	createParam(P_PG_Width_31_String,	asynParamInt32,			   &P_PG_Width_31);
	createParam(P_PG_Width_32_String,	asynParamInt32,			   &P_PG_Width_32);

	regmaptable[P_PG_Width_1] = regmapfile[P_PG_Width_1_String];
	regmaptable[P_PG_Width_2] = regmapfile[P_PG_Width_2_String];
	regmaptable[P_PG_Width_3] = regmapfile[P_PG_Width_3_String];
	regmaptable[P_PG_Width_4] = regmapfile[P_PG_Width_4_String];
	regmaptable[P_PG_Width_5] = regmapfile[P_PG_Width_5_String];
	regmaptable[P_PG_Width_6] = regmapfile[P_PG_Width_6_String];
	regmaptable[P_PG_Width_7] = regmapfile[P_PG_Width_7_String];
	regmaptable[P_PG_Width_8] = regmapfile[P_PG_Width_8_String];
	regmaptable[P_PG_Width_9] = regmapfile[P_PG_Width_9_String];
	regmaptable[P_PG_Width_10] = regmapfile[P_PG_Width_10_String];
	regmaptable[P_PG_Width_11] = regmapfile[P_PG_Width_11_String];
	regmaptable[P_PG_Width_12] = regmapfile[P_PG_Width_12_String];
	regmaptable[P_PG_Width_13] = regmapfile[P_PG_Width_13_String];
	regmaptable[P_PG_Width_14] = regmapfile[P_PG_Width_14_String];
	regmaptable[P_PG_Width_15] = regmapfile[P_PG_Width_15_String];
	regmaptable[P_PG_Width_16] = regmapfile[P_PG_Width_16_String];
	regmaptable[P_PG_Width_17] = regmapfile[P_PG_Width_17_String];
	regmaptable[P_PG_Width_18] = regmapfile[P_PG_Width_18_String];
	regmaptable[P_PG_Width_19] = regmapfile[P_PG_Width_19_String];
	regmaptable[P_PG_Width_20] = regmapfile[P_PG_Width_20_String];
	regmaptable[P_PG_Width_21] = regmapfile[P_PG_Width_21_String];
	regmaptable[P_PG_Width_22] = regmapfile[P_PG_Width_22_String];
	regmaptable[P_PG_Width_23] = regmapfile[P_PG_Width_23_String];
	regmaptable[P_PG_Width_24] = regmapfile[P_PG_Width_24_String];
	regmaptable[P_PG_Width_25] = regmapfile[P_PG_Width_25_String];
	regmaptable[P_PG_Width_26] = regmapfile[P_PG_Width_26_String];
	regmaptable[P_PG_Width_27] = regmapfile[P_PG_Width_27_String];
	regmaptable[P_PG_Width_28] = regmapfile[P_PG_Width_28_String];
	regmaptable[P_PG_Width_29] = regmapfile[P_PG_Width_29_String];
	regmaptable[P_PG_Width_30] = regmapfile[P_PG_Width_30_String];
	regmaptable[P_PG_Width_31] = regmapfile[P_PG_Width_31_String];
	regmaptable[P_PG_Width_32] = regmapfile[P_PG_Width_32_String];

	createParam(P_PG_Delay_1_String,	asynParamInt32,				&P_PG_Delay_1);
	createParam(P_PG_Delay_2_String,    asynParamInt32,				&P_PG_Delay_2);
	createParam(P_PG_Delay_3_String,    asynParamInt32,				&P_PG_Delay_3);
	createParam(P_PG_Delay_4_String,    asynParamInt32,				&P_PG_Delay_4);
	createParam(P_PG_Delay_5_String,    asynParamInt32,				&P_PG_Delay_5);
	createParam(P_PG_Delay_6_String,    asynParamInt32,				&P_PG_Delay_6);
	createParam(P_PG_Delay_7_String,    asynParamInt32,				&P_PG_Delay_7);
	createParam(P_PG_Delay_8_String,    asynParamInt32,				&P_PG_Delay_8);
	createParam(P_PG_Delay_9_String,    asynParamInt32,				&P_PG_Delay_9);
	createParam(P_PG_Delay_10_String,   asynParamInt32,				&P_PG_Delay_10);
	createParam(P_PG_Delay_11_String,   asynParamInt32,				&P_PG_Delay_11);
	createParam(P_PG_Delay_12_String,   asynParamInt32,				&P_PG_Delay_12);
	createParam(P_PG_Delay_13_String,   asynParamInt32,				&P_PG_Delay_13);
	createParam(P_PG_Delay_14_String,   asynParamInt32,				&P_PG_Delay_14);
	createParam(P_PG_Delay_15_String,   asynParamInt32,				&P_PG_Delay_15);
	createParam(P_PG_Delay_16_String,   asynParamInt32,				&P_PG_Delay_16);
	createParam(P_PG_Delay_17_String,   asynParamInt32,				&P_PG_Delay_17);
	createParam(P_PG_Delay_18_String,   asynParamInt32,				&P_PG_Delay_18);
	createParam(P_PG_Delay_19_String,   asynParamInt32,				&P_PG_Delay_19);
	createParam(P_PG_Delay_20_String,   asynParamInt32,				&P_PG_Delay_20);
	createParam(P_PG_Delay_21_String,   asynParamInt32,				&P_PG_Delay_21);
	createParam(P_PG_Delay_22_String,   asynParamInt32,				&P_PG_Delay_22);
	createParam(P_PG_Delay_23_String,   asynParamInt32,				&P_PG_Delay_23);
	createParam(P_PG_Delay_24_String,   asynParamInt32,				&P_PG_Delay_24);
	createParam(P_PG_Delay_25_String,   asynParamInt32,				&P_PG_Delay_25);
	createParam(P_PG_Delay_26_String,   asynParamInt32,				&P_PG_Delay_26);
	createParam(P_PG_Delay_27_String,   asynParamInt32,				&P_PG_Delay_27);
	createParam(P_PG_Delay_28_String,   asynParamInt32,				&P_PG_Delay_28);
	createParam(P_PG_Delay_29_String,   asynParamInt32,				&P_PG_Delay_29);
	createParam(P_PG_Delay_30_String,   asynParamInt32,				&P_PG_Delay_30);
	createParam(P_PG_Delay_31_String,   asynParamInt32,				&P_PG_Delay_31);
	createParam(P_PG_Delay_32_String,   asynParamInt32,				&P_PG_Delay_32);

	regmaptable[P_PG_Delay_1] = regmapfile[P_PG_Delay_1_String];
	regmaptable[P_PG_Delay_2] = regmapfile[P_PG_Delay_2_String];
	regmaptable[P_PG_Delay_3] = regmapfile[P_PG_Delay_3_String];
	regmaptable[P_PG_Delay_4] = regmapfile[P_PG_Delay_4_String];
	regmaptable[P_PG_Delay_5] = regmapfile[P_PG_Delay_5_String];
	regmaptable[P_PG_Delay_6] = regmapfile[P_PG_Delay_6_String];
	regmaptable[P_PG_Delay_7] = regmapfile[P_PG_Delay_7_String];
	regmaptable[P_PG_Delay_8] = regmapfile[P_PG_Delay_8_String];
	regmaptable[P_PG_Delay_9] = regmapfile[P_PG_Delay_9_String];
	regmaptable[P_PG_Delay_10] = regmapfile[P_PG_Delay_10_String];
	regmaptable[P_PG_Delay_11] = regmapfile[P_PG_Delay_11_String];
	regmaptable[P_PG_Delay_12] = regmapfile[P_PG_Delay_12_String];
	regmaptable[P_PG_Delay_13] = regmapfile[P_PG_Delay_13_String];
	regmaptable[P_PG_Delay_14] = regmapfile[P_PG_Delay_14_String];
	regmaptable[P_PG_Delay_15] = regmapfile[P_PG_Delay_15_String];
	regmaptable[P_PG_Delay_16] = regmapfile[P_PG_Delay_16_String];
	regmaptable[P_PG_Delay_17] = regmapfile[P_PG_Delay_17_String];
	regmaptable[P_PG_Delay_18] = regmapfile[P_PG_Delay_18_String];
	regmaptable[P_PG_Delay_19] = regmapfile[P_PG_Delay_19_String];
	regmaptable[P_PG_Delay_20] = regmapfile[P_PG_Delay_20_String];
	regmaptable[P_PG_Delay_21] = regmapfile[P_PG_Delay_21_String];
	regmaptable[P_PG_Delay_22] = regmapfile[P_PG_Delay_22_String];
	regmaptable[P_PG_Delay_23] = regmapfile[P_PG_Delay_23_String];
	regmaptable[P_PG_Delay_24] = regmapfile[P_PG_Delay_24_String];
	regmaptable[P_PG_Delay_25] = regmapfile[P_PG_Delay_25_String];
	regmaptable[P_PG_Delay_26] = regmapfile[P_PG_Delay_26_String];
	regmaptable[P_PG_Delay_27] = regmapfile[P_PG_Delay_27_String];
	regmaptable[P_PG_Delay_28] = regmapfile[P_PG_Delay_28_String];
	regmaptable[P_PG_Delay_29] = regmapfile[P_PG_Delay_29_String];
	regmaptable[P_PG_Delay_30] = regmapfile[P_PG_Delay_30_String];
	regmaptable[P_PG_Delay_31] = regmapfile[P_PG_Delay_31_String];
	regmaptable[P_PG_Delay_32] = regmapfile[P_PG_Delay_32_String];

	createParam(P_PG_POL_1_String,		asynParamInt32,				&P_PG_POL_1);
	createParam(P_PG_POL_2_String,      asynParamInt32,				&P_PG_POL_2);
	createParam(P_PG_POL_3_String,      asynParamInt32,				&P_PG_POL_3);
	createParam(P_PG_POL_4_String,      asynParamInt32,				&P_PG_POL_4);
	createParam(P_PG_POL_5_String,      asynParamInt32,				&P_PG_POL_5);
	createParam(P_PG_POL_6_String,      asynParamInt32,				&P_PG_POL_6);
	createParam(P_PG_POL_7_String,      asynParamInt32,				&P_PG_POL_7);
	createParam(P_PG_POL_8_String,      asynParamInt32,				&P_PG_POL_8);
	createParam(P_PG_POL_9_String,      asynParamInt32,				&P_PG_POL_9);
	createParam(P_PG_POL_10_String,     asynParamInt32,				&P_PG_POL_10);
	createParam(P_PG_POL_11_String,     asynParamInt32,				&P_PG_POL_11);
	createParam(P_PG_POL_12_String,     asynParamInt32,				&P_PG_POL_12);
	createParam(P_PG_POL_13_String,     asynParamInt32,				&P_PG_POL_13);
	createParam(P_PG_POL_14_String,     asynParamInt32,				&P_PG_POL_14);
	createParam(P_PG_POL_15_String,     asynParamInt32,				&P_PG_POL_15);
	createParam(P_PG_POL_16_String,     asynParamInt32,				&P_PG_POL_16);
	createParam(P_PG_POL_17_String,     asynParamInt32,				&P_PG_POL_17);
	createParam(P_PG_POL_18_String,     asynParamInt32,				&P_PG_POL_18);
	createParam(P_PG_POL_19_String,     asynParamInt32,				&P_PG_POL_19);
	createParam(P_PG_POL_20_String,     asynParamInt32,				&P_PG_POL_20);
	createParam(P_PG_POL_21_String,     asynParamInt32,				&P_PG_POL_21);
	createParam(P_PG_POL_22_String,     asynParamInt32,				&P_PG_POL_22);
	createParam(P_PG_POL_23_String,     asynParamInt32,				&P_PG_POL_23);
	createParam(P_PG_POL_24_String,     asynParamInt32,				&P_PG_POL_24);
	createParam(P_PG_POL_25_String,     asynParamInt32,				&P_PG_POL_25);
	createParam(P_PG_POL_26_String,     asynParamInt32,				&P_PG_POL_26);
	createParam(P_PG_POL_27_String,     asynParamInt32,				&P_PG_POL_27);
	createParam(P_PG_POL_28_String,     asynParamInt32,				&P_PG_POL_28);
	createParam(P_PG_POL_29_String,     asynParamInt32,				&P_PG_POL_29);
	createParam(P_PG_POL_30_String,     asynParamInt32,				&P_PG_POL_30);
	createParam(P_PG_POL_31_String,     asynParamInt32,				&P_PG_POL_31);
	createParam(P_PG_POL_32_String,     asynParamInt32,				&P_PG_POL_32);

	regmaptable[P_PG_POL_1] = regmapfile[P_PG_POL_1_String];
	regmaptable[P_PG_POL_2] = regmapfile[P_PG_POL_2_String];
	regmaptable[P_PG_POL_3] = regmapfile[P_PG_POL_3_String];
	regmaptable[P_PG_POL_4] = regmapfile[P_PG_POL_4_String];
	regmaptable[P_PG_POL_5] = regmapfile[P_PG_POL_5_String];
	regmaptable[P_PG_POL_6] = regmapfile[P_PG_POL_6_String];
	regmaptable[P_PG_POL_7] = regmapfile[P_PG_POL_7_String];
	regmaptable[P_PG_POL_8] = regmapfile[P_PG_POL_8_String];
	regmaptable[P_PG_POL_9] = regmapfile[P_PG_POL_9_String];
	regmaptable[P_PG_POL_10] = regmapfile[P_PG_POL_10_String];
	regmaptable[P_PG_POL_11] = regmapfile[P_PG_POL_11_String];
	regmaptable[P_PG_POL_12] = regmapfile[P_PG_POL_12_String];
	regmaptable[P_PG_POL_13] = regmapfile[P_PG_POL_13_String];
	regmaptable[P_PG_POL_14] = regmapfile[P_PG_POL_14_String];
	regmaptable[P_PG_POL_15] = regmapfile[P_PG_POL_15_String];
	regmaptable[P_PG_POL_16] = regmapfile[P_PG_POL_16_String];
	regmaptable[P_PG_POL_17] = regmapfile[P_PG_POL_17_String];
	regmaptable[P_PG_POL_18] = regmapfile[P_PG_POL_18_String];
	regmaptable[P_PG_POL_19] = regmapfile[P_PG_POL_19_String];
	regmaptable[P_PG_POL_20] = regmapfile[P_PG_POL_20_String];
	regmaptable[P_PG_POL_21] = regmapfile[P_PG_POL_21_String];
	regmaptable[P_PG_POL_22] = regmapfile[P_PG_POL_22_String];
	regmaptable[P_PG_POL_23] = regmapfile[P_PG_POL_23_String];
	regmaptable[P_PG_POL_24] = regmapfile[P_PG_POL_24_String];
	regmaptable[P_PG_POL_25] = regmapfile[P_PG_POL_25_String];
	regmaptable[P_PG_POL_26] = regmapfile[P_PG_POL_26_String];
	regmaptable[P_PG_POL_27] = regmapfile[P_PG_POL_27_String];
	regmaptable[P_PG_POL_28] = regmapfile[P_PG_POL_28_String];
	regmaptable[P_PG_POL_29] = regmapfile[P_PG_POL_29_String];
	regmaptable[P_PG_POL_30] = regmapfile[P_PG_POL_30_String];
	regmaptable[P_PG_POL_31] = regmapfile[P_PG_POL_31_String];
	regmaptable[P_PG_POL_32] = regmapfile[P_PG_POL_32_String];
#endif
			
	//Default Initial Value
	//setIntegerParam(P_TSMode, tsmode);
	//setIntegerParam(P_Run, 0);
	//setDoubleParam(P_UpdateTime, 1);
	//setDoubleParam(P_Firmware, pregmap->GetFirmware());
	//setDoubleParam(P_Software, pregmap->GetSoftware());
	setStringParam(P_BuildTime, "2018-06-01");
	setTime();


}

int timingAsynEpics::tsMode(const char* mode)
{
		//{1 [evg] | 2 [evr] | 3 [evf] | 4 [evs]}");
		if(strcmp(mode, "EVR") == 0 || strcmp(mode, "evs") == 0)
			tsmode = 2;
		else if(strcmp(mode, "EVF") == 0 || strcmp(mode, "evf") == 0)
			tsmode = 3;
		else if(strcmp(mode, "EVS") == 0 || strcmp(mode, "evs") == 0)
			tsmode = 4;
		else
			tsmode = 1;

		return(tsmode);
}

asynStatus timingAsynEpics::readInt32(asynUser *pasynUser, epicsInt32 *value)
{
    int function = pasynUser->reason;
    int addr=0;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
    static const char *functionName = "readInt32";
    
    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
    /* We just read the current value of the parameter from the parameter library.
     * Those values are updated whenever anything could cause them to change */
    status = (asynStatus) getIntegerParam(addr, function, value);
    /* Set the timestamp */
    pasynUser->timestamp = timeStamp;

	unsigned int rdData;
	if(function == P_Fan_Front_A)
	{
		regmap = regmaptable[P_Fan_Front_A];
		ts2ip_rd(gpSys->ip.ev.fd, regmap.address, (unsigned int*)&rdData);
		*value = S_ifanStt4_CntrFrontA(rdData);
	}
	else if(function == P_Fan_Front_B)
	{
		regmap = regmaptable[P_Fan_Front_B];
		ts2ip_rd(gpSys->ip.ev.fd, regmap.address, (unsigned int*)&rdData);
		*value = S_ifanStt4_CntrFrontB(rdData);
	}
	else if(function == P_Fan_Rear_A)
	{
		regmap = regmaptable[P_Fan_Rear_A];
		ts2ip_rd(gpSys->ip.ev.fd, regmap.address, (unsigned int*)&rdData);
		*value = S_ifanStt4_CntrRearA(rdData);
	}
	else if(function == P_Fan_Rear_B)
	{
		regmap = regmaptable[P_Fan_Rear_B];
		ts2ip_rd(gpSys->ip.ev.fd, regmap.address, (unsigned int*)&rdData);
		*value = S_ifanStt4_CntrRearB(rdData);
	}

    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, value=%d", 
                  driverName, functionName, status, function, *value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, value=%d\n", 
              driverName, functionName, function, *value);
	status = (asynStatus) callParamCallbacks();
    return(status);
}

asynStatus timingAsynEpics::writeInt32(asynUser *pasynUser, epicsInt32 value)
{
	int function = pasynUser->reason;
	asynStatus status = asynSuccess;
	const char *paramName;
	const char* functionName = "writeInt32";

	int addr=0;
	status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);

	/* Set the parameter in the parameter library. */
	status = (asynStatus) setIntegerParam(function, value);

	/* Fetch the parameter string name for possible use in debugging */
	getParamName(function, &paramName);
	bitset<40>	polbit;

	if (function == P_Run) {
		/* If run was set then wake up the userProcess task */
		if (value) epicsEventSignal(eventId_);
	} 
	else if (function == P_EVS_Time)
	{
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, C_evgW_set_h8(2) | C_evgW_set_m8(3) | C_evgW_set_s8(4) );
	}
	else if (function == P_EVS_Day)
	{
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address + 4, C_evgW_set_d16(360));
	}
	else if (function == P_EVS_Set)
	{
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address,1);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address,0);
	}
	else if (function == P_SEQ_User_Trigger)
	{
		//ADRR = 0x078
		//printf("USER Trigger Addr(%x)\n", addr);
		int value;
		regmap = regmaptable[function];
		getIntegerParam(P_SEQ_User_Trigger, &value);
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address,C_usrTrg(1));
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address,C_usrTrg(0));
		value = 0;
		setIntegerParam(P_SEQ_User_Trigger, value);
	}
	else if (function == P_PG_Update)
	{
		int pg_update;
		getIntegerParam(P_PG_Update, &pg_update);
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address,C_evr_setCtrlReg(1));
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address,C_evr_setCtrlReg(0));
	}
	else if (function == P_PG_Width_1 || function == P_PG_Width_2  || function == P_PG_Width_3  || function == P_PG_Width_4 ||
	         function == P_PG_Width_5 || function == P_PG_Width_6  || function == P_PG_Width_7  || function == P_PG_Width_8 ||
	         function == P_PG_Width_9 || function == P_PG_Width_10 || function == P_PG_Width_11 || function == P_PG_Width_12 ||
	         function == P_PG_Width_13|| function == P_PG_Width_14 || function == P_PG_Width_15 || function == P_PG_Width_16 ||
	         function == P_PG_Width_17|| function == P_PG_Width_18 || function == P_PG_Width_19 || function == P_PG_Width_20 ||
	         function == P_PG_Width_21|| function == P_PG_Width_22 || function == P_PG_Width_23 || function == P_PG_Width_24 ||
	         function == P_PG_Width_25|| function == P_PG_Width_26 || function == P_PG_Width_27 || function == P_PG_Width_28 ||
	         function == P_PG_Width_29|| function == P_PG_Width_30 || function == P_PG_Width_31 || function == P_PG_Width_32)
	{
		int width;
        getIntegerParam(function, &width);
		
		regmap = regmaptable[function];
		//0xC480
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, width);
	}
	else if (function == P_PG_Delay_1  || function == P_PG_Delay_2  || function == P_PG_Delay_3  || function == P_PG_Delay_4 ||
	         function == P_PG_Delay_5  || function == P_PG_Delay_6  || function == P_PG_Delay_7  || function == P_PG_Delay_8 ||
	         function == P_PG_Delay_9  || function == P_PG_Delay_10 || function == P_PG_Delay_11 || function == P_PG_Delay_12 ||
	         function == P_PG_Delay_13 || function == P_PG_Delay_14 || function == P_PG_Delay_15 || function == P_PG_Delay_16 ||
	         function == P_PG_Delay_17 || function == P_PG_Delay_18 || function == P_PG_Delay_19 || function == P_PG_Delay_20 ||
	         function == P_PG_Delay_21 || function == P_PG_Delay_22 || function == P_PG_Delay_23 || function == P_PG_Delay_24 ||
	         function == P_PG_Delay_25 || function == P_PG_Delay_26 || function == P_PG_Delay_27 || function == P_PG_Delay_28 ||
	         function == P_PG_Delay_29 || function == P_PG_Delay_30 || function == P_PG_Delay_31 || function == P_PG_Delay_32)

	{
		int delay;
        getIntegerParam(function, &delay);
		
		regmap = regmaptable[function];
		//0xC400
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, delay);
	}
	else if (function == P_PG_POL_1)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(1,0);
		else 		polbit.set(1,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	
	}
	else if (function == P_PG_POL_2)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(2,0);
		else 		polbit.set(2,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_3)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(3,0);
		else 		polbit.set(3,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_4)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(4,0);
		else 		polbit.set(4,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_5)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(5,0);
		else 		polbit.set(5,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_6)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(6,0);
		else 		polbit.set(6,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_7)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(7,0);
		else 		polbit.set(7,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_8)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(8,0);
		else 		polbit.set(8,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_9)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(9,0);
		else 		polbit.set(9,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_10)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(10,0);
		else 		polbit.set(10,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_11)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(11,0);
		else 		polbit.set(11,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_12)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(12,0);
		else 		polbit.set(12,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_13)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(13,0);
		else 		polbit.set(13,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_14)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(14,0);
		else 		polbit.set(14,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_15)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(15,0);
		else 		polbit.set(15,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_16)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(16,0);
		else 		polbit.set(16,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_17)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(17,0);
		else 		polbit.set(17,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_18)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(18,0);
		else 		polbit.set(18,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_19)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(19,0);
		else 		polbit.set(19,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_20)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(20,0);
		else 		polbit.set(20,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_21)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(21,0);
		else 		polbit.set(21,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_22)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(22,0);
		else 		polbit.set(22,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_23)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(23,0);
		else 		polbit.set(23,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_24)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(24,0);
		else 		polbit.set(24,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_25)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(25,0);
		else 		polbit.set(25,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_26)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(26,0);
		else 		polbit.set(26,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_27)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(27,0);
		else 		polbit.set(27,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_28)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(28,0);
		else 		polbit.set(28,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_29)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(29,0);
		else 		polbit.set(29,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_30)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(30,0);
		else 		polbit.set(30,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_31)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(31,0);
		else 		polbit.set(31,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}
	else if (function == P_PG_POL_32)
	{
		int pol;
        getIntegerParam(function, &pol);
		
		if(pol == 0) polbit.set(32,0);
		else 		polbit.set(32,1);
		
		int polality = (int)polbit.to_ulong();

		//0x104
		regmap = regmaptable[function];
		ts2ip_wr(gpSys->ip.ev.fd, regmap.address, polality);
	}


	/* Do callbacks so higher layers see any changes */
	status = (asynStatus) callParamCallbacks();

	if (status) 
		epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
			"%s:%s: status=%d, function=%d, name=%s, value=%d", 
			driverName, functionName, status, function, paramName, value);
	else        
		asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
			"%s:%s: function=%d, name=%s, value=%d\n", 
			driverName, functionName, function, paramName, value);
	return status;
}

void timingAsynEpics::setTime()
{
		epicsTimeStamp current;
		epicsTimeGetCurrent (&current);
		char timebuf[20];
		epicsTimeToStrftime(timebuf,20,"%Y/%m/%d %H:%M:%S", &current);
		setStringParam(P_IOCStartTime, timebuf);
}

void timingAsynEpics::setBuildTime()
{
	unsigned int rdData;
	ts2ip_rd(gpSys->ip.ev.fd, A_buildTime, (unsigned int*)&rdData);

	char timebuf[20];
	sprintf(timebuf,"20%d/%d/%d %d:%d:%d", S_buildYear(rdData), S_buildMonth(rdData), S_buildDay(rdData), S_buildHour(rdData), S_buildMin(rdData), S_buildSec(rdData));

	printf("Buildtime:%s\n", timebuf);
	setStringParam(P_BuildTime, timebuf);

#if 0
	pInfo->buildTime.year  = S_buildYear(rdData)  ;
	pInfo->buildTime.month = S_buildMonth(rdData) ;
	pInfo->buildTime.day   = S_buildDay(rdData)   ;
	pInfo->buildTime.hour  = S_buildHour(rdData)  ;
	pInfo->buildTime.min   = S_buildMin(rdData)   ;
	pInfo->buildTime.sec   = S_buildSec(rdData)   ;
#endif
}

asynStatus timingAsynEpics::readOctet(asynUser *pasynUser, char *value, size_t maxChars, size_t *nActual, int *eomReason)
{
    int function = pasynUser->reason;
    int addr=0;
    asynStatus status = asynSuccess;
    epicsTimeStamp timeStamp; getTimeStamp(&timeStamp);
    static const char *functionName = "readOctet";
   
    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);
    /* We just read the current value of the parameter from the parameter library.
     * Those values are updated whenever anything could cause them to change */
    status = (asynStatus)getStringParam(addr, function, (int)maxChars, value);

    /* Set the timestamp */
    pasynUser->timestamp = timeStamp;
	if(function == P_BuildTime)
	{
		printf("Set Build Time\n");
		unsigned int rdData;
		//ts2ip_rd(gpSys->ip.ev.fd, A_buildTime, (unsigned int*)&rdData);
		regmap = regmaptable[function];
		ts2ip_rd(gpSys->ip.ev.fd, regmap.address, (unsigned int*)&rdData);

		char timebuf[20];
		sprintf(timebuf,"20%d/%d/%d %d:%d:%d", S_buildYear(rdData), S_buildMonth(rdData), S_buildDay(rdData), S_buildHour(rdData), S_buildMin(rdData), S_buildSec(rdData));
		strcpy(value, (char*)timebuf);

		printf("Buildtime:%s\n", timebuf);
		//setBuildTime();
	};

		
    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, value=%s", 
                  driverName, functionName, status, function, value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, value=%s\n", 
              driverName, functionName, function, value);
    if (eomReason) *eomReason = ASYN_EOM_END;
    *nActual = strlen(value)+1;

		status = (asynStatus) callParamCallbacks();

    return(status);
}

asynStatus timingAsynEpics::writeOctet(asynUser *pasynUser, const char *value, size_t maxChars, size_t *nActual)
{
    int function = pasynUser->reason;
	asynStatus status = asynSuccess;
    //const char* functionName = "writeOctet";

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setStringParam(function, value);


    if (function == P_Starttime) {
			/* Make sure the update time is valid. If not change it and put back in parameter library */
			char svalue[maxChars] = "";
			getStringParam(P_Starttime, maxChars, svalue);
			//printf("writeOctet*****:%s(%ld)\n", svalue, maxChars);
		}

    status = (asynStatus) callParamCallbacks();
		return (status);
}

asynStatus timingAsynEpics::writeFloat64(asynUser *pasynUser, epicsFloat64 value)
{
    int function = pasynUser->reason;
    asynStatus status = asynSuccess;
    int run;
    int addr=0;
    const char *paramName;
    const char* functionName = "writeFloat64";

    /* Set the parameter in the parameter library. */
    status = (asynStatus) setDoubleParam(function, value);

    status = getAddress(pasynUser, &addr); if (status != asynSuccess) return(status);

    /* Fetch the parameter string name for possible use in debugging */
    getParamName(function, &paramName);
	int l_fd =gpSys->ip.ev.fd;

    if (function == P_UpdateTime) {
        /* Make sure the update time is valid. If not change it and put back in parameter library */
        if (value < MIN_UPDATE_TIME) {
            asynPrint(pasynUser, ASYN_TRACE_WARNING,
                "%s:%s: warning, update time too small, changed from %f to %f\n", 
                driverName, functionName, value, MIN_UPDATE_TIME);
            value = MIN_UPDATE_TIME;
            setDoubleParam(P_UpdateTime, value);
        }
        /* If the update time has changed and we are running then wake up the userProcess task */
        getIntegerParam(P_Run, &run);
        if (run) epicsEventSignal(eventId_);
    } 
	else if (function == P_Firmware) {
		getDoubleParam(P_Firmware, &value);
		pregmap->SetFirmware(value);
	}
	else if (function == P_Software) {
		getDoubleParam(P_Software, &value);
		pregmap->SetSoftware(value);
	}
	else if (function == P_MUX_Couter_A)
	{
		//ts2ip_wr(l_fd, A_mxc14_prescalerReg_00 + mxcN*4, freq );
		getDoubleParam(P_MUX_Couter_A, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}
	else if (function == P_MUX_Couter_B)
	{
		getDoubleParam(P_MUX_Couter_B, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}
	else if (function == P_MUX_Couter_C)
	{
		getDoubleParam(P_MUX_Couter_C, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}
	else if (function == P_MUX_Couter_D)
	{
		getDoubleParam(P_MUX_Couter_D, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}
	else if (function == P_MUX_Couter_E)
	{
		getDoubleParam(P_MUX_Couter_E, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}
	else if (function == P_MUX_Couter_F)
	{
		getDoubleParam(P_MUX_Couter_F, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}
	else if (function == P_MUX_Couter_G)
	{
		getDoubleParam(P_MUX_Couter_G, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}
	else if (function == P_MUX_Couter_H)
	{
		getDoubleParam(P_MUX_Couter_H, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}
	else if (function == P_MUX_Couter_I)
	{
		getDoubleParam(P_MUX_Couter_I, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}
	else if (function == P_MUX_Couter_J)
	{
		getDoubleParam(P_MUX_Couter_J, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}
	else if (function == P_MUX_Couter_K)
	{
		getDoubleParam(P_MUX_Couter_K, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}
	else if (function == P_MUX_Couter_L)
	{
		getDoubleParam(P_MUX_Couter_L, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}
	else if (function == P_MUX_Couter_M)
	{
		getDoubleParam(P_MUX_Couter_M, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}
	else if (function == P_MUX_Couter_N)
	{
		getDoubleParam(P_MUX_Couter_N, &value);
		regmap = regmaptable[function];
		ts2ip_wr(l_fd, regmap.address, value );
	}

		else {
        /* All other parameters just get set in parameter list, no need to
         * act on them here */
    }
    
    /* Do callbacks so higher layers see any changes */
    status = (asynStatus) callParamCallbacks();
    
    if (status) 
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize, 
                  "%s:%s: status=%d, function=%d, name=%s, value=%f", 
                  driverName, functionName, status, function, paramName, value);
    else        
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, 
              "%s:%s: function=%d, name=%s, value=%f\n", 
              driverName, functionName, function, paramName, value);
    return status;
}
#if 0
void timingAsynEpics::userProcess()
{
    int err;
    double sleepTime_s;
    struct timespec vartime;
    long time_elapsed_nanos;

    /* Loop forever */    
    while (1) { 
        if (scanRateHz!=0) {
            sleepTime_s = 1.0 / (double)scanRateHz;

						setDoubleParam(P_ReadLatencyUs, time_elapsed_nanos/1000.0); 
						callParamCallbacks();

						//doCallbacksInt16Array(daq->sample, daq->nb_sample, P_Waveform1 + i, 0);
            epicsThreadSleep(sleepTime_s);
        } else {
            epicsThreadSleep(1);
        }
    }
}
#else
void timingAsynEpics::userProcess()
{
    double updateTime;
    //struct timespec vartime;
    //long time_elapsed_nanos;
		int run;

		lock();

    /* Loop forever 
		 * If the logic will be able to block into scan thread in IOC, this thread should work in IOC.
		*/    

    while (1) { 
			getIntegerParam(P_Run, &run);
			getDoubleParam(P_UpdateTime, &updateTime);
			
			printf("Run:%d, Updatetime:%f\n",run, updateTime);

			/* Asynchronous Logic */
	
			
			unlock();
			if(run)	epicsEventWaitWithTimeout(eventId_, updateTime);
			else		epicsEventWait(eventId_);
			lock();

			getIntegerParam(P_Run, &run);
			if(!run)		continue;
			//
			
    }
}

#endif

void userProcess(void *drvPvt)
{
		timingAsynEpics *pPvt = (timingAsynEpics *)drvPvt;

    if (pPvt->clientThreadMode==10) 
		{
        fprintf(stderr, "start snapshot user task (mode real samples)\n");
        pPvt->userProcess();
    }
}

extern "C" {

/** EPICS iocsh callable function to call constructor for the testAsynPortDriver class.
  * \param[in] portName The name of the asyn port driver to be created.
  * \param[in] maxSizeSnapshot The maximum  number of sample in one snapshot
  * \param[in] maxNbSnapshot The number of snapshot buffered
 */
epicsShareFunc int timingAsynEpicsConfigure(const char *portName, int maxSizeSnapshot, int maxNbSnapshot, int clientMode, const char* registerfile,const char *deviceName)
{
    new timingAsynEpics(portName, maxSizeSnapshot, maxNbSnapshot, clientMode,registerfile, deviceName);
    return(asynSuccess);
}


/* EPICS iocsh shell commands */
static const iocshArg initArg0 = { "portName"          ,iocshArgString};
static const iocshArg initArg1 = { "max size snapshot" ,iocshArgInt};
static const iocshArg initArg2 = { "max size buffer MB",iocshArgInt};
static const iocshArg initArg3 = { "client mode"       ,iocshArgInt};
static const iocshArg initArg4 = { "register file name",iocshArgString};
static const iocshArg initArg5 = { "device name"       ,iocshArgString};
static const iocshArg * const initArgs[] = {&initArg0,
                                            &initArg1,
                                            &initArg2,
                                            &initArg3,
                                            &initArg4,
                                            &initArg5};
static const iocshFuncDef initFuncDef = {"timingAsynEpicsConfigure",6,initArgs};
static void initCallFunc(const iocshArgBuf *args)
{
    timingAsynEpicsConfigure(args[0].sval, args[1].ival, args[2].ival, args[3].ival, args[4].sval, args[5].sval);
}

void timingAsynEpicsRegister(void)
{
    iocshRegister(&initFuncDef,initCallFunc);
}

epicsExportRegistrar(timingAsynEpicsRegister);
}//end extern "C"
