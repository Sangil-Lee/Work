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
#if !defined( __ts2regmap_h__ )
#define __ts2regmap_h__

#ifdef __cplusplus
extern "C" {
#endif

#define slv_reg00 0x000
#define slv_reg01 0x004
#define slv_reg02 0x008
#define slv_reg03 0x00C
#define slv_reg04 0x010
#define slv_reg05 0x014
#define slv_reg06 0x018
#define slv_reg07 0x01C
#define slv_reg08 0x020
#define slv_reg09 0x024
#define slv_reg0a 0x028
#define slv_reg0b 0x02C
#define slv_reg0c 0x030
#define slv_reg0d 0x034
#define slv_reg0e 0x038
#define slv_reg0f 0x03C
#define slv_reg10 0x040
#define slv_reg11 0x044
#define slv_reg12 0x048
#define slv_reg13 0x04C
#define slv_reg14 0x050
#define slv_reg15 0x054
#define slv_reg16 0x058
#define slv_reg17 0x05C
#define slv_reg18 0x060
#define slv_reg19 0x064
#define slv_reg1a 0x068
#define slv_reg1b 0x06C
#define slv_reg1c 0x070
#define slv_reg1d 0x074
#define slv_reg1e 0x078
#define slv_reg1f 0x07C
#define slv_reg20 0x080
#define slv_reg21 0x084
#define slv_reg22 0x088
#define slv_reg23 0x08C
#define slv_reg24 0x090
#define slv_reg25 0x094
#define slv_reg26 0x098
#define slv_reg27 0x09C
#define slv_reg28 0x0A0
#define slv_reg29 0x0A4
#define slv_reg2a 0x0A8
#define slv_reg2b 0x0AC
#define slv_reg2c 0x0B0
#define slv_reg2d 0x0B4
#define slv_reg2e 0x0B8
#define slv_reg2f 0x0BC
#define slv_reg30 0x0C0
#define slv_reg31 0x0C4
#define slv_reg32 0x0C8
#define slv_reg33 0x0CC
#define slv_reg34 0x0D0
#define slv_reg35 0x0D4
#define slv_reg36 0x0D8
#define slv_reg37 0x0DC
#define slv_reg38 0x0E0
#define slv_reg39 0x0E4
#define slv_reg3a 0x0E8
#define slv_reg3b 0x0EC
#define slv_reg3c 0x0F0
#define slv_reg3d 0x0F4
#define slv_reg3e 0x0F8
#define slv_reg3f 0x0FC
#define slv_reg40 0x100
#define slv_reg41 0x104
#define slv_reg42 0x108
#define slv_reg43 0x10C
#define slv_reg44 0x110
#define slv_reg45 0x114
#define slv_reg46 0x118
#define slv_reg47 0x11C
#define slv_reg48 0x120
#define slv_reg49 0x124
#define slv_reg4a 0x128
#define slv_reg4b 0x12C
#define slv_reg4c 0x130
#define slv_reg4d 0x134
#define slv_reg4e 0x138
#define slv_reg4f 0x13C
#define slv_reg50 0x140
#define slv_reg51 0x144
#define slv_reg52 0x148
#define slv_reg53 0x14C
#define slv_reg54 0x150
#define slv_reg55 0x154
#define slv_reg56 0x158
#define slv_reg57 0x15C
#define slv_reg58 0x160
#define slv_reg59 0x164
#define slv_reg5a 0x168
#define slv_reg5b 0x16C
#define slv_reg5c 0x170
#define slv_reg5d 0x174
#define slv_reg5e 0x178
#define slv_reg5f 0x17C




#define A_vendor                  slv_reg00

#define A_ipInfo                  slv_reg01
#define S_ipID(sv)                ((sv >>  24) & 0xff)
#define S_ipID_sub(sv)            ((sv >>  16) & 0xff)
#define S_ipVer_major(sv)         ((sv >>  12) & 0xf)
#define S_ipVer_miner(sv)         ((sv >>  4) & 0xff)
#define S_ipVer_rev(sv)           ((sv >>  0) & 0xf)

#define A_buildTime               slv_reg02
#define S_buildYear(sv)           ((sv & 0x007E0000) >> 17) // 17
#define S_buildMonth(sv)          ((sv & 0x07800000) >> 23) // 23
#define S_buildDay(sv)            ((sv & 0xF8000000) >> 27) // 27 bit
#define S_buildHour(sv)           ((sv & 0x0001F000) >> 12) // 12
#define S_buildMin(sv)            ((sv & 0x00000FC0) >>  6) // 6
#define S_buildSec(sv)            ((sv & 0x0000003F) >>  0) // 0

#define A_axi_aclk_cntr           slv_reg03

#define A_userEvCodeB             slv_reg35










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
#define S_pdpRunCntr(sv)               ((sv >> 16) & 0xffff)
#define S_pdpErrCntr(sv)               ((sv >>  0) & 0xffff)

#define Mem_evCodeA          Mem1_mevCodeA

#define Mem_seqA_Config      Mem2_seqA_0A
#define Mem_seqA_TimeStamp   Mem4_seqA_1A

#define Mem_seqB_Config      Mem6_seqB_0A
#define Mem_seqB_TimeStamp   Mem8_seqB_1A

#define C_seq_option(sv)               ((sv & 0xffff) << 16)
#define C_seq_stop(sv)                 ((sv & 0x1) << 15)
#define C_seq_evCodeB(sv)              ((sv & 0x7ff) << 0)

#define Mem_evrDec           MemA_EVR_0










//==============================================================================
//----===@ define user register map Address
//==============================================================================
//  status check #1
#define A_gt_status               slv_reg04
#define S_gtp_soft_reset(sv)           ((sv >>  0) & 0x1)      // slv_reg04[0]      <= `DLY soft_reset_i           ;               
#define S_gt0_cpllfbclklost(sv)        ((sv >>  8) & 0x1)      // slv_reg04[8]      <= `DLY gt0_cpllfbclklost_i    ;               
#define S_gt0_cplllock(sv)             ((sv >>  9) & 0x1)      // slv_reg04[9]      <= `DLY gt0_cplllock_i         ;               
#define S_gt0_txresetdone(sv)          ((sv >> 12) & 0x1)      // slv_reg04[12]     <= `DLY gt0_txresetdone_i      ;               
#define S_gt0_txfsmresetdone(sv)       ((sv >> 13) & 0x1)      // slv_reg04[13]     <= `DLY gt0_txfsmresetdone_i   ;               
#define S_gt0_rxresetdone(sv)          ((sv >> 14) & 0x1)      // slv_reg04[14]     <= `DLY gt0_rxresetdone_i      ;               
#define S_gt0_rxfsmresetdone(sv)       ((sv >> 15) & 0x1)      // slv_reg04[15]     <= `DLY gt0_rxfsmresetdone_i   ;               
#define S_gt0_rxDataError(sv)          ((sv >> 16) & 0x1)      // slv_reg04[16]     <= `DLY gt0_rxDataError        ;               
#define S_gt0_trackOK(sv)              ((sv >> 18) & 0x1)      // slv_reg04[18]     <= `DLY gt0_trackOK       ;               
#define S_gt0_track_data(sv)           ((sv >> 19) & 0x1)      // slv_reg04[19]     <= `DLY gt0_track_data_i       ;               
#define S_gt1_cpllfbclklost(sv)        ((sv >> 20) & 0x1)      // slv_reg04[20]     <= `DLY gt1_cpllfbclklost_i    ;               
#define S_gt1_cplllock(sv)             ((sv >> 21) & 0x1)      // slv_reg04[21]     <= `DLY gt1_cplllock_i         ;               
#define S_gt1_txresetdone(sv)          ((sv >> 24) & 0x1)      // slv_reg04[24]     <= `DLY gt1_txresetdone_i      ;               
#define S_gt1_txfsmresetdone(sv)       ((sv >> 25) & 0x1)      // slv_reg04[25]     <= `DLY gt1_txfsmresetdone_i   ;               
#define S_gt1_rxresetdone(sv)          ((sv >> 26) & 0x1)      // slv_reg04[26]     <= `DLY gt1_rxresetdone_i      ;               
#define S_gt1_rxfsmresetdone(sv)       ((sv >> 27) & 0x1)      // slv_reg04[27]     <= `DLY gt1_rxfsmresetdone_i   ;               
#define S_gt1_rxDataError(sv)          ((sv >> 28) & 0x1)      // slv_reg04[28]     <= `DLY gt1_rxDataError        ;               
#define S_gt1_trackOK(sv)              ((sv >> 30) & 0x1)      // slv_reg04[30]     <= `DLY gt1_trackOK       ;               
#define S_gt1_track_data(sv)           ((sv >> 31) & 0x1)      // slv_reg04[31]     <= `DLY gt1_track_data_i       ;               

#define A_gt_trackLossCnt         slv_reg05
#define S_gt0_trackLossCnt(sv)         ((sv >>  0) & 0xffff)   // slv_reg05[31:16]  <= `DLY gt1_trackLossCnt       ;               
#define S_gt1_trackLossCnt(sv)         ((sv >> 16) & 0xffff)   // slv_reg05[15: 0]  <= `DLY gt0_trackLossCnt       ;                                                                                

#define A_gt0_clk_cntr            slv_reg06
#define S_gt0_rxClk_cntr(sv)            ((sv >>  0) & 0xffff)  // slv_reg06         <= `DLY {gt0_txClk_cntr,gt0_rxClk_cntr}       ;
#define S_gt0_txClk_cntr(sv)            ((sv >> 16) & 0xffff)  //

#define A_gt1_clk_cntr            slv_reg07
#define S_gt1_rxClk_cntr(sv)            ((sv >>  0) & 0xffff)  // slv_reg07         <= `DLY {gt1_txClk_cntr,gt1_rxClk_cntr}       ; 
#define S_gt1_txClk_cntr(sv)            ((sv >> 16) & 0xffff)  // 



//==============================================================================
#define A_gt_ctrl                 slv_reg10
#define C_gt0_TxUserSendEn(sv)          ((sv & 0x1) << 20)     // gt0_TxUserSendEn  <= `DLY slv_reg10[20];                                          
#define C_gt1_TxUserSendEn(sv)          ((sv & 0x1) << 16)     // gt1_TxUserSendEn  <= `DLY slv_reg10[16];                                          
#define C_gt_FrameSize(sv)              ((sv & 0xfff) << 0)    // gt_FrameSize      <= `DLY slv_reg10[11:0];      

#define A_gt_intr                 slv_reg11
#define C_intr(sv)                      ((sv & 0x1) << 31)     // intr              <= `DLY slv_reg11[31] & gt1_track_data_RE;      // default 32'h0

#define A_gt_reset                slv_reg12
#define C_soft_reset(sv)                ((sv & 0x1) << 31)     // soft_reset_i      <= `DLY slv_reg10[31];      // default 32'h0011_0128            




//==============================================================================
//  status check #1
#define A_ifanStt4_CntrFront      slv_reg04
#define S_ifanStt4_CntrFrontA(sv)               ((sv >> 16) & 0xffff)
#define S_ifanStt4_CntrFrontB(sv)               ((sv >>  0) & 0xffff)

#define A_ifanStt4_CntrRear       slv_reg05
#define S_ifanStt4_CntrRearA(sv)                ((sv >> 16) & 0xffff)
#define S_ifanStt4_CntrRearB(sv)                ((sv >>  0) & 0xffff)

#define A_emergencyStopCntr       slv_reg06
#define S_emergencyStopCntr(sv)                 ((sv >>  0) & 0xffff)            // slv_reg06[15: 0] <= `DLY emergencyStopCntr;                                                            

#define A_boardStat               slv_reg07
#define S_im_mF_in(sv)                          (((~sv) >> 31) & 0x1   )            // slv_reg07[31]    <= `DLY im_mF_in        ;                                                             
#define S_f_PRSNT_M2C_L(sv)                     (((~sv) >> 28) & 0x1)            // slv_reg07[28]    <= `DLY f_PRSNT_M2C_L   ;                                                             
#define S_f_SFP_Loss1(sv)                       ((sv >>  1) & 0x1 )            // slv_reg07[ 1: 0] <= `DLY f_SFP_Loss      ;                                                             
#define S_f_SFP_Loss0(sv)                       ((sv >>  0) & 0x1 )            // slv_reg07[ 1: 0] <= `DLY f_SFP_Loss      ;                                                             
// #define S_f_SFP_txFault1(sv)                    ((sv >>  5) & 0x1 )            // slv_reg07[ 5: 4] <= `DLY f_SFP_txFault   ;                                                             
// #define S_f_SFP_txFault0(sv)                    ((sv >>  4) & 0x1 )            // slv_reg07[ 5: 4] <= `DLY f_SFP_txFault   ;                                                             
// #define S_f_SFP_presentN1(sv)                   (((~sv) >>  9) & 0x1 )            // slv_reg07[ 9: 8] <= `DLY f_SFP_presentN  ;                                                             
// #define S_f_SFP_presentN0(sv)                   (((~sv) >>  8) & 0x1 )            // slv_reg07[ 9: 8] <= `DLY f_SFP_presentN  ;                                                             
#define S_f_SFP_presentN1(sv)                   (((~sv) >>  4) & 0x1 )            // slv_reg07[ 9: 8] <= `DLY f_SFP_presentN  ;                                                             
#define S_f_SFP_presentN0(sv)                   (((~sv) >>  5) & 0x1 )            // slv_reg07[ 9: 8] <= `DLY f_SFP_presentN  ;                                                             

#define A_seqTrg_runState         slv_reg08
#define S_seqTrgA_runState(sv)                  ((sv >>  0) & 0x3   )            // slv_reg08[1:0]   <= `DLY seqTrgA_runState;                                                             
#define S_seqTrgB_runState(sv)                  ((sv >>  4) & 0x3   )            // slv_reg08[5:4]   <= `DLY seqTrgB_runState;                                                             

#define A_pdpRunStt               slv_reg09

#define A_evrW_Time               slv_reg0a       //       <= `DLY {2'h0, evrW_h8[5:0], evrW_m8[5:0], evrW_s8[5:0], evrW_Hms4, evrW_Dms4, evrW_Oms4};  
#define S_evrW_TimeHour(sv)                     ((sv >> 24) & 0x03f  )            // {2'h0, evrW_h8[5:0], evrW_m8[5:0], evrW_s8[5:0], evrW_Hms4, evrW_Dms4, evrW_Oms4};  
#define S_evrW_TimeMin(sv)                      ((sv >> 18) & 0x03f  )            //                 24            18            12           8          4          0
#define S_evrW_TimeSec(sv)                      ((sv >> 12) & 0x03f  )
#define S_evrW_TimeMs(sv)                       (((sv >> 8) & 0xf)*100 + ((sv >> 4) & 0xf)*10 + ((sv >> 0) & 0xf))

#define A_evrW_Day                slv_reg0b       //       <= `DLY {16'h0, evrW_d16};  
#define S_evrW_Day(sv)                          ((sv >>  0) & 0xffff)            //       <= `DLY {16'h0, evrW_d16};  

#define A_intrReg                 slv_reg0c
#define S_intr_1sec(sv)                          ((sv >>  1) & 0x1 )
#define S_intr_evCodeA(sv)                       ((sv >>  0) & 0x1 )


//==============================================================================
#define A_resetCtrl               slv_reg10  
#define C_evgW_rst(sv)                          ((sv & 0x1) << 31)
#define C_evrW_rst(sv)                          ((sv & 0x1) << 30)
#define C_igen_rst(sv)                          ((sv & 0x1) << 29)
#define C_idec_rst(sv)                          ((sv & 0x1) << 28)
#define C_seqTrgA_resume(sv)                    ((sv & 0x1) << 2) 
#define C_seqTrgB_resume(sv)                    ((sv & 0x1) << 1) 
#define C_evgW_set_time(sv)                     ((sv & 0x1) << 0) 

#define A_m_st_sl                 slv_reg11  
#define A_m_st_sh                 slv_reg12  
#define C_m_st(sv)                           ((sv & 0xffff) << 0)             // m_st[15: 0]            <= `DLY slv_reg11[15:0];                                                        

#define A_tmpfCtrl                slv_reg13  
#define C_tmpf_usr_led(sv)                      ((sv & 0x3) << 4)                // tmpf_usr_led           <= `DLY slv_reg10[1:0];                                                         
#define C_tmpf_SFP_txDisable(sv)                ((sv & 0x3) << 2)                // tmpf_SFP_txDisable     <= `DLY slv_reg10[5:4];                                                         
#define C_tmpf_SFP_LinkStat(sv)                 ((sv & 0x3) << 0)                // tmpf_SFP_LinkStat      <= `DLY slv_reg10[9:8];                                                         

#define A_evgW_set_Time            slv_reg14  
#define C_evgW_set_h8(sv)                    ((sv & 0xff) << 16)              // sw_evgW_set_h8         <= `DLY slv_reg14[23:16];                                                       
#define C_evgW_set_m8(sv)                    ((sv & 0xff) << 8)               // sw_evgW_set_m8         <= `DLY slv_reg14[15: 8];                                                       
#define C_evgW_set_s8(sv)                    ((sv & 0xff) << 0)               // sw_evgW_set_s8         <= `DLY slv_reg14[ 7: 0];                                                       
//#define A_evgW_set_Day            slv_reg14  5
#define C_evgW_set_d16(sv)                   ((sv & 0xffff) << 0)             // sw_evgW_set_d16        <= `DLY slv_reg15[15: 0];                                                       

#define A_seqTrgMskA0             slv_reg16  
#define A_seqTrgMskA1             slv_reg17  
#define A_seqTrgMskB0             slv_reg18  
#define A_seqTrgMskB1             slv_reg19  

#define A_seqTrgCtrl              slv_reg1a  
#define C_seqTrgSelB(sv)                        ((sv & 0x7) << 12)               // seqTrgSelB             <= `DLY slv_reg1a[14:12];                                                       
#define C_seqTrgSelA(sv)                        ((sv & 0x3) << 4)                // seqTrgSelA             <= `DLY slv_reg1a[5:4];                                                         


#define A_txDBusSel_igenMode      slv_reg1b  
#define C_igen_decOff(sv)                       ((sv & 0x1) << 9)                // igen_mode1             <= `DLY slv_reg13[1];  // 0 : irigb decode mode,  1 : user time set mode        
#define C_igen_extOn(sv)                        ((sv & 0x1) << 8)                // igen_mode0             <= `DLY slv_reg13[0];  // 0 : internal irigb gen, 1 : external irigb            
#define C_txDBusSel0(sv)                         ((sv & 0x1) << 0)    
#define C_txDBusSel1(sv)                         ((sv & 0x1) << 1)    
#define C_txDBusSel2(sv)                         ((sv & 0x1) << 2)    
#define C_txDBusSel3(sv)                         ((sv & 0x1) << 3)    
#define C_txDBusSel4(sv)                         ((sv & 0x1) << 4)    
#define C_txDBusSel5(sv)                         ((sv & 0x1) << 5)    
#define C_txDBusSel6(sv)                         ((sv & 0x1) << 6)    
#define C_txDBusSel7(sv)                         ((sv & 0x1) << 7)    

#define A_evTrgCtrl               slv_reg1c  
#define C_evTrgSel(sv)                          ((sv & 0xffff) << 0)             // evTrgSel               <= `DLY slv_reg1c[15:0];                                                        

#define A_usrTrg                  slv_reg1e  
#define C_usrTrg(sv)                            ((sv & 0x1) << 0)                // usrTrg                 <= `DLY slv_reg1e[0];                                                           

#define A_mxc14_prescalerReg_00   slv_reg20  
#define A_mxc14_prescalerReg_01   slv_reg21  
#define A_mxc14_prescalerReg_02   slv_reg22  
#define A_mxc14_prescalerReg_03   slv_reg23  
#define A_mxc14_prescalerReg_04   slv_reg24  
#define A_mxc14_prescalerReg_05   slv_reg25  
#define A_mxc14_prescalerReg_06   slv_reg26  
#define A_mxc14_prescalerReg_07   slv_reg27  
#define A_mxc14_prescalerReg_08   slv_reg28  
#define A_mxc14_prescalerReg_09   slv_reg29  
#define A_mxc14_prescalerReg_10   slv_reg2a  
#define A_mxc14_prescalerReg_11   slv_reg2b  
#define A_mxc14_prescalerReg_12   slv_reg2c  
#define A_mxc14_prescalerReg_13   slv_reg2d  

#define A_usrTr_trgSrc01   A_usrTrg  
#define A_mxc00_trgSrc02   A_mxc14_prescalerReg_00
#define A_mxc01_trgSrc03   A_mxc14_prescalerReg_01
#define A_mxc02_trgSrc04   A_mxc14_prescalerReg_02
#define A_mxc03_trgSrc05   A_mxc14_prescalerReg_03
#define A_mxc04_trgSrc06   A_mxc14_prescalerReg_04
#define A_mxc05_trgSrc07   A_mxc14_prescalerReg_05
#define A_mxc06_trgSrc08   A_mxc14_prescalerReg_06
#define A_mxc07_trgSrc09   A_mxc14_prescalerReg_07
#define A_mxc08_trgSrc10   A_mxc14_prescalerReg_08
#define A_mxc09_trgSrc11   A_mxc14_prescalerReg_09
#define A_mxc10_trgSrc12   A_mxc14_prescalerReg_10
#define A_mxc11_trgSrc13   A_mxc14_prescalerReg_11
#define A_mxc12_trgSrc14   A_mxc14_prescalerReg_12
#define A_mxc13_trgSrc15   A_mxc14_prescalerReg_13

#define A_evgEnableMask           slv_reg2f
#define C_txEnable_DBus(sv)                     ((sv & 0x1) << 31)
#define C_txEnable_evCodeA(sv)                  ((sv & 0x1) << 30)
#define C_txEnable_evCodeB(sv)                  ((sv & 0x1) << 29)
#define C_txEnable_mevCodeA(sv)                 ((sv & 0x1) << 28)
#define C_seqTrgA_en(sv)                        ((sv & 0x1) << 27)
#define C_seqTrgB_en(sv)                        ((sv & 0x1) << 26)
#define C_mxc14Enable(sv)                       ((sv & 0x1) << 25)
#define C_evTrgMsk(sv)                          ((sv & 0xffff) << 0)             // evTrgMsk               <= `DLY slv_reg1c[31:16];                                                       


#define A_evr_RegUpdate           slv_reg40  
#define C_evr_setCtrlReg(sv)                    ((sv & 0x1) << 0)                // evr_setCtrlReg         <= `DLY slv_reg40[0];                                                           
#define C_evr_getStatReg(sv)                    ((sv & 0x1) << 4)                // evr_getStatReg         <= `DLY slv_reg40[4];                                                           

#define A_pdpPolarity             slv_reg41  

#define A_rxDBusSel_swapExOut     slv_reg42  
#define C_swapExOut(sv)                         ((sv & 0x1) << 31)               // swapExOut16            <= `DLY slv_reg42[31];                                                          
#define C_evr_pdpTrgSrcSel(sv)                  ((sv & 0x1) << 30)               // swapExOut16            <= `DLY slv_reg42[31];                                                          
#define C_rxDBusSel0(sv)                         ((sv & 0x1) << 0)    
#define C_rxDBusSel1(sv)                         ((sv & 0x1) << 1)    
#define C_rxDBusSel2(sv)                         ((sv & 0x1) << 2)    
#define C_rxDBusSel3(sv)                         ((sv & 0x1) << 3)    
#define C_rxDBusSel4(sv)                         ((sv & 0x1) << 4)    
#define C_rxDBusSel5(sv)                         ((sv & 0x1) << 5)    
#define C_rxDBusSel6(sv)                         ((sv & 0x1) << 6)    
#define C_rxDBusSel7(sv)                         ((sv & 0x1) << 7)    




#define A_intrMask                slv_reg43  
#define D_intr_evCodeA1           0x00000001
#define D_intr_evrAlarmSec        0x00000002

#define A_intrEnable              slv_reg44  //[0]
#define A_intrClear               slv_reg45  //[0]

#define A_evrEnableMask           slv_reg4f  
#define C_rxEnable_DBus(sv)                     ((sv & 0x1) << 31)
#define C_rxEnable_evCodeA(sv)                  ((sv & 0x1) << 30)
#define C_rxEnable_evCodeB(sv)                  ((sv & 0x1) << 29)
#define C_rxEnable_streamData(sv)               ((sv & 0x1) << 28)
#define C_pdpEnable(sv)                         ((sv & 0x1) << 1) //pdpEnable  <= `DLY slv_reg4f[1] | !emergencyStop;


#define D_trg00 (0)
#define D_trg01 (1)
#define D_trg02 (2)
#define D_trg03 (3)
#define D_trg04 (4)
#define D_trg05 (5)
#define D_trg06 (6)
#define D_trg07 (7)
#define D_trg08 (8)
#define D_trg09 (9)
#define D_trg10 (10)
#define D_trg11 (11)
#define D_trg12 (12)
#define D_trg13 (13)
#define D_trg14 (14)
#define D_trg15 (15)

#define D_trg01_usr   0x0002
#define D_trg02_mxc00 0x0004
#define D_trg03_mxc01 0x0008
#define D_trg04_mxc02 0x0010
#define D_trg05_mxc03 0x0020
#define D_trg06_mxc04 0x0040
#define D_trg07_mxc05 0x0080
#define D_trg08_mxc06 0x0100
#define D_trg09_mxc07 0x0200
#define D_trg10_mxc08 0x0400
#define D_trg11_mxc09 0x0800
#define D_trg12_mxc10 0x1000
#define D_trg13_mxc11 0x2000
#define D_trg14_mxc12 0x4000
#define D_trg15_mxc13 0x8000




#define D_txDBus0_USR (0x01)
#define D_txDBus1_MXC (0x02)
#define D_txDBus2_MXC (0x04)
#define D_txDBus3_MXC (0x08)
#define D_txDBus4_MXC (0x10)
#define D_txDBus5_MXC (0x20)
#define D_txDBus6_MXC (0x40)
#define D_txDBus7_MXC (0x80)

#define D_ExtOut0_pdpOut (0x01)
#define D_ExtOut1_pdpOut (0x02)
#define D_ExtOut2_pdpOut (0x04)
#define D_ExtOut3_pdpOut (0x08)
#define D_ExtOut4_pdpOut (0x10)
#define D_ExtOut5_pdpOut (0x20)
#define D_ExtOut6_pdpOut (0x40)
#define D_ExtOut7_pdpOut (0x80)

#define D_seqTableA 0
#define D_seqTableB 1

#define D_seqSrc_exIn0  0x00000001
#define D_seqSrc_exIn1  0x00000002
#define D_seqSrc_exIn2  0x00000004
#define D_seqSrc_exIn3  0x00000008
#define D_seqSrc_exIn4  0x00000010
#define D_seqSrc_exIn5  0x00000020
#define D_seqSrc_exIn6  0x00000040
#define D_seqSrc_exIn7  0x00000080
#define D_seqSrc_exIn8  0x00000100
#define D_seqSrc_exIn9  0x00000200
#define D_seqSrc_exIn10 0x00000400
#define D_seqSrc_exIn11 0x00000800
#define D_seqSrc_exIn12 0x00001000
#define D_seqSrc_exIn13 0x00002000
#define D_seqSrc_exIn14 0x00004000
#define D_seqSrc_exIn15 0x00008000
#define D_seqSrc_mxc0   0x00010000
#define D_seqSrc_mxc1   0x00020000
#define D_seqSrc_mxc2   0x00040000
#define D_seqSrc_mxc3   0x00080000
#define D_seqSrc_mxc4   0x00100000
#define D_seqSrc_mxc5   0x00200000
#define D_seqSrc_mxc6   0x00400000
#define D_seqSrc_mxc7   0x00800000
#define D_seqSrc_mxc8   0x01000000
#define D_seqSrc_mxc9   0x02000000
#define D_seqSrc_mxc10  0x04000000
#define D_seqSrc_mxc11  0x08000000
#define D_seqSrc_mxc12  0x10000000
#define D_seqSrc_mxc13  0x20000000
#define D_seqSrc_usr    0x40000000

#define D_pdpTrg0  0x00000001
#define D_pdpTrg1  0x00000002
#define D_pdpTrg2  0x00000004
#define D_pdpTrg3  0x00000008
#define D_pdpTrg4  0x00000010
#define D_pdpTrg5  0x00000020
#define D_pdpTrg6  0x00000040
#define D_pdpTrg7  0x00000080
#define D_pdpTrg8  0x00000100
#define D_pdpTrg9  0x00000200
#define D_pdpTrg10 0x00000400
#define D_pdpTrg11 0x00000800
#define D_pdpTrg12 0x00001000
#define D_pdpTrg13 0x00002000
#define D_pdpTrg14 0x00004000
#define D_pdpTrg15 0x00008000
#define D_pdpTrg16 0x00010000
#define D_pdpTrg17 0x00020000
#define D_pdpTrg18 0x00040000
#define D_pdpTrg19 0x00080000
#define D_pdpTrg20 0x00100000
#define D_pdpTrg21 0x00200000
#define D_pdpTrg22 0x00400000
#define D_pdpTrg23 0x00800000
#define D_pdpTrg24 0x01000000
#define D_pdpTrg25 0x02000000
#define D_pdpTrg26 0x04000000
#define D_pdpTrg27 0x08000000
#define D_pdpTrg28 0x10000000
#define D_pdpTrg29 0x20000000
#define D_pdpTrg30 0x40000000
#define D_pdpTrg31 0x80000000

#define D_seqSrc_OR0           1
#define D_seqSrc_OR1           2
#define D_seqSrc_OR0_AND_OR1   3

#define C_bit00(sv) ( (sv & 0x01) <<  0)
#define C_bit01(sv) ( (sv & 0x01) <<  1)
#define C_bit02(sv) ( (sv & 0x01) <<  2)
#define C_bit03(sv) ( (sv & 0x01) <<  3)
#define C_bit04(sv) ( (sv & 0x01) <<  4)
#define C_bit05(sv) ( (sv & 0x01) <<  5)
#define C_bit06(sv) ( (sv & 0x01) <<  6)
#define C_bit07(sv) ( (sv & 0x01) <<  7)
#define C_bit08(sv) ( (sv & 0x01) <<  8)
#define C_bit09(sv) ( (sv & 0x01) <<  9)
#define C_bit10(sv) ( (sv & 0x01) << 10)
#define C_bit11(sv) ( (sv & 0x01) << 11)
#define C_bit12(sv) ( (sv & 0x01) << 12)
#define C_bit13(sv) ( (sv & 0x01) << 13)
#define C_bit14(sv) ( (sv & 0x01) << 14)
#define C_bit15(sv) ( (sv & 0x01) << 15)
#define C_bit16(sv) ( (sv & 0x01) << 16)
#define C_bit17(sv) ( (sv & 0x01) << 17)
#define C_bit18(sv) ( (sv & 0x01) << 18)
#define C_bit19(sv) ( (sv & 0x01) << 19)
#define C_bit20(sv) ( (sv & 0x01) << 20)
#define C_bit21(sv) ( (sv & 0x01) << 21)
#define C_bit22(sv) ( (sv & 0x01) << 22)
#define C_bit23(sv) ( (sv & 0x01) << 23)
#define C_bit24(sv) ( (sv & 0x01) << 24)
#define C_bit25(sv) ( (sv & 0x01) << 25)
#define C_bit26(sv) ( (sv & 0x01) << 26)
#define C_bit27(sv) ( (sv & 0x01) << 27)
#define C_bit28(sv) ( (sv & 0x01) << 28)
#define C_bit29(sv) ( (sv & 0x01) << 29)
#define C_bit30(sv) ( (sv & 0x01) << 30)
#define C_bit31(sv) ( (sv & 0x01) << 31)


#define MAX_SEQ_NUM      2048
#define MAX_EVR_RAM_SIZE 2048
#define MAX_mxc_NUM      15
#define MAX_mevCodeA     16
#define MAX_PDP_NUM      32



//==============================================================================
//----===@ 
//==============================================================================
struct s_regRW
{
    unsigned int offset;     
    unsigned int val;
};





//==============================================================================
//----===@ get
//==============================================================================
struct s_evTime{
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int hour;
    unsigned int min;
    unsigned int sec;
    unsigned int ms;
};

struct s_ipInfo{
    unsigned int vendor;
    unsigned int ipID;
    unsigned int ipID_sub;
    unsigned int ipVer_major;
    unsigned int ipVer_miner;
    unsigned int ipVer_rev;
    unsigned int ipTick;
    struct s_evTime     buildTime;
};

struct s_pdpStat{
    unsigned char isRunning;
	unsigned int  runCnt;
	unsigned int  errCnt;
};

struct s_evStat{
    struct s_evTime      evrTime;
    unsigned int  isMaster;     
    unsigned int  PRSNT_M2C_L;
    unsigned int  f_SFP_LossA;
    unsigned int  f_SFP_LossB;
    unsigned int  f_SFP_prsntA;
    unsigned int  f_SFP_prsntB;
    unsigned int  fanA;
    unsigned int  fanB;
    unsigned int  fanC;
    unsigned int  fanD;
    unsigned int  emergencyStopCntr;
    unsigned int  seqA_isRunning;
    unsigned int  seqB_isRunning;
	struct s_pdpStat     pdp[MAX_PDP_NUM];
};

struct s_gtpStat{
    int gtp_soft_reset              ;
    int gt0_cpllfbclklost           ;
    int gt0_cplllock                ;
    int gt0_txresetdone             ;
    int gt0_txfsmresetdone          ;
    int gt0_rxresetdone             ;
    int gt0_rxfsmresetdone          ;
    int gt0_rxDataError             ;
    int gt0_track_data              ;
    int gt0_trackOK                 ;
    int gt0_trackLossCnt            ;
    int gt0_txClk_cntr              ;
    int gt0_rxClk_cntr              ;
    int gt1_cpllfbclklost           ;
    int gt1_cplllock                ;
    int gt1_txresetdone             ;
    int gt1_txfsmresetdone          ;
    int gt1_rxresetdone             ;
    int gt1_rxfsmresetdone          ;
    int gt1_rxDataError             ;
    int gt1_track_data              ;
    int gt1_trackOK                 ;
    int gt1_trackLossCnt            ;
    int gt1_txClk_cntr              ;
    int gt1_rxClk_cntr              ;
};

//==============================================================================
//----===@ set
//==============================================================================
struct s_evgEnable{
    unsigned int mxc;
    unsigned int evTrgMsk16;
    unsigned int DBus;
    unsigned int evCodeA;
    unsigned int evCodeB;
    unsigned int evCodeB_mevCodeA;
    unsigned int evCodeB_seqTrgA;
    unsigned int evCodeB_seqTrgB;
};

struct s_evrEnable{
    unsigned int streamData;
    unsigned int DBus;
    unsigned int evCodeA;
    unsigned int evCodeB;
    unsigned int pdpEnable;
};

//==============================================================================
//----===@ config
//==============================================================================
struct s_pdpConfig{
	unsigned int delay  ;
	unsigned int width ;
} ;

struct s_seqTable{
	unsigned int stopbit  ;
	unsigned int evOption ;
	unsigned int evCodeB  ;
	unsigned int timeStamp;
} ;

struct s_seqMask{
    unsigned int A_OR0;
    unsigned int A_OR1;
    unsigned int A_sel;
    unsigned int B_OR0;
    unsigned int B_OR1;
    unsigned int B_sel;
} ;


struct s_evConfig{
    struct s_evTime    evgTime;
    unsigned int       mxcFreq[MAX_mxc_NUM];
    unsigned int       igen_decOff;
    unsigned int       igen_extOn;

    unsigned int       evTrgSrcSel[16]; // 1: mxc , 0: ext[15:1]

    unsigned int       txDbusSrcSelect[8];

    unsigned int       swapExOut;
    unsigned int       rxDbusSrcSelect[8];
    unsigned int       pdpTrgSrcSel;

    unsigned int       mevCodeA[MAX_mevCodeA];

    unsigned int       pdpPolarity[MAX_PDP_NUM];
    struct s_pdpConfig pdp[MAX_PDP_NUM];
    unsigned int       evMappingRam[MAX_EVR_RAM_SIZE];
    unsigned int       slio;
    unsigned int       shio;

    struct s_seqTable  seqTableA[MAX_SEQ_NUM];
    struct s_seqTable  seqTableB[MAX_SEQ_NUM];
    struct s_seqMask   seqMask;

};


//==============================================================================
//----===@ system 
//==============================================================================
struct s_ts2ev{
	int                fd     ;	
	struct s_ipInfo    info   ;	
    struct s_evStat    stat   ;

	struct s_evgEnable enEvg  ;
	struct s_evrEnable enEvr  ;

    struct s_evConfig  cfg  ;
};

struct s_ts2gtp{
	int                fd     ;	
	struct s_ipInfo    info   ;	
    struct s_gtpStat   stat   ;
};








#define MAGIC_NUM 0xDB

#define IOCTL_R           _IOWR (MAGIC_NUM, 1, struct s_regRW)
#define IOCTL_W           _IOWR (MAGIC_NUM, 2, struct s_regRW)






#ifdef __cplusplus
}
#endif
#endif
/*==============================================================================
                                  END OF FILE
==============================================================================*/
