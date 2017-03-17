#include "debugPrint.h"

int epicsDebug = 0;
epicsExportAddress(int, epicsDebug);

int debugFile;
epicsExportAddress(int, debugFile);

int DebugPrint::Print(const char* fmt, ...)
{
    va_list args;
    char timestamp[40];
    time_t t;
    struct tm tm;
    time(&t);
    localtime_r(&t, &tm);
    strftime(timestamp, sizeof(timestamp), "%Y/%m/%d %H:%M:%S", &tm);

    va_start(args, fmt);
    const char* f = strrchr(file, '/');
    if (f) f++; else f = file;
    FILE* fp = outfile ? outfile : stderr;
    fprintf(fp, "%s ", timestamp);
    fprintf(fp, "%s:%d: ", f, line);
    vfprintf(fp, fmt, args);
    fflush(fp);
    va_end(args);
    return 1;
}

