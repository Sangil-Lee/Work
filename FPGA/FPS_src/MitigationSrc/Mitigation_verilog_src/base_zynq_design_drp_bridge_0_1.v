// (c) Copyright 1995-2016 Xilinx, Inc. All rights reserved.
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
// DO NOT MODIFY THIS FILE.


// IP VLNV: xilinx.com:user:drp_bridge:1.0
// IP Revision: 1

`timescale 1ns/1ps

(* DowngradeIPIdentifiedWarnings = "yes" *)
module base_zynq_design_drp_bridge_0_1 (
  AXI_aclk,
  AXI_aresetn,
  S_AXI_araddr,
  S_AXI_arready,
  S_AXI_arvalid,
  S_AXI_arprot,
  S_AXI_awaddr,
  S_AXI_awready,
  S_AXI_awvalid,
  S_AXI_awprot,
  S_AXI_bresp,
  S_AXI_bready,
  S_AXI_bvalid,
  S_AXI_rdata,
  S_AXI_rready,
  S_AXI_rvalid,
  S_AXI_rresp,
  S_AXI_wdata,
  S_AXI_wready,
  S_AXI_wvalid,
  S_AXI_wstrb,
  drp0_en,
  drp0_we,
  drp0_addr,
  drp0_di,
  drp0_do,
  drp0_rdy,
  drp1_en,
  drp1_we,
  drp1_addr,
  drp1_di,
  drp1_do,
  drp1_rdy,
  drp2_en,
  drp2_we,
  drp2_addr,
  drp2_di,
  drp2_do,
  drp2_rdy,
  drp3_en,
  drp3_we,
  drp3_addr,
  drp3_di,
  drp3_do,
  drp3_rdy,
  drp4_en,
  drp4_we,
  drp4_addr,
  drp4_di,
  drp4_do,
  drp4_rdy,
  drp5_en,
  drp5_we,
  drp5_addr,
  drp5_di,
  drp5_do,
  drp5_rdy,
  drp6_en,
  drp6_we,
  drp6_addr,
  drp6_di,
  drp6_do,
  drp6_rdy,
  drp7_en,
  drp7_we,
  drp7_addr,
  drp7_di,
  drp7_do,
  drp7_rdy,
  drp8_en,
  drp8_we,
  drp8_addr,
  drp8_di,
  drp8_do,
  drp8_rdy
);

(* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 AXI_signal_clock CLK" *)
input wire AXI_aclk;
(* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 AXI_signal_reset RST" *)
input wire AXI_aresetn;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI ARADDR" *)
input wire [31 : 0] S_AXI_araddr;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI ARREADY" *)
output wire S_AXI_arready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI ARVALID" *)
input wire S_AXI_arvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI ARPROT" *)
input wire [2 : 0] S_AXI_arprot;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI AWADDR" *)
input wire [31 : 0] S_AXI_awaddr;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI AWREADY" *)
output wire S_AXI_awready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI AWVALID" *)
input wire S_AXI_awvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI AWPROT" *)
input wire [2 : 0] S_AXI_awprot;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI BRESP" *)
output wire [1 : 0] S_AXI_bresp;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI BREADY" *)
input wire S_AXI_bready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI BVALID" *)
output wire S_AXI_bvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI RDATA" *)
output wire [31 : 0] S_AXI_rdata;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI RREADY" *)
input wire S_AXI_rready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI RVALID" *)
output wire S_AXI_rvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI RRESP" *)
output wire [1 : 0] S_AXI_rresp;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI WDATA" *)
input wire [31 : 0] S_AXI_wdata;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI WREADY" *)
output wire S_AXI_wready;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI WVALID" *)
input wire S_AXI_wvalid;
(* X_INTERFACE_INFO = "xilinx.com:interface:aximm:1.0 S_AXI WSTRB" *)
input wire [3 : 0] S_AXI_wstrb;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP0 DEN" *)
output wire drp0_en;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP0 DWE" *)
output wire drp0_we;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP0 DADDR" *)
output wire [8 : 0] drp0_addr;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP0 DI" *)
output wire [15 : 0] drp0_di;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP0 DO" *)
input wire [15 : 0] drp0_do;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP0 DRDY" *)
input wire drp0_rdy;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP1 DEN" *)
output wire drp1_en;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP1 DWE" *)
output wire drp1_we;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP1 DADDR" *)
output wire [8 : 0] drp1_addr;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP1 DI" *)
output wire [15 : 0] drp1_di;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP1 DO" *)
input wire [15 : 0] drp1_do;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP1 DRDY" *)
input wire drp1_rdy;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP2 DEN" *)
output wire drp2_en;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP2 DWE" *)
output wire drp2_we;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP2 DADDR" *)
output wire [8 : 0] drp2_addr;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP2 DI" *)
output wire [15 : 0] drp2_di;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP2 DO" *)
input wire [15 : 0] drp2_do;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP2 DRDY" *)
input wire drp2_rdy;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP3 DEN" *)
output wire drp3_en;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP3 DWE" *)
output wire drp3_we;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP3 DADDR" *)
output wire [8 : 0] drp3_addr;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP3 DI" *)
output wire [15 : 0] drp3_di;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP3 DO" *)
input wire [15 : 0] drp3_do;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP3 DRDY" *)
input wire drp3_rdy;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP4 DEN" *)
output wire drp4_en;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP4 DWE" *)
output wire drp4_we;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP4 DADDR" *)
output wire [8 : 0] drp4_addr;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP4 DI" *)
output wire [15 : 0] drp4_di;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP4 DO" *)
input wire [15 : 0] drp4_do;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP4 DRDY" *)
input wire drp4_rdy;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP5 DEN" *)
output wire drp5_en;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP5 DWE" *)
output wire drp5_we;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP5 DADDR" *)
output wire [8 : 0] drp5_addr;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP5 DI" *)
output wire [15 : 0] drp5_di;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP5 DO" *)
input wire [15 : 0] drp5_do;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP5 DRDY" *)
input wire drp5_rdy;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP6 DEN" *)
output wire drp6_en;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP6 DWE" *)
output wire drp6_we;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP6 DADDR" *)
output wire [8 : 0] drp6_addr;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP6 DI" *)
output wire [15 : 0] drp6_di;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP6 DO" *)
input wire [15 : 0] drp6_do;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP6 DRDY" *)
input wire drp6_rdy;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP7 DEN" *)
output wire drp7_en;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP7 DWE" *)
output wire drp7_we;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP7 DADDR" *)
output wire [8 : 0] drp7_addr;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP7 DI" *)
output wire [15 : 0] drp7_di;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP7 DO" *)
input wire [15 : 0] drp7_do;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP7 DRDY" *)
input wire drp7_rdy;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP8 DEN" *)
output wire drp8_en;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP8 DWE" *)
output wire drp8_we;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP8 DADDR" *)
output wire [8 : 0] drp8_addr;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP8 DI" *)
output wire [15 : 0] drp8_di;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP8 DO" *)
input wire [15 : 0] drp8_do;
(* X_INTERFACE_INFO = "xilinx.com:interface:drp:1.0 DRP8 DRDY" *)
input wire drp8_rdy;

  drp_bridge #(
    .DRP_COUNT(9),
    .DRP_ADDR_WIDTH(9),
    .DRP_DATA_WIDTH(16),
    .S_AXI_ADDR_WIDTH(32),
    .S_AXI_DATA_WIDTH(32)
  ) inst (
    .AXI_aclk(AXI_aclk),
    .AXI_aresetn(AXI_aresetn),
    .S_AXI_araddr(S_AXI_araddr),
    .S_AXI_arready(S_AXI_arready),
    .S_AXI_arvalid(S_AXI_arvalid),
    .S_AXI_arprot(S_AXI_arprot),
    .S_AXI_awaddr(S_AXI_awaddr),
    .S_AXI_awready(S_AXI_awready),
    .S_AXI_awvalid(S_AXI_awvalid),
    .S_AXI_awprot(S_AXI_awprot),
    .S_AXI_bresp(S_AXI_bresp),
    .S_AXI_bready(S_AXI_bready),
    .S_AXI_bvalid(S_AXI_bvalid),
    .S_AXI_rdata(S_AXI_rdata),
    .S_AXI_rready(S_AXI_rready),
    .S_AXI_rvalid(S_AXI_rvalid),
    .S_AXI_rresp(S_AXI_rresp),
    .S_AXI_wdata(S_AXI_wdata),
    .S_AXI_wready(S_AXI_wready),
    .S_AXI_wvalid(S_AXI_wvalid),
    .S_AXI_wstrb(S_AXI_wstrb),
    .drp0_en(drp0_en),
    .drp0_we(drp0_we),
    .drp0_addr(drp0_addr),
    .drp0_di(drp0_di),
    .drp0_do(drp0_do),
    .drp0_rdy(drp0_rdy),
    .drp1_en(drp1_en),
    .drp1_we(drp1_we),
    .drp1_addr(drp1_addr),
    .drp1_di(drp1_di),
    .drp1_do(drp1_do),
    .drp1_rdy(drp1_rdy),
    .drp2_en(drp2_en),
    .drp2_we(drp2_we),
    .drp2_addr(drp2_addr),
    .drp2_di(drp2_di),
    .drp2_do(drp2_do),
    .drp2_rdy(drp2_rdy),
    .drp3_en(drp3_en),
    .drp3_we(drp3_we),
    .drp3_addr(drp3_addr),
    .drp3_di(drp3_di),
    .drp3_do(drp3_do),
    .drp3_rdy(drp3_rdy),
    .drp4_en(drp4_en),
    .drp4_we(drp4_we),
    .drp4_addr(drp4_addr),
    .drp4_di(drp4_di),
    .drp4_do(drp4_do),
    .drp4_rdy(drp4_rdy),
    .drp5_en(drp5_en),
    .drp5_we(drp5_we),
    .drp5_addr(drp5_addr),
    .drp5_di(drp5_di),
    .drp5_do(drp5_do),
    .drp5_rdy(drp5_rdy),
    .drp6_en(drp6_en),
    .drp6_we(drp6_we),
    .drp6_addr(drp6_addr),
    .drp6_di(drp6_di),
    .drp6_do(drp6_do),
    .drp6_rdy(drp6_rdy),
    .drp7_en(drp7_en),
    .drp7_we(drp7_we),
    .drp7_addr(drp7_addr),
    .drp7_di(drp7_di),
    .drp7_do(drp7_do),
    .drp7_rdy(drp7_rdy),
    .drp8_en(drp8_en),
    .drp8_we(drp8_we),
    .drp8_addr(drp8_addr),
    .drp8_di(drp8_di),
    .drp8_do(drp8_do),
    .drp8_rdy(drp8_rdy),
    .drp9_en(),
    .drp9_we(),
    .drp9_addr(),
    .drp9_di(),
    .drp9_do(16'B0),
    .drp9_rdy(1'B0),
    .drp10_en(),
    .drp10_we(),
    .drp10_addr(),
    .drp10_di(),
    .drp10_do(16'B0),
    .drp10_rdy(1'B0),
    .drp11_en(),
    .drp11_we(),
    .drp11_addr(),
    .drp11_di(),
    .drp11_do(16'B0),
    .drp11_rdy(1'B0),
    .drp12_en(),
    .drp12_we(),
    .drp12_addr(),
    .drp12_di(),
    .drp12_do(16'B0),
    .drp12_rdy(1'B0),
    .drp13_en(),
    .drp13_we(),
    .drp13_addr(),
    .drp13_di(),
    .drp13_do(16'B0),
    .drp13_rdy(1'B0),
    .drp14_en(),
    .drp14_we(),
    .drp14_addr(),
    .drp14_di(),
    .drp14_do(16'B0),
    .drp14_rdy(1'B0),
    .drp15_en(),
    .drp15_we(),
    .drp15_addr(),
    .drp15_di(),
    .drp15_do(16'B0),
    .drp15_rdy(1'B0),
    .drp16_en(),
    .drp16_we(),
    .drp16_addr(),
    .drp16_di(),
    .drp16_do(16'B0),
    .drp16_rdy(1'B0),
    .drp17_en(),
    .drp17_we(),
    .drp17_addr(),
    .drp17_di(),
    .drp17_do(16'B0),
    .drp17_rdy(1'B0),
    .drp18_en(),
    .drp18_we(),
    .drp18_addr(),
    .drp18_di(),
    .drp18_do(16'B0),
    .drp18_rdy(1'B0),
    .drp19_en(),
    .drp19_we(),
    .drp19_addr(),
    .drp19_di(),
    .drp19_do(16'B0),
    .drp19_rdy(1'B0),
    .drp20_en(),
    .drp20_we(),
    .drp20_addr(),
    .drp20_di(),
    .drp20_do(16'B0),
    .drp20_rdy(1'B0),
    .drp21_en(),
    .drp21_we(),
    .drp21_addr(),
    .drp21_di(),
    .drp21_do(16'B0),
    .drp21_rdy(1'B0),
    .drp22_en(),
    .drp22_we(),
    .drp22_addr(),
    .drp22_di(),
    .drp22_do(16'B0),
    .drp22_rdy(1'B0),
    .drp23_en(),
    .drp23_we(),
    .drp23_addr(),
    .drp23_di(),
    .drp23_do(16'B0),
    .drp23_rdy(1'B0),
    .drp24_en(),
    .drp24_we(),
    .drp24_addr(),
    .drp24_di(),
    .drp24_do(16'B0),
    .drp24_rdy(1'B0),
    .drp25_en(),
    .drp25_we(),
    .drp25_addr(),
    .drp25_di(),
    .drp25_do(16'B0),
    .drp25_rdy(1'B0),
    .drp26_en(),
    .drp26_we(),
    .drp26_addr(),
    .drp26_di(),
    .drp26_do(16'B0),
    .drp26_rdy(1'B0),
    .drp27_en(),
    .drp27_we(),
    .drp27_addr(),
    .drp27_di(),
    .drp27_do(16'B0),
    .drp27_rdy(1'B0),
    .drp28_en(),
    .drp28_we(),
    .drp28_addr(),
    .drp28_di(),
    .drp28_do(16'B0),
    .drp28_rdy(1'B0),
    .drp29_en(),
    .drp29_we(),
    .drp29_addr(),
    .drp29_di(),
    .drp29_do(16'B0),
    .drp29_rdy(1'B0),
    .drp30_en(),
    .drp30_we(),
    .drp30_addr(),
    .drp30_di(),
    .drp30_do(16'B0),
    .drp30_rdy(1'B0),
    .drp31_en(),
    .drp31_we(),
    .drp31_addr(),
    .drp31_di(),
    .drp31_do(16'B0),
    .drp31_rdy(1'B0)
  );
endmodule
