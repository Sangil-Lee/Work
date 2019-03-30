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

// Forge data packet containing this node's data
//
//      0: Header
//      1: Input state [63:48]
//      2: Input state [47:32]
//      3: Input state [31:16]
//      4: Input state [15:0]
//      5: Transition present [63:48]
//      6: Transition present [47:32]
//      7: Transition present [31:16]
//      8: Transition present [15:0]
//      9: System monitoring adddress
//     10: System monitoring data
//     11: Checksum
//

module createLocalPacket #(
    parameter  AXI_WIDTH = 16,
    parameter DATA_WIDTH = 10*AXI_WIDTH,
    parameter DEBUG      = "false"
    ) (
    input  wire                   clk,
    (* mark_debug = DEBUG *) input  wire  [DATA_WIDTH-1:0] localData,
    (* mark_debug = DEBUG *) input  wire                   localDataToggle,

    (* mark_debug = DEBUG *) output reg    [AXI_WIDTH-1:0] localTDATA,
    (* mark_debug = DEBUG *) output reg                    localTLAST,
    (* mark_debug = DEBUG *) output reg                    localTVALID);

`include "pkInfo.v"

reg                                    localDataMatch = 0;
reg [$clog2(PKTYPE_INP_WORDCOUNT)-1:0] payloadRemaining;
reg              [PKH_SEQNO_WIDTH-1:0] seqno = 0;
reg                   [DATA_WIDTH-1:0] shiftReg;
reg                    [AXI_WIDTH-1:0] checksum;
wire [AXI_WIDTH-1:0] header = { PKH_MAGIC, seqno, PKTYPE_INP, PKH_INITIAL_TTL };
wire [AXI_WIDTH-1:0] shiftTop = shiftReg[DATA_WIDTH-1:DATA_WIDTH-AXI_WIDTH];

// There's enough of a FIFO downstream that we can ignore TREADY.
always @(posedge clk) begin
    if (localTVALID) begin
        payloadRemaining <= payloadRemaining - 1;
        shiftReg <= {shiftReg[DATA_WIDTH-AXI_WIDTH-1:0], {AXI_WIDTH{1'bx}}};
        checksum <= checksum + shiftTop;
        if (localTLAST) begin
            localTVALID <= 0;
        end
        if (payloadRemaining == 0) begin
            localTLAST <= 1;
            localTDATA <= checksum;
        end
        else begin
            localTDATA <= shiftTop;
        end
    end
    else if (localDataToggle != localDataMatch) begin
        localDataMatch <= !localDataMatch;
        shiftReg <= localData;
        localTLAST <= 0;
        localTVALID <= 1;
        localTDATA <= header;
        checksum <= header;
        seqno <= seqno + 1;
        payloadRemaining <= PKTYPE_INP_WORDCOUNT - 2;
    end
end

endmodule
