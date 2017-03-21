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
module timing_analyzer(  input clka,clkb,
                         input reset, 
                    
                         input wea,web,
                         
                         input [15:0] dina,dinb,
                         input [9:0] addra,addrb,
                                    
                         output reg [15:0] douta_qo,doutb_qo,
                         
                         output reg wea_qo
                                    
                       );  

    wire [15:0] douta,doutb;
    wire clka_mmcm;
    
    always @(posedge clka_mmcm)
        if(reset) begin
            douta_qo  <= 'b0;
        end    
        else begin
            douta_qo <= douta; 
        end       
    
    always @(posedge clkb)
        if(reset) begin
            doutb_qo  <= 'b0;
            wea_qo    <= 1'b0;
        end    
        else begin
            doutb_qo <= doutb; 
            wea_qo   <= wea;  // clocked by clkb
        end       

    
    blk_mem mem (
	    .clka(clka_mmcm),
	    .wea(wea), 
	    .addra(addra), 
	    .dina(dina),
	    .douta(douta), 
	    .clkb(clkb),
	    .web(web), 
	    .addrb(addrb), 
	    .dinb(dinb), 
	    .doutb(doutb)); 
        
        
  clka_mmcm mmcm (
    .CLK_IN1            (clka),    
    .CLK_OUT1           (clka_mmcm),
    .RESET              (reset));  
        
    
    
endmodule // timing_analyzer