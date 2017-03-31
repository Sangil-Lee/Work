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
module state_machines( input clk,
                       input reset, 
                       input [3:0] state_inputs,
                       output reg [3:0] state_outputs
                     );  

    localparam STATE_INIT   = 3'd0,
	           STATE_ONE    = 3'd1,
	           STATE_TWO    = 3'd2,
	           STATE_THREE  = 3'd3;

    reg [2:0] state_cur,
	          state_next;
              
    reg [1:11*8] state_str;          

    always @(posedge clk) begin
        if(reset) begin
            state_cur     <= STATE_INIT;
			state_outputs <= 4'b0;
		end
        else begin
            state_cur        <= state_next;    
			state_outputs[0] <= state_cur == STATE_INIT;
			state_outputs[1] <= state_cur == STATE_ONE;
			state_outputs[2] <= state_cur == STATE_TWO;
			state_outputs[3] <= state_cur == STATE_THREE;
        end
    end

    always @(*) begin
        state_next = state_cur;

        case(state_cur)
			STATE_INIT:  if(state_inputs[0]) state_next = STATE_ONE;  
			STATE_ONE:   if(state_inputs[1]) state_next = STATE_TWO;
			STATE_TWO:   if(state_inputs[2]) state_next = STATE_THREE;
			STATE_THREE: if(state_inputs[3]) state_next = STATE_INIT;
        endcase
    end
    
    always @(*) begin
        state_str = "";

        case(state_cur)
			STATE_INIT:  state_str = "STATE_INIT";  
			STATE_ONE:   state_str = "STATE_ONE";
			STATE_TWO:   state_str = "STATE_TWO";
			STATE_THREE: state_str = "STATE_THREE";
        endcase
    end
    
endmodule // state_machines