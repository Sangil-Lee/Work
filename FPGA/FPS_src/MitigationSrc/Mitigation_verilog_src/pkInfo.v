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

// Packet header values
parameter PKH_MAGIC_WIDTH  = 8;
parameter PKH_SEQNO_WIDTH  = 2;
parameter PKH_TYPE_WIDTH   = 3;
parameter PKH_TTL_WIDTH    = 3;

parameter PKH_MAGIC = 8'hD7;

parameter PKTYPE_INP  = 3'h0;

parameter PKH_INITIAL_TTL = 3'd4; // Up to (INITIAL_TTL+1) nodes in daisy chain
parameter PKH_NO_TTL      = 3'd0;

parameter PKTYPE_INP_WORDCOUNT = 12;

parameter PK_SYSMON_ADDR_WIDTH = 7;
