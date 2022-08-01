/*
 * File:        rtp.h
 *
 * Notes:  This file includes a number of defines and structures for generating
 * rtp messages.  Note that the structures are packed.
 *
 * Author: silee
 */

#ifndef RTP_PROTOCOL_H
#define RTP_PROTOCOL_H

/*********************************************
// Format of RTP communications message frame.
// Total Frame Size: 1455 Byte
==============================================
		SYNC	= 0xC3
----------------------------------------------
		LENGTH	= N+1		(L)
							(H)
----------------------------------------------
		NODE/PHASE
----------------------------------------------
		COMMAND/RESPONSE
----------------------------------------------
		DATA[1]
----------------------------------------------
		... 
		...
----------------------------------------------
		DATA[1392]
----------------------------------------------
		CRC[1]				(L)
							(H)
----------------------------------------------
		...
		...
----------------------------------------------
		CRC[58]				(L)
						    (H)
===============================================	
**********************************************/

// RTP Protocol
#define SYNC_BYTE					0xC3         // sync byte                            
#define FLOAT_READ					0x9D         // float 읽기 명령                      
#define INT_READ					0x8D         // int 읽기 명령                        
#define BOOL_READ					0x82         // bool 읽기 명령                       
#define FLOAT_WRITE					0x9E         // float 쓰기 명령                      
#define INT_WRITE					0x8E         // int 쓰기 명령                        
#define BOOL_WRITE					0x83         // bool 쓰기 명령                       
#define NODE						0            // NODE / PHASE                         
#define MAX_RTP_FRAME_SIZE			1455 

#define BOOL_START_INDEX			485       // 연속된 Bool 변수들의 시작 Index      
#define FLOAT_START_INDEX			9         // 연속된 Float 변수들의 시작 Index      
#define INT_START_INDEX				23        // 연속된 Int 변수들의 시작 Index       
#define NUMBER_TO_READ				5         // 읽어올 변수의 개수                   

#define SINGLE_READ_COMMANDMSG_SIZE		9
#define SINGLE_WRITE_COMMANDMSG_SIZE	13

#define FCSINIT						0xFFFF
#define DEFAULT_RING_BUFFER_SIZE	10
#define CRCSIZE						256

#define SINGLE_BOOL_VALUE_SIZE		9
#define SINGLE_INT_VALUE_SIZE		10
#define SINGLE_FLOAT_VALUE_SIZE		12

#define SINGLE_CRC_RINDEX			7

#define SINGLE_CRC_WBOOL_INDEX		8
#define SINGLE_CRC_WINT_INDEX		9
#define SINGLE_CRC_WFLOAT_INDEX		11

#define MULTIPLE					5
#define SINGLE						3


/* Pack all structures defined here on 1-byte boundaries */
#pragma pack(1)

/* Note: GCC X.XX on the ARM has a bug an does not correctly process #pragma pack(1),
 * so we use the following macro */
#ifdef ARM
#define PACKED_STRUCTURE __attribute__((__packed__))
#else
#define PACKED_STRUCTURE
#endif

typedef struct rtpMHeader_str
{
    unsigned char syncByte;
    unsigned char lengthL;
    unsigned char lengthH;
    unsigned char cpuNode;
    unsigned char command;
} PACKED_STRUCTURE rtpMHeader;

/*---------------------------------------------*/
/* structure definitions for Modbus requests */
/*---------------------------------------------*/

typedef struct modbusReadRequest_str
{
    unsigned char    slave;
    unsigned char    fcode;
    unsigned short   startReg;
    unsigned short   numRead;
} PACKED_STRUCTURE modbusReadRequest;

typedef struct modbusReadResponse_str
{
    unsigned char  fcode;
    unsigned char  byteCount;
    unsigned char  data[1];
} PACKED_STRUCTURE modbusReadResponse;

typedef struct modbusWriteSingleRequest_str
{
    unsigned char  slave;
    unsigned char  fcode;
    unsigned short startReg;
    unsigned short data;
} PACKED_STRUCTURE modbusWriteSingleRequest;


typedef struct modbusWriteSingleResponse_str
{
    unsigned char  fcode;
    unsigned short startReg;
    unsigned short data;
} PACKED_STRUCTURE modbusWriteSingleResponse;

typedef struct modbusWriteMultipleRequest_str
{
    unsigned char  slave;
    unsigned char  fcode;
    unsigned short startReg;
    unsigned short numOutput;
    unsigned char  byteCount;
    unsigned char  data[1];
} PACKED_STRUCTURE modbusWriteMultipleRequest;


typedef struct modbusWriteMultipleResponse_str
{
    unsigned char  fcode;
    unsigned short startReg;
    unsigned short numOutput;
} PACKED_STRUCTURE modbusWriteMultipleResponse;

typedef struct modbusReadWriteMultipleRequest_str
{
    unsigned char  slave;
    unsigned char  fcode;
    unsigned short startReadReg;
    unsigned short numRead;
    unsigned short startWriteReg;
    unsigned short numOutput;
    unsigned char  byteCount;
    unsigned char  data[1];
} PACKED_STRUCTURE modbusReadWriteMultipleRequest;

typedef struct modbusExceptionResponse_str
{
    unsigned char  fcode;
    unsigned char  exception;
} PACKED_STRUCTURE modbusExceptionResponse;

/* Revert to packing that was in effect when compilation started */
#pragma pack()

#endif 
