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

module pattern_match1(    input clk, 
                          input clk_en,
                          input cfg_in, 
                          input [79:0] pattern_in,
                     
                          input [79:0] pattern_mask,
                          input [79:0] pattern_match,
                           
                          output [31:0] match_cfglut);  

    wire [16:0] cfg_local;
    
    assign cfg_local[0] = cfg_in;
    
    // 80-bit pattern matcher using CFGLUT5 primitive
    genvar ll;
    generate
        for (ll = 0; ll < 16; ll = ll + 1) begin: gen_cfglut

    
            CFGLUT5 cfglut(
                .I4 (pattern_in[ll*5+4]),
                .I3 (pattern_in[ll*5+3]),
                .I2 (pattern_in[ll*5+2]),
                .I1 (pattern_in[ll*5+1]),
                .I0 (pattern_in[ll*5]),
                .O5 (match_cfglut[ll*2+1]),
                .O6 (match_cfglut[ll*2]),
                .CE (clk_en),
                .CLK(clk),
                .CDI(cfg_local[ll]),
                .CDO(cfg_local[ll+1]));
        end
    endgenerate // gen_cfglut
endmodule // pattern_match1
    

module pattern_match2(  input clk, reset,
                        input [79:0] pattern_in,
                        input [79:0] pattern_mask,
                        input [79:0] pattern_match,
                        output reg  match
                   );  
    
    always @(posedge clk)
        if(reset)
            match <= 1'b0;
        else    
            match <= (pattern_in & pattern_mask) == pattern_match;

endmodule // pattern_match2



