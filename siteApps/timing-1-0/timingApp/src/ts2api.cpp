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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <signal.h>


#include "ts2api.h"

//==============================================================================
//----===@ global variable
//==============================================================================
static struct s_ts2sys  gSys;
//static struct s_ts2sys* gpSys=NULL;
struct s_ts2sys* gpSys=NULL;

//==============================================================================
//----===@ ts2api_getInstance
// Parameters  :
// Description : 
//==============================================================================
//STATUS ts2api_getInstance(struct s_ts2sys** pSys)
STATUS ts2api_getInstance(struct s_ts2sys* pSys)
{
	pSys = &gSys;
	return RET_OK;
}

//==============================================================================
//----===@ ts2api_setCpsByMode
// Parameters  :
// Description : 
//==============================================================================
STATUS ts2api_setCpsByMode(struct s_ts2sys* pSys)
{
	ifRet(pSys == NULL)
	int i;

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
    prnM2_fmtDec("evMode", pSys->evMode, "F,R");
	}

	pSys->ob_cps.outP_En[CPS_GT0 ] = 0x30;
	pSys->ob_cps.outP_En[CPS_GT1 ] = 0x30;
	pSys->ob_cps.outP_En[CPS_FMC1] = 0x30;
	pSys->ob_cps.outP_En[CPS_FMC2] = 0x30;
	pSys->ob_cps.outP_En[CPS_SL  ] = 0x30;
	pSys->ob_cps.outP_En[CPS_SH  ] = 0x30;

	pSys->sl.cps.outP[CPS_SW_CTRL] = CPS_SW_00;
	pSys->sl.cps.outP_En[CPS_SW_CTRL] = 0x30;
	
	for( i = 0; i < 12; i++)
	{
		pSys->sl.cps.outP[CPS_SW_00 + i] = CPS_SW_CTRL;
		pSys->sl.cps.outP_En[CPS_SW_00 + i] = 0x30;
	}

	pSys->sh.cps.outP[CPS_SW_CTRL] = CPS_SW_00;
	pSys->sh.cps.outP_En[CPS_SW_CTRL] = 0x30;
	
	for( i = 0; i < 12; i++)
	{
		pSys->sh.cps.outP[CPS_SW_00 + i] = CPS_SW_CTRL;
		pSys->sh.cps.outP_En[CPS_SW_00 + i] = 0x30;
	}

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
	

  //----------------------------------------------------
  // txDbus src select 1 : user, mxc[6:0]
	//                   0 : external input [7:0]
	gpSys->ip.ev.cfg.txDbusSrcSelect = 0xffffffff; // 0 : external input, 1: user
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
	gpSys->ip.ev.cfg.seqTableB[0].evOption        = 500; // seq repeat count 0 ~ 65535
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
  // gpSys->ip.ev.cfg.seqTableA[2048].stopbit         = ;                    
	// gpSys->ip.ev.cfg.seqTableA[2048].evOption        = ;                    
	// gpSys->ip.ev.cfg.seqTableA[2048].evCodeB         = ;                    
	// gpSys->ip.ev.cfg.seqTableA[2048].timeStamp       = ;                    
	// gpSys->ip.ev.cfg.seqTableB[2048].stopbit         = ;                    
	// gpSys->ip.ev.cfg.seqTableB[2048].evOption        = ;                    
	// gpSys->ip.ev.cfg.seqTableB[2048].evCodeB         = ;                    
	// gpSys->ip.ev.cfg.seqTableB[2048].timeStamp       = ;                    
  // ts2ip_setSeqTable    (&gpSys->ip)                                      ;


/*	
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
*/

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

  ts2ip_setEvMappingRam(&gpSys->ip);

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
	gpSys->ip.ev.cfg.rxDbusSrcSelect = 0; // exOut[0], 0 : rxDBus[0] , 1: pdpOut[0] 
  ts2ip_setRxDbus      (&gpSys->ip)                                      ;

  //----------------------------------------------------
  // set pdp out polarity
  gpSys->ip.ev.cfg.pdpPolarity  = 0xffffffff;
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

  taskDelay(1000000);

  return RET_OK;
}


//==============================================================================
//----===@ ts2api_fin
// Parameters  :
// Description : 
//==============================================================================
STATUS ts2api_fin( void )
{
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


  return RET_OK;
}

//==============================================================================
//----===@ ts2api_shellCmd
// Parameters  :
// Description : 
//==============================================================================
STATUS ts2api_shellCmd ( int argc, char **argv )
{
	if(argc < 2)
	{
		prnM2("Usage: cmd {1 [prnStatus] | 2 [] | 3 [] | 4 []}\n");
		return RET_ERR;
	}

	int cmdNum = atoi(argv[1]);
  prnM2("cmdline args: %s %d\r\n", argv[0], cmdNum);

	switch (cmdNum) 
	{
    case 1 : 
			ts2api_prnStat (gpSys);
			break;
    case 2 : 	
			ts2ip_setEvgTime(&gpSys->ip);
			break;
    case 3 : 	
			prnM2("Usage: cmd {1 [prnStatus] | 2 [] | 3 [] | 4 []}\n");
			break;
    default: 
			prnM2("Usage: cmd {1 [prnStatus] | 2 [] | 3 [] | 4 []}\n");
			break;
  }

	return RET_OK;
}

