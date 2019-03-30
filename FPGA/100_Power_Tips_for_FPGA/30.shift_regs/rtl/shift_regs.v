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

module shift_regs(  input clk_ff,clk_srl, clk_bram,
                    input reset,
                    input enable_shift_ram,enable_shift_flops,enable_shift_bram, 
                    
                    input shift_ram_din, shift_flops_din,shift_bram_din,
                    output shift_ram_dout, shift_flops_dout,shift_bram_dout,shift_flops_for_loop_dout
                );  
                
                
    shift_ram_coregen shift_ram_coregen (
	    .d(shift_ram_din),
	    .clk(clk_srl),
	    .ce(enable_shift_ram),
	    .sclr(reset),
	    .q(shift_ram_dout)); 
                
 
    shift_flops shift_flops (clk_ff,reset,enable_shift_flops,shift_flops_din,shift_flops_dout);

    shift_bram shift_bram (clk_bram,reset,enable_shift_bram,shift_bram_din,shift_bram_dout);

    shift_flops_for_loop shift_flops_for_loop (clk_ff,reset,enable_shift_flops,shift_flops_din,shift_flops_for_loop_dout);                
    
endmodule // shift_regs


module shift_flops( input clk,
                    input reset,
                    input enable, 
                    
                    input shift_din,
                    output shift_dout );  
             
    localparam SHIFT_REG_SIZE = 64;
                
    reg [SHIFT_REG_SIZE-1:0] shift_reg;
    
    assign shift_dout = shift_reg[SHIFT_REG_SIZE-1];
    
                
    always @(posedge clk)
        if(reset) begin
            shift_reg  <= 'b0;
        end    
        else begin
            shift_reg  <= enable ? {shift_reg[SHIFT_REG_SIZE-2:0] , shift_din} : shift_reg;
        end       
    
endmodule // shift_flops


module shift_flops_for_loop( input clk,
                             input reset,
                             input enable, 
                             
                             input shift_din,
                             output shift_dout );  
             
    localparam SHIFT_REG_SIZE = 64;
                
    reg [SHIFT_REG_SIZE-1:0] shift_reg;
    
    integer ix;

    assign shift_dout = shift_reg[SHIFT_REG_SIZE-1];
    
                
    always @(posedge clk)
        if(reset) begin
            shift_reg  <= 'b0;
        end    
        else begin
		    for(ix=1; ix < SHIFT_REG_SIZE; ix = ix + 1)
                shift_reg[ix]  <= enable ? shift_reg[ix-1] : shift_reg[ix];
			shift_reg[0]  <= enable ? shift_din : shift_reg[0];
        end       
    
endmodule // shift_flops_for_loop


module shift_bram(  input clk,
                    input reset,
                    input enable, 
                    
                    input shift_din,
                    output shift_dout );  
             
    wire [63:0] dinb, douta;
    
    reg shift_din_q, enable_q;
                
    
    always @(posedge clk)
        if(reset) begin
            shift_din_q  <= 1'b0;
            enable_q     <= 1'b0;   
        end    
        else begin
            shift_din_q  <= shift_din;
            enable_q     <= enable;
        end   
        
    assign shift_dout = douta[63];
    assign dinb = {douta[62:0],shift_din_q};
        
	// read from side A
	// write shifted value to side B
	// BRAM mode is write-first
    blk_mem_gen_v4_1 blk_mem (
	    .clka(clk),
	    .wea(1'b0), 
	    .addra(1'b0), 
	    .dina(64'b0), 
	    .douta(douta), 

	    .clkb(clk),
     	.web(enable_q), 
	    .addrb(1'b0), 
     	.dinb(dinb), 
	    .doutb());
    
endmodule // shift_bram




