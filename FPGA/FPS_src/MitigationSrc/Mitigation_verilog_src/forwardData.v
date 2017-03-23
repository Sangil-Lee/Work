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

// Forward data from one clock domain to another.
// Very simple-minded.  No data transfer strobes.

module forwardData #(
    parameter WIDTH = 32
    ) (
    input  wire             srcClk,
    input  wire [WIDTH-1:0] srcData,

    input  wire             dstClk,
    output reg  [WIDTH-1:0] dstData);

reg srcToggle = 0, srcMatch_m = 0, srcMatch = 0;
reg [WIDTH-1:0] srcLatch;
reg dstToggle = 0, dstMatch_m = 0, dstMatch = 0;

always @(posedge srcClk) begin
    srcMatch_m <= dstToggle;
    srcMatch   <= srcMatch_m;
    if (srcToggle == srcMatch) begin
        srcLatch <= srcData;
        srcToggle <= !srcToggle;
    end
end

always @(posedge dstClk) begin
    dstMatch_m <= srcToggle;
    dstMatch   <= dstMatch_m;
    if (dstToggle != dstMatch) begin
        dstData <= srcLatch;
        dstToggle <= !dstToggle;
    end
end

endmodule
