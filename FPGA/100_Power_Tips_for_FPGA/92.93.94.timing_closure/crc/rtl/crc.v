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
module crc_floorplan(input clk1,clk2,clk3,clk4,
                     input crc_en1,crc_en2,crc_en3,crc_en4,
                     input rst, 
                     input [15:0] data_in,
                     output reg [31:0] crc_out1, crc_out2, crc_out3, crc_out4);  

  wire [31:0] crc_out1_i, crc_out2_i, crc_out3_i, crc_out4_i;
  reg crc_en1_q, crc_en2_q, crc_en3_q, crc_en4_q;
  reg [15:0] data_in1_q, data_in2_q, data_in3_q, data_in4_q;
  
  // add register stage to inputs and outputs
  always @(posedge clk1, posedge rst) begin
    if(rst) begin
      crc_out1   <= 32'h0;
      data_in1_q <= 16'h0;
      crc_en1_q  <= 1'b0;
    end
    else begin
      crc_out1    <= crc_out1_i;
      data_in1_q  <= data_in;
      crc_en1_q   <= crc_en1;
    end
  end // always

  always @(posedge clk2, posedge rst) begin
    if(rst) begin
      crc_out2   <= 32'h0;
      data_in2_q <= 16'h0;
      crc_en2_q  <= 1'b0;
    end
    else begin
      crc_out2    <= crc_out2_i;
      data_in2_q  <= data_in;
      crc_en2_q   <= crc_en2;
    end
  end // always

  always @(posedge clk3, posedge rst) begin
    if(rst) begin
      crc_out3   <= 32'h0;
      data_in3_q <= 16'h0;
      crc_en3_q  <= 1'b0;
    end
    else begin
      crc_out3    <= crc_out3_i;
      data_in3_q  <= data_in;
      crc_en3_q   <= crc_en3;
    end
  end // always

  always @(posedge clk4, posedge rst) begin
    if(rst) begin
      crc_out4   <= 32'h0;
      data_in4_q <= 16'h0;
      crc_en4_q  <= 1'b0;
    end
    else begin
      crc_out4    <= crc_out4_i;
      data_in4_q  <= data_in;
      crc_en4_q   <= crc_en4;
    end
  end // always

    crc crc1 (  .data_in_i(data_in1_q), 
                .crc_en_i(crc_en1_q), 
                .crc_out(crc_out1_i), 
                .rst(rst), .clk(clk1));
    
    crc crc2 (  .data_in_i(data_in2_q), 
                .crc_en_i(crc_en2_q), 
                .crc_out(crc_out2_i), 
                .rst(rst), .clk(clk2));

    crc crc3 (  .data_in_i(data_in3_q), 
                .crc_en_i(crc_en3_q), 
                .crc_out(crc_out3_i), 
                .rst(rst), .clk(clk3));

    crc crc4 (  .data_in_i(data_in4_q), 
                .crc_en_i(crc_en4_q), 
                .crc_out(crc_out4_i), 
                .rst(rst), .clk(clk4));
    
endmodule // crc_floorplan



// crc32 for Ethernet 
module crc(
  input [15:0] data_in_i,
  input crc_en_i,
  output reg [31:0] crc_out,
  input rst,
  input clk);

  reg [31:0] lfsr_q,lfsr_c;

  reg [15:0] data_in;
  reg crc_en;


  always @(*) begin
    lfsr_c[0] = lfsr_q[16] ^ lfsr_q[22] ^ lfsr_q[25] ^ lfsr_q[26] ^ lfsr_q[28] ^ data_in[0] ^ data_in[6] ^ data_in[9] ^ data_in[10] ^ data_in[12];
    lfsr_c[1] = lfsr_q[16] ^ lfsr_q[17] ^ lfsr_q[22] ^ lfsr_q[23] ^ lfsr_q[25] ^ lfsr_q[27] ^ lfsr_q[28] ^ lfsr_q[29] ^ data_in[0] ^ data_in[1] ^ data_in[6] ^ data_in[7] ^ data_in[9] ^ data_in[11] ^ data_in[12] ^ data_in[13];
    lfsr_c[2] = lfsr_q[16] ^ lfsr_q[17] ^ lfsr_q[18] ^ lfsr_q[22] ^ lfsr_q[23] ^ lfsr_q[24] ^ lfsr_q[25] ^ lfsr_q[29] ^ lfsr_q[30] ^ data_in[0] ^ data_in[1] ^ data_in[2] ^ data_in[6] ^ data_in[7] ^ data_in[8] ^ data_in[9] ^ data_in[13] ^ data_in[14];
    lfsr_c[3] = lfsr_q[17] ^ lfsr_q[18] ^ lfsr_q[19] ^ lfsr_q[23] ^ lfsr_q[24] ^ lfsr_q[25] ^ lfsr_q[26] ^ lfsr_q[30] ^ lfsr_q[31] ^ data_in[1] ^ data_in[2] ^ data_in[3] ^ data_in[7] ^ data_in[8] ^ data_in[9] ^ data_in[10] ^ data_in[14] ^ data_in[15];
    lfsr_c[4] = lfsr_q[16] ^ lfsr_q[18] ^ lfsr_q[19] ^ lfsr_q[20] ^ lfsr_q[22] ^ lfsr_q[24] ^ lfsr_q[27] ^ lfsr_q[28] ^ lfsr_q[31] ^ data_in[0] ^ data_in[2] ^ data_in[3] ^ data_in[4] ^ data_in[6] ^ data_in[8] ^ data_in[11] ^ data_in[12] ^ data_in[15];
    lfsr_c[5] = lfsr_q[16] ^ lfsr_q[17] ^ lfsr_q[19] ^ lfsr_q[20] ^ lfsr_q[21] ^ lfsr_q[22] ^ lfsr_q[23] ^ lfsr_q[26] ^ lfsr_q[29] ^ data_in[0] ^ data_in[1] ^ data_in[3] ^ data_in[4] ^ data_in[5] ^ data_in[6] ^ data_in[7] ^ data_in[10] ^ data_in[13];
    lfsr_c[6] = lfsr_q[17] ^ lfsr_q[18] ^ lfsr_q[20] ^ lfsr_q[21] ^ lfsr_q[22] ^ lfsr_q[23] ^ lfsr_q[24] ^ lfsr_q[27] ^ lfsr_q[30] ^ data_in[1] ^ data_in[2] ^ data_in[4] ^ data_in[5] ^ data_in[6] ^ data_in[7] ^ data_in[8] ^ data_in[11] ^ data_in[14];
    lfsr_c[7] = lfsr_q[16] ^ lfsr_q[18] ^ lfsr_q[19] ^ lfsr_q[21] ^ lfsr_q[23] ^ lfsr_q[24] ^ lfsr_q[26] ^ lfsr_q[31] ^ data_in[0] ^ data_in[2] ^ data_in[3] ^ data_in[5] ^ data_in[7] ^ data_in[8] ^ data_in[10] ^ data_in[15];
    lfsr_c[8] = lfsr_q[16] ^ lfsr_q[17] ^ lfsr_q[19] ^ lfsr_q[20] ^ lfsr_q[24] ^ lfsr_q[26] ^ lfsr_q[27] ^ lfsr_q[28] ^ data_in[0] ^ data_in[1] ^ data_in[3] ^ data_in[4] ^ data_in[8] ^ data_in[10] ^ data_in[11] ^ data_in[12];
    lfsr_c[9] = lfsr_q[17] ^ lfsr_q[18] ^ lfsr_q[20] ^ lfsr_q[21] ^ lfsr_q[25] ^ lfsr_q[27] ^ lfsr_q[28] ^ lfsr_q[29] ^ data_in[1] ^ data_in[2] ^ data_in[4] ^ data_in[5] ^ data_in[9] ^ data_in[11] ^ data_in[12] ^ data_in[13];
    lfsr_c[10] = lfsr_q[16] ^ lfsr_q[18] ^ lfsr_q[19] ^ lfsr_q[21] ^ lfsr_q[25] ^ lfsr_q[29] ^ lfsr_q[30] ^ data_in[0] ^ data_in[2] ^ data_in[3] ^ data_in[5] ^ data_in[9] ^ data_in[13] ^ data_in[14];
    lfsr_c[11] = lfsr_q[16] ^ lfsr_q[17] ^ lfsr_q[19] ^ lfsr_q[20] ^ lfsr_q[25] ^ lfsr_q[28] ^ lfsr_q[30] ^ lfsr_q[31] ^ data_in[0] ^ data_in[1] ^ data_in[3] ^ data_in[4] ^ data_in[9] ^ data_in[12] ^ data_in[14] ^ data_in[15];
    lfsr_c[12] = lfsr_q[16] ^ lfsr_q[17] ^ lfsr_q[18] ^ lfsr_q[20] ^ lfsr_q[21] ^ lfsr_q[22] ^ lfsr_q[25] ^ lfsr_q[28] ^ lfsr_q[29] ^ lfsr_q[31] ^ data_in[0] ^ data_in[1] ^ data_in[2] ^ data_in[4] ^ data_in[5] ^ data_in[6] ^ data_in[9] ^ data_in[12] ^ data_in[13] ^ data_in[15];
    lfsr_c[13] = lfsr_q[17] ^ lfsr_q[18] ^ lfsr_q[19] ^ lfsr_q[21] ^ lfsr_q[22] ^ lfsr_q[23] ^ lfsr_q[26] ^ lfsr_q[29] ^ lfsr_q[30] ^ data_in[1] ^ data_in[2] ^ data_in[3] ^ data_in[5] ^ data_in[6] ^ data_in[7] ^ data_in[10] ^ data_in[13] ^ data_in[14];
    lfsr_c[14] = lfsr_q[18] ^ lfsr_q[19] ^ lfsr_q[20] ^ lfsr_q[22] ^ lfsr_q[23] ^ lfsr_q[24] ^ lfsr_q[27] ^ lfsr_q[30] ^ lfsr_q[31] ^ data_in[2] ^ data_in[3] ^ data_in[4] ^ data_in[6] ^ data_in[7] ^ data_in[8] ^ data_in[11] ^ data_in[14] ^ data_in[15];
    lfsr_c[15] = lfsr_q[19] ^ lfsr_q[20] ^ lfsr_q[21] ^ lfsr_q[23] ^ lfsr_q[24] ^ lfsr_q[25] ^ lfsr_q[28] ^ lfsr_q[31] ^ data_in[3] ^ data_in[4] ^ data_in[5] ^ data_in[7] ^ data_in[8] ^ data_in[9] ^ data_in[12] ^ data_in[15];
    lfsr_c[16] = lfsr_q[0] ^ lfsr_q[16] ^ lfsr_q[20] ^ lfsr_q[21] ^ lfsr_q[24] ^ lfsr_q[28] ^ lfsr_q[29] ^ data_in[0] ^ data_in[4] ^ data_in[5] ^ data_in[8] ^ data_in[12] ^ data_in[13];
    lfsr_c[17] = lfsr_q[1] ^ lfsr_q[17] ^ lfsr_q[21] ^ lfsr_q[22] ^ lfsr_q[25] ^ lfsr_q[29] ^ lfsr_q[30] ^ data_in[1] ^ data_in[5] ^ data_in[6] ^ data_in[9] ^ data_in[13] ^ data_in[14];
    lfsr_c[18] = lfsr_q[2] ^ lfsr_q[18] ^ lfsr_q[22] ^ lfsr_q[23] ^ lfsr_q[26] ^ lfsr_q[30] ^ lfsr_q[31] ^ data_in[2] ^ data_in[6] ^ data_in[7] ^ data_in[10] ^ data_in[14] ^ data_in[15];
    lfsr_c[19] = lfsr_q[3] ^ lfsr_q[19] ^ lfsr_q[23] ^ lfsr_q[24] ^ lfsr_q[27] ^ lfsr_q[31] ^ data_in[3] ^ data_in[7] ^ data_in[8] ^ data_in[11] ^ data_in[15];
    lfsr_c[20] = lfsr_q[4] ^ lfsr_q[20] ^ lfsr_q[24] ^ lfsr_q[25] ^ lfsr_q[28] ^ data_in[4] ^ data_in[8] ^ data_in[9] ^ data_in[12];
    lfsr_c[21] = lfsr_q[5] ^ lfsr_q[21] ^ lfsr_q[25] ^ lfsr_q[26] ^ lfsr_q[29] ^ data_in[5] ^ data_in[9] ^ data_in[10] ^ data_in[13];
    lfsr_c[22] = lfsr_q[6] ^ lfsr_q[16] ^ lfsr_q[25] ^ lfsr_q[27] ^ lfsr_q[28] ^ lfsr_q[30] ^ data_in[0] ^ data_in[9] ^ data_in[11] ^ data_in[12] ^ data_in[14];
    lfsr_c[23] = lfsr_q[7] ^ lfsr_q[16] ^ lfsr_q[17] ^ lfsr_q[22] ^ lfsr_q[25] ^ lfsr_q[29] ^ lfsr_q[31] ^ data_in[0] ^ data_in[1] ^ data_in[6] ^ data_in[9] ^ data_in[13] ^ data_in[15];
    lfsr_c[24] = lfsr_q[8] ^ lfsr_q[17] ^ lfsr_q[18] ^ lfsr_q[23] ^ lfsr_q[26] ^ lfsr_q[30] ^ data_in[1] ^ data_in[2] ^ data_in[7] ^ data_in[10] ^ data_in[14];
    lfsr_c[25] = lfsr_q[9] ^ lfsr_q[18] ^ lfsr_q[19] ^ lfsr_q[24] ^ lfsr_q[27] ^ lfsr_q[31] ^ data_in[2] ^ data_in[3] ^ data_in[8] ^ data_in[11] ^ data_in[15];
    lfsr_c[26] = lfsr_q[10] ^ lfsr_q[16] ^ lfsr_q[19] ^ lfsr_q[20] ^ lfsr_q[22] ^ lfsr_q[26] ^ data_in[0] ^ data_in[3] ^ data_in[4] ^ data_in[6] ^ data_in[10];
    lfsr_c[27] = lfsr_q[11] ^ lfsr_q[17] ^ lfsr_q[20] ^ lfsr_q[21] ^ lfsr_q[23] ^ lfsr_q[27] ^ data_in[1] ^ data_in[4] ^ data_in[5] ^ data_in[7] ^ data_in[11];
    lfsr_c[28] = lfsr_q[12] ^ lfsr_q[18] ^ lfsr_q[21] ^ lfsr_q[22] ^ lfsr_q[24] ^ lfsr_q[28] ^ data_in[2] ^ data_in[5] ^ data_in[6] ^ data_in[8] ^ data_in[12];
    lfsr_c[29] = lfsr_q[13] ^ lfsr_q[19] ^ lfsr_q[22] ^ lfsr_q[23] ^ lfsr_q[25] ^ lfsr_q[29] ^ data_in[3] ^ data_in[6] ^ data_in[7] ^ data_in[9] ^ data_in[13];
    lfsr_c[30] = lfsr_q[14] ^ lfsr_q[20] ^ lfsr_q[23] ^ lfsr_q[24] ^ lfsr_q[26] ^ lfsr_q[30] ^ data_in[4] ^ data_in[7] ^ data_in[8] ^ data_in[10] ^ data_in[14];
    lfsr_c[31] = lfsr_q[15] ^ lfsr_q[21] ^ lfsr_q[24] ^ lfsr_q[25] ^ lfsr_q[27] ^ lfsr_q[31] ^ data_in[5] ^ data_in[8] ^ data_in[9] ^ data_in[11] ^ data_in[15];
  end // always

  always @(posedge clk, posedge rst) begin
    if(rst) begin
      lfsr_q  <= 32'hffffffff;
      crc_out <= 32'h0;
      data_in <= 16'h0;
      crc_en  <= 1'b0;
    end
    else begin
      lfsr_q  <= crc_en ? lfsr_c : lfsr_q;
      crc_out <= lfsr_q;
      data_in <= data_in_i;
      crc_en  <= crc_en_i;
    end
  end // always
endmodule // crc