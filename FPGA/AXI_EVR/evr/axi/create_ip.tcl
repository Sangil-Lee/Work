# Usage: "vivado -nolog -nojou -mode batch -source ./lbl_ip.tcl ./create_ip.tcl"
source ./lbl_ip.tcl

set ip_name evr_axi
ip_create $ip_name
ip_files $ip_name [list \
    src/hdl/evr_v3_0.v \
    src/hdl/evr_v3_0_S00_AXI.v \
    src/hdl/EventReceiverTop.v \
    src/hdl/EventReceiverChannel.v \
    src/hdl/DataBufferCntrlr.v \
    src/hdl/timeofDayReceiver.v \
    src/hdl/timestamp_forward.v \
    src/hdl/irq_forward.v 
]
set_property board_part xilinx.com:zc706:part0:1.2 [current_project]

set_property top evr_v3_0 [current_fileset]
ip_properties $ip_name
update_compile_order -fileset sources_1

set_property name {evr_axi} [ipx::current_core]
set_property version {3.0} [ipx::current_core]
set_property display_name {evr_v3_0} [ipx::current_core]
set_property description {AXI Embedded EVR with MGT removed} [ipx::current_core]

create_ip -name blk_mem_gen -vendor xilinx.com -library ip -version 8.2 -module_name MappingRam
set_property -dict [list CONFIG.Memory_Type {True_Dual_Port_RAM} CONFIG.Write_Depth_A {256} CONFIG.Enable_A {Always_Enabled} CONFIG.Operating_Mode_B {READ_FIRST} CONFIG.Enable_B {Always_Enabled} CONFIG.Register_PortB_Output_of_Memory_Primitives {false} CONFIG.Port_B_Clock {100} CONFIG.Port_B_Write_Rate {50} CONFIG.Port_B_Enable_Rate {100}] [get_ips MappingRam]

create_ip -name blk_mem_gen -vendor xilinx.com -library ip -version 8.2 -module_name PacketMem
set_property -dict [list CONFIG.Memory_Type {Simple_Dual_Port_RAM} CONFIG.Write_Width_A {8} CONFIG.Write_Depth_A {2048} CONFIG.Operating_Mode_A {WRITE_FIRST} CONFIG.Enable_A {Always_Enabled} CONFIG.Enable_B {Always_Enabled} CONFIG.Register_PortB_Output_of_Memory_Primitives {false} CONFIG.Read_Width_A {8} CONFIG.Write_Width_B {8} CONFIG.Read_Width_B {8} CONFIG.Register_PortA_Output_of_Memory_Primitives {false} CONFIG.Port_B_Clock {100} CONFIG.Port_B_Enable_Rate {100}] [get_ips PacketMem]

create_ip -name fifo_generator -vendor xilinx.com -library ip -version 12.0 -module_name timeStampFIFO
set_property -dict [list CONFIG.Fifo_Implementation {Independent_Clocks_Block_RAM} CONFIG.Input_Data_Width {72} CONFIG.Output_Data_Width {72} CONFIG.Full_Threshold_Assert_Value {1021} CONFIG.Full_Threshold_Negate_Value {1020}] [get_ips timeStampFIFO]

ipx::add_file _vivado/evr_axi.srcs/sources_1/ip/timeStampFIFO/timeStampFIFO.xci [ipx::get_file_groups xilinx_anylanguagesynthesis -of_objects [ipx::current_core]]
set_property type xci [ipx::get_files _vivado/evr_axi.srcs/sources_1/ip/timeStampFIFO/timeStampFIFO.xci -of_objects [ipx::get_file_groups xilinx_anylanguagesynthesis -of_objects [ipx::current_core]]]
ipx::add_file _vivado/evr_axi.srcs/sources_1/ip/MappingRam/MappingRam.xci [ipx::get_file_groups xilinx_anylanguagesynthesis -of_objects [ipx::current_core]]
set_property type xci [ipx::get_files _vivado/evr_axi.srcs/sources_1/ip/MappingRam/MappingRam.xci -of_objects [ipx::get_file_groups xilinx_anylanguagesynthesis -of_objects [ipx::current_core]]]
ipx::add_file _vivado/evr_axi.srcs/sources_1/ip/PacketMem/PacketMem.xci [ipx::get_file_groups xilinx_anylanguagesynthesis -of_objects [ipx::current_core]]
set_property type xci [ipx::get_files _vivado/evr_axi.srcs/sources_1/ip/PacketMem/PacketMem.xci -of_objects [ipx::get_file_groups xilinx_anylanguagesynthesis -of_objects [ipx::current_core]]]
ipx::add_file _vivado/evr_axi.srcs/sources_1/ip/timeStampFIFO/timeStampFIFO.xci [ipx::get_file_groups xilinx_anylanguagebehavioralsimulation -of_objects [ipx::current_core]]
set_property type xci [ipx::get_files _vivado/evr_axi.srcs/sources_1/ip/timeStampFIFO/timeStampFIFO.xci -of_objects [ipx::get_file_groups xilinx_anylanguagebehavioralsimulation -of_objects [ipx::current_core]]]
ipx::add_file _vivado/evr_axi.srcs/sources_1/ip/MappingRam/MappingRam.xci [ipx::get_file_groups xilinx_anylanguagebehavioralsimulation -of_objects [ipx::current_core]]
set_property type xci [ipx::get_files _vivado/evr_axi.srcs/sources_1/ip/MappingRam/MappingRam.xci -of_objects [ipx::get_file_groups xilinx_anylanguagebehavioralsimulation -of_objects [ipx::current_core]]]
ipx::add_file _vivado/evr_axi.srcs/sources_1/ip/PacketMem/PacketMem.xci [ipx::get_file_groups xilinx_anylanguagebehavioralsimulation -of_objects [ipx::current_core]]
set_property type xci [ipx::get_files _vivado/evr_axi.srcs/sources_1/ip/PacketMem/PacketMem.xci -of_objects [ipx::get_file_groups xilinx_anylanguagebehavioralsimulation -of_objects [ipx::current_core]]]

ipx::reorder_files -after _vivado/evr_axi.srcs/sources_1/ip/PacketMem/PacketMem.xci src/hdl/evr_v3_0.v [ipx::get_file_groups xilinx_anylanguagesynthesis -of_objects [ipx::current_core]]
ipx::reorder_files -after _vivado/evr_axi.srcs/sources_1/ip/PacketMem/PacketMem.xci src/hdl/evr_v3_0.v [ipx::get_file_groups xilinx_anylanguagebehavioralsimulation -of_objects [ipx::current_core]]
ipx::remove_bus_interface mgt_rec_clk [ipx::current_core]

ipx::save_core [ipx::current_core]
