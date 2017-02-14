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
module non_synth_circuits(input mux_in0, mux_in1, mux_sel,
     		              output mux_out_rtl, mux_out_passgate,mux_out_remodeled);  
   
    assign mux_out_rtl = mux_sel ? mux_in1 : mux_in0;

	// non-synthesizable passgate implementation
`ifdef SIM
    nmos p0 (mux_out_passgate,mux_in0, ~mux_sel);    
    pmos p1 (mux_out_passgate,mux_in0, mux_sel);        

    nmos p2 (mux_out_passgate,mux_in1, mux_sel);    
    pmos p3 (mux_out_passgate,mux_in1, ~mux_sel);        
`endif

	// implementation using remodelled transistors
    nmos_model p4 (mux_out_remodeled,mux_in0, ~mux_sel);    
    pmos_model p5 (mux_out_remodeled,mux_in0, mux_sel);        

    nmos_model p6 (mux_out_remodeled,mux_in1, mux_sel);    
    pmos_model p7 (mux_out_remodeled,mux_in1, ~mux_sel);        
       
endmodule // non_synth_circuits


module nmos_model(output out,input in,ctrl);

    assign out = ctrl ? in : 1'bz;

endmodule

module pmos_model(output out,input in,ctrl);

    assign out = ~ctrl ? in : 1'bz;

endmodule