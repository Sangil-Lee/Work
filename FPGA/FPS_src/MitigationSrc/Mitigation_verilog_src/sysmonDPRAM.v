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

// Storage and readout for acquisition node system monitors

module sysmonDPRAM #(
    parameter ADDR_WIDTH = 13,
    parameter DATA_WIDTH = 16,
    parameter DEBUG      = "false"
    ) (
    input  wire                                          wclk,
    input  wire                                          wen,
    (* mark_debug = DEBUG *) input wire [ADDR_WIDTH-1:0] waddr,
    (* mark_debug = DEBUG *) input wire [DATA_WIDTH-1:0] wdata,

    input  wire                                           clk,
    (* mark_debug = DEBUG *) input  wire [ADDR_WIDTH-1:0] addr,
    (* mark_debug = DEBUG *) output reg  [DATA_WIDTH-1:0] data);

reg [DATA_WIDTH-1:0] dpRAM [0:(1<<ADDR_WIDTH)-1];

always @(posedge wclk) begin
    if (wen) dpRAM[waddr] <= wdata;
end
always @(posedge clk) begin
    data <= dpRAM[addr];
end

endmodule
