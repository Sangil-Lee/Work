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

//
// Forward data from one clock domain to another
//
module forwardData #(
    parameter DATA_WIDTH = 32) (
    input  wire                  inClk,
    input  wire [DATA_WIDTH-1:0] inData,
    input  wire                  outClk,
    output reg  [DATA_WIDTH-1:0] outData);

//
// Input clock domain
//
reg [DATA_WIDTH-1:0] inLatch;
reg inReq = 0;
reg inAck_m = 0, inAck = 0;

always @(posedge inClk) begin
    if (inReq == inAck) begin
        inReq <= !inReq;
        inLatch <= inData;
    end
    inAck_m <= outReq_d;
    inAck <= inAck_m;
end

//
// Output clock domain
//
reg outReq_m = 0, outReq = 0, outReq_d = 0;
always @(posedge outClk) begin
    outReq_m <= inReq;
    outReq <= outReq_m;
    outReq_d <= outReq;
    if (outReq != outReq_d) begin
        outData <= inLatch;
    end
end

endmodule
