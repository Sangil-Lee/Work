`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: Lawrence Berkeley National Lab  
// Engineer: Jonah Weber
// 
// Create Date: 11/10/2014 04:09:27 PM
// Design Name: 
// Module Name: evr_mgt_top
// Project Name: 
// Target Devices: xc7z
// Tool Versions:
// Description: Multi-gigabit transceiver for use with MRF-compatible embedded
// event receiver. Includes support for force phase alignment to fix phase of RX 
// recovered clock with respect to Event Generator RF clock.
// 
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module evr_mgt_top(
    input reset,    // MGT reset
    input ref_clk,  // MGT reference clock
    input drp_clk,  // MGT Dynamic reconfiguration port clock (required)
    input rx_in_p,  // MGT Receive channel positive input
    input rx_in_n,  // MGT Receive channel negative input
    output rec_clk_out, // MGT Receive channel recovered clock
    output [15:0] rx_par_data_out, // MGT Receive channel parallel data out
    output [1:0] chariscomma,   // MGT Receive channel character is a comma (one bit per parallel data byte)
    output [1:0] charisk,   // MGT Receive channel character is a "K" (one bit per parallel data byte)
    output reset_done,  // MGT reset is done
    output [15:0] debug   // Debug port
    );
    
    // Include force phase align support. Comment next line to remove support.
    `define FORCE_PHASE_ALIGN

    // BITSLIDE_COUNTER_FORCE indicates how many bits does the serial stream needs to slide to achieve word aligment
    parameter [4:0] BITSLIDE_COUNTER_FORCE = 5;
    // COMMA_IS_LSB_FORCE == 1: Force comma to align with parallel_word_out[7:0]
    // COMMA_IS_LSB_FORCE == 0: Force comma to align with parallel_word_out[15:8]
    parameter COMMA_IS_LSB_FORCE = 0;

    wire    gt0_rxoutclk_i;
    wire    gt0_rxoutclk_buf;
    wire    gt0_gtrxreset_i;
    wire    [15:0]  gt0_rxdata_i;
    wire    comma_detected_i;
    wire    gt0_rxcommadet_i;
    wire    gt0_rxslide_i;
    wire    gt0_bitslide_reset_i;
    wire    tied_to_ground_i;
    wire    tied_to_vcc_i;
    wire    soft_reset_i;
    wire    [8:0]   gt0_drpaddr_i;
    wire    [15:0]  gt0_drpdi_i;
    wire    gt0_drpen_i;
    wire    gt0_drpwe_i;
    wire    [15:0] gt0_rxdata_i_sync;
    wire    bitslide_sync_out;
    wire    gt0_rxresetdone_i;
    wire    wdog_mgt_reset;
    wire    reset_done_i;
    wire    byteisaligned;
    wire    phase_align_force_reset;
    wire    [4:0]  bitslide_count_out;

    // comma_is_lsb == 1: comma characted falls into parallel_word_out[7:0]
    // comma_is_lsb == 0: comma characted falls into parallel_word_out[15:8]
    reg comma_is_lsb = 1'b0;
       
    reg     [15:0]  gt0_rxdata_i_sync_reg;
    reg gt0_rxresetdone_r;
    reg gt0_rxresetdone_r2;
    reg gt0_rxresetdone_r3;
    
    //  Static signal Assigments    
    assign tied_to_ground_i             = 1'b0;
    assign tied_to_vcc_i                = 1'b1;
 
    assign gt0_drpaddr_i = 9'd0;
    assign gt0_drpdi_i = 16'd0;
    assign gt0_drpen_i = 1'b0;
    assign gt0_drpwe_i = 1'b0;
    
   // Insert device-specific MGT instance here
     evr_mgt_gtx  evr_mgt_gtx_i
        (
         .sysclk_in(drp_clk), // input wire sysclk_in
         .soft_reset_rx_in(soft_reset_i), // input wire soft_reset_in
         .dont_reset_on_data_error_in(tied_to_ground_i), // input wire dont_reset_on_data_error_in
         .gt0_tx_fsm_reset_done_out(), // output wire gt0_tx_fsm_reset_done_out
         .gt0_rx_fsm_reset_done_out(), // output wire gt0_rx_fsm_reset_done_out
         .gt0_data_valid_in(tied_to_vcc_i), // input wire gt0_data_valid_in
    
        //_________________________________________________________________________
        //GT0  (X0Y0)
        //____________________________CHANNEL PORTS________________________________
        //------------------------------- CPLL Ports -------------------------------
            .gt0_cpllfbclklost_out          (cpllfbclklost_out), // output wire gt0_cpllfbclklost_out
            .gt0_cplllock_out               (cplllock_out), // output wire gt0_cplllock_out
            .gt0_cplllockdetclk_in          (drp_clk), // input wire gt0_cplllockdetclk_in
            .gt0_cpllreset_in               (tied_to_ground_i), // input wire gt0_cpllreset_in
        //------------------------ Channel - Clocking Ports ------------------------
            .gt0_gtrefclk1_in               (1'b0), // input wire gt0_gtrefclk1_in
            .gt0_gtrefclk0_in               (ref_clk), // input wire gt0_gtrefclk0_in
        //-------------------------- Channel - DRP Ports  --------------------------
            .gt0_drpaddr_in                 (gt0_drpaddr_i), // input wire [8:0] gt0_drpaddr_in
            .gt0_drpclk_in                  (drp_clk), // input wire gt0_drpclk_in
            .gt0_drpdi_in                   (gt0_drpdi_i), // input wire [15:0] gt0_drpdi_in
            .gt0_drpdo_out                  (), // output wire [15:0] gt0_drpdo_out
            .gt0_drpen_in                   (gt0_drpen_i), // input wire gt0_drpen_in
            .gt0_drprdy_out                 (), // output wire gt0_drprdy_out
            .gt0_drpwe_in                   (gt0_drpwe_i), // input wire gt0_drpwe_in
        //------------------------- Digital Monitor Ports --------------------------
            .gt0_dmonitorout_out            (), // output wire [7:0] gt0_dmonitorout_out
        //------------------- RX Initialization and Reset Ports --------------------
            .gt0_eyescanreset_in            (tied_to_ground_i), // input wire gt0_eyescanreset_in
            .gt0_rxuserrdy_in               (tied_to_ground_i), // input wire gt0_rxuserrdy_in
        //------------------------ RX Margin Analysis Ports ------------------------
            .gt0_eyescandataerror_out       (), // output wire gt0_eyescandataerror_out
            .gt0_eyescantrigger_in          (tied_to_ground_i), // input wire gt0_eyescantrigger_in
        //---------------- Receive Ports - FPGA RX Interface Ports -----------------
            .gt0_rxusrclk_in                (rec_clk_out), // input wire gt0_rxusrclk_in
            .gt0_rxusrclk2_in               (rec_clk_out), // input wire gt0_rxusrclk2_in
        //---------------- Receive Ports - FPGA RX interface Ports -----------------
            .gt0_rxdata_out                 (gt0_rxdata_i), // output wire [15:0] gt0_rxdata_out
        //---------------- Receive Ports - RX 8B/10B Decoder Ports -----------------
            .gt0_rxdisperr_out              (), // output wire [1:0] gt0_rxdisperr_out
            .gt0_rxnotintable_out           (), // output wire [1:0] gt0_rxnotintable_out
        //------------------------- Receive Ports - RX AFE -------------------------
            .gt0_gtxrxp_in                  (rx_in_p), // input wire gt0_gtxrxp_in
        //---------------------- Receive Ports - RX AFE Ports ----------------------
            .gt0_gtxrxn_in                  (rx_in_n), // input wire gt0_gtxrxn_in
        //------------ Receive Ports - RX Byte and Word Alignment Ports ------------
            .gt0_rxbyteisaligned_out        (), // output wire gt0_rxbyteisaligned_out
            .gt0_rxcommadet_out             (gt0_rxcommadet_i), // output wire gt0_rxcommadet_out
        //------------------- Receive Ports - RX Equalizer Ports -------------------
            .gt0_rxdfelpmreset_in           (tied_to_ground_i), // input wire gt0_rxdfelpmreset_in
            .gt0_rxmonitorout_out           (), // output wire [6:0] gt0_rxmonitorout_out
            .gt0_rxmonitorsel_in            (2'b00), // input wire [1:0] gt0_rxmonitorsel_in
        //------------- Receive Ports - RX Fabric Output Control Ports -------------
            .gt0_rxoutclk_out               (gt0_rxoutclk_i), // output wire gt0_rxoutclk_out
        //----------- Receive Ports - RX Initialization and Reset Ports ------------
            .gt0_gtrxreset_in               (tied_to_ground_i), // input wire gt0_gtrxreset_in
            .gt0_rxpmareset_in              (tied_to_ground_i), // input wire gt0_rxpmareset_in
        //-------------------- Receive Ports - RX gearbox ports --------------------
            .gt0_rxslide_in                 (gt0_rxslide_i), // input wire gt0_rxslide_in
        //----------------- Receive Ports - RX8B/10B Decoder Ports -----------------
            .gt0_rxchariscomma_out          (chariscomma), // output wire [1:0] gt0_rxchariscomma_out
            .gt0_rxcharisk_out              (charisk), // output wire [1:0] gt0_rxcharisk_out
        //------------ Receive Ports -RX Initialization and Reset Ports ------------
            .gt0_rxresetdone_out            (gt0_rxresetdone_i), // output wire gt0_rxresetdone_out
        //------------------- TX Initialization and Reset Ports --------------------
            .gt0_gttxreset_in               (tied_to_ground_i), // input wire gt0_gttxreset_in
    
    
        //____________________________COMMON PORTS________________________________
         .gt0_qplloutclk_in(tied_to_ground_i), // input wire gt0_qplloutclk_in
         .gt0_qplloutrefclk_in(tied_to_ground_i) // input wire gt0_qplloutrefclk_in
        );
        
    // Route the MGT recovered clock to a clock buffer for distribution to the rest of the fabric
    BUFG rxoutclk_bufg_i
    (
        .I(gt0_rxoutclk_i),
        .O(rec_clk_out)
    );
    
   // Multiplexes all zeroes to RX parallel data out if bitslide logic is not synchronized.
    // Register parallel data after mux.
    assign gt0_rxdata_i_sync = bitslide_sync_out ? gt0_rxdata_i : 8'h0;
    always @ (posedge rec_clk_out) gt0_rxdata_i_sync_reg <= gt0_rxdata_i_sync;
    assign rx_par_data_out = gt0_rxdata_i_sync_reg;
    
    // Indicates that a comma was detected in either byte of parallel word
    assign comma_detected_i = (chariscomma[0] | chariscomma[1]);
    
    // Assign soft reset
//    assign soft_reset_i = gt0_gtrxreset_i || wdog_mgt_reset;
//    assign soft_reset_i = reset || wdog_mgt_reset;
    assign soft_reset_i = gt0_gtrxreset_i ;

    // Add some delay to gtx0_rxresetdone signal for diagnostics (not sure delay is needed)
    always @(posedge rec_clk_out) begin
        gt0_rxresetdone_r    <=   gt0_rxresetdone_i;
        gt0_rxresetdone_r2   <=   gt0_rxresetdone_r;
        gt0_rxresetdone_r3   <=   gt0_rxresetdone_r2;
    end
    
    assign reset_done_i = gt0_rxresetdone_r3;

    // Pass a reset out of the tile to use for the next level up in the hierarchy
    assign reset_done = reset_done_i;
    
    
    // Conditionally generated logic to support force phase alignment
    `ifdef FORCE_PHASE_ALIGN

        // Compile-time configurable phase and word alignment
                
        
        // Indicate the bitlslide state machine whether parallel word is aligned (byteisaligned == 1)
        gtx_byteisaligned gtx_byteisaligned_i(
            .ref_clk(rec_clk_out),
            .comma_detected(comma_detected_i),
            .byteisaligned_out(byteisaligned)
        );
        
        // Detect which 8-bit word alignment within the 16-bit word stream 
        always @(posedge rec_clk_out) if (comma_detected_i == 1'b1) comma_is_lsb <= chariscomma[0];
        
        gtx_rx_phase_force #(.BITSLIDE_COUNTER_FORCE(BITSLIDE_COUNTER_FORCE), .COMMA_IS_LSB_FORCE(COMMA_IS_LSB_FORCE))
        gtx_rx_phase_force_i(
            .ref_clk(rec_clk_out),
            .bitslide_counter(bitslide_count_out),
            .bitslide_syncd(bitslide_sync_out),
            .comma_is_lsb(comma_is_lsb),
            .gtx_reset_out(phase_align_force_reset)
        );
        
        // Manual Bitslide mechanism for word alignment:
        // Needed to achieve deterministic phase relationship between the recovered clock (rec_clk_out), and the clock source at the transmitter end.
        gtp_bitslide my_gtp_bitslide(
            .gtp_rst_i(gt0_gtrxreset_i),
            .gtp_rx_clk_i(rec_clk_out),
            .gtp_rx_comma_det_i(gt0_rxcommadet_i),
            .gtp_rx_byte_is_aligned_i(byteisaligned),
            .serdes_ready_i(reset_done_i),
            .gtp_rx_slide_o(gt0_rxslide_i),
            .gtp_rx_cdr_rst_o(gt0_bitslide_reset_i),
            .bitslide_o(bitslide_count_out),
            .synced_o(bitslide_sync_out)
        );
        
        // MGT reset logic
        assign gt0_gtrxreset_i = reset || phase_align_force_reset || gt0_bitslide_reset_i;
        
        //top level debug port
        assign debug = {bitslide_count_out,gt0_rxcommadet_i,phase_align_force_reset,byteisaligned,gt0_rxslide_i,gt0_gtrxreset_i,bitslide_sync_out,gt0_bitslide_reset_i,comma_is_lsb,comma_detected_i,soft_reset_i,1'b0};
        
    `else
        assign gt0_rxslide_i = tied_to_ground_i;
        assign gt0_bitslide_reset_i =  tied_to_ground_i;
        assign gt0_gtrxreset_i = reset;
        assign bitslide_sync_out = tied_to_vcc_i;
        
    `endif
   
endmodule
