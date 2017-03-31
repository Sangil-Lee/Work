/*drvSisfadc.h */

/* Author:   Marty Kraimer */
/* Date:     213NOV2001     */

/*************************************************************************
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
* National Laboratory, and the Regents of the University of California, as
* Operator of Los Alamos National Laboratory. EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
*************************************************************************/

#ifndef drvUndulatorH
#define drvUndulatorH

#ifdef __cplusplus
extern "C" {
#endif

#include "typedefined.h"

#define		LLRF_CHANNEL		3
#define		LLRF_RAW_SIZE		112
#define		TEMPERATURE_DEV		3
#define		MEAN_READ_DATA		7

typedef enum {
	RUN_NORMAL = 0,
	RUN_GRAPH,
	RUN_STOP,
	MAX_RUN
} RUN_MODE;

typedef enum {
	CMD_LIVE_DATA = 0,
	CMD_REQ_INIT,
	CMD_ACK_INIT,
	CMD_ATTR,
	MAX_CMD
} LLRF_CMD;

typedef enum {
	ATTR_RUN_MODE = 0,
	ATTR_RMS_SAMPLE,
	ATTR_AMP_OFFSET1_ON,
	ATTR_AMP_OFFSET2_ON,
	ATTR_AMP_OFFSET3_ON,
	ATTR_PAC_AMPLITUDE,
	ATTR_PAC_PHASE,
	ATTR_PAC_OFFSET,
	ATTR_PHASE_FB,
	ATTR_AMPLITUDE_FB,
	ATTR_PKP,
	ATTR_AKP,
	ATTR_PKI,
	ATTR_AKI,	
	ATTR_PKD,
	ATTR_AKD,		
	ATTR_MAX_AMPLITUDE1,
	ATTR_MAX_AMPLITUDE2,
	ATTR_AUDIO_FREQ,
	ATTR_AUDIO_MUTE,
	ATTR_TRIGGER_SYNCPOL,
	ATTR_TRIGGER_ONTIME,
	ATTR_STEP_SIZE_AMPLITUDE,
	ATTR_STEP_SIZE_PHASE,
	ATTR_TIME_INTERVAL,	
	ATTR_AMP_HIGH_LIMIT,
	ATTR_TRIGGER_IN,
	ATTR_TRIGGER_OUT,
	ATTR_TRIGGER_FREQ,
	ATTR_POWERON,
	ATTR_REFRESH,
	ATTR_STEPMODE_AMP,
	ATTR_STEPMODE_PHASE,
	ATTR_IP,
	ATTR_SUBNET,
	ATTR_GW,
	ATTR_IOC,
	/*modified by hugh */
	ATTR_AMP_ON1,
	ATTR_AMP_ON2,
	ATTR_AMP_ON3,
	ATTR_FB_CH,
	ATTR_CLK_SEL,
	ATTR_AMP_OFFSET1_OFF,
	ATTR_AMP_OFFSET2_OFF,
	ATTR_AMP_OFFSET3_OFF,
	ATTR_AMP_LOSS1,
	ATTR_AMP_LOSS2,
	ATTR_AMP_LOSS3,
	MAX_ATTR
} LLRF_ATTR;

typedef enum {
	INDEX_AMP_MEAN = 0,
	INDEX_PHASE_MEAN,
	INDEX_RMS_AMP_SDEV,
	INDEX_RMS_PHASE_DIFF_SDEV,
	INDEX_RMS_PHASE_ORG_SDEV,
	INDEX_RMS_AMP_MEAN,
	INDEX_RMS_PHASE_MEAN,
	MAX_INDEX
} DATA_INDEX;

typedef struct _LLRF_CTRL_INFO_ {
    uint32	cmd;
    uint32	card_id;
    uint32	tag;
    int32	value[MAX_ATTR];
} LLRF_CTRL_INFO;

typedef struct _LLRF_RAW_INFO_ {
    uint32	cmd;
    uint32	card_id;
    uint32 	tag;
    double 	temper[TEMPERATURE_DEV];
	double	data[LLRF_CHANNEL][MEAN_READ_DATA];
    uint16 	raw[LLRF_CHANNEL][LLRF_RAW_SIZE];
} LLRF_RAW_INFO;

typedef struct _LLRF_MEAN_INFO_ {
    uint32	cmd;
    uint32	card_id;
    uint32 	tag;
    double 	temper[TEMPERATURE_DEV];
    double 	data[LLRF_CHANNEL][MEAN_READ_DATA];	
     double stepAmp[1];
     double stepPhase[1];
	 /*modified by hufh */	 
	 double outputDBM;
	uint16 powerState[LLRF_CHANNEL];
	uint16 adc_detect[LLRF_CHANNEL];
	uint16 amp_range[LLRF_CHANNEL];
} LLRF_MEAN_INFO;

typedef struct _LLRF_HEADER_ {
	uint32 magic;
	uint32 maxsize;
	uint32 curloc;
	uint32 cursize;
}LLRF_HEADER;

typedef struct _llrfInfo_ {
    ELLNODE     node;
    int         card;
    char        *name;
    gtrhandler usrIH;
    void        *handlerPvt;
    void        *userPvt;
	int32		iRunMode;
	int32		iRmsSample;
	int32		iAmpOffset[9];
	int32		iPacAmplitude;
	int32		iPacPhase;
	int32		iPacOffset;
	int32		bPhaseFB;
	int32		bAmplitudeFB;
	int32		iPKp;
	int32		iAKp;
	int32		iPKi;
	int32		iAKi;
	int32		iPKd;
	int32		iAKd;	
	int32		iMaxAmplitude[2];
	int32		iAudioFreq;
	int32		bAudioMute;
	int32		bTriggerSynPol;
	int32		iTriggerOnTime;
	int32		istepSizeAmp;	
	int32		istepSizePhase;		
	int32		itimeInterval;
	int32		iAmpHighLimit;
	int32		bstepModAmp;	
	int32		bstepModPhase;	
	int32		iTriggerIn;
	int32		iTriggerOut;
	int32		iTriggerFreq;
	int32		iPowerOn;
	int32		iReFresh;
	/* modified by hugh */
	int32		bAmpOn[3];
	int32		bFBCh;
	int32		iClockSelect;
	int32		bPowerSwitch;
	
	char		ipaddr[16];
	char 		subnet[16];
	char		gateway[16];
	char		iocserver[16];
} llrfInfo;


int llrfConfig(int card, const char *devIP, unsigned short nPort, int recvTimeout);

#ifdef __cplusplus
}
#endif

#endif /*drvUndulatorH*/
