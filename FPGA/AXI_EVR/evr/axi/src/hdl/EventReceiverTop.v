`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    14:36:28 03/12/2010 
// Design Name: 
// Module Name:    EventReceiverTop 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module EventReceiverTop(
//     input 				RESET,
//     input 				TILE2_REFCLK_PAD_P_IN,
//     input 				TILE2_REFCLK_PAD_N_IN,
//     input      		RXP_IN,
//     input      		RXN_IN,
     input              mgt_rec_clk,        // MGT recovered clock
     input  [15:0]      mgt_par_data,       // MGT parallel data
     input  [1:0]       mgt_charisk,        // MGT character is a "K"
     input  [1:0]       mgt_chariscomma,    // MGT character is a comma
     input              mgt_reset_done,     // MGT reset is done
//     input              mgt_bitslide_sync,  // MGT bitslide is synced
     input              bus_reset_n,        // Generic bus reset (active low)
     input              bus_clk,            // Generic bus clock
     input  [31:0]      bus_addr,           // Generic bus address
     input  [31:0]      bus_wr_data,        // Generic bus write data
     input              bus_wr_en,          // Generic bus write enable
     input              bus_rd_en,          // Generic bus read enable
     output [31:0]      bus_rd_data,        // Generic bus read data
     output             bus_rd_ack,         // Generic bus read acknowledge
     output     		intr_Event,         // Event interrupt
     output             intr_FIFO_notEmpty, // FIFO not empty interrupt
     output             intr_FIFO_full,     // FIFO full interrupt
     output             intr_BuffRdy,       // Buffer ready interrupt
//	 input				Bus2IP_Clk,
//	 input	[31:0]	Bus2IP_Addr,
//	 input	[2:0]		Bus2IP_CS,        // Bus to IP chip select for user logic memory selection
//	 input				Bus2IP_RNW,       // Bus to IP read/not write
//	 input	[31:0]	Bus2IP_Data,      // Bus to IP data bus	 
//	 input	[31:0]	Bus2IP_RdCE,		// reg rd selects
//	 input 	[31:0] 	Bus2IP_WrCE,		// reg wr selects
//	 input	[31:0]	slv_reg0,			// channel 0 delay
//	 input	[31:0]	slv_reg1,			// channel 0 width
//	 input	[31:0]	slv_reg2,			// channel 1 delay
//	 input	[31:0]	slv_reg3,			// channel 1 width
//	 input	[31:0]	slv_reg4,			// channel 2 delay
//	 input	[31:0]	slv_reg5,			// channel 2 width
//	 input	[31:0]	slv_reg6,			// channel 3 delay
//	 input	[31:0]	slv_reg7,			// channel 3 width
//	 input	[31:0]	slv_reg8,			// channel 4 delay
//	 input	[31:0]	slv_reg9,			// channel 4 width
//	 input	[31:0]	slv_reg10,			// channel 5 delay
//	 input	[31:0]	slv_reg11,			// channel 5 width
//	 input	[31:0]	slv_reg12,			// channel 6 delay
//	 input	[31:0]	slv_reg13,			// channel 6 width
//	 input	[31:0]	slv_reg14,			// channel 7 delay
//	 input	[31:0]	slv_reg15,			// channel 7 width
//	 input	[31:0]	slv_reg16,			// all channels polarity
//	 input	[31:0]	slv_reg17,			// select map ram A (0) or B (1) multiplex distributed bus (2) 
//	 input	[31:0]	slv_reg18,			
//	 input	[31:0]	slv_reg19,
//	 input	[31:0]	slv_reg20,
//	 input	[31:0]	slv_reg21,
//	 input	[31:0]	slv_reg22,
//	 output	[31:0]	slv_reg23,			// data buffer size, fault, ready
//	 output	[31:0]	slv_reg24,			// Time Stamp seconds
//	 output	[31:0]	slv_reg25,			// Time Stamp offset
//	 output	[31:0]	slv_reg26,			// Time Stamp reg seconds (latched on event) 
//	 output	[31:0]	slv_reg27,			// Time Stamp reg offset
//	 output	[31:0]	slv_reg28,			// Time Stamp FIFO flags
// 	 output	[31:0]	slv_reg29,			// Time Stamp FIFO seconds
//	 output	[31:0]	slv_reg30,			// Time Stamp FIFO offset
//	 output	[31:0]	slv_reg31,			// Time Stamp FIFO event

//    output  [31:0]   IP2Bus_Data,       // IP to Bus data bus
//    output     		IP2Bus_IntrEvent,  // IP to Bus interrupt event
//	 output				IP2Bus_Intr_LOL,
//	 output				IP2Bus_Intr_FIFO_notEmpty,
//	 output				IP2Bus_Intr_FIFO_full,
//	 output				IP2Bus_Intr_BuffRdy,
//    output     		TXN_OUT,
//    output     		TXP_OUT,
	 output 	[7:0]		TRIGGER,				 // TRIGGER outputs
	 output  [7:0]		DIST_BUS,
//	 output 				evr125RfLockedClk,
	 output 	[63:0] 	TimeStamp,
	 input  				TsRequest,
	 output  			TsValid,
	 output  reg [63:0] 	TsLatched

//	 output				GtxRefClock,
//	 output CommaDetected,
//	 output LosOfLock,
//	 inout   [35:0]   CONTROL1,
//	 inout   [35:0]   CONTROL2,
//	 output [7:0] evr_leds
    );
	
	assign intr_FIFO_notEmpty = !timeStampFIFOempty;
	assign intr_FIFO_full = timeStampFIFOfull;
	assign intr_BuffRdy = PacketReady;
	parameter DEBUG = 0;	
	
	
//	wire ResetOut;
	wire LocalReset;
	wire [7:0] EventStream;
	wire [15:0] Events;

	wire [4:0] Position;

	wire eventClock;
	
	wire [15:0] mappingRamDataA, mappingRamDataB;
	wire			timeStampFIFOrdEna;
	wire 			timeStampFIFOfull;
	wire			timeStampFIFOempty;
	wire [71:0] timeStampFIFOdataOut;

	reg 			PacketStart;
	reg 			PacketStop;
	wire 			PacketReady;
	wire 			PacketFault;
	wire [11:0] PacketSize;
	wire [7:0] 	DataBufferData;
	wire [7:0]  DataStream;
	
    wire MultiplexDisBus;
    wire  DistBusClock;
	wire  mappingRamWrA;
	wire  mappingRamWrB;
	wire [15:0] EventsA;
	wire [15:0] EventsB;

	wire [63:0] timeStamp_bus;
	reg 			timeStampRegClkEna, timeStampRegClkEna_reg;
 	reg  [63:0] timeStampLatch;
	reg  [7:0]	eventStream_reg;
//	reg  [7:0]  DataStream_reg;
	
//	 reg	[31:0]	slv_reg0;			// channel 0 delay
//	 reg	[31:0]	slv_reg1;			// channel 0 width
//	 reg	[31:0]	slv_reg2;			// channel 1 delay
//	 reg	[31:0]	slv_reg3;			// channel 1 width
//	 reg	[31:0]	slv_reg4;			// channel 2 delay
//	 reg	[31:0]	slv_reg5;			// channel 2 width
//	 reg	[31:0]	slv_reg6;			// channel 3 delay
//	 reg	[31:0]	slv_reg7;			// channel 3 width
//	 reg	[31:0]	slv_reg8;			// channel 4 delay
//	 reg	[31:0]	slv_reg9;			// channel 4 width
//	 reg	[31:0]	slv_reg10;			// channel 5 delay
//	 reg	[31:0]	slv_reg11;			// channel 5 width
//	 reg	[31:0]	slv_reg12;     		// channel 6 delay
//	 reg	[31:0]	slv_reg13;			// channel 6 width
//	 reg	[31:0]	slv_reg14;			// channel 7 delay
//	 reg	[31:0]	slv_reg15;     		// channel 7 width
//	 reg	[31:0]	slv_reg16;			// all channels polarity
//	 reg	[31:0]	slv_reg17;			// select map ram A (0) or B (1) multiplex distributed bus (2) 
//	 reg	[31:0]	slv_reg18;			
//	 reg	[31:0]	slv_reg19;
//	 reg	[31:0]	slv_reg20;
//	 reg	[31:0]	slv_reg21;
//	 reg	[31:0]	slv_reg22;
//	 reg	[31:0]	slv_reg23;     		// data buffer size, fault, ready
//	 reg	[31:0]	slv_reg24;			// Time Stamp seconds
//	 reg	[31:0]	slv_reg25;			// Time Stamp offset
//	 reg	[31:0]	slv_reg26;			// Time Stamp reg seconds (latched on event) 
//	 reg	[31:0]	slv_reg27;			// Time Stamp reg offset
//	 reg	[31:0]	slv_reg28;			// Time Stamp FIFO flags
// 	 reg	[31:0]	slv_reg29;			// Time Stamp FIFO seconds
//	 reg	[31:0]	slv_reg30;			// Time Stamp FIFO offset
//	 reg	[31:0]	slv_reg31;			// Time Stamp FIFO event
	 
	 wire  [31:0]  reg_sel;


    /////////////////////////////////
    // Generic Bus to EVR interface logic
    
    
    //  Address Decoding
    //  ================
    //  This component consumes 32kB of address space.
    //  Decoding within that space is:
    //   --+-----+--------------------------------+-----+
    //  ...|14 13|12 11 10 09 08 07 06 05 04 03 02|01 00|
    //   --+-----+--------------------------------+-----+
    //        ^                    ^                 ^
    //        |                    |                 |
    //        |                    |                 +-- Byte select (always 00)
    //        |                    | 
    //        |                    +-------------------- Index (10 bits)
    //        |
    //        +------------------------------------------ Bank select
    //                                                  00 - Registers
    //                                                  01 - Mapping RAM A
    //                                                  10 - Mapping RAM B
    //                                                  11 - Distributed data buffer
    wire regBankSel, mapABankSel, mapBBankSel, dataBufBankSel;
    assign regBankSel     = bus_addr[14:13] == 2'b00;
    assign mapABankSel    = bus_addr[14:13] == 2'b01;
    assign mapBBankSel    = bus_addr[14:13] == 2'b10;
    assign dataBufBankSel = bus_addr[14:13] == 2'b11;
    wire [2:0] evrChipSelects;
    assign evrChipSelects = { dataBufBankSel, mapBBankSel, mapABankSel };
    
    //
    // Read data multiplexer is registered, so:
    //   - Register block has 1 cycle read latency.
    //   - Memory blocks have 3 cycle read latency.
    // No write latency.
    //
    reg bus_rd_en_ff1;
    reg bus_rd_en_ff2;
//    reg rd_ack;
    reg bus_rd_en_1sh;
    reg bus_rd_en_1sh_dly;
    reg bus_rd_en_1sh_dly2;
    always @(posedge bus_clk) begin
        if (bus_reset_n == 0) begin
            bus_rd_en_ff1 <= 0;
            bus_rd_en_ff2 <= 0;
//            bus_rd_en_ff3 <= 0;
            bus_rd_en_1sh <= 0;
        end
        else begin
            bus_rd_en_ff1 <= bus_rd_en;
            bus_rd_en_ff2 <= bus_rd_en_ff1;
//            bus_rd_en_ff3 <= bus_rd_en_ff2;
            bus_rd_en_1sh <= bus_rd_en & ~bus_rd_en_ff1;
            bus_rd_en_1sh_dly <= bus_rd_en_1sh;
            bus_rd_en_1sh_dly2 <= bus_rd_en_1sh_dly;
        end
    end
//    assign bus_rd_en_1sh = regBankSel ? (bus_rd_en_ff1 & ~bus_rd_en) : (bus_rd_en_ff2 & ~bus_rd_en_ff1);
    assign bus_rd_ack = regBankSel ? bus_rd_en_1sh : bus_rd_en_1sh_dly2;
    
    //
    // Multiplex read lines from all blocks.
    // Register mux output to help this meet timing requirements.
    //
    reg  [31:0] readData, regReadData;
    wire [31:0] evrReadData;
    always @(posedge bus_clk) begin
        if (regBankSel) begin
            readData <= regReadData;
        end
        else begin
            readData <= evrReadData;
        end
    end
    assign bus_rd_data = readData;
    
    //
    // Register bank
    //
    reg [31:0] reg00, reg01, reg02, reg03, reg04, reg05, reg06, reg07, reg08, reg09;
    reg [31:0] reg10, reg11, reg12, reg13, reg14, reg15, reg16, reg17, reg18, reg19;
    reg [31:0] reg20, reg21, reg22;
    wire [31:0] reg23, reg24, reg25, reg26, reg27, reg28, reg29, reg30, reg31;
    wire [31:0] regReadSelects;
    wire [31:0] regWriteSelects;
    genvar i;
    
    generate
    for (i = 0 ; i < 32 ; i = i + 1) begin:regSel
        assign regReadSelects[i] = bus_rd_en && (bus_addr[14:13] == 2'd0)
                                               && (bus_addr[6:2] == i);
        assign regWriteSelects[i] = bus_wr_en && (bus_addr[14:13] == 2'd0)
                                                && (bus_addr[6:2] == i);
    end
    endgenerate
    
    // Register write decoder
    always @(posedge bus_clk) begin
        if (bus_reset_n == 0) begin
            reg00 <= 0; reg01 <= 0; reg02 <= 0; reg03 <= 0;
            reg04 <= 0; reg05 <= 0; reg06 <= 0; reg07 <= 0;
            reg08 <= 0; reg09 <= 0; reg10 <= 0; reg11 <= 0;
            reg12 <= 0; reg13 <= 0; reg14 <= 0; reg15 <= 0;
            reg16 <= 0; reg17 <= 0; reg18 <= 0; reg19 <= 0;
            reg20 <= 0; reg21 <= 0; reg22 <= 0;
        end
        else begin
            if (bus_wr_en && regBankSel) begin
                if (bus_addr[6:2] == 5'd00) reg00 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd01) reg01 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd02) reg02 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd03) reg03 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd04) reg04 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd05) reg05 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd06) reg06 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd07) reg07 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd08) reg08 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd09) reg09 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd10) reg10 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd11) reg11 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd12) reg12 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd13) reg13 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd14) reg14 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd15) reg15 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd16) reg16 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd17) reg17 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd18) reg18 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd19) reg19 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd20) reg20 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd21) reg21 <= bus_wr_data;
                if (bus_addr[6:2] == 5'd22) reg22 <= bus_wr_data;
            end
        end
    end
    
    // Register read multiplexer
    always @(bus_addr or
             reg00 or reg01 or reg02 or reg03 or reg04 or
             reg05 or reg06 or reg07 or reg08 or reg09 or
             reg10 or reg11 or reg12 or reg13 or reg14 or
             reg15 or reg16 or reg17 or reg18 or reg19 or
             reg20 or reg21 or reg22 or reg23 or reg24 or
             reg25 or reg26 or reg27 or reg28 or reg29 or
             reg30 or reg31) begin
        case (bus_addr[6:2])
        5'd00: regReadData = reg00;
        5'd01: regReadData = reg01;
        5'd02: regReadData = reg02;
        5'd03: regReadData = reg03;
        5'd04: regReadData = reg04;
        5'd05: regReadData = reg05;
        5'd06: regReadData = reg06;
        5'd07: regReadData = reg07;
        5'd08: regReadData = reg08;
        5'd09: regReadData = reg09;
        5'd10: regReadData = reg10;
        5'd11: regReadData = reg11;
        5'd12: regReadData = reg12;
        5'd13: regReadData = reg13;
        5'd14: regReadData = reg14;
        5'd15: regReadData = reg15;
        5'd16: regReadData = reg16;
        5'd17: regReadData = reg17;
        5'd18: regReadData = reg18;
        5'd19: regReadData = reg19;
        5'd20: regReadData = reg20;
        5'd21: regReadData = reg21;
        5'd22: regReadData = reg22;
        5'd23: regReadData = reg23;
        5'd24: regReadData = reg24;
        5'd25: regReadData = reg25;
        5'd26: regReadData = reg26;
        5'd27: regReadData = reg27;
        5'd28: regReadData = reg28;
        5'd29: regReadData = reg29;
        5'd30: regReadData = reg30;
        5'd31: regReadData = reg31;
        default: regReadData = 32'bx;
        endcase
    end

	assign LocalReset = !mgt_reset_done;
    irq_forward irq_forward (.clk_in(mgt_rec_clk),
                             .irq_in(Events[15]),
                             .clk_out(bus_clk),
                             .irq_out(intr_Event));
	assign reg23 = {16'hA6A4,2'h0,PacketSize,PacketFault,PacketReady};
	assign reg24 = timeStamp_bus[63:32];
	assign reg25 = timeStamp_bus[31:0];
	assign reg26 = timeStampLatch[63:32];
	assign reg27 = timeStampLatch[31:0];
	assign reg28 = {30'd0,timeStampFIFOfull,timeStampFIFOempty};
	assign reg29 = timeStampFIFOdataOut[71:40];
	assign reg30 = timeStampFIFOdataOut[39:8];
	assign reg31 = {24'd0,timeStampFIFOdataOut[7:0]};
	assign DIST_BUS  = DataStream;
	
	assign evrReadData = evrChipSelects[0] ? {16'd0,mappingRamDataA}: (evrChipSelects[1] ? {16'd0,mappingRamDataB} : {24'd0,DataBufferData});
	assign mappingRamWrA = (evrChipSelects[0] & bus_wr_en);
	assign mappingRamWrB = (evrChipSelects[1] & bus_wr_en);
	assign Events = reg17[0] ? EventsB : EventsA;
	
	assign EventStream = mgt_par_data[7:0];
	assign DataStream = mgt_par_data[15:8];
	
	parameter Kstart = 8'b00011100; // start of packet
    parameter Kstop  = 8'b00111100; // end of packet

    reg distBusClockDiv;
    
    always @ (posedge mgt_rec_clk) begin
        if ((mgt_charisk == 2'b10) && (DataStream == Kstart))
            PacketStart <= 1'b1;
        else if ((mgt_charisk == 2'b10) && (DataStream == Kstop))
            PacketStop <= 1'b1;
        else begin
            PacketStart <= 1'b0;
            PacketStop <= 1'b0;
        end
    end
    
    // distributed bus clock conditioning circuit
    assign MultiplexDisBus = reg17[2];
    assign DistBusClock = MultiplexDisBus ? mgt_rec_clk : distBusClockDiv;
    always @ (posedge mgt_rec_clk) begin
        if (mgt_chariscomma[0] == 1'b1) distBusClockDiv <= 1'b0;
        else distBusClockDiv <= !distBusClockDiv;
    end
//	IBUFDS_GTXE1 q4_clk0_refclk_ibufds_i
//    (
//        .O                              (GtxRefClock),
//        .ODIV2                          (),
//        .CEB                            (tied_to_ground_i),
//        .I                              (TILE2_REFCLK_PAD_P_IN),
//        .IB                             (TILE2_REFCLK_PAD_N_IN)
//    );

	// if the distributed bus is multiplexed with the data packet
	// transmit then the distributed bus can only be sampled at
	// half the 125 MHz clock rate. Dist bus clock is divided if
	// data transmission is enabled (reg 17 bit 2)
//	always @ (posedge DistBusClock) begin
//        if (mgt_bitslide_sync)
//            DataStream_reg <= DataStream;
//    end
	
	// Get time stamp into system clock domain
    timestamp_forward timestamp_forward (.clk_in(mgt_rec_clk),
                                         .timestamp_in(TimeStamp),
                                         .clk_out(bus_clk),
                                         .timestamp_out(timeStamp_bus));
	
	// create a latched version of the time stamp stored when bit 14
	// of the mapping ram is '1'
	always @ (posedge mgt_rec_clk)
	begin
		if (LocalReset) timeStampLatch <= 64'd0;
		else if (Events[14]) timeStampLatch <= TimeStamp;
	end

	// delay the event stream input into the FIFO by one clock tick
//	always @ (posedge mgt_rec_clk) eventStream_reg <= mgt_bitslide_sync ? EventStream : 8'h0;
	always @ (posedge mgt_rec_clk) eventStream_reg <= EventStream;
	
	// state machine to pop the next value off the time stamp FIFO after
	// reading the event from the FIFO
   parameter IDLE = 3'b001;
   parameter READ = 3'b010;
   parameter FTCH = 3'b100;

   (* FSM_ENCODING="ONE-HOT", SAFE_IMPLEMENTATION="NO" *) reg [2:0] state = IDLE;

   always@(posedge bus_clk)
      if (LocalReset) begin
         state <= IDLE;
      end
      else
         (* FULL_CASE, PARALLEL_CASE *)case (state)
            IDLE : begin
               if (regReadSelects[31] == 1'b1)
                  state <= READ;
               else
                  state <= IDLE;
            end
            READ : begin
               if (regReadSelects[31] == 1'b0)
                  state <= FTCH;
               else
                  state <= READ;
            end
            FTCH : begin
                  state <= IDLE;
            end
         endcase

	assign timeStampFIFOrdEna = (state == FTCH);

	// Request time stamp from arbitrary clock domain.
    // To avoid race conditions delay setting TsValid by one clock cycle.
	reg TsRequest_reg, TsRequest_reg_d;
	always @ (posedge mgt_rec_clk)
	begin
		if (LocalReset) TsRequest_reg <= 1'b0;
		else TsRequest_reg <= TsRequest;
        TsRequest_reg_d <= TsRequest_reg;
        if (!TsRequest_reg) TsLatched <= TimeStamp;
	end
	assign TsValid = TsRequest & TsRequest_reg && TsRequest_reg_d;


	// instances of an event reciever channel can be added as the sytem
	// dictates the clock input is recovered from the serial data stream
	// and is synchonized to the beam therefore the TRIGGER output is as well
	EventReceiverChannel EventReceiverChannel_0 (
		 .Clock(mgt_rec_clk), 
		 .Reset((LocalReset || regWriteSelects[0] || regWriteSelects[1] || regWriteSelects[16])), 
		 .myEvent(Events[0]),
		 .myDelay(reg00), 
		 .myWidth(reg01), 
		 .myPolarity(reg16[0]), 
		 .trigger(TRIGGER[0])
		 );

	EventReceiverChannel EventReceiverChannel_1 (
		 .Clock(mgt_rec_clk), 
		 .Reset((LocalReset || regWriteSelects[2] || regWriteSelects[3] || regWriteSelects[16])), 
		 .myEvent(Events[1]), 
		 .myDelay(reg02), 
		 .myWidth(reg03), 
		 .myPolarity(reg16[1]), 
		 .trigger(TRIGGER[1])
		 );

	EventReceiverChannel EventReceiverChannel_2 (
		 .Clock(mgt_rec_clk), 
		 .Reset((LocalReset || regWriteSelects[4] || regWriteSelects[5] || regWriteSelects[16])), 
		 .myEvent(Events[2]), 
		 .myDelay(reg04), 
		 .myWidth(reg05), 
		 .myPolarity(reg16[2]), 
		 .trigger(TRIGGER[2])
		 );

	EventReceiverChannel EventReceiverChannel_3 (
		 .Clock(mgt_rec_clk), 
		 .Reset((LocalReset || regWriteSelects[6] || regWriteSelects[7] || regWriteSelects[16])), 
		 .myEvent(Events[3]), 
		 .myDelay(reg06), 
		 .myWidth(reg07), 
		 .myPolarity(reg16[3]), 
		 .trigger(TRIGGER[3])
		 );

	EventReceiverChannel EventReceiverChannel_4 (
		 .Clock(mgt_rec_clk), 
		 .Reset((LocalReset || regWriteSelects[8] || regWriteSelects[9] || regWriteSelects[16])), 
		 .myEvent(Events[4]), 
		 .myDelay(reg08), 
		 .myWidth(reg09), 
		 .myPolarity(reg16[4]), 
		 .trigger(TRIGGER[4])
		 );

	EventReceiverChannel EventReceiverChannel_5 (
		 .Clock(mgt_rec_clk), 
		 .Reset((LocalReset || regWriteSelects[10] || regWriteSelects[11] || regWriteSelects[16])), 
		 .myEvent(Events[5]), 
		 .myDelay(reg10), 
		 .myWidth(reg11), 
		 .myPolarity(reg16[5]), 
		 .trigger(TRIGGER[5])
		 );

	EventReceiverChannel EventReceiverChannel_6 (
		 .Clock(mgt_rec_clk), 
		 .Reset((LocalReset || regWriteSelects[12] || regWriteSelects[13] || regWriteSelects[16])), 
		 .myEvent(Events[6]),
		 .myDelay(reg12), 
		 .myWidth(reg13), 
		 .myPolarity(reg16[6]), 
		 .trigger(TRIGGER[6])
		 );

	EventReceiverChannel EventReceiverChannel_7 (
		 .Clock(mgt_rec_clk), 
		 .Reset((LocalReset || regWriteSelects[14] || regWriteSelects[15] || regWriteSelects[16])), 
		 .myEvent(Events[7]), 
		 .myDelay(reg14), 
		 .myWidth(reg15), 
		 .myPolarity(reg16[7]), 
		 .trigger(TRIGGER[7])
		 );

//assign IP2Bus_Intr_LOL = ~mgt_bitslide_sync;
//assign LosOfLock = IP2Bus_Intr_LOL;
//wire [4:0] bitslide_count;
//wire bitslide_sync;
//wire [7:0] serial_receiver_debug_out;

//	serialReceiver mySerialReceiver (
//		 .ClockIn(GtxRefClock), 
//		 .evr125RfLockedClk(evr125RfLockedClk),
//		 .DistBusClock(DistBusClock),
//		 .MultiplexDisBus(slv_reg17[2]),
//		 .ResetIn(RESET), 
//		 .ResetOut(ResetOut), 
//		 .EventStreamOut({DataStream[7:0],EventStream[7:0]}),
//		 .PacketStart(PacketStart),
//	     .PacketStop(PacketStop),
//		 .rxLossOfSync(rxLossOfSync),
//		 .CommaDetected(CommaDetected),
//		 .BitslideCount(bitslide_count),
//		 .BitslideSync(bitslide_sync),
//		 .DebugOut(serial_receiver_debug_out),
//		 .RXN_IN(RXN_IN), 
//		 .RXP_IN(RXP_IN), 
//		 .TXN_OUT(TXN_OUT), 
//		 .TXP_OUT(TXP_OUT),
//		 .CONTROL0(CONTROL2)
//		 );

	MappingRam MyMappingRamA (
		.clka(bus_clk),
		.wea(mappingRamWrA),
		.addra(bus_addr[9:2]),
		.dina(bus_wr_data[15:0]), 
		.douta(mappingRamDataA),
		.clkb(mgt_rec_clk),
		.web(1'b0),
		.addrb(EventStream),
		.dinb(16'd0),
		.doutb(EventsA));

	MappingRam MyMappingRamB (
		.clka(bus_clk),
		.wea(mappingRamWrB),
		.addra(bus_addr[9:2]),
		.dina(bus_wr_data[15:0]),
		.douta(mappingRamDataB),
		.clkb(mgt_rec_clk),
		.web(1'b0),
		.addrb(EventStream),
		.dinb(16'd0),
		.doutb(EventsB));
	 
	timeofDayReceiver myTimeofDayReceiver (
		 .Clock(mgt_rec_clk), 
		 .Reset(LocalReset), 
		 .EventStream(EventStream), 
		 .TimeStamp(TimeStamp), 
		 .Seconds(), 
		 .Offset(), 
		 .Position(Position), 
		 .eventClock(eventClock));

	timeStampFIFO myTimeStampFIFO (
	    .rst(LocalReset), 
	    .wr_clk(mgt_rec_clk),
	    .rd_clk(bus_clk),
	    .din({TimeStamp,eventStream_reg}),
	    .wr_en(Events[13]),
	    .rd_en(timeStampFIFOrdEna),
	    .dout(timeStampFIFOdataOut),
	    .full(timeStampFIFOfull),
	    .empty(timeStampFIFOempty));
		 
	DataBufferCntrlr myDataBufferCntrlr (
		 .RxClock(mgt_rec_clk), 
		 .SysClock(bus_clk), 
		 .Reset(LocalReset), 
		 .PacketStart(PacketStart), 
		 .PacketStop(PacketStop), 
		 .DataIn(DataStream[7:0]), 
		 .Address(bus_addr[12:2]), 
		 .ReadEna(evrChipSelects[2]), 
		 .PacketReady(PacketReady), 
		 .PacketSize(PacketSize), 
		 .DataOut(DataBufferData), 
		 .PacketFault(PacketFault)
		 );


//assign evr_leds = serial_receiver_debug_out;

//	generate
//	if (DEBUG) 
//		begin : chipscope
		
//		reg [2:0]quickCount;
//		always @(posedge bus_clk) quickCount <= quickCount + 1;

//		reg [2:0]quickCount2;
//		always @(posedge mgt_rec_clk) quickCount2 <= quickCount2 + 1;
		
//		wire [255:0] TRIG0;
		
//		assign TRIG0[63:0] 		= TimeStamp;
//		assign TRIG0[95:64] 		= bus_rd_data;
//		assign TRIG0[127:96]		= {16'd0,mappingRamDataA};
//		assign TRIG0[143:128]	= Events;
//		assign TRIG0[144] 		= evrChipSelects[0];
//		assign TRIG0[145] 		= evrChipSelects[1];
//		assign TRIG0[146] 		= evrChipSelects[2];
//		assign TRIG0[147] 		= Bus2IP_RNW;
//		assign TRIG0[148] 		= mappingRamWrA;
//		assign TRIG0[149]			= PacketStart;
//		assign TRIG0[150]			= PacketStop;
//		assign TRIG0[151]			= PacketReady;
//		assign TRIG0[152]			= PacketFault;
//		assign TRIG0[153]			= Bus2IP_RdCE[29];
//		assign TRIG0[154]			= Bus2IP_RdCE[30];
//		assign TRIG0[155]			= Bus2IP_RdCE[31];
//		assign TRIG0[156]			= bus_addr[22];
//		assign TRIG0[164:157] 	= EventStream;	
//		assign TRIG0[196:165] 	= timeStampFIFOdataOut[71:40];	
//		assign TRIG0[228:197]	= timeStampFIFOdataOut[39:8];
//		assign TRIG0[236:229]	= timeStampFIFOdataOut[7:0];
//		assign TRIG0[237]			= timeStampFIFOrdEna;
//		assign TRIG0[255:238] 	= 0;		
		
//		ila myILA (
//		 .CONTROL(CONTROL1), // INOUT BUS [35:0]
//		 .CLK(bus_clk), // IN
//		 .TRIG0(TRIG0) // IN BUS [99:0]
//	);
//	end
//	endgenerate
endmodule
