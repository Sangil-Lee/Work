// Filename :DasSocketObject.C 
// ***************************************************************************
// *                                                                         *
// *    Copyright (C) 2002,BNF Technology Inc., All rights reserved.         *
// *    Proprietary information, BNF Technology Inc.                         *
// *                                                                         *
// ***************************************************************************
//
// ***** HIGH LEVEL DESCRIPTION **********************************************
//
// This module contains the implementation of the DASSocketObject class as 
// defined in DasSocketObject.h.
//
// **** REVISION HISTORY *****************************************************
//
// Revision 1  
// Author: Arun
//
// Revision 1  
// Author: Rajesh
// Initial revision
//
// ***************************************************************************
// System Include Files:
//
#include <fcntl.h>

//
// User Defined Include Files:
//
#include "DasSocketObject.h"	//socket object definition

// External Variables: NONE
//
// Local Constant Definitions:
//
//
// Function Definitions:
//
extern	void	openUDP() ;

DasSocket::DasSocket(const char* addr):ipAddr(addr)//the construtor
{
	//Contructing the socket.
}

DasSocket::~DasSocket ()		//the Destructor
{
	ErrorFile << "DasSocketObj :: Destructing the Dassocket " << endl ;
	printf("Destructing this DasSocket \n");
}


/* Initiate communication with a remote server.				
   returns: the file descriptor used for communication or retries to initialize sockets recursively
*/

short DasSocket::initSocket(short dasNodeNum,in_port_t dasPort)
{

	//cout<<"Inside initSocket"<<endl;
	port 	= dasPort;
	dasNode = dasNodeNum;

	if ( (Ignore_sigpipe() != 0 ) || (sockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
      		cout << "DasSocketObject..socket ERROR: "  << endl;
      		switch (errno)
      		{
         		case EAFNOSUPPORT:
            			cout << "The specified address family is not supported in this version of the system." << endl;
            			break;
         		case EHOSTDOWN:
            			cout << "The networking subsystem is not up." << endl;
            			break;
         		case EMFILE:
            			cout << "The per-process descriptor table or the system table of open files is full." << endl;
            			break;
         		case ENOBUFS:
            			cout << "No buffer space is available." << endl;
            			break;
         		case EPROTONOSUPPORT:
            			cout << "The specified protocol is not supported." << endl;
            			break;
         		case EPROTOTYPE:
            			cout << "The type of socket and protocol do not match." << endl;
            			break;
         		case ESOCKTNOSUPPORT:
            			cout << "The specified socket type is not supported in this address family." << endl;
            			break;
         		case ETIMEDOUT:
            			cout << "Connection timed out." << endl;
            			break;
         		default:
            			cout << "UNKNOWN ERROR from socket call." << endl;
            			break;
      		} // endswitch

	}

	int	flags	= fcntl(sockFd , F_GETFL , 0 );
	fcntl (sockFd , F_SETFL , flags| O_NONBLOCK ) ;
	//set the socket options

	struct timeval	waitTime ;
	waitTime.tv_sec		=	0;
	waitTime.tv_usec	=	500;
	setsockopt(sockFd , SOL_SOCKET , SO_RCVTIMEO ,(char*)&waitTime , sizeof(timeval) );
	setsockopt(sockFd , SOL_SOCKET , SO_SNDTIMEO ,(char*)&waitTime , sizeof(timeval) );
	int	reuse = 1 ;
	setsockopt(sockFd , SOL_SOCKET , SO_REUSEADDR ,(char*)&reuse , sizeof(reuse) );

	return sockFd;
}

short DasSocket::Connect(const char* ipaddr)
{
	struct sockaddr_in	das_server;
	struct hostent	*hp;
	short	retVal ;
		
	cout << " DasSocketObj :: Trying to connect/Reconnect to the Das node : " << dasNode << endl ;
	ErrorFile << " DasSocketObj :: Trying to connect/Reconnect to the Das node : " << dasNode << endl ;
	
	//memcpy ((char *)&das_server.sin_addr, hp->h_addr_list[0], hp->h_length);
	memcpy ((char *)&das_server.sin_addr, ipaddr, strlen(ipaddr)-1);

	das_server.sin_port = htons((short)port);
	das_server.sin_family = AF_INET;
//	das_server.sin_addr.s_addr = htonl(INADDR_ANY);
/*
	if(bind( sockFd, (struct sockaddr*)(&das_server), sizeof(das_server)) < 0 )
	{
		cout << "Error while binding " << errno << " error :" << strerror(errno) << endl ;
		return -1 ; 
	}
*/
	cout <<"Inside Connect :: Calling connect()" << endl ;
    if (((retVal = connect (sockFd, (struct sockaddr *)(&das_server), sizeof(das_server) )) < 0)) 
   {
		cout << "DASSocketObject..connect ERROR: " << endl;
        
		//Failure---- Try after a short delay
		cout<<"Failed to connect .....Will retry after a short delay"<<endl;
//		sleep(1);
	} //End of while 

	cout << " Error....." << strerror(errno ) << "retVal :: " << retVal << endl ;
	if(errno == EINPROGRESS )
		cout << "hey ...its in progress .... " << endl ;
/*
	if(retVal == -1)
	{
		Close();
		return -1;
	}
*/
#if 1
    //send the data
    struct timespec      interval;               //for nanosleep
    struct timespec      remainder;              //for nanosleep
    interval.tv_sec = 0;
    interval.tv_nsec = 100000000;       //default : 100000000
    nanosleep(&interval,&remainder);
#endif
	//sleep(1);
	//Success
	//To change the position of setting connected status
	return 0;
 
}//End of Connect Function

/* 								
* parameters:							
*       							
* returns:						
*      a negative value indicates an error occurred		
*      otherwise the number of bytes read is returned		
*/								

int DasSocket::sendMessage(void *sendMessageBuffer, short bytesToSend)
{
	ssize_t bytesSend ;
	//while(bytesSend = write(sockFd, sendMessageBuffer, bytesToSend),bytesSend == -1 && errno == EINTR);
	//cout <<"DasScoketObject.o :: Calling write " << endl ;
	bytesSend = write(sockFd, sendMessageBuffer, bytesToSend);
	//cout << "DasSocket.o Returning from sendMessage " << endl ;
	if(bytesSend < 0)
		printSendErrorMessage(errno);
	return (int) bytesSend;

//To be added
}    

/*
	Description : To read message from the DAS node..
*/

int DasSocket::receiveMessage (void* pBuff)
{
   	char readMessageBuffer[MAX_FRAME_LENGTH];
	ssize_t		numRead;
	ssize_t		totRead = 0;
	ssize_t		numLeft;
	char*		currPos	=	(char*)pBuff;
	short		readRet ;
 	
	//cout << "Calling actual read to receive the message " << endl ;
	//numRead = read(sockFd, currPos, DAS_HEADER_SIZE);
	while ( (numRead = read(sockFd, currPos, DAS_HEADER_SIZE)) == -1 && errno == EINTR ) ;
	//cout << "Out from actual read to receive the message " << endl ;
	ErrorFile << "DasSocketObject :: read returned a value of " << numRead << strerror(errno) 
			<< endl ;

	if (numRead < 0 && errno == ECONNRESET )
	{
		//connection is lost so retry connection
		closeAndConnect();
		return -2 ;
	}
	if (numRead >0 ) currPos += numRead ;
	if(numRead < DAS_HEADER_SIZE)
	{
		ErrorFile << "DasSocketObject :: numRead < DAS_HEADER_SIZE " << endl ;
		short	balRead ;
		if(numRead > 0 )
		{
			balRead = DAS_HEADER_SIZE - numRead;

			//numRead += read(sockFd, currPos, balRead);
			readRet = read(sockFd, currPos, balRead);
			if (readRet < 0 && errno == ECONNRESET )
			{
				//connection is lost so retry connection
				closeAndConnect();
				return -2 ;
			}
			else
				numRead += readRet ;
		}
		else
		{
			balRead = DAS_HEADER_SIZE ;
			numRead = read(sockFd, currPos, balRead);
			if (numRead < 0 && errno == ECONNRESET )
			{
				//connection is lost so retry connection
				closeAndConnect();
				return -2 ;
			}
		}
		if (numRead >0 ) 
		{
			currPos += numRead ;
			totRead	+=	numRead;
		}
	}
	else
		totRead	+=	numRead;
	

	short bodySize = (unsigned char) *((char*)pBuff +1);
	//cout << "\t\t\t:: reading the bodysize of " << bodySize <<endl;
	numLeft = 	bodySize;
	numRead	=	0;
	
	while(numLeft > 0)
	{
		//cout << " Inside while after reading some data " << endl;
		//numRead = read(sockFd, currPos, numLeft);
		while ( (numRead = read(sockFd, currPos, numLeft)) == -1 && errno == EINTR ) ;
		ErrorFile << "DasSocketObject :: Reading the body .. returned a value of " 
				<< numRead << strerror(errno)<<endl; 
		if (numRead < 0 && errno == ECONNRESET )
		{
			//connection is lost so retry connection
			closeAndConnect();
			return -2 ;
		}

		if(numRead > 0)
		{
			numLeft	-=	numRead;
			totRead	+=	numRead;
			currPos	+=	numRead;
		}
		if(numRead < 0 )
		{
			printReceiveErrorMessage(errno);
			if (errno == EAGAIN )
			{
				struct timespec      interval;               //for nanosleep
				struct timespec      remainder;              //for nanosleep
				interval.tv_sec = 0;
				interval.tv_nsec = 100000;       
				nanosleep(&interval,&remainder);
				ErrorFile << "DasSocketObject :: While reading the body.EAGAIN :: Will Read again ..." << endl ;
			}
			else
			{
				ErrorFile << "DasSocketObject :: Returning from numRead < 0 && errno != EAGAIN" << endl ;
				closeAndConnect();
				return -2;
			}
		}
		if(numRead == 0 && errno == ECONNRESET )
		{
			printReceiveErrorMessage(errno);
			ErrorFile << "DasSocketObject :: Returning from numRead = 0 && errno == ECONNRESET" << endl ;
			closeAndConnect();
			return -2;
		}
	}   

	return totRead;

}


void	DasSocket::closeAndConnect()
{
	bool	notConnected = true;


	Close();
	while(notConnected)
	{
		if(initSocket(dasNode,(in_port_t)DAS_PORT)<0)
			//if(debug > DEB_LEVEL0)
			cout<<"DASScoketObj :: Cannot initialize the Socket for DasNode :" << dasNode << endl;

		//If cannot initialize the socket then what to do ?????

		// Connect to the RTP 
		//if(debug > DEB_LEVEL3)
		cout<<"DASSocketObj::Calling Connect To Connect to the DAS Node : "<< dasNode << endl;
		if(Connect(ipAddr)<0)
		{
			notConnected = true;
			sleep(1);
			//if(debug > DEB_LEVEL0)
			cout << " DASSocketObj ::Failed to connect to the DAS Node Number::"<< dasNode << endl;
		}
		else
		{
			notConnected = false;
			//if(debug > DEB_LEVEL0)
			cout <<" DASSocketObj ::Successfully Connected to the DAS Node:"<< dasNode << endl;
		}
	}
}


void DasSocket::printReceiveErrorMessage(int errorno)
{
	switch(errorno)
	{
		case EBADF:
		cout<<"The socket parameter is not valid."<<endl;
		break;
		
		case ECONNRESET:
		cout<<" A connection was forcibly closed by a peer."<<endl;
		break;
		
		case EINTR:
		cout<<"A signal interrupted this function before any data was available."<<endl;
		break;
		
		case EIO:
		cout<<" An I/O error occurred while reading from or writing to the file system."<<endl;
		break;
		
		case ENOBUFS:
		cout<<" Insufficient resources were available in the system to complete the call."<<endl;
		break;
		
		case ENOMEM:
		cout<<"The system did not have sufficient memory to fulfill the request."<<endl;
		break;
		
		case ENOSR:
		cout<<" The available STREAMS resources were insufficient for the operation to complete."<<endl;
		break;
		
		case ENOTCONN:
		cout<<"A receive is attempted on a connection-oriented socket that is not connected."<<endl;
		break;
		
		case ENOTSOCK:
		cout<<"The socket parameter refers to a file, not a socket."<<endl;
		break;

		case ETIMEDOUT:
		cout<<" The connection timed out during connection establishment, or due to a transmission timeout on active connection."<<endl;
		break;
		
		case EWOULDBLOCK:
		cout<<" The socket is marked nonblocking and no data is ready to be received."<<endl;
		break;
		
		default:
		cout<<"Undefined Receive error"<<endl;
		break;
	}
}

 
// Function to print the error of socket send function 

void DasSocket::printSendErrorMessage(int errorno)
{
	switch(errorno)
	{
	
		case EACCES:
		cout<<"Search permission is denied for a component of the path prefix; or write access to the named socket is denied."<<endl;
		break;
	
		case EAFNOSUPPORT:
		cout<<" You cannot use addresses in the specified address family with this socket."<<endl;
		break;
	
		case EBADF:
		cout<<"The socket parameter is not valid."<<endl;
		break;
	
		case ECONNRESET:
		cout<<"A peer forcibly closed a connection."<<endl;
		break;
	
		case EDESTADDRREQ:
		cout<<" You did not specify a destination address for the connectionless socket that also did not have its peer address set."<<endl;
		break;
	
		case EHOSTUNREACH:
		cout<<" The destination host is unreachable.  Possible reasons include the host is down or the router has no route."<<endl;
		break;
	
		case EINTR:
		cout<<"A signal interrupted sendmsg before any data was transmitted."<<endl;
		break;
		
		case EIO:
		cout<<" An I/O error occurred while reading from or writing to the file system."<<endl;
		break;
		
		case EISCONN:
		cout<<"The connection-oriented socket for which a destination address was specified is already connected."<<endl;
		break;
		
		case EMSGSIZE:
		cout<<"The message is too large to be sent all at once, as the socket requires."<<endl;
		break;
		
		case ENETUNREACH:
		cout<<" The destination network is unreachable."<<endl;
		break;
		
		case ENOBUFS:
		cout<<" Insufficient resources were available in the system to complete the call."<<endl;
		break;
		
		case ENOMEM:
		cout<<"The system did not have sufficient memory to fulfill the request."<<endl;
		break;
		
		case ENOSR:
		cout<<"The available STREAMS resources were insufficient for the operation to complete."<<endl;
		break;
		
		case ENOTCONN:
		cout<<" A socket is connection-oriented but is not connected."<<endl;
		break;
		
		case EWOULDBLOCK:
		cout<<"  The socket is marked nonblocking, and no space is available for the sendmsg() function."<<endl;
		break;
		
		default:
		cout<<"Undefined Send Error"<<endl;
		break;
	}
}



// return 1 if error, 0 if OK
int DasSocket::Ignore_sigpipe()
{
   struct sigaction act;

   if (sigaction(SIGPIPE, (struct sigaction *)NULL, &act) < 0) return 1;
   if (act.sa_handler == SIG_DFL)
   {
      act.sa_handler = SIG_IGN;
      if (sigaction(SIGPIPE, &act, (struct sigaction *)NULL) < 0) return 1;
   }  
   return 0;
}

