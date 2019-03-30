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

// Measure fan speed

module fanTach #(
    parameter PULSES_PER_REVOLUTION = 2,
    parameter CLOCK_RATE            = 100000000
    ) (
    input  wire        clk,
    input  wire        fanTach,
    output reg  [15:0] fanRPM);

// Debounce the signal by sampling at only 50 kHz
localparam SAMPLE_INTERVAL_DIVIDER = (CLOCK_RATE + 25000) / 50000;
localparam SAMPLE_INTERVAL_RELOAD  = SAMPLE_INTERVAL_DIVIDER - 1;
reg [$clog2(SAMPLE_INTERVAL_DIVIDER)-1:0] sampleCounter = 0;
reg                                       sample = 0;

// Number of sampling intervals in 1/8 of a minute
localparam UPDATE_INTERVAL_TICKS = (60*50000)/8;
localparam UPDATE_INTERVAL_RELOAD = UPDATE_INTERVAL_TICKS - 1;
reg [$clog2(UPDATE_INTERVAL_TICKS)-1:0] updateCounter = 0;
reg                                       update = 0;

// Pulse counter
reg  [16:0] pulseCounter = 0;
wire [15:0] pulseSelect;
assign pulseSelect = (PULSES_PER_REVOLUTION == 8) ? pulseCounter[1+:16] :
                     (PULSES_PER_REVOLUTION == 4) ? pulseCounter[0+:16] :
                                                   {pulseCounter[0+:15], 1'b0};

// Watch for tachometer signal edges
reg  fanTach_m, fanTach_d0, fanTach_d1;
wire isEdge;
assign isEdge = (fanTach_d0 != fanTach_d1);

always @(posedge clk) begin
    if (sampleCounter == 0) begin
        sampleCounter <= SAMPLE_INTERVAL_RELOAD;
        sample <= 1;
    end
    else begin
        sampleCounter <= sampleCounter - 1;
        sample <= 0;
    end
    if (sample) begin
        fanTach_m  <= fanTach;
        fanTach_d0 <= fanTach_m;
        fanTach_d1 <= fanTach_d0;
        if (updateCounter == 0) begin
            updateCounter <= UPDATE_INTERVAL_RELOAD;
            update <= 1;
        end
        else begin
            update <= 0;
            updateCounter <= updateCounter - 1;
        end
        if (update) begin
            fanRPM <= pulseSelect;
            if (isEdge) begin
                pulseCounter <= 1;
            end
            else begin
                pulseCounter <= 0;
            end
        end
        else begin
            if (isEdge) begin
                pulseCounter <= pulseCounter + 1;
            end
        end
    end
end
endmodule
