module generate_statement;

wire [7:0] PMOD1_DIAG;

parameter  MITIGATION_COUNT = 16;
wire [7:0] auroraMitigationOutputs;

genvar i;
generate
for (i = 0 ; i < 8 ; i = i + 1) begin : pmod1
  if (i < MITIGATION_COUNT)
    assign PMOD1_DIAG[i] = auroraMitigationOutputs[i];
  else
    assign PMOD1_DIAG[i] = 1'b0;
end /* for */
endgenerate

endmodule
