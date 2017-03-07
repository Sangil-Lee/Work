#ifndef LCA_ERROR_H
#define LCA_ERROR_H
/* $Id: lcaError.h,v 1.6 2015/03/12 17:12:12 strauman Exp $ */

/* labca error interface */

#include <stdarg.h>
#include <shareLib.h>

#ifdef SCILAB_APP
#include <version.h>

#if SCI_VERSION_MAJOR > 5 || (SCI_VERSION_MAJOR == 5 && SCI_VERSION_MINOR >= 2)
#include <Scierror.h>
#endif
#endif

typedef struct LcaError_ {
	int  err;
	char msg[100];
	int  nerrs;  /* optional error vector */
	int  *errs;  /* optional error vector; must be FREEd when object goes out of scope */
} LcaError;

/* Errors returned by ezcaNewMonitorValue */

#define EZCA_NOMONITOR	20
#define EZCA_NOCHANNEL	21

#ifdef __cplusplus
extern "C" {
#endif

epicsShareFunc void epicsShareAPI
lcaErrorInit(LcaError *pe);

/* remember last error that occurred;
 * this takes over an optional error vector
 * and NULLs pe->errs
 */
epicsShareFunc void epicsShareAPI
lcaSaveLastError(LcaError *pe);

epicsShareFunc void epicsShareAPI
lcaSetError(LcaError *pe, int err, const char *fmt, ...);

epicsShareFunc LcaError * epicsShareAPI
lcaGetLastError();

#ifdef SCILAB_APP
#define LCA_RAISE_ERROR(theError)	\
	do { \
		if ( (theError)->err ) { \
			lcaSaveLastError(theError); \
			Scierror((theError)->err + 10000, (theError)->msg); \
		} else { \
		    ezcaFree((theError)->errs); (theError)->errs = 0; \
		    (theError)->nerrs = 0; \
		} \
	} while (0)
#endif

#ifdef __cplusplus
};
#endif

#endif
