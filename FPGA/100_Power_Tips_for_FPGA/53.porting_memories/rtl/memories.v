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
module memories(  input clk,
                  input reset, 
                  
                  input [15:0] mem1_din,
                  input [15:0] mem2_din,

                  input [9:0] mem1_addr,
                  input [9:0] mem2_addr,
                                    
                  input [15:0] mem1_wen,
                  input [15:0] mem2_wen,
                  
                  output [15:0] mem1_dout,
                  output [15:0] mem2_dout                  
                                    
                );  

   
    reg [9:0] mem1_addr_q,
              mem2_addr_q;
              
    reg [15:0] mem1_wen_q,
               mem2_wen_q;
              
    reg [15:0] mem1_din_q,
               mem1_dinb;

    reg [15:0] mem2_din_q,
               mem2_dinb;
               
    // bit-enable using read-modify-write with the same clock edge 
    
    always @(posedge clk)
        if(reset) begin
            mem1_addr_q <= 'b0;
            mem1_wen_q  <= 'b0;
            mem1_din_q  <= 'b0;
        end    
        else begin
            mem1_addr_q <= mem1_addr; 
            mem1_wen_q  <= mem1_wen;
            mem1_din_q  <= mem1_din;
        end       
    
    integer ix;
    always @(*) begin
        for(ix=0;ix<16;ix=ix+1)
            mem1_dinb[ix] = mem1_wen_q[ix] ? mem1_din_q[ix] : mem1_dout[ix];
    end

    
    blk_mem mem1 (
	    .clka(clk),
	    .wea(1'b0), 
	    .addra(mem1_addr),
	    .dina(16'h0),
	    .douta(mem1_dout), 
	    .clkb(clk),
	    .web(|mem1_wen_q), 
	    .addrb(mem1_addr_q), 
	    .dinb(mem1_dinb), 
	    .doutb()); 
    
  
  
    // bit-enable using read-modify-write using rising and falling edge of the clock

    always @(posedge clk)
        if(reset) begin
            mem2_addr_q <= 'b0;
            mem2_wen_q  <= 'b0;
            mem2_din_q  <= 'b0;
        end    
        else begin
            mem2_addr_q <= mem2_addr; 
            mem2_wen_q  <= mem2_wen;
            mem2_din_q  <= mem2_din;
        end       
    
    integer iy;
    always @(*) begin
        for(iy=0;iy<16;iy=iy+1)
            mem2_dinb[iy] = mem2_wen_q[iy] ? mem2_din_q[iy] : mem2_dout[iy];
    end
    
    

    blk_mem mem2 (
	    .clka(clk),
	    .wea(1'b0), 
	    .addra(mem2_addr),
	    .dina(16'h0),
	    .douta(mem2_dout), 
	    .clkb(~clk),
	    .web(|mem2_wen_q), 
	    .addrb(mem2_addr_q), 
	    .dinb(mem2_dinb), 
	    .doutb()); 
    
    
endmodule // memories