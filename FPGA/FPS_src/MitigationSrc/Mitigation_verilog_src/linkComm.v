// MIT License
//
// Copyright (c) 2106 Osprey DCS
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// Handle communication with a single communication daisy chain
// Everything in Aurora clock domain

module linkComm #(
    parameter AXI_WIDTH           = 16,
    parameter TIMESTAMP_WIDTH     = 64,
    parameter LOGDATA_WIDTH       = 32,
    parameter MONDATA_WIDTH       = 32,
    parameter INTERLOCKS_PER_NODE = 64,
    parameter NODES_PER_LINK      = 5,
    parameter LINK_INDEX          = -1,
    parameter MAX_LINK_COUNT      = -1,
    parameter DEBUG               = "false",
    parameter TIME_DEBUG          = "false",
    parameter COS_DEBUG           = "false",
    parameter LOG_DEBUG           = "false",
    parameter MON_DEBUG           = "false"
    ) (
    input  wire clk, // Everything is in Aurora clock domain

    // Incoming packets
    (* mark_debug = DEBUG *) input wire [AXI_WIDTH-1:0] incomingTDATA,
    (* mark_debug = DEBUG *) input wire                 incomingTVALID,
    (* mark_debug = DEBUG *) input wire                 incomingTLAST,

    // Time stamp packet generation
    output wire      [AXI_WIDTH-1:0] outgoingTDATA,
    output wire                      outgoingTVALID,
    output wire                      outgoingTLAST,
    input  wire                      outgoingTREADY,
    input wire [TIMESTAMP_WIDTH-1:0] timestamp,
    input wire                       secondsMarkerToggle,
    
    // COS packet results
    output wire                       [NODES_PER_LINK-1:0] interlockStale,
    output wire [(INTERLOCKS_PER_NODE*NODES_PER_LINK)-1:0] interlockState,

    // LOG packet results
    output wire                     logDataTVALID,
    output wire                     logDataTLAST,
    output wire [LOGDATA_WIDTH-1:0] logDataTDATA,
    
    // MON packet results
    output wire                     monDataTVALID,
    output wire [MONDATA_WIDTH-1:0] monDataTDATA);

// Handle ping requests and generate time synchronization packets
timeSync #(.DEBUG(TIME_DEBUG))
  timeSync (
   .clk(clk),
   .incomingTDATA(incomingTDATA),
   .incomingTVALID(incomingTVALID),
   .incomingTLAST(incomingTLAST),
   .outgoingTDATA(outgoingTDATA),
   .outgoingTVALID(outgoingTVALID),
   .outgoingTLAST(outgoingTLAST),
   .outgoingTREADY(outgoingTREADY),
   .timestamp(timestamp),
   .secondsMarkerToggle(secondsMarkerToggle));

// Handle interlock state packets
receiveCOSpacket #(.DEBUG(COS_DEBUG))
  receiveCOSpacket (
    .clk(clk),
    .incomingTDATA(incomingTDATA),
    .incomingTVALID(incomingTVALID),
    .incomingTLAST(incomingTLAST),
    .interlockStale(interlockStale),
    .interlockState(interlockState));

// Handle transition logging packets
receiveLOGpacket #(.LINK_INDEX(LINK_INDEX),
                   .MAX_LINK_COUNT(MAX_LINK_COUNT),
                   .DEBUG(LOG_DEBUG))
  receiveLOGpacket (
    .clk(clk),
    .incomingTDATA(incomingTDATA),
    .incomingTVALID(incomingTVALID),
    .incomingTLAST(incomingTLAST),
    .logDataTVALID(logDataTVALID),
    .logDataTLAST(logDataTLAST),
    .logDataTDATA(logDataTDATA));

// Handle system monitoring packets
receiveMONpacket #(.LINK_INDEX(LINK_INDEX),
                   .MAX_LINK_COUNT(MAX_LINK_COUNT),
                   .DEBUG(MON_DEBUG))
  receiveMONpacket (
    .clk(clk),
    .incomingTDATA(incomingTDATA),
    .incomingTVALID(incomingTVALID),
    .incomingTLAST(incomingTLAST),
    .monDataTVALID(monDataTVALID),
    .monDataTDATA(monDataTDATA));

endmodule
