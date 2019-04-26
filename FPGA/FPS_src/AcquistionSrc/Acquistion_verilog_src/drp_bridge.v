`timescale 1ns/1ns

module drp_bridge #(
  parameter DRP_COUNT            = 4, 
  parameter DRP_ADDR_WIDTH       = 9,                
  parameter DRP_DATA_WIDTH       = 16,
  parameter S_AXI_ADDR_WIDTH     = 32,               
  parameter S_AXI_DATA_WIDTH     = 32
  
)(

  //--------------  AXI Interface Signals         --------------
  input                             AXI_aclk,
  input                             AXI_aresetn,
  
  input  [32-1:0]                   S_AXI_araddr,
  output reg                        S_AXI_arready,
  input                             S_AXI_arvalid,
  input  [2:0]                      S_AXI_arprot,
  
  input [32-1:0]                    S_AXI_awaddr,
  output reg                        S_AXI_awready,
  input                             S_AXI_awvalid,
  input  [2:0]                      S_AXI_awprot,
  
  output reg [1:0]                  S_AXI_bresp,  
  input                             S_AXI_bready,
  output reg                        S_AXI_bvalid,
  
  output reg [S_AXI_DATA_WIDTH-1:0] S_AXI_rdata,
  input                             S_AXI_rready,
  output reg                        S_AXI_rvalid,
  output reg [1:0]                  S_AXI_rresp,
  
  input  [S_AXI_DATA_WIDTH-1:0]     S_AXI_wdata,
  output reg                        S_AXI_wready,
  input                             S_AXI_wvalid,
  input  [S_AXI_DATA_WIDTH/8-1:0]   S_AXI_wstrb,
  
  //-------------- Dynamic Reconfiguration Port (DRP) --------------
  
  output                                      drp0_en,
  output                                      drp0_we,
  output    [DRP_ADDR_WIDTH-1: 0]             drp0_addr,
  output    [DRP_DATA_WIDTH-1: 0]             drp0_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp0_do,
  input                                       drp0_rdy,  

  output                                      drp1_en,
  output                                      drp1_we,
  output    [DRP_ADDR_WIDTH-1: 0]             drp1_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp1_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp1_do,
  input                                       drp1_rdy,  

  output                                      drp2_en,
  output                                      drp2_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp2_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp2_di,            
  input  [DRP_DATA_WIDTH-1: 0]                drp2_do,
  input                                       drp2_rdy,
  
  output                                      drp3_en,
  output                                      drp3_we,
  output    [DRP_ADDR_WIDTH-1: 0]             drp3_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp3_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp3_do,
  input                                       drp3_rdy,
  
  output                                      drp4_en,
  output                                      drp4_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp4_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp4_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp4_do,
  input                                       drp4_rdy,

  output                                      drp5_en,
  output                                      drp5_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp5_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp5_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp5_do,
  input                                       drp5_rdy,    
  
  output                                      drp6_en,
  output                                      drp6_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp6_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp6_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp6_do,
  input                                       drp6_rdy,

  output                                      drp7_en,
  output                                      drp7_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp7_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp7_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp7_do,
  input                                       drp7_rdy,

  output                                      drp8_en,
  output                                      drp8_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp8_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp8_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp8_do,
  input                                       drp8_rdy,

  output                                      drp9_en,
  output                                      drp9_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp9_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp9_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp9_do,
  input                                       drp9_rdy,

  output                                      drp10_en,
  output                                      drp10_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp10_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp10_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp10_do,
  input                                       drp10_rdy,  

  output                                      drp11_en,
  output                                      drp11_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp11_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp11_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp11_do,
  input                                       drp11_rdy,

  output                                      drp12_en,
  output                                      drp12_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp12_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp12_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp12_do,
  input                                       drp12_rdy,

  output                                      drp13_en,
  output                                      drp13_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp13_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp13_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp13_do,
  input                                       drp13_rdy,

  output                                      drp14_en,
  output                                      drp14_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp14_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp14_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp14_do,
  input                                       drp14_rdy,

  output                                      drp15_en,
  output                                      drp15_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp15_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp15_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp15_do,
  input                                       drp15_rdy,

  output                                      drp16_en,
  output                                      drp16_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp16_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp16_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp16_do,
  input                                       drp16_rdy,

  output                                      drp17_en,
  output                                      drp17_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp17_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp17_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp17_do,
  input                                       drp17_rdy,

  output                                      drp18_en,
  output                                      drp18_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp18_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp18_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp18_do,
  input                                       drp18_rdy,

  output                                      drp19_en,
  output                                      drp19_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp19_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp19_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp19_do,
  input                                       drp19_rdy,

  output                                      drp20_en,
  output                                      drp20_we,
  output    [DRP_ADDR_WIDTH-1: 0]             drp20_addr,
  output    [DRP_DATA_WIDTH-1: 0]             drp20_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp20_do,
  input                                       drp20_rdy,  

  output                                      drp21_en,
  output                                      drp21_we,
  output    [DRP_ADDR_WIDTH-1: 0]             drp21_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp21_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp21_do,
  input                                       drp21_rdy,  

  output                                      drp22_en,
  output                                      drp22_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp22_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp22_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp22_do,
  input                                       drp22_rdy,
  
  output                                      drp23_en,
  output                                      drp23_we,
  output    [DRP_ADDR_WIDTH-1: 0]             drp23_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp23_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp23_do,
  input                                       drp23_rdy,
  
  output                                      drp24_en,
  output                                      drp24_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp24_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp24_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp24_do,
  input                                       drp24_rdy,

  output                                      drp25_en,
  output                                      drp25_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp25_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp25_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp25_do,
  input                                       drp25_rdy,    
  
  output                                      drp26_en,
  output                                      drp26_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp26_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp26_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp26_do,
  input                                       drp26_rdy,

  output                                      drp27_en,
  output                                      drp27_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp27_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp27_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp27_do,
  input                                       drp27_rdy,

  output                                      drp28_en,
  output                                      drp28_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp28_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp28_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp28_do,
  input                                       drp28_rdy,

  output                                      drp29_en,
  output                                      drp29_we,
  output     [DRP_ADDR_WIDTH-1: 0]            drp29_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp29_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp29_do,
  input                                       drp29_rdy,

  output                                      drp30_en,
  output                                      drp30_we,
  output    [DRP_ADDR_WIDTH-1: 0]             drp30_addr,
  output    [DRP_DATA_WIDTH-1: 0]             drp30_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp30_do,
  input                                       drp30_rdy,  

  output                                      drp31_en,
  output                                      drp31_we,
  output    [DRP_ADDR_WIDTH-1: 0]             drp31_addr,
  output     [DRP_DATA_WIDTH-1: 0]            drp31_di,
  input  [DRP_DATA_WIDTH-1: 0]                drp31_do,
  input                                       drp31_rdy  
  
  ) ; // drp_bridge
        

function integer clogb2;
    input [31:0] value;
    begin
        value = value - 1;
        for (clogb2 = 0; value > 0; clogb2 = clogb2 + 1) begin
            value = value >> 1;
        end
    end
endfunction

localparam MAXDRPs = 32;


//This is done due to limitations in the Packager
reg   [MAXDRPs-1:0] drp_en;
reg   [MAXDRPs-1:0] drp_we;
wire  [MAXDRPs-1:0] drp_rdy;
wire  [MAXDRPs*DRP_DATA_WIDTH-1:0] drp_do;
reg   [DRP_DATA_WIDTH-1:0] drp_di;
reg   [DRP_ADDR_WIDTH-1:0] drp_addr;

assign drp0_en = drp_en[0];
assign drp1_en = drp_en[1];
assign drp2_en = drp_en[2];
assign drp3_en = drp_en[3];
assign drp4_en = drp_en[4];
assign drp5_en = drp_en[5];
assign drp6_en = drp_en[6];
assign drp7_en = drp_en[7];
assign drp8_en = drp_en[8];
assign drp9_en = drp_en[9];
assign drp10_en = drp_en[10];
assign drp11_en = drp_en[11];
assign drp12_en = drp_en[12];
assign drp13_en = drp_en[13];
assign drp14_en = drp_en[14];
assign drp15_en = drp_en[15];
assign drp16_en = drp_en[16];
assign drp17_en = drp_en[17];
assign drp18_en = drp_en[18];
assign drp19_en = drp_en[19];
assign drp20_en = drp_en[20];
assign drp21_en = drp_en[21];
assign drp22_en = drp_en[22];
assign drp23_en = drp_en[23];
assign drp24_en = drp_en[24];
assign drp25_en = drp_en[25];
assign drp26_en = drp_en[26];
assign drp27_en = drp_en[27];
assign drp28_en = drp_en[28];
assign drp29_en = drp_en[29];
assign drp30_en = drp_en[30];
assign drp31_en = drp_en[31];

assign drp0_we = drp_we[0];
assign drp1_we = drp_we[1];
assign drp2_we = drp_we[2];
assign drp3_we = drp_we[3];
assign drp4_we = drp_we[4];
assign drp5_we = drp_we[5];
assign drp6_we = drp_we[6];
assign drp7_we = drp_we[7];
assign drp8_we = drp_we[8];
assign drp9_we = drp_we[9];
assign drp10_we = drp_we[10];
assign drp11_we = drp_we[11];
assign drp12_we = drp_we[12];
assign drp13_we = drp_we[13];
assign drp14_we = drp_we[14];
assign drp15_we = drp_we[15];
assign drp16_we = drp_we[16];
assign drp17_we = drp_we[17];
assign drp18_we = drp_we[18];
assign drp19_we = drp_we[19];
assign drp20_we = drp_we[20];
assign drp21_we = drp_we[21];
assign drp22_we = drp_we[22];
assign drp23_we = drp_we[23];
assign drp24_we = drp_we[24];
assign drp25_we = drp_we[25];
assign drp26_we = drp_we[26];
assign drp27_we = drp_we[27];
assign drp28_we = drp_we[28];
assign drp29_we = drp_we[29];
assign drp30_we = drp_we[30];
assign drp31_we = drp_we[31]; 

assign drp0_di = drp_di;
assign drp1_di = drp_di;
assign drp2_di = drp_di;
assign drp3_di = drp_di;
assign drp4_di = drp_di;
assign drp5_di = drp_di;
assign drp6_di = drp_di;
assign drp7_di = drp_di;
assign drp8_di = drp_di;
assign drp9_di = drp_di;
assign drp10_di = drp_di;
assign drp11_di = drp_di;
assign drp12_di = drp_di;
assign drp13_di = drp_di;
assign drp14_di = drp_di;
assign drp15_di = drp_di;
assign drp16_di = drp_di;
assign drp17_di = drp_di;
assign drp18_di = drp_di;
assign drp19_di = drp_di;
assign drp20_di = drp_di;
assign drp21_di = drp_di;
assign drp22_di = drp_di;
assign drp23_di = drp_di;
assign drp24_di = drp_di;
assign drp25_di = drp_di;
assign drp26_di = drp_di;
assign drp27_di = drp_di;
assign drp28_di = drp_di;
assign drp29_di = drp_di;
assign drp30_di = drp_di;
assign drp31_di = drp_di; 

assign drp0_addr = drp_addr;
assign drp1_addr = drp_addr;
assign drp2_addr = drp_addr;
assign drp3_addr = drp_addr;
assign drp4_addr = drp_addr;
assign drp5_addr = drp_addr;
assign drp6_addr = drp_addr;
assign drp7_addr = drp_addr;
assign drp8_addr = drp_addr;
assign drp9_addr = drp_addr;
assign drp10_addr = drp_addr;
assign drp11_addr = drp_addr;
assign drp12_addr = drp_addr;
assign drp13_addr = drp_addr;
assign drp14_addr = drp_addr;
assign drp15_addr = drp_addr;
assign drp16_addr = drp_addr;
assign drp17_addr = drp_addr;
assign drp18_addr = drp_addr;
assign drp19_addr = drp_addr;
assign drp20_addr = drp_addr;
assign drp21_addr = drp_addr;
assign drp22_addr = drp_addr;
assign drp23_addr = drp_addr;
assign drp24_addr = drp_addr;
assign drp25_addr = drp_addr;
assign drp26_addr = drp_addr;
assign drp27_addr = drp_addr;
assign drp28_addr = drp_addr;
assign drp29_addr = drp_addr;
assign drp30_addr = drp_addr;
assign drp31_addr = drp_addr; 

assign drp_rdy  = {   drp31_rdy, drp30_rdy, drp29_rdy, drp28_rdy, drp27_rdy, drp26_rdy, drp25_rdy, drp24_rdy, drp23_rdy, drp22_rdy, drp21_rdy, drp20_rdy,  
                    drp19_rdy, drp18_rdy, drp17_rdy, drp16_rdy, drp15_rdy, drp14_rdy, drp13_rdy, drp12_rdy, drp11_rdy, drp10_rdy, 
                    drp9_rdy, drp8_rdy, drp7_rdy, drp6_rdy, drp5_rdy, drp4_rdy, drp3_rdy, drp2_rdy, drp1_rdy, drp0_rdy};
assign drp_do   = { drp31_do, drp30_do, drp29_do, drp28_do, drp27_do, drp26_do, drp25_do, drp24_do, drp23_do, drp22_do, drp21_do, drp20_do, 
                    drp19_do, drp18_do, drp17_do, drp16_do, drp15_do, drp14_do, drp13_do, drp12_do, drp11_do, drp10_do, 
                    drp9_do, drp8_do, drp7_do, drp6_do, drp5_do, drp4_do, drp3_do, drp2_do, drp1_do, drp0_do};



localparam dly = 1;
localparam extra_addr_bits         = clogb2(DRP_COUNT);
localparam extra_addr_bits_plusone = clogb2(DRP_COUNT+1);
reg read_pending;

reg [extra_addr_bits-1:0] drp_select;
wire drp_write_done;
wire drp_read_done;
reg drp_we_r;
reg drp_en_r;
wire bad_wr_addr;
wire bad_rd_addr;
wire bad_wr_addr_r;
wire bad_rd_addr_r;



//+---------------------------------------------------------------------------+
//|                paramaterizable MUX                                        |
//+---------------------------------------------------------------------------+
integer ii;
reg     [DRP_DATA_WIDTH-1:0] drp_do_array [0:DRP_COUNT-1];

always @*
    for(ii=0; ii<DRP_COUNT; ii=ii+1) begin
        drp_do_array[ii] = drp_do[(ii*DRP_DATA_WIDTH)+:DRP_DATA_WIDTH];
    end 



//+---------------------------------------------------------------+
//|       Synchronizing the Reset                                 |
//+---------------------------------------------------------------+
reg AXI_aresetn_r1 ;
reg AXI_aresetn_r2 ;
wire AXI_sreset ;

always @ (posedge AXI_aclk or negedge AXI_aresetn)
begin
 if (AXI_aresetn == 1'b0) begin
  AXI_aresetn_r1                   <= #(dly) 1'd1;
  AXI_aresetn_r2                   <= #(dly) 1'd1;
 end
 else begin
  AXI_aresetn_r1                   <= #(dly) 1'b0;
  AXI_aresetn_r2                   <= #(dly) AXI_aresetn_r1;
 end
end

assign AXI_sreset = AXI_aresetn_r2;                
       
//State Machine Signals:
  localparam idle_state           = 3'b000;
  localparam drp_write_state      = 3'b001;
  localparam drp_write_data_latch = 3'b010;
  localparam drp_write_wait       = 3'b011;
  localparam write_response       = 3'b100;
  localparam drp_read_state       = 3'b101;
  localparam drp_read_wait        = 3'b110;
  reg [2:0] state_wr              = idle_state;
  
//+---------------------------------------------------------------------------+
//|                 Main State Machine                                        |
//+---------------------------------------------------------------------------+

reg [2:0] state_wr_r;
always @ (posedge AXI_aclk)
begin
  if ( AXI_sreset == 1'b1) 
    state_wr_r         <= #(dly) 0;
  else
    state_wr_r         <= #(dly) state_wr;
end

always @(posedge AXI_aclk)
begin
 if ( AXI_sreset == 1'b1) begin
   state_wr <= #(dly) idle_state;
 end
 else begin
   case (state_wr)
     idle_state : begin  //wait for a command to come in
       if (S_AXI_awvalid) //AXI has independant channels; need to handle both
          state_wr <= #(dly) drp_write_state;
       else if(S_AXI_arvalid)
          state_wr <= #(dly) drp_read_state;
       else
         state_wr <= #(dly) idle_state;
     end
     drp_write_state : begin //writing to DRP   
         state_wr <= #(dly) drp_write_data_latch;
     end  
     drp_write_data_latch : begin //writing to DRP
       if (S_AXI_wvalid)
         state_wr <= #(dly) drp_write_wait;
       else 
         state_wr <= #(dly) drp_write_data_latch;
     end         
     drp_write_wait : begin //writing to DRP
       if (drp_write_done  || S_AXI_bresp[0] )
         state_wr <= #(dly) write_response;
       else 
         state_wr <= #(dly) drp_write_wait;
     end 
     write_response : begin //writing to DRP
       if (S_AXI_bvalid && S_AXI_bready && S_AXI_arvalid )
         state_wr <= #(dly) drp_read_state;
       else if (S_AXI_bvalid && S_AXI_bready && !S_AXI_arvalid)
         state_wr <= #(dly) idle_state;         
       else 
         state_wr <= #(dly) write_response;
     end           
     drp_read_state : begin //reading from DRP
       state_wr <= #(dly) drp_read_wait;
     end
     drp_read_wait : begin //reading from DRP
       if (drp_read_done || S_AXI_rresp[0])
         state_wr <= #(dly) idle_state;
       else 
         state_wr <= #(dly) drp_read_wait;
     end

   endcase
 end // not AXI_sreset
end // AXI_aclk domain

//+----------------------------------------------------------------------------+
//|                 AXI Outputs                                                |
//+----------------------------------------------------------------------------+

always @ (posedge AXI_aclk or posedge AXI_sreset)
begin
  if (AXI_sreset == 1'b1)
    S_AXI_arready                  <= #(dly) 1'b0;
  else if (state_wr == drp_read_state) 
    S_AXI_arready                  <= #(dly) 1'b1;
  else
    S_AXI_arready                  <= #(dly) 1'b0;
end

always @ (posedge AXI_aclk or posedge AXI_sreset)
begin
  if (AXI_sreset == 1'b1)
    S_AXI_awready                  <= #(dly) 1'b0;
  else if (state_wr == drp_write_state) 
    S_AXI_awready                  <= #(dly) 1'b1;
  else
    S_AXI_awready                  <= #(dly) 1'b0;
end

always @ (posedge AXI_aclk or posedge AXI_sreset)
begin
  if (AXI_sreset == 1'b1)
    S_AXI_bvalid                  <= #(dly) 1'b0;
  else if ( state_wr == write_response && !S_AXI_bready)  
    S_AXI_bvalid                  <= #(dly) 1'b1;
  else if ((state_wr_r != write_response) && (state_wr == write_response))
    S_AXI_bvalid                  <= #(dly) 1'b1;
  else
    S_AXI_bvalid                  <= #(dly) 1'b0;  
end

always @ (posedge AXI_aclk or posedge AXI_sreset) 
begin
  if (AXI_sreset == 1'b1)
    S_AXI_rvalid                  <= #(dly) 1'b0;
  else if (((state_wr_r == drp_read_wait) && (|drp_rdy || S_AXI_rresp[0]) && read_pending )) begin    
    S_AXI_rvalid                  <= #(dly) 1'b1;
  end else if ((state_wr == drp_read_wait) && !S_AXI_rready && !read_pending)
    S_AXI_rvalid                  <= #(dly) 1'b1;
  else
    S_AXI_rvalid                  <= #(dly) 1'b0;
end

always @ (posedge AXI_aclk) 
begin
  if (|drp_rdy)
   // S_AXI_rdata                   <= #(dly) {drp_do[7:0], drp_do[15:8], 16'd0};
    S_AXI_rdata                   <= #(dly) drp_do_array[drp_select];
  
end

        
always @ (posedge AXI_aclk )
begin
  if (state_wr == idle_state)
    read_pending                  <= #(dly) 1'b0;
  else if (state_wr == drp_read_state)  
    read_pending                  <= #(dly) 1'b1;
  else if (|drp_rdy)
    read_pending                  <= #(dly) 1'b0; 
     
end

always @ (posedge AXI_aclk )
begin
  if (AXI_sreset == 1'b1)
    S_AXI_wready                  <= #(dly) 1'b0;
  else if ((state_wr == drp_write_data_latch) && S_AXI_wvalid && !S_AXI_wready) 
    S_AXI_wready                  <= #(dly) 1'b1;
  else begin
    S_AXI_wready                  <= #(dly) 1'b0;
 end
end

always @ (posedge AXI_aclk or posedge AXI_sreset)
begin
  if (AXI_sreset == 1'b1) begin
    drp_we                     <= #(dly) {DRP_COUNT{1'b0}};
    drp_en                     <= #(dly) {DRP_COUNT{1'b0}}; 
  end else if (S_AXI_wready && S_AXI_wvalid && (state_wr == drp_write_wait)) begin  
    drp_di                     <= #(dly) S_AXI_wdata[DRP_DATA_WIDTH-1:0];
    drp_we                     <= #(dly) 1'b1 << drp_select;
    drp_en                     <= #(dly) 1'b1 << drp_select;
  end else if ( S_AXI_arready && S_AXI_arvalid && (state_wr == drp_read_wait) ) begin
    drp_en                     <= #(dly) 1'b1 << drp_select;
  end else begin
    drp_we                     <= #(dly) {DRP_COUNT{1'b0}};
    drp_en                     <= #(dly) {DRP_COUNT{1'b0}};
  end  
end

always @ (posedge AXI_aclk )
begin
  if (S_AXI_awready && S_AXI_awvalid) begin  
    drp_addr                  <= #(dly) S_AXI_awaddr[DRP_ADDR_WIDTH+1:2];
    drp_select                <= #(dly) S_AXI_awaddr[DRP_ADDR_WIDTH+2+extra_addr_bits:DRP_ADDR_WIDTH+2];
  end else if ( S_AXI_arvalid) begin 
    drp_addr                  <= #(dly) S_AXI_araddr[DRP_ADDR_WIDTH+1:2]; 
    drp_select                <= #(dly) S_AXI_araddr[DRP_ADDR_WIDTH+2+extra_addr_bits:DRP_ADDR_WIDTH+2];
  end 
end

assign drp_write_done = |drp_rdy && (state_wr == drp_write_wait);
assign drp_read_done  = S_AXI_rready && S_AXI_rvalid && (state_wr == drp_read_wait);


//+---------------------------------------------------------------------------+
//|                Bad  addr detector                                         |
//+---------------------------------------------------------------------------+


if (DRP_COUNT > 1 && (extra_addr_bits_plusone == extra_addr_bits)) begin: find_bad_addr
   assign bad_wr_addr = (S_AXI_awvalid & (S_AXI_awaddr[DRP_ADDR_WIDTH+2+extra_addr_bits-1:DRP_ADDR_WIDTH+2] >= DRP_COUNT));
   assign bad_rd_addr = (S_AXI_arvalid & (S_AXI_araddr[DRP_ADDR_WIDTH+2+extra_addr_bits-1:DRP_ADDR_WIDTH+2] >= DRP_COUNT));
end else begin: dont_find_bad_addr
   assign bad_wr_addr = 0;
   assign bad_rd_addr = 0;
end

always @ (posedge AXI_aclk or posedge AXI_sreset)
begin
  if (AXI_sreset == 1'b1)
    S_AXI_arready                  <= #(dly) 1'b0;
  else if (state_wr == drp_read_state) 
    S_AXI_arready                  <= #(dly) 1'b1;
  else
    S_AXI_arready                  <= #(dly) 1'b0;
end

always @ (posedge AXI_aclk or posedge AXI_sreset)
begin
  if (AXI_sreset == 1'b1) 
    S_AXI_bresp                  <= #(dly) 2'b00;
  else if (state_wr == drp_write_state  && bad_wr_addr) 
    S_AXI_bresp                  <= #(dly) 2'b11;
  else if (state_wr == idle_state)
    S_AXI_bresp                  <= #(dly) 2'b00;
  else 
    S_AXI_bresp                  <= #(dly) S_AXI_bresp;
end

always @ (posedge AXI_aclk or posedge AXI_sreset)
begin
  if (AXI_sreset == 1'b1) 
    S_AXI_rresp                  <= #(dly) 2'b00;
  else if (state_wr == drp_read_state  && bad_rd_addr) 
    S_AXI_rresp                  <= #(dly) 2'b11;
  else if (state_wr == drp_read_state  && !bad_rd_addr)
    S_AXI_rresp                  <= #(dly) 2'b00;
  else 
    S_AXI_rresp                  <= #(dly) S_AXI_rresp;
end

endmodule
