// Filename : DasSocketObject.h 
// ***************************************************************************
// *                                                                         *
// *    Copyright (C) 2002,BNF Technology Inc., All rights reserved.         *
// *    Proprietary information, BNF Technology Inc.                         *
// *                                                                         *
// ***************************************************************************
//
// ***** HIGH LEVEL DESCRIPTION **********************************************
//
//  This module defines the socketObject class.
//
// **** REVISION HISTORY *****************************************************
//
// Revision 1  2002-03-30
// Author: Arun
// Initial revision
//
//
// ***************************************************************************
#ifndef DAS_SOCKET_OBJECT_H
#define DAS_SOCKET_OBJECT_H
//
// System Include Files:
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <error.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <iostream>
#include <fstream>
//
// User Defined Include Files:
//
// External Variables:
//
// Local Constant Definitions:
//
// Function Prototypes:
//
// The Class Definition
//
#define MAX_FRAME_LENGTH 	4096			// Tcp Message Length
#define	DAS_HEADER_SIZE		4
//RTP2200 port address
#define DAS_PORT			50199		//The DAS can be communicated only through port 50199

using namespace std;
class DasSocket
{
private:
	char hostName[12];
	int sockFd;
	in_port_t port;
	short	 dasNode;

public:
   DasSocket(const char* addr);
   ~DasSocket();
   

	void	setDasNodeNum(short dasNodeNum) { dasNode = dasNodeNum ; };
	short 	initSocket(short dasNode,in_port_t dasPort);
	short	Connect(const char* ipaddr);
	void	Close() { close(sockFd);};	
	void	closeAndConnect();
   
	int		sendMessage(void *sendMessageBuffer, short bytesToSend);
	int 	receiveMessage(void*);
	int		Ignore_sigpipe() ;
   

// Function to print the error of socket receive function
	void printReceiveErrorMessage(int errorno);

// Function to print the error of socket send function 
	void printSendErrorMessage(int errorno);   

	int	getSockFd() { return sockFd ;}

   //Public Attributes				//socket file descriptor

	ofstream	ErrorFile ;
	const char* ipAddr;

};

#endif
