#ifndef devS7nodave_h
#define devS7nodave_h

#include <epicsTypes.h>

// Status codes for record functions:
#ifndef RECORD_STATUS_OK
/* Record processing was successful */
#define RECORD_STATUS_OK 0
#endif

#ifndef RECORD_STATUS_OK_NO_CONVERT
/*
 * Record processing was successful and no conversion should be performed by the
 * record (only for ai and ao records).
 */
#define RECORD_STATUS_OK_NO_CONVERT 2
#endif

#ifndef RECORD_STATUS_ERROR
/*
 * An error occurred while processing the record.
 */
#define RECORD_STATUS_ERROR -1
#endif

// MAX_STRING_SIZE should be defined in epicsTypes.h, however if it is not,
// we define it here.
#ifndef MAX_STRING_SIZE
/* Fixed length of an EPICS string */
#define MAX_STRING_SIZE 40
#endif

#endif // devS7nodave_h
