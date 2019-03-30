///////////////////////////////////////////////////////////////////////////////
// (c) Copyright 2008 Xilinx, Inc. All rights reserved.
//
// This file contains confidential and proprietary information
// of Xilinx, Inc. and is protected under U.S. and
// international copyright and other intellectual property
// laws.
//
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// Xilinx, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) Xilinx shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or Xilinx had been advised of the
// possibility of the same.
//
// CRITICAL APPLICATIONS
// Xilinx products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of Xilinx products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
//
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
//
//
////////////////////////////////////////////////////////////////////////////////
`timescale 1ns / 1ps
`define DLY #1

//***************************** Entity Declaration ****************************

module base_zynq_design_aurora_8b10b_1_3_multi_gt #
(
    // Simulation attributes
    parameter   WRAPPER_SIM_GTRESET_SPEEDUP    =   "FALSE",     // Set to "true" to speed up sim reset
    parameter   RX_DFE_KL_CFG2_IN              =   32'h301148AC,
    parameter   PMA_RSV_IN                     =   32'h00018480
)
(
    //____________________________COMMON PORTS_______________________________{
   input                     gt_qpllclk_quad1_i,
   input                     gt_qpllrefclk_quad1_i,
    //____________________________COMMON PORTS_______________________________}


    //_________________________________________________________________________
    //_________________________________________________________________________
    //GT0 
    //____________________________CHANNEL PORTS________________________________
    //-------------- Channel - Dynamic Reconfiguration Port (DRP) --------------
    input   [8:0]   gt0_drpaddr_in,
    input           gt0_drpclk_in,
    input   [15:0]  gt0_drpdi_in,
    output  [15:0]  gt0_drpdo_out,
    input           gt0_drpen_in,
    output          gt0_drprdy_out,
    input           gt0_drpwe_in,
    //----------------------- Channel - Ref Clock Ports ------------------------
    input           gt0_gtrefclk0_in,
    //------------------------------ Channel PLL -------------------------------
    output          gt0_cpllfbclklost_out,
    output          gt0_cplllock_out,
    input           gt0_cplllockdetclk_in,
    output          gt0_cpllrefclklost_out,
    input           gt0_cpllreset_in,
    //----------------------------- Eye Scan Ports -----------------------------
    input           gt0_eyescantrigger_in,
    output          gt0_eyescandataerror_out,
    //---------------------- Loopback and Powerdown Ports ----------------------
    input   [2:0]   gt0_loopback_in,
    input   [1:0]   gt0_rxpd_in,
    input   [1:0]   gt0_txpd_in,
    //----------------------------- Receive Ports ------------------------------
    input           gt0_eyescanreset_in,
    input           gt0_rxuserrdy_in,
    //--------------------- Receive Ports - 8b10b Decoder ----------------------
    output  [1:0]   gt0_rxchariscomma_out,
    output  [1:0]   gt0_rxcharisk_out,
    output  [1:0]   gt0_rxdisperr_out,
    output  [1:0]   gt0_rxnotintable_out,
    //----------------- Receive Ports - Pattern Checker Ports ------------------
    output          gt0_rxprbserr_out,
    input   [2:0]   gt0_rxprbssel_in,
    //----------------- Receive Ports - Pattern Checker ports ------------------
    input           gt0_rxprbscntreset_in,
    //----------------- Receive Ports - Clock Correction Ports -----------------
    output  [1:0]   gt0_rxclkcorcnt_out,
    //------------- Receive Ports - Comma Detection and Alignment --------------
    output          gt0_rxbyteisaligned_out,
    output          gt0_rxbyterealign_out,
    output          gt0_rxcommadet_out,
    input           gt0_rxmcommaalignen_in,
    input           gt0_rxpcommaalignen_in,
    //--------------------- Receive Ports - RX Equalizer Ports -----------------
    input           gt0_rxdfeagchold_in,
    input           gt0_rxdfelfhold_in,
    input           gt0_rxdfelpmreset_in,
    output  [6:0]   gt0_rxmonitorout_out,
    input   [1:0]   gt0_rxmonitorsel_in,
    //----------------- Receive Ports - RX Data Path interface -----------------
    input           gt0_gtrxreset_in,
    input           gt0_rxpcsreset_in,
    input           gt0_rxpmareset_in,
    output  [15:0]  gt0_rxdata_out,
    output          gt0_rxoutclk_out,
    input           gt0_rxusrclk_in,
    input           gt0_rxusrclk2_in,
    //----- Receive Ports - RX Driver,OOB signalling,Coupling and Eq.,CDR ------
    input           gt0_gtxrxn_in,
    input           gt0_gtxrxp_in,
    output          gt0_rxcdrlock_out,
    output  [7:0]   gt0_dmonitorout_out,
    //------ Receive Ports - RX Elastic Buffer and Phase Alignment Ports -------
    input           gt0_rxbufreset_in,
    output  [2:0]   gt0_rxbufstatus_out,
    //---------------------- Receive Ports - RX PLL Ports ----------------------
    output          gt0_rxresetdone_out,
    //---------------------- TX Configurable Driver Ports ----------------------
    input   [4:0]   gt0_txpostcursor_in,
    input   [4:0]   gt0_txprecursor_in,
    //--------------- Receive Ports - RX Margin Analysis Ports ----------------
    input           gt0_rxlpmen_in,
    input           gt0_rxcdrovrden_in,
    input           gt0_rxcdrhold_in,
    //--------------- Receive Ports - RX Polarity Control Ports ----------------
    input           gt0_rxpolarity_in,
    //----------------------------- Transmit Ports -----------------------------
    input           gt0_txuserrdy_in,
    //-------------- Transmit Ports - 8b10b Encoder Control Ports --------------
    input   [1:0]   gt0_txchardispmode_in,
    input   [1:0]   gt0_txchardispval_in,
    input   [1:0]   gt0_txcharisk_in,
    //---------- Transmit Ports - TX Buffer and Phase Alignment Ports ----------
    output  [1:0]   gt0_txbufstatus_out,
    //---------------- Transmit Ports - TX Data Path interface -----------------
    input           gt0_gttxreset_in,
    input   [15:0]  gt0_txdata_in,
    output          gt0_txoutclk_out,
    output          gt0_txoutclkfabric_out,
    output          gt0_txoutclkpcs_out,
    input           gt0_txusrclk_in,
    input           gt0_txusrclk2_in,
    //------------------- Transmit Ports - PCI Express Ports -------------------
    input           gt0_txelecidle_in,
    //---------------- Transmit Ports - Pattern Generator Ports ----------------
    input           gt0_txprbsforceerr_in,
    //-------------- Transmit Ports - TX Driver and OOB signaling --------------
    output          gt0_gtxtxn_out,
    output          gt0_gtxtxp_out,
    input   [3:0]   gt0_txdiffctrl_in,
    input   [6:0]   gt0_txmaincursor_in,
    //--------------- Transmit Ports - TX Polarity Control Ports ---------------
    input           gt0_txpolarity_in,
    //---------------- Transmit Ports - pattern Generator Ports ----------------
    input   [2:0]   gt0_txprbssel_in,
    //--------------------- Transmit Ports - TX PLL Ports ----------------------
    input           gt0_txpcsreset_in,
    input           gt0_txinhibit_in,
    input           gt0_txpmareset_in,
    output          gt0_txresetdone_out



);

   
        
//********************************* Main Body of Code**************************



//------------------------- GT Instances  -------------------------------
    //_________________________________________________________________________
    //_________________________________________________________________________
    //GT0

    base_zynq_design_aurora_8b10b_1_3_gt #
    (
        // Simulation attributes
        .GT_SIM_GTRESET_SPEEDUP   (WRAPPER_SIM_GTRESET_SPEEDUP),
        .RX_DFE_KL_CFG2_IN        (RX_DFE_KL_CFG2_IN),
        .PCS_RSVD_ATTR_IN         (48'h000000000000),
        .PMA_RSV_IN               (PMA_RSV_IN)
    )
    gt0_base_zynq_design_aurora_8b10b_1_3_i
    (
        //-------------------------------- Channel ---------------------------------
        .qpllclk_in                     (gt_qpllclk_quad1_i),
        .qpllrefclk_in                  (gt_qpllrefclk_quad1_i),
        //-------------- Channel - Dynamic Reconfiguration Port (DRP) --------------
        .drpaddr_in                     (gt0_drpaddr_in),
        .drpclk_in                      (gt0_drpclk_in),
        .drpdi_in                       (gt0_drpdi_in),
        .drpdo_out                      (gt0_drpdo_out),
        .drpen_in                       (gt0_drpen_in),
        .drprdy_out                     (gt0_drprdy_out),
        .drpwe_in                       (gt0_drpwe_in),
        //----------------------- Channel - Ref Clock Ports ------------------------
        .gtrefclk0_in                   (gt0_gtrefclk0_in),
        //------------------------------ Channel PLL -------------------------------
        .cpllfbclklost_out              (gt0_cpllfbclklost_out),
        .cplllock_out                   (gt0_cplllock_out),
        .cplllockdetclk_in              (gt0_cplllockdetclk_in),
        .cpllrefclklost_out             (gt0_cpllrefclklost_out),
        .cpllreset_in                   (gt0_cpllreset_in),
        //---------------------- Loopback and Powerdown Ports ----------------------
        .loopback_in                    (gt0_loopback_in),
        .rxpd_in                        (gt0_rxpd_in),
        .txpd_in                        (gt0_txpd_in),
        //----------------------------- Receive Ports ------------------------------
        .eyescanreset_in                (gt0_eyescanreset_in),
        .rxuserrdy_in                   (gt0_rxuserrdy_in),
        //------------------------ RX Margin Analysis Ports ------------------------
        .eyescandataerror_out           (gt0_eyescandataerror_out),
        .eyescantrigger_in              (gt0_eyescantrigger_in),
        //--------------------- Receive Ports - 8b10b Decoder ----------------------
        .rxchariscomma_out              (gt0_rxchariscomma_out),
        .rxcharisk_out                  (gt0_rxcharisk_out),
        .rxdisperr_out                  (gt0_rxdisperr_out),
        .rxnotintable_out               (gt0_rxnotintable_out),
        //----------------- Receive Ports - Pattern Checker Ports ------------------
        .rxprbserr_out                  (gt0_rxprbserr_out),
        .rxprbssel_in                   (gt0_rxprbssel_in),
        //----------------- Receive Ports - Pattern Checker ports ------------------
        .rxprbscntreset_in              (gt0_rxprbscntreset_in),
        //----------------- Receive Ports - Clock Correction Ports -----------------
        .rxclkcorcnt_out                (gt0_rxclkcorcnt_out),
        //------------- Receive Ports - Comma Detection and Alignment --------------
        .rxbyteisaligned_out            (gt0_rxbyteisaligned_out),
        .rxbyterealign_out              (gt0_rxbyterealign_out),
        .rxcommadet_out                 (gt0_rxcommadet_out),
        .rxmcommaalignen_in             (gt0_rxmcommaalignen_in),
        .rxpcommaalignen_in             (gt0_rxpcommaalignen_in),
        //--------------------- Receive Ports - RX Equalizer Ports -----------------
        .rxdfeagchold_in                (gt0_rxdfeagchold_in),
        .rxdfelfhold_in                 (gt0_rxdfelfhold_in),
        .rxdfelpmreset_in               (gt0_rxdfelpmreset_in),
        .rxmonitorout_out               (gt0_rxmonitorout_out),
        .rxmonitorsel_in                (gt0_rxmonitorsel_in),
        //----------------- Receive Ports - RX Data Path interface -----------------
        .gtrxreset_in                   (gt0_gtrxreset_in),
        .rxpcsreset_in                  (gt0_rxpcsreset_in),
        .rxpmareset_in                  (gt0_rxpmareset_in),
        .rxdata_out                     (gt0_rxdata_out),
        .rxoutclk_out                   (gt0_rxoutclk_out),
        .rxusrclk_in                    (gt0_rxusrclk_in),
        .rxusrclk2_in                   (gt0_rxusrclk2_in),
        //----- Receive Ports - RX Driver,OOB signalling,Coupling and Eq.,CDR ------
        .gtxrxn_in                      (gt0_gtxrxn_in),
        .gtxrxp_in                      (gt0_gtxrxp_in),
        .rxcdrlock_out                  (gt0_rxcdrlock_out),
        .dmonitorout_out                (gt0_dmonitorout_out),
        //------ Receive Ports - RX Elastic Buffer and Phase Alignment Ports -------
        .rxbufreset_in                  (gt0_rxbufreset_in),
        .rxbufstatus_out                (gt0_rxbufstatus_out),
        //---------------------- Receive Ports - RX PLL Ports ----------------------
        .rxresetdone_out                (gt0_rxresetdone_out),
        //---------------------- TX Configurable Driver Ports ----------------------
        .txpostcursor_in                (gt0_txpostcursor_in),
        .txprecursor_in                 (gt0_txprecursor_in),
        //------------------ Receive Ports - RX Margin Analysis ports ----------------
        .rxlpmen_in                     (gt0_rxlpmen_in),
        .rxcdrovrden_in                 (gt0_rxcdrovrden_in),
        .rxcdrhold_in                   (gt0_rxcdrhold_in),
        //--------------- Receive Ports - RX Polarity Control Ports ----------------
        .rxpolarity_in                  (gt0_rxpolarity_in),
        //----------------------------- Transmit Ports -----------------------------
        .txuserrdy_in                   (gt0_txuserrdy_in),
        //-------------- Transmit Ports - 8b10b Encoder Control Ports --------------
        .txchardispmode_in              (gt0_txchardispmode_in),
        .txchardispval_in               (gt0_txchardispval_in),
        .txcharisk_in                   (gt0_txcharisk_in),
        //---------- Transmit Ports - TX Buffer and Phase Alignment Ports ----------
        .txbufstatus_out                (gt0_txbufstatus_out),
        //---------------- Transmit Ports - TX Data Path interface -----------------
        .gttxreset_in                   (gt0_gttxreset_in),
        .txdata_in                      (gt0_txdata_in),
        .txoutclk_out                   (gt0_txoutclk_out),
        .txoutclkfabric_out             (gt0_txoutclkfabric_out),
        .txoutclkpcs_out                (gt0_txoutclkpcs_out),
        .txusrclk_in                    (gt0_txusrclk_in),
        .txusrclk2_in                   (gt0_txusrclk2_in),
        //------------------- Transmit Ports - PCI Express Ports -------------------
        .txelecidle_in                  (gt0_txelecidle_in),
        //---------------- Transmit Ports - Pattern Generator Ports ----------------
        .txprbsforceerr_in              (gt0_txprbsforceerr_in),
        //-------------- Transmit Ports - TX Driver and OOB signaling --------------
        .gtxtxn_out                     (gt0_gtxtxn_out),
        .gtxtxp_out                     (gt0_gtxtxp_out),
        .txdiffctrl_in                  (gt0_txdiffctrl_in),
        .txmaincursor_in                (gt0_txmaincursor_in),
        //--------------------- Transmit Ports - TX PLL Ports ----------------------
        .txpcsreset_in                  (gt0_txpcsreset_in),
        .txinhibit_in                   (gt0_txinhibit_in),
        .txpmareset_in                  (gt0_txpmareset_in),
        .txresetdone_out                (gt0_txresetdone_out),
        //---------------- Transmit Ports - pattern Generator Ports ----------------
        .txprbssel_in                   (gt0_txprbssel_in),
        //--------------- Transmit Ports - TX Polarity Control Ports ---------------
        .txpolarity_in                  (gt0_txpolarity_in)

    );




endmodule

  
