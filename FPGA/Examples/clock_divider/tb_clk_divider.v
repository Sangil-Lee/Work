`timescale 1ns / 1ps
// fpga4student.com FPGA projects, VHDL projects, Verilog projects
// Verilog project: Verilog code for clock divider on FPGA
// Testbench Verilog code for clock divider on FPGA
module tb_clk_divider;
 // Inputs
 reg clock_in;
 // Outputs
 wire clock_out;
 // Instantiate the Unit Under Test (UUT)
 // Test the clock divider in Verilog
 clock_divider uut (
  .clk_in(clock_in), 
  .clk_out(clock_out)
 );
 
 initial begin
  // Initialize Inputs
  clock_in = 0;
  // create input clock 50MHz
        forever #10 clock_in = ~clock_in;
 end
endmodule
