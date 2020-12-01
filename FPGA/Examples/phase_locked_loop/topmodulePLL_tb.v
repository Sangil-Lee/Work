/* This module creates the input test vectors for the device under test. The time periods
of the REF_Clk and DDS Clk can be changed according to the frequency of the clocks. */

'timescale 10ps/1fs

module topmodulePLL_tb();

wire [15:0] FTW;
reg REF_Clk;
reg Reset;
reg DDS_Clk;

wire [15:0] ph_acc;
wire DDS_Out;
reg select;
reg REF_Clk1, REF_Clk2;

initial
begin
	$shm_open("waves. shm") ;
	$shm_probe("AS");
end


topmodulePLL t1 (.DDS_Out(DDS_Out), .REF_Clk(REF_Clk), .DDS_Clk(DDS_Clk), .Reset(Reset), .FTW(FTW));

always@(REF_Clk1 or REF_Clk2 or select)
begin
	if(select == 1)		 REF_Clk = REF_Clk1;
	else if(select == 0) REF_Clk = REF_Clk2;
end


initial
begin
	select			= 1'b1;
	REF_Clk1		= 1'b0;
	REF_Clk2		= 1'b0;
	DDS_Clk			= 1'b0;
	Reset			= 1'b0;
	#5 Reset        = 1'b1;
	#500 Reset      = 1'b0;
	#1000000 select = 1'b1;
end

	always REF_Clk1 = #10 ~REF_Clk1 ; /* Reference clock frequency - 50MHz */
	always REF_Clk2 = #50 ~REF_Clk2;
	always DDS_Clk  = #5  ~DDS_Clk;   /* System clock frequency - 100MHz */

end module
/* End of Module */ 
