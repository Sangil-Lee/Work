#ifndef  __EPICS_IOC_DEBUG_H
#define  __EPICS_IOC_DEBUG_H

#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <time.h>
#include <cstring>
#include <epicsExport.h>
#include <iocsh.h>

#ifndef __GNUC__
#define __attribute__(x)
#endif

#ifdef __cplusplus
extern "C" {
#endif

extern int epicsDebug;
extern int debugFile;
class DebugPrint
{
    const char* file;
    int line;
	FILE* outfile;
public:
    DebugPrint(const char* file, int line) : file(file), line(line), outfile(NULL){}
    DebugPrint(const char* file, int line, int defile) : file(file), line(line), outfile(NULL)
	{
		outfile = fopen("debugfile.txt","w");
	}
    ~DebugPrint(){ if(outfile!=NULL) fclose(outfile);}
    int Print(const char* fmt, ...) __attribute__ ((format(printf,2,3)));
};

inline DebugPrint
Debug(const char* file, int line)
{ return DebugPrint(file, line);}

inline DebugPrint
DebugFile(const char* file, int line, int defile)
{ return DebugPrint(file, line, defile);}

#define debugprint (!epicsDebug)?0:Debug(__FILE__,__LINE__).Print
#define debugprintfile (!debugFile)?0:DebugFile(__FILE__,__LINE__,debugFile).Print

#ifdef __cplusplus
}
#endif

#endif
