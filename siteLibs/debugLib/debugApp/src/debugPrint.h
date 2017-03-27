#ifndef  __EPICS_IOC_DEBUG_H
#define  __EPICS_IOC_DEBUG_H

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <time.h>
#include <string.h>
#include <epicsExport.h>
#include <epicsStdio.h>
#include <epicsTime.h>
#include <epicsPrint.h>
#include <epicsExit.h>
#include <epicsExport.h>
#include <epicsMutex.h>
#include <epicsThread.h>
#include <epicsEvent.h>
#include <epicsMessageQueue.h>
#include <iocsh.h>

#include <errlog.h>
#include <shareLib.h>
#include <ellLib.h>

#define FILE_PATH_NAME_LEN 128

extern int epicsDebugLevel;
typedef enum DebugLevel{ DEBUG_OFF	= 0,
	DEBUG_WARN=1,  DEBUG_ERR=2, DEBUG_INFO=3, DEBUG_DEBUG=4, DEBUG_TRACE=5, DEBUG_ALL=6
	} DebugLevel_t;

#ifdef __cplusplus
extern "C" {
#endif

epicsShareFunc int epicsShareAPI debugPrint(const char* file, const int line, const int debugLevel, const char *fmt, ...) 
							__attribute__((format(printf,4,5))); 

#define debug(fmt, args...) (!epicsDebugLevel)?0:debugPrint(__FILE__,__LINE__,fmt, ##args)

#ifdef __cplusplus
}
#endif
#endif
