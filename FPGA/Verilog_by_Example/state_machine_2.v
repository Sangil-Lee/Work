module state_machine_1 (clk, reset,go, kill, done);

	input clk,reset, go, kill;
	output done;

	reg done;
	reg [6:0]  count;
	reg [1:0] state_reg;

	parameter idle	 = 2'b00;
	parameter active = 2'b01;
	parameter finish = 2'b10;
	parameter abort  = 2'b11;

//State Machine
	always @(posedge clk or posedge reset)
	begin
		if(reset)
		begin
			state_reg <= idle;
			count <= 7'h00;
			done <= 1'b0;
		end
		else
			case (state_reg)
				idle:
					begin
						count <= 7'h00;
						done <= 1'b0;
						if(go)
							state_reg <= active;
					end
				active:
					begin
						count <= count+1;
						done <= 1'b0;
						if(kill)
							state_reg <= abort;
						else if(count <= 7'd100)
							state_reg <= finish;
					end
				finish:
					begin
						count <= 7'h00;
						done <= 1'b1;
						state_reg <= idle;
					end
				abort:
					begin
						count <= 7'h00;
						done <= 1'b0;
						if(!kill)
							state_reg <= idle;
					end
				default:
					begin
						count <= 7'h00;
						done <= 1'b0;
						state_reg <= idle;
					end
			endcase
	end
endmodule
