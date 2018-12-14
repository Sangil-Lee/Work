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
#if !defined( __timingData_h__ )
#define __timingData_h__

#ifdef __cplusplus
extern "C" {
#endif

#include "commonDefine.h"

//#############################################################################
// ipSys
//#############################################################################

// axi clock rate : 100 MHz

#define DRV_NAME_ip_ev  "/dev/raonts2ev_dev"
#define DRV_NAME_ip_gtp "/dev/raonts2gtp_dev"

#define MAGIC_NUM 0xDB
#define IOCTL_R           _IOWR (MAGIC_NUM, 1, struct s_regRW)
#define IOCTL_W           _IOWR (MAGIC_NUM, 2, struct s_regRW)

#define TS_OP_MODE_NORMAL     (0)    // timing system operation mode - normal.
#define TS_OP_MODE_DELAYCHECK (1)    // timing system operation mode - delay check.

#define RAON_EVG (1)
#define RAON_EVR (2)
#define RAON_EVF (3)
#define RAON_EVS (4) // ev stand alone
#define RAON_EVR_FP (5)
#define RAON_MODE_MAX (5)

#define TS2Floor_SL  (0)
#define TS2Floor_SH  (1)

#define TS2IO_ModeInput  (0)
#define TS2IO_ModeOutput (1)

#define TS2IP_seqN_A   (0)
#define TS2IP_seqN_B   (1)

#define TS2IP_seq_Enable   (0)
#define TS2IP_seq_Disable  (1)
#define TS2IP_seq_Resume   (2)

#define TS2_REF_CLK_FREQ    81250000

//-----------------------------------------------------------------------------
// Define : ts2ev memory map
//-----------------------------------------------------------------------------
#define Mem1_mevCodeA 0x1000
#define Mem2_seqA_0A  0x2000
#define Mem3_seqA_0B  0x3000
#define Mem4_seqA_1A  0x4000
#define Mem5_seqA_1B  0x5000
#define Mem6_seqB_0A  0x6000
#define Mem7_seqB_0B  0x7000
#define Mem8_seqB_1A  0x8000
#define Mem9_seqB_1B  0x9000
#define MemA_EVR_0    0xA000
#define MemB_EVR_1    0xB000

#define MemC_EVR_pdpDelay 0xC400          //ctrl
#define MemC_EVR_pdpWidth 0xC480          //ctrl
#define MemC_EVR_pdpStat  0xC500          //status

#define Mem_evCodeA          Mem1_mevCodeA
#define Mem_seqA_Config      Mem2_seqA_0A
#define Mem_seqA_TimeStamp   Mem4_seqA_1A
#define Mem_seqB_Config      Mem6_seqB_0A
#define Mem_seqB_TimeStamp   Mem8_seqB_1A
#define Mem_evrDec           MemA_EVR_0

//-----------------------------------------------------------------------------
// Define : ipSys
//-----------------------------------------------------------------------------
#define C_ipSys_intrEnable          (BIT0)
#define C_ipSys_intrClear           (BIT0)

#define C_ipSys_rFifo_RST_0         (BIT0)
#define C_ipSys_rFifo_RST_1         (BIT1)
#define C_ipSys_rFifo_RST_2         (BIT2)
#define C_ipSys_rFifo_RST_3         (BIT3)
#define C_ipSys_wFifo_RST_0         (BIT4)
#define C_ipSys_wFifo_RST_1         (BIT5)
#define C_ipSys_wFifo_RST_2         (BIT6)
#define C_ipSys_wFifo_RST_3         (BIT7)

//-----------------------------------------------------------------------------
// Define : ts2ev user
//-----------------------------------------------------------------------------
#define MAX_SEQ_NUM      2048
#define MAX_EVR_RAM_SIZE 2048

#define MAX_MXC_NUM      14
#define MAX_mevCodeA     16
#define MAX_PDP_NUM      32
#define MAX_OUT_NUM      32

#define C_intrMsk_evCodeA           (BIT0)
#define C_intrMsk_evrW_alarm_sec    (BIT1)

#define C_intrMsk_evCodeB_7F0       (BIT16)
#define C_intrMsk_evCodeB_7F1       (BIT17)
#define C_intrMsk_evCodeB_7F2       (BIT18)
#define C_intrMsk_evCodeB_7F3       (BIT19)
#define C_intrMsk_evCodeB_7F4       (BIT20)
#define C_intrMsk_evCodeB_7F5       (BIT21)
#define C_intrMsk_evCodeB_7F6       (BIT22)
#define C_intrMsk_evCodeB_7F7       (BIT23)
#define C_intrMsk_evCodeB_7F8       (BIT24)
#define C_intrMsk_evCodeB_7F9       (BIT25)
#define C_intrMsk_evCodeB_7Fa       (BIT26)
#define C_intrMsk_evCodeB_7Fb       (BIT27)
#define C_intrMsk_evCodeB_7Fc       (BIT28)
#define C_intrMsk_evCodeB_7Fd       (BIT29)
#define C_intrMsk_evCodeB_7Fe       (BIT30)
#define C_intrMsk_evCodeB_7Ff       (BIT31)

#define C_seq_stop(sv)              ((sv & 0x1) << 15)
#define C_seq_evCodeB(sv)           ((sv & 0x7ff) << 0)

/*--- sequence trigger source define ---*/
// evSrc = {1'h0, exIn32[15:0], mxc14[13:0], usrTrg};
#define seqS_usrTrg  BIT0 
#define seqS_mxc00   BIT1 
#define seqS_mxc01   BIT2 
#define seqS_mxc02   BIT3 
#define seqS_mxc03   BIT4 
#define seqS_mxc04   BIT5 
#define seqS_mxc05   BIT6 
#define seqS_mxc06   BIT7 
#define seqS_mxc07   BIT8 
#define seqS_mxc08   BIT9 
#define seqS_mxc09   BIT10
#define seqS_mxc10   BIT11
#define seqS_mxc11   BIT12
#define seqS_mxc12   BIT13
#define seqS_mxc13   BIT14
#define seqS_ext00   BIT15
#define seqS_ext01   BIT16
#define seqS_ext02   BIT17
#define seqS_ext03   BIT18
#define seqS_ext04   BIT19
#define seqS_ext05   BIT20
#define seqS_ext06   BIT21
#define seqS_ext07   BIT22
#define seqS_ext08   BIT23
#define seqS_ext09   BIT24
#define seqS_ext10   BIT25
#define seqS_ext11   BIT26
#define seqS_ext12   BIT27
#define seqS_ext13   BIT28
#define seqS_ext14   BIT29
#define seqS_ext15   BIT30

/*--- sequence trigger source combination ---*/
#define seqS_CMB_NONE          0 
#define seqS_CMB_OR0           1
#define seqS_CMB_OR1           2
#define seqS_CMB_OR0_and_OR1   3
#define seqS_CMB_OR2           5
#define seqS_CMB_OR3           6
#define seqS_CMB_OR2_and_OR3   7

//-----------------------------------------------------------------------------
// Define : ev 
//-----------------------------------------------------------------------------
/*--- A_setSysEvg                   slv_reg30 ---*/
#define SET_evgW_rst                (BIT31)
#define SET_igen_rst                (BIT29)
#define SET_idec_rst                (BIT28)
#define SET_tx_DBusSel_set          (BIT7)
#define SET_evSrc_set               (BIT6)
#define SET_usrTrg                  (BIT5)
#define SET_getTime                 (BIT4)
#define SET_igen_set_time           (BIT3)
#define SET_sw_evgW_set_time        (BIT2) 
#define SET_seqTrgA_resume          (BIT1)
#define SET_seqTrgB_resume          (BIT0)

/*--- A_setEvr                      slv_reg60 ---*/
#define SET_evrW_rst                (BIT31) // evr watch reset
#define SET_evr_setCtrlReg          (BIT0)
#define SET_evr_getStatReg          (BIT4)

/*--- A_setGtp_reset                slv_reg30 ---*/
#define SET_gtp_reset               (BIT31)


//-----------------------------------------------------------------------------
// Status : ipSys 
//-----------------------------------------------------------------------------
#define A_ipSys_vendor            slv_reg00 // 32'h44555255, // ascii "DURU"
#define A_ipSys_info              slv_reg01 // {ipid_h8[7:0], s00_axi_ipid_l8, ipversion[15:0]};
#define A_ipSys_buildTime         slv_reg02 // {d9, y6, h5, m6, s6};
#define A_ipSys_clkCntr           slv_reg03 // 
#define A_ipSys_intrReg           slv_reg04 //

#define A_ipSys_setting           slv_reg08 //
#define A_ipSys_config            slv_reg09 //
#define A_ipSys_intrMask          slv_reg0a //
#define A_ipSys_fifoReset         slv_reg0f //
#define A_ipSys_rFifoStat0        slv_reg70 //
#define A_ipSys_rFifoData0        slv_reg71 //
#define A_ipSys_rFifoStat1        slv_reg72 //
#define A_ipSys_rFifoData1        slv_reg73 //
#define A_ipSys_rFifoStat2        slv_reg74 //
#define A_ipSys_rFifoData2        slv_reg75 //
#define A_ipSys_rFifoStat3        slv_reg76 //
#define A_ipSys_rFifoData3        slv_reg77 //
#define A_ipSys_wFifoStat0        slv_reg78 //
#define A_ipSys_wFifoData0        slv_reg79 //
#define A_ipSys_wFifoStat1        slv_reg7a //
#define A_ipSys_wFifoData1        slv_reg7b //
#define A_ipSys_wFifoStat2        slv_reg7c //
#define A_ipSys_wFifoData2        slv_reg7d //
#define A_ipSys_wFifoStat3        slv_reg7e //
#define A_ipSys_wFifoData3        slv_reg7f //

//#############################################################################
// ts2ev ip
//#############################################################################

//-----------------------------------------------------------------------------
// Status : ts2ev EVG 
//-----------------------------------------------------------------------------
#define A_ifanStt4_CntrFront      slv_reg10
#define A_ifanStt4_CntrRear       slv_reg11
#define A_boardStat               slv_reg12

#define A_seqTrg_runState         slv_reg18 // idec_status[30], seqTrgA_runState[1:0], seqTrgB_runState[1:0]

#define A_fifo_evCodeA_STAT       slv_reg19 
#define A_fifo_seqA_STAT          slv_reg1a 
#define A_fifo_seqB_STAT          slv_reg1b 
#define A_fifo_evLogT0_STAT       A_ipSys_rFifoStat0
#define A_fifo_evLogT1_STAT       A_ipSys_rFifoStat1
#define A_fifo_evLogEv_STAT       A_ipSys_rFifoStat2
#define A_fifo_evLogPdp_STAT      A_ipSys_rFifoStat3
#define A_fifo_evUser_STAT        A_ipSys_wFifoStat0

#define A_tx_evCode_cntr          slv_reg1c // {tx_evCodeA_cntr, tx_evCodeB_cntr};
#define A_evrW_evgW_set_terr      slv_reg1d // {evrW_set_terr_cnt16,evgW_set_terr_cnt16};
#define A_evgW_time0              slv_reg1e // {evgW_d9, evgW_y7[5:0], evgW_h5, evgW_m6, evgW_s6};  
#define A_evgW_time1              slv_reg1f // {evgW_y7[6], 1'h0, evgW_ms10, evgW_cntr20};

//-----------------------------------------------------------------------------
// Status : ts2ev EVR
//-----------------------------------------------------------------------------
#define A_rx_evCode_cntr          slv_reg20 // {rx_evCodeA_cntr, rx_evCodeB_cntr};
#define A_streamRx_evCode_cntr    slv_reg21 // {streamRx_evCodeA_cnt, streamRx_evCodeB_cnt};
#define A_emergencyStopCntr       slv_reg22 // emergencyStopCntr[15:0]
#define A_pdpRunStt               slv_reg23
#define A_evrW_time0              slv_reg24 // {evrW_d9, evrW_y7[5:0], evrW_h5, evrW_m6, evrW_s6};  
#define A_evrW_time1              slv_reg25 // {evrW_y7[6], 1'h0, evrW_ms10, evrW_cntr20};
#define A_evrRx_linkErr           slv_reg26 // 
#define A_evrRx_linkErrCntr       slv_reg27 // 

//-----------------------------------------------------------------------------
// Control : ts2ev System & EVG 
//-----------------------------------------------------------------------------
#define A_setSysEvg               slv_reg30 //
#define A_cfgSys_IO_LSB           slv_reg31 // m_st[15: 0]
#define A_cfgSys_IO_MSB           slv_reg32 // m_st[31:16]
#define A_cfgSys_FMC              slv_reg33
#define A_cfgEvg_igen_0           slv_reg34
#define A_cfgEvg_igen_1           slv_reg35
#define A_cfgEvg_sw_evgW0         slv_reg36
#define A_cfgEvg_sw_evgW1         slv_reg37
#define A_cfgEvg_enable           slv_reg38
// #define A_cfgEvg_tx_DBusSel       slv_reg40
#define A_cfgEvg_evTrgMsk         slv_reg41
// #define A_cfgEvg_evTrgSel         slv_reg42
#define A_cfgEvg_seqTrgSel        slv_reg43
#define A_cfgEvg_seqTrgMskA0      slv_reg44
#define A_cfgEvg_seqTrgMskA1      slv_reg45
#define A_cfgEvg_seqTrgMskB0      slv_reg46
#define A_cfgEvg_seqTrgMskB1      slv_reg47
#define A_cfgEvg_seqA_repeatReg   slv_reg48
#define A_cfgEvg_seqB_repeatReg   slv_reg49
#define A_cfgEvg_mxcFreq_00       slv_reg50
#define A_cfgEvg_mxcFreq_01       slv_reg51
#define A_cfgEvg_mxcFreq_02       slv_reg52
#define A_cfgEvg_mxcFreq_03       slv_reg53
#define A_cfgEvg_mxcFreq_04       slv_reg54
#define A_cfgEvg_mxcFreq_05       slv_reg55
#define A_cfgEvg_mxcFreq_06       slv_reg56
#define A_cfgEvg_mxcFreq_07       slv_reg57
#define A_cfgEvg_mxcFreq_08       slv_reg58
#define A_cfgEvg_mxcFreq_09       slv_reg59
#define A_cfgEvg_mxcFreq_10       slv_reg5a
#define A_cfgEvg_mxcFreq_11       slv_reg5b
#define A_cfgEvg_mxcFreq_12       slv_reg5c
#define A_cfgEvg_mxcFreq_13       slv_reg5d

#define A_cfgEvg_evTrgSelA        slv_reg65
#define A_cfgEvg_evTrgSelB        slv_reg66
#define A_cfgEvg_evTrgSelC        slv_reg67
#define A_cfgEvg_evTrgSelD        slv_reg68
#define A_cfgEvg_evTrgSelE        slv_reg69
#define A_cfgEvg_evTrgSelF        slv_reg6a
#define A_cfgEvg_evTrgSelG        slv_reg6b
#define A_cfgEvg_evTrgSelH        slv_reg6c

#define A_cfgEvg_tx_DBusSelA      slv_reg6d
#define A_cfgEvg_tx_DBusSelB      slv_reg6e

//-----------------------------------------------------------------------------
// Control : ts2ev EVR
//-----------------------------------------------------------------------------
#define A_setEvr                  slv_reg60 //
#define A_cfgEvr_enable           slv_reg61 //
#define A_cfgEvr_rx_DBusSel       slv_reg62 //
#define A_cfgEvr_evLogFifo_Enable slv_reg63 // slv_reg63[0];
#define A_cfgEvr_pdpPolarity      slv_reg64 //

//-----------------------------------------------------------------------------
// Read, Write Fifo 
//-----------------------------------------------------------------------------
#define A_get_evLogT0       A_ipSys_rFifoData0
#define A_get_evLogT1       A_ipSys_rFifoData1
#define A_get_evLogEv       A_ipSys_rFifoData2
#define A_get_evLogPdp      A_ipSys_rFifoData3
#define A_set_evUser        A_ipSys_wFifoData0

//#############################################################################
// ts2gtp ip
//#############################################################################

//-----------------------------------------------------------------------------
// STATUS : ts2gtp ip
//-----------------------------------------------------------------------------
  //  status check
#define A_sttGtp_ip          slv_reg10
#define A_sttGtp_trackLoss   slv_reg11
#define A_sttGtp_gt0clk_cntr slv_reg12 // {gt0_txClk_cntr,gt0_rxClk_cntr} ;
#define A_sttGtp_gt1clk_cntr slv_reg13 // {gt1_txClk_cntr,gt1_rxClk_cntr} ;
#define A_sttGtp_chkDelayCnt slv_reg14

//-----------------------------------------------------------------------------
// Control : ts2gtp ip
//-----------------------------------------------------------------------------
#define A_setGtp_reset       slv_reg30
#define A_cfgGtp             slv_reg31


//#############################################################################
//
//#############################################################################
#define DRV_NAME_iic_bp "/dev/i2c-1"
#define DRV_NAME_iic_ob "/dev/i2c-2"
#define DRV_NAME_iic_sl "/dev/i2c-4"
#define DRV_NAME_iic_sh "/dev/i2c-3"
#define DRV_NAME_lcd    "/dev/ttyUL1"

#define IIC_BUS_bp (0)
#define IIC_BUS_ob (1)
#define IIC_BUS_sl (2)
#define IIC_BUS_sh (3)
#define IIC_BUS_MAX (3)

#define REF_CLK_Ext     0
#define REF_CLK_ob25MHz 1

#define CPS_GT0      (0)
#define CPS_GT1      (1)
#define CPS_FMC0     (8)
#define CPS_FMC1     (9)
#define CPS_FMC2     (10)
#define CPS_FMC3     (11)
#define CPS_SL       (12)
#define CPS_SH       (13)
#define CPS_CPS      (14) // ctrl <-> ctrl

#define CPS_SW_CTRL  (0)
#define CPS_SW_SW    (1) //SL <-> SH
#define CPS_SW_FR    (2) //front <-> rear (same floor)
#define CPS_SW_X     (3) //front <-> rear (cross floor)

#define CPS_SW_00    (4)
#define CPS_SW_01    (5)
#define CPS_SW_02    (6)
#define CPS_SW_03    (7)
#define CPS_SW_04    (8)
#define CPS_SW_05    (9)
#define CPS_SW_06    (10)
#define CPS_SW_07    (11)
#define CPS_SW_08    (12)
#define CPS_SW_09    (13)
#define CPS_SW_10    (14)
#define CPS_SW_11    (15)

#define CPS_PORT_EN  (0x30)
#define CPS_PORT_DIS (0x00)

#define IIC_maxDrv (4)
#define IIC_maxDrvNameL (15)
#define IIC_maxSlv_bp (1)

#define LCD_LINE_1 1
#define LCD_LINE_2 2
#define LCD_LINE_3 3
#define LCD_LINE_4 4
#define LCD_LINE_5 5
#define LCD_LINE_6 6
#define LCD_LINE_7 7
#define LCD_LINE_8 8
#define LCD_LINE_9 9

/************************** Constant Definitions *****************************/
#define IIC_SLAVE_FORCE 0x0706
#define IIC_SLAVE       0x0703  /* Change slave address     */
#define IIC_FUNCS       0x0705  /* Get the adapter functionality */
#define IIC_RDWR        0x0707  /* Combined R/W transfer (one stop only)*/

#define MAX_IIC_RW_BUF_SIZE (512)

/* Slave IIC Device Number... exclude IIC MUX */
#define IIC_maxSlv_bp (1)
#define IIC_maxSlv_ob (4)
#define IIC_maxSlv_sl (4)
#define IIC_maxSlv_sh (4)

/* slv iic name by num*/
#define SLV_MAX7313_A (0)
#define SLV_SI5338A   (1)
#define SLV_SI570     (2)
#define SLV_ADN4604   (3)
#define SLV_INA226A   (4)
#define SLV_MAX7313_B (5)
#define SLV_TMP112A   (6)

#define IIC_MUX_ADDR  (0x74) // PCA9546

/************************** si5338 *****************************/
#define IIC_SCLK_RATE       400000
#define IIC_ADDR            0x70
#define IIC_BUS             0

#define LOS_MASK_IN1IN2IN3  0x04
#define LOS_MASK            LOS_MASK_IN1IN2IN3
#define PLL_LOL             0x10
#define LOS_FDBK            0x08
#define LOS_CLKIN           0x04
#define SYS_CAL             0x01
#define LOCK_MASK           (PLL_LOL | LOS_CLKIN | SYS_CAL)
#define FCAL_OVRD_EN        0x80
#define SOFT_RESET          0x02
#define EOB_ALL             0x10
#define DIS_LOL             0x80


enum ev_src{
  USER_TRIGGER,

  MXC_00, MXC_01, MXC_02, MXC_03,
  MXC_04, MXC_05, MXC_06, MXC_07,
  MXC_08, MXC_09, MXC_10, MXC_11,
  MXC_12, MXC_13,
  
  EXT_IN_SL_00, EXT_IN_SL_01, EXT_IN_SL_02, EXT_IN_SL_03,
  EXT_IN_SL_04, EXT_IN_SL_05, EXT_IN_SL_06, EXT_IN_SL_07,
  EXT_IN_SL_08, EXT_IN_SL_09, EXT_IN_SL_10, EXT_IN_SL_11,
  EXT_IN_SL_12, EXT_IN_SL_13, EXT_IN_SL_14, EXT_IN_SL_15,
  EXT_IN_SH_00, EXT_IN_SH_01, EXT_IN_SH_02, EXT_IN_SH_03,
  EXT_IN_SH_04, EXT_IN_SH_05, EXT_IN_SH_06, EXT_IN_SH_07,
  EXT_IN_SH_08, EXT_IN_SH_09, EXT_IN_SH_10, EXT_IN_SH_11,
  EXT_IN_SH_12, EXT_IN_SH_13, EXT_IN_SH_14, EXT_IN_SH_15,
  EV_SRC_MAX
};

enum ev_out{
  EXT_OUT_SL_00, EXT_OUT_SL_01, EXT_OUT_SL_02, EXT_OUT_SL_03,
  EXT_OUT_SL_04, EXT_OUT_SL_05, EXT_OUT_SL_06, EXT_OUT_SL_07,
  EXT_OUT_SL_08, EXT_OUT_SL_09, EXT_OUT_SL_10, EXT_OUT_SL_11,
  EXT_OUT_SL_12, EXT_OUT_SL_13, EXT_OUT_SL_14, EXT_OUT_SL_15,
  EXT_OUT_SH_00, EXT_OUT_SH_01, EXT_OUT_SH_02, EXT_OUT_SH_03,
  EXT_OUT_SH_04, EXT_OUT_SH_05, EXT_OUT_SH_06, EXT_OUT_SH_07,
  EXT_OUT_SH_08, EXT_OUT_SH_09, EXT_OUT_SH_10, EXT_OUT_SH_11,
  EXT_OUT_SH_12, EXT_OUT_SH_13, EXT_OUT_SH_14, EXT_OUT_SH_15,
  EV_OUT_MAX
};

enum ev_out_src{
  evrOut_1pps,

  RXDBUS_00, RXDBUS_01, RXDBUS_02, RXDBUS_03,
  RXDBUS_04, RXDBUS_05, RXDBUS_06, RXDBUS_07,

  PDP_OUT_00, PDP_OUT_01, PDP_OUT_02, PDP_OUT_03,
  PDP_OUT_04, PDP_OUT_05, PDP_OUT_06, PDP_OUT_07,
  PDP_OUT_08, PDP_OUT_09, PDP_OUT_10, PDP_OUT_11,
  PDP_OUT_12, PDP_OUT_13, PDP_OUT_14, PDP_OUT_15,
  PDP_OUT_16, PDP_OUT_17, PDP_OUT_18, PDP_OUT_19,
  PDP_OUT_20, PDP_OUT_21, PDP_OUT_22, PDP_OUT_23,
  PDP_OUT_24, PDP_OUT_25, PDP_OUT_26, PDP_OUT_27,
  PDP_OUT_28, PDP_OUT_29, PDP_OUT_30, PDP_OUT_31,
  EV_OUT_SRC_MAX
};
//==============================================================================
//----===@ ip slave register direct access
//==============================================================================
struct s_regRW{
    uint         offset;     
    uint         val;
};


//==============================================================================
//----===@ get
//==============================================================================
struct s_evTime{
    char         name[32];  // time name
    uint         type;      // time type, 0:yyyy.mm.dd.hh.ss, 1:yyyy.mmm.hh.ss, 2:yyyy.mm.dd.hh.ss.ns, 3:yyyy.mm.dd.hh.ss.ms
    uint         year;
    uint         month;
    uint         day;
    uint         hour;
    uint         min;
    uint         sec;
    uint         ms;
    uint         tick;
};

struct s_ipInfo{
    char         name[32];
    uint         vendor;
    uint         ipID;
    uint         ipID_sub;
    uint         ipVer_major;
    uint         ipVer_miner;
    uint         ipVer_rev;
    uint         ipTick;
    struct s_evTime     buildTime;
};

struct s_gtpStat{
    int cpllfbclklost  ;
    int cplllock       ;
    int txresetdone    ;
    int txfsmresetdone ;
    int rxresetdone    ;
    int rxfsmresetdone ;
    int rxDataError    ;
    int track_data     ;
    int trackOK        ;
    int trackLossCnt   ;
    int txClk_cntr     ;
    int rxClk_cntr     ;
};

struct s_fifoStat{
    char name[32];
    int WRCOUNT;
    int RDCOUNT;
    int RDERR;
    int WRERR;
    int EMPTY;
    int ALMOSTEMPTY;
    int FULL;
    int ALMOSTFULL;
};


//==============================================================================
//----===@ config
//==============================================================================
struct s_seqTable{
    uint         stopbit  ;
    uint         evOption ;
    uint         evCodeB  ;
    uint         timeStamp;
} ;

struct s_pdp{
    unsigned char isRunning;
    uint          runCnt;
    uint          errCnt;
    uint          delay  ;
    uint          width ;
    uint          polarity ;
};

//==============================================================================
//----===@ system 
//==============================================================================
struct s_ts2ev{
    int                fd     ; 
    struct s_ipInfo    info   ; 

    /*--- Status : SYSTEM ---*/
    uint               fanA;
    uint               fanB;
    uint               fanC;
    uint               fanD;

    uint               isMaster;     
    uint               PRSNT_M2C_L;
    uint               f_SFP_LossA;
    uint               f_SFP_LossB;
    uint               f_SFP_prsntA;
    uint               f_SFP_prsntB;

    uint               slio;
    uint               shio;

    /*--- EVG ---*/
    uint               igen_enable;
    uint               igen_mode_ext;
    uint               idec_status;     
    uint               seqA_isRunning;
    uint               seqB_isRunning;

    uint               tx_evCodeA_cntr;     
    uint               tx_evCodeB_cntr;     

    uint               evrW_set_terr_cnt;
    uint               evgW_set_terr_cnt;

    struct s_fifoStat  fStat_evCodeA;
    struct s_fifoStat  fStat_seqA;
    struct s_fifoStat  fStat_seqB;
    struct s_fifoStat  fStat_evUser;
    struct s_fifoStat  fStat_evLogT0;
    struct s_fifoStat  fStat_evLogT1;
    struct s_fifoStat  fStat_evLogEv;
    struct s_fifoStat  fStat_evLogPdp;

    struct s_evTime    evgTime;

    float              mxcFreq_Hz[MAX_MXC_NUM];
    ev_src             tx_DBusSel[8];
    ev_src             evTrg[32];

    uint               seqTrgCmbSrcSel[2]; //seq trigger combination source select seqS_CMB_NONE ~ seqS_CMB_OR2_and_OR3
    uint               seqSrcOR[4];        //seq trigger source OR value (seqS_usrTrg, seqS_mxc00~13, seqS_ext00~31)
    uint               seq_repeatReg[2];

    uint               evg_txEn_DBus     ; // slv_reg38[31];
    uint               evg_txEn_evCodeA  ; // slv_reg38[30];
    uint               evg_txEn_evCodeB  ; // slv_reg38[29];
    uint               evg_txEn_mevCodeA ; // slv_reg38[28];
    uint               evg_seqTrgA_en    ; // slv_reg38[27];
    uint               evg_seqTrgB_en    ; // slv_reg38[26];
    uint               evg_mxc14Enable   ; // slv_reg38[25];

    /*--- EVR ---*/
    uint               evrRx_linkErr;
    uint               evrRx_linkErrCntr;
    uint               rx_evCodeA_cntr;     
    uint               rx_evCodeB_cntr;     

    uint               streamRx_evCodeA_cntr;     
    uint               streamRx_evCodeB_cntr;     

    uint               emergencyStopCntr;

    struct s_evTime    evrTime;

    uint               evr_rxEn_DBus      ; // slv_reg61[31];
    uint               evr_rxEn_evCodeA   ; // slv_reg61[30];
    uint               evr_rxEn_evCodeB   ; // slv_reg61[29];
    uint               evr_rxEn_streamData; // slv_reg61[27];
    uint               evr_pdpEn          ; // slv_reg61[0] | (!emergencyStop);

    struct s_pdp       pdp[MAX_PDP_NUM];
    ev_out_src         evOut[EV_OUT_MAX];  // slv_reg62[7:0]; if(rx_DBusSel8[7] == 1'b0)begin exOut32[7] <= `DLY rx_DBus[7]; end else begin exOut32[7] <= `DLY pdpOutput[7]; end

    uint               extOutSwap;         // swapExOut16     slv_reg62[31];
    uint               exOut31_1pps;       // extOutSel_1pps  slv_reg62[30];
    uint               pdpTrgMode;         // if set 1 : pdp 16~31 are triggered from rx_evCodeA.
                                           //            evr_pdpTrgSrcSel  slv_reg62[8];

    uint               evLogFifo_En_user ; // slv_reg63[0];
};

struct s_ts2gtp{
    int                fd              ;  
    struct s_ipInfo    info            ;

    int                reset           ;
    uint               chkDelayCnt     ;
    struct s_gtpStat   stat[2]         ;
};

struct s_ts2slv{
  int                busId;
  unsigned char      id  ;  

  unsigned char      cpsOut[16]   ;
  unsigned char      cpsOutEn[16] ;

  float              temp;
  uint               io  ;  
};

struct s_ts2iic{
  uint          name;
  unsigned char addr;
  unsigned char isMux;
  unsigned char muxAddress;
  unsigned char muxNum;
};


#ifdef __cplusplus
}
#endif
#endif
/*==============================================================================
                                  END OF FILE
==============================================================================*/
