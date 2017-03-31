module gtx_rx_phase_force(
	input ref_clk,
	input [4:0] bitslide_counter,
	input bitslide_syncd,
	input comma_is_lsb,
	output gtx_reset_out
);

parameter [4:0] BITSLIDE_COUNTER_FORCE = 5;
parameter COMMA_IS_LSB_FORCE = 1;
reg bitslide_syncd_d = 1'b0;
wire bitslide_syncd_posedge = bitslide_syncd && !bitslide_syncd_d;
wire gtx_reset_condition = bitslide_syncd_posedge && ((bitslide_counter != BITSLIDE_COUNTER_FORCE) || (comma_is_lsb != COMMA_IS_LSB_FORCE));
reg gtx_reset = 1'b0;

always @(posedge ref_clk) begin
	bitslide_syncd_d <= bitslide_syncd;
	gtx_reset <= gtx_reset_condition;
end

assign gtx_reset_out = gtx_reset;

endmodule
