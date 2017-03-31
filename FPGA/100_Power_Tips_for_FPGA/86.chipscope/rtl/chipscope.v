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
module chipscope(input btn0,btn1,
                 output led0,led1,
                 input sys_clk_in_p, sys_clk_in_n // 200mhz clock
             );

    wire clkin;

    wire [35:0] CONTROL0, CONTROL1;  
    
    wire [1:0]  vio_sync_out;
    
    wire [7:0]  ILA_DATA;
    wire [3:0]  ILA_TRIG;


    assign ILA_DATA =  {4'b0,btn0,btn1,led0,led1};
    assign ILA_TRIG =  {btn0,btn1,led0,led1};
   
    // LEDs are turned on from either a button or from a VIO output                      
    assign led0 = btn0 | vio_sync_out[0];
    assign led1 = btn1 | vio_sync_out[1];
    
    IBUFGDS clkin_buf (.I  (sys_clk_in_p), 
                       .IB (sys_clk_in_n),
                       .O  (clkin));

    chipscope_icon
      icon (
        .CONTROL0(CONTROL0), 
        .CONTROL1(CONTROL1));
                             
                             
    chipscope_vio 
      vio (
        .CONTROL(CONTROL0), 
        .CLK(clkin),
        .ASYNC_IN({btn0,btn1}), 
        .SYNC_OUT(vio_sync_out));
                         
    
    chipscope_ila 
      ila (
        .CONTROL(CONTROL1), 
        .CLK(clkin), 
        .DATA(ILA_DATA), 
        .TRIG0(ILA_TRIG[0]),
        .TRIG1(ILA_TRIG[1]),
        .TRIG2(ILA_TRIG[2]),
        .TRIG3(ILA_TRIG[3]));
    

endmodule // chipscope