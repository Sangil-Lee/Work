/* rtpInterpose.c */

/* 
 * RTP interpose interfaces for asyn.  The RTP driver builds RTP frames
 * and sends them to the drvAsynIPPort or drvAsynSerialPort drivers.  This
 * file implements the interpose interface that adds the required header information
 * for TCP, RTU serial or ASCII serial before sending the frame to the underlying
 * driver.  It removes header information from the response frame before returning it
 * to the Modbus driver.
 *
 * Author: silee
 */

#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <cantProceed.h>
#include <epicsAssert.h>
#include <epicsStdio.h>
#include <epicsString.h>
#include <osiSock.h>
#include <iocsh.h>

#include <epicsThread.h>
#include "asynDriver.h"
#include "asynOctet.h"
#include "rtpInterpose.h"
#include "rtp.h"
#include <epicsExport.h>

static char *driver="rtpInterpose";

#define DEFAULT_TIMEOUT 2.0

static unsigned short fcstab[CRCSIZE] = {
// 16 bit FCS lookup table per RFC1331 : For HDLC CRC calculation
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf, 
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7, 
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e, 
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876, 
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd, 
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5, 
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c, 
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974, 
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb, 
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3, 
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a, 
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72, 
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9, 
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1, 
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738, 
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70, 
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7, 
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff, 
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036, 
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e, 
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5, 
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd, 
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134, 
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c, 
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3, 
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb, 
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232, 
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a, 
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1, 
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9, 
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330, 
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

typedef struct rtpPvt {
    char           *portName;
    double         timeout;
    double         writeDelay;
    asynInterface  rtpInterface;
    asynOctet      *pasynOctet;           /* Table for low level driver */
    void           *octetPvt;
    asynUser       *pasynUser;
    char           buffer[MAX_RTP_FRAME_SIZE];
} rtpPvt;
    
/* asynOctet methods */
static asynStatus writeIt(void *ppvt,asynUser *pasynUser,
    const char *data,size_t numchars,size_t *nbytesTransfered);

#if 0
static asynStatus readIt(void *ppvt,asynUser *pasynUser,
    char *data,size_t maxchars,size_t *nbytesTransfered,int *eomReason);

static asynStatus flushIt(void *ppvt,asynUser *pasynUser);
static asynStatus registerInterruptUser(void *ppvt,asynUser *pasynUser,
    interruptCallbackOctet callback, void *userPvt,void **registrarPvt);

static asynStatus cancelInterruptUser(void *drvPvt,asynUser *pasynUser,
     void *registrarPvt);

static asynStatus setInputEos(void *ppvt,asynUser *pasynUser,
    const char *eos,int eoslen);

static asynStatus getInputEos(void *ppvt,asynUser *pasynUser,
    char *eos,int eossize ,int *eoslen);

static asynStatus setOutputEos(void *ppvt,asynUser *pasynUser,
    const char *eos,int eoslen);

static asynStatus getOutputEos(void *ppvt,asynUser *pasynUser,
    char *eos,int eossize,int *eoslen);

static asynOctet octet = {
    writeIt,readIt,flushIt,
    registerInterruptUser, cancelInterruptUser,
    setInputEos,getInputEos,setOutputEos,getOutputEos
};
#endif
static asynOctet octet = {
    writeIt,NULL,NULL,
    NULL, NULL,
    NULL,NULL,NULL,NULL
};


epicsShareFunc int rtpInterposeConfig(const char *portName,
                                int timeoutMsec, int writeDelayMsec)
{
    rtpPvt			*pPvt;
    asynInterface	*pasynInterface;
    asynStatus		status;
    asynUser		*pasynUser;

    pPvt = callocMustSucceed(1, sizeof(*pPvt), "rtpInterposeConfig");
    pPvt->portName = epicsStrDup(portName);
    pPvt->timeout = timeoutMsec/1000.;
    pPvt->writeDelay = writeDelayMsec/1000.;

    if (pPvt->timeout == 0.0) pPvt->timeout = DEFAULT_TIMEOUT;

    pPvt->rtpInterface.interfaceType = asynOctetType;
    pPvt->rtpInterface.pinterface = &octet;
    pPvt->rtpInterface.drvPvt = pPvt;
    pasynUser = pasynManager->createAsynUser(0,0);
    pPvt->pasynUser = pasynUser;
    pPvt->pasynUser->userPvt = pPvt;
    status = pasynManager->connectDevice(pasynUser,portName,0);
    if(status!=asynSuccess) {
        printf("%s connectDevice failed\n",portName);
        goto bad;
    }
    /* Find the asynOctet interface */
    pasynInterface = pasynManager->findInterface(pasynUser, asynOctetType, 1);
    if (!pasynInterface) {
        printf("%s findInterface error for asynOctetType %s\n",
               portName, pasynUser->errorMessage);
        goto bad;
    }
    
    status = pasynManager->interposeInterface(portName, 0,
       &pPvt->rtpInterface, &pasynInterface);
    if(status!=asynSuccess) {
        printf("%s interposeInterface failed\n", portName);
        goto bad;
    }
    pPvt->pasynOctet = (asynOctet *)pasynInterface->pinterface;
    pPvt->octetPvt = pasynInterface->drvPvt;

    return(0);
    
    bad:
    pasynManager->freeAsynUser(pasynUser);
    free(pPvt);
    return -1;
}

static unsigned short computeCRC(unsigned char *writecmd, int loopcnt) 
{
	int i;

	unsigned short fcsval = FCSINIT;
	
	for(i = 1; i < loopcnt; i++)
	{
		fcsval = ((fcsval >> 8) & 0xff) ^ fcstab[(fcsval ^ writecmd[i]) & 0xff]; 
	}
	
	writecmd[loopcnt] = fcsval & 0xff;
	writecmd[loopcnt + 1] = (fcsval >> 8) & 0xff;
	
	return fcsval;
}

static void encodeASCII(char *buffer, unsigned char value) 
{
    unsigned char temp;
    
    temp = value >> 4;
    if (temp < 10) temp+= '0'; else temp+= 'A'-10;
    *buffer++ = (char)temp;
    temp = value & 0x0F;
    if (temp < 10) temp+= '0'; else temp+= 'A'-10;
    *buffer = (char)temp;
}

static void decodeASCII(char *buffer, char *value) 
{
    char temp;
    unsigned char uvalue;
    
    temp = *buffer++;
    if (temp > '9') uvalue = temp - 'A' + 10; else uvalue = temp - '0';
    uvalue = uvalue << 4;
    temp = *buffer;
    if (temp > '9') uvalue += temp - 'A' + 10; else uvalue += temp - '0';
    *value = (char)uvalue;
}


/* asynOctet methods */
static asynStatus writeIt(void *ppvt, asynUser *pasynUser,
                          const char *data, size_t numchars,
                          size_t *nbytesTransfered)
{
    rtpPvt  *pPvt = (rtpPvt *)ppvt;
    asynStatus status = asynSuccess;
    size_t     nbytesActual = 0;
    size_t     nWrite;
    rtpMHeader mrtpHeader;
    unsigned char syncByte=SYNC_BYTE;
    unsigned char cpuNode = 0;
    unsigned char readType= 3;
    int mbapSize = sizeof(rtpMHeader);
    unsigned char CRC_Hi;
    unsigned char CRC_Lo;
    unsigned char LRC;
    char *pout;
    int i;

    if (pPvt->writeDelay > 0.0) epicsThreadSleep(pPvt->writeDelay);
    
    pasynUser->timeout = pPvt->timeout;

	/* Build the MBAP header */
	mrtpHeader.syncByte    = htons(syncByte);
	mrtpHeader.lengthL     = htons(readType);
	mrtpHeader.lengthH     = htons(readType);
	mrtpHeader.cpuNode     = htons(cpuNode);

	/* Copy the MBAP header to the local buffer */
	memcpy(pPvt->buffer, &mrtpHeader, mbapSize);

	/* Copy the Modbus data to the local buffer */
	memcpy(pPvt->buffer + mbapSize, data, numchars);

	/* Send the frame with the underlying driver */
	nWrite = numchars + mbapSize;
	status = pPvt->pasynOctet->write(pPvt->octetPvt, pasynUser,
			pPvt->buffer, nWrite, 
			&nbytesActual);
	*nbytesTransfered = (nbytesActual > numchars) ? numchars : nbytesActual;

    return status;
}


static asynStatus readIt(void *ppvt, asynUser *pasynUser,
                         char *data, size_t maxchars, size_t *nbytesTransfered,
                         int *eomReason)
{
    rtpPvt *pPvt = (rtpPvt *)ppvt;
    size_t nRead;
    size_t nbytesActual;
    asynStatus status = asynSuccess;
    int mbapSize = sizeof(rtpMHeader);
    unsigned char CRC_Hi;
    unsigned char CRC_Lo;
    unsigned char LRC;
    int i;
    char *pin;

    pasynUser->timeout = pPvt->timeout;

    /* Set number read to 0 in case of errors */
    *nbytesTransfered = 0;
    
	nRead = maxchars + mbapSize + 1;
	status = pPvt->pasynOctet->read(pPvt->octetPvt, pasynUser,
			pPvt->buffer, nRead, 
			&nbytesActual, eomReason);
	if (status != asynSuccess) return status;
	/* Copy bytes beyond mrtpHeader to output buffer */
	nRead = nbytesActual;
	nRead = nRead - mbapSize - 1;
	if (nRead < 0) nRead = 0;
	if (nRead > maxchars) nRead = maxchars;
	if (nRead > 0) memcpy(data, pPvt->buffer + mbapSize + 1, nRead);
	if(nRead<maxchars) data[nRead] = 0; /*null terminate string if room*/
	*nbytesTransfered = nRead;

    return status;
}


static asynStatus flushIt(void *ppvt, asynUser *pasynUser)
{
    rtpPvt *pPvt = (rtpPvt *)ppvt;
    return pPvt->pasynOctet->flush(pPvt->octetPvt, pasynUser);
}

static asynStatus registerInterruptUser(void *ppvt, asynUser *pasynUser,
    interruptCallbackOctet callback, void *userPvt, void **registrarPvt)
{
    rtpPvt *pPvt = (rtpPvt *)ppvt;
    return pPvt->pasynOctet->registerInterruptUser(pPvt->octetPvt,
                                               pasynUser, callback, userPvt, 
                                               registrarPvt);
} 

static asynStatus cancelInterruptUser(void *drvPvt, asynUser *pasynUser,
     void *registrarPvt)
{
    rtpPvt *pPvt = (rtpPvt *)drvPvt;
    return pPvt->pasynOctet->cancelInterruptUser(pPvt->octetPvt,
                                             pasynUser,registrarPvt);
} 

static asynStatus setInputEos(void *ppvt, asynUser *pasynUser,
    const char *eos, int eoslen)
{
    rtpPvt *pPvt = (rtpPvt *)ppvt;
    return pPvt->pasynOctet->setInputEos(pPvt->octetPvt, pasynUser,
                                     eos,eoslen);
}

static asynStatus getInputEos(void *ppvt, asynUser *pasynUser,
    char *eos, int eossize, int *eoslen)
{
    rtpPvt *pPvt = (rtpPvt *)ppvt;
    return pPvt->pasynOctet->getInputEos(pPvt->octetPvt, pasynUser,
                                     eos, eossize, eoslen);
}
static asynStatus setOutputEos(void *ppvt, asynUser *pasynUser,
    const char *eos, int eoslen)
{
    rtpPvt *pPvt = (rtpPvt *)ppvt;
    return pPvt->pasynOctet->setOutputEos(pPvt->octetPvt, pasynUser,
                                     eos,eoslen);
}

static asynStatus getOutputEos(void *ppvt, asynUser *pasynUser,
    char *eos, int eossize, int *eoslen)
{
    rtpPvt *pPvt = (rtpPvt *)ppvt;
    return pPvt->pasynOctet->getOutputEos(pPvt->octetPvt, pasynUser,
                                     eos, eossize, eoslen);
}


/* register rtpInterposeConfig*/
static const iocshArg rtpInterposeConfigArg0 = { "portName", iocshArgString };
static const iocshArg rtpInterposeConfigArg1 = { "timeout (msec)", iocshArgInt };
static const iocshArg rtpInterposeConfigArg2 = { "write delay (msec)", iocshArgInt };
static const iocshArg *rtpInterposeConfigArgs[] = {
                                                    &rtpInterposeConfigArg0,
                                                    &rtpInterposeConfigArg1,
                                                    &rtpInterposeConfigArg2};
static const iocshFuncDef rtpInterposeConfigFuncDef =
    {"rtpInterposeConfig", 3, rtpInterposeConfigArgs};
static void rtpInterposeConfigCallFunc(const iocshArgBuf *args)
{
    rtpInterposeConfig(args[0].sval,
                          args[1].ival,
                          args[2].ival);
}

static void rtpInterposeRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&rtpInterposeConfigFuncDef, rtpInterposeConfigCallFunc);
    }
}
epicsExportRegistrar(rtpInterposeRegister);
