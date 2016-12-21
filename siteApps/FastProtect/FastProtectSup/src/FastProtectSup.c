/*
 * MIT License
 *
 * Copyright (c) 2106 Osprey DCS
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * ASYN support module for Fast Protection System
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <cantProceed.h>
#include <errlog.h>
#include <epicsExit.h>
#include <epicsExport.h>
#include <epicsEvent.h>
#include <epicsString.h>
#include <epicsThread.h>
#include <iocsh.h>
#include <drvAsynIPPort.h>
#include <asynStandardInterfaces.h>
#include <asynCommonSyncIO.h>
#include <asynOctetSyncIO.h>
#include "fastProtectionProtocol.h"

/*
 * ASYN subaddresses
 */
#define A_LO_SHIFT  8
#define A_LO_MASK   0xFF00
#define A_IDX_MASK  0x00FF

#define A_HI_MASK           0xFF0000
#define A_HI_COMMAND        0x000000
#define A_HI_INT32_SCANIO   0x000000
#define A_HI_SYSMON         0x050000
#define A_HI_STATS          0x080000

#define A_LO_STATS_CMD_COUNT    0
#define A_LO_STATS_CMD_FAILURES 1
#define A_LO_INT32_SCANIO_OUTPUT  0x40

/*
 * Time conversion
 */
#define NS_PER_SECOND 1000000000
#define EVENT_RATE    125000000
#define LOGNAME_SIZE 40 /* Enough for strftime "%F_%T.dat.bz2" */

/*
 * Input/Output states
 */
enum ioState { iosZero=0, iosOne=1, iosUnknown=2, iosInit=3 };
#define INPUTS_PER_ACQ_NODE 64
#define ACQNODE_CAPACITY (FAST_PROTECT_PROTOCOL_MAX_NODES_PER_LINK * \
                          FAST_PROTECT_PROTOCOL_MAX_LINKS)
#define INPUT_CAPACITY (ACQNODE_CAPACITY * INPUTS_PER_ACQ_NODE)
#define OUTPUT_CAPACITY 16

/*
 * Mitigation bitmaps
 */
#define BITMAP_INT32_COUNT (ACQNODE_CAPACITY * 2)

/*
 * Number of times to retry a command
 */
#define COMMAND_RETRY_LIMIT 4

/*
 * Logging status
 */
#define LOGSTATUS_IDLE              0
#define LOGSTATUS_ACTIVE            1
#define LOGSTATUS_FOPEN_FAULT       2
#define LOGSTATUS_FWRITE_FAULT      3
#define LOGSTATUS_FFLUSH_FAULT      4
#define LOGSTATUS_FCLOSE_FAULT      5

/*
 * Links to lower-level UDP I/O
 */
struct udpLink {
    const char  *portName;
    const char  *hostName;
    asynUser    *pasynUserCommon;
    asynUser    *pasynUserOctet;
    int          isConnected;
};

/*
 * Driver private storage
 */
struct dpvt {
    /*
     * The ASYN interfaces we provide
     */
    char                *portName;
    asynUser            *pasynUser;  /* For controlling diagnostic messages */
    asynStandardInterfaces asynInterfaces;


    struct udpLink cmdLink;

    /*
     * Command/response client
     */
    struct fastProtectPacket    command;
    struct fastProtectPacket    response;
    size_t                      responseSize;

    /*
     * Change-of-state logging subscriber
     */
    struct udpLink                          logLink;
    epicsEventId                            shutdownEvent;
    struct fastProtectSubscriptionPacket    subscribePacket;
    struct fastProtectLogPacket             logPacket;
    struct fastProtectLogAckPacket          logAckPacket;
    double                                  nsecPerTick;
    int                                     logResubscribe;
    int                                     disconnectLogged;

    /*
     * Update SCAN=I/O Intr only when state changes
     */
    unsigned char inStateNew[INPUT_CAPACITY];
    unsigned char inStateOld[INPUT_CAPACITY];
    unsigned char outStateNew[OUTPUT_CAPACITY];
    unsigned char outStateOld[OUTPUT_CAPACITY];

    /*
     * Logging
     */
    unsigned long        whenToRotate;
    char                *logPath;
    char                *logFilename;
    FILE                *logStream;
    int                  logStatus;

    /*
     * Statistics
     */
    unsigned long   commandCount[COMMAND_RETRY_LIMIT+1];
    unsigned long   commandFailedCount;
};


/*
 * epicsAtExit callback to shut down subscriber thread
 */
static void
exitHandler(void *arg)
{
    struct dpvt *pdpvt = (struct dpvt *)arg;
    epicsEventSignal(pdpvt->shutdownEvent);
}

/*
 * Wait for a longish period but keep responsive to termination requests
 */
static int
longWait(struct dpvt *pdpvt)
{
    int i;

    for (i = 0 ; i < 15 ; i++) {
        if (epicsEventTryWait(pdpvt->shutdownEvent) == epicsEventWaitOK) {
            return 1;
        }
        epicsThreadSleep(2.0);
    }
    return 0;
}

/*
 * Log change-of-state data
 */
static void
logData(struct dpvt *pdpvt, int count)
{
    epicsTimeStamp now, ts;
    struct tm tm;
    unsigned long ns;

    if (pdpvt->logPath == NULL) return;
    epicsTimeGetCurrent(&now);
    if (pdpvt->logStream) {
        int logIndex;
        fastProtectLogEntry_t *lp, log;
        if (count) {
            lp = pdpvt->logPacket.log;
            pdpvt->disconnectLogged = 0;
        }
        else {
            if (pdpvt->disconnectLogged) return;
            memset(&log, 0, sizeof log);
            log.status = 1UL << 31;
            log.seconds = now.secPastEpoch + POSIX_TIME_AT_EPICS_EPOCH;
            log.ticks = now.nsec / pdpvt->nsecPerTick;
            lp = &log;
            count = 1;
            pdpvt->disconnectLogged = 1;
        }
        for (logIndex = 0 ; logIndex < count ; logIndex++, lp++) {
            if (lp->status != (uint32_t)~0UL) { /* Ignore keepalives */
                fwrite(lp, sizeof *lp, 1, pdpvt->logStream);
                if (ferror(pdpvt->logStream)) {
                    asynPrint(pdpvt->logLink.pasynUserOctet,
                                  ASYN_TRACE_ERROR,
                                            "%s: fwrite failed, errno:%d.\n",
                                                        pdpvt->portName, errno);
                    pdpvt->logStatus = LOGSTATUS_FWRITE_FAULT;
                    fclose(pdpvt->logStream);
                    pdpvt->logStream = NULL;
                    break;
                }
                else {
                    pdpvt->logStatus = LOGSTATUS_ACTIVE;
                }
            }
        }
    }
    if (pdpvt->logStream) {
        fflush(pdpvt->logStream);
        if (ferror(pdpvt->logStream)) {
            asynPrint(pdpvt->logLink.pasynUserOctet, ASYN_TRACE_ERROR,
                                "%s: fflush failed, errno:%d.\n",
                                                    pdpvt->portName, errno);
            pdpvt->logStatus = LOGSTATUS_FFLUSH_FAULT;
            fclose(pdpvt->logStream);
            pdpvt->logStream = NULL;
        }
    }
    if (now.secPastEpoch >= pdpvt->whenToRotate) {
        if (pdpvt->logStream != NULL) {
            if (fclose(pdpvt->logStream) != 0) {
                asynPrint(pdpvt->logLink.pasynUserOctet, ASYN_TRACE_ERROR,
                                    "%s: fclose failed.\n", pdpvt->portName);
                pdpvt->logStatus = LOGSTATUS_FCLOSE_FAULT;
            }
            pdpvt->logStream = NULL;
        }
        epicsTimeToStrftime(pdpvt->logFilename, LOGNAME_SIZE, "%F_%T.dat",&now);
        epicsTimeToTM(&tm, &ns, &now);
        tm.tm_hour = 23;
        tm.tm_min = 59;
        tm.tm_sec = 59;
        epicsTimeFromTM(&ts, &tm, 0);
        pdpvt->whenToRotate = ts.secPastEpoch + 1;
        pdpvt->logStream = fopen(pdpvt->logPath, "wb");
        if (pdpvt->logStream == NULL) {
            pdpvt->logStatus = LOGSTATUS_FOPEN_FAULT;
            asynPrint(pdpvt->logLink.pasynUserOctet, ASYN_TRACE_ERROR,
                          "%s can't open acquisition log file %s: %s\n",
                                                        pdpvt->logLink.portName,
                                                        pdpvt->logPath,
                                                        strerror(errno));
        }
        else {
            /* Force transmission of all input states */
            pdpvt->logResubscribe = 1;
        }
    }
}

/*
 * Consume a change-of-state logging packet
 * For now convert the packet to SCAN=I/O Intr callbacks.
 * At some point it might also get sent to a database or
 * stored in a local ring buffer.
 */
static void
handleLogPacket(struct dpvt *pdpvt, size_t pkSize)
{
    ELLLIST *pclientList;
    interruptNode *pnode;
    int logIndex;
    fastProtectLogEntry_t *lp = pdpvt->logPacket.log;
    int count;
    epicsTimeStamp ts;

    asynPrint(pdpvt->pasynUser, ASYN_TRACEIO_DRIVER,
                                             "Log packet pksize %zu\n", pkSize);
    if (pkSize >= FAST_PROTECT_PROTOCOL_LOG_COUNT_TO_LENGTH(0)) {
        count = FAST_PROTECT_PROTOCOL_LOG_LENGTH_TO_COUNT(pkSize);
        if (count) logData(pdpvt, count);
    }
    else {
        if (pkSize == 0) logData(pdpvt, 0);
        epicsTimeGetCurrent(&ts);
        pkSize = 0;
        count = 1;
    }
    if (pkSize && (pdpvt->logPacket.subscriptionNumber !=
                             pdpvt->subscribePacket.subscriptionNumber)) return;

    /*
     * Generating a SCAN=I/O Intr callback is fairly expensive, so do
     * so only when an input or output is known to have changed state.
     */
    for (logIndex = 0 ; logIndex < count ; logIndex++, lp++) {
        int lo;
        if (pkSize) {
            int i, idx;
            uint32_t v, b;
            if (lp->status == (uint32_t)~0UL) continue; /* Ignore keepalives */
            asynPrint(pdpvt->pasynUser, ASYN_TRACEIO_DRIVER,
                                "S:%08X T:%8u:%09u %08X:%08X\n",
                                                (unsigned int)lp->status,
                                                (unsigned int)lp->seconds,
                                                (unsigned int)lp->ticks,
                                                (unsigned int)lp->inputStateHi,
                                                (unsigned int)lp->inputStateLo);
            lo = (lp->status &
                    FAST_PROTECT_PROTOCOL_LOG_ENTRY_STATUS_ACQINDEX_MASK) >>
                          FAST_PROTECT_PROTOCOL_LOG_ENTRY_STATUS_ACQINDEX_SHIFT;
            ts.secPastEpoch =  lp->seconds - POSIX_TIME_AT_EPICS_EPOCH;
            ts.nsec = lp->ticks * pdpvt->nsecPerTick;
            if (ts.nsec >= NS_PER_SECOND) {
                ts.secPastEpoch += ts.nsec / NS_PER_SECOND;
                ts.nsec %= NS_PER_SECOND;
            }
            if (lo >= ACQNODE_CAPACITY) continue;
            if (lp->status &
                    FAST_PROTECT_PROTOCOL_LOG_ENTRY_STATUS_VALID_ACQINFO_MASK) {
                idx = lo * INPUTS_PER_ACQ_NODE;
                b = 0x1;
                v = lp->inputStateLo;
                for (i = 0 ; i < 32 ; i++, idx++, b <<= 1)
                    pdpvt->inStateNew[idx] = v & b ? iosOne : iosZero;
                b = 0x1;
                v = lp->inputStateHi;
                for (i = 0 ; i < 32 ; i++, idx++, b <<= 1)
                    pdpvt->inStateNew[idx] = v & b ? iosOne : iosZero;
            }
            b = 0x1;
            v = lp->status;
            for (i = 0 ; i < OUTPUT_CAPACITY ; i++, b <<= 1)
                pdpvt->outStateNew[i] = v & b ? iosOne : iosZero;
        }
        else {
            lo = 0;
            memset(pdpvt->inStateNew, iosUnknown, sizeof pdpvt->inStateNew);
            memset(pdpvt->outStateNew, iosUnknown, sizeof pdpvt->outStateNew);
        }
        pasynManager->interruptStart(pdpvt->asynInterfaces.int32InterruptPvt,
                                                                  &pclientList);
        pnode = (interruptNode *)ellFirst(pclientList);
        while (pnode) {
            asynInt32Interrupt *int32Interrupt = pnode->drvPvt;
            int aHi = int32Interrupt->addr & A_HI_MASK;
            pnode = (interruptNode *)ellNext(&pnode->node);
            if (aHi == A_HI_INT32_SCANIO) {
                int aLo = (int32Interrupt->addr & A_LO_MASK) >> A_LO_SHIFT;
                int aIdx = int32Interrupt->addr & A_IDX_MASK;
                int v;
                unsigned char *np, *op;
                if (aLo < ACQNODE_CAPACITY) {
                    int idx = aLo * INPUTS_PER_ACQ_NODE + aIdx;
                    if (aIdx >= INPUTS_PER_ACQ_NODE) continue;
                    np = &pdpvt->inStateNew[idx];
                    op = &pdpvt->inStateOld[idx];
                }
                else if (aLo == A_LO_INT32_SCANIO_OUTPUT) {
                    if (aIdx >= OUTPUT_CAPACITY) continue;
                    np = &pdpvt->outStateNew[aIdx];
                    op = &pdpvt->outStateOld[aIdx];
                }
                else {
                    continue;
                }
                if (*np == *op) continue;
                if (*np == iosUnknown) {
                    int32Interrupt->pasynUser->auxStatus = asynError;
                    v = 0;
                }
                else {
                    int32Interrupt->pasynUser->auxStatus = asynSuccess;
                    v = *np;
                }
                *op = *np;
                int32Interrupt->pasynUser->timestamp = ts;
                int32Interrupt->callback(int32Interrupt->userPvt,
                                         int32Interrupt->pasynUser, v);
            }
        }
        pasynManager->interruptEnd(pdpvt->asynInterfaces.int32InterruptPvt);
    }
}

/*
 * Read and consume logging packets
 */
static int
consumeLogPackets(struct dpvt *pdpvt)
{
    int newSubscription = 1;
    uint32_t previousSequenceNumber = 0;
    int eomReason;
    size_t nRead, nWrite;

    for (;;) {
        if (pdpvt->logResubscribe) return 1;
        if (epicsEventTryWait(pdpvt->shutdownEvent) == epicsEventWaitOK) {
            return 0;
        }

        /* Fetch */
        if (pasynOctetSyncIO->read(pdpvt->logLink.pasynUserOctet,
                                                   (char *)&pdpvt->logPacket,
                                                   sizeof pdpvt->logPacket,
                                                   10.0,
                                                   &nRead,
                                                   &eomReason) != asynSuccess) {
            asynPrint(pdpvt->logLink.pasynUserCommon, ASYN_TRACE_ERROR,
                        "Port %s read failed: %s\n",
                                   pdpvt->logLink.portName,
                                   pdpvt->logLink.pasynUserOctet->errorMessage);
            break;
        }
        if (nRead < FAST_PROTECT_PROTOCOL_COUNT_TO_LENGTH(1)) {
            continue;
        }
        pdpvt->logAckPacket.sequenceNumber = pdpvt->logPacket.sequenceNumber;
        if (pasynOctetSyncIO->write(pdpvt->logLink.pasynUserOctet,
                                    (const char *)&pdpvt->logAckPacket,
                                    sizeof pdpvt->logAckPacket,
                                    2.0,
                                    &nWrite) != asynSuccess) {
            asynPrint(pdpvt->logLink.pasynUserCommon, ASYN_TRACE_ERROR,
                        "Port %s write failed: %s\n",
                                   pdpvt->logLink.portName,
                                   pdpvt->logLink.pasynUserOctet->errorMessage);
            break;
        }
        if (newSubscription
         || (pdpvt->logPacket.sequenceNumber != previousSequenceNumber)) {
            newSubscription = 0;
            previousSequenceNumber = pdpvt->logPacket.sequenceNumber;
            handleLogPacket(pdpvt, nRead);

        }
    }
    handleLogPacket(pdpvt, 0);
    return 1;
}

/*
 * Subscribe to change-of-state log and system monitor publications
 */
static void
subscriberTask(void *arg)
{
    struct dpvt *pdpvt = (struct dpvt *)arg;
    uint32_t subscriptionNumber;
    size_t ntrans;
    epicsTimeStamp now;
    extern volatile int interruptAccept;

    while (!interruptAccept) {
        if (epicsEventTryWait(pdpvt->shutdownEvent) == epicsEventWaitOK)
            return;
        epicsThreadSleep(1.0);
    }
    epicsTimeGetCurrent(&now);
    subscriptionNumber = now.secPastEpoch;
    pdpvt->subscribePacket.magic=FAST_PROTECT_PROTOCOL_MAGIC_SUBSCRIBE_LOG;
    pdpvt->subscribePacket.subscriptionNumber = subscriptionNumber;
    pdpvt->logAckPacket.magic = FAST_PROTECT_PROTOCOL_MAGIC_ACKNOWLEDGE_LOG;
    pdpvt->logAckPacket.subscriptionNumber = subscriptionNumber;
    for (;;) {
        /* Connect */
        asynPrint(pdpvt->logLink.pasynUserCommon, ASYN_TRACE_FLOW,
                                  "Port %s connect\n", pdpvt->logLink.portName);
        while (pasynCommonSyncIO->connectDevice(pdpvt->logLink.pasynUserCommon)
                                                               != asynSuccess) {

            asynPrint(pdpvt->logLink.pasynUserCommon, ASYN_TRACE_ERROR,
                           "%s failed to connect: %s\n",
                                   pdpvt->cmdLink.portName,
                                   pdpvt->cmdLink.pasynUserOctet->errorMessage);
            if (longWait(pdpvt)) return;
        }
        pdpvt->logLink.isConnected = 1;

        /* Subscribe */
        if (pasynOctetSyncIO->write(pdpvt->logLink.pasynUserOctet,
                                          (const char *)&pdpvt->subscribePacket,
                                          sizeof pdpvt->subscribePacket,
                                          2.0,
                                          &ntrans) == asynSuccess) {
            pdpvt->logResubscribe = 0;
            if (consumeLogPackets(pdpvt) == 0) {
                break;
            }
        }
        else {
            asynPrint(pdpvt->logLink.pasynUserCommon, ASYN_TRACE_ERROR,
                        "Port %s subscribe failed: %s\n",
                                   pdpvt->logLink.portName,
                                   pdpvt->logLink.pasynUserOctet->errorMessage);
            if (longWait(pdpvt)) return;
        }
        if (pasynCommonSyncIO->disconnectDevice(pdpvt->logLink.pasynUserCommon)
                                                            != asynSuccess) {
            errlogPrintf("PANIC -- can't disconnect port %s host %s.\n",
                                                       pdpvt->logLink.portName,
                                                       pdpvt->logLink.hostName);
            break;
        }
        pdpvt->logLink.isConnected = 0;
    }
    errlogPrintf("Fast protection subscriber terminating!\n");
}

static asynStatus
badAddress(asynUser *pasynUser)
{
    asynStatus status;
    int address;

    if ((status = pasynManager->getAddr(pasynUser, &address)) != asynSuccess)
        return status;
    epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                       "Bad ASYN address %d (0x%X)", address, address);
    return asynError;
}

/*
 * asynCommon methods
 */
static void
report(void *pvt, FILE *fp, int details)
{
    struct dpvt *pdpvt = (struct dpvt *)pvt;

    fprintf(fp, " CMD %sconnected\n", pdpvt->cmdLink.isConnected ? "" : "not ");
    fprintf(fp, " LOG %sconnected\n", pdpvt->logLink.isConnected ? "" : "not ");
}

static asynStatus
connect(void *pvt, asynUser *pasynUser)
{
    pasynManager->exceptionConnect(pasynUser);
    return asynSuccess;
}

static asynStatus
disconnect(void *pvt, asynUser *pasynUser)
{
    pasynManager->exceptionDisconnect(pasynUser);
    return asynSuccess;
}
static asynCommon commonMethods = { report, connect, disconnect };

/*
 * Consume a system monitoring reply
 * Mapping of monitoring values in to reply packet 'args' array:
 *  All system monitor values are 16 bits.
 *  Two 16-bit values packed into each array element.  The offsets in
 *  the following tables are for 16-bit values.  The 'args' offsets
 *  would be these values divided by two, litle endian.
 *  8 values per node (7 XADC, FAN) and 1+5*8 nodes:
 *        0-7   Mitigation
 *        8-15  L0N0
 *       16-23  L1N0
 *       ...
 *      312-319 L6N4
 *      320-327 L7N4
 *  SFP receiver power (low word) and temperature (high word):
 *      328-329: Mitigation node EVR
 *      330-345: Mitigation node from node 0 in each chain (L0, L1, ... L7)
 *      346-361: L0N0, L1N0, ... L7N0 to migitaion in each chain
 *      362-377: L0N0, L1N0, ... L7N0 from node 1 in each chain
 *      378-393: L0N1, L1N1, ... L7N1 to node 0 in each chain
 *      394-408: L0N1, L1N1, ... L7N1 from node 2 in each chain
 *      410-425: L0N2, L1N2, ... L7N2 to node 1 in each chain
 *      426-441: L0N2, L1N2, ... L7N2 from node 3 in each chain
 *      442-457: L0N3, L1N3, ... L7N3 to node 2 in each chain
 *      458-473: L0N3, L1N3, ... L7N3 from node 4 in each chain
 *      474-489: L0N4, L1N4, ... L7N4 to node 3 in each chain
 */
static void
handleSysmonReply(struct dpvt *pdpvt, int isGood)
{
    ELLLIST *pclientList;
    interruptNode *pnode;
    epicsTimeStamp now;
    int count = FAST_PROTECT_PROTOCOL_LENGTH_TO_COUNT(pdpvt->responseSize);

    asynPrint(pdpvt->pasynUser,ASYN_TRACEIO_DRIVER,"Sysmon packet %d\n",count);
    epicsTimeGetCurrent(&now);
    pasynManager->interruptStart(pdpvt->asynInterfaces.int32InterruptPvt,
                                                                  &pclientList);
    pnode = (interruptNode *)ellFirst(pclientList);
    while (pnode) {
        asynInt32Interrupt *int32Interrupt = pnode->drvPvt;
        int aHi = int32Interrupt->addr & A_HI_MASK;
        int aIdx = int32Interrupt->addr & ~A_HI_MASK;
        pnode = (interruptNode *)ellNext(&pnode->node);
        if (aHi == A_HI_SYSMON) {
            int value;
            if (isGood) {
                uint32_t v;
                int argIndex = aIdx >> 1;
                if (argIndex >= count) continue;
                int32Interrupt->pasynUser->auxStatus = asynSuccess;
                v = pdpvt->response.args[argIndex];
                value = (aIdx & 0x1) ? (v >> 16) : (v & 0xFFFF);
            }
            else {
                int32Interrupt->pasynUser->auxStatus = asynError;
                value = 0;
            }
//{char tbuf[40]; epicsTimeToStrftime (tbuf, sizeof tbuf, "%F %T.%07f", &now); printf("%3d %4d %d %s\n", aIdx, value, int32Interrupt->pasynUser->auxStatus, tbuf); }
            int32Interrupt->pasynUser->timestamp = now;
            int32Interrupt->callback(int32Interrupt->userPvt,
                                     int32Interrupt->pasynUser, value);
        }
    }
    pasynManager->interruptEnd(pdpvt->asynInterfaces.int32InterruptPvt);
}

/*
 * Send command, get response
 */
static asynStatus
commandResponse(struct dpvt *pdpvt, asynUser *pasynUser, int commandArgCount)
{
    int retryCount;
    size_t nwrite = FAST_PROTECT_PROTOCOL_COUNT_TO_LENGTH(commandArgCount);
    size_t ntrans;
    int eomReason;
    int omitSend = 0;
    asynStatus status;
    double timeout = 0.2;
    static uint32_t sequenceNumber;

    asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s command %#X, argc %d\n",
              pdpvt->cmdLink.portName, pdpvt->command.args[0], commandArgCount);
    pdpvt->command.sequenceNumber = ++sequenceNumber;
    if (!pdpvt->cmdLink.isConnected) {
        status = pasynCommonSyncIO->connectDevice(pdpvt->cmdLink.pasynUserCommon);
        if (status != asynSuccess) {
            asynPrint(pasynUser, ASYN_TRACE_ERROR,
                                    "%s failed to connect to %s: %s\n",
                                        pdpvt->cmdLink.portName,
                                        pdpvt->cmdLink.hostName,
                                        pdpvt->cmdLink.pasynUserOctet->errorMessage);
            pdpvt->commandFailedCount++;
            return status;
        }
        pdpvt->cmdLink.isConnected = 1;
    }
    for (retryCount = 0 ; ; ) {
        if (omitSend) {
            status = asynSuccess;
            omitSend = 0;
        }
        else {
            status = pasynOctetSyncIO->write(pdpvt->cmdLink.pasynUserOctet,
                                                (const char *)&pdpvt->command,
                                                nwrite, timeout, &ntrans);
        }
        if (status == asynSuccess) {
            status = pasynOctetSyncIO->read(pdpvt->cmdLink.pasynUserOctet,
                                                  (char *)&pdpvt->response,
                                                  sizeof pdpvt->response,
                                                  timeout, &ntrans, &eomReason);
        }
        if (status == asynSuccess) {
            if (pdpvt->response.sequenceNumber == pdpvt->command.sequenceNumber) {
                pdpvt->commandCount[retryCount]++;
                pdpvt->responseSize = ntrans;
                return asynSuccess;
            }
            epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                            "Sent command %u, got reply %u",
                                 (unsigned int)pdpvt->command.sequenceNumber,
                                 (unsigned int)pdpvt->response.sequenceNumber);
            status = asynError;
            omitSend = 1;
        }
        if (++retryCount > COMMAND_RETRY_LIMIT)
            break;
        asynPrint(pasynUser, ASYN_TRACEIO_DRIVER, "%s retry: %s\n",
                   pdpvt->cmdLink.portName,
                   status == asynTimeout ? "Timeout" : pasynUser->errorMessage);
        timeout = 2.0;
    }
    if (status == asynTimeout)
        epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                                                                    "Timeout");
    if (pasynCommonSyncIO->disconnectDevice(pdpvt->cmdLink.pasynUserCommon)
                                                               != asynSuccess) {
        errlogPrintf("%s can't disconnect: %s\n", pdpvt->cmdLink.portName,
                                pdpvt->cmdLink.pasynUserCommon->errorMessage);
    }
    pdpvt->cmdLink.isConnected = 0;
    pdpvt->commandFailedCount++;
    return status;
}

/*
 * asynInt32 support
 */
static asynStatus
int32Read(void *pvt, asynUser *pasynUser, epicsInt32 *value)
{
    struct dpvt *pdpvt = (struct dpvt *)pvt;
    asynStatus status;
    int address;
    int aHi, aLo, aIdx;

    if ((status = pasynManager->getAddr(pasynUser, &address)) != asynSuccess)
        return status;
    aHi = address & A_HI_MASK;
    aLo = (address & A_LO_MASK) >> A_LO_SHIFT;
    aIdx = address & A_IDX_MASK;
    switch (aHi) {
    case A_HI_STATS:
        switch (aLo) {
        case A_LO_STATS_CMD_COUNT:
            if (aIdx <= COMMAND_RETRY_LIMIT) {
                *value = pdpvt->commandCount[aIdx];
                return asynSuccess;
            }
            break;

        case A_LO_STATS_CMD_FAILURES:
            *value = pdpvt->commandFailedCount;
            return asynSuccess;
        }
        break;

    case A_HI_SYSMON:
        pdpvt->command.magic = FAST_PROTECT_PROTOCOL_MAGIC_COMMAND_READ_SYSMON;
        status = commandResponse(pdpvt, pasynUser, 0);
        handleSysmonReply(pdpvt, status == asynSuccess);
        return status;
    }
    return badAddress(pasynUser);
}

static asynStatus
int32Write(void *pvt, asynUser *pasynUser, epicsInt32 value)
{
    struct dpvt *pdpvt = (struct dpvt *)pvt;
    asynStatus status;
    int address;
    int aHi;

    if ((status = pasynManager->getAddr(pasynUser, &address)) != asynSuccess)
        return status;
    aHi = address & A_HI_MASK;
    switch (aHi) {
    case A_HI_COMMAND:
        pdpvt->command.magic = FAST_PROTECT_PROTOCOL_MAGIC_COMMAND_WRITE_SCALAR;
        pdpvt->command.args[0] = address;
        pdpvt->command.args[1] = value;
        return commandResponse(pdpvt, pasynUser, 2);
    }
    return badAddress(pasynUser);
}

static asynInt32 int32Methods = { int32Write, int32Read };

/*
 * asynInt32Array support
 */
static asynStatus
badElementCount(asynUser *pasynUser, size_t n)
{
    epicsSnprintf(pasynUser->errorMessage, pasynUser->errorMessageSize,
                   "Bad element count %zu, expected %d", n, BITMAP_INT32_COUNT);
    return asynError;
}

static asynStatus
int32ArrayRead(void *pvt, asynUser *pasynUser, epicsInt32 *value,
                                                size_t nelements, size_t *nIn)
{
    struct dpvt *pdpvt = (struct dpvt *)pvt;
    asynStatus status;
    int address;
    int aHi, aLo;

    if (nelements != BITMAP_INT32_COUNT)
        return badElementCount(pasynUser, nelements);
    if ((status = pasynManager->getAddr(pasynUser, &address)) != asynSuccess)
        return status;
    aHi = address & A_HI_MASK;
    aLo = (address >> A_LO_SHIFT) & A_LO_MASK;
    if ((aHi == A_HI_COMMAND) && ((aLo == 0) || (aLo == 1))) {
        pdpvt->command.magic = FAST_PROTECT_PROTOCOL_MAGIC_COMMAND_READ_BITMAP;
        pdpvt->command.args[0] = address;
        status = commandResponse(pdpvt, pasynUser, 1);
        if (status == asynSuccess) {
            *nIn = BITMAP_INT32_COUNT;
            memcpy(value, pdpvt->response.args,
                                           BITMAP_INT32_COUNT*sizeof(uint32_t));
        }
        return status;
    }
    return badAddress(pasynUser);
}

static asynStatus
int32ArrayWrite(void *pvt, asynUser *pasynUser, epicsInt32 *value,
                                                               size_t nelements)
{
    struct dpvt *pdpvt = (struct dpvt *)pvt;
    asynStatus status;
    int address;
    int aHi, aLo;

    if (nelements != BITMAP_INT32_COUNT)
        return badElementCount(pasynUser, nelements);
    if ((status = pasynManager->getAddr(pasynUser, &address)) != asynSuccess)
        return status;
    aHi = address & A_HI_MASK;
    aLo = (address >> A_LO_SHIFT) & A_LO_MASK;
    if ((aHi == A_HI_COMMAND) && ((aLo == 0) || (aLo == 1))) {
        pdpvt->command.magic = FAST_PROTECT_PROTOCOL_MAGIC_COMMAND_WRITE_BITMAP;
        pdpvt->command.args[0] = address;
        memcpy(&pdpvt->command.args[1], value,
                                           BITMAP_INT32_COUNT*sizeof(uint32_t));
        return commandResponse(pdpvt, pasynUser, 1 + BITMAP_INT32_COUNT);
    }
    return badAddress(pasynUser);
}

static asynInt32Array int32ArrayMethods = { int32ArrayWrite, int32ArrayRead };

/*
 * Set up a UDP link
 */
static int
setupLink(struct udpLink *l,
          const char *portName, const char *ext,
          const char *hostName, unsigned int udpPortNumber, int priority)
{
    char *cp;

    cp = mallocMustSucceed(strlen(portName)+strlen(ext)+1, "Fast Protect");
    sprintf(cp, "%s%s", portName, ext);
    l->portName = cp;
    cp = mallocMustSucceed(strlen(hostName)+20, "Fast Protect");
    sprintf(cp, "%s:%u UDP", hostName, udpPortNumber);
    l->hostName = cp;
    /* No autoconnect, No process EOS */
    drvAsynIPPortConfigure(l->portName, l->hostName, priority, 1, 1);
    if (pasynCommonSyncIO->connect(l->portName, 0, &l->pasynUserCommon, NULL)
                                                               != asynSuccess) {
        errlogPrintf("Can't connect to %s common sync I/O\n", l->portName);
        return 1;
    }
    if (pasynOctetSyncIO->connect(l->portName, 0, &l->pasynUserOctet, NULL)
                                                               != asynSuccess) {
        errlogPrintf("Can't connect to %s octet sync I/O\n", l->portName);
        return 1;
    }
    return 0;
}

static void
fpConfigure(const char *portName, const char *hostName,
            const char *logDir, int priority, double eventRate)
{
    struct dpvt *pdpvt;
    asynStandardInterfaces *pInterfaces;
    unsigned int subscriberPriority;
    epicsThreadId tid;
    asynStatus status;

    pdpvt = callocMustSucceed(sizeof *pdpvt, 1, "Fast Protect");
    if (!portName || !hostName) {
        errlogPrintf("Port/host name missing");
        return;
    }
    if (strchr(hostName, ':')) {
        errlogPrintf("UDP port number not allowed in host name");
        return;
    }
    if (eventRate <= 0) eventRate = EVENT_RATE;
    if ((eventRate <= 50e6) || (eventRate > 500e6)) {
        errlogPrintf("Unreasonable event clock frequency.\n");
        return;
    }
    pdpvt->nsecPerTick = 1.0e9 / eventRate;
    pdpvt->portName = epicsStrDup(portName);
    if (priority <= 0) priority = epicsThreadPriorityMedium;

    /*
     * Initialize change-of-state buffers
     */
    memset(pdpvt->inStateNew, iosInit, sizeof pdpvt->inStateNew);
    memset(pdpvt->inStateOld, iosInit, sizeof pdpvt->inStateOld);
    memset(pdpvt->outStateNew, iosInit, sizeof pdpvt->outStateNew);
    memset(pdpvt->outStateOld, iosInit, sizeof pdpvt->outStateOld);

    /*
     * Set up logging
     */
    if ((logDir != NULL) && (*logDir != '\0')) {
        size_t l;
        if (*logDir != '/')
            printf("Warning -- Log directory path is not absolute.\n");
        l = strlen(logDir);
        pdpvt->logPath = callocMustSucceed(l + LOGNAME_SIZE, 1, "bpmConf");
        strcpy(pdpvt->logPath, logDir);
        pdpvt->logFilename = pdpvt->logPath + l;
        if (pdpvt->logFilename[-1] != '/') *pdpvt->logFilename++ = '/';
    }

    /*
     * Set up the ASYN ports that do the actual I/O
     */
    setupLink(&pdpvt->cmdLink, portName, "_CMD", hostName,
                            FAST_PROTECT_PROTOCOL_COMMAND_UDP_PORT, priority);
    setupLink(&pdpvt->logLink, portName, "_LOG", hostName,
                            FAST_PROTECT_PROTOCOL_PUBLISHER_UDP_PORT, priority);

    /*
     * Create our port
     */
    pdpvt->pasynUser = pasynManager->createAsynUser(NULL, NULL);
    status = pasynManager->registerPort(pdpvt->portName,
                                        ASYN_CANBLOCK|ASYN_MULTIDEVICE,
                                        1, 0, 0);
    if(status != asynSuccess) {
        errlogPrintf("registerPort failed\n");
        return;
    }
    pInterfaces = &pdpvt->asynInterfaces;
    pInterfaces->common.pinterface     = &commonMethods;
    pInterfaces->int32.pinterface      = &int32Methods;
    pInterfaces->int32Array.pinterface = &int32ArrayMethods;
    //pInterfaces->octet.pinterface      = &octetMethods;
    pInterfaces->int32CanInterrupt     = 1;
    status = pasynStandardInterfacesBase->initialize(pdpvt->portName,
                                                     pInterfaces,
                                                     pdpvt->pasynUser, pdpvt);
    if (status != asynSuccess) {
        errlogPrintf("Can't register interfaces: %s.\n",
                                                pdpvt->pasynUser->errorMessage);
        return;
    }

    /*
     * Set up local pasynuser for controlling diagnostic messages
     */
    status = pasynManager->connectDevice(pdpvt->pasynUser, pdpvt->portName, 0);
    if (status != asynSuccess) {
        errlogPrintf("Can't find the port we just created!\n");
        return;
    }

    /*
     * Start the change-of-state log subcscriber
     */
    pdpvt->shutdownEvent = epicsEventMustCreate(epicsEventEmpty);
    epicsAtExit(exitHandler, pdpvt);
    epicsThreadLowestPriorityLevelAbove(priority, &subscriberPriority);
    tid = epicsThreadCreate(portName,
                            subscriberPriority,
                            epicsThreadGetStackSize(epicsThreadStackMedium),
                            subscriberTask,
                            pdpvt);
    if (!tid) {
        errlogPrintf("Can't set up %s reader thread!\n", portName);
        return;
    }
}

/*
 * IOC shell command registration
 */
static const iocshArg fpConfigureArg0 = { "port",              iocshArgString};
static const iocshArg fpConfigureArg1 = { "host",              iocshArgString};
static const iocshArg fpConfigureArg2 = { "logDir",            iocshArgString};
static const iocshArg fpConfigureArg3 = { "priority",          iocshArgInt};
static const iocshArg fpConfigureArg4 = { "Event clock rate", iocshArgDouble};
static const iocshArg *fpConfigureArgs[] = {
                      &fpConfigureArg0, &fpConfigureArg1,
                      &fpConfigureArg2, &fpConfigureArg3,
                      &fpConfigureArg4 };
static const iocshFuncDef fpConfigureFuncDef =
                                  {"fastProtectConfigure", 5, fpConfigureArgs};
static void fpConfigureCallFunc(const iocshArgBuf *args)
{
    fpConfigure(args[0].sval, args[1].sval,
                args[2].sval, args[3].ival, args[4].dval);
}

static void
fastProtect_RegisterCommands(void)
{
    iocshRegister(&fpConfigureFuncDef, fpConfigureCallFunc);
}
epicsExportRegistrar(fastProtect_RegisterCommands);
