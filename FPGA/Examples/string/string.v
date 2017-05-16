module string;

reg [8*21:0] string;

initial begin
	string = "Sample String..";
	$display("%s\n", string);
end

endmodule
