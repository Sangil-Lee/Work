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

// Interlock input signal processing

module inputHander #(
    parameter DATA_WIDTH        = 160,
    parameter INTERLOCK_COUNT   = 64,
    parameter SYSMON_ADDR_WIDTH = 7,
    parameter CLK_PERIOD_PS     = 3200,
    parameter DEBUG             = "false"
    ) (
    input  wire                        clk,
    (* mark_debug = DEBUG *)
    input  wire  [INTERLOCK_COUNT-1:0] interlockPins,
    (* mark_debug = DEBUG *)
    input  wire                  [1:0] switches,

    (* mark_debug = DEBUG *)
    output reg [SYSMON_ADDR_WIDTH-1:0] sysmonAddr = 0,
    (* mark_debug = DEBUG *)
    input  wire                 [15:0] sysmonData,

    (* mark_debug = DEBUG *)
    output reg                         txToggle = 0,
    (* mark_debug = DEBUG *)
    output wire       [DATA_WIDTH-1:0] localData);

// Send accumulated interlock state about every 400 ns
localparam TX_INTERVAL_TICKS = (400 *1000) / CLK_PERIOD_PS;
reg [$clog2(TX_INTERVAL_TICKS)-1:0] txIntervalCounter = TX_INTERVAL_TICKS-1;
reg inputlatchEnable = 0;

// Allow push button override for testing
wire [INTERLOCK_COUNT-1:0] interlockInputs_r =
                    switches[0]  ? 64'hFEDCBA9876543210 :
                    switches[1] ? 64'h0123456789ABCDEF : interlockPins;
reg [INTERLOCK_COUNT-1:0] inputs_m;
reg [INTERLOCK_COUNT-1:0] inputs;
reg [INTERLOCK_COUNT-1:0] inputs_d;
wire[INTERLOCK_COUNT-1:0] inputDiff = inputs ^
                                                      inputs_d;
reg [INTERLOCK_COUNT-1:0] inputTransitions;

// Processed inputs
reg [INTERLOCK_COUNT-1:0] txData;
reg [INTERLOCK_COUNT-1:0] txTransitions;
reg                       txTransitionFlag;

// System monitoring
reg [SYSMON_ADDR_WIDTH-1:0] txSysmonAddr;
reg                  [15:0] txSysmonData;

// Local packet contents
assign localData = {
                 txData, txTransitions,
                 txTransitionFlag, {16-1-SYSMON_ADDR_WIDTH{1'b0}}, txSysmonAddr,
                 txSysmonData };

always @(posedge clk) begin
    inputs_m <= interlockInputs_r;
    inputs   <= inputs_m;
    inputs_d <= inputs;
    if (txIntervalCounter == 0) begin
       txIntervalCounter <= TX_INTERVAL_TICKS - 1;
        inputlatchEnable <= 1;
    end
    else begin
       txIntervalCounter <= txIntervalCounter - 1;
        inputlatchEnable <= 0;
    end
    if (inputlatchEnable) begin
        // Note that new local data are present.
        txToggle <= !txToggle;
        // Latch input state
        txData <= inputs;
        // Latch whether or not there were any
        // transitions during the sampling interval.
        txTransitions <= inputTransitions | inputDiff;
        txTransitionFlag <= |(inputTransitions | inputDiff);
        inputTransitions <= 0;
        // Latch system monitor data
        txSysmonAddr <= sysmonAddr;
        txSysmonData <= sysmonData;
        sysmonAddr <= sysmonAddr + 1;
    end
    else begin
        inputTransitions <= inputTransitions | inputDiff;
    end
end
endmodule
