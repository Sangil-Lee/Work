//State Machine

module state_machine1(clk, reset, go, kill, done);
	input clk, reset, go, kill;
	output done;

	reg [6:0] count;
	reg		  done;
	reg [1:0] state_reg;

	//state machine parameters

	parameter idle		= 2'b00;
	parameter active	= 2'b01;
	parameter finish	= 2'b10;
	parameter abort		= 2'b11;

	always @(posedge clk or posedge reset)
	begin
		if (reset)
			state_reg <= idle;
		else
			case (state_reg)
				idle: if(go)  state_reg <= active;

				active: 
					if(kill)  state_reg <= abort;
					else if (count == 7'd100) state_reg <= finish;

				finish: 
					state_reg <= idle;

				abort: 
					if(!kill)  state_reg <= idle;

				default: 
					state_reg <= idle;
			endcase
	end

	always @(posedge clk or posedge reset)
	begin
		if (reset)
			count <= 7'h00;
		else if (state_reg == finish || state_reg == abort )
			count <= 7'h00;
		else if (state_reg == active )
			count <= count + 1;
	end

	always @(posedge clk or posedge reset)
	begin
		if (reset)
			done <= 1'b0;
		else if (state_reg == finish)
			done <= 1'b1;
		else 
			done <= 1'b0;
	end

endmodule
