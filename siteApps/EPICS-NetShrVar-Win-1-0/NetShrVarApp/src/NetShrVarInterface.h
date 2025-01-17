/*************************************************************************\ 
* Copyright (c) 2013 Science and Technology Facilities Council (STFC), GB. 
* All rights reverved. 
* This file is distributed subject to a Software License Agreement found 
* in the file LICENSE.txt that is included with this distribution. 
\*************************************************************************/ 

/// @file NetShrVarInterface.h header for #NetShrVarInterface class. 
/// @author Freddie Akeroyd, STFC ISIS Facility, GB

#ifndef NETSHRVAR_INTERFACE_H
#define NETSHRVAR_INTERFACE_H

#ifdef epicsExportSharedSymbols
#define NetShrVarSymbols
#undef epicsExportSharedSymbols
#include <shareLib.h>
#endif

#include <stdio.h>

#include <string>
#include <vector>
#include <map>
#include <list>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iostream>

#include <epicsMutex.h>
#include <epicsThread.h>
#include <epicsExit.h>
#include <macLib.h>
#include <asynDriver.h>

#include <cvirte.h>		
#include <userint.h>
#include <cvinetv.h>

#include "pugixml.hpp"

#ifdef NetShrVarSymbols
#undef NetShrVarSymbols
#define epicsExportSharedSymbols
#include <shareLib.h>
#endif

/// option argument in NetShrVarConfigure() of @link st.cmd @endlink not used at present
enum NetShrVarOptions { NVNothing = 0, NVSomething=1 };

struct NvItem;
class asynPortDriver;
struct CallbackData;


/// Manager class for the NetVar Interaction. Parses an @link netvarconfig.xml @endlink file and provides access to the 9variables described within. 
class epicsShareClass NetShrVarInterface
{
public:
	NetShrVarInterface(const char* configSection, const char *configFile, int options);
	size_t nParams();
	~NetShrVarInterface() { }
	void updateValues();
	void createParams(asynPortDriver* driver);
	void report(FILE* fp, int details);
	void readValue(const char* param);
	void dataTransferredCallback (void * handle, int error, CallbackData* cb_data);
	void dataCallback (void * handle, CNVData data, CallbackData* cb_data);
	void statusCallback (void * handle, CNVConnectionStatus status, int error, CallbackData* cb_data);
	template<typename T> void setValue(const char* param, const T& value);
	template<typename T> void setArrayValue(const char* param, const T* value, size_t nElements);
	template<typename T> void readArrayValue(const char* paramName, T* value, size_t nElements, size_t* nIn);
	static bool varExists(const std::string& path);
	static bool pathExists(const std::string& path);
  
private:
	std::string m_configSection;  ///< section of \a configFile to load information from
	std::string m_configFile;   
	int m_options; ///< the various #NetShrVarOptions currently in use
//	epicsMutex m_lock;
	asynPortDriver* m_driver;
	typedef std::map<std::string,NvItem*> params_t;
	params_t m_params;
	pugi::xml_document m_xmlconfig;
    MAC_HANDLE* m_mac_env;
	int m_writer_wait_ms; ///< how long to wait for a write operation to complete in milliseconds
	int m_b_writer_wait_ms; ///< how long to wait for a buffered write operation to complete in milliseconds
	
    template<typename T> void getAsynParamValue(int param, T& value);
    char* envExpand(const char *str);
	void getParams();
	void setValueCNV(const std::string& name, CNVData value);
	static void epicsExitFunc(void* arg);
	bool checkOption(NetShrVarOptions option) { return ( m_options & static_cast<int>(option) ) != 0; }
	void connectVars();
    bool convertTimeStamp(unsigned __int64 timestamp, epicsTimeStamp *epicsTS);
	template<typename T> void updateParamValue(int param_index, T val, epicsTimeStamp* epicsTS, bool do_asyn_param_callbacks);
	template<typename T> void updateParamArrayValue(int param_index, T* val, size_t nElements, epicsTimeStamp* epicsTS);
	void updateParamCNV (int param_index, CNVData data, bool do_asyn_param_callbacks);
	template<CNVDataType cnvType> void updateParamCNVImpl(int param_index, CNVData data, CNVDataType type, unsigned int nDims, bool do_asyn_param_callbacks);
	template<typename T,typename U> void updateParamArrayValueImpl(int param_index, T* val, size_t nElements);
	void readVarInit(NvItem* item);
    void setParamStatus(int param_id, asynStatus status, epicsAlarmCondition alarmStat = epicsAlarmNone, epicsAlarmSeverity alarmSevr = epicsSevNone);
	void getParamStatus(int param_id, asynStatus& status, int& alarmStat, int& alarmSevr);
    void initAsynParamIds();
	void updateConnectedAlarmStatus(const std::string& paramName, int value, const std::string& alarmStr, epicsAlarmCondition stat, epicsAlarmSeverity sevr);
};

#endif /* NETSHRVAR_INTERFACE_H */
