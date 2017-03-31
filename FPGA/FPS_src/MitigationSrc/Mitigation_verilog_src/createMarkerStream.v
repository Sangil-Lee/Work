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

// Create AXI stream of marker data
// This creates a 'keep-alive' stream to the IOC and provides an end-to-end
// verification of mitigation node firmware, mitigation node software, event
// system, network data transfer and IOC software.

module createMarkerStream #(
    parameter SYSCLK_RATE     = 100000000,
    parameter TIMESTAMP_WIDTH = 64,
    parameter AXI_WIDTH       = 32,
    parameter DMA_COUNT       = 7,
    parameter DEBUG           = "false"
    ) (
    // Everything is in the system clock domain
    input wire                       sysClk,
    input wire                       sysLogEnable,
    input wire [TIMESTAMP_WIDTH-1:0] sysTimestamp,

    (*mark_debug=DEBUG*) output wire [AXI_WIDTH-1:0] sysMarkerTDATA,
    (*mark_debug=DEBUG*) input  wire                 sysMarkerTREADY,
    (*mark_debug=DEBUG*) output reg                  sysMarkerTVALID = 0,
    (*mark_debug=DEBUG*) output reg                  sysMarkerTLAST = 0);

// Send marker every two seconds
localparam SYSCLK_DIVIDER = 2 * SYSCLK_RATE;

reg                 [$clog2(SYSCLK_DIVIDER-1):0] sysClkDivisor = 0;
reg                        [TIMESTAMP_WIDTH-1:0] packetTimestamp;
(*mark_debug=DEBUG*) reg                         dmaStrobe = 0;
(*mark_debug=DEBUG*) reg [$clog2(DMA_COUNT)-1:0] dmaCount = 0;
reg [AXI_WIDTH+TIMESTAMP_WIDTH-1:0] shiftReg;
assign sysMarkerTDATA = shiftReg[TIMESTAMP_WIDTH+:AXI_WIDTH];

always @(posedge sysClk) begin
    if (sysClkDivisor == 0) begin
        sysClkDivisor <= SYSCLK_DIVIDER - 1;
        dmaStrobe <= 1;
    end
    else begin
        sysClkDivisor <= sysClkDivisor - 1;
        dmaStrobe <= 0;
    end
    if (sysMarkerTVALID) begin
        if (sysMarkerTREADY) begin
            dmaCount <= dmaCount - 1;
            shiftReg <= { shiftReg[0+:TIMESTAMP_WIDTH], {AXI_WIDTH{1'b1}} };;
            if (sysMarkerTLAST) begin
                sysMarkerTVALID <= 0;
                sysMarkerTLAST <= 0;
            end
            if (dmaCount == 0) begin
                sysMarkerTLAST <= 1;
            end
        end
    end
    else begin
        if (dmaStrobe && sysLogEnable) begin
            sysMarkerTVALID <= 1;
            shiftReg <= { {AXI_WIDTH{1'b1}}, sysTimestamp };
            dmaCount <= DMA_COUNT - 2;
        end
    end
end
endmodule
