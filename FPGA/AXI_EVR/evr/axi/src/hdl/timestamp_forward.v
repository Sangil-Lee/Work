//
// Forward time stamp from one clock domain to another.
//
module timestamp_forward(clk_in, timestamp_in, clk_out, timestamp_out);

parameter TIMESTAMP_WIDTH = 64;

input                        clk_in;
input  [TIMESTAMP_WIDTH-1:0] timestamp_in;
input                        clk_out;
output [TIMESTAMP_WIDTH-1:0] timestamp_out;
 
// clk_in domain
reg req_in = 0;
reg ack_in = 0;
reg [TIMESTAMP_WIDTH-1:0] timestamp_in_latched;

// clk_out domain
reg req_out = 0;
reg req_d_out = 0;
reg [TIMESTAMP_WIDTH-1:0] timestamp_out;

always @(posedge clk_in)
begin
    if (req_in == ack_in) begin
        req_in <= !req_in;
        timestamp_in_latched <= timestamp_in;
    end
    ack_in <= req_d_out;
end
        
always @(posedge clk_out)
begin
    req_out <= req_in;
    req_d_out <= req_out;
    if (req_d_out != req_out)
        timestamp_out <= timestamp_in_latched;
end

endmodule
