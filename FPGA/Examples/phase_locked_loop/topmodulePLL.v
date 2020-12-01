'timescale 1 ns/1 Ops

module topmodulePLL (DDS_Out, Slow, Fast, DDS_Cik, Reset, REF_Clk);
/* Declaration of Output Ports */
output DDS_Out;

/* Declaration of I nput Ports */
input DDS_Cik;
input Reset;
input REF_Clk;
inout [15:0] FTW;
inout Slow, Fast;

wire [7:0] Time_Frame;

/*I nstantiation of the PFD module */

PFD pd(.Time_Frame(Time_Frame) , .Slow (Slow), .Fast (Fast), .REF_Clk (REF_Clk),
	.DDS_Out(DDS_Out) , .Reset(Reset));

/* Instantiation of the Phase Accumulator module */
PhaseAccumulator pa(.DDS_Out (DDS_Out), .DDS_Cik (DDS_Cik), . FTW(FTW), . Reset(Reset));

/* Instantiation of the Control Unit module */
Controllogic ci(.FTW(FTW), .REF_Clk(REF_Clk), .Reset(Reset), .Time_Frame(Time_Frame), .Slow(Slow) , .Fast(Fast));


endmodule 
