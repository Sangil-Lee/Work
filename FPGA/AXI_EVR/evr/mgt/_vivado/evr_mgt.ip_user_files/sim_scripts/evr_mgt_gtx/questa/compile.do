vlib work
vlib msim

vlib msim/xil_defaultlib

vmap xil_defaultlib msim/xil_defaultlib

vlog -work xil_defaultlib -64 \
"../../../../evr_mgt.srcs/sources_1/ip/evr_mgt_gtx/evr_mgt_gtx/example_design/evr_mgt_gtx_tx_startup_fsm.v" \
"../../../../evr_mgt.srcs/sources_1/ip/evr_mgt_gtx/evr_mgt_gtx/example_design/evr_mgt_gtx_rx_startup_fsm.v" \
"../../../../evr_mgt.srcs/sources_1/ip/evr_mgt_gtx/evr_mgt_gtx_init.v" \
"../../../../evr_mgt.srcs/sources_1/ip/evr_mgt_gtx/evr_mgt_gtx_cpll_railing.v" \
"../../../../evr_mgt.srcs/sources_1/ip/evr_mgt_gtx/evr_mgt_gtx_gt.v" \
"../../../../evr_mgt.srcs/sources_1/ip/evr_mgt_gtx/evr_mgt_gtx_multi_gt.v" \
"../../../../evr_mgt.srcs/sources_1/ip/evr_mgt_gtx/evr_mgt_gtx/example_design/evr_mgt_gtx_sync_block.v" \
"../../../../evr_mgt.srcs/sources_1/ip/evr_mgt_gtx/evr_mgt_gtx.v" \


vlog -work xil_defaultlib "glbl.v"

