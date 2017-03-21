//
// Forward edge-triggered interrupt requests from one clock domain to another.
// A rising edge on the input results in a one cycle pulse on the output.
// Rising edges arriving while a previous
// edge is being processed will be ignored.
//
module irq_forward(clk_in, irq_in, clk_out, irq_out);

input  clk_in;
input  irq_in;
input  clk_out;
output irq_out;
 
// clk_in domain
reg irq_d_in = 0;
reg irq_req_in = 0;
reg ack_in = 0;

// clk_out domain
reg irq_req_out = 0;
reg  irq_req_d_out = 0;
wire irq_out;

always @(posedge clk_in)
begin
    irq_d_in <= irq_in;
    if (ack_in)
        irq_req_in <= 0;
    else if (irq_in && !irq_d_in)
        irq_req_in <= 1;
    ack_in <= irq_req_d_out;
end
        
always @(posedge clk_out)
begin
    irq_req_out <= irq_req_in;
    irq_req_d_out <= irq_req_out;
end

assign irq_out = irq_req_out && !irq_req_d_out;

endmodule
