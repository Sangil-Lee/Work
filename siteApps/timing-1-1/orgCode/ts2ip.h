/*******************************************************************************
 *                                                                             *
 *  Copyright (c) 2014 ~ by DURUTRONIX. All Rights Reserved.                   *
 *                                                                             *
 ******************************************************************************/

/*==============================================================================
                        EDIT HISTORY FOR MODULE

when                who            what, where, why
------------------- -------------  ---------------------------------------------
2018-05-07 14:53:53 laykim         Create
==============================================================================*/
#if !defined( __ts2ip_h__ )
#define __ts2ip_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "inc/com_def.h"
#include "ts2regmap.h"


/************************** Constant Definitions *****************************/
#define DRV_NAME_ip_ev  "/dev/raonts2ev_dev"
#define DRV_NAME_ip_gtp "/dev/raonts2gtp_dev"

#define RAON_EVG (1)
#define RAON_EVR (2)
#define RAON_EVF (3)
#define RAON_EVS (4) // ev stand alone

#define RAON_EVT (9) // ev system test mode

#define CPS_GT0  (0)
#define CPS_GT1  (1)
#define CPS_FMC0 (8)
#define CPS_FMC1 (9)
#define CPS_FMC2 (10)
#define CPS_FMC3 (11)
#define CPS_SL   (12)
#define CPS_SH   (13)
#define CPS_CPS  (14) // ctrl <-> ctrl

#define CPS_SW_CTRL (0)
#define CPS_SW_SW   (1) //SL <-> SH
#define CPS_SW_FR   (2) //front <-> rear (same floor)
#define CPS_SW_X    (3) //front <-> rear (cross floor)

#define CPS_SW_00   (4)
#define CPS_SW_01   (5)
#define CPS_SW_02   (6)
#define CPS_SW_03   (7)
#define CPS_SW_04   (8)
#define CPS_SW_05   (9)
#define CPS_SW_06   (10)
#define CPS_SW_07   (11)
#define CPS_SW_08   (12)
#define CPS_SW_09   (13)
#define CPS_SW_10   (14)
#define CPS_SW_11   (15)

#define TS2Floor_SL  (0)
#define TS2Floor_SH  (1)



#define TS2IP_seqN_A   (0)
#define TS2IP_seqN_B   (1)

#define TS2IP_seq_Enable   (0)
#define TS2IP_seq_Disable  (1)
#define TS2IP_seq_Resume   (2)

struct s_ts2ip{
  int                isInit ;
	struct s_ts2ev     ev     ;	
  struct s_ts2gtp    gtp    ;
};


STATUS ts2ip_init         (struct s_ts2ip* pIP);
STATUS ts2ip_resetGtp     (struct s_ts2ip* pIP);

STATUS ts2ip_setSlv_IO    (struct s_ts2ip* pIP);
STATUS ts2ip_setEvgTime   (struct s_ts2ip* pIP);
STATUS ts2ip_genUsrTrg    (struct s_ts2ip* pIP);
STATUS ts2ip_genUsrEvCodeB(struct s_ts2ip* pIP, unsigned int usrEvCodeB);
STATUS ts2ip_seqResume    (struct s_ts2ip* pIP, unsigned int seqN);

STATUS ts2ip_setTxDbusIRIGB (struct s_ts2ip* pIP);
STATUS ts2ip_setEvgTrgSrc   (struct s_ts2ip* pIP);
STATUS ts2ip_setMxcFreq     (struct s_ts2ip* pIP, unsigned int mxcN, unsigned int freq);
STATUS ts2ip_setMevCodeA    (struct s_ts2ip* pIP);
STATUS ts2ip_setSeqTable    (struct s_ts2ip* pIP);
STATUS ts2ip_setSeqTrgSrc   (struct s_ts2ip* pIP);
STATUS ts2ip_setRxDbus      (struct s_ts2ip* pIP);
STATUS ts2ip_setEvMappingRam(struct s_ts2ip* pIP);
STATUS ts2ip_setPdpPolarity (struct s_ts2ip* pIP);
STATUS ts2ip_setPdpShape    (struct s_ts2ip* pIP);

STATUS ts2ip_setEvgEnable(struct s_ts2ip* pIP);
STATUS ts2ip_setEvrEnable(struct s_ts2ip* pIP);

STATUS ts2ip_getGtpStat(struct s_ts2ip* pIP);
STATUS ts2ip_getEvStat(struct s_ts2ip* pIP);

STATUS ts2ip_enableEvIntr (struct s_ts2ip* pIP, int enable);
STATUS ts2ip_clearEvIntr(struct s_ts2ip* pIP);
STATUS ts2ip_setEvIntrMask(struct s_ts2ip* pIP, unsigned int maskVal);


STATUS prnEvStat (struct s_ts2ip* pIP);
STATUS prnGtpStat(struct s_ts2ip* pIP);


#ifdef __cplusplus
}
#endif
#endif
/*==============================================================================
                                  END OF FILE
==============================================================================*/
