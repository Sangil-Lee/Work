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

// Create AXI stream of logging data

module createLOGstream #(
    parameter MITIGATION_COUNT    = 4,
    parameter TIMESTAMP_WIDTH     = 64,
    parameter INTERLOCKS_PER_NODE = 64,
    parameter AXI_WIDTH           = 32,
    parameter ACQ_INDEX_WIDTH     = 3,
    parameter DEBUG               = "false"
    ) (
    // Log controls and output AXI stream are in system clock domain.
    // Inputs are in aurora clock domain
    input  wire                        sysClk,
    input  wire                        sysLogEnable,
    input  wire                        sysLogForce,

    // From other than link multiplexer
    input  wire                        clk,
    input  wire  [TIMESTAMP_WIDTH-1:0] timestamp,
    input  wire [MITIGATION_COUNT-1:0] outputState,

    // From link multiplexer
    (*mark_debug=DEBUG*) input  wire                           newDataStrobe,
    (*mark_debug=DEBUG*) input  wire                           transitionFlag,
    (*mark_debug=DEBUG*) input  wire     [ACQ_INDEX_WIDTH-1:0] acqIndex,
                         input  wire [INTERLOCKS_PER_NODE-1:0] inputState,
                         input  wire [INTERLOCKS_PER_NODE-1:0] inputTransitions,

    // To DMA logic
    (*mark_debug=DEBUG*) output reg [AXI_WIDTH-1:0] sysLogTDATA = 0,
    (*mark_debug=DEBUG*) input  wire                sysLogTREADY,
    (*mark_debug=DEBUG*) output reg                 sysLogTVALID = 0,
    (*mark_debug=DEBUG*) output reg                 sysLogTLAST = 0);

localparam MAX_OUTPUTS = 16;
localparam FIFO_WIDTH = 1 + ACQ_INDEX_WIDTH + MAX_OUTPUTS +
                        TIMESTAMP_WIDTH + 2 * INTERLOCKS_PER_NODE;
localparam HEADER_PAD_WIDTH = AXI_WIDTH - (FIFO_WIDTH % AXI_WIDTH);
localparam DMA_COUNT = (FIFO_WIDTH + AXI_WIDTH - 1) / AXI_WIDTH;
localparam ACQ_CAPACITY = 1 << ACQ_INDEX_WIDTH;

///////////////////////////////////////////////////////////////////////////////
// FIFO incoming data since send values to DMA engine requires multiple cycles.

// Write to FIFO only when processor has allowed it
reg enable_m = 0, enable = 0;

// Force a write of values from all nodes on request from processor
reg auForce_m = 0, auForce = 0, auForce_d = 0;
reg [ACQ_CAPACITY-1:0] forceLog = 0;

// Force a write whenever mitigation values change
reg [MITIGATION_COUNT-1:0] outputState_stashed = 0;
(*mark_debug=DEBUG*) reg   outputChanged = 0;

(*mark_debug=DEBUG*) wire fifoEMPTY;
(*mark_debug=DEBUG*) reg fifoWREN;
(*mark_debug=DEBUG*) reg [FIFO_WIDTH-1:0] fifoIN;

always @(posedge clk) begin
    enable_m <= sysLogEnable;
    enable   <= enable_m;
    auForce_m <= sysLogForce;
    auForce   <= auForce_m;
    auForce_d <= auForce;
    if (auForce && !auForce_d) begin
        forceLog <= {ACQ_CAPACITY{1'b1}};
    end
    else if (enable && newDataStrobe) begin
        forceLog[acqIndex] <= 0;
    end
    if (enable && ((newDataStrobe && (transitionFlag || forceLog[acqIndex])) 
                 || outputChanged)) begin
        fifoWREN <= 1;
        outputState_stashed <= outputState;
        outputChanged <= 0;
    end
    else begin
        fifoWREN <= 0;
        if (outputState != outputState_stashed) begin
            outputChanged <= 1;
        end
    end
    fifoIN <= { newDataStrobe, acqIndex,
                {MAX_OUTPUTS-MITIGATION_COUNT{1'b0}}, outputState,
                timestamp, inputState, inputTransitions };
end

reg syslogStart = 0;
(*mark_debug=DEBUG*) wire fifoRDEN = !fifoEMPTY&&!syslogStart&&!sysLogTVALID;
wire [FIFO_WIDTH-1:0] fifoOUT;

logFIFO logFIFO (.rst(1'b0),
                 .wr_clk(clk),
                 .rd_clk(sysClk),
                 .din(fifoIN),
                 .wr_en(fifoWREN),
                 .rd_en(fifoRDEN),
                 .dout(fifoOUT),
                 .full(),
                 .empty(fifoEMPTY));

///////////////////////////////////////////////////////////////////////////////
// Convert FIFO output to AXI stream
(*mark_debug=DEBUG*) reg [$clog2(DMA_COUNT)-1:0] dmaCount = 0;

always @(posedge sysClk) begin
    if (sysLogTVALID) begin
        if (sysLogTREADY) begin
            if (sysLogTLAST) begin
                sysLogTVALID <= 0;
            end
            else begin
                dmaCount <= dmaCount - 1;
                sysLogTDATA <= fifoOUT[dmaCount*AXI_WIDTH+:AXI_WIDTH];
                if (dmaCount == 0) begin
                    sysLogTLAST <= 1;
                end
            end
        end
    end
    else if (syslogStart) begin
        syslogStart <= 0;
        sysLogTVALID <= 1;
        sysLogTLAST <= 0;
        sysLogTDATA <= { {HEADER_PAD_WIDTH{1'b0}},
                          fifoOUT[FIFO_WIDTH-1:(DMA_COUNT-1)*AXI_WIDTH] };
        dmaCount <= DMA_COUNT - 2;
    end
    else if (fifoRDEN) begin
        syslogStart <= 1;
    end
end

endmodule
