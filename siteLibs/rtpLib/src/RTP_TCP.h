#ifndef RTP_TCP_H
#define RTP_TCP_H

// WinSock Ver 2.0
#include <winsock2.h>

/* ��ȣ��� ���� */
// TCP/IP ��� ����
#define PORT_NUM	     50199

// RTP �������� ����
#define SYNC_BYTE         0xC3                // sync byte                            
#define FLOAT_READ        0x9D                // float �б� ���                      
#define INT_READ          0x8D                // int �б� ���                        
#define BOOL_READ         0x82                // bool �б� ���                       
#define FLOAT_WRITE       0x9E                // float ���� ���                      
#define INT_WRITE         0x8E                // int ���� ���                        
#define BOOL_WRITE        0x83                // bool ���� ���                       
#define NODE              0                   // NODE / PHASE                         
#define BOOL_START_INDEX  485                 // ���ӵ� Bool �������� ���� Index      
#define FLOAT_START_INDEX 9                   // ���ӵ� Float �������� ���� Index      
#define INT_START_INDEX   23                  // ���ӵ� Int �������� ���� Index       
#define NUMBER_TO_READ    5                   // �о�� ������ ����                   

// CRC
#define FCSINIT           0xFFFF

/* ���� ���� ���� */
// ����
WSADATA wsaData;                              // ���� �ʱ�ȭ
SOCKET rtpSocket;                             // ����


// ������ ������ ��� 
char float_Read_Command[11];                  // float �б� ���
char int_Read_Command[11];                    // int �б� ���
char bool_Read_Command[11];                   // bool �б� ���
char float_Write_Command[13];                 // float ���� ���
char int_Write_Command[11];                   // int ���� ���
char bool_Write_Command[10];                  // bool ���� ���

// �������� ���� ��ɿ� ���� ����
char float_Read_Response[35];                 // float �б� ���
char int_Read_Response[25];                   // int �б� ���
char bool_Read_Response[16];                  // bool �б� ���
char float_Write_Response[7];                 // float ���� ���
char int_Write_Response[7];                   // int ���� ���
char bool_Write_Response[7];                  // bool ���� ���

// ����� ���� �Լ� ����
void InitConnection(char *servIPAddress);                                     // �������� ���� �ʱ�ȭ
void ReadFloatData(void);                                                     // float �б�(multi) 
void ReadIntData(void);                                                       // int �б�(multi)
void ReadBoolData(void);                                                      // bool �б�(multi)
void WriteFloatData(unsigned short writeIndex, float *writeFloat);            // float ����(single)
void WriteIntData(unsigned short writeIndex, int *writeInt);                  // int ����(single) 
void WriteBoolData(unsigned short writeIndex, char writeBool);                // bool ����(single) 
unsigned short getCRC(unsigned char *VARWriteCMD, int LoopCount);             // CRC�˻� 


#endif // _RTP_TCP_H 


