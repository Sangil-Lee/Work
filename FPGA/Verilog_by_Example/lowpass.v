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

