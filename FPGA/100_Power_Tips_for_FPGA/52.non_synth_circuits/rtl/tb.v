`timescale 1ns / 100ps

//-----------------------------------------------------------------------------
// Copyright (C) 2011 OutputLogic.com 
// This source file may be used and distributed without restriction 
// provided that this copyright statement is not removed from the file 
// and that any derivative work contains the original copyright notice 
// and the associated disclaimer. 
// 
// THIS SOURCE FILE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS 
// OR IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED	
// WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. 
//-----------------------------------------------------------------------------

module tb;

	reg mux_in0, mux_in1, mux_sel;

    wire mux_out_rtl, mux_out_passgate,mux_out_remodeled;


	non_synth_circuits uut (
	    .mux_in0(mux_in0), 
	    .mux_in1(mux_in1), 
	    .mux_sel(mux_sel), 
	    .mux_out_rtl(mux_out_rtl), 
	    .mux_out_passgate(mux_out_passgate),
		.mux_out_remodeled(mux_out_remodeled)
	    );


	initial begin
		mux_sel = 0;
		mux_in1 = 0;
		mux_in0 = 0;

		#100;
        mux_in0 = 1;
		mux_in1 = 0;

        #100;
        mux_sel = 1;

		#100;
        mux_in0 = 0;
		mux_in1 = 1;
       
        #100;        
        $finish;
	end
    
endmodule

