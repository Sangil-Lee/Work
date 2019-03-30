// Copyright 1986-2015 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2015.4 (lin64) Build 1412921 Wed Nov 18 09:44:32 MST 2015
// Date        : Fri Mar  4 14:37:06 2016
// Host        : xildev4.lbl.gov running 64-bit Scientific Linux release 6.7 (Carbon)
// Command     : write_verilog -force -mode synth_stub
//               /home/4/mixedusers1/wenorum/src/ip_repo/evr/mgt/_vivado/evr_mgt.srcs/sources_1/ip/evr_mgt_gtx/evr_mgt_gtx_stub.v
// Design      : evr_mgt_gtx
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z045ffg900-2
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* X_CORE_INFO = "evr_mgt_gtx,gtwizard_v3_6_1,{protocol_file=Start_from_scratch}" *)
module evr_mgt_gtx(sysclk_in, soft_reset_rx_in, dont_reset_on_data_error_in, gt0_tx_fsm_reset_done_out, gt0_rx_fsm_reset_done_out, gt0_data_valid_in, gt0_cpllfbclklost_out, gt0_cplllock_out, gt0_cplllockdetclk_in, gt0_cpllreset_in, gt0_gtrefclk0_in, gt0_gtrefclk1_in, gt0_drpaddr_in, gt0_drpclk_in, gt0_drpdi_in, gt0_drpdo_out, gt0_drpen_in, gt0_drprdy_out, gt0_drpwe_in, gt0_dmonitorout_out, gt0_eyescanreset_in, gt0_rxuserrdy_in, gt0_eyescandataerror_out, gt0_eyescantrigger_in, gt0_rxusrclk_in, gt0_rxusrclk2_in, gt0_rxdata_out, gt0_rxdisperr_out, gt0_rxnotintable_out, gt0_gtxrxp_in, gt0_gtxrxn_in, gt0_rxbyteisaligned_out, gt0_rxcommadet_out, gt0_rxdfelpmreset_in, gt0_rxmonitorout_out, gt0_rxmonitorsel_in, gt0_rxoutclk_out, gt0_rxoutclkfabric_out, gt0_gtrxreset_in, gt0_rxpmareset_in, gt0_rxslide_in, gt0_rxchariscomma_out, gt0_rxcharisk_out, gt0_rxresetdone_out, gt0_gttxreset_in, gt0_qplloutclk_in, gt0_qplloutrefclk_in)
/* synthesis syn_black_box black_box_pad_pin="sysclk_in,soft_reset_rx_in,dont_reset_on_data_error_in,gt0_tx_fsm_reset_done_out,gt0_rx_fsm_reset_done_out,gt0_data_valid_in,gt0_cpllfbclklost_out,gt0_cplllock_out,gt0_cplllockdetclk_in,gt0_cpllreset_in,gt0_gtrefclk0_in,gt0_gtrefclk1_in,gt0_drpaddr_in[8:0],gt0_drpclk_in,gt0_drpdi_in[15:0],gt0_drpdo_out[15:0],gt0_drpen_in,gt0_drprdy_out,gt0_drpwe_in,gt0_dmonitorout_out[7:0],gt0_eyescanreset_in,gt0_rxuserrdy_in,gt0_eyescandataerror_out,gt0_eyescantrigger_in,gt0_rxusrclk_in,gt0_rxusrclk2_in,gt0_rxdata_out[15:0],gt0_rxdisperr_out[1:0],gt0_rxnotintable_out[1:0],gt0_gtxrxp_in,gt0_gtxrxn_in,gt0_rxbyteisaligned_out,gt0_rxcommadet_out,gt0_rxdfelpmreset_in,gt0_rxmonitorout_out[6:0],gt0_rxmonitorsel_in[1:0],gt0_rxoutclk_out,gt0_rxoutclkfabric_out,gt0_gtrxreset_in,gt0_rxpmareset_in,gt0_rxslide_in,gt0_rxchariscomma_out[1:0],gt0_rxcharisk_out[1:0],gt0_rxresetdone_out,gt0_gttxreset_in,gt0_qplloutclk_in,gt0_qplloutrefclk_in" */;
  input sysclk_in;
  input soft_reset_rx_in;
  input dont_reset_on_data_error_in;
  output gt0_tx_fsm_reset_done_out;
  output gt0_rx_fsm_reset_done_out;
  input gt0_data_valid_in;
  output gt0_cpllfbclklost_out;
  output gt0_cplllock_out;
  input gt0_cplllockdetclk_in;
  input gt0_cpllreset_in;
  input gt0_gtrefclk0_in;
  input gt0_gtrefclk1_in;
  input [8:0]gt0_drpaddr_in;
  input gt0_drpclk_in;
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
  input gt0_rxusrclk_in;
  input gt0_rxusrclk2_in;
  output [15:0]gt0_rxdata_out;
  output [1:0]gt0_rxdisperr_out;
  output [1:0]gt0_rxnotintable_out;
  input gt0_gtxrxp_in;
  input gt0_gtxrxn_in;
  output gt0_rxbyteisaligned_out;
  output gt0_rxcommadet_out;
  input gt0_rxdfelpmreset_in;
  output [6:0]gt0_rxmonitorout_out;
  input [1:0]gt0_rxmonitorsel_in;
  output gt0_rxoutclk_out;
  output gt0_rxoutclkfabric_out;
  input gt0_gtrxreset_in;
  input gt0_rxpmareset_in;
  input gt0_rxslide_in;
  output [1:0]gt0_rxchariscomma_out;
  output [1:0]gt0_rxcharisk_out;
  output gt0_rxresetdone_out;
  input gt0_gttxreset_in;
  input gt0_qplloutclk_in;
  input gt0_qplloutrefclk_in;
endmodule
