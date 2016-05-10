`timescale 1ns / 1ps

module gtx_byteisaligned(
	input ref_clk,
	input comma_detected,
	output byteisaligned_out
);

parameter no_detect_threshold = 20;
parameter byte_aligned_threshold = 20;

reg [4:0] commas_missed = 5'b0;
reg [4:0] comma_detected_count = 5'b0;
reg byteisaligned = 1'b0;

always @(posedge ref_clk) begin

	if(comma_detected == 1'b1) begin
		commas_missed <= 5'b0;
		if (comma_detected_count == byte_aligned_threshold) begin
		comma_detected_count <= 5'b0;
		byteisaligned <= 1'b1;
		end else comma_detected_count <= comma_detected_count + 1'b1;
	end else begin 
		if(commas_missed == no_detect_threshold) begin
			commas_missed <= 5'b0;
			byteisaligned <= 1'b0;
		end else commas_missed <= commas_missed + 1'b1;
	end
end

assign byteisaligned_out = byteisaligned;

endmodule
