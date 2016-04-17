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

// RTP ������ ����� üũ�ϰ� ����� �������� ��������
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

// RTP ���� �����ϰ� RTP�κ��� ���� �о���̴� Thread �Լ�(ReadMsg) ����
bool initComm()
{
	SOCKADDR_IN svrAddr;
	bRtpConnected = false;
		
	// ���� �Լ� ����
	sock = socket(AF_INET, SOCK_STREAM, 0);


	if(sock == INVALID_SOCKET) 
	{
		saveLog("Socket fail to create");
		closeComm();
		return false;
	}	else
	{
		// �ڵ� ���� �Է� (�ڵ��� SMC�� Configuration Parameter�� �̿�)
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
	
	// RTP�κ��� ���� �о���̴� Thread �Լ� ����
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



