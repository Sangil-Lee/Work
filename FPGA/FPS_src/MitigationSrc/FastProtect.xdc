# MIT License
#
# Copyright (c) 2106 Osprey DCS
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

# SI5324 clock generator
set_property PACKAGE_PIN AC7 [get_ports SI5324_REFCLK_N]
set_property PACKAGE_PIN AC8 [get_ports SI5324_REFCLK_P]
set_property PACKAGE_PIN W23 [get_ports SI5324_RST]
set_property PACKAGE_PIN AJ25 [get_ports SI5324_INT_ALM]
set_property IOSTANDARD LVCMOS25 [get_ports SI5324_RST]
set_property IOSTANDARD LVCMOS25 [get_ports SI5324_INT_ALM]

# HPC FMC reference clocks
set_property PACKAGE_PIN AD9 [get_ports FMC_HPC_GBTCLK0_M2C_N]
set_property PACKAGE_PIN AD10 [get_ports FMC_HPC_GBTCLK0_M2C_P]
set_property PACKAGE_PIN AA7 [get_ports FMC_HPC_GBTCLK1_M2C_N]
set_property PACKAGE_PIN AA8 [get_ports FMC_HPC_GBTCLK1_M2C_P]

# On-board SFP (Bank 111 GTX 2)
set_property PACKAGE_PIN AA18 [get_ports SFP0_TX_DISABLE]
set_property IOSTANDARD LVCMOS25 [get_ports SFP0_TX_DISABLE]
set_property PACKAGE_PIN Y6 [get_ports SFP0_RX_P]
set_property PACKAGE_PIN Y5 [get_ports SFP0_RX_N]
##set_property PACKAGE_PIN W3 [get_ports SFP0_TX_N]
##set_property PACKAGE_PIN W4 [get_ports SFP0_TX_P]

# FPGA fan
set_property PACKAGE_PIN AA19 [get_ports SM_FAN_TACH]
set_property IOSTANDARD LVCMOS25 [get_ports SM_FAN_TACH]
set_property PACKAGE_PIN AB19 [get_ports SM_FAN_PWM]
set_property IOSTANDARD LVCMOS25 [get_ports SM_FAN_PWM]

# GPIO LEDs
set_property PACKAGE_PIN A17 [get_ports GPIO_LED_0]
set_property IOSTANDARD LVCMOS15 [get_ports GPIO_LED_0]
set_property PACKAGE_PIN G2 [get_ports GPIO_LED_CENTER]
set_property IOSTANDARD LVCMOS15 [get_ports GPIO_LED_CENTER]
set_property PACKAGE_PIN Y21 [get_ports GPIO_LED_LEFT]
set_property IOSTANDARD LVCMOS25 [get_ports GPIO_LED_LEFT]
set_property PACKAGE_PIN W21 [get_ports GPIO_LED_RIGHT]
set_property IOSTANDARD LVCMOS25 [get_ports GPIO_LED_RIGHT]

# Logic analyzer PMOD (J58 1-8)
set_property PACKAGE_PIN AJ21 [get_ports {PMOD1_DIAG[0]}]
set_property PACKAGE_PIN AK21 [get_ports {PMOD1_DIAG[1]}]
set_property PACKAGE_PIN AB21 [get_ports {PMOD1_DIAG[2]}]
set_property PACKAGE_PIN AB16 [get_ports {PMOD1_DIAG[3]}]
set_property PACKAGE_PIN Y20 [get_ports {PMOD1_DIAG[4]}]
set_property PACKAGE_PIN AA20 [get_ports {PMOD1_DIAG[5]}]
set_property PACKAGE_PIN AC18 [get_ports {PMOD1_DIAG[6]}]
set_property PACKAGE_PIN AC19 [get_ports {PMOD1_DIAG[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {PMOD1_DIAG[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {PMOD1_DIAG[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {PMOD1_DIAG[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {PMOD1_DIAG[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {PMOD1_DIAG[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {PMOD1_DIAG[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {PMOD1_DIAG[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {PMOD1_DIAG[7]}]

# Mitigation outputs (XM105 Debug Card on LPC FMC)
# J20-1 FMC_LA20_P
set_property PACKAGE_PIN AG26 [get_ports {MITIGATION_OUTPUTS[0]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[0]}]
# J20-2 FMC_LA24_P
set_property PACKAGE_PIN AF30 [get_ports {MITIGATION_OUTPUTS[1]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[1]}]
# J20-3 FMC_LA20_N
set_property PACKAGE_PIN AG27 [get_ports {MITIGATION_OUTPUTS[2]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[2]}]
# J20-4 FMC_LA24_N
set_property PACKAGE_PIN AG30 [get_ports {MITIGATION_OUTPUTS[3]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[3]}]
# J20-5 FMC_LA21_P
set_property PACKAGE_PIN AH28 [get_ports {MITIGATION_OUTPUTS[4]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[4]}]
# J20-6 FMC_LA25_P
set_property PACKAGE_PIN AF29 [get_ports {MITIGATION_OUTPUTS[5]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[5]}]
# J20-7 FMC_LA21_N
set_property PACKAGE_PIN AH29 [get_ports {MITIGATION_OUTPUTS[6]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[6]}]
# J20-8 FMC_LA25_N
set_property PACKAGE_PIN AG29 [get_ports {MITIGATION_OUTPUTS[7]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[7]}]
# J20-9 FMC_LA22_P
set_property PACKAGE_PIN AK27 [get_ports {MITIGATION_OUTPUTS[8]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[8]}]
# J20-10 FMC_LA26_P
set_property PACKAGE_PIN AJ30 [get_ports {MITIGATION_OUTPUTS[9]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[9]}]
# J20-11 FMC_LA22_N
set_property PACKAGE_PIN AK28 [get_ports {MITIGATION_OUTPUTS[10]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[10]}]
# J20-12 FMC_LA26_N
set_property PACKAGE_PIN AK30 [get_ports {MITIGATION_OUTPUTS[11]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[11]}]
# J20-13 FMC_LA23_P
set_property PACKAGE_PIN AJ26 [get_ports {MITIGATION_OUTPUTS[12]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[12]}]
# J20-14 FMC_LA27_P
set_property PACKAGE_PIN AJ28 [get_ports {MITIGATION_OUTPUTS[13]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[13]}]
# J20-15 FMC_LA23_N
set_property PACKAGE_PIN AK26 [get_ports {MITIGATION_OUTPUTS[14]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[14]}]
# J20-16 FMC_LA27_N
set_property PACKAGE_PIN AJ29 [get_ports {MITIGATION_OUTPUTS[15]}]
set_property IOSTANDARD LVCMOS25 [get_ports {MITIGATION_OUTPUTS[15]}]

###############################################################################
# CAENels FMC-4SFP+ on HPC FMC
# Machine generated from Xilinx zc706_r1.0.ucf.xdc
# Bank 109
set_property PACKAGE_PIN AE7 [get_ports FMC_HPC_SFP1_RX_N]
set_property PACKAGE_PIN AK1 [get_ports FMC_HPC_SFP1_TX_N]
set_property PACKAGE_PIN AK2 [get_ports FMC_HPC_SFP1_TX_P]
set_property PACKAGE_PIN AE8 [get_ports FMC_HPC_SFP1_RX_P]
set_property PACKAGE_PIN AJ7 [get_ports FMC_HPC_SFP2_RX_N]
set_property PACKAGE_PIN AK5 [get_ports FMC_HPC_SFP2_TX_N]
set_property PACKAGE_PIN AK6 [get_ports FMC_HPC_SFP2_TX_P]
set_property PACKAGE_PIN AJ8 [get_ports FMC_HPC_SFP2_RX_P]
set_property PACKAGE_PIN AH9 [get_ports FMC_HPC_SFP3_RX_N]
set_property PACKAGE_PIN AK9 [get_ports FMC_HPC_SFP3_TX_N]
set_property PACKAGE_PIN AK10 [get_ports FMC_HPC_SFP3_TX_P]
set_property PACKAGE_PIN AH10 [get_ports FMC_HPC_SFP3_RX_P]
set_property PACKAGE_PIN AG7 [get_ports FMC_HPC_SFP4_RX_N]
set_property PACKAGE_PIN AJ3 [get_ports FMC_HPC_SFP4_TX_N]
set_property PACKAGE_PIN AG8 [get_ports FMC_HPC_SFP4_RX_P]
set_property PACKAGE_PIN AJ4 [get_ports FMC_HPC_SFP4_TX_P]
set_property PACKAGE_PIN AJ24 [get_ports FMC_HPC_SFP1_MODABS]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP1_MODABS]
set_property PACKAGE_PIN AJ23 [get_ports FMC_HPC_SFP2_MODABS]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP2_MODABS]
set_property PACKAGE_PIN AH22 [get_ports FMC_HPC_SFP3_MODABS]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP3_MODABS]
set_property PACKAGE_PIN AG22 [get_ports FMC_HPC_SFP4_MODABS]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP4_MODABS]
set_property PACKAGE_PIN AH24 [get_ports FMC_HPC_SFP1_RXLOS]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP1_RXLOS]
set_property PACKAGE_PIN AH23 [get_ports FMC_HPC_SFP2_RXLOS]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP2_RXLOS]
set_property PACKAGE_PIN AK20 [get_ports FMC_HPC_SFP3_RXLOS]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP3_RXLOS]
set_property PACKAGE_PIN AJ20 [get_ports FMC_HPC_SFP4_RXLOS]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP4_RXLOS]
set_property PACKAGE_PIN AJ19 [get_ports FMC_HPC_SFP1_TXFAULT]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP1_TXFAULT]
set_property PACKAGE_PIN AH19 [get_ports FMC_HPC_SFP2_TXFAULT]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP2_TXFAULT]
set_property PACKAGE_PIN AK18 [get_ports FMC_HPC_SFP3_TXFAULT]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP3_TXFAULT]
set_property PACKAGE_PIN AK17 [get_ports FMC_HPC_SFP4_TXFAULT]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP4_TXFAULT]
set_property PACKAGE_PIN AE21 [get_ports FMC_HPC_SFP1_TXDISABLE]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP1_TXDISABLE]
set_property PACKAGE_PIN AD21 [get_ports FMC_HPC_SFP2_TXDISABLE]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP2_TXDISABLE]
set_property PACKAGE_PIN AG19 [get_ports FMC_HPC_SFP3_TXDISABLE]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP3_TXDISABLE]
set_property PACKAGE_PIN AF19 [get_ports FMC_HPC_SFP4_TXDISABLE]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP4_TXDISABLE]
set_property PACKAGE_PIN AE23 [get_ports FMC_HPC_SFP1_RS0]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP1_RS0]
set_property PACKAGE_PIN AD23 [get_ports FMC_HPC_SFP2_RS0]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP2_RS0]
set_property PACKAGE_PIN AG25 [get_ports FMC_HPC_SFP3_RS0]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP3_RS0]
set_property PACKAGE_PIN AG24 [get_ports FMC_HPC_SFP4_RS0]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP4_RS0]
set_property PACKAGE_PIN AA23 [get_ports FMC_HPC_SFP1_RS1]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP1_RS1]
set_property PACKAGE_PIN AA22 [get_ports FMC_HPC_SFP2_RS1]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP2_RS1]
set_property PACKAGE_PIN AD24 [get_ports FMC_HPC_SFP3_RS1]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP3_RS1]
set_property PACKAGE_PIN AC24 [get_ports FMC_HPC_SFP4_RS1]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP4_RS1]
set_property PACKAGE_PIN W29 [get_ports FMC_HPC_SFP1_SCL]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP1_SCL]
set_property PULLUP true [get_ports FMC_HPC_SFP1_SCL]
set_property PACKAGE_PIN V27 [get_ports FMC_HPC_SFP2_SCL]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP2_SCL]
set_property PULLUP true [get_ports FMC_HPC_SFP2_SCL]
set_property PACKAGE_PIN T30 [get_ports FMC_HPC_SFP3_SCL]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP3_SCL]
set_property PULLUP true [get_ports FMC_HPC_SFP3_SCL]
set_property PACKAGE_PIN P25 [get_ports FMC_HPC_SFP4_SCL]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP4_SCL]
set_property PULLUP true [get_ports FMC_HPC_SFP4_SCL]
set_property PACKAGE_PIN W30 [get_ports FMC_HPC_SFP1_SDA]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP1_SDA]
set_property PULLUP true [get_ports FMC_HPC_SFP1_SDA]
set_property PACKAGE_PIN W28 [get_ports FMC_HPC_SFP2_SDA]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP2_SDA]
set_property PULLUP true [get_ports FMC_HPC_SFP2_SDA]
set_property PACKAGE_PIN U30 [get_ports FMC_HPC_SFP3_SDA]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP3_SDA]
set_property PULLUP true [get_ports FMC_HPC_SFP3_SDA]
set_property PACKAGE_PIN P26 [get_ports FMC_HPC_SFP4_SDA]
set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SFP4_SDA]
set_property PULLUP true [get_ports FMC_HPC_SFP4_SDA]
#set_property PACKAGE_PIN AD10 [get_ports FMC_HPC_SI570_CLK_P]
#set_property PACKAGE_PIN AD9 [get_ports FMC_HPC_SI570_CLK_N]
#set_property PACKAGE_PIN T29 [get_ports FMC_HPC_SI570_SCL]
#set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SI570_SCL]
#set_property PULLUP true [get_ports FMC_HPC_SI570_SCL]
#set_property PACKAGE_PIN U29 [get_ports FMC_HPC_SI570_SDA]
#set_property IOSTANDARD LVCMOS25 [get_ports FMC_HPC_SI570_SDA]
#set_property PULLUP true [get_ports FMC_HPC_SI570_SDA]


###############################################################################
# Additional transceivers as provided by an octal FMC card such as
# the Faster Technology FM-S18
# Bank 110
set_property PACKAGE_PIN AD5 [get_ports FMC_HPC_SFP5_RX_N]
set_property PACKAGE_PIN AD6 [get_ports FMC_HPC_SFP5_RX_P]
set_property PACKAGE_PIN AD1 [get_ports FMC_HPC_SFP5_TX_N]
set_property PACKAGE_PIN AD2 [get_ports FMC_HPC_SFP5_TX_P]
set_property PACKAGE_PIN AF5 [get_ports FMC_HPC_SFP6_RX_N]
set_property PACKAGE_PIN AF6 [get_ports FMC_HPC_SFP6_RX_P]
set_property PACKAGE_PIN AE3 [get_ports FMC_HPC_SFP6_TX_N]
set_property PACKAGE_PIN AE4 [get_ports FMC_HPC_SFP6_TX_P]
set_property PACKAGE_PIN AG3 [get_ports FMC_HPC_SFP7_RX_N]
set_property PACKAGE_PIN AG4 [get_ports FMC_HPC_SFP7_RX_P]
set_property PACKAGE_PIN AF1 [get_ports FMC_HPC_SFP7_TX_N]
set_property PACKAGE_PIN AF2 [get_ports FMC_HPC_SFP7_TX_P]
set_property PACKAGE_PIN AH1 [get_ports FMC_HPC_SFP8_TX_N]
set_property PACKAGE_PIN AH2 [get_ports FMC_HPC_SFP8_TX_P]
set_property PACKAGE_PIN AH5 [get_ports FMC_HPC_SFP8_RX_N]
set_property PACKAGE_PIN AH6 [get_ports FMC_HPC_SFP8_RX_P]



set_false_path -from [get_clocks clk_fpga_0] -to [get_clocks clk_fpga_1]
set_false_path -from [get_clocks clk_fpga_0] -to [get_clocks evrGTX_i/inst/evrGTX_i/gt0_evrGTX_i/gtxe2_i/RXOUTCLK]
set_false_path -from [get_clocks clk_fpga_0] -to [get_clocks base_zynq_design_i/AuroraA/aurora_8b10b_0/inst/base_zynq_design_aurora_8b10b_0_0_core_i/gt_wrapper_i/base_zynq_design_aurora_8b10b_0_0_multi_gt_i/gt0_base_zynq_design_aurora_8b10b_0_0_i/gtxe2_i/TXOUTCLK]

set_false_path -from [get_clocks evrGTX_i/inst/evrGTX_i/gt0_evrGTX_i/gtxe2_i/RXOUTCLK] -to [get_clocks clk_fpga_0]
set_false_path -from [get_clocks evrGTX_i/inst/evrGTX_i/gt0_evrGTX_i/gtxe2_i/RXOUTCLK] -to [get_clocks base_zynq_design_i/AuroraA/aurora_8b10b_0/inst/base_zynq_design_aurora_8b10b_0_0_core_i/gt_wrapper_i/base_zynq_design_aurora_8b10b_0_0_multi_gt_i/gt0_base_zynq_design_aurora_8b10b_0_0_i/gtxe2_i/TXOUTCLK]

set_false_path -from [get_clocks base_zynq_design_i/AuroraA/aurora_8b10b_0/inst/base_zynq_design_aurora_8b10b_0_0_core_i/gt_wrapper_i/base_zynq_design_aurora_8b10b_0_0_multi_gt_i/gt0_base_zynq_design_aurora_8b10b_0_0_i/gtxe2_i/TXOUTCLK] -to [get_clocks clk_fpga_0]
set_false_path -from [get_clocks base_zynq_design_i/AuroraA/aurora_8b10b_0/inst/base_zynq_design_aurora_8b10b_0_0_core_i/gt_wrapper_i/base_zynq_design_aurora_8b10b_0_0_multi_gt_i/gt0_base_zynq_design_aurora_8b10b_0_0_i/gtxe2_i/TXOUTCLK] -to [get_clocks evrGTX_i/inst/evrGTX_i/gt0_evrGTX_i/gtxe2_i/RXOUTCLK]
set_false_path -from [get_clocks base_zynq_design_i/AuroraA/aurora_8b10b_0/inst/base_zynq_design_aurora_8b10b_0_0_core_i/gt_wrapper_i/base_zynq_design_aurora_8b10b_0_0_multi_gt_i/gt0_base_zynq_design_aurora_8b10b_0_0_i/gtxe2_i/TXOUTCLK] -to [get_clocks base_zynq_design_i/AuroraB/aurora_8b10b_0/inst/base_zynq_design_aurora_8b10b_0_1_core_i/gt_wrapper_i/base_zynq_design_aurora_8b10b_0_1_multi_gt_i/gt0_base_zynq_design_aurora_8b10b_0_1_i/gtxe2_i/TXOUTCLK]

set_false_path -from [get_clocks base_zynq_design_i/AuroraB/aurora_8b10b_0/inst/base_zynq_design_aurora_8b10b_0_1_core_i/gt_wrapper_i/base_zynq_design_aurora_8b10b_0_1_multi_gt_i/gt0_base_zynq_design_aurora_8b10b_0_1_i/gtxe2_i/TXOUTCLK] -to [get_clocks base_zynq_design_i/AuroraA/aurora_8b10b_0/inst/base_zynq_design_aurora_8b10b_0_0_core_i/gt_wrapper_i/base_zynq_design_aurora_8b10b_0_0_multi_gt_i/gt0_base_zynq_design_aurora_8b10b_0_0_i/gtxe2_i/TXOUTCLK]








