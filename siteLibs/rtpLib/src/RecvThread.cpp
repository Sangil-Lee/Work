#include "StdAfx.h"
#include "common.h"

#include <fstream>

using namespace std;

DATA_T t_data;

SOE arSOE_Data[112];   // SOE 데이터
FIFO arFIFO_Data[112]; // FIFO 데이터

float dd[10000];
int getValue[10000];
bool b_value[10000];

bool g_mutex = false;

UCHAR mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

DWORD WINAPI ReadMsg(LPVOID arg) {
	g_mutex = false;
	while (bRtpConnected) {
		while (g_mutex) Sleep(1);
		g_mutex = true;
		FMReadMsg();
		FMReadMsg2();
		IMReadMsg();
		BMReadMsg();
		g_mutex = false;
		Sleep(SCAN_CYCLE);
	}

	return 0;
}

// Float 읽기
DWORD FMReadMsg()
{
	int recvByte;
	int sendByte;
	int dataLength;
	int totalLen;
	int crcNum;

	float f_val;
	
	unsigned char MFReadCMD[11];
	unsigned char MFReadRSP[2048];

	memset(MFReadCMD, '\0', sizeof(MFReadCMD));
	memset(MFReadRSP, '\0', sizeof(MFReadRSP));

	MFReadCMD[0] = (unsigned char)0xC3;
	MFReadCMD[1] = (unsigned char)0x05;
	MFReadCMD[2] = (unsigned char)0x00;
	MFReadCMD[3] = (unsigned char)0x00;
	MFReadCMD[4] = (unsigned char)0x9D;
	MFReadCMD[5] = (unsigned char)(FLOAT_START_INDEX_1%256);
	MFReadCMD[6] = (unsigned char)(FLOAT_START_INDEX_1/256);
	MFReadCMD[7] = (unsigned char)(FLOAT_NUMBER_TO_READ_1%256);
	MFReadCMD[8] = (unsigned char)(FLOAT_NUMBER_TO_READ_1/256);
	
	getCrc(MFReadCMD, 9);
	
	sendByte = send(sock, (char *)MFReadCMD, 11, 0);

	if(sendByte == SOCKET_ERROR)
	{
		saveLog("RTP float read thread fail to send");
		closeComm();

		return -1;
	}
	else
	{
		recvByte = recv(sock, (char *)MFReadRSP, sizeof(MFReadRSP), 0);

		if(recvByte == SOCKET_ERROR)
		{
			saveLog("RTP float read thread fail to receive");
			closeComm();

			return -1;
		}
	
		// Packet Header check
		if (MFReadRSP[0] != 0xc3) return -1;

		// Data Length check
		dataLength = *(unsigned short *)&MFReadRSP[1];
		if (dataLength != (FLOAT_NUMBER_TO_READ_1*4+1)) return -1;

		// Packet Length Check
		totalLen = 4 + FLOAT_NUMBER_TO_READ_1 * 4 ; 
        crcNum = totalLen / 48 + 1;
		if (recvByte != totalLen + crcNum * 2 +1) return -1;

		// copy communication data to memory
		for(int k=0; k<FLOAT_NUMBER_TO_READ_1; k++)
		{
			memcpy(&f_val, &MFReadRSP[5+(k*4)], 4);
			dd[FLOAT_START_INDEX_1+k] = f_val;
		}

		
	}

	return 0;
}

DWORD FMReadMsg2()
{
	int recvByte;
	int sendByte;
	int dataLength;

	float f_val;
	
	unsigned char MFReadCMD[11];
	unsigned char MFReadRSP[2048];

	memset(MFReadCMD, '\0', sizeof(MFReadCMD));
	memset(MFReadRSP, '\0', sizeof(MFReadRSP));

	MFReadCMD[0] = (unsigned char)0xC3;
	MFReadCMD[1] = (unsigned char)0x05;
	MFReadCMD[2] = (unsigned char)0x00;
	MFReadCMD[3] = (unsigned char)0x00;
	MFReadCMD[4] = (unsigned char)0x9D;
	MFReadCMD[5] = (unsigned char)(FLOAT_START_INDEX_2%256);
	MFReadCMD[6] = (unsigned char)(FLOAT_START_INDEX_2/256);
	MFReadCMD[7] = (unsigned char)(FLOAT_NUMBER_TO_READ_2%256);
	MFReadCMD[8] = (unsigned char)(FLOAT_NUMBER_TO_READ_2/256);
	
	getCrc(MFReadCMD, 9);
	
		sendByte = send(sock, (char *)MFReadCMD, 11, 0);

		if(sendByte == SOCKET_ERROR)
		{
			saveLog("RTP float read thread2 fail to send");
			closeComm();

			return -1;
		}
		else
		{
			recvByte = recv(sock, (char *)MFReadRSP, sizeof(MFReadRSP), 0);

			if(recvByte == SOCKET_ERROR)
			{
				saveLog("RTP float read thread fail to receive");
				closeComm();

				return -1;
			}

			// Packet Header check
			if (MFReadRSP[0] != 0xc3) return -1;

			// Data Length check
			dataLength = *(unsigned short *)&MFReadRSP[1];
			if (dataLength != (FLOAT_NUMBER_TO_READ_2*4+1)) return -1;

			// Packet Length Check
			int totalLen = 4 + FLOAT_NUMBER_TO_READ_2 * 4 ; 
			int crcNum = totalLen / 48 + 1;
			if (recvByte != totalLen + crcNum * 2+1 ) return -1;

			for(int k=0; k<FLOAT_NUMBER_TO_READ_2; k++)
			{
				memcpy(&f_val, &MFReadRSP[5+(k*4)], 4);
				dd[FLOAT_START_INDEX_2+k] = f_val;			
			}

		}

	return 0;
}

// Float 쓰기
void write_float()
{
	int sendByte, recvByte;

	unsigned char SFWriteCMD[13];
	unsigned char SFWriteRSP[7];

	float Fval;

	memset(SFWriteCMD, '\0', sizeof(SFWriteCMD));
	memset(SFWriteRSP, '\0', sizeof(SFWriteRSP));

	SFWriteCMD[0] = 0xC3;
	SFWriteCMD[1] = 0x07;
	SFWriteCMD[2] = 0x00;
	SFWriteCMD[3] = 0x00;
	SFWriteCMD[4] = 0x9E;

	SFWriteCMD[5] = (unsigned char)(rtp_idx%256);
	SFWriteCMD[6] = (unsigned char)(rtp_idx/256);

	Fval = cc;
	memcpy(&SFWriteCMD[7], &Fval, 4);

	getCrc(SFWriteCMD, 11);

	// Lock Socket
	while (g_mutex) Sleep(1);
	g_mutex = true;

	sendByte = send(sock, (char *)SFWriteCMD, 13, 0);
	if(sendByte == SOCKET_ERROR)
	{
		saveLog("RTP float fail to write send");
		closeComm();
	}
	else
	{
		if((recvByte = recv(sock, (char*)SFWriteRSP, 7, 0)) == SOCKET_ERROR)
		{
			saveLog("RTP float fail to write receive");
			closeComm();
		}
	}
	g_mutex = false;
}

// Int 읽기
DWORD IMReadMsg()
{
	int sendByte, recvByte;

	short i_val;

	unsigned char IMReadCMD[11];
	unsigned char IMReadRSP[4096];

	IMReadCMD[0] = 0xC3;
	IMReadCMD[1] = 0x05;
	IMReadCMD[2] = 0x00;
	IMReadCMD[3] = 0x00;
	IMReadCMD[4] = 0x8D;

	IMReadCMD[5] = (unsigned char)(INT_START_INDEX%256);
	IMReadCMD[6] = (unsigned char)(INT_START_INDEX/256);
	IMReadCMD[7] = (unsigned char)(INT_NUMBER_TO_READ%256);
	IMReadCMD[8] = (unsigned char)(INT_NUMBER_TO_READ/256);

	getCrc(IMReadCMD, 9);

	sendByte = send(sock, (char *)IMReadCMD, 11, 0);

		if(sendByte == SOCKET_ERROR)
		{
			saveLog("RTP int read thread fail to send");
			closeComm();

			return -1;
		}
		else
		{
			recvByte = recv(sock, (char *)IMReadRSP, sizeof(IMReadRSP), 0);

			if(recvByte == SOCKET_ERROR)
			{
				saveLog("RTP int read thread fail to receive");
				closeComm();

				return -1;			
			}
			
			// Packet Header check
			if (IMReadRSP[0] != 0xc3) return -1;

			// Data Length check
			int dataLength = *(unsigned short *)&IMReadRSP[1];
			if (dataLength != (INT_NUMBER_TO_READ*2+1)) return -1;

			// Packet Length Check
			int totalLen = 4 + INT_NUMBER_TO_READ * 2 ; 
			int crcNum = totalLen / 48 + 1;
			if (recvByte != totalLen + crcNum * 2+1 ) return -1;


			for(int k=0; k<INT_NUMBER_TO_READ; k++)
			{
				memcpy(&i_val, &IMReadRSP[5+(k*2)], 2);
				getValue[INT_START_INDEX+k] = i_val;			
			}
		}


	return 0;
}

// Int 쓰기
void write_int()
{
	int sendByte, recvByte;

	unsigned char SIWriteCMD[11];
	unsigned char SIWriteRSP[7];

	short ival;

	memset(SIWriteCMD, '\0', sizeof(SIWriteCMD));
	memset(SIWriteRSP, '\0', sizeof(SIWriteRSP));

	SIWriteCMD[0] = 0xC3;
	SIWriteCMD[1] = 0x05;
	SIWriteCMD[2] = 0x00;
	SIWriteCMD[3] = 0x00;
	SIWriteCMD[4] = 0x8E;

	SIWriteCMD[5] = (unsigned char)(rtp_idx%256);
	SIWriteCMD[6] = (unsigned char)(rtp_idx/256);

	ival = iWriteValue;
	memcpy(&SIWriteCMD[7], &ival, 2);

	getCrc(SIWriteCMD, 9);

	// Lock Socket
	while (g_mutex) Sleep(1);
	g_mutex = true;

	sendByte = send(sock, (char *)SIWriteCMD, 11, 0);
	if(sendByte == SOCKET_ERROR)
	{
		saveLog("RTP int fail to write send");
		closeComm();
	}
	else
	{
		if((recvByte = recv(sock, (char*)SIWriteRSP, 7, 0)) == SOCKET_ERROR)
		{
			saveLog("RTP int to write receive");
			closeComm();
		}
	}

	g_mutex = false;
}

// Bool 읽기
DWORD BMReadMsg()
{
	int sendByte, recvByte;
	bool b_tmp=0;
	unsigned int result=0;

	unsigned char MBReadCMD[11];
	unsigned char MBReadRSP[4096];

	memset(MBReadCMD, '\0', sizeof(MBReadCMD));
	memset(MBReadRSP, '\0', sizeof(MBReadRSP));

	MBReadCMD[0] = (unsigned char)0xC3;
	MBReadCMD[1] = (unsigned char)0x05;
	MBReadCMD[2] = (unsigned char)0x00;
	MBReadCMD[3] = (unsigned char)0x00;
	MBReadCMD[4] = (unsigned char)0x82;
	
	MBReadCMD[5] = (unsigned char)(BOOL_START_INDEX%256);
	MBReadCMD[6] = (unsigned char)(BOOL_START_INDEX/256);
	MBReadCMD[7] = (unsigned char)(BOOL_NUMBER_TO_READ%256);
	MBReadCMD[8] = (unsigned char)(BOOL_NUMBER_TO_READ/256);

	getCrc(MBReadCMD, 9);

	sendByte = send(sock, (char *)MBReadCMD, 11, 0);
	
	if(sendByte == SOCKET_ERROR)
	{
		saveLog("RTP bool read thread fail to send");
		closeComm();

		return -1;

	}
	else
	{
		recvByte = recv(sock, (char *)MBReadRSP, sizeof(MBReadRSP), 0);

		if(recvByte == SOCKET_ERROR)
		{
			saveLog("RTP bool read thread fail to receive");
			closeComm();

			return -1;
		}
		else
		{
			
			// Packet Header check
			if (MBReadRSP[0] != 0xc3) return -1;

			// Data Length check
			int dataLength = *(unsigned short *)&MBReadRSP[1];
			if (dataLength != (BOOL_NUMBER_TO_READ/8+1)) return -1;

			// Packet Length Check
		
			if (recvByte != dataLength + 16 ) return -1;


			for(int i=5; i < 5+(((BOOL_NUMBER_TO_READ)+7)/8); i++)
			{
				for(int j=0;j<8; j++)
				{
					result = (unsigned int)MBReadRSP[i] & mask[j];
					
					if(result)
						b_tmp = 0x01;
					else
						b_tmp = 0x00;

					b_value[BOOL_START_INDEX+j+8*(i-5)] = b_tmp; 
				}
			}
		}
	} 

	return 0;
}

// Bool 쓰기
void write_bool()
{
	int sendByte, recvByte;

	unsigned char SBWriteCMD[10];
	unsigned char SBWriteRSP[7];

	bool bValue;

	memset(SBWriteCMD, '\0', sizeof(SBWriteCMD));
	memset(SBWriteRSP, '\0', sizeof(SBWriteRSP));

	SBWriteCMD[0] = 0xC3;
	SBWriteCMD[1] = 0x04;
	SBWriteCMD[2] = 0x00;
	SBWriteCMD[3] = 0x00;
	SBWriteCMD[4] = 0x83;

	SBWriteCMD[5] = (unsigned char)(rtp_idx%256);
	SBWriteCMD[6] = (unsigned char)(rtp_idx/256);

	bValue = bWriteValue;
	memcpy(&SBWriteCMD[7], &bValue, 1);

	getCrc(SBWriteCMD, 8);

	// Lock Socket
	while (g_mutex) Sleep(1);
	g_mutex = true;

	sendByte = send(sock, (char *)SBWriteCMD, 10, 0);
	if(sendByte == SOCKET_ERROR)
	{
		saveLog("RTP bool fail to write send");
		closeComm();
	}
	else
	{
		if((recvByte = recv(sock, (char*)SBWriteRSP, 7, 0)) == SOCKET_ERROR)
		{
			saveLog("RTP bool int to write receive");
			closeComm();
		}
	}
	g_mutex = false;
}

// SOE 데이터 읽기(2013.07.02, 류재국)
DWORD WINAPI SOEReadMsg(LPVOID arg)
{
	int recvByte;
	int sendByte;

	unsigned char SOEReadCMD[7];
	unsigned char SOEReadRSP[4096];

	short iLength = 0;
	short iData_Number_of_SOE = 0;
	short iRemainder = 0;
	
	short iTemp = 0;
	char cTemp;	
		
	memset(SOEReadCMD, '\0', sizeof(SOEReadCMD));
	memset(SOEReadRSP, '\0', sizeof(SOEReadRSP));

	SOEReadCMD[0] = (unsigned char)0xC3;
	SOEReadCMD[1] = (unsigned char)0x01;
	SOEReadCMD[2] = (unsigned char)0x00;
	SOEReadCMD[3] = (unsigned char)0x00;
	SOEReadCMD[4] = (unsigned char)0xA3;
		
	getCrc(SOEReadCMD, 5);
	
	while((bRtpConnected == true) && (bServerRun == true))
	{
		sendByte = send(sock, (char *)SOEReadCMD, 7, 0);

		if(sendByte == SOCKET_ERROR)
		{
			saveLog("RTP SOE read thread fail to send");
			closeComm();

			return -1;
		}
		else
		{
			recvByte = recv(sock, (char *)SOEReadRSP, sizeof(SOEReadRSP), 0);

			if(recvByte == SOCKET_ERROR)
			{
				saveLog("RTP SOE read thread fail to receive");							
				closeComm();
				return -1;
			}
			
			memcpy(&iLength, &SOEReadRSP[1], 2);

			if(iLength == 3)
			{
				iLength = 0;
				iData_Number_of_SOE = 0;
				continue;
			}
			else
			{
				iData_Number_of_SOE = (iLength - 3) / 12;
				iRemainder = (iLength - 3) % 12;

				if((iData_Number_of_SOE > 0) && (iRemainder == 0))
				{

					for(int i = 0; i < iData_Number_of_SOE; i++)
					{
						iTemp =0;
						memcpy(&iTemp, &SOEReadRSP[7+(i*12)], 2);
						arSOE_Data[i].iIndex = (int)iTemp;

						iTemp = 0;
						memcpy(&iTemp, &SOEReadRSP[(7+(i*12))+2], 2);
						arSOE_Data[i].iValue = (int)iTemp;

						memcpy(&cTemp, &SOEReadRSP[(7+(i*12))+4], 1);
						arSOE_Data[i].iYear = (int)cTemp;

						memcpy(&cTemp, &SOEReadRSP[(7+(i*12))+5], 1);
						arSOE_Data[i].iMonth = (int)cTemp;

						memcpy(&cTemp, &SOEReadRSP[(7+(i*12))+6], 1);
						arSOE_Data[i].iDay = (int)cTemp;

						memcpy(&cTemp, &SOEReadRSP[(7+(i*12))+7], 1);
						arSOE_Data[i].iHour = (int)cTemp;

						memcpy(&cTemp, &SOEReadRSP[(7+(i*12))+8], 1);
						arSOE_Data[i].iMinute = (int)cTemp;

						memcpy(&cTemp, &SOEReadRSP[(7+(i*12))+9], 1);
						arSOE_Data[i].iSecond = (int)cTemp;

						memcpy(&iTemp, &SOEReadRSP[(7+(i*12))+10], 2);
						arSOE_Data[i].iMilliSecond = (int)iTemp;
					}
					
					Save_SOE_Data(iLength, iData_Number_of_SOE);
				}
				else
				{
					continue;
				}

				iLength = 0;
				iData_Number_of_SOE = 0;
				iRemainder = 0;
			}
						
		}			

		Sleep(SCAN_CYCLE);		
	}
	
	closeComm();

	return 0;
	
}

//SOE 데이터 텍스트 파일로 저장(2013.07.03, 류재국)
void Save_SOE_Data(short iLength, short iData_Number_of_SOE)
{
	ofstream ofile;
	ofile.open("C:\\SOE_Data.Log", ios::out | ios::app);

	if(ofile.is_open())
	{
		char szLog[MAX_PATH];
		
		sprintf(szLog, "Length : %d\n", iLength);
		ofile.write(szLog, strlen(szLog));
		sprintf(szLog, "Count : %d\n", iData_Number_of_SOE);
		ofile.write(szLog, strlen(szLog));

		for(int i = 0; i < iData_Number_of_SOE; i++)
		{
			memset(szLog, 0, sizeof(szLog));

			sprintf(szLog, "%02d-%02d-%02d %2d:%02d:%02d.%03d\tIndex : %d, Value : %d\n",  
				arSOE_Data[i].iYear, arSOE_Data[i].iMonth, arSOE_Data[i].iDay,
				arSOE_Data[i].iHour, arSOE_Data[i].iMinute, arSOE_Data[i].iSecond, arSOE_Data[i].iMilliSecond,
				arSOE_Data[i].iIndex, arSOE_Data[i].iValue);

			ofile.write(szLog, strlen(szLog));
		}		
	}

	ofile.close();
}


// FIFO 기능 추가(2013.07.04, 류재국)
DWORD WINAPI FIFOReadMsg(LPVOID arg)
{
	int recvByte;
	int sendByte;

	unsigned char FIFOReadCMD[10];
	unsigned char FIFOReadRSP[4096];

	short iData_Number_of_FIFO = 0;
	
	float fTemp = 0.0;
	short iTemp = 0;
	char cTemp;	
		
	memset(FIFOReadCMD, '\0', sizeof(FIFOReadCMD));
	memset(FIFOReadRSP, '\0', sizeof(FIFOReadRSP));

	FIFOReadCMD[0] = (unsigned char)0xC3;
	FIFOReadCMD[1] = (unsigned char)0x04;
	FIFOReadCMD[2] = (unsigned char)0x00;
	FIFOReadCMD[3] = (unsigned char)0x00;
	FIFOReadCMD[4] = (unsigned char)0xA7;

	FIFOReadCMD[5] = (unsigned char)FIFO_VALUE_INDEX%256;
	FIFOReadCMD[6] = (unsigned char)FIFO_VALUE_INDEX/256;
	FIFOReadCMD[7] = (unsigned char)0x70;
		
	getCrc(FIFOReadCMD, 8);
	
	while(bRtpConnected)
	{
		sendByte = send(sock, (char *)FIFOReadCMD, 10, 0);

		if(sendByte == SOCKET_ERROR)
		{
			saveLog("RTP FIFO read thread fail to send");
			closeComm();

			return -1;
		}
		else
		{
			recvByte = recv(sock, (char *)FIFOReadRSP, sizeof(FIFOReadRSP), 0);

			if(recvByte == SOCKET_ERROR)
			{
				saveLog("RTP FIFO read thread fail to receive");							
				closeComm();
				return -1;
			}
			
			memcpy(&cTemp, &FIFOReadRSP[5], 1);
			iData_Number_of_FIFO = (int)cTemp;	

			if(iData_Number_of_FIFO == 0)
			{
				continue;
			}
			else
			{
				for(int i = 0; i < iData_Number_of_FIFO; i++)
				{
					fTemp =0.0;
					memcpy(&fTemp, &FIFOReadRSP[6+(i*12)], 4);
					arFIFO_Data[i].fValue = fTemp;

					memcpy(&cTemp, &FIFOReadRSP[(6+(i*12))+4], 1);
					arFIFO_Data[i].iYear = (int)cTemp;

					memcpy(&cTemp, &FIFOReadRSP[(6+(i*12))+5], 1);
					arFIFO_Data[i].iMonth = (int)cTemp;

					memcpy(&cTemp, &FIFOReadRSP[(6+(i*12))+6], 1);
					arFIFO_Data[i].iDay = (int)cTemp;

					memcpy(&cTemp, &FIFOReadRSP[(6+(i*12))+7], 1);
					arFIFO_Data[i].iHour = (int)cTemp;

					memcpy(&cTemp, &FIFOReadRSP[(6+(i*12))+8], 1);
					arFIFO_Data[i].iMinute = (int)cTemp;

					memcpy(&cTemp, &FIFOReadRSP[(6+(i*12))+9], 1);
					arFIFO_Data[i].iSecond = (int)cTemp;

					memcpy(&iTemp, &FIFOReadRSP[(6+(i*12))+10], 2);
					arFIFO_Data[i].iMilliSecond = (int)iTemp;
				}
				
				Save_FIFO_Data(iData_Number_of_FIFO);
				
				iData_Number_of_FIFO = 0;
			}
						
		}

		Sleep(SCAN_CYCLE);		
	}
	
	closeComm();

	return 0;
}

//FIFO 데이터 텍스트 파일로 저장(2013.07.04, 류재국)
void Save_FIFO_Data(short iData_Number_of_FIFO)
{
	ofstream ofile;
	ofile.open("C:\\FIFO_Data.Log", ios::out | ios::app);

	if(ofile.is_open())
	{
		char szLog[MAX_PATH];

		sprintf(szLog, "Count : %d\n", iData_Number_of_FIFO);
		ofile.write(szLog, strlen(szLog));

		for(int i = 0; i < iData_Number_of_FIFO; i++)
		{
			memset(szLog, 0, sizeof(szLog));

			sprintf(szLog, "%02d-%02d-%02d %2d:%02d:%02d.%03d\tValue : %f\n", 
				arFIFO_Data[i].iYear, arFIFO_Data[i].iMonth, arFIFO_Data[i].iDay,
				arFIFO_Data[i].iHour, arFIFO_Data[i].iMinute, arFIFO_Data[i].iSecond, arFIFO_Data[i].iMilliSecond,
				arFIFO_Data[i].fValue);

			ofile.write(szLog, strlen(szLog));
		}

		
	}

	ofile.close();
}
