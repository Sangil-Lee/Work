#include "debugPrint.h"

int epicsDebugLevel = 0;
static char debugFile[FILE_PATH_NAME_LEN]="";

int debugPrint(const char* file, const int line, const int debugLevel, const char *fmt, ...) 
{
	char buf[1024];
	char bufTime[64];
	va_list argp;
	epicsTimeStamp epics_time;

	static FILE *fp = NULL;
	static char debugFileName[FILE_PATH_NAME_LEN] = "";

	va_start(argp, fmt);
	vsprintf(buf, fmt, argp);
	va_end(argp);

	epicsTimeGetCurrent (&epics_time);
	epicsTimeToStrftime (bufTime, sizeof(bufTime), "%Y/%m/%d %H:%M:%S.%03f", &epics_time);

	if (epicsDebugLevel >= debugLevel) {
		if (epicsDebugLevel >= DEBUG_TRACE) {
			fprintf (stdout,"[file(%s),line(%d)]=>[%s] %s",file, line, bufTime, buf);
		}
		else {
			fprintf (stdout, "[%s] %s", bufTime, buf);
		};
		fflush(stdout);
	};

	if(debugFile!=NULL)
	{
		if (epicsDebugLevel >= debugLevel) {
			if ( (strlen(debugFile) > 0)
					&& ((NULL == fp) || (0 != strcmp (debugFileName, debugFile)) ) ) {
				if (NULL != fp) {
					fclose (fp);
				}

				strcpy (debugFileName, debugFile);

				if (NULL == (fp = fopen (debugFileName, "a+"))) {
					fprintf (stderr, "[%s] open failed \n", debugFileName);
				}
				else {
					fprintf (stdout, "[%s] logfile %s is opened ...\n", bufTime, debugFileName);
				}
			};

			if (NULL != fp) {
				if (epicsDebugLevel >= DEBUG_TRACE) {
					fprintf (fp, "[file(%s),line(%d)]=>[%s] %s",__FILE__,__LINE__, bufTime, buf);
				}
				else {
					fprintf (fp, "[%s] %s", bufTime, buf);
				};
				fflush (fp);
			};
		};
	};

	return(0);
};

static const iocshArg		setDebugFileArg0		= {"debugFileName", iocshArgString};
static const iocshArg		*setDebugFileArgs[]		= {&setDebugFileArg0};
static const iocshFuncDef	setDebugFileFuncDef		= {"setDebugFile", 1, setDebugFileArgs};

void setDebugFile (const char *logFile)
{
	if (logFile && (FILE_PATH_NAME_LEN > strlen(logFile)) ) {
		strncpy (debugFile, logFile, FILE_PATH_NAME_LEN);
	}
}

static void setDebugFileCallFunc(const iocshArgBuf *args) {
	setDebugFile (args[0].sval);
}

static void SetDebugFileRegister(void) {
	iocshRegister (&setDebugFileFuncDef, setDebugFileCallFunc);
}
epicsExportAddress(int, epicsDebugLevel);
epicsExportRegistrar(SetDebugFileRegister);


