export XILINX=/opt/Xilinx/12.1/ISE_DS/ISE
export PATH=${XILINX}/bin/lin64:${XILINX}/sysgen/util:${PATH}

bitgen -b -w -g ActiveReconfig:Yes -g Persist:Yes -g Binary:Yes -r synth/top.bit synth_pr/top_pr.ncd top_pr.bit

