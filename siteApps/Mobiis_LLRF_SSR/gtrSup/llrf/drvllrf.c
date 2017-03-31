/* Author:   Marty Kraimer */
/* Date:     17SEP2001     */

/*************************************************************************
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
* National Laboratory, and the Regents of the University of California, as
* Operator of Los Alamos National Laboratory. EPICS BASE Versions 3.13.7
* and higher are distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution.
*************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

#include <menuFtype.h>
#include <epicsInterrupt.h>
#include <epicsThread.h>
#include <epicsExit.h>
#include <epicsExport.h>
#include <epicsTimer.h>

#include <osiSock.h>
#include <osdSock.h>

#include "ellLib.h"
#include "errlog.h"
#include "devLib.h"
#include "drvSup.h"

#include "drvGtr.h"
#include "drvllrf.h"

#define STATIC static
/* memory map register offset 1s  and sizes*/


#define MAX_RECV_QUEUE	1
#define LLRF_CARD_SIZE	2

typedef struct _RECV_QUEUE_ {
    uint16 	raw[LLRF_CHANNEL][LLRF_RAW_SIZE];
    double 	mean[LLRF_CHANNEL][MEAN_READ_DATA];
	double	dbmPower[LLRF_CHANNEL];
    double 	temper[TEMPERATURE_DEV];

	/*add auto step Amp,Phase : stepAmp, stepPhase*/ 
	double	stepAmp[1];
	double	stepPhase[1];	

	/* modified by hugh: Add powerState variable */
	double  outputDBM;
	uint16 powerState[LLRF_CHANNEL];
	uint16 curMode;
	uint16 adc_detect[LLRF_CHANNEL];
	uint16 amp_range[LLRF_CHANNEL];
	int front;
	int rear;
} RECV_QUEUE;

typedef struct _STEP_TARGET_ {
	int mode_phase;	
	int mode_amp;	
	double target_phase;	
	double target_amp;	
	int32 time_interval;

	/*Auto Step 을 1이라고 하면 1,2,3,4....으로 저장 되는 변수 선언: sum_Tamp, sum_Tphase */
	double sum_Tamp;	
	double sum_Tphase;	
} STEP_TARGET;

static RECV_QUEUE gCirQueue[LLRF_CARD_SIZE];

static ELLLIST llrfList;
static int32 llrfIsInited = 0;
static int32 isRebooting;
static int32 llrfDebug = 1;

static int32 g_sock = -1;
static uint32 aliveCount = 0;

epicsThreadId g_threadid;
static epicsTimerQueueId g_timerQueue;
static epicsTimerId g_timerInit = 0;
static epicsTimerId timerReboot = 0;
static double timerDelay = 2;
static int g_iRecvTimeout = 5;
static double g_timerDelay = 2.0;

static char g_ipAddr[16] = {0,};
static uint16 g_port = 3600;
static struct sockaddr_in hostAddr;
static struct sockaddr_in ServerAddr;

static LLRF_MEAN_INFO 	meanData;
static LLRF_RAW_INFO 	rawData;

static uint32	g_runMode = 0;

static int32 flag = 0;
static epicsTimeStamp g_startTime;

#define ERR			printf

void llrfIH(void *arg);

void DBG(const char* fmt, ...)
{
	va_list ap;
	char buf[2048] = {0,};

	if( llrfDebug == 0 )
		return ;
	
	va_start(ap, fmt);
	vsnprintf(buf, sizeof(buf),fmt, ap);

	printf(buf);
}

static int sendUDPData(unsigned long card, LLRF_CTRL_INFO *sendData)
{
	int status = 0;

	sendData->tag = 0;
	sendData->card_id = card;
	
	status = sendto(g_sock, (void*)sendData, sizeof(LLRF_CTRL_INFO), 0, (struct sockaddr*)&ServerAddr, sizeof(ServerAddr));
	if ( status < 0 ) {
		int errnoCpy = SOCKERRNO;
		/*
		 * Different OS return different codes when the repeater isnt running.
		 * Its ok to supress these messages because I print another warning message
		 * if we time out registerring with the repeater.
		 *
		 * Linux returns SOCK_ECONNREFUSED
		 * Windows 2000 returns SOCK_ECONNRESET
		 */
		if (	errnoCpy != SOCK_EINTR && 
				errnoCpy != SOCK_ECONNREFUSED && 
				errnoCpy != SOCK_ECONNRESET ) {
			char sockErrBuf[64];
			epicsSocketConvertErrnoToString ( 
				sockErrBuf, sizeof ( sockErrBuf ) );
			fprintf ( stderr, "error sending registration message to CA repeater daemon was \"%s\"\n", 
				sockErrBuf );
		}
	}
	return 0;
}

static int sendCmd(llrfInfo *pllrfInfo, uint32 cmd, int32 attr, int32 value)
{
	LLRF_CTRL_INFO sendData;

	if( g_sock == (-1) ) {
		ERR("[%s, %d] g_sock : %d, %d\n", __FUNCTION__, __LINE__, g_sock, cmd);
		return (gtrStatusError);
	}
	sendData.cmd = cmd;

	switch( cmd ) {
		case CMD_REQ_INIT :
			break;
		case CMD_ACK_INIT :
			sendData.value[ATTR_RUN_MODE] = pllrfInfo->iRunMode;
			sendData.value[ATTR_RMS_SAMPLE] = pllrfInfo->iRmsSample;
			sendData.value[ATTR_AMP_OFFSET1_ON] = pllrfInfo->iAmpOffset[0];
			sendData.value[ATTR_AMP_OFFSET2_ON] = pllrfInfo->iAmpOffset[1];
			sendData.value[ATTR_AMP_OFFSET3_ON] = pllrfInfo->iAmpOffset[2];
			
			sendData.value[ATTR_AMP_OFFSET1_OFF] = pllrfInfo->iAmpOffset[3];
			sendData.value[ATTR_AMP_OFFSET2_OFF] = pllrfInfo->iAmpOffset[4];
			sendData.value[ATTR_AMP_OFFSET3_OFF] = pllrfInfo->iAmpOffset[5];

			sendData.value[ATTR_AMP_LOSS1] = pllrfInfo->iAmpOffset[6];
			sendData.value[ATTR_AMP_LOSS2] = pllrfInfo->iAmpOffset[7];
			sendData.value[ATTR_AMP_LOSS3] = pllrfInfo->iAmpOffset[8];
			
			sendData.value[ATTR_PAC_AMPLITUDE] = pllrfInfo->iPacAmplitude;
			sendData.value[ATTR_PAC_PHASE] = pllrfInfo->iPacPhase;
			sendData.value[ATTR_PAC_OFFSET] = pllrfInfo->iPacOffset;
			sendData.value[ATTR_PHASE_FB] = pllrfInfo->bPhaseFB;
			sendData.value[ATTR_AMPLITUDE_FB] = pllrfInfo->bAmplitudeFB;

			/*add PID  : pKp, aKp, pKi, aKi, pKd, aKd, udp로 FPGA로 설정값 보내 줌*/  
			sendData.value[ATTR_PKP] = pllrfInfo->iPKp;
			sendData.value[ATTR_AKP] = pllrfInfo->iAKp;
			sendData.value[ATTR_PKI] = pllrfInfo->iPKi;
			sendData.value[ATTR_AKI] = pllrfInfo->iAKi;
			sendData.value[ATTR_PKD] = pllrfInfo->iPKd;
			sendData.value[ATTR_AKD] = pllrfInfo->iAKd;			
			
			sendData.value[ATTR_MAX_AMPLITUDE1] = pllrfInfo->iMaxAmplitude[0];
			sendData.value[ATTR_MAX_AMPLITUDE2] = pllrfInfo->iMaxAmplitude[1];
			sendData.value[ATTR_AUDIO_FREQ] = pllrfInfo->iAudioFreq;
			sendData.value[ATTR_AUDIO_MUTE] = pllrfInfo->bAudioMute;
			sendData.value[ATTR_TRIGGER_SYNCPOL] = pllrfInfo->bTriggerSynPol;
			sendData.value[ATTR_TRIGGER_ONTIME] = pllrfInfo->iTriggerOnTime;
			sendData.value[ATTR_TRIGGER_IN] = pllrfInfo->iTriggerIn;
			sendData.value[ATTR_TRIGGER_OUT] = pllrfInfo->iTriggerOut;
			sendData.value[ATTR_TRIGGER_FREQ] = pllrfInfo->iTriggerFreq;
			sendData.value[ATTR_POWERON] = pllrfInfo->iPowerOn;
			sendData.value[ATTR_REFRESH] = pllrfInfo->iReFresh;

			/*add auto step Amp,Phase : stepAmp, stepPhase , udp로 FPGA로 설정값 보내 줌*/ 
			sendData.value[ATTR_STEP_SIZE_AMPLITUDE] = pllrfInfo->istepSizeAmp;
			sendData.value[ATTR_STEP_SIZE_PHASE] = pllrfInfo->istepSizePhase;		

			
			sendData.value[ATTR_TIME_INTERVAL] = pllrfInfo->itimeInterval;

			/*Auto Step Mode On/Off : stepModAmp, stepModPhase , udp로 FPGA로 설정값 보내 줌*/
			sendData.value[ATTR_STEPMODE_AMP] = pllrfInfo->bstepModAmp;
			sendData.value[ATTR_STEPMODE_PHASE] = pllrfInfo->bstepModPhase;

			sendData.value[ATTR_AMP_HIGH_LIMIT] = pllrfInfo->iAmpHighLimit;

			/* modified by hugh */
			sendData.value[ATTR_AMP_ON1] = pllrfInfo->bAmpOn[0];
			sendData.value[ATTR_AMP_ON2] = pllrfInfo->bAmpOn[1];
			sendData.value[ATTR_AMP_ON3] = pllrfInfo->bAmpOn[2];
			sendData.value[ATTR_FB_CH] = pllrfInfo->bFBCh;
			sendData.value[ATTR_CLK_SEL] = pllrfInfo->iClockSelect;
			break;
		case CMD_ATTR :
			sendData.value[0] = attr;
			sendData.value[1] = value;
			break;
		default :
			ERR("[%s, %d] Invalid arg : %d\n", __FUNCTION__, __LINE__, cmd);
			return (gtrStatusError);
	}

	return sendUDPData(pllrfInfo->card, &sendData);
}

static int pushQueueNormal(RECV_QUEUE *queue, LLRF_MEAN_INFO *normal)
{
	int rear = 0;

	rear = (queue->rear + 1) % MAX_RECV_QUEUE;

	/* queue full */
	if( (queue->front == rear) && (MAX_RECV_QUEUE != 1) )
		return FALSE;

	memcpy((void*)queue->mean, (void*)normal->data, sizeof(normal->data));
	memcpy((void*)queue->temper, (void*)normal->temper, sizeof(normal->temper));

	/*add auto step Amp,Phase : stepAmp, stepPhase*/ 
	memcpy((void*)queue->stepAmp,(void*)normal->stepAmp, sizeof(normal->stepAmp));
	memcpy((void*)queue->stepPhase,(void*)normal->stepPhase, sizeof(normal->stepPhase));

	/*modified by hugh: power state */
	memcpy((void*)queue->powerState,(void*)normal->powerState, sizeof(normal->powerState));
	memcpy((void*)queue->adc_detect, (void*)normal->adc_detect, sizeof(normal->adc_detect));
	memcpy((void*)queue->amp_range, (void*)normal->amp_range, sizeof(normal->amp_range));
	
	queue->outputDBM = normal->outputDBM;

	/* jmk 여기서 vol -> mean 으로 변경해서 넣기으면 됨. 
	계산식이 기억안남 ㅠ,.ㅠ
	mean 이게 sdk 에서 넘어온 voltage 값임.
	
	queue->dbmPower[0] = queue->mean[0][INDEX_AMP_MEAN];
	queue->dbmPower[1] = queue->mean[1][INDEX_AMP_MEAN];
	queue->dbmPower[2] = queue->mean[2][INDEX_AMP_MEAN];
	*/
	queue->dbmPower[0] = 10 * log10( (queue->mean[0][INDEX_AMP_MEAN] / 1000) * (queue->mean[0][INDEX_AMP_MEAN] / 1000) * 20);
	queue->dbmPower[1] = 10 * log10( (queue->mean[1][INDEX_AMP_MEAN] / 1000) * (queue->mean[1][INDEX_AMP_MEAN] / 1000) * 20);	
	queue->dbmPower[2] = 10 * log10( (queue->mean[2][INDEX_AMP_MEAN] / 1000) * (queue->mean[2][INDEX_AMP_MEAN] / 1000) * 20);
	queue->rear = rear;
	
	return TRUE;
}

static int pushQueueGraph(RECV_QUEUE *queue, LLRF_RAW_INFO *graph)
{
	int rear = 0;

	rear = (queue->rear + 1) % MAX_RECV_QUEUE;

	/* queue full */
	if( (queue->front == rear) && (MAX_RECV_QUEUE != 1) )
		return FALSE;

	memcpy((void*)queue->mean, (void*)graph->data, sizeof(graph->data));
	memcpy((void*)queue->raw, (void*)graph->raw, sizeof(graph->raw));
	memcpy((void*)queue->temper, (void*)graph->temper, sizeof(graph->temper));

	queue->dbmPower[0] = 10 * log10( (queue->mean[0][INDEX_AMP_MEAN] / 1000) * (queue->mean[0][INDEX_AMP_MEAN] / 1000) * 20);
	queue->dbmPower[1] = 10 * log10( (queue->mean[1][INDEX_AMP_MEAN] / 1000) * (queue->mean[1][INDEX_AMP_MEAN] / 1000) * 20);	
	queue->dbmPower[2] = 10 * log10( (queue->mean[2][INDEX_AMP_MEAN] / 1000) * (queue->mean[2][INDEX_AMP_MEAN] / 1000) * 20);

	queue->rear = rear;
	
	return TRUE;
}

static void initHandler(void *pPrivate)
{   
	llrfInfo *pllrfInfo = (llrfInfo *)pPrivate;

	sendCmd(pllrfInfo, CMD_ACK_INIT, 0, 0);

}

static void rebootHandler(void *pPrivate)
{   
//	llrfReboot(NULL);

	exit(1);
}

void print_mean_packet()
{
	int i, j;

	printf("cmd: %d\n", meanData.cmd);
	printf("card_id: %d\n", meanData.card_id);
	printf("tag: %d\n", meanData.tag);
	putchar('\n');

	printf("--- Temperature ---\n");
	for (i = 0; i < TEMPERATURE_DEV; i++)
		printf("temper[%d]: %lf\n", i, meanData.temper[i]);
	putchar('\n');

	printf("--- data ---\n");
	for (i = 0; i < LLRF_CHANNEL; i++) {
		for (j = 0; j < MEAN_READ_DATA; j++) {
			printf("data[%d][%d]: %lf\n", i, j, meanData.data[i][j]);
		}
	}
	putchar('\n');

	printf("stepAmp: %lf\n", meanData.stepAmp[0]);
	printf("stepPhase: %lf\n", meanData.stepPhase[0]);
	printf("outputDBM: %lf\n", meanData.outputDBM);
	putchar('\n');

	for (i = 0; i < LLRF_CHANNEL; i++)
		printf("powerState[%d]: %d\n", i, meanData.powerState[i]);

	for (i = 0; i < LLRF_CHANNEL; i++)
		printf("adc_detect[%d]: %d\n", i, meanData.adc_detect[i]);
	for (i = 0; i < LLRF_CHANNEL; i++)
		printf("amp_range[%d]: %d\n", i, meanData.amp_range[i]);
}

void print_raw_packet()
{
	int i, j;

	printf("cmd: %d\n", rawData.cmd);
	printf("card_id: %d\n", rawData.card_id);
	printf("tag: %d\n", rawData.tag);
	putchar('\n');

	printf("--- Temperature ---\n");
	for (i = 0; i < TEMPERATURE_DEV; i++)
		printf("temper[%d]: %lf\n", i, rawData.temper[i]);
	putchar('\n');

	printf("--- data ---\n");
	for (i = 0; i < LLRF_CHANNEL; i++) {
		for (j = 0; j < MEAN_READ_DATA; j++) {
			printf("data[%d][%d]: %lf\n", i, j, rawData.data[i][j]);
		}
	}
	putchar('\n');

	printf("--- raw data ---\n");
	for (i = 0; i < LLRF_CHANNEL; i++) {
		for (j = 0; j < LLRF_RAW_SIZE; j++) {
			if (j && !(j%8)) putchar('\n');
			printf("raw[%3d][%3d]: %3d\t", i, j, rawData.raw[i][j]);
		}
		putchar('\n');
	}
	putchar('\n');
	putchar('\n');
}

void reassemble_mean_data(char *buf)
{
	int i, j;
	char *pos = buf;

	meanData.cmd = *((unsigned int *)pos);
	pos += sizeof(unsigned int);

	meanData.card_id = *((unsigned int *)pos);
	pos += sizeof(unsigned int);

	meanData.tag = *((unsigned int *)pos);
	pos += sizeof(unsigned int);

	for (i = 0; i < TEMPERATURE_DEV; i++) {
		meanData.temper[i] = *((double *)pos);
		pos += sizeof(double);
	}

	for (i = 0; i < LLRF_CHANNEL; i++) {
		for (j = 0; j < MEAN_READ_DATA; j++) {
			meanData.data[i][j] = *((double *)pos);
			pos += sizeof(double);
		}
	}

	meanData.stepAmp[0] = *((double *)pos);
	pos += sizeof(double);

	meanData.stepPhase[0] = *((double *)pos);
	pos += sizeof(double);

	meanData.outputDBM = *((double *)pos);
	pos += sizeof(double);

	for (i = 0; i < LLRF_CHANNEL; i++) {
		meanData.powerState[i] = *((unsigned short *)pos);
		pos += sizeof(unsigned short);
	}

	for (i = 0; i < LLRF_CHANNEL; i++) {
		meanData.adc_detect[i] = *((unsigned short *)pos);
		pos += sizeof(unsigned short);
	}
 
	for (i = 0; i < LLRF_CHANNEL; i++) {
		meanData.amp_range[i] = *((unsigned short *)pos);
		pos += sizeof(unsigned short);
	}
}

void reassemble_raw_data(char *buf)
{
	int i, j;
	char *pos = buf;

	rawData.cmd = *((unsigned int *)pos);
	pos += sizeof(unsigned int);

	rawData.card_id = *((unsigned int *)pos);
	pos += sizeof(unsigned int);

	rawData.tag = *((unsigned int *)pos);
	pos += sizeof(unsigned int);

	for (i = 0; i < TEMPERATURE_DEV; i++) {
		rawData.temper[i] = *((double *)pos);
		pos += sizeof(double);
	}

	for (i = 0; i < LLRF_CHANNEL; i++) {
		for (j = 0; j < MEAN_READ_DATA; j++) {
			rawData.data[i][j] = *((double *)pos);
			pos += sizeof(double);
		}
	}

	for (i = 0; i < LLRF_CHANNEL; i++) {
		for (j = 0; j < LLRF_RAW_SIZE; j++) {
			rawData.raw[i][j] = *((unsigned short *)pos);
			pos += sizeof(unsigned short);
		}
	}
}

/*
	llrf Receive Handler
*/
static void * llrf_Recv_Routine(void *arg)
{
	int ret = 0;
	struct timeval timeout;
	static fd_set rset;
	static struct sockaddr_in Addr;
	socklen_t clientlen = sizeof(Addr);
	int readlen = 0;
	LLRF_HEADER header;
	uint32 magic, maxsize, curloc, cursize;
	char *pos = NULL;
	char buf[2048];
	uint32	cmd = 0;
	int bReconnect = 0;

	/* Temporary buffer */
	char tMeanBuf[246];
	char tRawBuf[876];
	
	llrfInfo *pllrfInfo = (llrfInfo *)arg;

/*
	DBG("######### UND_RSP_DATA : %d\n", sizeof(llrfRspData));
	DBG("######### LLRF_CTRL_INFO : %d\n", sizeof(llrfReqData));
	DBG("######### AXIS_REF_STATUS %d\n", sizeof(AXIS_REF_STATUS));
	DBG("######### NCTOPLC_AXIS_REF %d\n", sizeof(NCTOPLC_AXIS_REF));
	DBG("######### PLCTONC_AXIS_REF %d\n", sizeof(PLCTONC_AXIS_REF));
	DBG("######### AxisParameterSet %d\n", sizeof(AxisParameterSet));
*/

	memset((void*)&meanData, 0, sizeof(meanData));
	memset((void*)&rawData, 0, sizeof(rawData));

	while(!isRebooting){
		if( g_sock != (-1) ) {
			FD_ZERO(&rset);
			FD_SET(g_sock, &rset);

			timeout.tv_sec = g_iRecvTimeout;
			timeout.tv_usec = 0;

			ret = select(g_sock + 1, &rset, NULL, NULL, &timeout);
			if( ret == 0 ) {
				DBG("select timeout.\n");
				closeUDPSocket();
				initUDPSocket();
				bReconnect = 1;
				continue;
			}

			if(bReconnect) {
				bReconnect = 0;
				sendCmd(pllrfInfo, CMD_ACK_INIT, 0, 0);
			}
			
			if( FD_ISSET(g_sock, &rset) ) {
				readlen = recvfrom(g_sock, (void*)&header, sizeof(header), MSG_PEEK, (struct sockaddr*)&Addr, &clientlen);
				if( readlen < 0 ) {
					continue;
				}
	
				magic = header.magic;
				maxsize = header.maxsize;
				curloc = header.curloc;
				cursize = header.cursize;
	
				/* 0x2106E84 : normal,	0x584814 : graph */
				/*printf("##### magic : 0x%x\n", magic);*/
				if( 0x2106E84 == magic ) {
#if 0
					pos = ((char*)&meanData + curloc);
					readlen = recvfrom(g_sock, (void*)buf, sizeof(header) + cursize, 0, (struct sockaddr*)&Addr, &clientlen);
					if( maxsize > (curloc + cursize) )
						continue;
	
					memcpy(pos, &buf[sizeof(header)], cursize);
					cmd = meanData.cmd;
					g_runMode = RUN_NORMAL;
#else
					memset(tMeanBuf, 0, 246);
					readlen = recvfrom(g_sock, (void *)buf, sizeof(header) + cursize, 0, (struct sockaddr *)&Addr, &clientlen);
					if (maxsize > (curloc + cursize))
						continue;

					memcpy(tMeanBuf, &buf[sizeof(header)], cursize);
					reassemble_mean_data(tMeanBuf);
					cmd = meanData.cmd;
					g_runMode = RUN_NORMAL;
#endif
//					print_mean_packet();
				} else if( 0x584814 == magic ) {
#if 0
					pos = ((char*)&rawData + curloc);
					readlen = recvfrom(g_sock, (void*)buf, sizeof(header) + cursize, 0, (struct sockaddr*)&Addr, &clientlen);
					memcpy(pos, &buf[sizeof(header)], cursize);
					if( maxsize > (curloc + cursize) )
						continue;
					
					cmd = rawData.cmd;
					g_runMode = RUN_GRAPH;
#else
					memset(tRawBuf, 0, 876);
					readlen = recvfrom(g_sock, (void *)buf, sizeof(header) + cursize, 0, (struct sockaddr *)&Addr, &clientlen);
					if (maxsize > (curloc + cursize))
						continue;

					memcpy(tRawBuf, &buf[sizeof(header)], cursize);
					reassemble_raw_data(tRawBuf);
					cmd = rawData.cmd;
					g_runMode = RUN_GRAPH;
#endif
//					print_raw_packet();
				} else {
					readlen = recvfrom(g_sock, (void*)&buf, sizeof(buf), 0, (struct sockaddr*)&Addr, &clientlen);
					printf("##### [%s,%d] 0x%x\n", __FUNCTION__, __LINE__, magic);
					continue;
				}
				
				switch (cmd) {
					case CMD_LIVE_DATA :
						if( RUN_NORMAL == g_runMode ) {
							pushQueueNormal(&gCirQueue[meanData.card_id], &meanData);
							gCirQueue[meanData.card_id].curMode = RUN_NORMAL;
						} else if( RUN_GRAPH == g_runMode ) {
							pushQueueGraph(&gCirQueue[rawData.card_id], &rawData);
							gCirQueue[rawData.card_id].curMode = RUN_GRAPH;
						}
						memset((void*)&meanData, 0, sizeof(meanData));
						memset((void*)&rawData, 0, sizeof(rawData));
						llrfIH(pllrfInfo);	
						break;
					case CMD_REQ_INIT :
						printf("This is req packet!!\n");
						sendCmd(pllrfInfo, CMD_ACK_INIT, 0, 0);
						break;
					default :
						printf("## [%s,%d] : default cmd, %ld\n", __FUNCTION__, __LINE__, cmd);
						break;
				}

			}
		} else {
			sleep(1);
//			printf("#################............check........#################\n");
			printf("#################............Socket allocate is failed...Restart IOC........#################\n");
			exit(1);
		}		
	}	
	DBG("[Und_Recv_Routine] exit.\n");

    isRebooting = 1;
	timerDelay = 6;

	if( 0 != timerReboot ) {
		timerReboot = epicsTimerQueueCreateTimer(g_timerQueue, rebootHandler, 0); 
		/* Start a timer */
		epicsTimerStartDelay(timerReboot, timerDelay);	
	}
	return NULL;
}

int closeUDPSocket(void)
{
	if( g_sock != -1 ) {
		close(g_sock);
		g_sock = -1;
	}
	return 0;
}
 
int initUDPSocket(void)
{
/*	unsigned long sndbufsize;*/

	if( (g_sock = epicsSocketCreate(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0 ) {
		printf("epicsSocketCreate() failed.\n");
		return (-1);
	}
/*
	sndbufsize = 17408;
	setsockopt (g_sock, SOL_SOCKET, SO_SNDBUF, &sndbufsize, sizeof(sndbufsize));
*/
	/* Construct the Server address structure */
	memset((void*)&hostAddr, 0, sizeof(hostAddr));
	hostAddr.sin_family = AF_INET;
	hostAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	hostAddr.sin_port = htons(g_port);

	if( bind(g_sock, (struct sockaddr*)&hostAddr, sizeof(hostAddr)) < 0 ) {
		printf("bind() failed.\n");
		closeUDPSocket();
		return (-1);
	}

	return 0;
}

static void llrfReboot(void *arg)
{
    llrfInfo  *pllrfInfo;

    isRebooting = 1;
	printf("#############################\n");
    pllrfInfo = (llrfInfo *)ellFirst(&llrfList);
    while(pllrfInfo) {

        pllrfInfo = (llrfInfo *)ellNext(&pllrfInfo->node);
    }
}
    
static void initialize()
{
    if(llrfIsInited) return;
    llrfIsInited=1;
    isRebooting = 0;
    ellInit(&llrfList);
   epicsAtExit(llrfReboot,NULL);
}

void llrfIH(void *arg)
{
    llrfInfo *pllrfInfo = (llrfInfo *)arg;

    if(pllrfInfo->usrIH) (*pllrfInfo->usrIH)(pllrfInfo->handlerPvt);
}


STATIC void llrfinit(gtrPvt pvt)
{
    llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	DBG("###### %d, %s\n", pllrfInfo->card, pllrfInfo->name);

	gCirQueue[pllrfInfo->card].front = 0;
	gCirQueue[pllrfInfo->card].rear = 0;

	if( g_threadid == 0 ) {
		initUDPSocket();
	
		g_threadid = epicsThreadCreate ("recvDaemon"
										, 99
										, epicsThreadGetStackSize (epicsThreadStackBig)
										, (EPICSTHREADFUNC)llrf_Recv_Routine
										, pvt);
	}

	/* Create the queue of timer requests */
	g_timerQueue = epicsTimerQueueAllocate(1, epicsThreadPriorityScanHigh);

	/* Create the timers */
	g_timerInit = epicsTimerQueueCreateTimer(g_timerQueue, initHandler, pllrfInfo);

	epicsTimerStartDelay(g_timerInit, g_timerDelay);

    return;
}

STATIC void llrfreport(gtrPvt pvt,int level)
{
    llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	llrfDebug = level;
#if 0
	DBG("##########################################\n");
	DBG("## %s Device Information\n");
	DBG("## state : %d\n", pllrfInfo->stRspFast.state);
	DBG("## linearEncEnabled : %d\n", pllrfInfo->stRspData.linearEncEnabled);
	DBG("## gapValue : %lf\n", pllrfInfo->stRspData.gapValue);
	DBG("## rotaryEncUT : %lf\n", pllrfInfo->stRspData.rotaryEncUT);
	DBG("## rotaryEncDT : %lf\n", pllrfInfo->stRspData.rotaryEncDT);
	DBG("## rotaryEncUB : %lf\n", pllrfInfo->stRspData.rotaryEncUB);
	DBG("## rotaryEncDB : %lf\n", pllrfInfo->stRspData.rotaryEncDB);
	DBG("## edgePosUT : %lf\n", pllrfInfo->stRspData.edgePosUT);
	DBG("## edgePosDT : %lf\n", pllrfInfo->stRspData.edgePosDT);
	DBG("## edgePosUB : %lf\n", pllrfInfo->stRspData.edgePosUB);
	DBG("## edgePosDB : %lf\n", pllrfInfo->stRspData.edgePosDB);
	DBG("## operableUT : %s\n", pllrfInfo->stRspData.operableUT == 1 ? "On" : "Off");
	DBG("## operableDT : %s\n", pllrfInfo->stRspData.operableDT == 1 ? "On" : "Off");
	DBG("## operableUB : %s\n", pllrfInfo->stRspData.operableUB == 1 ? "On" : "Off");
	DBG("## operableDB : %s\n", pllrfInfo->stRspData.operableDB == 1 ? "On" : "Off");
	DBG("## movingTopCarrier : %s\n", pllrfInfo->stRspData.movingTopCarrier == 1 ? "On" : "Off");
	DBG("## movingBottomCarrier : %s\n", pllrfInfo->stRspData.movingBottomCarrier == 1 ? "On" : "Off");
	DBG("##########################################\n");
#endif
}

STATIC gtrStatus llrftemperature(gtrPvt pvt, double *value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if( index < 0 || index >= TEMPERATURE_DEV )
		return (gtrStatusError);

	*value = gCirQueue[pllrfInfo->card].temper[index];
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfpower(gtrPvt pvt, double *value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if( index < 0 || index >= LLRF_CHANNEL )
		return (gtrStatusError);
	
	*value = gCirQueue[pllrfInfo->card].mean[index][INDEX_AMP_MEAN];
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfphase(gtrPvt pvt, double *value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if( index < 0 || index >= LLRF_CHANNEL )
		return (gtrStatusError);
	
	*value = gCirQueue[pllrfInfo->card].mean[index][INDEX_PHASE_MEAN];

	return (gtrStatusOK);
}

STATIC gtrStatus llrfrmsAmplitude(gtrPvt pvt, double *value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if( index < 0 || index >= LLRF_CHANNEL )
		return (gtrStatusError);
	
	*value = gCirQueue[pllrfInfo->card].mean[index][INDEX_RMS_AMP_SDEV];

	return (gtrStatusOK);
}

STATIC gtrStatus llrfrmsDiffPhase(gtrPvt pvt, double *value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if( index < 0 || index >= LLRF_CHANNEL )
		return (gtrStatusError);
	
	*value = gCirQueue[pllrfInfo->card].mean[index][INDEX_RMS_PHASE_DIFF_SDEV];

	return (gtrStatusOK);
}

STATIC gtrStatus llrfrmsOrgPhase(gtrPvt pvt, double *value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if( index < 0 || index >= LLRF_CHANNEL )
		return (gtrStatusError);
	
	*value = gCirQueue[pllrfInfo->card].mean[index][INDEX_RMS_PHASE_ORG_SDEV];

	return (gtrStatusOK);
}

STATIC gtrStatus llrfdBmPower(gtrPvt pvt, double *value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if( index < 0 || index >= LLRF_CHANNEL )
		return (gtrStatusError);
	
	*value = gCirQueue[pllrfInfo->card].dbmPower[index];
	
	return (gtrStatusOK);
}


/*add auto step Amp,Phase : stepAmp, stepPhase*/ 
STATIC gtrStatus llrfstepAmp(gtrPvt pvt, double *value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	*value = gCirQueue[pllrfInfo->card].stepAmp[0];

	//printf("llrfstepAmp = %lf \r\n",*value);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfstepPhase(gtrPvt pvt, double *value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	*value = gCirQueue[pllrfInfo->card].stepPhase[0];

	//printf("llrfstepAmp = %lf \r\n",*value);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfoutputDBM(gtrPvt pvt, double *value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	*value = gCirQueue[pllrfInfo->card].outputDBM;

	//printf("llrfstepAmp = %lf \r\n",*value);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfaiFunction(long parm, gtrPvt pvt, double *value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;
	long status = 0;

	switch(parm) {
		case temperature:
			status = llrftemperature(pvt,value,index);
			break;
		case power:
			status = llrfpower(pvt,value,index);
			break;
		case phase:
			status = llrfphase(pvt,value,index);
			break;
		case rmsAmplitude:
			status = llrfrmsAmplitude(pvt,value,index);
			break;
		case rmsDiffPhase:
			status = llrfrmsDiffPhase(pvt,value,index);
			break;
		case rmsOrgPhase:
			status = llrfrmsOrgPhase(pvt,value,index);
			break;
		case dBmPower:
			status = llrfdBmPower(pvt,value,index);
			break;
		case stepAmp:
			status = llrfstepAmp(pvt,value);
			break;
		case stepPhase:
			status = llrfstepPhase(pvt,value);
			break;
		case outputDBM:
			status = llrfoutputDBM(pvt,value);
			break;
	}
	return (status);
}

STATIC gtrStatus llrfampOffset(gtrPvt pvt, double value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;
	int ATTR_RECV;

	if( index < 0 || index >= 9 )
		return (gtrStatusError);	
		
	pllrfInfo->iAmpOffset[index] =(int32)(value * 10000);

	switch(index) {
		case 0 :
			ATTR_RECV = ATTR_AMP_OFFSET1_ON;
			break;
		case 1 :
			ATTR_RECV = ATTR_AMP_OFFSET2_ON;
			break;
		case 2 :
			ATTR_RECV = ATTR_AMP_OFFSET3_ON;
			break;
		case 3 :
			ATTR_RECV = ATTR_AMP_OFFSET1_OFF;
			break;
		case 4 :
			ATTR_RECV = ATTR_AMP_OFFSET2_OFF;
			break;
		case 5 :
			ATTR_RECV = ATTR_AMP_OFFSET3_OFF;
			break;
		case 6 :
			ATTR_RECV = ATTR_AMP_LOSS1;
			break;
		case 7 :
			ATTR_RECV = ATTR_AMP_LOSS2;
			break;
		case 8 :
			ATTR_RECV = ATTR_AMP_LOSS3;
			break;
	}
	printf("%d / %d / %lf\n", index, pllrfInfo->iAmpOffset[index], value);
	sendCmd(pllrfInfo, CMD_ATTR, ATTR_RECV, pllrfInfo->iAmpOffset[index]);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfpacAmplitude(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	pllrfInfo->iPacAmplitude = (int32)(value * 1000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_PAC_AMPLITUDE, pllrfInfo->iPacAmplitude);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfpacPhase(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;
 
	pllrfInfo->iPacPhase = (int32)(value * 1000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_PAC_PHASE, pllrfInfo->iPacPhase);

	return (gtrStatusOK);
}

STATIC gtrStatus llrfpacOffset(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

 	pllrfInfo->iPacOffset =  (int32)(value * 10000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_PAC_OFFSET, pllrfInfo->iPacOffset);
	
	return (gtrStatusOK);
}

/*add PID  : pKp, aKp, pKi, aKi, pKd, aKd*/
STATIC gtrStatus llrfpKp(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

 	pllrfInfo->iPKp = (int32)(value * 1000000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_PKP, pllrfInfo->iPKp);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfaKp(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

  	pllrfInfo->iAKp =  (int32)(value * 1000000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_AKP, pllrfInfo->iAKp);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfpKi(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

 	pllrfInfo->iPKi = (int32)(value * 1000000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_PKI, pllrfInfo->iPKi);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfaKi(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

  	pllrfInfo->iAKi =  (int32)(value * 1000000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_AKI, pllrfInfo->iAKi);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfpKd(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

 	pllrfInfo->iPKd = (int32)(value * 1000000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_PKD, pllrfInfo->iPKd);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfaKd(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

  	pllrfInfo->iAKd =  (int32)(value * 1000000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_AKD, pllrfInfo->iAKd);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfmaxAmplitude(gtrPvt pvt, double value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if(index < 0 || index >= 2)
		return (gtrStatusError);

   	pllrfInfo->iMaxAmplitude[index]=  (int32)(value * 1000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_MAX_AMPLITUDE1 + index, pllrfInfo->iMaxAmplitude[index]);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrftriggerOnTime(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

   	pllrfInfo->iTriggerOnTime= (int32)(value * 1000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_TRIGGER_ONTIME, pllrfInfo->iTriggerOnTime);

	return (gtrStatusOK);
}

/*add auto step Amp,Phase : stepAmp, stepPhase*/ 
STATIC gtrStatus llrfstepSizeAmp(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

   	pllrfInfo->istepSizeAmp= (int32)(value * 1000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_STEP_SIZE_AMPLITUDE, pllrfInfo->istepSizeAmp);

	return (gtrStatusOK);

}
STATIC gtrStatus llrfstepSizePhase(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

   	pllrfInfo->istepSizePhase= (int32)(value * 1000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_STEP_SIZE_PHASE, pllrfInfo->istepSizePhase);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrftimeInterval(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

   	pllrfInfo->itimeInterval= (int32)(value*1000);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_TIME_INTERVAL, pllrfInfo->itimeInterval);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfampHighLimit(gtrPvt pvt, double value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

   	pllrfInfo->iAmpHighLimit= (int32)(value*1000);

	printf("AMP:HIGH:LIMIT -> %lf\n", value);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_AMP_HIGH_LIMIT, pllrfInfo->iAmpHighLimit);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfaoFunction(long parm, gtrPvt pvt, double value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;
	long status;

	switch(parm) {
		case ampOffset:
			status = llrfampOffset(pvt,value,index);
			break;
		case pacAmplitude:
			status = llrfpacAmplitude(pvt,value);
			break;
		case pacPhase:
			status = llrfpacPhase(pvt,value);
			break;
		case pacOffset:
			status = llrfpacOffset(pvt,value);
			break;
		case pKp:
			status = llrfpKp(pvt,value);
			break;
		case aKp:
			status = llrfaKp(pvt,value);
			break;
		case pKi:
			status = llrfpKi(pvt,value);
			break;
		case aKi:
			status = llrfaKi(pvt,value);
			break;
		case pKd:
			status = llrfpKd(pvt,value);
			break;
		case aKd:
			status = llrfaKd(pvt,value);
			break;
		case maxAmplitude:
			status = llrfmaxAmplitude(pvt,value,index);
			break;
		case triggerOnTime:
			status = llrftriggerOnTime(pvt,value);
			break;
		case stepSizeAmp:
			status = llrfstepSizeAmp(pvt,value);
			break;
		case stepSizePhase:
			status = llrfstepSizePhase(pvt,value);
			break;
		case timeInterval:
			status = llrftimeInterval(pvt,value);
			break;
		case ampHighLimit:
			status = llrfampHighLimit(pvt,value);
			break;
	}
	return (status);
}

STATIC gtrStatus llrfpowerState(gtrPvt pvt, int *value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if(index < 0 || index >= 3)
		return (gtrStatusError);

	*value = gCirQueue[pllrfInfo->card].powerState[index];

	return(gtrStatusOK);
}

STATIC gtrStatus llrfcurrentMode(gtrPvt pvt, int *value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;
	
	*value = gCirQueue[pllrfInfo->card].curMode;

	return (gtrStatusOK);
}

STATIC gtrStatus llrfadcState(gtrPvt pvt, int *value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if(index < 0 || index >= 3)
		return (gtrStatusError);

	*value = gCirQueue[pllrfInfo->card].adc_detect[index];

	return (gtrStatusOK);
}

STATIC gtrStatus llrfampRange(gtrPvt pvt, int *value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if(index < 0 || index >= 3)
		return (gtrStatusError);

	*value = gCirQueue[pllrfInfo->card].amp_range[index];

	return (gtrStatusOK);
}

STATIC gtrStatus llrfbiFunction(long parm, gtrPvt pvt, int *value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;
	long status;

	switch(parm) {
		case powerState:
			status = llrfpowerState(pvt,value,index);
			break;
		case currentMode:
			status = llrfcurrentMode(pvt, value);
			break;
		case adcState:
			status = llrfadcState(pvt, value, index);
			break;
		case ampRange:
			status = llrfampRange(pvt, value, index);
			break;
	}
	
	return(status);
}

STATIC gtrStatus llrfphaseFB(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

   	pllrfInfo->bPhaseFB= value;

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_PHASE_FB, pllrfInfo->bPhaseFB);

	return (gtrStatusOK);
}

STATIC gtrStatus llrfamplitudeFB(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	pllrfInfo->bAmplitudeFB = value;

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_AMPLITUDE_FB, pllrfInfo->bAmplitudeFB);
	
	return (gtrStatusOK);
}

STATIC gtrStatus llrfaudioMute(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	pllrfInfo->bAudioMute = value;

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_AUDIO_MUTE, pllrfInfo->bAudioMute);

	return (gtrStatusOK);
}

STATIC gtrStatus llrftriggerSyncPol(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	pllrfInfo->bTriggerSynPol = value;

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_TRIGGER_SYNCPOL, pllrfInfo->bTriggerSynPol);

	return (gtrStatusOK);
}

STATIC gtrStatus llrfTriggerOut(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	pllrfInfo->iTriggerOut = value;

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_TRIGGER_OUT, pllrfInfo->iTriggerOut);

	return (gtrStatusOK);
}

STATIC gtrStatus llrfrunMode(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	pllrfInfo->iRunMode = value;

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_RUN_MODE, pllrfInfo->iRunMode);

	return (gtrStatusOK);
}

STATIC gtrStatus llrfpowerOn(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if ( pllrfInfo->iPowerOn != value ) {
		pllrfInfo->iPowerOn = value;
	
		sendCmd(pllrfInfo, CMD_ATTR, ATTR_POWERON, pllrfInfo->iPowerOn);
	}

	return (gtrStatusOK);
}
STATIC gtrStatus llrfreFresh(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	pllrfInfo->iReFresh = value;
	
	//sendCmd(pllrfInfo, CMD_ATTR, ATTR_REFRESH, pllrfInfo->iReFresh);
	sendCmd(pllrfInfo, CMD_ACK_INIT, 0, 0);
	
	return (gtrStatusOK);
}
STATIC gtrStatus llrfstepModAmp(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if (pllrfInfo->bstepModAmp != value) {
		pllrfInfo->bstepModAmp = value;
		sendCmd(pllrfInfo, CMD_ATTR, ATTR_STEPMODE_AMP, pllrfInfo->bstepModAmp);
	}
	
	return (gtrStatusOK);
}
STATIC gtrStatus llrfstepModPhase(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if( pllrfInfo->bstepModPhase != value) {
		pllrfInfo->bstepModPhase = value;
		sendCmd(pllrfInfo, CMD_ATTR, ATTR_STEPMODE_PHASE, pllrfInfo->bstepModPhase);
	}

	return (gtrStatusOK);

}

STATIC gtrStatus llrfampOn(gtrPvt pvt, int value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	if(index < 0 || index >= 3)
		return (gtrStatusError);

	pllrfInfo->bAmpOn[index] = (int32)(value);

	sendCmd(pllrfInfo, CMD_ATTR, (ATTR_AMP_ON1 + index), pllrfInfo->bAmpOn[index]);

	return (gtrStatusOK);
}

STATIC gtrStatus llrfFBCh(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	pllrfInfo->bFBCh = value;

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_FB_CH, pllrfInfo->bFBCh);

	return (gtrStatusOK);
}

STATIC gtrStatus llrfboFunction(long parm, gtrPvt pvt, int value, int index)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;
	long status = 0;

	switch(parm) {
		case phaseFB:
			status = llrfphaseFB(pvt,value);
			break;
		case amplitudeFB:
			status = llrfamplitudeFB(pvt,value);
			break;
		case audioMute:
			status = llrfaudioMute(pvt,value);
			break;
		case triggerSyncPol:
			status = llrftriggerSyncPol(pvt,value);
			break;
		case TriggerOut:
			status = llrfTriggerOut(pvt,value);
			break;
		case runMode:
			status = llrfrunMode(pvt,value);
			break;
		case powerOn:
			status = llrfpowerOn(pvt,value);
			break;
		case reFresh:
			status = llrfreFresh(pvt,value);
			break;
		case stepModAmp:
			status = llrfstepModAmp(pvt,value);
			break;
		case stepModPhase:
			status = llrfstepModPhase(pvt,value);
			break;
		case ampOn:
			status = llrfampOn(pvt,value,index);
			break;
		case FBCh:
			status = llrfFBCh(pvt, value);
			break;
	}
	return (status);
}

STATIC gtrStatus llrfrmsSample(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	pllrfInfo->iRmsSample = value;

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_RMS_SAMPLE, pllrfInfo->iRmsSample);

	return (gtrStatusOK);
}

STATIC gtrStatus llrfaudioFreq(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	pllrfInfo->iAudioFreq = value;

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_AUDIO_FREQ, pllrfInfo->iAudioFreq);

	return (gtrStatusOK);
}

STATIC gtrStatus llrftriggerFreq(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	pllrfInfo->iTriggerFreq = value;

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_TRIGGER_FREQ, pllrfInfo->iTriggerFreq);

	return (gtrStatusOK);
}

STATIC gtrStatus llrflongoutFunction(long parm, gtrPvt pvt, int value)
{
	long status = 0;

	switch(parm) {
		case rmsSample:
			status = llrfrmsSample(pvt,value);
			break;
		case audioFreq:
			status = llrfaudioFreq(pvt,value);
			break;
		case triggerFreq:
			status = llrftriggerFreq(pvt,value);
			break;
	}
	return (status);
}

STATIC gtrStatus llrftriggerIn(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	pllrfInfo->iTriggerIn = value;

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_TRIGGER_IN, pllrfInfo->iTriggerIn);

	return (gtrStatusOK);
}

STATIC gtrStatus llrfclockSelect(gtrPvt pvt, int value)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	pllrfInfo->iClockSelect = value;

	printf("%d\n", value);

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_CLK_SEL, pllrfInfo->iClockSelect);

	return (gtrStatusOK);
}

STATIC gtrStatus llrfmbboFunction(long parm, gtrPvt pvt, int value)
{
	long status = 0;
	switch(parm) {
		case triggerIn:
			status = llrftriggerIn(pvt,value);
			break;
		case clockSelect:
			status = llrfclockSelect(pvt, value);
			break;
	}
	return (status);
}

STATIC gtrStatus llrfregisterHandler(gtrPvt pvt,
     gtrhandler usrIH,void *handlerPvt)
{
    llrfInfo *pllrfInfo = (llrfInfo *)pvt;
   
    pllrfInfo->usrIH = usrIH;
    pllrfInfo->handlerPvt = handlerPvt;
    return(gtrStatusOK);
}

/* function name : KxRound   
// in parameter  : lfExp - 반올림 처리 할 값   
//                 nPlac - 반올림 할 자릿수   
// out parameter : 없음   
// return value  : 반올림된 값   
*/
unsigned short KxRound(const double lfExp)   
{   
	unsigned short lfVal;   

	/* 반올림을 위해 0.5 더한다.   */
	lfVal = (unsigned short)(lfExp + 0.5);   

	return lfVal;   
} 

STATIC void readContiguous(gtrchannel *pch, uint16 *pmemory, int nmax)
{
#if 1
	int i = 0;
	int k = 0;
	uint16 data;
	uint16 mask = 0xFFFF;
	uint16 *addr = (uint16*)pmemory;
	double amp, phase;
	double vI, vQ;
	double center[4] = {32767.5, 32767.5, -32767.5, -32767.5};
	int pOffset[4] = {0, 270, 180, 90};
	int degCnt = 0;
	//int degree[8] = {0, 45, 90, 135, 180, 225, 270, 315};
	int degree[] = {0,	9,	18,	27,	36,
					45,	54,	63,	72,	81,
					90,	99,	108, 117, 126,
					135, 144, 153, 162, 171,
					180, 189, 198, 207, 216,
					225, 234, 243, 252, 261,
					270, 279, 288, 297, 306,
					315, 324, 333, 342, 351};
	
	for( i = 0 ; i < nmax ; i++ ) {
		vQ = 0.0000381475547417411*(addr[i]-32767.5);
		vI = 0.0000381475547417411*(addr[i+1]-32767.5);

		amp = sqrt(pow(vI,2)+ pow(vQ,2));
		phase = atan2(vQ, vI) * 57.2957795130823;

		for( k = 0 ; k < 30 ; k++ ) {
			(pch->pdata)[pch->ndata++] = KxRound(((amp * sin((pOffset[i%4] + phase + degree[degCnt++])*0.0174532)) + 1.25) * 26214);
			if( degCnt >= 40 )
				degCnt = 0;
			if( pch->ndata >= pch->len ) break;
		}
		if( pch->ndata >= pch->len ) break;
	}
#else
	int ind = 0;
	int qnd = 0;
	uint16 data;
	uint16 mask = 0xFFFF;
	uint16 *addr = (uint16*)pmemory;
	uint16 signal[4];
	
	for( ind = 0; ind < nmax; ind+=4 ) {
		signal[0] = addr[ind] & mask;
		signal[1] = addr[ind+3] & mask;
		signal[2] = addr[ind+2] & mask;
		signal[3] = addr[ind+1] & mask;

		for( qnd = 0; qnd < 12; qnd++ ) {
			(pch->pdata)[pch->ndata++] = signal[qnd%LLRF_PERIOD];
			if( pch->ndata >= pch->len ) break;
		}
		if( pch->ndata >= pch->len ) break;
	}
#endif	
}


STATIC gtrStatus llrfreadMemory(gtrPvt pvt,gtrchannel **papgtrchannel)
{
    llrfInfo *pllrfInfo = (llrfInfo *)pvt;
    int indgroup;
	int nmax = LLRF_RAW_SIZE - 1;
	int index;
	if( RUN_GRAPH == g_runMode ){
		for( indgroup = 0 ; indgroup < LLRF_CHANNEL ; indgroup++ ) {
			papgtrchannel[indgroup];
			papgtrchannel[indgroup]->ndata = 0;

			readContiguous(papgtrchannel[indgroup], &gCirQueue[pllrfInfo->card].raw[indgroup][0], nmax);
		}
	}
    return(gtrStatusOK);
}

STATIC gtrStatus llrfreadRawMemory(gtrPvt pvt,gtrchannel **papgtrchannel)
{
    /*llrfInfo *pllrfInfo = (llrfInfo *)pvt;*/
    return(gtrStatusOK);
}


STATIC gtrStatus llrfname(gtrPvt pvt,char *pname,int maxchars)
{
    llrfInfo *pllrfInfo = (llrfInfo *)pvt;
    strncpy(pname,pllrfInfo->name,maxchars);
    pname[maxchars-1] = 0;
    return(gtrStatusOK);
}

STATIC gtrStatus llrfcurTime(gtrPvt pvt,char *pname,int maxchars)
{
	const char *pFormat =  "%Y-%m-%d %H:%M:%S";
	epicsTimeStamp curTime;
	epicsTimeGetCurrent(&curTime);
	epicsTimeToStrftime(pname, maxchars, pFormat, &curTime);

    return(gtrStatusOK);
}
STATIC gtrStatus llrfupTime(gtrPvt pvt,char *pname,int maxchars)
{
	static int init = 0;
	double d_diff = 0;
	epicsTimeStamp curTime;
	int tm_day, tm_hour, tm_min, tm_sec;
	
	if( 0 == init ) {
		init = 1;
		epicsTimeGetCurrent(&g_startTime);
	}
		
	epicsTimeGetCurrent(&curTime);
	d_diff = epicsTimeDiffInSeconds(&curTime, &g_startTime);

	tm_day	 = d_diff / ( 60 *60 * 24);
	d_diff	 = d_diff - ( tm_day *60 *60 *24);
	
	tm_hour  = d_diff / ( 60 *60);
	d_diff	 = d_diff - ( tm_hour *60 *60);
	
	tm_min	 = d_diff / 60;
	d_diff	 = d_diff - ( tm_min *60);
	
	tm_sec	 = d_diff;

	snprintf(pname, maxchars, "%02d:%02d:%02d up %4d days", tm_hour, tm_min, tm_sec, tm_day);
    return(gtrStatusOK);
}

STATIC gtrStatus llrfstringinFunction(long parm, gtrPvt pvt, char *pname, int maxchars, int initFlag)
{
	long status = 0;

	/* initFlag is 0 or 1: 1 -> init, 0 -> read */
	if(initFlag ) {
		switch(parm) {
			case name:
				status = llrfname(pvt,pname,maxchars);
				break;
		}
	} else {
		switch(parm) {
			case curTime:
				status = llrfcurTime(pvt,pname,maxchars);
				break;
			case upTime:
				status = llrfupTime(pvt,pname,maxchars);
				break;
		}
	}
	return (status);
}

STATIC gtrStatus llrfipaddr(gtrPvt pvt, const char *pvalue)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;
	char buf[128];
	
	memcpy(pllrfInfo->ipaddr, pvalue, sizeof(pllrfInfo->ipaddr));

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_IP, inet_addr(pllrfInfo->ipaddr));

	system("rm -rf llrf_config");

	/* Must input delay(5) */
	sprintf(buf, "echo \"llrfConfig(0,%s,3600,5)\" >> llrf_config", pvalue);
	system(buf);
	
	return(gtrStatusOK);
}

STATIC gtrStatus llrfsubnet(gtrPvt pvt, const char *pvalue)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	memcpy(pllrfInfo->subnet, pvalue, sizeof(pllrfInfo->subnet));

	sendCmd(pllrfInfo, CMD_ATTR, ATTR_SUBNET, inet_addr(pllrfInfo->subnet));
	
	return(gtrStatusOK);
}

STATIC gtrStatus llrfgateway(gtrPvt pvt, const char *pvalue)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;

	memcpy(pllrfInfo->gateway, pvalue, sizeof(pllrfInfo->gateway));
	sendCmd(pllrfInfo, CMD_ATTR, ATTR_GW, inet_addr(pllrfInfo->gateway));
	
	return(gtrStatusOK);
}

STATIC gtrStatus llrfiocserver(gtrPvt pvt, const char *pvalue)
{
	llrfInfo *pllrfInfo = (llrfInfo *)pvt;
	
	memcpy(pllrfInfo->iocserver, pvalue, sizeof(pllrfInfo->iocserver));
	sendCmd(pllrfInfo, CMD_ATTR, ATTR_IOC, inet_addr(pllrfInfo->iocserver));

    isRebooting = 1;
	timerDelay = 5;
	
	timerReboot = epicsTimerQueueCreateTimer(g_timerQueue, rebootHandler, 0); 

	/* Start a timer */
	epicsTimerStartDelay(timerReboot, timerDelay);	

	return(gtrStatusOK);
}

STATIC gtrStatus llrfstringoutFunction(long parm, gtrPvt pvt, const char *pvalue)
{
	long status = 0;

	switch(parm) {
		case ipaddr:
			status = llrfipaddr(pvt,pvalue);
			break;
		case subnet:
			status = llrfsubnet(pvt,pvalue);
			break;
		case gateway:
			status = llrfgateway(pvt,pvalue);
			break;
		case iocserver:
			status = llrfiocserver(pvt,pvalue);
			break;
	}
	return (status);
}

static gtrops llrfops = {
llrfinit,
llrfreport,
llrfaiFunction,
llrfaoFunction,
llrfbiFunction,
llrfboFunction,
llrflongoutFunction,
llrfmbboFunction,
llrfregisterHandler,
llrfreadMemory,
llrfreadRawMemory,
llrfstringinFunction,
llrfstringoutFunction,
0, /*setUser*/
0, /*getUser*/
0, /*lock*/
0  /*unlock*/
};

int llrfConfig(int card, const char *devIP, unsigned short nPort, int recvTimeout)
{
    char name[80] = "llrf";
    gtrops *pgtrops;
    llrfInfo *pllrfInfo;

    if(!llrfIsInited) initialize();
    if(gtrFind(card,&pgtrops)) {
        printf("card is already configured\n");
        return(0);
    }

    pllrfInfo = calloc(1,sizeof(llrfInfo));
    if(!pllrfInfo) {
        printf("llrfConfig: calloc failed\n");
        return(0);
    }

    pllrfInfo->card = card;
    pllrfInfo->name = calloc(1,strlen(name)+1);
    strcpy(pllrfInfo->name,name);

	memset((void*)g_ipAddr, 0, sizeof(g_ipAddr));
	memcpy((void*)g_ipAddr, devIP, sizeof(g_ipAddr));
	g_port = nPort;

	memset((void*)&ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(g_port);
	ServerAddr.sin_addr.s_addr = inet_addr(devIP);	

	g_iRecvTimeout = recvTimeout;
	
    ellAdd(&llrfList,&pllrfInfo->node);
    gtrRegisterDriver(card,pllrfInfo->name,&llrfops,pllrfInfo);
    return(0);
}


/*
 * IOC shell command registration
 */
#include <iocsh.h>
static const iocshArg llrfConfigArg0 = { "card",iocshArgInt};
static const iocshArg llrfConfigArg1 = { "device ip",iocshArgString};
static const iocshArg llrfConfigArg2 = { "TwinCat Port",iocshArgInt};
static const iocshArg llrfConfigArg3 = { "Receive Timeout",iocshArgInt};

static const iocshArg *llrfConfigArgs[] = {
    &llrfConfigArg0, &llrfConfigArg1, &llrfConfigArg2, &llrfConfigArg3};
static const iocshFuncDef llrfConfigFuncDef =
                      {"llrfConfig",4,llrfConfigArgs};
static void llrfConfigCallFunc(const iocshArgBuf *args)
{
    llrfConfig(args[0].ival, args[1].sval, args[2].ival, args[3].ival);
}

/*
 * This routine is called before multitasking has started, so there's
 * no race condition in the test/set of firstTime.
 */
static void
drvllrfRegisterCommands(void)
{
    static int firstTime = 1;
    if (firstTime) {
        iocshRegister(&llrfConfigFuncDef,llrfConfigCallFunc);
        firstTime = 0;
    }
}
epicsExportRegistrar(drvllrfRegisterCommands);
