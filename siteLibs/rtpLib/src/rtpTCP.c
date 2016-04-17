#include <userint.h>
#include <ansi_c.h>

#include "RTP_TCP.h"

// CRC 
unsigned char maskBit[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80}; // 비트별 마스킹 
unsigned short fcstab[256] = {		                                         // 16 bit FCS lookup table per RFC1331 : For HDLC CRC calculation
0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf, 
0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7, 
0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e, 
0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876, 
0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd, 
0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5, 
0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c, 
0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974, 
0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb, 
0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3, 
0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a, 
0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72, 
0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9, 
0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1, 
0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738, 
0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70, 
0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7, 
0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff, 
0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036, 
0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e, 
0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5, 
0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd, 
0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134, 
0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c, 
0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3, 
0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb, 
0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232, 
0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a, 
0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1, 
0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9, 
0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330, 
0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
};

// 서버와의 연결 초기화 
void InitConnection(char *servIPAddress) 
{
	WSADATA ws;
	SOCKET sock;
	SOCKADDR_IN server;             // 서버 정보 
	
	int returnVal;
	
	// 소켓 초기화
	if(WSAStartup(MAKEWORD(2, 2), &ws) != 0)
	{
		printf("WSAStartup() error!\n");
		WSACleanup();
	}
	wsaData = ws;
	
	// 소켓 생성
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock == INVALID_SOCKET)
	{
		printf("socket() error\n");
		WSACleanup();
	}
	rtpSocket = sock;
	
	// 서버 정보 설정
	memset(&server, 0, sizeof(server));                            // server초기화
	server.sin_family = AF_INET;						 
	server.sin_addr.S_un.S_addr = inet_addr(servIPAddress);        // 서버 IP주소 설정
	server.sin_port = htons(PORT_NUM);                             // 서버 port 설정
	
	// 서버에 접속
	returnVal = connect(rtpSocket, (struct sockaddr*)&server, sizeof(server));
	
	if(returnVal == SOCKET_ERROR)
	{
		MessagePopup("오류", "RTP3000에 연결할 수 없습니다");
		closesocket(rtpSocket);
		WSACleanup();
	}					 
	else
	{
		MessagePopup("연결", "RTP3000에 연결 되었습니다"); 
	}
}

// float 읽기(multi) 
void ReadFloatData(void)                                         
{
	int sendByte;
	int recvByte;
	
	float temp;
	
	// 메시지 초기화
	memset(float_Read_Command, 0, sizeof(float_Read_Command));
	memset(float_Read_Response, 0, sizeof(float_Read_Response));
	
	// 메시지 작성
	float_Read_Command[0] = SYNC_BYTE;                                  						// SYNC BYTE
	float_Read_Command[1] = 5;						                    						// LENGTH LOW
	float_Read_Command[2] = 0x00;					                    						// LENGTH HIGH
	float_Read_Command[3] = NODE;					                    						// NODE / PHASE
	float_Read_Command[4] = FLOAT_READ;				                    						// float읽기
	float_Read_Command[5] = (unsigned char)(FLOAT_START_INDEX % 256);         					// REF LOW
	float_Read_Command[6] = (unsigned char)(FLOAT_START_INDEX / 256);         					// REF HIGH
	float_Read_Command[7] = (unsigned char)(NUMBER_TO_READ % 256);	    						// NUMBER TO READ LOW 
	float_Read_Command[8] = (unsigned char)(NUMBER_TO_READ / 256);	    						// NUMBER TO READ HIGH
	
	getCRC(float_Read_Command, 9);                                      						// CRC 검사
	
	sendByte = send(rtpSocket, (const char*)float_Read_Command, sizeof(float_Read_Command), 0); // 메시지 전송
	
	if(sendByte != sizeof(float_Read_Command))
	{
		MessagePopup("오류", "명령 메시지를 보낼 수 없습니다!");
		printf("sendByte is %d\n", sendByte);
	}
	else
	{
		recvByte = recv(rtpSocket, (char*)float_Read_Response, 35, 0);
		if(recvByte == SOCKET_ERROR)
		{
			MessagePopup("오류", "RTP3000이 응답하지 않습니다!");       
			printf("에러코드 : %d\n", WSAGetLastError());
		}
	}
}

// int 읽기(multi)   
void ReadIntData(void)                          
{
	int sendByte;
	int recvByte;
	
	// 메시지 초기화
	memset(int_Read_Command, 0, sizeof(int_Read_Command));
	memset(int_Read_Response, 0, sizeof(int_Read_Response));
	
	// 메시지 작성
	int_Read_Command[0] = SYNC_BYTE;                                 						// SYNC BYTE
	int_Read_Command[1] = 5;						                 						// LENGTH LOW
	int_Read_Command[2] = 0x00;					                     						// LENGTH HIGH
	int_Read_Command[3] = NODE;					                     						// NODE / PHASE
	int_Read_Command[4] = INT_READ;				                     						// int읽기
	int_Read_Command[5] = (unsigned char)(INT_START_INDEX % 256);        					// REF LOW
	int_Read_Command[6] = (unsigned char)(INT_START_INDEX / 256);        					// REF HIGH
	int_Read_Command[7] = (unsigned char)(NUMBER_TO_READ % 256);	 						// NUMBER TO READ LOW 
	int_Read_Command[8] = (unsigned char)(NUMBER_TO_READ / 256);	 						// NUMBER TO READ HIGH
	
	getCRC(int_Read_Command, 9);                                     						// CRC 검사
	
	sendByte = send(rtpSocket, (const char*)int_Read_Command, sizeof(int_Read_Command), 0); // 메시지 전송
	
	if(sendByte != sizeof(int_Read_Command))
	{
		MessagePopup("오류", "명령 메시지를 보낼 수 없습니다!");     
		printf("sendByte is %d\n", sendByte);
	}
	else
	{
		recvByte = recv(rtpSocket, (char*)int_Read_Response, 25, 0);
		if(recvByte == SOCKET_ERROR)
		{
			MessagePopup("오류", "RTP3000이 응답하지 않습니다!");      
			printf("에러코드 : %d\n", WSAGetLastError());
		}
	}
}

// bool 읽기(multi)
void ReadBoolData(void)                          
{
	int sendByte;
	int recvByte;
	
	// 메시지 초기화
	memset(bool_Read_Command, 0, sizeof(bool_Read_Command));
	memset(bool_Read_Response, 0, sizeof(bool_Read_Response));
	
	// 메시지 작성
	bool_Read_Command[0] = SYNC_BYTE;                                                         // SYNC BYTE
	bool_Read_Command[1] = 5;						                                          // LENGTH LOW
	bool_Read_Command[2] = 0x00;					                                          // LENGTH HIGH
	bool_Read_Command[3] = NODE;					                                          // NODE / PHASE
	bool_Read_Command[4] = BOOL_READ;				                                          // bool읽기
	bool_Read_Command[5] = (unsigned char)(BOOL_START_INDEX % 256);                           // REF LOW
	bool_Read_Command[6] = (unsigned char)(BOOL_START_INDEX / 256);                           // REF HIGH
	bool_Read_Command[7] = (unsigned char)(NUMBER_TO_READ % 256);                             // NUMBER TO READ LOW 
	bool_Read_Command[8] = (unsigned char)(NUMBER_TO_READ / 256);	                          // NUMBER TO READ HIGH
	
	getCRC(bool_Read_Command, 9);                                                             // CRC 검사
	
	sendByte = send(rtpSocket, (const char*)bool_Read_Command, sizeof(bool_Read_Command), 0); // 메시지 전송
	
	if(sendByte != sizeof(bool_Read_Command))
	{
		MessagePopup("오류", "명령 메시지를 보낼 수 없습니다!");   
		printf("sendByte is %d\n", sendByte);
	}
	else
	{
		recvByte = recv(rtpSocket, (char*)bool_Read_Response, 16, 0);
		if(recvByte == SOCKET_ERROR)
		{
			MessagePopup("오류", "RTP3000이 응답하지 않습니다!");
			printf("에러코드 : %d\n", WSAGetLastError());
		}
	}
}

// float 쓰기(single)
void WriteFloatData(unsigned short writeIndex, float *writeFloat)                        
{
	int sendByte;
	int recvByte;
	int i;
	int j = 0;
	
	float sentValue = 0;
	int sentIndex = 0;
	
	// 메시지 초기화
	memset(float_Write_Command, 0, sizeof(float_Write_Command));
	memset(float_Write_Response, 0, sizeof(float_Write_Response));
	
	// 메시지 작성
	float_Write_Command[0] = SYNC_BYTE;                                                            // SYNC BYTE
	float_Write_Command[1] = 7;						                                               // LENGTH LOW
	float_Write_Command[2] = 0x00;					                                               // LENGTH HIGH
	float_Write_Command[3] = NODE;					                                               // NODE / PHASE
	float_Write_Command[4] = FLOAT_WRITE;				                                           // float쓰기
	float_Write_Command[5] = (unsigned char)((FLOAT_START_INDEX + writeIndex - 1) % 256);          // REF LOW
	float_Write_Command[6] = (unsigned char)((FLOAT_START_INDEX + writeIndex - 1) / 256);          // REF HIGH
	
	for(i = 7; i <= 10; i++)														               // 사용자가 입력한 값을 명령 메시지에 넣음
	{
		memcpy(&float_Write_Command[i], ((unsigned char*)writeFloat) + j, sizeof(unsigned char));
		j++; 
	}
	
	getCRC(float_Write_Command, 11);                      							              // CRC 검사
	
	sendByte = send(rtpSocket, (const char*)float_Write_Command, sizeof(float_Write_Command), 0); // 메시지 전송
	
	if(sendByte != sizeof(float_Write_Command))
	{
		MessagePopup("오류", "명령 메시지를 보낼 수 없습니다!");   
		printf("sendByte is %d\n", sendByte);
	}
	else
	{
		recvByte = recv(rtpSocket, (char*)float_Write_Response, 7, 0);
		if(recvByte == SOCKET_ERROR)
		{
			MessagePopup("오류", "RTP3000이 응답하지 않습니다!"); 
			printf("에러코드 : %d\n", WSAGetLastError());
		}
	}
}


void WriteIntData(unsigned short writeIndex, int *writeInt)                                   // int 쓰기(single)
{
	int sendByte;
	int recvByte;
	int i;
	int j = 0;
	
	int sentIndex = 0;
	int sentValue = 0;
	
	// 메시지 초기화
	memset(int_Write_Command, 0, sizeof(int_Write_Command));
	memset(int_Write_Response, 0, sizeof(int_Write_Response));
	
	// 메시지 작성
	int_Write_Command[0] = SYNC_BYTE;                                                         // SYNC BYTE
	int_Write_Command[1] = 5;						                                          // LENGTH LOW
	int_Write_Command[2] = 0x00;					                                          // LENGTH HIGH
	int_Write_Command[3] = NODE;					                                          // NODE / PHASE
	int_Write_Command[4] = INT_WRITE;				                                          // int쓰기
	int_Write_Command[5] = (unsigned char)((INT_START_INDEX + writeIndex - 1) % 256);         // REF LOW
	int_Write_Command[6] = (unsigned char)((INT_START_INDEX + writeIndex - 1) / 256);         // REF HIGH
	
	for(i = 7; i <= 10; i++)
	{																				          // 사용자가 입력한 값을 명령 메시지에 넣음
		memcpy(&int_Write_Command[i], ((unsigned char*)writeInt) + j, sizeof(unsigned char));
		j++;
	}
	getCRC(int_Write_Command, 9);                                                             // CRC 검사
	
	sendByte = send(rtpSocket, (const char*)int_Write_Command, sizeof(int_Write_Command), 0); // 메시지 전송
	
	if(sendByte != 11)
	{
		MessagePopup("오류", "명령 메시지를 보낼 수 없습니다!");   
		printf("sendByte is %d\n", sendByte);
	}
	else
	{
		recvByte = recv(rtpSocket, (char*)int_Write_Response, 7, 0);
		if(recvByte == SOCKET_ERROR)
		{
			MessagePopup("오류", "RTP3000이 응답하지 않습니다!"); 
			printf("에러코드 : %d\n", WSAGetLastError());
		}
	}
}

// bool 쓰기(single)
void WriteBoolData(unsigned short writeIndex, char writeBool)                         
{
	int sendByte;
	int recvByte;
	
	// 메시지 초기화
	memset(bool_Write_Command, 0, sizeof(bool_Write_Command));
	memset(bool_Write_Response, 0, sizeof(bool_Write_Response));
	
	// 메시지 작성
	bool_Write_Command[0] = SYNC_BYTE;                                                      	// SYNC BYTE
	bool_Write_Command[1] = 4;						                                        	// LENGTH LOW
	bool_Write_Command[2] = 0x00;					                                        	// LENGTH HIGH
	bool_Write_Command[3] = NODE;					                                        	// NODE / PHASE
	bool_Write_Command[4] = BOOL_WRITE;				                                        	// bool쓰기
	bool_Write_Command[5] = (unsigned char)((BOOL_START_INDEX + writeIndex - 1) % 256);         // REF LOW
	bool_Write_Command[6] = (unsigned char)((BOOL_START_INDEX + writeIndex - 1) / 256);         // REF HIGH
	bool_Write_Command[7] = writeBool;                                                      	// 사용자가 입력한 값을 명령 메시지에 넣음
	
	getCRC(bool_Write_Command, 8);                                                          	// CRC 검사
																				  
	sendByte = send(rtpSocket, (const char*)bool_Write_Command, sizeof(bool_Write_Command), 0); // 메시지 전송
	
	if(sendByte != sizeof(bool_Write_Command))
	{
		MessagePopup("오류", "명령 메시지를 보낼 수 없습니다!");
		printf("sendByte is %d\n", sendByte);
	}
	else
	{
		recvByte = recv(rtpSocket, (char*)bool_Write_Response, 7, 0);
		if(recvByte == SOCKET_ERROR)
		{
			MessagePopup("오류", "RTP3000이 응답하지 않습니다!");
			printf("에러코드 : %d\n", WSAGetLastError());
		}
	}
}						

// CRC검사 
unsigned short getCRC(unsigned char *VARWriteCMD, int LoopCount) 
{
	int i;

	unsigned short fcsval = FCSINIT;
	
	for(i = 1; i < LoopCount; i++)
	{
		fcsval = ((fcsval >> 8) & 0xff) ^ fcstab[(fcsval ^ VARWriteCMD[i]) & 0xff]; 
	}
	
	VARWriteCMD[LoopCount] = fcsval & 0xff;
	VARWriteCMD[LoopCount + 1] = (fcsval >> 8) & 0xff;
	
	return fcsval;
}
