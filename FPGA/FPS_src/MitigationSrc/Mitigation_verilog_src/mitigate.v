// MIT License
//
// Copyright (c) 2106 Osprey DCS
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Drive mitigation outputs.
module mitigate #(
    parameter INTERLOCKS_PER_NODE = -1,
    parameter MITIGATION_COUNT    = -1,
    parameter MAX_LINK_COUNT      = -1,
    parameter NODES_PER_LINK      = -1,
    parameter ACQ_INDEX_WIDTH     = -1,
    parameter DEBUG               = "false"
    ) (
    // System clock domain for loading bitmaps and enabling outputs
    input  wire                        sysClk,
    input  wire                 [13:0] sysMemAddress,
    input  wire                 [31:0] sysMemWrData,
    input  wire                        sysMemWrStrobe,
    input  wire [MITIGATION_COUNT-1:0] sysSetToggle,

    // Aurora clock domain for everything else
    // From link multiplexer
    input  wire                           clk,
    (* mark_debug = DEBUG *)
    input  wire                           newDataStrobe,
    (* mark_debug = DEBUG *)
    input  wire     [ACQ_INDEX_WIDTH-1:0] acqIndex,
    (* mark_debug = DEBUG *)
    input  wire [INTERLOCKS_PER_NODE-1:0] inputState,
    (* mark_debug = DEBUG *)
    input  wire [INTERLOCKS_PER_NODE-1:0] inputTransitions,
    (* mark_debug = DEBUG *)
    input  wire [(NODES_PER_LINK*MAX_LINK_COUNT)-1:0] staleNodes,

    (* mark_debug = DEBUG *)
    output reg  [MITIGATION_COUNT-1:0] mitigationOutputs = 0);

// Allow up to 16 mitigation outputs
parameter OUTPUT_CAPACITY = 16;
parameter OUTPUT_CAPACITY_WIDTH = $clog2(OUTPUT_CAPACITY);

// Extract values from memory write address.
// This assumes 64 bits per node, 32 bit writes.
// Type select is 0-"good state", 1-"important".
wire                       sys_wordSel  = sysMemAddress[2];
wire [ACQ_INDEX_WIDTH-1:0] sys_acqIdx   = sysMemAddress[3+:ACQ_INDEX_WIDTH];
wire                       sys_typeSel  = sysMemAddress[3+ACQ_INDEX_WIDTH+:1];
wire [OUTPUT_CAPACITY_WIDTH-1:0] sysMemOutputSelect =
                                       sysMemAddress[3+ACQ_INDEX_WIDTH+1+:
                                                         OUTPUT_CAPACITY_WIDTH];

genvar i;
generate
for (i = 0 ; i < MITIGATION_COUNT ; i = i + 1) begin : mitigateOut

(* mark_debug = (i == 0) ? DEBUG : "false" *)
reg                            newDataStrobe_d;
(* mark_debug = (i == 0) ? DEBUG : "false" *)
wire [INTERLOCKS_PER_NODE-1:0] goodStateBits, importantBits;
(* mark_debug = (i == 0) ? DEBUG : "false" *)
reg  [INTERLOCKS_PER_NODE-1:0] inputState_d, inputTransitions_d;
(* mark_debug = (i == 0) ? DEBUG : "false" *)
reg      [ACQ_INDEX_WIDTH-1:0] acqIndex_d;

(* mark_debug = (i == 0) ? DEBUG : "false" *)
reg setToggle_m = 0, setToggle = 0, setToggle_d = 0;
(* mark_debug = (i == 0) ? DEBUG : "false" *)
reg [9:0] badInputTimer = ~0;

(* mark_debug = (i == 0) ? DEBUG : "false" *)
reg [(NODES_PER_LINK*MAX_LINK_COUNT)-1:0] importantNodes;

// Here's where the real fast protection operations are performed
always @(posedge clk) begin
    // Allow for memory latency
    newDataStrobe_d    <= newDataStrobe;
    inputState_d       <= inputState;
    inputTransitions_d <= inputTransitions;
    acqIndex_d         <= acqIndex;
    if (newDataStrobe_d &&
         ((((inputState_d^goodStateBits)|inputTransitions_d)&importantBits)!=0))
                                                                           begin
        badInputTimer <= ~0;
    end
    else begin
        if (badInputTimer != 0) begin
            badInputTimer <= badInputTimer - 1;
        end
    end

    setToggle_m <= sysSetToggle[i];
    setToggle   <= setToggle_m;
    setToggle_d <= setToggle;
    if ((badInputTimer != 0) || ((staleNodes & importantNodes) != 0)) begin
        mitigationOutputs[i] <= 0;
    end
    else if (setToggle != setToggle_d) begin
        mitigationOutputs[i] <= 1;
    end
end

// Keep track of nodes that have one or more important input
// Assume that sys_wordSel==0 word is written first
always @(posedge sysClk) begin
    if (sysMemWrStrobe && (sysMemOutputSelect==i) && (sys_typeSel==1)) begin
        if (sys_wordSel == 0) begin
            if (sysMemWrData == 0) begin
                importantNodes[sys_acqIdx] <= 0;
            end
            else begin
                importantNodes[sys_acqIdx] <= 1;
            end
        end
        else begin
            if (sysMemWrData != 0) begin
                importantNodes[sys_acqIdx] <= 1;
            end
        end
    end
end

mitigationDPRAM goodStateDPRAM (
    .clka(sysClk),
    .wea(sysMemWrStrobe && (sysMemOutputSelect==i) && (sys_typeSel==0)),
    .addra({sys_acqIdx, sys_wordSel}),
    .dina(sysMemWrData),
    .clkb(clk),
    .addrb(acqIndex),
    .doutb(goodStateBits));

mitigationDPRAM importantDPRAM (
    .clka(sysClk),
    .wea(sysMemWrStrobe && (sysMemOutputSelect==i) && (sys_typeSel==1)),
    .addra({sys_acqIdx, sys_wordSel}),
    .dina(sysMemWrData),
    .clkb(clk),
    .addrb(acqIndex),
    .doutb(importantBits));

end
endgenerate
endmodule

module mitigationDPRAM #(
    parameter WRITE_DATA_WIDTH  = 32,
    parameter WRITE_ADDR_WIDTH  = 7,
    parameter BITMAP_DATA_WIDTH = 64,
    parameter BITMAP_ADDR_WIDTH = 6
    ) (
    input                              clka,
    input                              wea,
    input       [WRITE_ADDR_WIDTH-1:0] addra,
    input       [WRITE_DATA_WIDTH-1:0] dina,
    input                              clkb,
    input      [BITMAP_ADDR_WIDTH-1:0] addrb,
    output reg [BITMAP_DATA_WIDTH-1:0] doutb);

localparam RAM_WORD32_COUNT = 1 << BITMAP_ADDR_WIDTH;

reg [WRITE_DATA_WIDTH-1:0] rambufLo[0:RAM_WORD32_COUNT-1];
reg [WRITE_DATA_WIDTH-1:0] rambufHi[0:RAM_WORD32_COUNT-1];

wire                         word32Select = addra[0];
wire [BITMAP_ADDR_WIDTH-1:0] word32Index = addra[1+:BITMAP_ADDR_WIDTH];

always @(posedge clka) begin
    if (wea && (word32Select == 0)) rambufLo[word32Index] <= dina;
    if (wea && (word32Select == 1)) rambufHi[word32Index] <= dina;
end
always @(posedge clkb) begin
    doutb <= { rambufHi[addrb], rambufLo[addrb] };
end
endmodule
