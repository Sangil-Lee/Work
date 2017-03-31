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

// Handle incoming INP packets
// We can get away with a simple latch on the output (no need for a FIFO)
// since we know that the value will be read by the downstream logic faster
// than another incoming packet can arrive.
// Each link is checked every 8th cycle and an INP packet has 12 words:
//      0: Header
//      1: Input state [63:48]
//      2: Input state [47:32]
//      3: Input state [31:16]
//      4: Input state [15:0]
//      5: Transition present [63:48]
//      6: Transition present [47:32]
//      7: Transition present [31:16]
//      8: Transition present [15:0]
//      9: System monitoring adddress (MSB is 'transitions present' flag)
//     10: System monitoring data
//     11: Checksum
//

module receiveINPpacket #(
    parameter MAX_LINK_COUNT      = -1,
    parameter AURORA_WIDTH        = 16,
    parameter INTERLOCKS_PER_NODE = 64,
    parameter NODES_PER_LINK      = 5,
    parameter NODE_INDEX_WIDTH    = $clog2(NODES_PER_LINK),
    parameter SYSMON_ADDR_WIDTH   = 7,
    parameter DEBUG               = "false"
    ) (
    input  wire clk,

    (* mark_debug = DEBUG *) input wire [AURORA_WIDTH-1:0] incomingTDATA,
    (* mark_debug = DEBUG *) input wire                    incomingTVALID,
    (* mark_debug = DEBUG *) input wire                    incomingTLAST,

    // Keep track of data arrival
    (* mark_debug = DEBUG *) output reg [NODES_PER_LINK-1:0] staleNodes =
                                                         {NODES_PER_LINK{1'b1}},

    // Note that new data are available
    (* mark_debug = DEBUG *) output reg newDataToggle = 0,

    // Packet contents
    output reg [INTERLOCKS_PER_NODE-1:0] inputState,
    output reg [INTERLOCKS_PER_NODE-1:0] inputTransitions,
    output reg                           transitionFlag,

    // Data indices
    (* mark_debug = DEBUG *) output reg  [NODE_INDEX_WIDTH-1:0] nodeIndex,
    (* mark_debug = DEBUG *) output reg [SYSMON_ADDR_WIDTH-1:0] sysmonIndex,

    // To system monitoring logic
    (* mark_debug = DEBUG *) output reg [AURORA_WIDTH-1:0] sysmonTDATA);

`include "pkInfo.v"

// Incoming packet
// First payload word is stored in most significant bits of rxPayload.
localparam INP_PAYLOAD_WORDCOUNT = PKTYPE_INP_WORDCOUNT - 2;
localparam INP_PAYLOAD_BITCOUNT = INP_PAYLOAD_WORDCOUNT * AURORA_WIDTH;
localparam PAYLOAD_STATE_BIT_INDEX = INP_PAYLOAD_BITCOUNT -
                                     INTERLOCKS_PER_NODE;
localparam PAYLOAD_TRANSITIONS_BIT_INDEX = PAYLOAD_STATE_BIT_INDEX -
                                           INTERLOCKS_PER_NODE;
localparam PAYLOAD_SYSMON_ADDR_BIT_INDEX = PAYLOAD_TRANSITIONS_BIT_INDEX -
                                           AURORA_WIDTH;
localparam PAYLOAD_SYSMON_DATA_BIT_INDEX = PAYLOAD_SYSMON_ADDR_BIT_INDEX -
                                           AURORA_WIDTH;

// Incoming packet
// First payload word is stored in most significant bits of rxPayload.
(* mark_debug = DEBUG *) reg nextIncomingIsHeader=0, rxActive=0, rxActive_d=0;
(* mark_debug = DEBUG *) reg [$clog2(PKTYPE_INP_WORDCOUNT)-1:0] rxRemaining;
(* mark_debug = DEBUG *) reg [AURORA_WIDTH-1:0] rxHeader, rxChecksum;
(* mark_debug = DEBUG *) reg [INP_PAYLOAD_BITCOUNT-1:0] rxPayload;
(* mark_debug = DEBUG *) reg goodChecksum = 0;
                         reg newDataToggle_d = 0;

always @(posedge clk) begin
    rxActive_d <= rxActive;
    newDataToggle_d <= newDataToggle;
    if (rxActive_d && !rxActive) begin
        inputState <= rxPayload[PAYLOAD_STATE_BIT_INDEX+:INTERLOCKS_PER_NODE];
        inputTransitions <=
                  rxPayload[PAYLOAD_TRANSITIONS_BIT_INDEX+:INTERLOCKS_PER_NODE];
        transitionFlag <= rxPayload[PAYLOAD_SYSMON_ADDR_BIT_INDEX+15+:1];
        nodeIndex <= PKH_INITIAL_TTL - rxHeader[0+:PKH_TTL_WIDTH];
        sysmonIndex <= rxPayload[PAYLOAD_SYSMON_ADDR_BIT_INDEX+:
                                                          PK_SYSMON_ADDR_WIDTH];
        sysmonTDATA <= rxPayload[PAYLOAD_SYSMON_DATA_BIT_INDEX+:AURORA_WIDTH];
        // Moving the checksum test here works around a bug in
        // the 2016.1 toolchain used to process the firmware!
        if (goodChecksum) newDataToggle <= !newDataToggle;
    end
    if (incomingTVALID) begin
        nextIncomingIsHeader <= incomingTLAST;
        if (rxActive) begin
            rxRemaining <= rxRemaining - 1;
            rxChecksum <= rxChecksum + incomingTDATA;
            goodChecksum <= (incomingTDATA == rxChecksum);
            if (rxRemaining == 0) begin
                rxActive <= 0;
            end
            else begin
                rxPayload[(rxRemaining-1)*AURORA_WIDTH+:AURORA_WIDTH] <=
                                                                  incomingTDATA;
            end
        end
        else if (nextIncomingIsHeader
              && (incomingTDATA[0+:PKH_TTL_WIDTH] <= PKH_INITIAL_TTL)
              && (incomingTDATA[PKH_TTL_WIDTH+:PKH_TYPE_WIDTH] == PKTYPE_INP)
              && (incomingTDATA[AURORA_WIDTH-PKH_MAGIC_WIDTH+:PKH_MAGIC_WIDTH]==
                                                               PKH_MAGIC)) begin
            rxRemaining <= PKTYPE_INP_WORDCOUNT - 2;
            rxHeader <= incomingTDATA;
            rxChecksum <= incomingTDATA;
            rxActive <= 1;
        end
    end
end

// One watchdog timer per node
genvar i;
generate
for (i = 0 ; i <= PKH_INITIAL_TTL ; i = i + 1) begin : perNode
reg [9:0] timeoutCounter = 0;
always @(posedge clk) begin
    if ((newDataToggle != newDataToggle_d) && (nodeIndex == i)) begin
        timeoutCounter <= ~0;
        staleNodes[i] <= 0;
    end
    else if (timeoutCounter != 0) begin
        timeoutCounter <= timeoutCounter - 1;
    end
    else begin
        staleNodes[i] <= 1;
    end
end
end /* for */
endgenerate

endmodule
