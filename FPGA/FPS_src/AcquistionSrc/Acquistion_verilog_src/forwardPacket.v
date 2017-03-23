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

// Forward packet with updated header and checksum
// Note that we do not regenerate the checksum.  This ensures that a
// bad packet is detected by the mitigation node.

module forwardPacket #(
    parameter AXI_WIDTH  = 16,
    parameter      DEBUG = "false"
    ) (
    input  wire                                          clk,

    (* mark_debug = DEBUG *) input  wire [AXI_WIDTH-1:0] incomingTDATA,
    (* mark_debug = DEBUG *) input  wire                 incomingTVALID,
    (* mark_debug = DEBUG *) input  wire                 incomingTLAST,

    (* mark_debug = DEBUG *) output reg  [AXI_WIDTH-1:0] outgoingTDATA,
    (* mark_debug = DEBUG *) output reg                  outgoingTVALID,
    (* mark_debug = DEBUG *) output reg                  outgoingTLAST);

`include "pkInfo.v"

(* mark_debug = DEBUG *) reg forwarding = 0, startOfPacket = 0;

wire [AXI_WIDTH-1:0] headerWithUpdatedTTL, checksumWithUpdatedTTL;
wire [PKH_TTL_WIDTH-1:0] incomingTTL = incomingTDATA[PKH_TTL_WIDTH-1:0];
wire [PKH_TTL_WIDTH-1:0] updatedTTL = incomingTTL - 1;
assign headerWithUpdatedTTL = {
                incomingTDATA[AXI_WIDTH-1:PKH_TTL_WIDTH], updatedTTL };
assign checksumWithUpdatedTTL = incomingTDATA[AXI_WIDTH-1:0] - 1;
wire [AXI_WIDTH-1:0] dataMUX = incomingTLAST ? checksumWithUpdatedTTL :
                               startOfPacket ? headerWithUpdatedTTL   :
                                               incomingTDATA;
reg [4:0] watchdog = 0;
reg       timeout = 0;

always @(posedge clk) begin
    outgoingTDATA <= dataMUX;
    outgoingTLAST <= incomingTLAST || timeout;
    if (incomingTVALID || timeout) begin
        if (forwarding) begin
            outgoingTVALID <= 1;
            if (incomingTLAST || timeout) begin
                forwarding <= 0;
                timeout <= 0;
            end
        end
        else begin
            if (startOfPacket && (incomingTTL != 0)) begin
                watchdog <= ~0;
                timeout <= 0;
                forwarding <= 1;
                outgoingTVALID <= 1;
            end
        end
        if (incomingTLAST) begin
            startOfPacket <= 1;
        end
        else begin
            startOfPacket <= 0;
        end
    end
    else begin
        outgoingTVALID <= 0;
        if (watchdog) begin
            watchdog <= watchdog - 1;
        end
        else if (forwarding) begin
            timeout <= 1;
        end
    end
end

endmodule
