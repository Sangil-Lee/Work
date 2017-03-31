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

// Very simple single-clock FIFO

module scFIFO #(
    parameter DEPTH=8,
    parameter WIDTH=4
    ) (
    input  wire             clk,
    input  wire             wr,
    input  wire [WIDTH-1:0] din,
    output wire             full,

    input  wire             rd,
    output wire [WIDTH-1:0] dout,
    output wire             empty);

parameter COUNT_WIDTH = $clog2(DEPTH+2);
parameter  ADDR_WIDTH = $clog2(DEPTH);

reg [COUNT_WIDTH-1:0] wrCount = 0, rdCount = 0;
wire [ADDR_WIDTH-1:0] wrAddr, rdAddr;
assign wrAddr = wrCount[ADDR_WIDTH-1:0];
assign rdAddr = rdCount[ADDR_WIDTH-1:0];

assign full = ((wrCount - rdCount) == DEPTH);
assign empty = (wrCount == rdCount);

// Dual-port, non-registered RAM
reg [WIDTH-1:0] dpram [0:(1<<DEPTH)-1];
assign dout = dpram[rdAddr];

always @(posedge clk) begin
    if (wr && !full) begin
        dpram[wrAddr] <= din;
        wrCount <= wrCount + 1;
    end
    if (rd && !empty) begin
        rdCount <= rdCount + 1;
    end
end

endmodule
