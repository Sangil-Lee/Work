/* rtpInterpose.h */
/* 
 * Interpose interface support for Modbus with TCP/IP or serial
 *
 * Author: Mark Rivers
 */

#ifndef rtpInterpose_H
#define rtpInterpose_H

#include <shareLib.h>
#include <epicsExport.h>

#if 0
typedef enum {
    rtpLinkTCP,
    rtpLinkRTU,
    rtpLinkASCII
} rtpLinkType;
#endif

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

epicsShareFunc int rtpInterposeConfig(const char *portName, 
						 int timeoutMsec, int writeDelayMsec);
#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif /* rtpInterpose_H */
