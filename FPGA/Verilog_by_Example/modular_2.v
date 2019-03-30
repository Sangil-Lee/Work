`include "state_machine_1.v"
module modular_2 ( clk, reset, go_1, kill_1, go_2, kill_2, go_3, kill_3, kill_clr, done_out, kill_ltchd);
 input clk, reset, go_1, kill_1, go_2, kill_2, go_3, kill_3, kill_clr;
 output done_out;
 output kill_ltchd;

 reg kill_ltchd;
 wire done_1;
 wire done_2;

 // ------ Design implementation -----
 // first module instantiation
 state_machine_1 go_delay_1
 (
 .reset ( reset ),
 .clk ( clk ),
 .go ( go_1 ),
 .kill ( kill_1 ),
 .done ( done_1 )
 );
 // second module instantiation
 state_machine_1 go_delay_2
 (
 .reset ( reset ),
 .clk ( clk ),
 .go ( done_1 | go_2 ),
 .kill ( kill_2 ), 
 .done ( done_2 )
 );

 // third module instantiation
 state_machine_1 go_delay_3
 (
 .reset ( reset ),
 .clk ( clk ),
 .go ( done_1 | done_2 | go_3),
 .kill ( kill_3 ),
 .done ( done_out )
 );

 // Kill Latch
 always @( posedge clk or posedge reset )
 begin
 if ( reset )
	 kill_ltchd <= 1'b0;
 else if ( kill_1 || kill_2 || kill_3)
	 kill_ltchd <= 1'b1;
 else if ( kill_clr )
	 kill_ltchd <= 1'b0;
 end

endmodule 
//////////////////////////////////////////
