/*----------------------------------------------------------------------
 *  file:        drvRtpAsyn.c                                         
 *----------------------------------------------------------------------
 * EPICS asyn driver support for RTP protocol communication with RTP
 * 
 *-----------------------------------------------------------------------
 *
 */

/* ANSI C includes  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* EPICS includes */
#include <dbAccess.h>
#include <epicsStdio.h>
#include <epicsString.h>
#include <epicsThread.h>
#include <epicsMutex.h>
#include <epicsTime.h>
#include <epicsEndian.h>
#include <cantProceed.h>
#include <errlog.h>
#include <osiSock.h>
#include <iocsh.h>

/* Asyn includes */
#include "asynDriver.h"
#include "asynOctetSyncIO.h"
#include "asynCommonSyncIO.h"
#include "asynStandardInterfaces.h"

#include "rtp.h"
#include "drvRtpAsyn.h"
#include <epicsExport.h>




/********************************************************************
**  global driver functions
*********************************************************************
*/

/*
** drvRtpAsynConfigure() - create and init an asyn port driver for a PLC
**                                                                    
*/
int drvRtpAsynConfigure(char *portName, 
                        char *octetPortName, 
                        int nodeNumber,
                        int rtpStartIndex, 
                        int numberToRead,
                        rtpDataType_t dataType,
                        int pollMsec, 
                        char *nodeName)
{
    return asynSuccess;
}
