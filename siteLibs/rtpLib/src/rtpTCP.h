#ifndef RTP_TCP_H
#define RTP_TCP_H

// WinSock Ver 2.0
#include <winsock2.h>

/* 기호상수 정의 */
// TCP/IP 통신 관련
#define PORT_NUM	     50199

// RTP 프로토콜 관련
#define SYNC_BYTE         0xC3                // sync byte                            
#define FLOAT_READ        0x9D                // float 읽기 명령                      
#define INT_READ          0x8D                // int 읽기 명령                        
#define BOOL_READ         0x82                // bool 읽기 명령                       
#define FLOAT_WRITE       0x9E                // float 쓰기 명령                      
#define INT_WRITE         0x8E                // int 쓰기 명령                        
#define BOOL_WRITE        0x83                // bool 쓰기 명령                       
#define NODE              0                   // NODE / PHASE                         
#define BOOL_START_INDEX  485                 // 연속된 Bool 변수들의 시작 Index      
#define FLOAT_START_INDEX 9                   // 연속된 Float 변수들의 시작 Index      
#define INT_START_INDEX   23                  // 연속된 Int 변수들의 시작 Index       
#define NUMBER_TO_READ    5                   // 읽어올 변수의 개수                   

// CRC
#define FCSINIT           0xFFFF

/* 전역 변수 선언 */
// 소켓
WSADATA wsaData;                              // 소켓 초기화
SOCKET rtpSocket;                             // 소켓


// 서버로 전송할 명령 
char float_Read_Command[11];                  // float 읽기 명령
char int_Read_Command[11];                    // int 읽기 명령
char bool_Read_Command[11];                   // bool 읽기 명령
char float_Write_Command[13];                 // float 쓰기 명령
char int_Write_Command[11];                   // int 쓰기 명령
char bool_Write_Command[10];                  // bool 쓰기 명령

// 서버에서 오는 명령에 대한 응답
char float_Read_Response[35];                 // float 읽기 명령
char int_Read_Response[25];                   // int 읽기 명령
char bool_Read_Response[16];                  // bool 읽기 명령
char float_Write_Response[7];                 // float 쓰기 명령
char int_Write_Response[7];                   // int 쓰기 명령
char bool_Write_Response[7];                  // bool 쓰기 명령

// 사용자 정의 함수 선언
void InitConnection(char *servIPAddress);                                     // 서버와의 연결 초기화
void ReadFloatData(void);                                                     // float 읽기(multi) 
void ReadIntData(void);                                                       // int 읽기(multi)
void ReadBoolData(void);                                                      // bool 읽기(multi)
void WriteFloatData(unsigned short writeIndex, float *writeFloat);            // float 쓰기(single)
void WriteIntData(unsigned short writeIndex, int *writeInt);                  // int 쓰기(single) 
void WriteBoolData(unsigned short writeIndex, char writeBool);                // bool 쓰기(single) 
unsigned short getCRC(unsigned char *VARWriteCMD, int LoopCount);             // CRC검사 


#endif // _RTP_TCP_H 


