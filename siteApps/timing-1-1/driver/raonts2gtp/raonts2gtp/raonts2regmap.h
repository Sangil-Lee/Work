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
#if !defined( __raonts2regmap_h__ )
#define __raonts2regmap_h__

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
#define A_axi_aclk_freq           slv_reg02
#define A_axi_aclk_cntr           slv_reg03



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
#define MemC_EVR_CS   0xC000          //ctrl & status




//==============================================================================
//----===@ define user register map Address
//==============================================================================
//  status check #1
#define A_gt_status               slv_reg04
#define A_gt_trackLossCnt         slv_reg05
#define A_gt0_clk_cntr            slv_reg06
#define A_gt1_clk_cntr            slv_reg07

//==============================================================================
#define A_gt_ctrl                 slv_reg10
#define A_gt_intr                 slv_reg11


//==============================================================================
//----===@ define user register data macro
//==============================================================================
// status bit (bit Position)
#define S_gtp_soft_reset(sv)           ((sv >>  0) & 0x1)      // slv_reg04[0]      <= `DLY soft_reset_i           ;               
#define S_gt0_cpllfbclklost(sv)        ((sv >>  8) & 0x1)      // slv_reg04[8]      <= `DLY gt0_cpllfbclklost_i    ;               
#define S_gt0_cplllock(sv)             ((sv >>  9) & 0x1)      // slv_reg04[9]      <= `DLY gt0_cplllock_i         ;               
#define S_gt0_txresetdone(sv)          ((sv >> 12) & 0x1)      // slv_reg04[12]     <= `DLY gt0_txresetdone_i      ;               
#define S_gt0_txfsmresetdone(sv)       ((sv >> 13) & 0x1)      // slv_reg04[13]     <= `DLY gt0_txfsmresetdone_i   ;               
#define S_gt0_rxresetdone(sv)          ((sv >> 14) & 0x1)      // slv_reg04[14]     <= `DLY gt0_rxresetdone_i      ;               
#define S_gt0_rxfsmresetdone(sv)       ((sv >> 15) & 0x1)      // slv_reg04[15]     <= `DLY gt0_rxfsmresetdone_i   ;               
#define S_gt0_rxDataError(sv)          ((sv >> 16) & 0x1)      // slv_reg04[16]     <= `DLY gt0_rxDataError        ;               
#define S_gt0_track_data(sv)           ((sv >> 19) & 0x1)      // slv_reg04[19]     <= `DLY gt0_track_data_i       ;               
#define S_gt1_cpllfbclklost(sv)        ((sv >> 20) & 0x1)      // slv_reg04[20]     <= `DLY gt1_cpllfbclklost_i    ;               
#define S_gt1_cplllock(sv)             ((sv >> 21) & 0x1)      // slv_reg04[21]     <= `DLY gt1_cplllock_i         ;               
#define S_gt1_txresetdone(sv)          ((sv >> 24) & 0x1)      // slv_reg04[24]     <= `DLY gt1_txresetdone_i      ;               
#define S_gt1_txfsmresetdone(sv)       ((sv >> 25) & 0x1)      // slv_reg04[25]     <= `DLY gt1_txfsmresetdone_i   ;               
#define S_gt1_rxresetdone(sv)          ((sv >> 26) & 0x1)      // slv_reg04[26]     <= `DLY gt1_rxresetdone_i      ;               
#define S_gt1_rxfsmresetdone(sv)       ((sv >> 27) & 0x1)      // slv_reg04[27]     <= `DLY gt1_rxfsmresetdone_i   ;               
#define S_gt1_rxDataError(sv)          ((sv >> 28) & 0x1)      // slv_reg04[28]     <= `DLY gt1_rxDataError        ;               
#define S_gt1_track_data(sv)           ((sv >> 31) & 0x1)      // slv_reg04[31]     <= `DLY gt1_track_data_i       ;               

#define S_gt0_trackLossCnt(sv)         ((sv >>  0) & 0xffff)   // slv_reg05[31:16]  <= `DLY gt1_trackLossCnt       ;               
#define S_gt1_trackLossCnt(sv)         ((sv >> 16) & 0xffff)   // slv_reg05[15: 0]  <= `DLY gt0_trackLossCnt       ;                                                                                

#define S_gt0_rxClk_cntr(sv)            ((sv >>  0) & 0xffff)  // slv_reg06         <= `DLY {gt0_txClk_cntr,gt0_rxClk_cntr}       ;
#define S_gt0_txClk_cntr(sv)            ((sv >> 16) & 0xffff)  //

#define S_gt1_rxClk_cntr(sv)            ((sv >>  0) & 0xffff)  // slv_reg07         <= `DLY {gt1_txClk_cntr,gt1_rxClk_cntr}       ; 
#define S_gt1_txClk_cntr(sv)            ((sv >> 16) & 0xffff)  // 

// ctrl bit(bitPosition)                                                         
#define C_soft_reset(sv)                ((sv & 0x1) << 31)     // soft_reset_i      <= `DLY slv_reg10[31];      // default 32'h0011_0128            
#define C_gt0_TxUserSendEn(sv)          ((sv & 0x1) << 20)     // gt0_TxUserSendEn  <= `DLY slv_reg10[20];                                          
#define C_gt1_TxUserSendEn(sv)          ((sv & 0x1) << 16)     // gt1_TxUserSendEn  <= `DLY slv_reg10[16];                                          
#define C_gt_FrameSize(sv)              ((sv & 0xfff) << 0)    // gt_FrameSize      <= `DLY slv_reg10[11:0];      
                                  
#define C_intr(sv)                      ((sv & 0x1) << 31)     // intr              <= `DLY slv_reg11[31] & gt1_track_data_RE;      // default 32'h0

// Mask bit (bit Position)
// #define M_                 0x80000000













//==============================================================================
//  status check #1
#define A_ifanStt4_CntrFront      slv_reg04
#define A_ifanStt4_CntrRear       slv_reg05
#define A_emergencyStopCntr       slv_reg06
#define A_boardStat               slv_reg07
#define A_seqTrg_runState         slv_reg08
#define A_pdpRunStt               slv_reg09
#define A_evrW_Time               slv_reg0a       //       <= `DLY {2'h0, evrW_h8[5:0], evrW_m8[5:0], evrW_s8[5:0], evrW_Hms4, evrW_Dms4, evrW_Oms4};  
#define A_evrW_Day                slv_reg0b       //       <= `DLY {16'h0, evrW_d16};  


//==============================================================================
#define A_resetCtrl               slv_reg10  
#define A_m_st_sl                 slv_reg11  
#define A_m_st_sh                 slv_reg12  

#define A_tmpfCtrl                slv_reg13  

#define A_sw_evgW_Time            slv_reg14  
#define A_sw_evgW_Day             slv_reg15  

#define A_seqTrgMskA0             slv_reg16  
#define A_seqTrgMskA1             slv_reg17  
#define A_seqTrgMskB0             slv_reg18  
#define A_seqTrgMskB1             slv_reg19  

#define A_seqTrgCtrl              slv_reg1a  

#define A_igenMOde_txDBusSel      slv_reg1b  

#define A_evTrgCtrl               slv_reg1c  

#define A_usrTrg                  slv_reg1e  

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
#define A_mxc14_reset             slv_reg2f

#define A_evr_RegUpdate           slv_reg40  
#define A_pdpPolarity             slv_reg41  
#define A_exOutCtrl               slv_reg42  
#define A_intrCtrl                slv_reg43  




//==============================================================================
//----===@ define user register data macro
//==============================================================================

// status bit (bit Position)
#define S_ifanStt4_CntrFrontA(sv)               ((sv >> 16) & 0xffff)
#define S_ifanStt4_CntrFrontB(sv)               ((sv >>  0) & 0xffff)
#define S_ifanStt4_CntrRearA(sv)                ((sv >> 16) & 0xffff)
#define S_ifanStt4_CntrRearB(sv)                ((sv >>  0) & 0xffff)

#define S_emergencyStopCntr(sv)                 ((sv >>  0) & 0xffff)            // slv_reg06[15: 0] <= `DLY emergencyStopCntr;                                                            

#define S_im_mF_in(sv)                          ((sv >> 31) & 0x1   )            // slv_reg07[31]    <= `DLY im_mF_in        ;                                                             
#define S_f_PRSNT_M2C_L(sv)                     ((sv >> 28) & 0x1   )            // slv_reg07[28]    <= `DLY f_PRSNT_M2C_L   ;                                                             
#define S_f_SFP_Loss(sv)                        ((sv >>  0) & 0x3   )            // slv_reg07[ 1: 0] <= `DLY f_SFP_Loss      ;                                                             
#define S_f_SFP_txFault(sv)                     ((sv >>  4) & 0x3   )            // slv_reg07[ 5: 4] <= `DLY f_SFP_txFault   ;                                                             
#define S_f_SFP_presentN(sv)                    ((sv >>  8) & 0x3   )            // slv_reg07[ 9: 8] <= `DLY f_SFP_presentN  ;                                                             

#define S_seqTrgA_runState(sv)                  ((sv >>  0) & 0x3   )            // slv_reg08[1:0]   <= `DLY seqTrgA_runState;                                                             
#define S_seqTrgB_runState(sv)                  ((sv >>  4) & 0x3   )            // slv_reg08[5:4]   <= `DLY seqTrgB_runState;                                                             

#define S_evrW_TimeHour(sv)                     ((sv >> 26) & 0x01f  )            // {2'h0, evrW_h8[5:0], evrW_m8[5:0], evrW_s8[5:0], evrW_Hms4, evrW_Dms4, evrW_Oms4};  
#define S_evrW_TimeMin(sv)                      ((sv >> 19) & 0x03f  )            //                 24            18            12           8          4          0
#define S_evrW_TimeSec(sv)                      ((sv >> 12) & 0x03f  )
#define S_evrW_TimeMs(sv)                       (((sv >> 8) & 0xf)*100 + ((sv >> 4) & 0xf)*10 + ((sv >> 0) & 0xf))

#define S_evrW_Day(sv)                          ((sv >>  0) & 0xffff)            //       <= `DLY {16'h0, evrW_d16};  


// ctrl bit(bitPosition)                                                         
#define C_evgW_rst(sv)                          ((sv & 0x1) << 31)                // tmpf_usr_led           <= `DLY slv_reg10[1:0];                                                         
#define C_evrW_rst(sv)                          ((sv & 0x1) << 30)                // tmpf_usr_led           <= `DLY slv_reg10[1:0];                                                         
#define C_igen_rst(sv)                          ((sv & 0x1) << 29)                // tmpf_usr_led           <= `DLY slv_reg10[1:0];                                                         
#define C_idec_rst(sv)                          ((sv & 0x1) << 28)                // tmpf_usr_led           <= `DLY slv_reg10[1:0];                                                         
#define C_pdpReset(sv)                          ((sv & 0x1) << 27)                // tmpf_usr_led           <= `DLY slv_reg10[1:0];                                                         
#define C_sw_evgW_set_time0(sv)                 ((sv & 0x1) << 0)                // tmpf_usr_led           <= `DLY slv_reg10[1:0]; 

#define C_m_st_sl(sv)                           ((sv & 0xffff) << 0)             // m_st[15: 0]            <= `DLY slv_reg11[15:0];                                                        
#define C_m_st_sh(sv)                           ((sv & 0xffff) << 0)             // m_st[31:16]            <= `DLY slv_reg12[15:0];                                                        

#define C_tmpf_usr_led(sv)                      ((sv & 0x3) << 4)                // tmpf_usr_led           <= `DLY slv_reg10[1:0];                                                         
#define C_tmpf_SFP_txDisable(sv)                ((sv & 0x3) << 2)                // tmpf_SFP_txDisable     <= `DLY slv_reg10[5:4];                                                         
#define C_tmpf_SFP_LinkStat(sv)                 ((sv & 0x3) << 0)                // tmpf_SFP_LinkStat      <= `DLY slv_reg10[9:8];                                                         

#define C_sw_evgW_set_h8(sv)                    ((sv & 0xff) << 16)              // sw_evgW_set_h8         <= `DLY slv_reg14[23:16];                                                       
#define C_sw_evgW_set_m8(sv)                    ((sv & 0xff) << 8)               // sw_evgW_set_m8         <= `DLY slv_reg14[15: 8];                                                       
#define C_sw_evgW_set_s8(sv)                    ((sv & 0xff) << 0)               // sw_evgW_set_s8         <= `DLY slv_reg14[ 7: 0];                                                       

#define C_sw_evgW_set_d16(sv)                   ((sv & 0xffff) << 0)             // sw_evgW_set_d16        <= `DLY slv_reg15[15: 0];                                                       

#define C_seqTrgMskA0(sv)                       ((sv & 0x7fffffff) << 0)         // seqTrgMskA0            <= `DLY slv_reg16[30:0];                                                        
#define C_seqTrgMskA1(sv)                       ((sv & 0x7fffffff) << 0)         // seqTrgMskA1            <= `DLY slv_reg17[30:0];                                                        
#define C_seqTrgMskB0(sv)                       ((sv & 0x7fffffff) << 0)         // seqTrgMskB0            <= `DLY slv_reg18[30:0];                                                        
#define C_seqTrgMskB1(sv)                       ((sv & 0x7fffffff) << 0)         // seqTrgMskB1            <= `DLY slv_reg19[30:0];                                                        

#define C_seqTrgSelB(sv)                        ((sv & 0x7) << 12)               // seqTrgSelB             <= `DLY slv_reg1a[14:12];                                                       
#define C_seqTrgB_resume(sv)                    ((sv & 0x1) << 9)                // seqTrgB_resume         <= `DLY slv_reg1a[9];                                                           
#define C_seqTrgB_en(sv)                        ((sv & 0x1) << 8)                // seqTrgB_en             <= `DLY slv_reg1a[8];                                                           
#define C_seqTrgSelA(sv)                        ((sv & 0x3) << 4)                // seqTrgSelA             <= `DLY slv_reg1a[5:4];                                                         
#define C_seqTrgA_resume(sv)                    ((sv & 0x1) << 1)                // seqTrgA_resume         <= `DLY slv_reg1a[1];                                                           
#define C_seqTrgA_en(sv)                        ((sv & 0x1) << 0)                // seqTrgA_en             <= `DLY slv_reg1a[0];                                                           

#define C_igen_mode1(sv)                        ((sv & 0x1) << 9)                // igen_mode1             <= `DLY slv_reg13[1];  // 0 : irigb decode mode,  1 : user time set mode        
#define C_igen_mode0(sv)                        ((sv & 0x1) << 8)                // igen_mode0             <= `DLY slv_reg13[0];  // 0 : internal irigb gen, 1 : external irigb            
#define C_tx_DBusSel8(sv)                       ((sv & 0xff) << 0)    

#define C_evTrgMsk(sv)                          ((sv & 0xffff) << 16)            // evTrgMsk               <= `DLY slv_reg1c[31:16];                                                       
#define C_evTrgSel(sv)                          ((sv & 0xffff) << 0)             // evTrgSel               <= `DLY slv_reg1c[15:0];                                                        

#define C_usrTrg(sv)                            ((sv & 0x1) << 0)                // usrTrg                 <= `DLY slv_reg1e[0];                                                           

#define C_mxc14_reset(sv)                       ((sv & 0x1) << 0)                // mxc14_reset            <= `DLY slv_reg1d[0];                                                           

#define C_evr_setCtrlReg(sv)                    ((sv & 0x1) << 0)                // evr_setCtrlReg         <= `DLY slv_reg40[0];                                                           
#define C_evr_getStatReg(sv)                    ((sv & 0x1) << 4)                // evr_getStatReg         <= `DLY slv_reg40[4];                                                           
#define C_rx_DBusSel8(sv)                       ((sv & 0xffff) << 0)             // rx_DBusSel8            <= `DLY slv_reg42[7:0];                                                         
#define C_swapExOut16(sv)                       ((sv & 0x1) << 31)               // swapExOut16            <= `DLY slv_reg42[31];                                                          
#define C_intrMask(sv)                          ((sv & 0x1) << 31)               // intrMask               <= `DLY slv_reg43[31];                                                          
#define C_intrClear(sv)                         ((sv & 0x1) << 0)                // intrClear              <= `DLY slv_reg43[0];                                                           



//==============================================================================
//----===@ 
//==============================================================================
struct st_regRW
{
    unsigned int offset;     
    unsigned int val;
};

struct st_evTime
{
    unsigned int day;     
    unsigned int hour;
    unsigned int min;
    unsigned int sec;
    unsigned int ms;
};

typedef struct 
{
    unsigned int im_mF_in;     
    unsigned int f_PRSNT_M2C_L;
    unsigned int f_SFP_Loss;
    unsigned int f_SFP_txFault;
    unsigned int f_SFP_presentN;
}st_A_boardStat;


struct st_ts2gtp_stat
{
  int gtp_soft_reset              ;

  int gt0_cpllfbclklost           ;
  int gt0_cplllock                ;

  int gt0_txresetdone             ;
  int gt0_txfsmresetdone          ;
  int gt0_rxresetdone             ;
  int gt0_rxfsmresetdone          ;
  
  int gt0_rxDataError             ;
  int gt0_track_data              ;
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
  int gt1_trackLossCnt            ;

  int gt1_txClk_cntr              ;
  int gt1_rxClk_cntr              ;
};

#define MAGIC_NUM 0xDB

#define IOCTL_R           _IOWR (MAGIC_NUM, 1, struct st_regRW)
#define IOCTL_W           _IOWR (MAGIC_NUM, 2, struct st_regRW)

#define IOCTL_ts2gtp_stat _IOWR (MAGIC_NUM, 3, struct st_ts2gtp_stat)






#ifdef __cplusplus
}
#endif
#endif
/*==============================================================================
                                  END OF FILE
==============================================================================*/
