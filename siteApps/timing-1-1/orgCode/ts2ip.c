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
#include <sys/time.h>

#include "ts2regmap.h"
#include "ts2ip.h"

//==============================================================================
//----===@ global variable
//==============================================================================



//==============================================================================
//----===@ ts2ip_rd
// Parameters  :
// Description : 
//==============================================================================
static STATUS ts2ip_rd(int fd, unsigned int offset, unsigned int *rdData)
{
  if(fd == -1)
		prnErrRet();

  struct s_regRW reg;
	reg.offset = offset;
	reg.val    = 0;

	if (ioctl(fd, IOCTL_R, &reg) < 0) 
		prnErrRet();

  *rdData = reg.val;
  return RET_OK;
}

//==============================================================================
//----===@ ts2ip_wr
// Parameters  :
// Description : 
//==============================================================================
static STATUS ts2ip_wr(int fd, unsigned int offset, unsigned int wrData)
{
  if (fd == -1) 
		prnErrRet();

  struct s_regRW reg;
	reg.offset = offset;
	reg.val    = wrData;

	if (ioctl(fd, IOCTL_W, &reg) < 0) 
		prnErrRet();

  return RET_OK;
}



//==============================================================================
//----===@ ts2ip_setSlv_IO
// Parameters  :
// Description : 
//==============================================================================
STATUS ts2ip_setSlv_IO(struct s_ts2ip* pIP)
{
  ifRet (pIP->ev.fd < 0) 
	
  ts2ip_wr(pIP->ev.fd, A_m_st_sl, C_m_st(pIP->ev.cfg.slio));
  ts2ip_wr(pIP->ev.fd, A_m_st_sh, C_m_st(pIP->ev.cfg.shio));

  return RET_OK;
}

//==============================================================================
//----===@ ts2ip_init
// Parameters  :
// Description : raon devs open
//==============================================================================
STATUS ts2ip_init(struct s_ts2ip* pIP)
{
	int i;
	prnM2("\n");
	prnM2("======================================================\r\n");
	prnM2("ts2ip_init\r\n");
	prnM2("======================================================\r\n");
	
	for(i=0;i<5;i++)
	{
		pIP->ev.fd  = open(DRV_NAME_ip_ev,O_RDWR);
		if(pIP->ev.fd == -1)
			taskDelay(200000);
		else
			break;
	}

	for(i=0;i<5;i++)
	{
		pIP->gtp.fd = open(DRV_NAME_ip_gtp,O_RDWR);
		if(pIP->gtp.fd == -1)
			taskDelay(200000);
		else
			break;
	}

	if (pIP->ev.fd  == -1)prnErrRet();
	if (pIP->gtp.fd == -1)prnErrRet();

	//output reg  [31 : 0] m_st, // 3-state enable input, high=input, low=output
  ts2ip_wr(pIP->ev.fd, A_m_st_sl, C_m_st(0xffff));
  ts2ip_wr(pIP->ev.fd, A_m_st_sh, C_m_st(0xffff));

	prnM2("ts2ip_init ok..\n");
	pIP->isInit = 1;
	return RET_OK;
}


//==============================================================================
//----===@ ts2ip_getInfo
// Parameters  :
// Description :
//==============================================================================
static STATUS ts2ip_getInfo(int l_fd, struct s_ipInfo *pInfo)
{
	ifRet(l_fd < 0);
  
  int rdData;

	ts2ip_rd(l_fd, A_axi_aclk_cntr, &rdData);	
	pInfo->ipTick = rdData;

	ts2ip_rd(l_fd, A_vendor, &rdData);	
	pInfo->vendor = rdData;

	ts2ip_rd(l_fd, A_ipInfo, &rdData);
  pInfo->ipID        = S_ipID(rdData);
  pInfo->ipID_sub    = S_ipID_sub(rdData);   

  pInfo->ipVer_major = S_ipVer_major(rdData); 
  pInfo->ipVer_miner = S_ipVer_miner(rdData); 
  pInfo->ipVer_rev   = S_ipVer_rev(rdData); 

	ts2ip_rd(l_fd, A_buildTime, &rdData);
	pInfo->buildTime.year  = S_buildYear(rdData)  ;
	pInfo->buildTime.month = S_buildMonth(rdData) ;
	pInfo->buildTime.day   = S_buildDay(rdData)   ;
	pInfo->buildTime.hour  = S_buildHour(rdData)  ;
	pInfo->buildTime.min   = S_buildMin(rdData)   ;
	pInfo->buildTime.sec   = S_buildSec(rdData)   ;

  return RET_OK;
}


//==============================================================================
//----===@ ts2ip_getGtpStat
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_getGtpStat(struct s_ts2ip* pIP)
{
  int rdData;
	int l_fd = pIP->gtp.fd;
	struct s_gtpStat* pStat = &pIP->gtp.stat;

	ifRet(pIP->gtp.fd < 0);

  memset(pStat,0,sizeof(struct s_gtpStat));

	ts2ip_getInfo(l_fd, &pIP->gtp.info);

	ts2ip_rd(l_fd, A_gt_status, &rdData);	
	//prnM2_fmtHex("A_gt_status", rdData      , "rdData");

  pStat->gtp_soft_reset       = S_gtp_soft_reset(rdData)      ;
  pStat->gt0_cpllfbclklost    = S_gt0_cpllfbclklost(rdData)   ;
  pStat->gt0_cplllock         = S_gt0_cplllock(rdData)        ;
  pStat->gt0_txresetdone      = S_gt0_txresetdone(rdData)     ;
  pStat->gt0_txfsmresetdone   = S_gt0_txfsmresetdone(rdData)  ;
  pStat->gt0_rxresetdone      = S_gt0_rxresetdone(rdData)     ;
  pStat->gt0_rxfsmresetdone   = S_gt0_rxfsmresetdone(rdData)  ;
  pStat->gt0_rxDataError      = S_gt0_rxDataError(rdData)     ;
  pStat->gt0_track_data       = S_gt0_track_data(rdData)      ;
  pStat->gt0_trackOK          = S_gt0_trackOK(rdData)      ;
  pStat->gt1_cpllfbclklost    = S_gt1_cpllfbclklost(rdData)   ;
  pStat->gt1_cplllock         = S_gt1_cplllock(rdData)        ;
  pStat->gt1_txresetdone      = S_gt1_txresetdone(rdData)     ;
  pStat->gt1_txfsmresetdone   = S_gt1_txfsmresetdone(rdData)  ;
  pStat->gt1_rxresetdone      = S_gt1_rxresetdone(rdData)     ;
  pStat->gt1_rxfsmresetdone   = S_gt1_rxfsmresetdone(rdData)  ;
  pStat->gt1_rxDataError      = S_gt1_rxDataError(rdData)     ;
  pStat->gt1_track_data       = S_gt1_track_data(rdData)      ;
  pStat->gt1_trackOK          = S_gt1_trackOK(rdData)      ;

	ts2ip_rd(l_fd, A_gt_trackLossCnt, &rdData);	
  pStat->gt0_trackLossCnt     = S_gt0_trackLossCnt(rdData)    ;
  pStat->gt1_trackLossCnt     = S_gt1_trackLossCnt(rdData)    ;

	ts2ip_rd(l_fd, A_gt0_clk_cntr, &rdData);	
  pStat->gt0_txClk_cntr       = S_gt0_rxClk_cntr(rdData)      ;
  pStat->gt0_rxClk_cntr       = S_gt0_txClk_cntr(rdData)      ;

	ts2ip_rd(l_fd, A_gt1_clk_cntr, &rdData);	
  pStat->gt1_txClk_cntr       = S_gt1_rxClk_cntr(rdData)      ;
  pStat->gt1_rxClk_cntr       = S_gt1_txClk_cntr(rdData)      ;

  return RET_OK;
}



//==============================================================================
//----===@ ts2ip_getEvrTime
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_getEvrTime(struct s_ts2ip* pIP)
{
  int rdData;
	int l_fd = pIP->ev.fd;
	struct s_evStat* pStat = &pIP->ev.stat;
	ifRet(l_fd < 0);

	ts2ip_rd(l_fd, A_evrW_Time, &rdData);
  prnM0_fmtHex("rdData        ", rdData      , "A_evrW_Time");
  pStat->evrTime.hour = S_evrW_TimeHour(rdData);  
  pStat->evrTime.min  = S_evrW_TimeMin(rdData);  
  pStat->evrTime.sec  = S_evrW_TimeSec(rdData);  
  pStat->evrTime.ms   = S_evrW_TimeMs(rdData);  

	ts2ip_rd(l_fd, A_evrW_Day, &rdData);
  prnM0_fmtHex("rdData        ", rdData      , "A_evrW_Day");
  pStat->evrTime.day  = S_evrW_Day(rdData);  

  return RET_OK;
}


//==============================================================================
//----===@ ts2ip_getEvStat
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_getEvStat(struct s_ts2ip* pIP)
{
	ifRet(pIP->ev.fd < 0);

	int i;
  int rdData;
	int l_fd = pIP->ev.fd;
	struct s_evStat* pStat = &pIP->ev.stat;

	ts2ip_getInfo(l_fd, &pIP->ev.info);
	//ts2ip_getEvrTime(pIP);

  ts2ip_rd(l_fd, A_boardStat, &rdData);
	pStat->isMaster     = S_im_mF_in(rdData);
	pStat->PRSNT_M2C_L  = S_f_PRSNT_M2C_L(rdData);
  pStat->f_SFP_LossA  = S_f_SFP_Loss0(rdData);
  pStat->f_SFP_LossB  = S_f_SFP_Loss1(rdData);
  pStat->f_SFP_prsntA = S_f_SFP_presentN0(rdData);
  pStat->f_SFP_prsntB = S_f_SFP_presentN1(rdData);

  ts2ip_rd(l_fd, A_ifanStt4_CntrFront, &rdData);
	pStat->fanA = S_ifanStt4_CntrFrontA(rdData);
	pStat->fanB = S_ifanStt4_CntrFrontB(rdData);

  ts2ip_rd(l_fd, A_ifanStt4_CntrRear, &rdData);
	pStat->fanC = S_ifanStt4_CntrRearA(rdData);
	pStat->fanD = S_ifanStt4_CntrRearB(rdData);

  ts2ip_rd(l_fd, A_emergencyStopCntr, &rdData);
	pStat->emergencyStopCntr = rdData;

  ts2ip_rd(l_fd, A_seqTrg_runState, &rdData);
	pStat->seqA_isRunning = S_seqTrgA_runState(rdData);
	pStat->seqB_isRunning = S_seqTrgB_runState(rdData);

	ts2ip_rd(l_fd, A_pdpRunStt, &rdData);
	for(i=0;i<32;i++)
    pStat->pdp[i].isRunning = (rdData >> i) & 0x00000001;

	ts2ip_wr(l_fd, A_evr_RegUpdate,C_evr_getStatReg(1));
	ts2ip_wr(l_fd, A_evr_RegUpdate,C_evr_getStatReg(0));
	taskDelay(10);

	for(i=0;i<32;i++)
	{
		ts2ip_rd(l_fd, MemC_EVR_pdpStat + i*4, &rdData);
		pStat->pdp[i].runCnt = S_pdpRunCntr(rdData);
		pStat->pdp[i].errCnt = S_pdpErrCntr(rdData);
	}

  return RET_OK;
}


//==============================================================================
//----===@ ts2ip_resetGtp
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_resetGtp(struct s_ts2ip* pIP)
{
  int l_fd = pIP->gtp.fd;
	ifRet(pIP->gtp.fd < 0);

  ts2ip_wr(l_fd, A_gt_reset, C_soft_reset(1));
  taskDelay(1000);
  ts2ip_wr(l_fd, A_gt_reset, C_soft_reset(0));

  return RET_OK;
}




//==============================================================================
//----===@ ts2ip_setEvgTime
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_setEvgTime(struct s_ts2ip* pIP )
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

  struct s_evTime* pTime = &pIP->ev.cfg.evgTime;

  ts2ip_wr(l_fd, A_evgW_set_Time    , C_evgW_set_h8(pTime->hour) | C_evgW_set_m8(pTime->min) | C_evgW_set_s8(pTime->sec) );
  ts2ip_wr(l_fd, A_evgW_set_Time + 4, C_evgW_set_d16(pTime->day));

  ts2ip_wr(l_fd, A_resetCtrl, C_evgW_set_time(1));
	taskDelay(10);
  ts2ip_wr(l_fd, A_resetCtrl, C_evgW_set_time(0));

  return RET_OK;
}

//==============================================================================
//----===@ ts2ip_setEvgEnable
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_setEvgEnable(struct s_ts2ip* pIP)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

	struct s_evgEnable* pEn = &pIP->ev.enEvg; 
  ts2ip_wr(l_fd, A_evgEnableMask, 0 \
														| C_mxc14Enable      (pEn->mxc             ) \
														| C_evTrgMsk         (pEn->evTrgMsk16      ) \
														| C_txEnable_DBus    (pEn->DBus            ) \
														| C_txEnable_evCodeA (pEn->evCodeA         ) \
														| C_txEnable_evCodeB (pEn->evCodeB         ) \
														| C_txEnable_mevCodeA(pEn->evCodeB_mevCodeA) \
														| C_seqTrgA_en       (pEn->evCodeB_seqTrgA ) \
														| C_seqTrgB_en       (pEn->evCodeB_seqTrgB ) \
														);
	return RET_OK;
}



//==============================================================================
//----===@ ts2ip_setEvrEnable
// Parameters  : 
// Description : 
//==============================================================================
STATUS ts2ip_setEvrEnable(struct s_ts2ip* pIP)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

	struct s_evrEnable* pEn = &pIP->ev.enEvr; 
	ts2ip_wr(l_fd, A_evrEnableMask, 0 \
														| C_rxEnable_streamData(pEn->streamData) \
														| C_rxEnable_DBus      (pEn->DBus      ) \
														| C_rxEnable_evCodeA   (pEn->evCodeA   ) \
														| C_rxEnable_evCodeB   (pEn->evCodeB   ) \
														| C_pdpEnable          (pEn->pdpEnable ) \
														); 
  return RET_OK;
}



//==============================================================================
//----===@ ts2ip_seqResume
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_seqResume(struct s_ts2ip* pIP, unsigned int seqN)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

  unsigned int writeVal;

	if(seqN == TS2IP_seqN_A)
		writeVal = C_seqTrgA_resume(1);

	if(seqN == TS2IP_seqN_B)
		writeVal = C_seqTrgB_resume(1);


	ts2ip_wr(l_fd, A_resetCtrl, writeVal);
	taskDelay(1);
	ts2ip_wr(l_fd, A_resetCtrl, 0);

	prnM2("ts2ip_seqResume.\r\n");
	return RET_OK;
}

//==============================================================================
//----===@ ts2ip_setTxDbusIRIGB
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_setTxDbusIRIGB(struct s_ts2ip* pIP)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

	struct s_evConfig* pCfg = &pIP->ev.cfg;

  ts2ip_wr(l_fd, A_txDBusSel_igenMode , 0\
	                                    | C_txDBusSel0(pCfg->txDbusSrcSelect[0]) \
	                                    | C_txDBusSel1(pCfg->txDbusSrcSelect[1]) \
	                                    | C_txDBusSel2(pCfg->txDbusSrcSelect[2]) \
	                                    | C_txDBusSel3(pCfg->txDbusSrcSelect[3]) \
	                                    | C_txDBusSel4(pCfg->txDbusSrcSelect[4]) \
	                                    | C_txDBusSel5(pCfg->txDbusSrcSelect[5]) \
	                                    | C_txDBusSel6(pCfg->txDbusSrcSelect[6]) \
	                                    | C_txDBusSel7(pCfg->txDbusSrcSelect[7]) \
																			| C_igen_decOff(pCfg->igen_decOff)   \
																			| C_igen_extOn(pCfg->igen_extOn)     );
  return RET_OK;
}


//==============================================================================
//----===@ ts2ip_genUsrTrg
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_genUsrTrg(struct s_ts2ip* pIP)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

  ts2ip_wr(l_fd, A_usrTrg , C_usrTrg(1));
  ts2ip_wr(l_fd, A_usrTrg , C_usrTrg(0));

  return RET_OK;
}

//==============================================================================
//----===@ ts2ip_genUsrEvCodeB
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_genUsrEvCodeB(struct s_ts2ip* pIP, unsigned int usrEvCodeB)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);
	ifRet((usrEvCodeB & 0xfffff800) != 0)

  ts2ip_wr(l_fd, A_userEvCodeB , usrEvCodeB & 0x7ff);

  return RET_OK;
}




























//==============================================================================
//----===@ ts2ip_setEvgTrgSrc
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_setEvgTrgSrc(struct s_ts2ip* pIP)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);
	
  ts2ip_wr(l_fd, A_evTrgCtrl, 0 \
	                          | ((pIP->ev.cfg.evTrgSrcSel[1]  & 0x01) <<  1 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[2]  & 0x01) <<  2 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[3]  & 0x01) <<  3 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[4]  & 0x01) <<  4 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[5]  & 0x01) <<  5 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[6]  & 0x01) <<  6 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[7]  & 0x01) <<  7 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[8]  & 0x01) <<  8 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[9]  & 0x01) <<  9 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[10] & 0x01) << 10 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[11] & 0x01) << 11 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[12] & 0x01) << 12 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[13] & 0x01) << 13 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[14] & 0x01) << 14 ) \
	                          | ((pIP->ev.cfg.evTrgSrcSel[15] & 0x01) << 15 ) \
														);
  return RET_OK;
}

//==============================================================================
//----===@ ts2ip_setMxcFreq
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_setMxcFreq(struct s_ts2ip* pIP, unsigned int mxcN, unsigned int freq)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

  ts2ip_wr(l_fd, A_mxc14_prescalerReg_00 + mxcN*4, freq );
  return RET_OK;
}


//==============================================================================
//----===@ ts2ip_enableEvIntr
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_enableEvIntr(struct s_ts2ip* pIP, int enable)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);
	if(enable>0)  
		ts2ip_wr(l_fd, A_intrEnable, 1 );
	else
		ts2ip_wr(l_fd, A_intrEnable, 0 );
	
  return RET_OK;
}

//==============================================================================
//----===@ ts2ip_clearEvIntr
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_clearEvIntr(struct s_ts2ip* pIP)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

	ts2ip_wr(l_fd, A_intrClear, 1 );
	ts2ip_wr(l_fd, A_intrClear, 0 );

  return RET_OK;
}

//==============================================================================
//----===@ ts2ip_setEvIntr
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_setEvIntrMask(struct s_ts2ip* pIP, unsigned int maskVal)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

	ts2ip_wr(l_fd, A_intrMask, maskVal );
	
  return RET_OK;
}















//==============================================================================
//----===@ ts2ip_setMevCodeA
// Parameters  : evTrgN 0~15, evCodeB 11bit
// Description : 
//==============================================================================
STATUS ts2ip_setMevCodeA(struct s_ts2ip* pIP)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);
  
	int i;
	unsigned int* pmevA = pIP->ev.cfg.mevCodeA;

	for(i=0;i<MAX_mevCodeA;i++)
	{
  	ts2ip_wr(l_fd, Mem_evCodeA + (((i & 0x1f) + 1) * 4) , (pmevA[i] & 0x7ff));
  }

  return RET_OK;
}


//==============================================================================
//----===@ ts2ip_setSeqTable
// Parameters  : tableNum 0-A, 1-B
// Description : 
//==============================================================================
STATUS ts2ip_setSeqTable(struct s_ts2ip* pIP)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

	int i;
	unsigned int seqMemOffset;

	struct s_seqTable* p_seqTableA = pIP->ev.cfg.seqTableA;
	struct s_seqTable* p_seqTableB = pIP->ev.cfg.seqTableB;

	seqMemOffset = 0;
	for(i=0;i<MAX_SEQ_NUM;i++)
	{
		ts2ip_wr(l_fd, Mem_seqA_Config + seqMemOffset + (i*4),\
															 C_seq_option (p_seqTableA[i].evOption)  |\
															 C_seq_stop   (p_seqTableA[i].stopbit)   |\
															 C_seq_evCodeB(p_seqTableA[i].evCodeB));

		ts2ip_wr(l_fd, Mem_seqA_Config + seqMemOffset + 0x2000 + (i*4), 	p_seqTableA[i].timeStamp);
	}
  
	seqMemOffset = 0x4000;
	for(i=0;i<MAX_SEQ_NUM;i++)
	{
		ts2ip_wr(l_fd, Mem_seqA_Config + seqMemOffset + (i*4),\
															 C_seq_option (p_seqTableB[i].evOption)  |\
															 C_seq_stop   (p_seqTableB[i].stopbit)   |\
															 C_seq_evCodeB(p_seqTableB[i].evCodeB));

		ts2ip_wr(l_fd, Mem_seqA_Config + seqMemOffset + 0x2000 + (i*4), 	p_seqTableB[i].timeStamp);
	}

	return RET_OK;
}


/*==============================================================================
//----===@ ts2ip_setSeqTrgSrc
// Parameters  : maskA_sel 0-(OR0), 1-(OR1), 2-(OR0 and OR1)
// Description : 
//============================================================================*/
STATUS ts2ip_setSeqTrgSrc(struct s_ts2ip* pIP)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

	struct s_seqMask* psMask = &pIP->ev.cfg.seqMask;

	// evg seqTrgMskA0, A1 
	// seqTrgSrc 30    - usrTrg
	// seqTrgSrc 29:16 - mxc  13:0
	// seqTrgSrc 15:0  - exIn 15:0
	// seqTrg = A_seqTrgMskA0 | A_seqTrgMskA1 
	ts2ip_wr(l_fd, A_seqTrgMskA0, psMask->A_OR0); // select : mxc0
	ts2ip_wr(l_fd, A_seqTrgMskA1, psMask->A_OR1); // select : none
	ts2ip_wr(l_fd, A_seqTrgMskB0, psMask->B_OR0); // select : mxc0
	ts2ip_wr(l_fd, A_seqTrgMskB1, psMask->B_OR1); // select : none

	ts2ip_wr(l_fd, A_seqTrgCtrl, C_seqTrgSelA(psMask->A_sel) | C_seqTrgSelB(psMask->B_sel));
  
	return RET_OK;
}


//==============================================================================
//----===@ ts2ip_getGtpStat
// Parameters  :
// Description :
//==============================================================================
STATUS ts2ip_setRxDbus(struct s_ts2ip* pIP)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

	struct s_evConfig* pCfg = &pIP->ev.cfg;

	                                    // | C_rxDBusSel0(pCfg->rxDbusSrcSelect[0]) \
	                                    // | C_rxDBusSel1(pCfg->rxDbusSrcSelect[1]) \
	                                    // | C_rxDBusSel2(pCfg->rxDbusSrcSelect[2]) \
	                                    // | C_rxDBusSel3(pCfg->rxDbusSrcSelect[3]) \
	                                    // | C_rxDBusSel4(pCfg->rxDbusSrcSelect[4]) \
	                                    // | C_rxDBusSel5(pCfg->rxDbusSrcSelect[5]) \
	                                    // | C_rxDBusSel6(pCfg->rxDbusSrcSelect[6]) \
	                                    // | C_rxDBusSel7(pCfg->rxDbusSrcSelect[7]) \
																			// | C_evr_pdpTrgSrcSel(pCfg->pdpTrgSrcSel   ) \
	                                    // | C_swapExOut       (pCfg->swapExOut      ) \

  ts2ip_wr(l_fd, A_rxDBusSel_swapExOut, 0\
	                                    | C_rxDBusSel0(pCfg->rxDbusSrcSelect[0]) \
	                                    | C_rxDBusSel1(pCfg->rxDbusSrcSelect[1]) \
	                                    | C_rxDBusSel2(pCfg->rxDbusSrcSelect[2]) \
	                                    | C_rxDBusSel3(pCfg->rxDbusSrcSelect[3]) \
	                                    | C_rxDBusSel4(pCfg->rxDbusSrcSelect[4]) \
	                                    | C_rxDBusSel5(pCfg->rxDbusSrcSelect[5]) \
	                                    | C_rxDBusSel6(pCfg->rxDbusSrcSelect[6]) \
	                                    | C_rxDBusSel7(pCfg->rxDbusSrcSelect[7]) \
																			| C_evr_pdpTrgSrcSel(pCfg->pdpTrgSrcSel   ) \
																			);
  return RET_OK;
}

//==============================================================================
//----===@ ts2ip_setEvMappingRam
// Parameters  : 
// Description : 
//==============================================================================
STATUS ts2ip_setEvMappingRam(struct s_ts2ip* pIP)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

	int i;
	unsigned int* pdpCtrl = pIP->ev.cfg.evMappingRam;

	for(i=1;i<MAX_EVR_RAM_SIZE;i++)
	{
 		ts2ip_wr(l_fd, MemA_EVR_0 + 4*(i & 0x7ff), pdpCtrl[i]);
  }
  return RET_OK;
}


//==============================================================================
//----===@ ts2ip_setPdpPolarity
// Parameters  : 
// Description : 
//==============================================================================
STATUS ts2ip_setPdpPolarity(struct s_ts2ip* pIP)
{
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);

	ts2ip_wr(l_fd, A_pdpPolarity, 0\
															| C_bit00(pIP->ev.cfg.pdpPolarity[0] ) \
															| C_bit01(pIP->ev.cfg.pdpPolarity[1] ) \
															| C_bit02(pIP->ev.cfg.pdpPolarity[2] ) \
															| C_bit03(pIP->ev.cfg.pdpPolarity[3] ) \
															| C_bit04(pIP->ev.cfg.pdpPolarity[4] ) \
															| C_bit05(pIP->ev.cfg.pdpPolarity[5] ) \
															| C_bit06(pIP->ev.cfg.pdpPolarity[6] ) \
															| C_bit07(pIP->ev.cfg.pdpPolarity[7] ) \
															| C_bit08(pIP->ev.cfg.pdpPolarity[8] ) \
															| C_bit09(pIP->ev.cfg.pdpPolarity[9] ) \
															| C_bit10(pIP->ev.cfg.pdpPolarity[10]) \
															| C_bit11(pIP->ev.cfg.pdpPolarity[11]) \
															| C_bit12(pIP->ev.cfg.pdpPolarity[12]) \
															| C_bit13(pIP->ev.cfg.pdpPolarity[13]) \
															| C_bit14(pIP->ev.cfg.pdpPolarity[14]) \
															| C_bit15(pIP->ev.cfg.pdpPolarity[15]) \
															| C_bit16(pIP->ev.cfg.pdpPolarity[16]) \
															| C_bit17(pIP->ev.cfg.pdpPolarity[17]) \
															| C_bit18(pIP->ev.cfg.pdpPolarity[18]) \
															| C_bit19(pIP->ev.cfg.pdpPolarity[19]) \
															| C_bit20(pIP->ev.cfg.pdpPolarity[20]) \
															| C_bit21(pIP->ev.cfg.pdpPolarity[21]) \
															| C_bit22(pIP->ev.cfg.pdpPolarity[22]) \
															| C_bit23(pIP->ev.cfg.pdpPolarity[23]) \
															| C_bit24(pIP->ev.cfg.pdpPolarity[24]) \
															| C_bit25(pIP->ev.cfg.pdpPolarity[25]) \
															| C_bit26(pIP->ev.cfg.pdpPolarity[26]) \
															| C_bit27(pIP->ev.cfg.pdpPolarity[27]) \
															| C_bit28(pIP->ev.cfg.pdpPolarity[28]) \
															| C_bit29(pIP->ev.cfg.pdpPolarity[29]) \
															| C_bit30(pIP->ev.cfg.pdpPolarity[30]) \
															| C_bit31(pIP->ev.cfg.pdpPolarity[31]) \
															);
  return RET_OK;
}

//==============================================================================
//----===@ ts2ip_setPdpShape
// Parameters  : 
// Description : 
//==============================================================================
STATUS ts2ip_setPdpShape(struct s_ts2ip* pIP)
{
	int i;
	int l_fd = pIP->ev.fd;
	ifRet(l_fd < 0);
	
	struct s_pdpConfig* pPdp = pIP->ev.cfg.pdp;

	for(i=0;i<MAX_PDP_NUM;i++)
	{
		ts2ip_wr(l_fd, MemC_EVR_pdpDelay + 4 * i, pPdp[i].delay);
		ts2ip_wr(l_fd, MemC_EVR_pdpWidth + 4 * i, pPdp[i].width);
	}

	ts2ip_wr(l_fd, A_evr_RegUpdate, C_evr_setCtrlReg(1)); 
	taskDelay(10);
	ts2ip_wr(l_fd, A_evr_RegUpdate, C_evr_setCtrlReg(0)); 
	taskDelay(1000);

  return RET_OK;
}































































//==============================================================================
//----===@ prnIpInfo
// Parameters  :
// Description :
//==============================================================================
static STATUS prnIpInfo(struct s_ipInfo *pInfo)
{
	ifRet(pInfo == NULL);

  prnM2("------------------------------------------------------\r\n");
  prnM2("++ prnIpInfo ++\r\n");
	prnM2_fmtHex("vendor", pInfo->vendor      , " ");
	prnM2("%-22s: %02x.%02x\r\n"      ,\
							"ipID.sub"            ,\
			        pInfo->ipID        ,\
			        pInfo->ipID_sub );

	prnM2("%-22s: %02x.%02x%01x\r\n"   ,\
							"ipVer"                ,\
			        pInfo->ipVer_major  ,\
			        pInfo->ipVer_miner  ,\
			        pInfo->ipVer_rev);

prnM2("%-22s: %02d.%02d.%02d/%02d:%02d:%02d\r\n",\
							"fpga Build Time"                 ,\
			        pInfo->buildTime.year          ,\
			        pInfo->buildTime.month         ,\
			        pInfo->buildTime.day           ,\
			        pInfo->buildTime.hour          ,\
			        pInfo->buildTime.min           ,\
			        pInfo->buildTime.sec);
  prnM2("------------------------------------------------------\r\n");

  return RET_OK;
}




//==============================================================================
//----===@ prnEvStat
// Parameters  :
// Description :
//==============================================================================
STATUS prnEvStat(struct s_ts2ip* pIP)
{
	ifRet(pIP->ev.fd < 0);
	ts2ip_getEvStat(pIP);

  int i;
  int rdData;
	int l_fd = pIP->ev.fd;
	struct s_evStat* pStat = &pIP->ev.stat;

  prnM2("======================================================\r\n");
	prnM2("prnEvStat\r\n");
  prnM2("======================================================\r\n");

	prnIpInfo(&pIP->ev.info);
  
	prnM2("%-22s: %03d/%02d:%02d:%02d.%03d\r\n", \
							"EVR Time"            ,\
			        pStat->evrTime.day   ,\
			        pStat->evrTime.hour  ,\
			        pStat->evrTime.min   ,\
			        pStat->evrTime.sec   ,\
			        pStat->evrTime.ms);

	prnM2_fmtDec("isMaster      ", pStat->isMaster      , "[1:master (1st floor), 0:2nd floor]");
	
	prnM2_fmtDec("FMC PRSNT_M2C" , pStat->PRSNT_M2C_L    , "[0:Loss]");

	prnM2_fmtDec("f_SFP_Loss A  ", pStat->f_SFP_LossA    , "[1:Loss]");
	prnM2_fmtDec("f_SFP_Loss B  ", pStat->f_SFP_LossB    , "[1:Loss]");

	prnM2_fmtDec("f_SFP_present A", pStat->f_SFP_prsntA, "[1:Present]");
	prnM2_fmtDec("f_SFP_present B", pStat->f_SFP_prsntB, "[1:Present]");

	prnM2_fmtDec("fan A", pStat->fanA      , " ");
	prnM2_fmtDec("fan B", pStat->fanB      , " ");
	prnM2_fmtDec("fan C", pStat->fanC      , " ");
	prnM2_fmtDec("fan D", pStat->fanD      , " ");


	prnM2_fmtHex("emergency Cntr", pStat->emergencyStopCntr      , " ");

	prnM2_fmtDec("seqA_isRunning", pStat->seqA_isRunning      , " ");
	prnM2_fmtDec("seqB_isRunning", pStat->seqB_isRunning      , " ");

	for( i = 0 ; i < 32; i = i + 1)
		prnM2("pdp[%02d]               : [%01d] %5d runs, %5d err\r\n", i, pStat->pdp[i].isRunning, pStat->pdp[i].runCnt, pStat->pdp[i].errCnt);

	prnM2("\r\n");
	prnM2("\r\n");
  return RET_OK;
}

//==============================================================================
//----===@ prnGtpStat
// Parameters  :
// Description :
//==============================================================================
STATUS prnGtpStat(struct s_ts2ip* pIP)
{
	ifRet(pIP->gtp.fd < 0);
  ts2ip_getGtpStat(pIP);

  int rdData;
	int l_fd = pIP->gtp.fd;
	struct s_gtpStat* pStat = &pIP->gtp.stat;

	prnM2("======================================================\r\n");
	prnM2("prnGtpStat\r\n");
  prnM2("======================================================\r\n");

	prnIpInfo(&pIP->gtp.info);

  prnM2_fmtDec("gtp_soft_reset    ", pStat->gtp_soft_reset     , " ");
  prnM2("------------------------------------------------------\r\n");
  prnM2_fmtDec("gt0_cplllock      ", pStat->gt0_cplllock       , " ");
  prnM2_fmtDec("gt0_txfsmresetdone", pStat->gt0_txfsmresetdone , " ");
  prnM2_fmtDec("gt0_rxfsmresetdone", pStat->gt0_rxfsmresetdone , " ");
  prnM2("------------------------------------------------------\r\n");
  prnM2_fmtDec("gt1_cplllock      ", pStat->gt1_cplllock       , " ");
  prnM2_fmtDec("gt1_txfsmresetdone", pStat->gt1_txfsmresetdone , " ");
  prnM2_fmtDec("gt1_rxfsmresetdone", pStat->gt1_rxfsmresetdone , " ");
  prnM2("------------------------------------------------------\r\n");
  prnM2_fmtDec("gt0_trackOK"       , pStat->gt0_trackOK     , "<------");
  prnM2_fmtDec("gt1_trackOK"       , pStat->gt1_trackOK     , "<------");
  prnM2("------------------------------------------------------\r\n");
  prnM2_fmtDec("gt0_trackLossCnt  ", pStat->gt0_trackLossCnt   , " ");
  prnM2_fmtDec("gt1_trackLossCnt  ", pStat->gt1_trackLossCnt   , " ");
	prnM2("------------------------------------------------------\r\n");
  prnM2_fmtDec("gt0_txClk_cntr  ", pStat->gt0_txClk_cntr   , " ");
  prnM2_fmtDec("gt0_rxClk_cntr  ", pStat->gt0_rxClk_cntr   , " ");
  prnM2_fmtDec("gt1_txClk_cntr  ", pStat->gt1_txClk_cntr   , " ");
  prnM2_fmtDec("gt1_rxClk_cntr  ", pStat->gt1_rxClk_cntr   , " ");

	prnM2("\r\n");
	prnM2("\r\n");

  return RET_OK;
}


