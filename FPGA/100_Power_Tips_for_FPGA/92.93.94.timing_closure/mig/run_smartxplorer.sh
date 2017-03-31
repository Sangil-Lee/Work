#!/bin/bash

#
# Copyright (C) 2011 OutputLogic.com
#

export XILINX=/opt/Xilinx/12.1/ISE_DS/ISE
export PATH=${XILINX}/bin/lin64:${XILINX}/sysgen/util:${PATH}


# echo "list all implementation strategies"
# smartxplorer -p xc6vlx75t-ff484-2 -la  example_top.ngd

echo "running smartxplorer"
smartxplorer -p xc6vlx75t-ff484-2 -l host_list.txt -uc example_top.ucf -area_report on -b  -sf strategy_file.sf -variability_passes 100 -ra -wd results example_top.ngd
