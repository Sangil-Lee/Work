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

// Simple time-to-digital converter
// Measure time between falling edges of two inputs

module simpleTDC #(
    parameter DEADTIME_TICKS = 2000000,
    parameter DEBUG          = "false"
    ) (
                         input  wire        clk,
    (*mark_debug=DEBUG*) input  wire        start_n,
    (*mark_debug=DEBUG*) input  wire        stop_n,
    (*mark_debug=DEBUG*) output reg  [31:0] dout);

localparam INTERVAL_WIDTH = 12;
(*mark_debug=DEBUG*) reg [INTERVAL_WIDTH-1:0] interval;
(*mark_debug=DEBUG*) reg [32-INTERVAL_WIDTH-1:0] passCount = 0;
reg        deadTime = 0;
reg [$clog2(DEADTIME_TICKS)-1:0] deadTimeCounter;

(*mark_debug=DEBUG*) reg start_m, start, start_d;
(*mark_debug=DEBUG*) reg stop_m, stop;
(*mark_debug=DEBUG*) reg running = 0;

always @(posedge clk) begin
    // Debounce and make active high
    start_m <= !start_n;
    start   <= start_m;
    start_d <= start;
    stop_m <= !stop_n;
    stop   <= stop_m;

    if (deadTime) begin
        deadTimeCounter <= deadTimeCounter - 1;
        if (deadTimeCounter == 0) begin
            deadTime <= 0;
        end
    end
    else if (running) begin
        interval <= interval + 1;
        if (stop || (interval == {INTERVAL_WIDTH{1'b1}})) begin
            dout <= {passCount, interval};
            running <= 0;
            deadTime = 1;

        end
    end
    else begin
        deadTimeCounter <= DEADTIME_TICKS - 1;
        if (start && !start_d && !stop) begin
            passCount = passCount + 1;
            interval <= 1;
            running <= 1;
        end
    end
end
endmodule
