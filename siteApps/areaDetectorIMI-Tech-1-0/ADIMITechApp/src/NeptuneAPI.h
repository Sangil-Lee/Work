/*
 * NeptuneAPI.h
 *
 *  Created on: Jan 28, 2019
 *      Author: root
 */

#ifndef NEPTUNEAPI_H_
#define NEPTUNEAPI_H_


#define		MAX_VENDOR		32
#define		MAX_MODEL		32
#define		MAX_ID			64
#define 	MAX_NODE_LIST_COUNT				128	// for XML tree node
#define 	MAX_ENUM_LIST_COUNT				64	// for enumeration
#define 	MAX_XML_NODE_STRING_LENGTH		256
#define 	MAX_STRING_LENGTH				512

// typedef
typedef void * NCamHandle;


// enum
typedef enum {
	eNeptuneSuccess = 0,
	eNeptuneInvalidParameter = 1,
	eNeptuneCameraNotOpen = 2,
	eNepuneCameraOpenFail = 3,
	eNeptuneCreateStreamFail = 4,
	eNeptuneAcqNotStart = 5,
	eNeptuneAcqRunning = 6,
	eNeptuneFeatureNotAvailable = 7,
	eNeptuneFeatureNotImplement = 8,
	eNeptuneFeatureLock = 9,
	eNeptuneInvalidFormat = 10,
	eNeptuneNoFrameImage = 11,
	eNeptuneUnknownError = 99,
}NErr;

typedef enum {
	eNeptuneBayerNone = 0,
	eNeptuneBayerNearest = 1,
	eNeptuneBayerBilinear = 2,
}NBayerParam;

typedef enum _tagENeptuneNodeType {
	NEPTUNE_NODE_TYPE_UKNOWN		= -1,
	NEPTUNE_NODE_TYPE_CATEGORY		= 0,
	NEPTUNE_NODE_TYPE_COMMAND,				// command type node
	NEPTUNE_NODE_TYPE_RAW,					// raw node
	NEPTUNE_NODE_TYPE_STRING,				// string node
	NEPTUNE_NODE_TYPE_ENUM,					// enumeration node
	NEPTUNE_NODE_TYPE_INT,					// int type node
	NEPTUNE_NODE_TYPE_FLOAT,				// float type node
	NEPTUNE_NODE_TYPE_BOOLEAN				// boolean type node
} ENeptuneNodeType;
//////////////////////////////////////////////////////////////////////////
typedef enum _tagENeptuneNodeAccessMode {
	NEPTUNE_NODE_ACCESSMODE_NI			= 0,		// Not Implemented
	NEPTUNE_NODE_ACCESSMODE_NA			= 1,		// Not Available
	NEPTUNE_NODE_ACCESSMODE_WO			= 2,		// Write Only
	NEPTUNE_NODE_ACCESSMODE_RO			= 3,		// Read Only
	NEPTUNE_NODE_ACCESSMODE_RW			= 4,		// Read, Write
	NEPTUNE_NODE_ACCESSMODE_UNDEFINED	= 5			// undefined
} ENeptuneNodeAccessMode;
//////////////////////////////////////////////////////////////////////////
typedef enum _tagENeptuneNodeVisibility {
	NEPTUNE_NODE_VISIBLE_UNKNOWN		= -1,
	NEPTUNE_NODE_VISIBLE_BEGINNER		= 0,		// beginner
	NEPTUNE_NODE_VISIBLE_EXPERT			= 1,		// expert
	NEPTUNE_NODE_VISIBLE_GURU			= 2			// guru
} ENeptuneNodeVisibility;

// struct
typedef struct
{
    char strVendor[MAX_VENDOR];
    char strModel[MAX_MODEL];
    char strDevcieID[MAX_ID];

    int pixelformat;
    int gain;
    double exposure;
}NCamInfo, *PNCamInfo;

typedef struct
{
	int x;
	int y;
	int width;
	int height;
	int maxWidth;
	int maxHeight;
}NSizeInfo, *PNSizeInfo;

typedef struct {
    unsigned int 	uiImgBufSize;

    unsigned int	uiWidth;
    unsigned int	uiHeight;
    unsigned int    uiBpp;
    unsigned char*	pRawBuf;
    unsigned char*	pRGBBuf;
    unsigned char*	pRaw16To8Buf;
} NImageFrame, *PNImageFrame;

#define		CALI_SIGN_DEF		0x4D49

typedef struct _CALI_INFO
{
	unsigned short sign; // 0x4D49 'IM' ascii
	unsigned short vers;
	unsigned short headersize;
	unsigned short bpp;
	unsigned int width;
	unsigned int height;
}CALI_INFO, *PCALI_INFO;

typedef struct _tagNEPTUNE_XML_NODE_LIST {
	unsigned int	nCount;												// number of nodes
	char			pstrList[MAX_NODE_LIST_COUNT][MAX_XML_NODE_STRING_LENGTH];	// node list

	_tagNEPTUNE_XML_NODE_LIST() {
		nCount = MAX_NODE_LIST_COUNT;
	}
} NEPTUNE_XML_NODE_LIST, *PNEPTUNE_XML_NODE_LIST;

typedef struct _tagNEPTUNE_XML_ENUM_LIST {
	unsigned int		nCount;												// number of enumeration node string
	char				pstrList[MAX_ENUM_LIST_COUNT][MAX_XML_NODE_STRING_LENGTH];		// enumeration node string list
	unsigned int		nIndex;												// current index of the string list

	_tagNEPTUNE_XML_ENUM_LIST() {
		nCount = MAX_ENUM_LIST_COUNT;
		nIndex = 0;
	}
} NEPTUNE_XML_ENUM_LIST, *PNEPTUNE_XML_ENUM_LIST;
//////////////////////////////////////////////////////////////////////////
typedef struct _tagNEPTUNE_XML_NODE_INFO {
	ENeptuneNodeType			Type;			// node type(int, float, boolean, string, enumeration, command)
	ENeptuneNodeAccessMode		AccessMode;		// access mode
	bool						bHasChild;	// has child ?
	char						strDisplayName[MAX_STRING_LENGTH];	// node name
	char						strTooltip[MAX_STRING_LENGTH];		// node tooltip
	char						strDescription[MAX_STRING_LENGTH];	// node description

	_tagNEPTUNE_XML_NODE_INFO() {
		Type = NEPTUNE_NODE_TYPE_UKNOWN;
		AccessMode = NEPTUNE_NODE_ACCESSMODE_RO;
		bHasChild = false;
	}
} NEPTUNE_XML_NODE_INFO, *PNEPTUNE_XML_NODE_INFO;
//////////////////////////////////////////////////////////////////////////
typedef struct _tagNEPTUNE_XML_INT_VALUE_INFO {
	int					nValue;				// current value
	int					nMin;				// minimum value supported
	int					nMax;				// maximum value supported
	int					nInc;				// increment step

	_tagNEPTUNE_XML_INT_VALUE_INFO() {
		nValue = 0;
		nMin = 0;
		nMax = 0;
		nInc = 0;
	}
} NEPTUNE_XML_INT_VALUE_INFO, *PNEPTUNE_XML_INT_VALUE_INFO;
//////////////////////////////////////////////////////////////////////////
typedef struct _tagNEPTUNE_XML_FLOAT_VALUE_INFO {
	double					dValue;			// current value
	double					dMin;			// minimum
	double					dMax;			// maximum
	double					dInc;			// increment

	_tagNEPTUNE_XML_FLOAT_VALUE_INFO() {
		dValue = 0;
		dMin = 0;
		dMax = 0;
		dInc = 0.001;
	}
} NEPTUNE_XML_FLOAT_VALUE_INFO, *PNEPTUNE_XML_FLOAT_VALUE_INFO;


typedef void (*NFrameCallback)(PNImageFrame pImgFrame);

#ifdef __cplusplus
extern "C"
{
#endif

	//////////////////////////////////////////////////////////
	//		C API interface Functions
	// init/uninit
	NErr	NeptuneInit();
	NErr	NeptuneUninit();

	NErr	NeptuneNumberOfCameras(int *pnNumbers);
	NErr	NeptuneGetDisplayName(int idx, char* strDisplayName);
	NErr	NeptuneGetCameraID(int idx, char* strCamID);

	NErr	NeptuneOpenCamera(char *strID, NCamHandle *pHandle);
	NErr	NeptuneCloseCamera(NCamHandle handle);

	NErr	NeptuneReadRegister(NCamHandle handle, int address, int *data);
	NErr	NeptuneWriteRegiser(NCamHandle handle, int address, int data);

	NErr	NeptuneGetCamInfo(NCamHandle handle, NCamInfo *pInfo);

	// size
	NErr	NeptuneGetSizeInfo(NCamHandle handle, NSizeInfo *pInfo);
	NErr	NeptuneSetSizeInfo(NCamHandle handle, NSizeInfo info);

	// pixel format
    NErr	NeptuneGetNumAvailablePixelFormats(NCamHandle handle, int *pnNumbers);
    NErr	NeptuneGetAvailablePixelFormatsIdxToStr(NCamHandle handle, int index, char *strPixelformat);
    NErr	NeptuneGetPixelFormatStr(NCamHandle handle, char *strPixelformat);
    NErr	NeptuneSetPixelFormatStr(NCamHandle handle, char *strPixelformat);

    // trigger
    NErr	NeptuneGetTriggerEnable(NCamHandle handle, bool *pbEnabled);
    NErr	NeptuneSetTriggerEnable(NCamHandle handle, bool bEnabled);

    NErr	NeptuneGetTriggerParameter(NCamHandle handle, int *pnParam);
    NErr	NeptuneSetTriggerParameter(NCamHandle handle, int nParam);

    NErr	NeptuneGetNumAvailableTriggerModes(NCamHandle handle, int *pnNumbers);
    NErr	NeptuneGetAvailableTriggerModesIdxToStr(NCamHandle handle, int index, char *strMode);
    NErr	NeptuneGetTriggerModeStr(NCamHandle handle, char *strMode);
    NErr	NeptuneSetTriggerModeStr(NCamHandle handle, char *strMode);

    NErr	NeptuneGetNumAvailableTriggerPolaritys(NCamHandle handle, int *pnNumbers);
    NErr	NeptuneGetAvailableTriggerPolaritysIdxToStr(NCamHandle handle, int index, char *strPolarity);
    NErr	NeptuneGetTriggerPolarityStr(NCamHandle handle, char *strPolarity);
    NErr	NeptuneSetTriggerPolarityStr(NCamHandle handle, char *strPolarity);

    NErr	NeptuneGetNumAvailableTriggerSources(NCamHandle handle, int *pnNumbers);
    NErr	NeptuneGetAvailableTriggerSourcesIdxToStr(NCamHandle handle, int index, char *strSource);
    NErr	NeptuneGetTriggerSourceStr(NCamHandle handle, char *strSource);
    NErr	NeptuneSetTriggerSourceStr(NCamHandle handle, char *strSource);

    NErr	NeptuneRunSoftwareTrigger(NCamHandle handle);

    NErr	NeptuneGetTriggerDelay(NCamHandle handle, int *pnValue);
    NErr 	NeptuneSetTriggerDelay(NCamHandle handle, int nValue);
    NErr 	NeptuneGetTriggerNoiseFilter(NCamHandle handle, int *pnValue);
    NErr 	NeptuneSetTriggerNoiseFilter(NCamHandle handle, int nValue);

    // strobe
    NErr 	NeptuneGetStrobeEnable(NCamHandle handle, bool *pbEnabled);
    NErr 	NeptuneSetStrobeEnable(NCamHandle handle, bool bEnabled);

    NErr 	NeptuneGetNumAvailableStrobeModes(NCamHandle handle, int *pnNumbers);
    NErr 	NeptuneGetAvailableStrobeModesIdxToStr(NCamHandle handle, int index, char *strMode);
    NErr 	NeptuneGetStrobeModeStr(NCamHandle handle, char *strMode);
    NErr 	NeptuneSetStrobeModeStr(NCamHandle handle, char *strMode);

    NErr 	NeptuneGetNumAvailableStrobePolaritys(NCamHandle handle, int *pnNumbers);
    NErr 	NeptuneGetAvailableStrobePolaritysIdxToStr(NCamHandle handle, int index, char *strPolarity);
    NErr 	NeptuneGetStrobePolarityStr(NCamHandle handle, char *strPolarity);
    NErr 	NeptuneSetStrobePolarityStr(NCamHandle handle, char *strPolarity);

    NErr 	NeptuneGetStrobeDelay(NCamHandle handle, int *pnValue);
    NErr 	NeptuneSetStrobeDelay(NCamHandle handle, int nValue);
    NErr 	NeptuneGetStrobeDuration(NCamHandle handle, int *pnValue);
    NErr 	NeptuneSetStrobeDuration(NCamHandle handle, int nValue);

    // feature control
    NErr 	NeptuneGetGammaMinMax(NCamHandle handle, float *pfMin, float *pfMax);
    NErr 	NeptuneGetGamma(NCamHandle handle, float *pfValue);
    NErr  	NeptuneSetGamma(NCamHandle handle, float fValue);
    NErr  	NeptuneGetGainMinMax(NCamHandle handle, int *pnMin, int *pnMax);
    NErr  	NeptuneGetGain(NCamHandle handle, int *pnValue, bool *pbAuto);
    NErr  	NeptuneSetGain(NCamHandle handle, int nValue, bool bAuto);
    NErr  	NeptuneGetRGainMinMax(NCamHandle handle, int *pnMin, int *pnMax);
    NErr  	NeptuneGetRGain(NCamHandle handle, int *pnValue);
    NErr  	NeptuneSetRGain(NCamHandle handle, int nValue);
    NErr  	NeptuneGetGGainMinMax(NCamHandle handle, int *pnMin, int *pnMax);
    NErr  	NeptuneGetGGain(NCamHandle handle, int *pnValue);
    NErr  	NeptuneSetGGain(NCamHandle handle, int nValue);
    NErr  	NeptuneGetBGainMinMax(NCamHandle handle, int *pnMin, int *pnMax);
    NErr  	NeptuneGetBGain(NCamHandle handle, int *pnValue);
    NErr  	NeptuneSetBGain(NCamHandle handle, int nValue);
    NErr  	NeptuneGetBlackLevelMinMax(NCamHandle handle, int *pnMin, int *pnMax);
    NErr  	NeptuneGetBlackLevel(NCamHandle handle, int *pnValue);
    NErr  	NeptuneSetBlackLevel(NCamHandle handle, int nValue);
    NErr  	NeptuneGetContrastMinMax(NCamHandle handle, int *pnMin, int *pnMax);
    NErr  	NeptuneGetContrast(NCamHandle handle, int *pnValue);
    NErr  	NeptuneSetContrast(NCamHandle handle, int nValue);
    NErr  	NeptuneGetHueMinMax(NCamHandle handle, int *pnMin, int *pnMax);
    NErr  	NeptuneGetHue(NCamHandle handle, int *pnValue);
    NErr  	NeptuneSetHue(NCamHandle handle, int nValue);
    NErr  	NeptuneGetSaturationMinMax(NCamHandle handle, int *pnMin, int *pnMax);
    NErr  	NeptuneGetSaturation(NCamHandle handle, int *pnValue);
    NErr  	NeptuneSetSaturation(NCamHandle handle, int nValue);
    NErr  	NeptuneGetSharpnessMinMax(NCamHandle handle, int *pnMin, int *pnMax);
    NErr  	NeptuneGetSharpness(NCamHandle handle, int *pnValue);
    NErr  	NeptuneSetSharpness(NCamHandle handle, int nValue);
    NErr  	NeptuneGetShutterMinMax(NCamHandle handle, unsigned int *pnMin,unsigned int *pnMax);
    NErr  	NeptuneGetShutter(NCamHandle handle, unsigned int *pnValue, bool *pbAuto);
    NErr  	NeptuneSetShutter(NCamHandle handle, unsigned int nValue, bool bAuto);
    NErr  	NeptuneGetAutoExposureMinMax(NCamHandle handle, int *pnMin, int *pnMax);
    NErr  	NeptuneGetAutoExposure(NCamHandle handle, int *pnValue);
    NErr  	NeptuneSetAutoExposure(NCamHandle handle, int nValue);

    NErr  	NeptuneGetNumAvailableWhiteBalances(NCamHandle handle, int *pnNumbers);
    NErr  	NeptuneGetAvailableWhiteBalancesIdxToStr(NCamHandle handle, int index, char *strMode);
    NErr  	NeptuneGetWhiteBalanceStr(NCamHandle handle, char *strMode);
    NErr  	NeptuneSetWhiteBalanceStr(NCamHandle handle, char *strMode);

    NErr 	NeptuneSetUserDefault(NCamHandle handle);

    // bayer conversion
    NErr	NeptuneSetBayerConversion(NCamHandle handle, NBayerParam param);

    // acquisition
	NErr	NeptuneStartAcquisition(NCamHandle handle, NFrameCallback callback);
	NErr	NeptuneStopAcquisition(NCamHandle handle);

	// grab
	NErr	NeptuneGrabImage(NCamHandle handle, NImageFrame *pData);

	// optional
	NErr	NeptuneGetFrameRateMinMax(NCamHandle handle, float *pfMin, float *pfMax);
	NErr	NeptuneGetFrameRate(NCamHandle handle, float *pfValue);
	NErr	NeptuneSetFrameRate(NCamHandle handle, float fValue);
	NErr	NeptuneCameraReset(NCamHandle handle);
	NErr	NeptuneSetCalibrationParameter(NCamHandle handle, bool bApply, char *filepath);

	// xml control
	NErr	NeptuneGetNodeList(NCamHandle handle, const char *strParentNodeName, PNEPTUNE_XML_NODE_LIST pNodeInfoList);
	NErr	NeptuneGetNodeListChar(NCamHandle handle, const char *strParentNodeName, char *pBuffer);
	NErr	NeptuneGetNodeInfo(NCamHandle handle, const char *strNodeName, PNEPTUNE_XML_NODE_INFO pNodeInfo);
	NErr	NeptuneGetNodeInt(NCamHandle handle, const char *strNodeName, PNEPTUNE_XML_INT_VALUE_INFO pValueInfo);
	NErr	NeptuneSetNodeInt(NCamHandle handle, const char *strNodeName, int nValue);
	NErr	NeptuneGetNodeFloat(NCamHandle handle, const char *strNodeName, PNEPTUNE_XML_FLOAT_VALUE_INFO pValueInfo);
	NErr	NeptuneSetNodeFloat(NCamHandle handle, const char *strNodeName, double dbValue);
	NErr	NeptuneGetNodeEnum(NCamHandle handle, const char *strNodeName, PNEPTUNE_XML_ENUM_LIST pEnumList);
	NErr	NeptuneSetNodeEnum(NCamHandle handle, const char *strNodeName, const char *strValue);
	NErr	NeptuneGetNodeString(NCamHandle handle, const char *strNodeName, char *pBuffer);
	NErr	NeptuneSetNodeString(NCamHandle handle, const char *strNodeName, const char *strValue);
	NErr	NeptuneGetNodeBoolean(NCamHandle handle, const char *strNodeName, bool* pbState);
	NErr	NeptuneSetNodeBoolean(NCamHandle handle, const char *strNodeName, bool bState);
	NErr	NeptuneSetNodeCommand(NCamHandle handle, const char *strNodeName);


	///////////////////////////////////////////////////////////
	// typedefs for dynamic loading
#define 	DY_FNC(function)	typedef NErr( *function )
	DY_FNC (F_NeptuneInit					)(void);
	DY_FNC (F_NeptuneUninit					)(void);

	DY_FNC (F_NeptuneNumberOfCameras		)(int *pnNumbers);
	DY_FNC (F_NeptuneGetCameraID			)(int idx, char* strCamID);

	DY_FNC (F_NeptuneOpenCamera				)(char *strID, NCamHandle *pHandle);
	DY_FNC (F_NeptuneCloseCamera			)(NCamHandle handle);

	DY_FNC (F_NeptuneReadRegister			)(NCamHandle handle, int address, int *data);
	DY_FNC (F_NeptuneWriteRegiser			)(NCamHandle handle, int address, int data);

	DY_FNC (F_NeptuneGetCamInfo				)(NCamHandle handle, NCamInfo *pInfo);

	// size
	DY_FNC (F_NeptuneGetSizeInfo			)(NCamHandle handle, NSizeInfo *pInfo);
	DY_FNC (F_NeptuneSetSizeInfo			)(NCamHandle handle, NSizeInfo info);

	// pixel format
	DY_FNC (F_NeptuneGetNumAvailablePixelFormats		)(NCamHandle handle, int *pnNumbers);
	DY_FNC (F_NeptuneGetAvailablePixelFormatsIdxToStr	)(NCamHandle handle, int index, char *strPixelformat);
	DY_FNC (F_NeptuneGetPixelFormatStr					)(NCamHandle handle, char *strPixelformat);
	DY_FNC (F_NeptuneSetPixelFormatStr					)(NCamHandle handle, char *strPixelformat);

    // trigger
	DY_FNC (F_NeptuneGetTriggerEnable					)(NCamHandle handle, bool *pbEnabled);
	DY_FNC (F_NeptuneSetTriggerEnable					)(NCamHandle handle, bool bEnabled);

	DY_FNC (F_NeptuneGetTriggerParameter				)(NCamHandle handle, int *pnParam);
	DY_FNC (F_NeptuneSetTriggerParameter				)(NCamHandle handle, int nParam);

	DY_FNC (F_NeptuneGetNumAvailableTriggerModes		)(NCamHandle handle, int *pnNumbers);
	DY_FNC (F_NeptuneGetAvailableTriggerModesIdxToStr	)(NCamHandle handle, int index, char *strMode);
	DY_FNC (F_NeptuneGetTriggerModeStr					)(NCamHandle handle, char *strMode);
	DY_FNC (F_NeptuneSetTriggerModeStr					)(NCamHandle handle, char *strMode);

	DY_FNC (F_NeptuneGetNumAvailableTriggerPolaritys		)(NCamHandle handle, int *pnNumbers);
	DY_FNC (F_NeptuneGetAvailableTriggerPolaritysIdxToStr	)(NCamHandle handle, int index, char *strPolarity);
	DY_FNC (F_NeptuneGetTriggerPolarityStr					)(NCamHandle handle, char *strPolarity);
	DY_FNC (F_NeptuneSetTriggerPolarityStr					)(NCamHandle handle, char *strPolarity);

	DY_FNC (F_NeptuneGetNumAvailableTriggerSources			)(NCamHandle handle, int *pnNumbers);
	DY_FNC (F_NeptuneGetAvailableTriggerSourcesIdxToStr		)(NCamHandle handle, int index, char *strSource);
	DY_FNC (F_NeptuneGetTriggerSourceStr					)(NCamHandle handle, char *strSource);
	DY_FNC (F_NeptuneSetTriggerSourceStr					)(NCamHandle handle, char *strSource);

	DY_FNC (F_NeptuneRunSoftwareTrigger						)(NCamHandle handle);

	DY_FNC (F_NeptuneGetTriggerDelay						)(NCamHandle handle, int *pnValue);
	DY_FNC (F_NeptuneSetTriggerDelay						)(NCamHandle handle, int nValue);
	DY_FNC (F_NeptuneGetTriggerNoiseFilter					)(NCamHandle handle, int *pnValue);
	DY_FNC (F_NeptuneSetTriggerNoiseFilter					)(NCamHandle handle, int nValue);

    // strobe
	DY_FNC (F_NeptuneGetStrobeEnable						)(NCamHandle handle, bool *pbEnabled);
	DY_FNC (F_NeptuneSetStrobeEnable						)(NCamHandle handle, bool bEnabled);

	DY_FNC (F_NeptuneGetNumAvailableStrobeModes				)(NCamHandle handle, int *pnNumbers);
	DY_FNC (F_NeptuneGetAvailableStrobeModesIdxToStr		)(NCamHandle handle, int index, char *strMode);
	DY_FNC (F_NeptuneGetStrobeModeStr						)(NCamHandle handle, char *strMode);
	DY_FNC (F_NeptuneSetStrobeModeStr						)(NCamHandle handle, char *strMode);

	DY_FNC (F_NeptuneGetNumAvailableStrobePolaritys			)(NCamHandle handle, int *pnNumbers);
	DY_FNC (F_NeptuneGetAvailableStrobePolaritysIdxToStr	)(NCamHandle handle, int index, char *strPolarity);
	DY_FNC (F_NeptuneGetStrobePolarityStr					)(NCamHandle handle, char *strPolarity);
	DY_FNC (F_NeptuneSetStrobePolarityStr					)(NCamHandle handle, char *strPolarity);

	DY_FNC (F_NeptuneGetStrobeDelay							)(NCamHandle handle, int *pnValue);
	DY_FNC (F_NeptuneSetStrobeDelay							)(NCamHandle handle, int nValue);
	DY_FNC (F_NeptuneGetStrobeDuration						)(NCamHandle handle, int *pnValue);
	DY_FNC (F_NeptuneSetStrobeDuration						)(NCamHandle handle, int nValue);

    // feature control
	DY_FNC (F_NSeptuneGetGamma								)(NCamHandle handle, float *pfValue);
	DY_FNC (F_NeptuneSetGamma								)(NCamHandle handle, float fValue);
	DY_FNC (F_NeptuneGetGain								)(NCamHandle handle, int *pnValue, bool *pbAuto);
	DY_FNC (F_NeptuneSetGain								)(NCamHandle handle, int nValue, bool bAuto);
	DY_FNC (F_NeptuneGetRGain								)(NCamHandle handle, int *pnValue);
	DY_FNC (F_NeptuneSetRGain								)(NCamHandle handle, int nValue);
	DY_FNC (F_NeptuneGetGGain								)(NCamHandle handle, int *pnValue);
	DY_FNC (F_NeptuneSetGGain								)(NCamHandle handle, int nValue);
	DY_FNC (F_NeptuneGetBGain								)(NCamHandle handle, int *pnValue);
	DY_FNC (F_NeptuneSetBGain								)(NCamHandle handle, int nValue);
	DY_FNC (F_NeptuneGetBlackLevel							)(NCamHandle handle, int *pnValue);
	DY_FNC (F_NeptuneSetBlackLevel							)(NCamHandle handle, int nValue);
	DY_FNC (F_NeptuneGetContrast							)(NCamHandle handle, int *pnValue);
	DY_FNC (F_NeptuneSetContrast							)(NCamHandle handle, int nValue);
	DY_FNC (F_NeptuneGetSaturation							)(NCamHandle handle, int *pnValue);
	DY_FNC (F_NeptuneSetSaturation							)(NCamHandle handle, int nValue);
	DY_FNC (F_NeptuneGetSharpness							)(NCamHandle handle, int *pnValue);
	DY_FNC (F_NeptuneSetSharpness							)(NCamHandle handle, int nValue);
	DY_FNC (F_NeptuneGetShutter								)(NCamHandle handle, int *pnValue, bool *pbAuto);
	DY_FNC (F_NeptuneSetShutter								)(NCamHandle handle, int nValue, bool bAuto);
	DY_FNC (F_NeptuneGetAutoExposure						)(NCamHandle handle, int *pnValue);
	DY_FNC (F_NeptuneSetAutoExposure						)(NCamHandle handle, int nValue);

	DY_FNC (F_NeptuneGetNumAvailableWhiteBalances			)(NCamHandle handle, int *pnNumbers);
	DY_FNC (F_NeptuneGetAvailableWhiteBalancesIdxToStr		)(NCamHandle handle, int index, char *strMode);
	DY_FNC (F_NeptuneGetWhiteBalanceStr						)(NCamHandle handle, char *strMode);
	DY_FNC (F_NeptuneSetWhiteBalanceStr						)(NCamHandle handle, char *strMode);

	DY_FNC (F_NeptuneSetUserDefault							)(NCamHandle handle);

    // bayer conversion
	DY_FNC (F_NeptuneSetBayerConversion						)(NCamHandle handle, NBayerParam param);

	// acquisition
	DY_FNC (F_NeptuneStartAcquisition						)(NCamHandle handle, NFrameCallback callback);
	DY_FNC (F_NeptuneStopAcquisition						)(NCamHandle handle);

	// optional
	DY_FNC (F_NeptuneSetCalibrationParameter				)(NCamHandle handle, char *filepath);
	DY_FNC (F_NeptuneCameraReset							)(NCamHandle handle);

#ifdef __cplusplus
}
#endif

#endif /* NEPTUNEAPI_H_ */
