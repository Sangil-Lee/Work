// Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2016.1 (lin64) Build 1538259 Fri Apr  8 15:45:23 MDT 2016
// Date        : Mon Dec 19 09:33:56 2016
// Host        : localhost.localdomain running 64-bit CentOS Linux release 7.2.1511 (Core)
// Command     : write_verilog -force -mode synth_stub
//               /home/fastprot/src/tinyEVG/tinyEVG.srcs/sources_1/ip/evgTransceiver/evgTransceiver_stub.v
// Design      : evgTransceiver
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z045ffg900-2
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "evgTransceiver,gtwizard_v3_6_2,{protocol_file=Start_from_scratch}" *)
module evgTransceiver(soft_reset_tx_in, soft_reset_rx_in, dont_reset_on_data_error_in, q1_clk1_gtrefclk_pad_n_in, q1_clk1_gtrefclk_pad_p_in, gt0_tx_fsm_reset_done_out, gt0_rx_fsm_reset_done_out, gt0_data_valid_in, gt0_txusrclk_out, gt0_txusrclk2_out, gt0_rxusrclk_out, gt0_rxusrclk2_out, gt0_cpllfbclklost_out, gt0_cplllock_out, gt0_cpllreset_in, gt0_drpaddr_in, gt0_drpdi_in, gt0_drpdo_out, gt0_drpen_in, gt0_drprdy_out, gt0_drpwe_in, gt0_dmonitorout_out, gt0_eyescanreset_in, gt0_rxuserrdy_in, gt0_eyescandataerror_out, gt0_eyescantrigger_in, gt0_rxdata_out, gt0_rxdisperr_out, gt0_rxnotintable_out, gt0_gtxrxp_in, gt0_gtxrxn_in, gt0_rxphmonitor_out, gt0_rxphslipmonitor_out, gt0_rxdfelpmreset_in, gt0_rxmonitorout_out, gt0_rxmonitorsel_in, gt0_rxoutclkfabric_out, gt0_gtrxreset_in, gt0_rxpmareset_in, gt0_rxcharisk_out, gt0_rxresetdone_out, gt0_gttxreset_in, gt0_txuserrdy_in, gt0_txdata_in, gt0_gtxtxn_out, gt0_gtxtxp_out, gt0_txoutclkfabric_out, gt0_txoutclkpcs_out, gt0_txcharisk_in, gt0_txresetdone_out, gt0_qplloutclk_out, gt0_qplloutrefclk_out, sysclk_in)
/* synthesis syn_black_box black_box_pad_pin="soft_reset_tx_in,soft_reset_rx_in,dont_reset_on_data_error_in,q1_clk1_gtrefclk_pad_n_in,q1_clk1_gtrefclk_pad_p_in,gt0_tx_fsm_reset_done_out,gt0_rx_fsm_reset_done_out,gt0_data_valid_in,gt0_txusrclk_out,gt0_txusrclk2_out,gt0_rxusrclk_out,gt0_rxusrclk2_out,gt0_cpllfbclklost_out,gt0_cplllock_out,gt0_cpllreset_in,gt0_drpaddr_in[8:0],gt0_drpdi_in[15:0],gt0_drpdo_out[15:0],gt0_drpen_in,gt0_drprdy_out,gt0_drpwe_in,gt0_dmonitorout_out[7:0],gt0_eyescanreset_in,gt0_rxuserrdy_in,gt0_eyescandataerror_out,gt0_eyescantrigger_in,gt0_rxdata_out[15:0],gt0_rxdisperr_out[1:0],gt0_rxnotintable_out[1:0],gt0_gtxrxp_in,gt0_gtxrxn_in,gt0_rxphmonitor_out[4:0],gt0_rxphslipmonitor_out[4:0],gt0_rxdfelpmreset_in,gt0_rxmonitorout_out[6:0],gt0_rxmonitorsel_in[1:0],gt0_rxoutclkfabric_out,gt0_gtrxreset_in,gt0_rxpmareset_in,gt0_rxcharisk_out[1:0],gt0_rxresetdone_out,gt0_gttxreset_in,gt0_txuserrdy_in,gt0_txdata_in[15:0],gt0_gtxtxn_out,gt0_gtxtxp_out,gt0_txoutclkfabric_out,gt0_txoutclkpcs_out,gt0_txcharisk_in[1:0],gt0_txresetdone_out,gt0_qplloutclk_out,gt0_qplloutrefclk_out,sysclk_in" */;
  input soft_reset_tx_in;
  input soft_reset_rx_in;
  input dont_reset_on_data_error_in;
  input q1_clk1_gtrefclk_pad_n_in;
  input q1_clk1_gtrefclk_pad_p_in;
  output gt0_tx_fsm_reset_done_out;
  output gt0_rx_fsm_reset_done_out;
  input gt0_data_valid_in;
  output gt0_txusrclk_out;
  output gt0_txusrclk2_out;
  output gt0_rxusrclk_out;
  output gt0_rxusrclk2_out;
  output gt0_cpllfbclklost_out;
  output gt0_cplllock_out;
  input gt0_cpllreset_in;
  input [8:0]gt0_drpaddr_in;
  input [15:0]gt0_drpdi_in;
  output [15:0]gt0_drpdo_out;
  input gt0_drpen_in;
  output gt0_drprdy_out;
  input gt0_drpwe_in;
  output [7:0]gt0_dmonitorout_out;
  input gt0_eyescanreset_in;
  input gt0_rxuserrdy_in;
  output gt0_eyescandataerror_out;
  input gt0_eyescantrigger_in;
  output [15:0]gt0_rxdata_out;
  output [1:0]gt0_rxdisperr_out;
  output [1:0]gt0_rxnotintable_out;
  input gt0_gtxrxp_in;
  input gt0_gtxrxn_in;
  output [4:0]gt0_rxphmonitor_out;
  output [4:0]gt0_rxphslipmonitor_out;
  input gt0_rxdfelpmreset_in;
  output [6:0]gt0_rxmonitorout_out;
  input [1:0]gt0_rxmonitorsel_in;
  output gt0_rxoutclkfabric_out;
  input gt0_gtrxreset_in;
  input gt0_rxpmareset_in;
  output [1:0]gt0_rxcharisk_out;
  output gt0_rxresetdone_out;
  input gt0_gttxreset_in;
  input gt0_txuserrdy_in;
  input [15:0]gt0_txdata_in;
  output gt0_gtxtxn_out;
  output gt0_gtxtxp_out;
  output gt0_txoutclkfabric_out;
  output gt0_txoutclkpcs_out;
  input [1:0]gt0_txcharisk_in;
  output gt0_txresetdone_out;
  output gt0_qplloutclk_out;
  output gt0_qplloutrefclk_out;
  input sysclk_in;
endmodule
