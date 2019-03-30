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
(* core_generation_info = "base_zynq_design_aurora_8b10b_1_2,aurora_8b10b_v11_0_4,{user_interface=AXI_4_Streaming,backchannel_mode=Sidebands,c_aurora_lanes=1,c_column_used=left,c_gt_clock_1=GTXQ0,c_gt_clock_2=None,c_gt_loc_1=X,c_gt_loc_10=X,c_gt_loc_11=X,c_gt_loc_12=X,c_gt_loc_13=X,c_gt_loc_14=X,c_gt_loc_15=X,c_gt_loc_16=X,c_gt_loc_17=X,c_gt_loc_18=X,c_gt_loc_19=X,c_gt_loc_2=X,c_gt_loc_20=X,c_gt_loc_21=X,c_gt_loc_22=X,c_gt_loc_23=X,c_gt_loc_24=X,c_gt_loc_25=X,c_gt_loc_26=X,c_gt_loc_27=X,c_gt_loc_28=X,c_gt_loc_29=X,c_gt_loc_3=X,c_gt_loc_30=X,c_gt_loc_31=X,c_gt_loc_32=X,c_gt_loc_33=X,c_gt_loc_34=X,c_gt_loc_35=X,c_gt_loc_36=X,c_gt_loc_37=X,c_gt_loc_38=X,c_gt_loc_39=X,c_gt_loc_4=1,c_gt_loc_40=X,c_gt_loc_41=X,c_gt_loc_42=X,c_gt_loc_43=X,c_gt_loc_44=X,c_gt_loc_45=X,c_gt_loc_46=X,c_gt_loc_47=X,c_gt_loc_48=X,c_gt_loc_5=X,c_gt_loc_6=X,c_gt_loc_7=X,c_gt_loc_8=X,c_gt_loc_9=X,c_lane_width=2,c_line_rate=62500,c_nfc=false,c_nfc_mode=IMM,c_refclk_frequency=125000,c_simplex=false,c_simplex_mode=TX,c_stream=false,c_ufc=false,flow_mode=None,interface_mode=Framing,dataflow_config=Duplex}" *)

//***************************** Entity Declaration ****************************

module base_zynq_design_aurora_8b10b_1_2_gt #
(
    // Simulation attributes
    parameter   GT_SIM_GTRESET_SPEEDUP   =   "FALSE",       // Set to 1 to speed up sim reset;
    parameter   RX_DFE_KL_CFG2_IN        =   32'h301148AC,
    parameter   PMA_RSV_IN               =   32'h00018480,
    parameter   PCS_RSVD_ATTR_IN         =   48'h000000000000
)
(
    //-------------------------------- Channel ---------------------------------
    input           qpllclk_in,
    input           qpllrefclk_in,
    //-------------- Channel - Dynamic Reconfiguration Port (DRP) --------------
    input   [8:0]   drpaddr_in,
    input           drpclk_in,
    input   [15:0]  drpdi_in,
    output  [15:0]  drpdo_out,
    input           drpen_in,
    output          drprdy_out,
    input           drpwe_in,
    //----------------------- Channel - Ref Clock Ports ------------------------
    input           gtrefclk0_in,
    //------------------------------ Channel PLL -------------------------------
    output          cpllfbclklost_out,
    output          cplllock_out,
    input           cplllockdetclk_in,
    output          cpllrefclklost_out,
    input           cpllreset_in,
    //----------------------------- Eye Scan Ports -----------------------------
    output          eyescandataerror_out,
    input           eyescantrigger_in,
    //---------------------- Loopback and Powerdown Ports ----------------------
    input   [2:0]   loopback_in,
    input   [1:0]   rxpd_in,
    input   [1:0]   txpd_in,
    //----------------------------- Receive Ports ------------------------------
    input           eyescanreset_in,
    input           rxuserrdy_in,
    //--------------------- Receive Ports - 8b10b Decoder ----------------------
    output  [1:0]   rxchariscomma_out,
    output  [1:0]   rxcharisk_out,
    output  [1:0]   rxdisperr_out,
    output  [1:0]   rxnotintable_out,
    //----------------- Receive Ports - Pattern Checker Ports ------------------
    output          rxprbserr_out,
    input   [2:0]   rxprbssel_in,
    //----------------- Receive Ports - Pattern Checker ports ------------------
    input           rxprbscntreset_in,
    //----------------- Receive Ports - Clock Correction Ports -----------------
    output  [1:0]   rxclkcorcnt_out,
    //------------- Receive Ports - Comma Detection and Alignment --------------
    output          rxbyteisaligned_out,
    output          rxbyterealign_out,
    output          rxcommadet_out,
    input           rxmcommaalignen_in,
    input           rxpcommaalignen_in,
    //--------------------- Receive Ports - RX Equalizer Ports -----------------
    input           rxdfeagchold_in,
    input           rxdfelfhold_in,
    input           rxdfelpmreset_in,
    output  [6:0]   rxmonitorout_out,
    input   [1:0]   rxmonitorsel_in,
    //----------------- Receive Ports - RX Data Path interface -----------------
    input           gtrxreset_in,
    input           rxpcsreset_in,
    input           rxpmareset_in,
    output  [15:0]  rxdata_out,
    output          rxoutclk_out,
    input           rxusrclk_in,
    input           rxusrclk2_in,
    //----- Receive Ports - RX Driver,OOB signalling,Coupling and Eq.,CDR ------
    input           gtxrxn_in,
    input           gtxrxp_in,
    output          rxcdrlock_out,
    output  [7:0]   dmonitorout_out,
    //------ Receive Ports - RX Elastic Buffer and Phase Alignment Ports -------
    input           rxbufreset_in,
    output  [2:0]   rxbufstatus_out,
    //---------------------- Receive Ports - RX PLL Ports ----------------------
    output          rxresetdone_out,
    //---------------------- TX Configurable Driver Ports ----------------------
    input   [4:0]   txpostcursor_in,
    input   [4:0]   txprecursor_in,
    //------------------ Receive Ports - RX Margin Analysis ports ----------------
    input           rxlpmen_in,
    input           rxcdrovrden_in,
    input           rxcdrhold_in,
    //--------------- Receive Ports - RX Polarity Control Ports ----------------
    input           rxpolarity_in,
    //----------------------------- Transmit Ports -----------------------------
    input           txuserrdy_in,
    //-------------- Transmit Ports - 8b10b Encoder Control Ports --------------
    input   [1:0]   txcharisk_in,
    input   [1:0]   txchardispmode_in,
    input   [1:0]   txchardispval_in,
    //---------- Transmit Ports - TX Buffer and Phase Alignment Ports ----------
    output  [1:0]   txbufstatus_out,
    //---------------- Transmit Ports - TX Data Path interface -----------------
    input           gttxreset_in,
    input   [15:0]  txdata_in,
    output          txoutclk_out,
    output          txoutclkfabric_out,
    output          txoutclkpcs_out,
    input           txusrclk_in,
    input           txusrclk2_in,
    //------------------- Transmit Ports - PCI Express Ports -------------------
    input           txelecidle_in,
    //---------------- Transmit Ports - Pattern Generator Ports ----------------
    input           txprbsforceerr_in,
    //-------------- Transmit Ports - TX Driver and OOB signaling --------------
    output          gtxtxn_out,
    output          gtxtxp_out,
    input   [3:0]   txdiffctrl_in,
    input   [6:0]   txmaincursor_in,
    //--------------------- Transmit Ports - TX PLL Ports ----------------------
    input           txpcsreset_in,
    input           txinhibit_in,
    input           txpmareset_in,
    output          txresetdone_out,
    //---------------- Transmit Ports - pattern Generator Ports ----------------
    input   [2:0]   txprbssel_in,
    //--------------- Transmit Ports - TX Polarity Control Ports ---------------
    input           txpolarity_in


);


//***************************** Wire Declarations *****************************

    // ground and vcc signals
    wire            tied_to_ground_i;
    wire    [63:0]  tied_to_ground_vec_i;
    wire            tied_to_vcc_i;
    wire    [63:0]  tied_to_vcc_vec_i;


    //RX Datapath signals
    wire    [63:0]  rxdata_i;
    wire    [5:0]   rxchariscomma_float_i;
    wire    [5:0]   rxcharisk_float_i;
    wire    [5:0]   rxdisperr_float_i;
    wire    [5:0]   rxnotintable_float_i;

    //TX Datapath signals
    wire    [63:0]  txdata_i;         


(* equivalent_register_removal="no" *) reg [95:0]   cpllpd_wait    =  96'hFFFFFFFFFFFFFFFFFFFFFFFF;
(* equivalent_register_removal="no" *) reg [127:0]  cpllreset_wait = 128'h000000000000000000000000000000FF;
  wire    cpllpd_ovrd_i ;
  wire    cpllreset_ovrd_i ;
  wire    cpll_reset_i;
  wire    cpllreset_sync;
  wire    cpllpd_sync;
  wire    cpll_pd_i;
  wire    ack_i;
  reg     flag = 1'b0;
  reg     flag2 = 1'b0;
  reg     ack_flag = 1'b0;
  // Internal Signals
  wire data_sync1;
  wire data_sync2;
  wire data_sync3;
  wire data_sync4;
  wire data_sync5;
  wire data_sync6;
  wire ack_sync1;
  wire ack_sync2;
  wire ack_sync3;
  wire ack_sync4;
  wire ack_sync5;
  wire ack_sync6;


//
//********************************* Main Body of Code**************************
                     
    //-------------------------  Static signal Assigments --------------------- 

    assign tied_to_ground_i             = 1'b0;
    assign tied_to_ground_vec_i         = 64'h0000000000000000;
    assign tied_to_vcc_i                = 1'b1;
    assign tied_to_vcc_vec_i            = 64'hffffffffffffffff;
  
    //-------------------  GT Datapath byte mapping  -----------------
    assign  rxdata_out    =   rxdata_i[15:0];


    assign  txdata_i    =   {tied_to_ground_vec_i[47:0],txdata_in};




    //------------------------- GT Instantiations  --------------------------
        GTXE2_CHANNEL #
        (
            //_______________________ Simulation-Only Attributes __________________
  
            .SIM_RECEIVER_DETECT_PASS   ("TRUE"),
            .SIM_TX_EIDLE_DRIVE_LEVEL   ("X"),
            .SIM_RESET_SPEEDUP          (GT_SIM_GTRESET_SPEEDUP),
            .SIM_CPLLREFCLK_SEL         (3'b001),
            .SIM_VERSION                ("4.0"),
          

           //----------------RX Byte and Word Alignment Attributes---------------
            .ALIGN_COMMA_DOUBLE                     ("FALSE"),
            .ALIGN_COMMA_ENABLE                     (10'b1111111111),
            .ALIGN_COMMA_WORD                       (2),
            .ALIGN_MCOMMA_DET                       ("TRUE"),
            .ALIGN_MCOMMA_VALUE                     (10'b1010000011),
            .ALIGN_PCOMMA_DET                       ("TRUE"),
            .ALIGN_PCOMMA_VALUE                     (10'b0101111100),
            .SHOW_REALIGN_COMMA                     ("TRUE"),
            .RXSLIDE_AUTO_WAIT                      (7),
            .RXSLIDE_MODE                           ("OFF"),
            .RX_SIG_VALID_DLY                       (10),

           //----------------RX 8B/10B Decoder Attributes---------------
            .RX_DISPERR_SEQ_MATCH                   ("TRUE"),
            .DEC_MCOMMA_DETECT                      ("TRUE"),
            .DEC_PCOMMA_DETECT                      ("TRUE"),
            .DEC_VALID_COMMA_ONLY                   ("FALSE"),

           //----------------------RX Clock Correction Attributes----------------------
            .CBCC_DATA_SOURCE_SEL                   ("DECODED"),
            .CLK_COR_SEQ_2_USE                      ("FALSE"),
            .CLK_COR_KEEP_IDLE                      ("FALSE"),
            .CLK_COR_MAX_LAT                        (17),
            .CLK_COR_MIN_LAT                        (12),
            .CLK_COR_PRECEDENCE                     ("TRUE"),
            .CLK_COR_REPEAT_WAIT                    (0),
            .CLK_COR_SEQ_LEN                        (2),
            .CLK_COR_SEQ_1_ENABLE                   (4'b1111),
            .CLK_COR_SEQ_1_1                        (10'b0111110111),
            .CLK_COR_SEQ_1_2                        (10'b0111110111),
            .CLK_COR_SEQ_1_3                        (10'b0100000000),
            .CLK_COR_SEQ_1_4                        (10'b0100000000),
            .CLK_CORRECT_USE                        ("TRUE"),
            .CLK_COR_SEQ_2_ENABLE                   (4'b1111),
            .CLK_COR_SEQ_2_1                        (10'b0100000000),
            .CLK_COR_SEQ_2_2                        (10'b0100000000),
            .CLK_COR_SEQ_2_3                        (10'b0100000000),
            .CLK_COR_SEQ_2_4                        (10'b0100000000),

           //----------------------RX Channel Bonding Attributes----------------------
            .CHAN_BOND_KEEP_ALIGN                   ("FALSE"),
            .CHAN_BOND_MAX_SKEW                     (7),
            .CHAN_BOND_SEQ_LEN                      (1),
            .CHAN_BOND_SEQ_1_1                      (10'b0101111100),
            .CHAN_BOND_SEQ_1_2                      (10'b0000000000),
            .CHAN_BOND_SEQ_1_3                      (10'b0000000000),
            .CHAN_BOND_SEQ_1_4                      (10'b0000000000),
            .CHAN_BOND_SEQ_1_ENABLE                 (4'b0001),
            .CHAN_BOND_SEQ_2_1                      (10'b0000000000),
            .CHAN_BOND_SEQ_2_2                      (10'b0000000000),
            .CHAN_BOND_SEQ_2_3                      (10'b0000000000),
            .CHAN_BOND_SEQ_2_4                      (10'b0000000000),
            .CHAN_BOND_SEQ_2_ENABLE                 (4'b0000),
            .CHAN_BOND_SEQ_2_USE                    ("FALSE"),
            .FTS_DESKEW_SEQ_ENABLE                  (4'b1111),
            .FTS_LANE_DESKEW_CFG                    (4'b1111),
            .FTS_LANE_DESKEW_EN                     ("FALSE"),

           //-------------------------RX Margin Analysis Attributes----------------------------
            .ES_CONTROL                             (6'b000000),
            .ES_ERRDET_EN                           ("FALSE"),
            .ES_EYE_SCAN_EN                         ("TRUE"),
            .ES_HORZ_OFFSET                         (12'h000),
            .ES_PMA_CFG                             (10'b0000000000),
            .ES_PRESCALE                            (5'b00000),
            .ES_QUALIFIER                           (80'h00000000000000000000),
            .ES_QUAL_MASK                           (80'h00000000000000000000),
            .ES_SDATA_MASK                          (80'h00000000000000000000),
            .ES_VERT_OFFSET                         (9'b000000000),

           //-----------------------FPGA RX Interface Attributes-------------------------
            .RX_DATA_WIDTH                          (20),

           //-------------------------PMA Attributes----------------------------
            .OUTREFCLK_SEL_INV                      (2'b11),
            .PMA_RSV                                (PMA_RSV_IN),
            .PMA_RSV2                               (16'h2050),
            .PMA_RSV3                               (2'b00),
            .PMA_RSV4                               (32'h00000000),
            .RX_BIAS_CFG                            (12'b000000000100),
            .DMONITOR_CFG                           (24'h000A00),
            .RX_CM_SEL                              (2'b11),
            .RX_CM_TRIM                             (3'b010),
            .RX_DEBUG_CFG                           (12'b000000000000),
            .RX_OS_CFG                              (13'b0000010000000),
            .TERM_RCAL_CFG                          (5'b10000),
            .TERM_RCAL_OVRD                         (1'b0),
            .TST_RSV                                (32'h00000000),
            .RX_CLK25_DIV                           (5),
            .TX_CLK25_DIV                           (5),
            .UCODEER_CLR                            (1'b0),

           //-------------------------PCI Express Attributes----------------------------
            .PCS_PCIE_EN                            ("FALSE"),

           //-------------------------PCS Attributes----------------------------
            .PCS_RSVD_ATTR                          (PCS_RSVD_ATTR_IN),

           //-----------RX Buffer Attributes------------
            .RXBUF_ADDR_MODE                        ("FULL"),
            .RXBUF_EIDLE_HI_CNT                     (4'b1000),
            .RXBUF_EIDLE_LO_CNT                     (4'b0000),
            .RXBUF_EN                               ("TRUE"),
            .RX_BUFFER_CFG                          (6'b000000),
            .RXBUF_RESET_ON_CB_CHANGE               ("TRUE"),
            .RXBUF_RESET_ON_COMMAALIGN              ("FALSE"),
            .RXBUF_RESET_ON_EIDLE                   ("FALSE"),
            .RXBUF_RESET_ON_RATE_CHANGE             ("TRUE"),
            .RXBUFRESET_TIME                        (5'b00001),
            .RXBUF_THRESH_OVFLW                     (61),
            .RXBUF_THRESH_OVRD                      ("FALSE"),
            .RXBUF_THRESH_UNDFLW                    (4),
            .RXDLY_CFG                              (16'h001F),
            .RXDLY_LCFG                             (9'h030),
            .RXDLY_TAP_CFG                          (16'h0000),
            .RXPH_CFG                               (24'h000000),
            .RXPHDLY_CFG                            (24'h084020),
            .RXPH_MONITOR_SEL                       (5'b00000),
            .RX_XCLK_SEL                            ("RXREC"),
            .RX_DDI_SEL                             (6'b000000),
            .RX_DEFER_RESET_BUF_EN                  ("TRUE"),

           //---------------------CDR Attributes-------------------------
            .RXCDR_CFG                              (72'h03000023FF20400020),
            .RXCDR_FR_RESET_ON_EIDLE                (1'b0),
            .RXCDR_HOLD_DURING_EIDLE                (1'b0),
            .RXCDR_PH_RESET_ON_EIDLE                (1'b0),
            .RXCDR_LOCK_CFG                         (6'b010101),

           //-----------------RX Initialization and Reset Attributes-------------------
            .RXCDRFREQRESET_TIME                    (5'b00001),
            .RXCDRPHRESET_TIME                      (5'b00001),
            .RXISCANRESET_TIME                      (5'b00001),
            .RXPCSRESET_TIME                        (5'b00001),
            .RXPMARESET_TIME                        (5'b00011),

           //-----------------RX OOB Signaling Attributes-------------------
            .RXOOB_CFG                              (7'b0000110),

           //-----------------------RX Gearbox Attributes---------------------------
            .RXGEARBOX_EN                           ("FALSE"),
            .GEARBOX_MODE                           (3'b000),

           //-----------------------PRBS Detection Attribute-----------------------
            .RXPRBS_ERR_LOOPBACK                    (1'b0),

           //-----------Power-Down Attributes----------
            .PD_TRANS_TIME_FROM_P2                  (12'h03c),
            .PD_TRANS_TIME_NONE_P2                  (8'h19),
            .PD_TRANS_TIME_TO_P2                    (8'h64),

           //-----------RX OOB Signaling Attributes----------
            .SAS_MAX_COM                            (64),
            .SAS_MIN_COM                            (36),
            .SATA_BURST_SEQ_LEN                     (4'b0101),
            .SATA_BURST_VAL                         (3'b100),
            .SATA_EIDLE_VAL                         (3'b100),
            .SATA_MAX_BURST                         (8),
            .SATA_MAX_INIT                          (21),
            .SATA_MAX_WAKE                          (7),
            .SATA_MIN_BURST                         (4),
            .SATA_MIN_INIT                          (12),
            .SATA_MIN_WAKE                          (4),

           //-----------RX Fabric Clock Output Control Attributes----------
            .TRANS_TIME_RATE                        (8'h0E),

           //------------TX Buffer Attributes----------------
            .TXBUF_EN                               ("TRUE"),
            .TXBUF_RESET_ON_RATE_CHANGE             ("TRUE"),
            .TXDLY_CFG                              (16'h001F),
            .TXDLY_LCFG                             (9'h030),
            .TXDLY_TAP_CFG                          (16'h0000),
            .TXPH_CFG                               (16'h0780),
            .TXPHDLY_CFG                            (24'h084020),
            .TXPH_MONITOR_SEL                       (5'b00000),
            .TX_XCLK_SEL                            ("TXOUT"),

           //-----------------------FPGA TX Interface Attributes-------------------------
            .TX_DATA_WIDTH                          (20),

           //-----------------------TX Configurable Driver Attributes-------------------------
            .TX_DEEMPH0                             (5'b00000),
            .TX_DEEMPH1                             (5'b00000),
            .TX_EIDLE_ASSERT_DELAY                  (3'b110),
            .TX_EIDLE_DEASSERT_DELAY                (3'b100),
            .TX_LOOPBACK_DRIVE_HIZ                  ("FALSE"),
            .TX_MAINCURSOR_SEL                      (1'b0),
            .TX_DRIVE_MODE                          ("DIRECT"),
            .TX_MARGIN_FULL_0                       (7'b1001110),
            .TX_MARGIN_FULL_1                       (7'b1001001),
            .TX_MARGIN_FULL_2                       (7'b1000101),
            .TX_MARGIN_FULL_3                       (7'b1000010),
            .TX_MARGIN_FULL_4                       (7'b1000000),
            .TX_MARGIN_LOW_0                        (7'b1000110),
            .TX_MARGIN_LOW_1                        (7'b1000100),
            .TX_MARGIN_LOW_2                        (7'b1000010),
            .TX_MARGIN_LOW_3                        (7'b1000000),
            .TX_MARGIN_LOW_4                        (7'b1000000),

           //-----------------------TX Gearbox Attributes--------------------------
            .TXGEARBOX_EN                           ("FALSE"),

           //-----------------------TX Initialization and Reset Attributes--------------------------
            .TXPCSRESET_TIME                        (5'b00001),
            .TXPMARESET_TIME                        (5'b00001),

           //-----------------------TX Receiver Detection Attributes--------------------------
            .TX_RXDETECT_CFG                        (14'h1832),
            .TX_RXDETECT_REF                        (3'b100),

           //--------------------------CPLL Attributes----------------------------
            .CPLL_CFG                               (24'hBC07DC),
            .CPLL_FBDIV                             (5),
            .CPLL_FBDIV_45                          (5),
            .CPLL_INIT_CFG                          (24'h00001E),
            .CPLL_LOCK_CFG                          (16'h01E8),
            .CPLL_REFCLK_DIV                        (1),
            .RXOUT_DIV                              (1),
            .TXOUT_DIV                              (1),
            .SATA_CPLL_CFG                          ("VCO_3000MHZ"),

           //------------RX Initialization and Reset Attributes-------------
            .RXDFELPMRESET_TIME                     (7'b0001111),

           //------------RX Equalizer Attributes-------------
            .RXLPM_HF_CFG                           (14'b00000011110000),
            .RXLPM_LF_CFG                           (14'b00000011110000),
            .RX_DFE_GAIN_CFG                        (23'h020FEA),
            .RX_DFE_H2_CFG                          (12'b000000000000),
            .RX_DFE_H3_CFG                          (12'b000001000000),
            .RX_DFE_H4_CFG                          (11'b00011110000),
            .RX_DFE_H5_CFG                          (11'b00011100000),
            .RX_DFE_KL_CFG                          (13'b0000011111110),
            .RX_DFE_LPM_CFG                         (16'h0954),
            .RX_DFE_LPM_HOLD_DURING_EIDLE           (1'b0),
            .RX_DFE_UT_CFG                          (17'b10001111000000000),
            .RX_DFE_VP_CFG                          (17'b00011111100000011),

           //-----------------------Power-Down Attributes-------------------------
            .RX_CLKMUX_PD                           (1'b1),
            .TX_CLKMUX_PD                           (1'b1),

           //-----------------------FPGA RX Interface Attribute-------------------------
            .RX_INT_DATAWIDTH                       (0),

           //-----------------------FPGA TX Interface Attribute-------------------------
            .TX_INT_DATAWIDTH                       (0),

           //----------------TX Configurable Driver Attributes---------------
            .TX_QPI_STATUS_EN                       (1'b0),

           //-----------------------RX Equalizer Attributes--------------------------
            .RX_DFE_KL_CFG2                         (RX_DFE_KL_CFG2_IN),
            .RX_DFE_XYD_CFG                         (13'b0000000000000),

           //-----------------------TX Configurable Driver Attributes--------------------------
            .TX_PREDRIVER_MODE                      (1'b0)

         
        )
        gtxe2_i
        (
     
        //-------------------------------- Channel ---------------------------------
        .CFGRESET                       (tied_to_ground_i),
        .CLKRSVD                        (4'b0000),
        .DMONITOROUT                    (dmonitorout_out),
        .GTRESETSEL                     (tied_to_ground_i),
        .GTRSVD                         (16'b0000000000000000),
        .QPLLCLK                        (qpllclk_in),
        .QPLLREFCLK                     (qpllrefclk_in),
        .RESETOVRD                      (tied_to_ground_i),
        //-------------- Channel - Dynamic Reconfiguration Port (DRP) --------------
        .DRPADDR                        (drpaddr_in),
        .DRPCLK                         (drpclk_in),
        .DRPDI                          (drpdi_in),
        .DRPDO                          (drpdo_out),
        .DRPEN                          (drpen_in),
        .DRPRDY                         (drprdy_out),
        .DRPWE                          (drpwe_in),
        //----------------------- Channel - Ref Clock Ports ------------------------
        .GTGREFCLK                      (tied_to_ground_i),
        .GTNORTHREFCLK0                 (tied_to_ground_i),
        .GTNORTHREFCLK1                 (tied_to_ground_i),
        .GTREFCLK0                      (gtrefclk0_in),
        .GTREFCLK1                      (tied_to_ground_i),
        .GTREFCLKMONITOR                (),
        .GTSOUTHREFCLK0                 (tied_to_ground_i),
        .GTSOUTHREFCLK1                 (tied_to_ground_i),
        //------------------------------ Channel PLL -------------------------------
        .CPLLFBCLKLOST                  (cpllfbclklost_out),
        .CPLLLOCK                       (cplllock_out),
        .CPLLLOCKDETCLK                 (cplllockdetclk_in),
        .CPLLLOCKEN                     (tied_to_vcc_i),
        .CPLLPD                         (cpll_pd_i),
        .CPLLREFCLKLOST                 (cpllrefclklost_out),
        .CPLLREFCLKSEL                  (3'b001),
        .CPLLRESET                      (cpll_reset_i),
        //----------------------------- Eye Scan Ports -----------------------------
        .EYESCANDATAERROR               (eyescandataerror_out),
        .EYESCANMODE                    (tied_to_ground_i),
        .EYESCANRESET                   (eyescanreset_in),
        .EYESCANTRIGGER                 (eyescantrigger_in),
        //---------------------- Loopback and Powerdown Ports ----------------------
        .LOOPBACK                       (loopback_in),
        .RXPD                           (rxpd_in),
        .TXPD                           (txpd_in),
        //--------------------------- PCS Reserved Ports ---------------------------
        .PCSRSVDIN                      (16'b0000000000000000),
        .PCSRSVDIN2                     (5'b00000),
        .PCSRSVDOUT                     (),
        //--------------------------- PMA Reserved Ports ---------------------------
        .PMARSVDIN                      (5'b00000),
        .PMARSVDIN2                     (5'b00000),
        //----------------------------- Receive Ports ------------------------------
        .RXQPIEN                        (tied_to_ground_i),
        .RXQPISENN                      (),
        .RXQPISENP                      (),
        .RXSYSCLKSEL                    (2'b00),
        .RXUSERRDY                      (rxuserrdy_in),
        //------------ Receive Ports - 64b66b and 64b67b Gearbox Ports -------------
        .RXDATAVALID                    (),
        .RXGEARBOXSLIP                  (tied_to_ground_i),
        .RXHEADER                       (),
        .RXHEADERVALID                  (),
        .RXSTARTOFSEQ                   (),
        //--------------------- Receive Ports - 8b10b Decoder ----------------------
        .RX8B10BEN                      (tied_to_vcc_i),
        .RXCHARISCOMMA                  ({rxchariscomma_float_i,rxchariscomma_out}),
        .RXCHARISK                      ({rxcharisk_float_i,rxcharisk_out}),
        .RXDISPERR                      ({rxdisperr_float_i,rxdisperr_out}),
        .RXNOTINTABLE                   ({rxnotintable_float_i,rxnotintable_out}),
        //----------------- Receive Ports - Channel Bonding Ports ------------------
        .RXCHANBONDSEQ                  (),
        .RXCHBONDEN                     (tied_to_ground_i),
        .RXCHBONDI                      (5'b00000),
        .RXCHBONDLEVEL                  (tied_to_ground_vec_i[2:0]),
        .RXCHBONDMASTER                 (tied_to_ground_i),
        .RXCHBONDO                      (),
        .RXCHBONDSLAVE                  (tied_to_ground_i),
        //----------------- Receive Ports - Channel Bonding Ports  -----------------
        .RXCHANISALIGNED                (),
        .RXCHANREALIGN                  (),
        //----------------- Receive Ports - Clock Correction Ports -----------------
        .RXCLKCORCNT                    (rxclkcorcnt_out),
        //------------- Receive Ports - Comma Detection and Alignment --------------
        .RXBYTEISALIGNED                (rxbyteisaligned_out),
        .RXBYTEREALIGN                  (rxbyterealign_out),
        .RXCOMMADET                     (rxcommadet_out),
        .RXCOMMADETEN                   (tied_to_vcc_i),
        .RXMCOMMAALIGNEN                (rxmcommaalignen_in),
        .RXPCOMMAALIGNEN                (rxpcommaalignen_in),
        .RXSLIDE                        (tied_to_ground_i),
        //--------------------- Receive Ports - PRBS Detection ---------------------
        .RXPRBSCNTRESET                 (rxprbscntreset_in),
        .RXPRBSERR                      (rxprbserr_out),
        .RXPRBSSEL                      (rxprbssel_in),
        //----------------- Receive Ports - RX Data Path interface -----------------
        .GTRXRESET                      (gtrxreset_in),
        .RXDATA                         (rxdata_i),
        .RXOUTCLK                       (rxoutclk_out),
        .RXOUTCLKFABRIC                 (),
        .RXOUTCLKPCS                    (),
        .RXOUTCLKSEL                    (3'b010),
        .RXPCSRESET                     (rxpcsreset_in),
        .RXPMARESET                     (rxpmareset_in),
        .RXUSRCLK                       (rxusrclk_in),
        .RXUSRCLK2                      (rxusrclk2_in),
        //---------- Receive Ports - RX Decision Feedback Equalizer(DFE) -----------
        .RXDFEAGCHOLD                   (rxdfeagchold_in),
        .RXDFEAGCOVRDEN                 (tied_to_ground_i),
        .RXDFECM1EN                     (tied_to_ground_i),
        .RXDFELFHOLD                    (rxdfelfhold_in),
        .RXDFELFOVRDEN                  (tied_to_vcc_i),
        .RXDFELPMRESET                  (rxdfelpmreset_in),
        .RXDFETAP2HOLD                  (tied_to_ground_i),
        .RXDFETAP2OVRDEN                (tied_to_ground_i),
        .RXDFETAP3HOLD                  (tied_to_ground_i),
        .RXDFETAP3OVRDEN                (tied_to_ground_i),
        .RXDFETAP4HOLD                  (tied_to_ground_i),
        .RXDFETAP4OVRDEN                (tied_to_ground_i),
        .RXDFETAP5HOLD                  (tied_to_ground_i),
        .RXDFETAP5OVRDEN                (tied_to_ground_i),
        .RXDFEUTHOLD                    (tied_to_ground_i),
        .RXDFEUTOVRDEN                  (tied_to_ground_i),
        .RXDFEVPHOLD                    (tied_to_ground_i),
        .RXDFEVPOVRDEN                  (tied_to_ground_i),
        .RXDFEVSEN                      (tied_to_ground_i),
        .RXDFEXYDEN                     (tied_to_vcc_i),
        .RXDFEXYDHOLD                   (tied_to_ground_i),
        .RXDFEXYDOVRDEN                 (tied_to_ground_i),
        .RXMONITOROUT                   (rxmonitorout_out),
        .RXMONITORSEL                   (rxmonitorsel_in),
        .RXOSHOLD                       (tied_to_ground_i),
        .RXOSOVRDEN                     (tied_to_ground_i),
        //----- Receive Ports - RX Driver,OOB signalling,Coupling and Eq.,CDR ------
        .GTXRXN                         (gtxrxn_in),
        .GTXRXP                         (gtxrxp_in),
        .RXCDRFREQRESET                 (tied_to_ground_i),
        .RXCDRHOLD                      (rxcdrhold_in),
        .RXCDRLOCK                      (rxcdrlock_out),
        .RXCDROVRDEN                    (rxcdrovrden_in),
        .RXCDRRESET                     (tied_to_ground_i),
        .RXCDRRESETRSV                  (tied_to_ground_i),
        .RXELECIDLE                     (),
        .RXELECIDLEMODE                 (2'b11),
        .RXLPMEN                        (rxlpmen_in),
        .RXLPMHFHOLD                    (tied_to_ground_i),
        .RXLPMHFOVRDEN                  (tied_to_ground_i),
        .RXLPMLFHOLD                    (tied_to_ground_i),
        .RXLPMLFKLOVRDEN                (tied_to_ground_i),
        .RXOOBRESET                     (tied_to_ground_i),
        //------ Receive Ports - RX Elastic Buffer and Phase Alignment Ports -------
        .RXBUFRESET                     (rxbufreset_in),
        .RXBUFSTATUS                    (rxbufstatus_out),
        .RXDDIEN                        (tied_to_ground_i),
        .RXDLYBYPASS                    (tied_to_vcc_i),
        .RXDLYEN                        (tied_to_ground_i),
        .RXDLYOVRDEN                    (tied_to_ground_i),
        .RXDLYSRESET                    (tied_to_ground_i),
        .RXDLYSRESETDONE                (),
        .RXPHALIGN                      (tied_to_ground_i),
        .RXPHALIGNDONE                  (),
        .RXPHALIGNEN                    (tied_to_ground_i),
        .RXPHDLYPD                      (tied_to_ground_i),
        .RXPHDLYRESET                   (tied_to_ground_i),
        .RXPHMONITOR                    (),
        .RXPHOVRDEN                     (tied_to_ground_i),
        .RXPHSLIPMONITOR                (),
        .RXSTATUS                       (),
        //---------------------- Receive Ports - RX PLL Ports ----------------------
        .RXRATE                         (tied_to_ground_vec_i[2:0]),
        .RXRATEDONE                     (),
        .RXRESETDONE                    (rxresetdone_out),
        //------------ Receive Ports - RX Pipe Control for PCI Express -------------
        .PHYSTATUS                      (),
        .RXVALID                        (),
        //--------------- Receive Ports - RX Polarity Control Ports ----------------
        .RXPOLARITY                     (rxpolarity_in),
        //------------------- Receive Ports - RX Ports for SATA --------------------
        .RXCOMINITDET                   (),
        .RXCOMSASDET                    (),
        .RXCOMWAKEDET                   (),
        //----------------------------- Transmit Ports -----------------------------
        .SETERRSTATUS                   (tied_to_ground_i),
        .TSTIN                          (20'b11111111111111111111),
        .TSTOUT                         (),
        .TXPHDLYTSTCLK                  (tied_to_ground_i),
        .TXPOSTCURSOR                   (txpostcursor_in),
        .TXPOSTCURSORINV                (tied_to_ground_i),
        .TXPRECURSOR                    (txprecursor_in),
        .TXPRECURSORINV                 (tied_to_ground_i),
        .TXQPIBIASEN                    (tied_to_ground_i),
        .TXQPISENN                      (),
        .TXQPISENP                      (),
        .TXQPISTRONGPDOWN               (tied_to_ground_i),
        .TXQPIWEAKPUP                   (tied_to_ground_i),
        .TXSYSCLKSEL                    (2'b00),
        .TXUSERRDY                      (txuserrdy_in),
        //------------ Transmit Ports - 64b66b and 64b67b Gearbox Ports ------------
        .TXGEARBOXREADY                 (),
        .TXHEADER                       (tied_to_ground_vec_i[2:0]),
        .TXSEQUENCE                     (tied_to_ground_vec_i[6:0]),
        .TXSTARTSEQ                     (tied_to_ground_i),
        //-------------- Transmit Ports - 8b10b Encoder Control Ports --------------
        .TX8B10BBYPASS                  (tied_to_ground_vec_i[7:0]),
        .TX8B10BEN                      (tied_to_vcc_i),
        .TXCHARDISPMODE                 ({tied_to_ground_vec_i[5:0],txchardispmode_in}),
        .TXCHARDISPVAL                  ({tied_to_ground_vec_i[5:0],txchardispval_in}),
        .TXCHARISK                      ({tied_to_ground_vec_i[5:0],txcharisk_in}),
        //---------- Transmit Ports - TX Buffer and Phase Alignment Ports ----------
        .TXBUFSTATUS                    (txbufstatus_out),
        .TXDLYBYPASS                    (tied_to_vcc_i),
        .TXDLYEN                        (tied_to_ground_i),
        .TXDLYHOLD                      (tied_to_ground_i),
        .TXDLYOVRDEN                    (tied_to_ground_i),
        .TXDLYSRESET                    (tied_to_ground_i),
        .TXDLYSRESETDONE                (),
        .TXDLYUPDOWN                    (tied_to_ground_i),
        .TXPHALIGN                      (tied_to_ground_i),
        .TXPHALIGNDONE                  (),
        .TXPHALIGNEN                    (tied_to_ground_i),
        .TXPHDLYPD                      (tied_to_ground_i),
        .TXPHDLYRESET                   (tied_to_ground_i),
        .TXPHINIT                       (tied_to_ground_i),
        .TXPHINITDONE                   (),
        .TXPHOVRDEN                     (tied_to_ground_i),
        //---------------- Transmit Ports - TX Data Path interface -----------------
        .GTTXRESET                      (gttxreset_in),
        .TXDATA                         (txdata_i),
        .TXOUTCLK                       (txoutclk_out),
        .TXOUTCLKFABRIC                 (txoutclkfabric_out),
        .TXOUTCLKPCS                    (txoutclkpcs_out),
        .TXOUTCLKSEL                    (3'b010),
        .TXPCSRESET                     (txpcsreset_in),
        .TXPMARESET                     (txpmareset_in),
        .TXUSRCLK                       (txusrclk_in),
        .TXUSRCLK2                      (txusrclk2_in),
        //-------------- Transmit Ports - TX Driver and OOB signaling --------------
        .GTXTXN                         (gtxtxn_out),
        .GTXTXP                         (gtxtxp_out),
        .TXBUFDIFFCTRL                  (3'b100),
        .TXDIFFCTRL                     (txdiffctrl_in),
        .TXDIFFPD                       (tied_to_ground_i),
        .TXINHIBIT                      (txinhibit_in),
        .TXMAINCURSOR                   (txmaincursor_in),
        .TXPDELECIDLEMODE               (tied_to_ground_i),
        .TXPISOPD                       (tied_to_ground_i),
        //--------------------- Transmit Ports - TX PLL Ports ----------------------
        .TXRATE                         (tied_to_ground_vec_i[2:0]),
        .TXRATEDONE                     (),
        .TXRESETDONE                    (txresetdone_out),
        //------------------- Transmit Ports - TX PRBS Generator -------------------
        .TXPRBSFORCEERR                 (txprbsforceerr_in),
        .TXPRBSSEL                      (txprbssel_in),
        //------------------ Transmit Ports - TX Polarity Control ------------------
        .TXPOLARITY                     (txpolarity_in),
        //--------------- Transmit Ports - TX Ports for PCI Express ----------------
        .TXDEEMPH                       (tied_to_ground_i),
        .TXDETECTRX                     (tied_to_ground_i),
        .TXELECIDLE                     (txelecidle_in),
        .TXMARGIN                       (tied_to_ground_vec_i[2:0]),
        .TXSWING                        (tied_to_ground_i),
        //------------------- Transmit Ports - TX Ports for SATA -------------------
        .TXCOMFINISH                    (),
        .TXCOMINIT                      (tied_to_ground_i),
        .TXCOMSAS                       (tied_to_ground_i),
        .TXCOMWAKE                      (tied_to_ground_i)

     );



always @(posedge gtrefclk0_in)
begin
  cpllpd_wait <= {cpllpd_wait[94:0], 1'b0};
  cpllreset_wait <= {cpllreset_wait[126:0], 1'b0};
end

assign cpllpd_ovrd_i = cpllpd_wait[95];
assign cpllreset_ovrd_i = cpllreset_wait[127];

assign cpll_pd_i = cpllpd_ovrd_i;

always @(posedge cplllockdetclk_in)
begin
if(cpllreset_in == 1'b1 && ack_flag == 1'b0)
begin
    flag <= !flag;
    flag2 <= 1'b1;
end
else
begin
    flag <= flag; 
    flag2 <= 1'b0;
end
end


always @(posedge cplllockdetclk_in)
begin
if(flag2 == 1'b1)
 ack_flag <= 1'b1;
else if(ack_i == 1'b1)
 ack_flag <= 1'b0;
end



  (* shreg_extract = "no", ASYNC_REG = "TRUE" *)
  FD #(
    .INIT (1'b0)
  ) data_sync_reg1 (
    .C  (gtrefclk0_in),
    .D  (flag),
    .Q  (data_sync1)
  );


  (* shreg_extract = "no", ASYNC_REG = "TRUE" *)
  FD #(
   .INIT (1'b0)
  ) data_sync_reg2 (
  .C  (gtrefclk0_in),
  .D  (data_sync1),
  .Q  (data_sync2)
  );


  (* shreg_extract = "no", ASYNC_REG = "TRUE" *)
  FD #(
   .INIT (1'b0)
  ) data_sync_reg3 (
  .C  (gtrefclk0_in),
  .D  (data_sync2),
  .Q  (data_sync3)
  );

  (* shreg_extract = "no", ASYNC_REG = "TRUE" *)
  FD #(
   .INIT (1'b0)
  ) data_sync_reg4 (
  .C  (gtrefclk0_in),
  .D  (data_sync3),
  .Q  (data_sync4)
  );

  (* shreg_extract = "no", ASYNC_REG = "TRUE" *)
  FD #(
   .INIT (1'b0)
  ) data_sync_reg5 (
  .C  (gtrefclk0_in),
  .D  (data_sync4),
  .Q  (data_sync5)
  );

  (* shreg_extract = "no", ASYNC_REG = "TRUE" *)
  FD #(
   .INIT (1'b0)
  ) data_sync_reg6 (
  .C  (gtrefclk0_in),
  .D  (data_sync5),
  .Q  (data_sync6)
  );

assign cpllreset_sync = data_sync5 ^ data_sync6;

  (* shreg_extract = "no", ASYNC_REG = "TRUE" *)
  FD #(
   .INIT (1'b0)
  ) ack_sync_reg1 (
  .C  (cplllockdetclk_in),
  .D  (data_sync6),
  .Q  (ack_sync1)
  );

  (* shreg_extract = "no", ASYNC_REG = "TRUE" *)
  FD #(
   .INIT (1'b0)
  ) ack_sync_reg2 (
  .C  (cplllockdetclk_in),
  .D  (ack_sync1),
  .Q  (ack_sync2)
  );

  (* shreg_extract = "no", ASYNC_REG = "TRUE" *)
  FD #(
   .INIT (1'b0)
  ) ack_sync_reg3 (
  .C  (cplllockdetclk_in),
  .D  (ack_sync2),
  .Q  (ack_sync3)
  );

  (* shreg_extract = "no", ASYNC_REG = "TRUE" *)
  FD #(
   .INIT (1'b0)
  ) ack_sync_reg4 (
  .C  (cplllockdetclk_in),
  .D  (ack_sync3),
  .Q  (ack_sync4)
  );

  (* shreg_extract = "no", ASYNC_REG = "TRUE" *)
  FD #(
   .INIT (1'b0)
  ) ack_sync_reg5 (
  .C  (cplllockdetclk_in),
  .D  (ack_sync4),
  .Q  (ack_sync5)
  );

  (* shreg_extract = "no", ASYNC_REG = "TRUE" *)
  FD #(
   .INIT (1'b0)
  ) ack_sync_reg6 (
  .C  (cplllockdetclk_in),
  .D  (ack_sync5),
  .Q  (ack_sync6)
  );

assign ack_i = ack_sync5 ^ ack_sync6;

assign cpll_reset_i = cpllreset_sync || cpllreset_ovrd_i;

endmodule   
