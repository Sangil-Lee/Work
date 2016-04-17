#include "StdAfx.h"
#include "common.h"
#include <fstream>

using namespace std;

SOCKET sock = NULL;
bool bServerRun = false;
bool bRtpConnected = false;


bool startServer()
{
	HANDLE hThread = NULL;
	DWORD dwID = 0;
	bServerRun = true;
	
	hThread = CreateThread(NULL, NULL, threadServer, NULL, 0, &dwID);

	if(hThread == NULL)
	{
		bServerRun = false;

		if(::WaitForSingleObject(hThread, 1000L) == WAIT_TIMEOUT)
			::TerminateThread(hThread, 1L);

		::CloseHandle(hThread);
		
		saveLog("Server fail to start");
		return false;
	}

	saveLog("Server start");
	return true;
}

// RTP 장비와의 통신을 체크하고 통신이 끊어지면 재접속함
DWORD WINAPI threadServer(LPVOID lpVoid)
{
	while(bServerRun)
	{
		if(!bRtpConnected)
		{
			initComm();
		}

		Sleep(COMM_CHECK_CYCLE);
	}

	closeComm();

	return 0;
}

// RTP 장비와 접속하고 RTP로부터 값을 읽어들이는 Thread 함수(ReadMsg) 실행
bool initComm()
{
	SOCKADDR_IN svrAddr;
	bRtpConnected = false;
		
	// 소켓 함수 생성
	sock = socket(AF_INET, SOCK_STREAM, 0);


	if(sock == INVALID_SOCKET) 
	{
		saveLog("Socket fail to create");
		closeComm();
		return false;
	}	else
	{
		// 자동 정보 입력 (자동은 SMC의 Configuration Parameter를 이용)
		svrAddr.sin_family = AF_INET;
		svrAddr.sin_port = htons(PORT_NUM);
		svrAddr.sin_addr.s_addr = inet_addr(IPADDR);

		// connect
		if(connect(sock, (SOCKADDR *)&svrAddr, sizeof(svrAddr)) == SOCKET_ERROR)
		{
			saveLog("Socket fail to connect");
			closeComm();
			return false;
		}
	}
	
	bRtpConnected = true;
	saveLog("RTP communication connect");

	/// Running Data Acquisition Thread
	HANDLE hThread = NULL;

	DWORD dwID = 0;
	
	// RTP로부터 값을 읽어들이는 Thread 함수 실행
	hThread = CreateThread(NULL, 0, ReadMsg, NULL, 0, &dwID);

	if(hThread == NULL)
	{
		saveLog("RTP read thread fail to start");
		return false;
	}
    
	return true;
}

void closeComm()
{
	closesocket(sock);
	sock = NULL;

	bRtpConnected = false;
}

void stopServer()
{
	bServerRun = false;
	saveLog("Server Stop");

	closeComm();
}

void saveLog(const char* szMsg)
{
	ofstream ofile;
	ofile.open("C:\\DAServer.Log", ios::out | ios::app);

	if(ofile.is_open())
	{
		SYSTEMTIME st;
		GetLocalTime(&st);

		char szLog[MAX_PATH];
		memset(szLog, 0, sizeof(szLog));

		sprintf(szLog, "%04d-%02d-%02d %02d:%02d:%02d\t%s\n",
			st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, szMsg);

		ofile.write(szLog, strlen(szLog));
	}

	ofile.close();
}



