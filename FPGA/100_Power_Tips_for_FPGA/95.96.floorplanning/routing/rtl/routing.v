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
`timescale 1ns / 100ps

module routing(input  data_in_0,data_in_1,data_in_2,data_in_3,
               output data_out_0,data_out_1,data_out_2,data_out_3 );
                
    assign data_out_0 = data_in_0;  // horizontal route
    assign data_out_1 = data_in_1;  // vertical route
    assign data_out_2 = data_in_2;  // diagonal route
    assign data_out_3 = data_in_3;  // routing around         
endmodule // routing


