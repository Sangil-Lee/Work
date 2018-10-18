//============================================================================//
//                                                                            //
//  Copyright (c) 2014 ~ by DURUTRONIX. All Rights Reserved.                  //
//                                                                            //
//============================================================================//

/*==============================================================================
                        EDIT HISTORY FOR MODULE

when                who            what, where, why
------------------- -------------  ---------------------------------------------
2018-05-07 14:53:53 laykim         Create
==============================================================================*/

//==============================================================================
//----===@ include header
//==============================================================================
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <termios.h>
#include "ts2api.h"



//==============================================================================
//----===@ global variable
//==============================================================================
static struct s_ts2sys  gSys;
static struct s_ts2sys* gpSys=NULL;









//==============================================================================
//----===@ ts2api_getInstance
// Parameters  :
// Description : 
//==============================================================================
STATUS ts2api_getInstance(struct s_ts2sys** pSys)
{
	pSys = &gpSys;
	return RET_OK;
}























//==============================================================================
//----===@ ts2api_setCpsByMode
// Parameters  :
// Description : 
//==============================================================================

//                     PE // Main Tap Current(mA) / Delayed Tap Current (mA) / Boost (dB) / Overshoot (%) / DC Swing (mV p-p)
#define PE_SETTING_0   0  // 16                   / 0                        / 0.0        / 0             / 800
#define PE_SETTING_1   1  // 16                   / 2                        / 2.0        / 25            / 800
#define PE_SETTING_2   2  // 16                   / 5                        / 4.2        / 62.5          / 800
#define PE_SETTING_3   3  // 16                   / 8                        / 6.0        / 100           / 800
#define PE_SETTING_4   4  // 11                   / 8                        / 7.8        / 145           / 550
#define PE_SETTING_5   5  // 8                    / 8                        / 9.5        / 200           / 400
#define PE_SETTING_6   6  // 4                    / 6                        / 12.0       / 300           / 300
#define PE_SETTING_7   7  // 4                    / 6                        / 12.0       / 300           / 300 


STATUS ts2api_setCpsByMode(struct s_ts2sys* pSys)
{
	ifRet(pSys == NULL)
	int i;
	unsigned char cps0_En_Val = 0x30 + PE_SETTING_0;
	unsigned char cps1_En_Val = 0x30 + PE_SETTING_0;
	unsigned char cps2_En_Val = 0x30 + PE_SETTING_0;
	unsigned char cps3_En_Val = 0x30 + PE_SETTING_0;

	pSys->ob_busId = IIC_BUS_ob;
	pSys->sl.busId = IIC_BUS_sl;
	pSys->sh.busId = IIC_BUS_sh;

	if((pSys->evMode == RAON_EVG) || (pSys->evMode == RAON_EVS))
	{
		pSys->ob_cps.outP[CPS_GT0 ] = CPS_GT0;
		pSys->ob_cps.outP[CPS_GT1 ] = CPS_GT0;
		
		pSys->ob_cps.outP[CPS_FMC1] = CPS_GT0;
		pSys->ob_cps.outP[CPS_FMC2] = CPS_GT0;
		pSys->ob_cps.outP[CPS_SL  ] = CPS_GT0;
		pSys->ob_cps.outP[CPS_SH  ] = CPS_GT0;
    prnM2_fmtDec("evMode", pSys->evMode, "G, S");
	}
	else
	{
		pSys->ob_cps.outP[CPS_GT0 ] = CPS_FMC1;
		pSys->ob_cps.outP[CPS_GT1 ] = CPS_FMC1;
		pSys->ob_cps.outP[CPS_FMC1] = CPS_FMC1;
		pSys->ob_cps.outP[CPS_FMC2] = CPS_FMC1;
		pSys->ob_cps.outP[CPS_SL  ] = CPS_FMC1;
		pSys->ob_cps.outP[CPS_SH  ] = CPS_FMC1;
    prnM2_fmtDec("evMode", pSys->evMode, "F, R, T");
	}

	pSys->ob_cps.outP_En[CPS_GT0 ] = cps0_En_Val;
	pSys->ob_cps.outP_En[CPS_GT1 ] = cps0_En_Val;
	pSys->ob_cps.outP_En[CPS_FMC1] = cps1_En_Val;
	pSys->ob_cps.outP_En[CPS_FMC2] = cps1_En_Val;
	pSys->ob_cps.outP_En[CPS_SL  ] = cps2_En_Val;
	pSys->ob_cps.outP_En[CPS_SH  ] = cps2_En_Val;

	pSys->sl.cps.outP[CPS_SW_CTRL] = CPS_SW_CTRL;
	pSys->sl.cps.outP_En[CPS_SW_CTRL] = cps2_En_Val;
	
	for( i = 0; i < 12; i++)
	{
		pSys->sl.cps.outP[CPS_SW_00 + i] = CPS_SW_CTRL;
		pSys->sl.cps.outP_En[CPS_SW_00 + i] = cps3_En_Val;
	}

	pSys->sh.cps.outP[CPS_SW_CTRL] = CPS_SW_CTRL;	
	pSys->sh.cps.outP_En[CPS_SW_CTRL] = cps2_En_Val;
	
	for( i = 0; i < 12; i++)
	{
		pSys->sh.cps.outP[CPS_SW_00 + i] = CPS_SW_CTRL;
		pSys->sh.cps.outP_En[CPS_SW_00 + i] = cps3_En_Val;
	}

  pSys->ob_cps.eqBoost0 = 0xff;
  pSys->ob_cps.eqBoost1 = 0xff;
  pSys->sl.cps.eqBoost0 = 0xff;
  pSys->sl.cps.eqBoost1 = 0xff;
  pSys->sh.cps.eqBoost0 = 0xff;
  pSys->sh.cps.eqBoost1 = 0xff;

	ts2iic_setCpsConfig(pSys->ob_busId, &pSys->ob_cps);

	if(pSys->sl.id==0x02)
		ts2iic_setCpsConfig(pSys->sl.busId, &pSys->sl.cps);

	if(pSys->sh.id==0x02)
		ts2iic_setCpsConfig(pSys->sh.busId, &pSys->sh.cps);
	
  return RET_OK;
}








//==============================================================================
//----===@ ts2api_setIoByMode
// Parameters  :
// Description : 
//==============================================================================
STATUS ts2api_setIoByMode(struct s_ts2sys* pSys)
{
	ifRet(pSys == NULL)

 	pSys->ip.ev.cfg.slio = 0xffff; // default input
 	pSys->ip.ev.cfg.shio = 0xffff;
  
	if(pSys->sl.id==0x01)
	{
		if(pSys->evMode == RAON_EVG)
		{
			ts2iic_setSlv_IO(pSys->sl.busId, TS2IO_ModeInput);
		  pSys->ip.ev.cfg.slio = 0xffff;
		}
		else
		{
			ts2iic_setSlv_IO(pSys->sl.busId, TS2IO_ModeOutput);
		  pSys->ip.ev.cfg.slio = 0x0000;
		}
	}

	if(pSys->sh.id==0x01)
	{
		ts2iic_setSlv_IO(pSys->sh.busId, TS2IO_ModeOutput);
  	pSys->ip.ev.cfg.shio = 0x0000;
	}

	ts2ip_setSlv_IO(&pSys->ip);	
	
  return RET_OK;
}



//==============================================================================
unsigned char lcdWrite(unsigned char value) //write a byte to serial port (UART/I2C/SPI)
{
    unsigned char ret =0;
	
		write( 	gpSys->lcd_fd, (const char*)&value, 1);
    //ret = (unsigned char)CSerialManager::Instance()->SendCOM((const char*)&value, 1);
    return ret;
}

//==============================================================================
void lcdWriteStr(const char *s)
{
    while (*s != 0){
        lcdWrite(*s);
        s++;
    }
}

//==============================================================================
void lcdWrite2B(unsigned int v)
{
    if (v < 255)
        lcdWrite(v);
    else {
        lcdWrite(255);
        lcdWrite(v - 255);
    }
}

//==============================================================================
void lcdDrawStr(unsigned int x, unsigned int y, const char *s) 
{
    lcdWriteStr("TP");
    lcdWrite2B(x);
    lcdWrite2B(y);
    lcdWriteStr("TT");
    lcdWriteStr(s);
    lcdWrite((unsigned char) 0);
}

//==============================================================================
void lcdClear(void) 
{
    lcdWriteStr("CL");
}

//------------------------------------------------------------------------------
//----===@ ts2 thread
//------------------------------------------------------------------------------

int g_bExit = 1;
pthread_t m_Tid;
void* threadFunc( void* pTr );


STATUS threadStart( )
{
	int rc = pthread_create (&m_Tid, NULL, threadFunc, NULL);
	ifRet( rc != 0 );
}

void threadStop()
{
    pthread_cancel (m_Tid);
}

#define LCD_LINE_1 2
#define LCD_LINE_2 1
#define LCD_LINE_3 3
#define LCD_LINE_4 4
#define LCD_LINE_5 5
#define LCD_LINE_6 6
#define LCD_LINE_7 7
#define LCD_LINE_8 8
#define LCD_LINE_9 9

void* threadFunc( void* pTr )
{
	int gt0_loss = 1;
	int gt0_tLoss_pre = -1;
	int gt1_tLoss_pre = -1;

	int sec_pre = 0;
	float slTemp_pre = 0.0;
	float shTemp_pre = 0.0;

	g_bExit = 1;

	char lcdStr[100];

	lcdClear();

	lcdDrawStr(1,LCD_LINE_1,"Timing System v1.2\n");
  lcdDrawStr(1,LCD_LINE_4,"Date :\n");
	lcdDrawStr(1,LCD_LINE_5,"Time :\n");
	
	lcdDrawStr(1,LCD_LINE_6,"sh Temp :\n");
	lcdDrawStr(1,LCD_LINE_7,"sl Temp :\n");

	if(gpSys->evMode == RAON_EVG)
		lcdDrawStr(1,LCD_LINE_2,"   [ RAON EVG ]\n");
	
	if(gpSys->evMode == RAON_EVR)
		lcdDrawStr(1,LCD_LINE_2,"   [ RAON EVR ]\n");
	
	if(gpSys->evMode == RAON_EVF)
		lcdDrawStr(1,LCD_LINE_2,"   [ RAON EVF ]\n");

	if(gpSys->evMode == RAON_EVS)
		lcdDrawStr(1,LCD_LINE_2,"   [ RAON EVS ]\n");

		ts2iic_setFLed( FLED_PWR_OK | FLED_EPICS | FLED_TIMNG_NET);
	
	unsigned int rdData;
	int rdDataSize;

  while(g_bExit)
  {
		rdDataSize = read(gpSys->ip.ev.fd, &rdData, sizeof(rdData));

		ts2ip_getGtpStat(&gpSys->ip);
		ts2ip_getEvStat(&gpSys->ip);
		ts2iic_getSlv_temp(&gpSys->sl.temp, &gpSys->sh.temp);

    if(gt0_tLoss_pre != gpSys->ip.gtp.stat.gt0_trackLossCnt)
		{
			// #define FLED_PWR_OK    0x01 
			// #define FLED_FAN_ERR   0x02 
			// #define FLED_EPICS     0x04
			// #define FLED_TIMNG_NET 0x08
			ts2iic_setFLed( FLED_PWR_OK | FLED_EPICS);
			prnM2("[ERR]gt0 trackLoss : %d\r\n", gpSys->ip.gtp.stat.gt0_trackLossCnt - gt0_tLoss_pre);
			gt0_loss = 1;
		}

    if(( 1 == gpSys->ip.gtp.stat.gt0_trackOK) && (gt0_loss == 1))
		{
		  gt0_loss = 0;
			ts2iic_setFLed( FLED_PWR_OK | FLED_EPICS | FLED_TIMNG_NET);
		}

    if(sec_pre != gpSys->ip.ev.stat.evrTime.sec)
		{
			sprintf(lcdStr, "%03d\n" , gpSys->ip.ev.stat.evrTime.day  );
			lcdDrawStr(9,LCD_LINE_4,lcdStr);

			sprintf(lcdStr, "%02d:%02d:%02d\n", gpSys->ip.ev.stat.evrTime.hour \
																				, gpSys->ip.ev.stat.evrTime.min  \
																				, gpSys->ip.ev.stat.evrTime.sec  );
			lcdDrawStr(9,LCD_LINE_5,lcdStr);

		  sec_pre = gpSys->ip.ev.stat.evrTime.sec;
		}

    if(slTemp_pre != gpSys->sl.temp)
		{
			sprintf(lcdStr, "%02.03f\n" , gpSys->sl.temp  );
			lcdDrawStr(11,LCD_LINE_6,lcdStr);
			slTemp_pre = gpSys->sl.temp;
		}

    if(shTemp_pre != gpSys->sh.temp)
		{
			sprintf(lcdStr, "%02.03f\n" , gpSys->sh.temp  );
			lcdDrawStr(11,LCD_LINE_7,lcdStr);
			shTemp_pre = gpSys->sh.temp;
		}

		gt0_tLoss_pre = gpSys->ip.gtp.stat.gt0_trackLossCnt;
		gt1_tLoss_pre = gpSys->ip.gtp.stat.gt1_trackLossCnt;
    taskDelay(100000);
  }

  pthread_exit(0);
}






static void sigio_handler( int signo )
{
	//=================================================================
	static unsigned int irqCnt = 0;
	unsigned int irqTime;
	unsigned int irqTerm;
	static unsigned int pirqTime = 0;

	struct tm tm_ptr;
	time_t m_time;

	irqCnt++;
	ts2ip_clearEvIntr(&gpSys->ip);
	ts2ip_getEvrTime(&gpSys->ip);

	// prnM2("evr time %02d:%02d:%02d.%03d\r\n", gpSys->ip.ev.stat.evrTime.hour \
	// 																			, gpSys->ip.ev.stat.evrTime.min  \
	// 																			, gpSys->ip.ev.stat.evrTime.sec  \
	// 																			, gpSys->ip.ev.stat.evrTime.ms  );


	tm_ptr.tm_year  = 2018 - 1900;
	tm_ptr.tm_mon   = 1-1;
	tm_ptr.tm_mday  = 1;
	tm_ptr.tm_hour  = 0;
	tm_ptr.tm_min   = 0;
	tm_ptr.tm_sec   = 0;
	tm_ptr.tm_isdst = 0;

	m_time = mktime(&tm_ptr) 
		+ gpSys->ip.ev.stat.evrTime.day*24*60*60 
		+ gpSys->ip.ev.stat.evrTime.hour*60*60 
		+ gpSys->ip.ev.stat.evrTime.min*60 
		+ gpSys->ip.ev.stat.evrTime.sec;

	printf("m_time : %ld\r\n",m_time);
	//if(irqCnt == 10)stime(&m_time);
	stime(&m_time);

	struct timeval val;
	struct tm *ptm;

	gettimeofday(&val, NULL);
	ptm = localtime(&val.tv_sec);

	// format : YYMMDDhhmmssuuuuuu
	printf("%04d%02d%02d%02d%02d%02d%06ld \r\n"
			, ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday
			, ptm->tm_hour, ptm->tm_min, ptm->tm_sec
			, val.tv_usec);

	ts2ip_enableEvIntr(&gpSys->ip, 1);
}




//==============================================================================
//----===@ ts2api_init
// Parameters  :
// Description : 
//==============================================================================
STATUS ts2api_init(int evMode)
{
	gpSys = &gSys;

	ifRet(gpSys == NULL);

	int i;
	int rt;

	gpSys->evMode = evMode;


//---------------------------------------------
// iic default setting
	rt = ts2iic_init();
	ifRet(rt == 0);
	
	rt = ts2iic_getSlv_ID( &gpSys->sl.id, &gpSys->sh.id);
	ifRet(rt == 0);

	rt = ts2iic_initCps(IIC_BUS_ob);
	ifRet(rt == 0);

//---------------------------------------------
// ev, gtp ip default setting
	taskDelay(1000000);
	rt = ts2ip_init(&gpSys->ip);
	ifRet(rt == 0);

 
//---------------------------------------------
// system setting with evg, evr, evf, evs
  if(gpSys->evMode == RAON_EVG)
	{
		rt = ts2iic_initClk(REF_CLK_Ext);
		gpSys->ip.ev.cfg.igen_extOn  = 1;
		gpSys->ip.ev.cfg.igen_decOff = 0;
	}
	else
	{
		rt = ts2iic_initClk(REF_CLK_ob25MHz);
		gpSys->ip.ev.cfg.igen_extOn  = 0;
		gpSys->ip.ev.cfg.igen_decOff = 1;
	}
	ifRet(rt == 0);
	taskDelay(300000);

	rt = ts2ip_resetGtp(&gpSys->ip);
	ifRet(rt == 0);
	taskDelay(1500000);

	if(gpSys->sl.id == 0x02)
	{
		rt = ts2iic_initCps(IIC_BUS_sl);
		ifRet(rt == 0);
	}

	if(gpSys->sh.id == 0x02)
	{
		rt = ts2iic_initCps(IIC_BUS_sh);
		ifRet(rt == 0);
	}

	gpSys->isInit = 1;

	rt = ts2api_setCpsByMode(gpSys);
	ifRet(rt == 0);

	rt = ts2api_setIoByMode(gpSys);
	ifRet(rt == 0);


	// #define FLED_PWR_OK    0x01 
	// #define FLED_FAN_ERR   0x02 
	// #define FLED_EPICS     0x04
	// #define FLED_TIMNG_NET 0x08
	ts2iic_setFLed( FLED_PWR_OK | FLED_EPICS | FLED_TIMNG_NET);

	

    struct termios newtio;
    gpSys->lcd_fd = open( "/dev/ttyUL1", O_RDWR | O_NOCTTY );
    ifRet(gpSys->lcd_fd<0);
		memset( &newtio, 0, sizeof(newtio) );
    newtio.c_cflag = B115200;
    newtio.c_cflag |= CS8;
    newtio.c_cflag |= CLOCAL;
    newtio.c_cflag |= CREAD;
    newtio.c_iflag = IGNPAR;
 //   newtio.c_iflag = ICRNL;
    newtio.c_oflag = 0;
    newtio.c_lflag = 0;
    newtio.c_cc[VTIME] = 0;
    newtio.c_cc[VMIN] = 0;
 
    tcflush (gpSys->lcd_fd, TCIFLUSH );
    tcsetattr(gpSys->lcd_fd, TCSANOW, &newtio );













  //----------------------------------------------------
  // txDbus src select 1 : user, mxc[6:0]
	//                   0 : external input [7:0]
	gpSys->ip.ev.cfg.txDbusSrcSelect[0] = 1; // 0 : external input, 1: user
	gpSys->ip.ev.cfg.txDbusSrcSelect[1] = 1; // 0 : external input, 1: mxc[0]
	gpSys->ip.ev.cfg.txDbusSrcSelect[2] = 1; // 0 : external input, 1: mxc[1]
	gpSys->ip.ev.cfg.txDbusSrcSelect[3] = 1; // 0 : external input, 1: mxc[2]
	gpSys->ip.ev.cfg.txDbusSrcSelect[4] = 1; // 0 : external input, 1: mxc[3]
	gpSys->ip.ev.cfg.txDbusSrcSelect[5] = 1; // 0 : external input, 1: mxc[4]
	gpSys->ip.ev.cfg.txDbusSrcSelect[6] = 1; // 0 : external input, 1: mxc[5]
	gpSys->ip.ev.cfg.txDbusSrcSelect[7] = 1; // 0 : external input, 1: mxc[6]
	ts2ip_setTxDbusIRIGB(&gpSys->ip);

  //----------------------------------------------------
  // set evg Time manually.
	gpSys->ip.ev.cfg.evgTime.day  = 128;
	gpSys->ip.ev.cfg.evgTime.hour = 23;
	gpSys->ip.ev.cfg.evgTime.min  = 53;
	gpSys->ip.ev.cfg.evgTime.sec  = 51;
  ts2ip_setEvgTime(&gpSys->ip);

	//----------------------------------------------------
	// evCodeA, mevCodeA(-> evCodeB) Trigger Source select
	// gpSys->ip.ev.cfg.evTrgSrcSel[0]  : always external input;
  gpSys->ip.ev.cfg.evTrgSrcSel[1]  = 1; // 0 : external , 1: user trigger
  gpSys->ip.ev.cfg.evTrgSrcSel[2]  = 1; // 0 : external , 1: mxc 0
  gpSys->ip.ev.cfg.evTrgSrcSel[3]  = 1; // 0 : external , 1: mxc 1
  gpSys->ip.ev.cfg.evTrgSrcSel[4]  = 1; // 0 : external , 1: mxc 2
  gpSys->ip.ev.cfg.evTrgSrcSel[5]  = 1; // 0 : external , 1: mxc 3
  gpSys->ip.ev.cfg.evTrgSrcSel[6]  = 1; // 0 : external , 1: mxc 4
  gpSys->ip.ev.cfg.evTrgSrcSel[7]  = 1; // 0 : external , 1: mxc 5
  gpSys->ip.ev.cfg.evTrgSrcSel[8]  = 1; // 0 : external , 1: mxc 6
  gpSys->ip.ev.cfg.evTrgSrcSel[9]  = 1; // 0 : external , 1: mxc 7
  gpSys->ip.ev.cfg.evTrgSrcSel[10] = 1; // 0 : external , 1: mxc 8
  gpSys->ip.ev.cfg.evTrgSrcSel[11] = 1; // 0 : external , 1: mxc 9
  gpSys->ip.ev.cfg.evTrgSrcSel[12] = 1; // 0 : external , 1: mxc 10
  gpSys->ip.ev.cfg.evTrgSrcSel[13] = 1; // 0 : external , 1: mxc 11
  gpSys->ip.ev.cfg.evTrgSrcSel[14] = 1; // 0 : external , 1: mxc 12
  gpSys->ip.ev.cfg.evTrgSrcSel[15] = 1; // 0 : external , 1: mxc 13
	ts2ip_setEvgTrgSrc   (&gpSys->ip );

	//----------------------------------------------------
	// mxc freq config 1 = 12.3ns, 0 - not allowed.
  gpSys->ip.ev.cfg.mxcFreq[0]  = 50000;
  gpSys->ip.ev.cfg.mxcFreq[1]  = 500;
  gpSys->ip.ev.cfg.mxcFreq[2]  = 500;
  gpSys->ip.ev.cfg.mxcFreq[3]  = 500;
  gpSys->ip.ev.cfg.mxcFreq[4]  = 500;
  gpSys->ip.ev.cfg.mxcFreq[5]  = 500;
  gpSys->ip.ev.cfg.mxcFreq[6]  = 500;
  gpSys->ip.ev.cfg.mxcFreq[7]  = 500;
  gpSys->ip.ev.cfg.mxcFreq[8]  = 500;
  gpSys->ip.ev.cfg.mxcFreq[9]  = 500;
  gpSys->ip.ev.cfg.mxcFreq[10] = 500;
  gpSys->ip.ev.cfg.mxcFreq[11] = 500;
  gpSys->ip.ev.cfg.mxcFreq[12] = 500;
  gpSys->ip.ev.cfg.mxcFreq[13] = 500;
	for(i=0;i<MAX_mxc_NUM;i++)
	  ts2ip_setMxcFreq(&gpSys->ip, i, gpSys->ip.ev.cfg.mxcFreq[i]);

	//----------------------------------------------------
	// evCodeA[5bit] -> evCodeB[11 bit] convert / 0 (no action) ~ 0x7ff
  gpSys->ip.ev.cfg.mevCodeA[0]  = 0;
  gpSys->ip.ev.cfg.mevCodeA[1]  = 0;
  gpSys->ip.ev.cfg.mevCodeA[2]  = 0;
  gpSys->ip.ev.cfg.mevCodeA[3]  = 0;
  gpSys->ip.ev.cfg.mevCodeA[4]  = 0;
  gpSys->ip.ev.cfg.mevCodeA[5]  = 0;
  gpSys->ip.ev.cfg.mevCodeA[6]  = 0;
  gpSys->ip.ev.cfg.mevCodeA[7]  = 0;
  gpSys->ip.ev.cfg.mevCodeA[8]  = 0;
  gpSys->ip.ev.cfg.mevCodeA[9]  = 0;
  gpSys->ip.ev.cfg.mevCodeA[10] = 0;
  gpSys->ip.ev.cfg.mevCodeA[11] = 0;
  gpSys->ip.ev.cfg.mevCodeA[12] = 0;
  gpSys->ip.ev.cfg.mevCodeA[13] = 0;
  gpSys->ip.ev.cfg.mevCodeA[14] = 0;
  gpSys->ip.ev.cfg.mevCodeA[15] = 0;
  ts2ip_setMevCodeA    (&gpSys->ip)                                      ;

	//----------------------------------------------------
	// evOption is only set to first seq packet, seq repeat count 0 ~ 65535
	// stopbit is only set to last seq packet, 0 or 1
	// evCodeB 0x0 ~ 0x7ff
	// timeStamp 0x0 ~ 0xffffffff
	int tmp_timeStamp;
	tmp_timeStamp = 100;

	// seq A
  gpSys->ip.ev.cfg.seqTableA[0].stopbit         = 0;                    
	gpSys->ip.ev.cfg.seqTableA[0].evOption        = 100; // seq repeat count 0 ~ 65535
	gpSys->ip.ev.cfg.seqTableA[0].evCodeB         = 400; 
	gpSys->ip.ev.cfg.seqTableA[0].timeStamp       = tmp_timeStamp;                    
	tmp_timeStamp += 200;
  gpSys->ip.ev.cfg.seqTableA[1].stopbit         = 0;
	gpSys->ip.ev.cfg.seqTableA[1].evOption        = 0;
	gpSys->ip.ev.cfg.seqTableA[1].evCodeB         = 401;
	gpSys->ip.ev.cfg.seqTableA[1].timeStamp       = tmp_timeStamp;
	tmp_timeStamp += 200;
  gpSys->ip.ev.cfg.seqTableA[2].stopbit         = 0;
	gpSys->ip.ev.cfg.seqTableA[2].evOption        = 0;
	gpSys->ip.ev.cfg.seqTableA[2].evCodeB         = 402;
	gpSys->ip.ev.cfg.seqTableA[2].timeStamp       = tmp_timeStamp;
	tmp_timeStamp += 200;
  gpSys->ip.ev.cfg.seqTableA[3].stopbit         = 1;
	gpSys->ip.ev.cfg.seqTableA[3].evOption        = 0;
	gpSys->ip.ev.cfg.seqTableA[3].evCodeB         = 403;
	gpSys->ip.ev.cfg.seqTableA[3].timeStamp       = tmp_timeStamp;
	tmp_timeStamp += 200;

	// seq B
	tmp_timeStamp = 100;
  gpSys->ip.ev.cfg.seqTableB[0].stopbit         = 0;                    
	gpSys->ip.ev.cfg.seqTableB[0].evOption        = 1; // seq repeat count 0 ~ 65535
	gpSys->ip.ev.cfg.seqTableB[0].evCodeB         = 500; 
	gpSys->ip.ev.cfg.seqTableB[0].timeStamp       = tmp_timeStamp;                    
	tmp_timeStamp += 200;
  gpSys->ip.ev.cfg.seqTableB[1].stopbit         = 0;
	gpSys->ip.ev.cfg.seqTableB[1].evOption        = 0;
	gpSys->ip.ev.cfg.seqTableB[1].evCodeB         = 501;
	gpSys->ip.ev.cfg.seqTableB[1].timeStamp       = tmp_timeStamp;
	tmp_timeStamp += 200;
  gpSys->ip.ev.cfg.seqTableB[2].stopbit         = 0;
	gpSys->ip.ev.cfg.seqTableB[2].evOption        = 0;
	gpSys->ip.ev.cfg.seqTableB[2].evCodeB         = 502;
	gpSys->ip.ev.cfg.seqTableB[2].timeStamp       = tmp_timeStamp;
	tmp_timeStamp += 200;
  gpSys->ip.ev.cfg.seqTableB[3].stopbit         = 1;
	gpSys->ip.ev.cfg.seqTableB[3].evOption        = 0;
	gpSys->ip.ev.cfg.seqTableB[3].evCodeB         = 503;
	gpSys->ip.ev.cfg.seqTableB[3].timeStamp       = tmp_timeStamp;
	tmp_timeStamp += 200;

  ts2ip_setSeqTable    (&gpSys->ip)                                      ;




	//----------------------------------------------------
	// seq A, B Trigger Source Select 
	// seqTrgSrc[15: 0] = exIn[15:0]
	// seqTrgSrc[29:16] = mxc[13:0]
	// seqTrgSrc[30]    = user Trigger
																		// | D_seqSrc_exIn0  \
																		// | D_seqSrc_exIn1  \
																		// | D_seqSrc_exIn2  \
																		// | D_seqSrc_exIn3  \
																		// | D_seqSrc_exIn4  \
																		// | D_seqSrc_exIn5  \
																		// | D_seqSrc_exIn6  \
																		// | D_seqSrc_exIn7  \
																		// | D_seqSrc_exIn8  \
																		// | D_seqSrc_exIn9  \
																		// | D_seqSrc_exIn10 \
																		// | D_seqSrc_exIn11 \
																		// | D_seqSrc_exIn12 \
																		// | D_seqSrc_exIn13 \
																		// | D_seqSrc_exIn14 \
																		// | D_seqSrc_exIn15 \
																		// | D_seqSrc_mxc0   \
																		// | D_seqSrc_mxc1   \
																		// | D_seqSrc_mxc2   \
																		// | D_seqSrc_mxc3   \
																		// | D_seqSrc_mxc4   \
																		// | D_seqSrc_mxc5   \
																		// | D_seqSrc_mxc6   \
																		// | D_seqSrc_mxc7   \
																		// | D_seqSrc_mxc8   \
																		// | D_seqSrc_mxc9   \
																		// | D_seqSrc_mxc10  \
																		// | D_seqSrc_mxc11  \
																		// | D_seqSrc_mxc12  \
																		// | D_seqSrc_mxc13  \
																		// | D_seqSrc_usr    \

  gpSys->ip.ev.cfg.seqMask.A_OR0  = 0 \
																		| D_seqSrc_usr    \
																		;        

  gpSys->ip.ev.cfg.seqMask.A_OR1  = 0 \
																		;        

	// gpSys->ip.ev.cfg.seqMask.A_sel, 	0 : none(seq disable), 
	//																	1 : A_OR0
	//																	2 : A_OR1
	//																	3 : A_OR0 and A_OR1
	gpSys->ip.ev.cfg.seqMask.A_sel  = 1; 

  gpSys->ip.ev.cfg.seqMask.B_OR0  = 0 \
																		;        

  gpSys->ip.ev.cfg.seqMask.B_OR1  = 0 \
																		;        

	// gpSys->ip.ev.cfg.seqMask.B_sel, 	0 : none(seq disable), 
	//																	1 : B_OR0
	//																	2 : B_OR1
	//																	3 : B_OR0 and B_OR1
	//																	4 : none(seq disable), 
	//																	5 : A_OR0
	//																	6 : A_OR1
	//																	7 : A_OR0 and A_OR1
	gpSys->ip.ev.cfg.seqMask.B_sel  = 0; 
  ts2ip_setSeqTrgSrc   (&gpSys->ip)                                      ;

  
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	//----------------------------------------------------
	// evr mapping ram config
  gpSys->ip.ev.cfg.evMappingRam[ 10] = 0xffffffff;  
  gpSys->ip.ev.cfg.evMappingRam[400] = 0x00010000;  
  gpSys->ip.ev.cfg.evMappingRam[401] = 0x00020000;  
  gpSys->ip.ev.cfg.evMappingRam[402] = 0x00040000;  
  gpSys->ip.ev.cfg.evMappingRam[403] = 0x00080000;  
  gpSys->ip.ev.cfg.evMappingRam[500] = 0x00100000;  
  gpSys->ip.ev.cfg.evMappingRam[501] = 0x00200000;  
  gpSys->ip.ev.cfg.evMappingRam[502] = 0x00400000;  
  gpSys->ip.ev.cfg.evMappingRam[503] = 0x00800000;  

  if(gpSys->evMode == RAON_EVT)
	{
			gpSys->ip.ev.cfg.evMappingRam[700] = 0x00000001;  
			gpSys->ip.ev.cfg.evMappingRam[701] = 0x00000002;  
			gpSys->ip.ev.cfg.evMappingRam[702] = 0x00000004;  
			gpSys->ip.ev.cfg.evMappingRam[703] = 0x00000008;  
			gpSys->ip.ev.cfg.evMappingRam[704] = 0x00000010;  
			gpSys->ip.ev.cfg.evMappingRam[705] = 0x00000020;  
			gpSys->ip.ev.cfg.evMappingRam[706] = 0x00000040;  
			gpSys->ip.ev.cfg.evMappingRam[707] = 0x00000080;  
			gpSys->ip.ev.cfg.evMappingRam[708] = 0x00000100;  
			gpSys->ip.ev.cfg.evMappingRam[709] = 0x00000200;  
			gpSys->ip.ev.cfg.evMappingRam[710] = 0x00000400;  
			gpSys->ip.ev.cfg.evMappingRam[711] = 0x00000800;  
			gpSys->ip.ev.cfg.evMappingRam[712] = 0x00001000;  
			gpSys->ip.ev.cfg.evMappingRam[713] = 0x00002000;  
			gpSys->ip.ev.cfg.evMappingRam[714] = 0x00004000;  
			gpSys->ip.ev.cfg.evMappingRam[715] = 0x00008000;  
	}
  ts2ip_setEvMappingRam(&gpSys->ip)                                      ;

	//----------------------------------------------------
  // gpSys->ip.ev.cfg.swapExOut
	// for test
	gpSys->ip.ev.cfg.swapExOut       = 0;
  
	// pdpTrgSrcSel 1 : pdpTrigger[31:16] from evCodeA
	//                  pdpTrigger[31:16] from EvMappingRam[15:0]
	//              0 : pdpTrigger[31: 0] from EvMappingRam[31:0]
	gpSys->ip.ev.cfg.pdpTrgSrcSel    = 0;      

	// rxDbusSrcSelect 1 : exOut[7:0] from pdpOutput[7:0]
	//                 0 : exOut[7:0] from rxDBus[7:0]
	gpSys->ip.ev.cfg.rxDbusSrcSelect[0] = 0; // exOut[0], 0 : rxDBus[0] , 1: pdpOut[0] 
	gpSys->ip.ev.cfg.rxDbusSrcSelect[1] = 0; // exOut[1], 0 : rxDBus[1] , 1: pdpOut[1] 
	gpSys->ip.ev.cfg.rxDbusSrcSelect[2] = 0; // exOut[2], 0 : rxDBus[2] , 1: pdpOut[2] 
	gpSys->ip.ev.cfg.rxDbusSrcSelect[3] = 0; // exOut[3], 0 : rxDBus[3] , 1: pdpOut[3] 
	gpSys->ip.ev.cfg.rxDbusSrcSelect[4] = 0; // exOut[4], 0 : rxDBus[4] , 1: pdpOut[4] 
	gpSys->ip.ev.cfg.rxDbusSrcSelect[5] = 0; // exOut[5], 0 : rxDBus[5] , 1: pdpOut[5] 
	gpSys->ip.ev.cfg.rxDbusSrcSelect[6] = 0; // exOut[6], 0 : rxDBus[6] , 1: pdpOut[6] 
	gpSys->ip.ev.cfg.rxDbusSrcSelect[7] = 0; // exOut[7], 0 : rxDBus[7] , 1: pdpOut[7] 
  ts2ip_setRxDbus      (&gpSys->ip)                                      ;

  //----------------------------------------------------
  // set pdp out polarity
  gpSys->ip.ev.cfg.pdpPolarity[0]  = 1;
  gpSys->ip.ev.cfg.pdpPolarity[1]  = 1;
  gpSys->ip.ev.cfg.pdpPolarity[2]  = 1;
  gpSys->ip.ev.cfg.pdpPolarity[3]  = 1;
  gpSys->ip.ev.cfg.pdpPolarity[4]  = 1;
  gpSys->ip.ev.cfg.pdpPolarity[5]  = 1;
  gpSys->ip.ev.cfg.pdpPolarity[6]  = 1;
  gpSys->ip.ev.cfg.pdpPolarity[7]  = 1;
  gpSys->ip.ev.cfg.pdpPolarity[8]  = 1;
  gpSys->ip.ev.cfg.pdpPolarity[9]  = 1;
  gpSys->ip.ev.cfg.pdpPolarity[10] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[11] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[12] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[13] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[14] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[15] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[16] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[17] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[18] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[19] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[20] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[21] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[22] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[23] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[24] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[25] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[26] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[27] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[28] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[29] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[30] = 1;
  gpSys->ip.ev.cfg.pdpPolarity[31] = 1;
  ts2ip_setPdpPolarity (&gpSys->ip);

  //----------------------------------------------------
  // set pdp out delay
	gpSys->ip.ev.cfg.pdp[0].delay        = 0; // max 0xffffffff
	gpSys->ip.ev.cfg.pdp[1].delay        = 0;
	gpSys->ip.ev.cfg.pdp[2].delay        = 0;
	gpSys->ip.ev.cfg.pdp[3].delay        = 0;
	gpSys->ip.ev.cfg.pdp[4].delay        = 0;
	gpSys->ip.ev.cfg.pdp[5].delay        = 0;
	gpSys->ip.ev.cfg.pdp[6].delay        = 0;
	gpSys->ip.ev.cfg.pdp[7].delay        = 0;
	gpSys->ip.ev.cfg.pdp[8].delay        = 0;
	gpSys->ip.ev.cfg.pdp[9].delay        = 0;
	gpSys->ip.ev.cfg.pdp[10].delay       = 0;
	gpSys->ip.ev.cfg.pdp[11].delay       = 0;
	gpSys->ip.ev.cfg.pdp[12].delay       = 0;
	gpSys->ip.ev.cfg.pdp[13].delay       = 0;
	gpSys->ip.ev.cfg.pdp[14].delay       = 0;
	gpSys->ip.ev.cfg.pdp[15].delay       = 0;
	gpSys->ip.ev.cfg.pdp[16].delay       = 0;
	gpSys->ip.ev.cfg.pdp[17].delay       = 0;
	gpSys->ip.ev.cfg.pdp[18].delay       = 0;
	gpSys->ip.ev.cfg.pdp[19].delay       = 0;
	gpSys->ip.ev.cfg.pdp[20].delay       = 0;
	gpSys->ip.ev.cfg.pdp[21].delay       = 0;
	gpSys->ip.ev.cfg.pdp[22].delay       = 0;
	gpSys->ip.ev.cfg.pdp[23].delay       = 0;
	gpSys->ip.ev.cfg.pdp[24].delay       = 0;
	gpSys->ip.ev.cfg.pdp[25].delay       = 0;
	gpSys->ip.ev.cfg.pdp[26].delay       = 0;
	gpSys->ip.ev.cfg.pdp[27].delay       = 0;
	gpSys->ip.ev.cfg.pdp[28].delay       = 0;
	gpSys->ip.ev.cfg.pdp[29].delay       = 0;
	gpSys->ip.ev.cfg.pdp[30].delay       = 0;
	gpSys->ip.ev.cfg.pdp[31].delay       = 0;
	
  // set pdp out width
	gpSys->ip.ev.cfg.pdp[0].width        = 20; // 0: 1 pulse [12.3ns], max 0xffffffff
	gpSys->ip.ev.cfg.pdp[1].width        = 20;
	gpSys->ip.ev.cfg.pdp[2].width        = 20;
	gpSys->ip.ev.cfg.pdp[3].width        = 20;
	gpSys->ip.ev.cfg.pdp[4].width        = 20;
	gpSys->ip.ev.cfg.pdp[5].width        = 20;
	gpSys->ip.ev.cfg.pdp[6].width        = 20;
	gpSys->ip.ev.cfg.pdp[7].width        = 20;
	gpSys->ip.ev.cfg.pdp[8].width        = 20;
	gpSys->ip.ev.cfg.pdp[9].width        = 20;
	gpSys->ip.ev.cfg.pdp[10].width       = 20;
	gpSys->ip.ev.cfg.pdp[11].width       = 20;
	gpSys->ip.ev.cfg.pdp[12].width       = 20;
	gpSys->ip.ev.cfg.pdp[13].width       = 20;
	gpSys->ip.ev.cfg.pdp[14].width       = 20;
	gpSys->ip.ev.cfg.pdp[15].width       = 20;
	gpSys->ip.ev.cfg.pdp[16].width       = 20;
	gpSys->ip.ev.cfg.pdp[17].width       = 20;
	gpSys->ip.ev.cfg.pdp[18].width       = 20;
	gpSys->ip.ev.cfg.pdp[19].width       = 20;
	gpSys->ip.ev.cfg.pdp[20].width       = 20;
	gpSys->ip.ev.cfg.pdp[21].width       = 20;
	gpSys->ip.ev.cfg.pdp[22].width       = 20;
	gpSys->ip.ev.cfg.pdp[23].width       = 20;
	gpSys->ip.ev.cfg.pdp[24].width       = 20;
	gpSys->ip.ev.cfg.pdp[25].width       = 20;
	gpSys->ip.ev.cfg.pdp[26].width       = 20;
	gpSys->ip.ev.cfg.pdp[27].width       = 20;
	gpSys->ip.ev.cfg.pdp[28].width       = 20;
	gpSys->ip.ev.cfg.pdp[29].width       = 20;
	gpSys->ip.ev.cfg.pdp[30].width       = 20;
	gpSys->ip.ev.cfg.pdp[31].width       = 20;
  ts2ip_setPdpShape    (&gpSys->ip);


	// ts2ip_enableEvIntr(&gpSys->ip, 	D_intr_evCodeA1    \
	// 																D_intr_evrAlarmSec \
	// 																);

	ts2ip_setEvIntrMask(&gpSys->ip,	D_intr_evrAlarmSec \
																	);

  //----------------------------------------------------
  // enable evr control register
	gpSys->ip.ev.enEvr.streamData        = 1;       
  gpSys->ip.ev.enEvr.DBus              = 1; 
  gpSys->ip.ev.enEvr.evCodeA           = 1;    
  gpSys->ip.ev.enEvr.evCodeB           = 1;    
  gpSys->ip.ev.enEvr.pdpEnable         = 1;      
	ts2ip_setEvrEnable(&gpSys->ip);

  taskDelay(3000000);


  //----------------------------------------------------
  // enable evg control register
	gpSys->ip.ev.enEvg.mxc               = 1;   
	gpSys->ip.ev.enEvg.evTrgMsk16        = 0;   // evTrigger Enable Mask 16bit (evCodeA mask)
	gpSys->ip.ev.enEvg.DBus              = 1;  
	gpSys->ip.ev.enEvg.evCodeA           = 0;     
	gpSys->ip.ev.enEvg.evCodeB           = 1;     
	gpSys->ip.ev.enEvg.evCodeB_mevCodeA  = 0;              
	gpSys->ip.ev.enEvg.evCodeB_seqTrgA   = 1;             
	gpSys->ip.ev.enEvg.evCodeB_seqTrgB   = 0;             
	ts2ip_setEvgEnable(&gpSys->ip);
 
  struct sigaction sigact, oldact;
	int oflag;
	sigact.sa_handler = sigio_handler;
	sigemptyset( &sigact.sa_mask );
	sigact.sa_flags = SA_INTERRUPT;

	if( sigaction( SIGIO, &sigact,&oldact ) < 0 )
	{
		perror( "sigaction error : " );
		exit(0);
	}

	fcntl( gpSys->ip.ev.fd, F_SETOWN,getpid() );
	oflag = fcntl( gpSys->ip.ev.fd, F_GETFL );
	fcntl( gpSys->ip.ev.fd, F_SETFL, oflag | FASYNC );
	prnM2( "sigaction( SIGIO, &sigact,&oldact )\n");

	threadStart();

		usleep(3000);

	ts2ip_enableEvIntr(&gpSys->ip, 1);





  return RET_OK;
}


//==============================================================================
//----===@ ts2api_fin
// Parameters  :
// Description : 
//==============================================================================
STATUS ts2api_fin( void )
{
  g_bExit = 1;
  taskDelay(2000000);

#if 0
	s_evgEnable.mxc = 0;
	evg_enableCtrl(g_pSys, &s_evgEnable);
	prnM2("ts2api_fin\n\r");

  taskDelay(2000000);

  ts2api_finSystem();
#endif	
  return RET_OK;
}































//===========================================================================================================================

//==============================================================================
//----===@ ts2api_prnCpsStat
// Parameters  :
// Description :
//==============================================================================
static STATUS ts2api_prnCpsStat(int busId, struct s_cpsConfig* pCfg)
{
	ifRet(pCfg == NULL);
	ifRet(busId > IIC_BUS_MAX);
	
  prnM2("------------------------------------------------------\r\n");
	int i;
	switch (busId) 
	{
    case IIC_BUS_ob : prnM2("prnCpsStat - %s\r\n", "onboard cps"); break;
    case IIC_BUS_sl : prnM2("prnCpsStat - %s\r\n", "sl cps"); break;
    case IIC_BUS_sh : prnM2("prnCpsStat - %s\r\n", "sh cps"); break;
    default: 
			prnErrRet(); 
		break;
  }
  
	for(i=0;i<16;i++)
	{
		prnM2("%-2s %02d                 : [ %02d ] - 0x%02x\r\n"   ,\
							" "                ,\
			        i  ,\
			        pCfg->outP[i]  ,\
			        pCfg->outP_En[i]);
  }
	
	prnM2("\r\n");
  return RET_OK;
}




//==============================================================================
//----===@ ts2api_prnStat
// Parameters  :
// Description :
//==============================================================================
static STATUS ts2api_prnStat(struct s_ts2sys* pSys)
{
  prnEvStat (&pSys->ip);

#if 1
  prnGtpStat(&pSys->ip);

	ts2iic_getSlv_temp(&pSys->sl.temp, &pSys->sh.temp);

  prnM2("======================================================\r\n");
	prnM2("ts2 system Stat\r\n");
  prnM2("======================================================\r\n");

	prnM2_fmtDec("sl_ID"         , pSys->sl.id      , "1:IO_TTL, 2:FanOut");
	prnM2_fmtDec("sh_ID"         , pSys->sh.id      , "1:IO_TTL, 2:FanOut");
	prnM2("%-22s: %f\r\n", "SL temperature", pSys->sl.temp );
	prnM2("%-22s: %f\r\n", "SH temperature", pSys->sh.temp );

	ts2iic_getCpsStat(pSys->ob_busId, &pSys->ob_cpsStat);
	ts2api_prnCpsStat(pSys->ob_busId, &pSys->ob_cpsStat);

	if(pSys->sl.id == 0x02) 
	{
		ts2iic_getCpsStat(pSys->sl.busId, &pSys->sl.cpsStat);
		ts2api_prnCpsStat(pSys->sl.busId, &pSys->sl.cpsStat);
	}

	if(pSys->sh.id == 0x02) 
	{
		ts2iic_getCpsStat(pSys->sh.busId, &pSys->sh.cpsStat);
		ts2api_prnCpsStat(pSys->sh.busId, &pSys->sh.cpsStat);
	}

#endif
  return RET_OK;
}










//==============================================================================
//----===@ ts2api_cpsTest
// Parameters  :
// Description :
//==============================================================================
STATUS ts2api_cpsTest( int argc, char **argv )
{
	prnM2("Usage: cpsT gt0 fmc1 loop back / gt1 sl sfp 0 loopback.\r\n");

	int ret;
	int i;
	struct s_ts2sys* pSys = gpSys;
  
	unsigned char cps0_En_Val = (unsigned char) 0x30;
	unsigned char cps1_En_Val = (unsigned char) 0x30;
	unsigned char cps2_En_Val = (unsigned char) 0x30;
	unsigned char cps3_En_Val = (unsigned char) 0x30;

	pSys->ob_busId = IIC_BUS_ob;
	pSys->sl.busId = IIC_BUS_sl;
	pSys->sh.busId = IIC_BUS_sh;

	pSys->ob_cps.outP[CPS_GT0 ] = CPS_SL;
	pSys->ob_cps.outP[CPS_GT1 ] = CPS_SH;
	pSys->ob_cps.outP[CPS_FMC1] = CPS_GT0;
	pSys->ob_cps.outP[CPS_FMC2] = CPS_GT0;
	pSys->ob_cps.outP[CPS_SL  ] = CPS_FMC1;
	pSys->ob_cps.outP[CPS_SH  ] = CPS_FMC1;
	prnM2_fmtDec("evMode", pSys->evMode, "");

	pSys->ob_cps.outP_En[CPS_GT0 ] = cps0_En_Val;
	pSys->ob_cps.outP_En[CPS_GT1 ] = cps0_En_Val;
	pSys->ob_cps.outP_En[CPS_FMC1] = cps1_En_Val;
	pSys->ob_cps.outP_En[CPS_FMC2] = cps1_En_Val;
	pSys->ob_cps.outP_En[CPS_SL  ] = cps2_En_Val;
	pSys->ob_cps.outP_En[CPS_SH  ] = cps2_En_Val;




	pSys->sl.cps.outP[CPS_SW_CTRL] = CPS_SW_11;
	pSys->sh.cps.outP[CPS_SW_CTRL] = CPS_SW_11;

	pSys->sl.cps.outP_En[CPS_SW_CTRL] = cps2_En_Val;
	pSys->sh.cps.outP_En[CPS_SW_CTRL] = cps2_En_Val;
	
	pSys->sl.cps.outP[CPS_SW_00] = CPS_SW_CTRL;
	pSys->sl.cps.outP[CPS_SW_01] = CPS_SW_00;
	pSys->sl.cps.outP[CPS_SW_02] = CPS_SW_01;
	pSys->sl.cps.outP[CPS_SW_03] = CPS_SW_02;
	pSys->sl.cps.outP[CPS_SW_04] = CPS_SW_03;
	pSys->sl.cps.outP[CPS_SW_05] = CPS_SW_04;
	pSys->sl.cps.outP[CPS_SW_06] = CPS_SW_05;
	pSys->sl.cps.outP[CPS_SW_07] = CPS_SW_06;
	pSys->sl.cps.outP[CPS_SW_08] = CPS_SW_07;
	pSys->sl.cps.outP[CPS_SW_09] = CPS_SW_08;
	pSys->sl.cps.outP[CPS_SW_10] = CPS_SW_09;
	pSys->sl.cps.outP[CPS_SW_11] = CPS_SW_10;

	pSys->sh.cps.outP[CPS_SW_00] = CPS_SW_CTRL;
	pSys->sh.cps.outP[CPS_SW_01] = CPS_SW_00;
	pSys->sh.cps.outP[CPS_SW_02] = CPS_SW_01;
	pSys->sh.cps.outP[CPS_SW_03] = CPS_SW_02;
	pSys->sh.cps.outP[CPS_SW_04] = CPS_SW_03;
	pSys->sh.cps.outP[CPS_SW_05] = CPS_SW_04;
	pSys->sh.cps.outP[CPS_SW_06] = CPS_SW_05;
	pSys->sh.cps.outP[CPS_SW_07] = CPS_SW_06;
	pSys->sh.cps.outP[CPS_SW_08] = CPS_SW_07;
	pSys->sh.cps.outP[CPS_SW_09] = CPS_SW_08;
	pSys->sh.cps.outP[CPS_SW_10] = CPS_SW_09;
	pSys->sh.cps.outP[CPS_SW_11] = CPS_SW_10;

	for( i = 0; i < 12; i++)
	{
		pSys->sl.cps.outP_En[CPS_SW_00 + i] = cps3_En_Val;
		pSys->sh.cps.outP_En[CPS_SW_00 + i] = cps3_En_Val;
	}

  pSys->ob_cps.eqBoost0 = 0xff;
  pSys->ob_cps.eqBoost1 = 0xff;
  pSys->sl.cps.eqBoost0 = 0xff;
  pSys->sl.cps.eqBoost1 = 0xff;
  pSys->sh.cps.eqBoost0 = 0xff;
  pSys->sh.cps.eqBoost1 = 0xff;

	ts2iic_setCpsConfig(pSys->ob_busId, &pSys->ob_cps);

	if(pSys->sl.id==0x02)
		ts2iic_setCpsConfig(pSys->sl.busId, &pSys->sl.cps);

	if(pSys->sh.id==0x02)
		ts2iic_setCpsConfig(pSys->sh.busId, &pSys->sh.cps);
	
  return RET_OK;
}



//==============================================================================
//----===@ ts2api_cpsCtrl
// Parameters  :
// Description :
//==============================================================================
STATUS ts2api_cpsCtrl( int argc, char **argv )
{
	if(argc < 5)
	{
		prnM2("Usage: cps x1, x2, x3, x4 [0~7]\n");
		return RET_ERR;
	}
	
	int ret;
	int i;
	struct s_ts2sys* pSys = gpSys;

	int PE0 = atoi(argv[1]);
	int PE1 = atoi(argv[2]);
	int PE2 = atoi(argv[3]);
	int PE3 = atoi(argv[4]);
  
	unsigned char cps0_En_Val = (unsigned char) 0x30 + PE0;
	unsigned char cps1_En_Val = (unsigned char) 0x30 + PE1;
	unsigned char cps2_En_Val = (unsigned char) 0x30 + PE2;
	unsigned char cps3_En_Val = (unsigned char) 0x30 + PE3;

	pSys->ob_busId = IIC_BUS_ob;
	pSys->sl.busId = IIC_BUS_sl;
	pSys->sh.busId = IIC_BUS_sh;

	if((pSys->evMode == RAON_EVG) || (pSys->evMode == RAON_EVS))
	{
		pSys->ob_cps.outP[CPS_GT0 ] = CPS_GT0;
		pSys->ob_cps.outP[CPS_GT1 ] = CPS_GT0;
		pSys->ob_cps.outP[CPS_FMC1] = CPS_GT0;
		pSys->ob_cps.outP[CPS_FMC2] = CPS_GT0;
		pSys->ob_cps.outP[CPS_SL  ] = CPS_GT0;
		pSys->ob_cps.outP[CPS_SH  ] = CPS_GT0;
    prnM2_fmtDec("evMode", pSys->evMode, "G, S");
	}
	else
	{
		pSys->ob_cps.outP[CPS_GT0 ] = CPS_FMC1;
		pSys->ob_cps.outP[CPS_GT1 ] = CPS_FMC1;
		pSys->ob_cps.outP[CPS_FMC1] = CPS_FMC1;
		pSys->ob_cps.outP[CPS_FMC2] = CPS_FMC1;
		pSys->ob_cps.outP[CPS_SL  ] = CPS_FMC1;
		pSys->ob_cps.outP[CPS_SH  ] = CPS_FMC1;
    prnM2_fmtDec("evMode", pSys->evMode, "F, R, T");
	}

	pSys->ob_cps.outP_En[CPS_GT0 ] = cps0_En_Val;
	pSys->ob_cps.outP_En[CPS_GT1 ] = cps0_En_Val;
	pSys->ob_cps.outP_En[CPS_FMC1] = cps1_En_Val;
	pSys->ob_cps.outP_En[CPS_FMC2] = cps1_En_Val;
	pSys->ob_cps.outP_En[CPS_SL  ] = cps2_En_Val;
	pSys->ob_cps.outP_En[CPS_SH  ] = cps2_En_Val;

	pSys->sl.cps.outP[CPS_SW_CTRL] = CPS_SW_00;
	pSys->sl.cps.outP_En[CPS_SW_CTRL] = cps2_En_Val;
	
	for( i = 0; i < 12; i++)
	{
		pSys->sl.cps.outP[CPS_SW_00 + i] = CPS_SW_CTRL;
		pSys->sl.cps.outP_En[CPS_SW_00 + i] = cps3_En_Val;
	}

	pSys->sh.cps.outP[CPS_SW_CTRL] = CPS_SW_00;
	pSys->sh.cps.outP_En[CPS_SW_CTRL] = cps2_En_Val;
	
	for( i = 0; i < 12; i++)
	{
		pSys->sh.cps.outP[CPS_SW_00 + i] = CPS_SW_CTRL;
		pSys->sh.cps.outP_En[CPS_SW_00 + i] = cps3_En_Val;
	}

  pSys->ob_cps.eqBoost0 = 0xff;
  pSys->ob_cps.eqBoost1 = 0xff;
  pSys->sl.cps.eqBoost0 = 0xff;
  pSys->sl.cps.eqBoost1 = 0xff;
  pSys->sh.cps.eqBoost0 = 0xff;
  pSys->sh.cps.eqBoost1 = 0xff;

	ts2iic_setCpsConfig(pSys->ob_busId, &pSys->ob_cps);

	if(pSys->sl.id==0x02)
		ts2iic_setCpsConfig(pSys->sl.busId, &pSys->sl.cps);

	if(pSys->sh.id==0x02)
		ts2iic_setCpsConfig(pSys->sh.busId, &pSys->sh.cps);
	
  return RET_OK;
}





//==============================================================================
//----===@ ts2api_usrTrg
// Parameters  :
// Description :
//==============================================================================
STATUS ts2api_usrTrg( void )
{
	ifRet(gpSys<0);
  int ret;
	
	ret = ts2ip_genUsrTrg (&gpSys->ip);

	return ret;
}


//==============================================================================
//----===@ ts2api_seqResume
// Parameters  :
// Description :
//==============================================================================
STATUS ts2api_seqResume( void )
{
	ifRet(gpSys<0);
  int ret;
	
	ret = ts2ip_seqResume (&gpSys->ip, 0);

	return ret;
}



//==============================================================================
//----===@ ts2api_usrTrg
// Parameters  :
// Description :
//==============================================================================
STATUS ts2api_usrEvCode( int argc, char **argv )
{
	if(argc < 2)
	{
		prnM2("Usage: uev xxxx [1~2047]\n");
		return RET_ERR;
	}
	
	int ret;
	int usrEvCodeB = atoi(argv[1]);
  
	ret = ts2ip_genUsrEvCodeB(&gpSys->ip, usrEvCodeB);
	if(ret == RET_ERR)
		prnErr();

	return RET_OK;
}


//==============================================================================
//----===@ ts2api_setLed
// Parameters  :
// Description : front lcd panel LED control function [if master]
//==============================================================================
STATUS ts2api_setLed(int argc, char **argv)
{
	if(argc < 2)
	{
		prnM2("Usage: fled [0~15]\n");
		return RET_ERR;
	}

	int ret;
	int val = atoi(argv[1]);

  ts2iic_setFLed(val);
	
	return RET_OK;
}


//==============================================================================
//----===@ ts2api_shellCmd
// Parameters  :
// Description :
// cmd 1 print status
// cmd 2,ddd,hh,mm,ss set evg time
//==============================================================================
STATUS ts2api_shellCmd_prnUsage( void );

STATUS ts2api_shellCmd ( int argc, char **argv )
{
	int i;

	if(argc < 2)
	{
		ts2api_shellCmd_prnUsage();
		return RET_ERR;
	}

	int cmdNum = atoi(argv[1]);

	switch (cmdNum) 
	{
    case 0 : 
//		prnM2("%-30s: %s\r\n", "cmd 0"                    ,"prnStatus");
			ts2api_prnStat (gpSys);
			break;

    case 1 : 	
//		prnM2("%-30s: %s\r\n", "cmd 1,ddd,hh,mm,ss"       ,"ts2ip_setEvgTime. Must run with EVS[4] mode.");
			if(argc < 6)
			{
  			ts2api_shellCmd_prnUsage();
				return RET_ERR;
			}

			gpSys->ip.ev.cfg.evgTime.day  = atoi(argv[2]);
			gpSys->ip.ev.cfg.evgTime.hour = atoi(argv[3]);
			gpSys->ip.ev.cfg.evgTime.min  = atoi(argv[4]);
			gpSys->ip.ev.cfg.evgTime.sec  = atoi(argv[5]);
			ts2ip_setEvgTime(&gpSys->ip);

			break;

    case 2 : 	
//		prnM2("%-30s: %s\r\n", "cmd 2"                    ,"evCodeA Test 0 - ext in 16 to evCodeA");
			//----------------------------------------------------
			// evCodeA, mevCodeA(-> evCodeB) Trigger Source select
			// gpSys->ip.ev.cfg.evTrgSrcSel[0]  : always external input;
      for(i=0;i<16;i++)
				gpSys->ip.ev.cfg.evTrgSrcSel[i]  = 0;

			ts2ip_setEvgTrgSrc   (&gpSys->ip );

			//----------------------------------------------------
			// pdpTrgSrcSel 1 : pdpTrigger[31:16] from evCodeA
			//                  pdpTrigger[31:16] from EvMappingRam[15:0]
			//              0 : pdpTrigger[31: 0] from EvMappingRam[31:0]
			gpSys->ip.ev.cfg.pdpTrgSrcSel    = 1;       // <-------------------------------------------

			// rxDbusSrcSelect 1 : exOut[7:0] from pdpOutput[7:0]
			//                 0 : exOut[7:0] from rxDBus[7:0]
			// gpSys->ip.ev.cfg.rxDbusSrcSelect[0] = 1; // exOut[0], 0 : rxDBus[0] , 1: pdpOut[0] 
			ts2ip_setRxDbus      (&gpSys->ip)                                      ;

		  //----------------------------------------------------
			// enable evg control register
			gpSys->ip.ev.enEvg.mxc               = 0;   
			gpSys->ip.ev.enEvg.evTrgMsk16        = 0xffff;   // evTrigger Enable Mask 16bit (evCodeA mask)
			gpSys->ip.ev.enEvg.DBus              = 0;  
			gpSys->ip.ev.enEvg.evCodeA           = 1;     
			gpSys->ip.ev.enEvg.evCodeB           = 1;     
			gpSys->ip.ev.enEvg.evCodeB_mevCodeA  = 0;              
			gpSys->ip.ev.enEvg.evCodeB_seqTrgA   = 0;             
			gpSys->ip.ev.enEvg.evCodeB_seqTrgB   = 0;             
			ts2ip_setEvgEnable(&gpSys->ip);

			//----------------------------------------------------
			// enable evr control register
			gpSys->ip.ev.enEvr.streamData        = 1;       
			gpSys->ip.ev.enEvr.DBus              = 1; 
			gpSys->ip.ev.enEvr.evCodeA           = 1;    
			gpSys->ip.ev.enEvr.evCodeB           = 1;    
			gpSys->ip.ev.enEvr.pdpEnable         = 1;      
			ts2ip_setEvrEnable(&gpSys->ip);

			break;

    case 3 : 	
		// prnM2("%-30s: %s\r\n", "cmd 3, trgNum, mxcFreq"                    ,"evCodeA Test 1 - user trigger, -> 'uTrg'" );
			if(argc < 4)
			{
  			ts2api_shellCmd_prnUsage();
				return RET_ERR;
			}

			int trgNum  = atoi(argv[2]);
			int mxcFreq = atoi(argv[3]);

			ifRet((trgNum < 1) || (trgNum > 15));
			ifRet(mxcFreq < 1);

			//----------------------------------------------------
			// mxc freq config 1 = 12.3ns, 0 - not allowed.
			for(i=0;i<MAX_mxc_NUM;i++)
				ts2ip_setMxcFreq(&gpSys->ip, i, 1000);

			if(trgNum > 1);
				ts2ip_setMxcFreq(&gpSys->ip, trgNum - 2, mxcFreq);

			//----------------------------------------------------
			// set pdp out polarity
      for(i=0;i<32;i++)
				gpSys->ip.ev.cfg.pdpPolarity[i]  = 1;
			
			ts2ip_setPdpPolarity (&gpSys->ip);

			//----------------------------------------------------
			// set pdp out delay
      for(i=0;i<32;i++)
				gpSys->ip.ev.cfg.pdp[i].delay        = 0; // max 0xffffffff

			// set pdp out width
      for(i=0;i<32;i++)
				gpSys->ip.ev.cfg.pdp[i].width        = 10; // 0: 1 pulse [12.3ns], max 0xffffffff

			gpSys->ip.ev.cfg.pdp[trgNum].width = 100; // 0: 1 pulse [12.3ns], max 0xffffffff	

			ts2ip_setPdpShape    (&gpSys->ip);

			//----------------------------------------------------
			// evCodeA, mevCodeA(-> evCodeB) Trigger Source select
			// gpSys->ip.ev.cfg.evTrgSrcSel[0]  : always external input;

      for(i=0;i<16;i++)
				gpSys->ip.ev.cfg.evTrgSrcSel[i]  = 0;

			gpSys->ip.ev.cfg.evTrgSrcSel[trgNum]  = 1;

			ts2ip_setEvgTrgSrc   (&gpSys->ip );

			//----------------------------------------------------
			// pdpTrgSrcSel 1 : pdpTrigger[31:16] from evCodeA
			//                  pdpTrigger[31:16] from EvMappingRam[15:0]
			//              0 : pdpTrigger[31: 0] from EvMappingRam[31:0]
			gpSys->ip.ev.cfg.pdpTrgSrcSel    = 1;       // <-------------------------------------------

			// rxDbusSrcSelect 1 : exOut[7:0] from pdpOutput[7:0]
			//                 0 : exOut[7:0] from rxDBus[7:0]
			// gpSys->ip.ev.cfg.rxDbusSrcSelect[0] = 1; // exOut[0], 0 : rxDBus[0] , 1: pdpOut[0] 
			ts2ip_setRxDbus      (&gpSys->ip)                                      ;
			

		  //----------------------------------------------------
			// enable evg control register
			gpSys->ip.ev.enEvg.mxc               = 1;   
			gpSys->ip.ev.enEvg.evTrgMsk16        = 0xffff;   // evTrigger Enable Mask 16bit (evCodeA mask)
			gpSys->ip.ev.enEvg.DBus              = 0;  
			gpSys->ip.ev.enEvg.evCodeA           = 1;     
			gpSys->ip.ev.enEvg.evCodeB           = 1;     
			gpSys->ip.ev.enEvg.evCodeB_mevCodeA  = 0;              
			gpSys->ip.ev.enEvg.evCodeB_seqTrgA   = 0;             
			gpSys->ip.ev.enEvg.evCodeB_seqTrgB   = 0;             
			ts2ip_setEvgEnable(&gpSys->ip);

			//----------------------------------------------------
			// enable evr control register
			gpSys->ip.ev.enEvr.streamData        = 1;       
			gpSys->ip.ev.enEvr.DBus              = 1; 
			gpSys->ip.ev.enEvr.evCodeA           = 1;    
			gpSys->ip.ev.enEvr.evCodeB           = 1;    
			gpSys->ip.ev.enEvr.pdpEnable         = 1;      
			ts2ip_setEvrEnable(&gpSys->ip);

			break;
			
    case 4 : 	
//		prnM2("%-30s: %s\r\n", "cmd 4"                    ,"evCodeB Test - from mevCodeA ");

			//----------------------------------------------------
			// set pdp out polarity
      for(i=0;i<32;i++)
				gpSys->ip.ev.cfg.pdpPolarity[i]  = 1;
			
			ts2ip_setPdpPolarity (&gpSys->ip);

			//----------------------------------------------------
			// set pdp out delay
      for(i=0;i<32;i++)
				gpSys->ip.ev.cfg.pdp[i].delay        = 0; // max 0xffffffff

			// set pdp out width
      for(i=0;i<32;i++)
				gpSys->ip.ev.cfg.pdp[i].width        = 50; // 0: 1 pulse [12.3ns], max 0xffffffff

			ts2ip_setPdpShape    (&gpSys->ip);

			//----------------------------------------------------
			// evCodeA, mevCodeA(-> evCodeB) Trigger Source select
			// gpSys->ip.ev.cfg.evTrgSrcSel[0]  : always external input;
      for(i=0;i<16;i++)
				gpSys->ip.ev.cfg.evTrgSrcSel[i]  = 0;

			gpSys->ip.ev.cfg.evTrgSrcSel[1]  = 1; //USER TRIGGER

			ts2ip_setEvgTrgSrc   (&gpSys->ip );

			//----------------------------------------------------
			// pdpTrgSrcSel 1 : pdpTrigger[31:16] from evCodeA
			//                  pdpTrigger[31:16] from EvMappingRam[15:0]
			//              0 : pdpTrigger[31: 0] from EvMappingRam[31:0]
			gpSys->ip.ev.cfg.pdpTrgSrcSel    = 0;       // <-------------------------------------------

			// rxDbusSrcSelect 1 : exOut[7:0] from pdpOutput[7:0]
			//                 0 : exOut[7:0] from rxDBus[7:0]
			// gpSys->ip.ev.cfg.rxDbusSrcSelect[0] = 1; // exOut[0], 0 : rxDBus[0] , 1: pdpOut[0] 
			ts2ip_setRxDbus      (&gpSys->ip)                                      ;




			
			//----------------------------------------------------
			// seq A, B Trigger Source Select 
			// seqTrgSrc[15: 0] = exIn[15:0]
			// seqTrgSrc[29:16] = mxc[13:0]
			// seqTrgSrc[30]    = user Trigger
																				// | D_seqSrc_exIn0  \
																				// | D_seqSrc_exIn1  \
																				// | D_seqSrc_exIn2  \
																				// | D_seqSrc_exIn3  \
																				// | D_seqSrc_exIn4  \
																				// | D_seqSrc_exIn5  \
																				// | D_seqSrc_exIn6  \
																				// | D_seqSrc_exIn7  \
																				// | D_seqSrc_exIn8  \
																				// | D_seqSrc_exIn9  \
																				// | D_seqSrc_exIn10 \
																				// | D_seqSrc_exIn11 \
																				// | D_seqSrc_exIn12 \
																				// | D_seqSrc_exIn13 \
																				// | D_seqSrc_exIn14 \
																				// | D_seqSrc_exIn15 \
																				// | D_seqSrc_mxc0   \
																				// | D_seqSrc_mxc1   \
																				// | D_seqSrc_mxc2   \
																				// | D_seqSrc_mxc3   \
																				// | D_seqSrc_mxc4   \
																				// | D_seqSrc_mxc5   \
																				// | D_seqSrc_mxc6   \
																				// | D_seqSrc_mxc7   \
																				// | D_seqSrc_mxc8   \
																				// | D_seqSrc_mxc9   \
																				// | D_seqSrc_mxc10  \
																				// | D_seqSrc_mxc11  \
																				// | D_seqSrc_mxc12  \
																				// | D_seqSrc_mxc13  \
																				// | D_seqSrc_usr    \

			gpSys->ip.ev.cfg.seqMask.A_OR0  = 0 \
																				| D_seqSrc_usr    \
																				;        

			gpSys->ip.ev.cfg.seqMask.A_OR1  = 0 \
																				;        

			gpSys->ip.ev.cfg.seqMask.B_OR0  = 0 \
																				;        

			gpSys->ip.ev.cfg.seqMask.B_OR1  = 0 \
																				;        

			// gpSys->ip.ev.cfg.seqMask.A_sel, 	0 : none(seq disable), 
			//																	1 : A_OR0
			//																	2 : A_OR1
			//																	3 : A_OR0 and A_OR1
			// gpSys->ip.ev.cfg.seqMask.B_sel, 	0 : none(seq disable), 
			//																	1 : B_OR0
			//																	2 : B_OR1
			//																	3 : B_OR0 and B_OR1
			//																	4 : none(seq disable), 
			//																	5 : A_OR0
			//																	6 : A_OR1
			//																	7 : A_OR0 and A_OR1
			gpSys->ip.ev.cfg.seqMask.A_sel  = 1; 
			gpSys->ip.ev.cfg.seqMask.B_sel  = 0; 
			ts2ip_setSeqTrgSrc   (&gpSys->ip)                                      ;


		  //----------------------------------------------------
			// enable evg control register
			gpSys->ip.ev.enEvg.mxc               = 0;   
			gpSys->ip.ev.enEvg.evTrgMsk16        = 0XFFFF;   // evTrigger Enable Mask 16bit (evCodeA mask)
			gpSys->ip.ev.enEvg.DBus              = 0;  
			gpSys->ip.ev.enEvg.evCodeA           = 0;     
			gpSys->ip.ev.enEvg.evCodeB           = 1;     
			gpSys->ip.ev.enEvg.evCodeB_mevCodeA  = 0;              
			gpSys->ip.ev.enEvg.evCodeB_seqTrgA   = 1;             
			gpSys->ip.ev.enEvg.evCodeB_seqTrgB   = 0;             
			ts2ip_setEvgEnable(&gpSys->ip);

			//----------------------------------------------------
			// enable evr control register
			gpSys->ip.ev.enEvr.streamData        = 1;       
			gpSys->ip.ev.enEvr.DBus              = 1; 
			gpSys->ip.ev.enEvr.evCodeA           = 1;    
			gpSys->ip.ev.enEvr.evCodeB           = 1;    
			gpSys->ip.ev.enEvr.pdpEnable         = 1;      
			ts2ip_setEvrEnable(&gpSys->ip);

			break;


    case 9 : 	
			prnM2("Usage: cmd {1 [prnStatus] | 2 [] | 3 [] | 4 []}\n");
			break;
    default: 
			ts2api_shellCmd_prnUsage();
			break;
  }

	return RET_OK;
}


STATUS ts2api_shellCmd_prnUsage( void )
{
		prnM2("[ Usage ]\r\n");
		prnM2("%-30s: %s\r\n", "cmd 0"                    ,"prnStatus");
		
		prnM2("%-30s: %s\r\n", "cmd 1,ddd,hh,mm,ss"       ,"ts2ip_setEvgTime. Must run with EVS[4] mode.");
		
		prnM2("%-30s: %s\r\n", "cmd 2"                    ,"evCodeA Test 0 - ext in 16 to evCodeA");
		prnM2("%-30s: %s\r\n", "cmd 3,trgNum,mxcFreq"     ,"evCodeA Test 1 - user trigger, -> 'uTrg'" );

		prnM2("%-30s: %s\r\n", "cmd 4"                    ,"evCodeB Test - from mevCodeA ");
		prnM2("%-30s: %s\r\n", "cmd 5"                    ,"evCodeB Test - from seqA ");
		prnM2("%-30s: %s\r\n", "cmd 6"                    ,"evCodeB Test - from seqB ");

	return RET_OK;
}
