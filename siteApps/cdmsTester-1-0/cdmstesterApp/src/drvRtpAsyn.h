/* drvRtpAsyn.h
 *
 *   Author: silee
 *   18-Nov-2016
 *
 *   These are the public definitions for drvRtpAsyn.
 * 
 */

/* These are the strings that device support passes to drivers via 
 * the asynDrvUser interface.
 * Drivers must return a value in pasynUser->reason that is unique 
 * for that command.
 */

#define RTP_DATA_STRING             "RTP_DATA" 
#define RTP_READ_STRING             "RTP_READ" 
#define RTP_UINT16_STRING           "UINT16" 
#define RTP_INT16_SM_STRING         "INT16SM" 
#define RTP_BCD_UNSIGNED_STRING     "BCD_UNSIGNED" 
#define RTP_BCD_SIGNED_STRING       "BCD_SIGNED" 
#define RTP_INT16_STRING            "INT16" 
#define RTP_INT32_LE_STRING         "INT32_LE" 
#define RTP_INT32_BE_STRING         "INT32_BE" 
#define RTP_FLOAT32_LE_STRING       "FLOAT32_LE" 
#define RTP_FLOAT32_BE_STRING       "FLOAT32_BE" 
#define RTP_FLOAT64_LE_STRING       "FLOAT64_LE" 
#define RTP_FLOAT64_BE_STRING       "FLOAT64_BE" 
#define RTP_ENABLE_HISTOGRAM_STRING "ENABLE_HISTOGRAM"
#define RTP_READ_HISTOGRAM_STRING   "READ_HISTOGRAM"
#define RTP_HISTOGRAM_BIN_TIME_STRING  "HISTOGRAM_BIN_TIME"
#define RTP_HISTOGRAM_TIME_AXIS_STRING "HISTOGRAM_TIME_AXIS"
#define RTP_POLL_DELAY_STRING       "POLL_DELAY"
#define RTP_READ_OK_STRING          "READ_OK"
#define RTP_WRITE_OK_STRING         "WRITE_OK"
#define RTP_IO_ERRORS_STRING        "IO_ERRORS"
#define RTP_LAST_IO_TIME_STRING     "LAST_IO_TIME"
#define RTP_MAX_IO_TIME_STRING      "MAX_IO_TIME"

typedef enum {
    dataTypeUInt16,           /* 16-bit unsigned               drvUser=UINT16 */
    dataTypeInt16SM,          /* 16-bit sign and magnitude     drvUser=INT16SM */
    dataTypeBCDUnsigned,      /* 16-bit unsigned BCD           drvUser=BCD_SIGNED */
    dataTypeBCDSigned,        /* 16-bit signed BCD             drvUser=BCD_UNSIGNED */
    dataTypeInt16,            /* 16-bit 2's complement         drvUser=INT16 */
    dataTypeInt32LE,          /* 32-bit integer little-endian  drvUser=INT32_LE */
    dataTypeInt32BE,          /* 32-bit integer big-endian     drvUser=INT32_BE */
    dataTypeFloat32LE,        /* 32-bit float little-endian    drvuser=FLOAT32_LE */
    dataTypeFloat32BE,        /* 32-bit float big-endian       drvUser=FLOAT32_BE */
    dataTypeFloat64LE,        /* 64-bit float little-endian    drvuser=FLOAT64_LE */
    dataTypeFloat64BE         /* 64-bit float big-endian       drvUser=FLOAT64_BE */
} rtpDataType_t;

#define MAX_RTP_DATA_TYPES 11


int drvRtpAsynConfigure(char *portName, 
                        char *octetPortName, 
                        int nodeNumber,
                        int rtpStartIndex, 
                        int numberToRead,
                        rtpDataType_t dataType,
                        int pollMsec, 
                        char *nodeName);
