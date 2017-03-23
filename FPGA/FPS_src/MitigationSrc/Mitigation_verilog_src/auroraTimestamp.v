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

// Provide mitigation transition time stamps to the nanosecond level.
// Nets with names beginning with evr are in EVR clock domain seconds,
// others are in Aurora clock domain.

module auroraTimestamp #(
    parameter TIMESTAMP_WIDTH = 64,
    parameter CM_DEBUG        = "false",
    parameter DEBUG           = "false"
    ) (
    input  wire                         clk,
    input  wire                         evrPPSmarker,
    input  wire [TIMESTAMP_WIDTH/2-1:0] evrSeconds,
    (* mark_debug = DEBUG *)
    output reg                          secondsMarkerToggle = 0,
    (* mark_debug = DEBUG *)
    output wire   [TIMESTAMP_WIDTH-1:0] timestamp);

localparam NOMINAL_CLOCK_RATE = 312500000;
localparam SECONDS_WIDTH = TIMESTAMP_WIDTH/2;
localparam TICKS_WIDTH   = TIMESTAMP_WIDTH/2;

// System time
localparam TICK_UPDATE_WIDTH = 26;
reg [SECONDS_WIDTH-1:0] seconds;

// Most significant bit of tickUpdate is least significant bit of ticks.
localparam TICK_ACCUMULATOR_WIDTH = TICKS_WIDTH + TICK_UPDATE_WIDTH - 1;
reg [TICK_ACCUMULATOR_WIDTH-1:0] tickAccumulator;
reg [TICK_UPDATE_WIDTH-1:0] tickUpdate = {1'b1, {TICK_UPDATE_WIDTH-1{1'b0}}};
wire [TICKS_WIDTH-1:0] ticks;
assign ticks = tickAccumulator[TICK_ACCUMULATOR_WIDTH-1:TICK_UPDATE_WIDTH-1];
assign timestamp = { seconds, ticks };

// Measure frequency of local clock
localparam CLOCK_OFFSET_WIDTH = 16;
localparam CLOCK_COUNTER_VALID_FLOOR = NOMINAL_CLOCK_RATE - 
                                                ((1<<(CLOCK_OFFSET_WIDTH-1))-1);
localparam CLOCK_COUNTER_VALID_CEILING = NOMINAL_CLOCK_RATE + 
                                                ((1<<(CLOCK_OFFSET_WIDTH-1))-1);
parameter CLOCK_COUNTER_WIDTH = $clog2(CLOCK_COUNTER_VALID_CEILING+1);
(* mark_debug = CM_DEBUG *) reg [CLOCK_COUNTER_WIDTH-1:0] clockCounter;
(* mark_debug = CM_DEBUG *) reg  [CLOCK_OFFSET_WIDTH-1:0] clockOffsetRaw;
(* mark_debug = CM_DEBUG *) wire [CLOCK_OFFSET_WIDTH-1:0] clockOffset;
(* mark_debug = CM_DEBUG *) reg  clockCounterInRange = 0;
(* mark_debug = CM_DEBUG *) reg  clockOffsetFilterEnable = 0;
(* mark_debug = CM_DEBUG *) reg  clockOffsetDividerEnable = 0;
(* mark_debug = CM_DEBUG *) wire clockOffsetQuotientValid;
(* mark_debug = CM_DEBUG *) wire [TICK_UPDATE_WIDTH-1:0] clockOffsetQuotient; 

reg PPSmarker_m;
(* mark_debug = DEBUG *) reg PPSmarker, PPSmarker_d;

// Keep track of time to Aurora clock resolution
// Measure local clock frequency and adjust tick update value
always @(posedge clk) begin
    PPSmarker_m <= evrPPSmarker;
    PPSmarker   <= PPSmarker_m;
    PPSmarker_d <= PPSmarker;
    if (PPSmarker && !PPSmarker_d) begin
        secondsMarkerToggle <= !secondsMarkerToggle;
        seconds <= evrSeconds;
        tickAccumulator <= 0;
        clockCounter <= 1;
        clockCounterInRange <= 0;
        clockOffsetRaw <= clockCounter - NOMINAL_CLOCK_RATE;
        if (clockCounterInRange) begin
            clockOffsetFilterEnable <= 1;
        end
        else begin
            clockOffsetFilterEnable <= 0;
        end
        clockOffsetDividerEnable <= 0;
    end
    else if (ticks != {TICKS_WIDTH{1'b1}}) begin
        tickAccumulator <= tickAccumulator +
               {{TICK_ACCUMULATOR_WIDTH-TICK_UPDATE_WIDTH-1{1'b0}}, tickUpdate};
        if (clockCounter != 0) begin
            clockCounter <= clockCounter + 1;
            if (clockCounter == CLOCK_COUNTER_VALID_FLOOR)
                clockCounterInRange <= 1;
            else if (clockCounter == CLOCK_COUNTER_VALID_CEILING)
                clockCounterInRange <= 0;
        end
        if (clockOffsetFilterEnable) begin
            clockOffsetFilterEnable <= 0;
            clockOffsetDividerEnable <= 1;
        end
        else if (clockOffsetDividerEnable) begin
            if (clockOffsetQuotientValid) begin
                clockOffsetDividerEnable <= 0;
                tickUpdate <= clockOffsetQuotient;
            end
        end
    end
end

// Smooth out measurement variations
lowpass #(.WIDTH(CLOCK_OFFSET_WIDTH),
          .L2_ALPHA(6))
 clockOffsetLowpass (
    .clk(clk),
    .en(clockOffsetFilterEnable),
    .u(clockOffsetRaw),
    .y(clockOffset));

// Divide nominal clock rate by actual to get tick update value
offsetDivider #(.OFFSET_WIDTH(CLOCK_OFFSET_WIDTH),
                .QUOTIENT_WIDTH(TICK_UPDATE_WIDTH))
    offsetDivider (
        .clk(clk),
        .start(clockOffsetDividerEnable),
        .offset(clockOffset),
        .quotientValid(clockOffsetQuotientValid),
        .quotient(clockOffsetQuotient));

endmodule

//
// First-order low-pass
// Pole at z = 1 - 1 / (1 << L2_ALPHA)
// Unsigned in/out
//
module lowpass #(parameter WIDTH    = 30,
                 parameter L2_ALPHA = 16)(
                     input                   clk,
                     input                   en,
                     input       [WIDTH-1:0] u,
                     output wire [WIDTH-1:0] y);

parameter SUM_WIDTH = WIDTH + L2_ALPHA;
reg [SUM_WIDTH-1:0] sum = 0;
assign y = sum[SUM_WIDTH-1:SUM_WIDTH-WIDTH];
always @(posedge clk)
begin
    if (en) begin
        sum <= { {SUM_WIDTH-WIDTH{1'b0}}, u[WIDTH-1:0]} + sum -
               { {L2_ALPHA{1'b0}}, sum[SUM_WIDTH-1:L2_ALPHA]};
    end
end

endmodule

//
// Compute N/(N+O) to specified number of bits
// Simple long division
//
module offsetDivider #(
    parameter DIVIDEND       = 312500000,
    parameter OFFSET_WIDTH   = 16,
    parameter QUOTIENT_WIDTH = 24
    ) (
    input  wire                     clk,
    input  wire                     start,
    input  wire  [OFFSET_WIDTH-1:0] offset,
    output reg                      quotientValid,
    output reg [QUOTIENT_WIDTH-1:0] quotient);

parameter REG_WIDTH = $clog2(DIVIDEND+(1<<(OFFSET_WIDTH-1)))+1;

wire [REG_WIDTH-1:0] DIVISOR_LOAD;
assign DIVISOR_LOAD = DIVIDEND +
                    {{REG_WIDTH-OFFSET_WIDTH{offset[OFFSET_WIDTH-1]}}, offset};
reg [REG_WIDTH-1:0] dividend, divisor;
wire [REG_WIDTH-1:0] diff;
assign diff = dividend - divisor;

reg start_d, busy = 0;
reg [$clog2(QUOTIENT_WIDTH)-1:0] bitCount;

always @(posedge clk) begin
    start_d <= start;
    if (busy) begin
        bitCount <= bitCount - 1;
        if (bitCount == 0) begin
            quotientValid <= 1;
            busy <= 0;
        end
        if (dividend >= divisor) begin
            dividend <= {diff[0+:REG_WIDTH-1], 1'b0};
            quotient <= {quotient[0+:QUOTIENT_WIDTH-1], 1'b1};
        end
        else begin
            dividend <= {dividend[0+:REG_WIDTH-1], 1'b0};
            quotient <= {quotient[0+:QUOTIENT_WIDTH-1], 1'b0};
        end
    end
    else begin
        if (start && !start_d) begin
            busy <= 1;
            quotientValid <= 0;
            dividend <= DIVIDEND;
            divisor <= DIVISOR_LOAD;
            bitCount <= QUOTIENT_WIDTH - 1;
        end
    end
end
endmodule
