// File gtp_bitslide.vhd translated with vhd2vl v2.4 VHDL to Verilog RTL translator
// vhd2vl settings:
//  * Verilog Module Declaration Style: 1995

// vhd2vl is Free (libre) Software:
//   Copyright (C) 2001 Vincenzo Liguori - Ocean Logic Pty Ltd
//     http://www.ocean-logic.com
//   Modifications Copyright (C) 2006 Mark Gonzales - PMC Sierra Inc
//   Modifications (C) 2010 Shankar Giri
//   Modifications Copyright (C) 2002, 2005, 2008-2010 Larry Doolittle - LBNL
//     http://doolittle.icarus.com/~larry/vhd2vl/
//
//   vhd2vl comes with ABSOLUTELY NO WARRANTY.  Always check the resulting
//   Verilog for correctness, ideally with a formal verification tool.
//
//   You are welcome to redistribute vhd2vl under certain conditions.
//   See the license (GPLv2) file included with the source for details.

// The result of translation follows.  Its copyright status should be
// considered unchanged from the original VHDL.

//-----------------------------------------------------------------------------
// Title      : Deterministic Xilinx GTP wrapper - bitslide state machine
// Project    : White Rabbit Switch
//-----------------------------------------------------------------------------
// File       : gtp_bitslide.vhd
// Author     : Tomasz Wlostowski
// Company    : CERN BE-CO-HT
// Created    : 2010-11-18
// Last update: 2011-04-11
// Platform   : FPGA-generic
// Standard   : VHDL'93
//-----------------------------------------------------------------------------
// Description: Module implements a manual bitslide alignment state machine and
// provides the obtained bitslide value to the MAC.
//-----------------------------------------------------------------------------
//
// Original EASE design (c) 2010 NIKHEF / Peter Jansweijer and Henk Peek
// VHDL port (c) 2010 CERN / Tomasz Wlostowski
//
// <license>
//
//-----------------------------------------------------------------------------
// Revisions  :
// Date        Version  Author    Description
// 2010-11-18  0.4      twlostow  Ported EASE design to VHDL 
// 2011-02-07  0.5      twlostow  Verified on Spartan6 GTP
//-----------------------------------------------------------------------------
// no timescale needed

module gtp_bitslide(
	input gtp_rst_i,
	// GTP
	input gtp_rx_clk_i,
	// '1' indicates that the GTP has detected a comma in the incoming serial stream
	input gtp_rx_comma_det_i,
	input gtp_rx_byte_is_aligned_i,
	// GTP ready flag (PLL locked and RX signal present)
	input serdes_ready_i,
	// GTP manual bitslip control line
	output reg gtp_rx_slide_o,
	// GTP CDR reset, asserted when the link is lost to set the bitslide to a known value
	output reg gtp_rx_cdr_rst_o,
	// Current bitslide, in UIs
	output reg [4:0] bitslide_o,
	// '1' when the bitsliding has been completed and the link is up
	output reg synced_o
);

// set to non-zero value to enable some simulation speedups (reduce delays)

//parameter g_simulation;
//  function f_eval_sync_detect_threshold
//    return integer is
//  begin
//    if(g_simulation /= 0) then
//      return 256;
//    else
//      return 8192;
//    end if;
//  end f_eval_sync_detect_threshold;

// Ignore VHDL function and use non-simulation value
parameter f_eval_sync_detect_threshold = 8192;
parameter c_sync_detect_threshold = f_eval_sync_detect_threshold;
parameter [15:0] c_pause_tics = 100;
parameter [2:0]
	S_SYNC_LOST = 0,
	S_STABILIZE = 1,
	S_SLIDE = 2,
	S_PAUSE = 3,
	S_GOT_SYNC = 4,
	S_RESET_CDR = 5;

reg [2:0] state=3'b0;
reg [15:0] counter = 16'b0;
reg [4:0] cur_slide = 5'b0;
reg [1:0] commas_missed;

// behavioral
always @(posedge gtp_rx_clk_i or posedge gtp_rst_i) begin
	if(gtp_rst_i == 1'b 1) begin
		state <= S_SYNC_LOST;
		gtp_rx_slide_o <= 1'b 0;
		counter <= {16{1'b0}};
		synced_o <= 1'b 0;
		gtp_rx_cdr_rst_o <= 1'b 0;
	end else begin
		if((serdes_ready_i == 1'b 0)) begin
			state <= S_SYNC_LOST;
		end
		case(state)
			// State: synchronization lost. Waits until a comma pattern is detected
			S_SYNC_LOST : begin
				cur_slide <= {5{1'b0}};
				counter <= {16{1'b0}};
				gtp_rx_slide_o <= 1'b 0;
				synced_o <= 1'b 0;
				gtp_rx_cdr_rst_o <= 1'b 0;
				commas_missed <= {2{1'b0}};
				if((gtp_rx_comma_det_i == 1'b 1)) begin
					state <= S_STABILIZE;
				end
			end

			// State: stabilize: 
			S_STABILIZE : begin
        		if((gtp_rx_comma_det_i == 1'b 1)) begin
        			counter <= counter + 1;
        			commas_missed <= {2{1'b0}};
        		end
        		else begin
       		 		commas_missed <= commas_missed + 1;
        			if((commas_missed[1] == 1'b 1)) begin
        	   			state <= S_SYNC_LOST;
          			end
        		end
        		if((counter == ((c_sync_detect_threshold)))) begin
        			counter <= {16{1'b0}};
        			state <= S_PAUSE;
        		end
        		if((serdes_ready_i == 1'b 0)) begin
        			state <= S_SYNC_LOST;
        		end
      		end

			S_SLIDE : begin
				cur_slide <= cur_slide + 1;
   	    		gtp_rx_slide_o <= 1'b 1;
   	    		counter <= {16{1'b0}};
   		 		state <= S_PAUSE;
   	    		if((serdes_ready_i == 1'b 0)) begin
    	   			state <= S_SYNC_LOST;
    	   		end
			end

			S_PAUSE : begin
				counter <= counter + 1;
    	   	 	gtp_rx_slide_o <= 1'b 0;
       		 	if(counter == c_pause_tics) begin
					if((gtp_rx_byte_is_aligned_i == 1'b 0)) begin
       	     		state <= S_SLIDE;
       		   		end
          			else begin
           		 		state <= S_GOT_SYNC;
          			end
        		end
      		end

			S_GOT_SYNC : begin
				gtp_rx_slide_o <= 1'b 0;
				bitslide_o <= (cur_slide);
				synced_o <= 1'b 1;
				if((gtp_rx_byte_is_aligned_i == 1'b 0 || serdes_ready_i == 1'b 0)) begin
    	      		gtp_rx_cdr_rst_o <= 1'b 1;
    	      		state <= S_SYNC_LOST;
    	    	end
    	  	end

	      	default : begin
   		   	end

		endcase
	end
end

endmodule
