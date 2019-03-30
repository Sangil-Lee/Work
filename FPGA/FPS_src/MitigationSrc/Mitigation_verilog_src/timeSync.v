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

// Handle ping requests and generate time synchronization packets
//      0: Header
//      1: Seconds [31:16]
//      2: Seconds [15:0]
//      3: Ticks [31:16]
//      4: Ticks [15:0]
//      5: Checksum

module timeSync #(
    parameter AXI_WIDTH       = 16,
    parameter TIMESTAMP_WIDTH = 64,
    parameter DEBUG           = "false"
    ) (
    input  wire                      clk,

    (* mark_debug = DEBUG *)
    input  wire      [AXI_WIDTH-1:0] incomingTDATA,
    (* mark_debug = DEBUG *)
    input  wire                      incomingTVALID,
    (* mark_debug = DEBUG *)
    input  wire                      incomingTLAST,

    (* mark_debug = DEBUG *)
    output wire      [AXI_WIDTH-1:0] outgoingTDATA,
    (* mark_debug = DEBUG *)
    output wire                      outgoingTVALID,
    (* mark_debug = DEBUG *)
    output wire                      outgoingTLAST,
    (* mark_debug = DEBUG *)
    input  wire                      outgoingTREADY,

    input wire [TIMESTAMP_WIDTH-1:0] timestamp,
    input wire                       secondsMarkerToggle);

localparam PACKET_WORDCOUNT = 6;
localparam PACKET_COUNTER_WIDTH = 3;

// System time
reg secondsMarkerMatch = 0;
reg sendRequested = 0, sending = 0;

// Latency measurement response support.
`include "pkInfo.v"
reg nextIncomingIsHeader = 0;
reg responseRequested = 0, responding = 0;

// Outgoing time synchronization packet
localparam SHIFTREG_WIDTH = PACKET_WORDCOUNT * (1 + AXI_WIDTH);
reg [AXI_WIDTH-(PKH_TYPE_WIDTH+PKH_TTL_WIDTH)-1:0] seqno = 0;
reg [SHIFTREG_WIDTH-1:0] txShift;
reg      [AXI_WIDTH-1:0] txChecksum;
wire     [AXI_WIDTH-1:0] txHeader, txWord;
wire                     txLast;
assign txHeader = { seqno, PKTYPE_TIME, PKH_INITIAL_TTL };
assign txWord = txShift[(PACKET_WORDCOUNT-1)*(1 + AXI_WIDTH)+:AXI_WIDTH];
assign txLast = txShift[(PACKET_WORDCOUNT-1)*(1 + AXI_WIDTH)+AXI_WIDTH+:1];
assign outgoingTDATA = sending ? (txLast ? txChecksum : txWord) : PKHEADER_PING;
assign outgoingTLAST = sending ? txLast : 1'b1;
assign outgoingTVALID = sending || responding;

always @(posedge clk) begin
    // Keep track of time
    if (secondsMarkerToggle != secondsMarkerMatch) begin
        secondsMarkerMatch <= !secondsMarkerMatch;
        if (!sendRequested) begin
            sendRequested <= 1;
        end
    end

    // Watch for ping requests
    if (incomingTVALID) begin
        if (nextIncomingIsHeader
         && !responseRequested
         && (incomingTDATA[PKH_TYPE_WIDTH+PKH_TTL_WIDTH-1:0] ==
                                            {PKTYPE_MON,PKH_INITIAL_TTL})) begin
            responseRequested <= 1;
        end
        if (incomingTLAST) nextIncomingIsHeader <= 1;
        else               nextIncomingIsHeader <= 0;
    end

    // Transmission state machine
    if (responding) begin
        if (outgoingTREADY) begin
            responding <= 0;
        end
    end
    else if (sending) begin
        if (outgoingTREADY) begin
            txShift <= {txShift[SHIFTREG_WIDTH-(1+AXI_WIDTH)-1:0],
                                                           {1+AXI_WIDTH{1'bx}}};
            txChecksum <= txChecksum + txWord;
            if (txLast) begin
                sending <= 0;
                seqno <= seqno + 1;
            end
        end
    end
    else begin
        if (responseRequested) begin
            responseRequested <= 0;
            responding <= 1;
        end
        else if (sendRequested) begin
            sendRequested <= 0;
            sending <= 1;
            txChecksum <= 0;
            txShift <= {
                  1'b0, txHeader,
                  1'b0, timestamp[63:48],
                  1'b0, timestamp[47:32],
                  1'b0, timestamp[31:16],
                  1'b0, timestamp[15:0],
                  1'b1, {AXI_WIDTH{1'bx}}};
        end
    end
end
endmodule
