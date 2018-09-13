#include <sys/epoll.h>
#include <errno.h>
....
....
int retVal = -1;
socklen_t retValLen = sizeof (retVal);

int status = connect(socketFD, ...);
if (status == 0)
{
	// OK -- socket is ready for IO
}
else if (errno == EINPROGRESS)
{
	struct epoll_event newPeerConnectionEvent;
	int epollFD = -1;
	struct epoll_event processableEvents;
	unsigned int numEvents = -1;

	if ((epollFD = epoll_create (1)) == -1)
	{
		printf ("Could not create the epoll FD list. Aborting!");
		exit (2);
	}     

	newPeerConnectionEvent.data.fd = socketFD;
	newPeerConnectionEvent.events = EPOLLOUT | EPOLLIN | EPOLLERR;

	if (epoll_ctl (epollFD, EPOLL_CTL_ADD, socketFD, &newPeerConnectionEvent) == -1)
	{
		printf ("Could not add the socket FD to the epoll FD list. Aborting!");
		exit (2);
	}

	numEvents = epoll_wait (epollFD, &processableEvents, 1, -1);

	if (numEvents < 0)
	{
		printf ("Serious error in epoll setup: epoll_wait () returned < 0 status!");
		exit (2);
	}

	if (getsockopt (socketFD, SOL_SOCKET, SO_ERROR, &retVal, &retValLen) < 0)
	{
		// ERROR, fail somehow, close socket
	}

	if (retVal != 0) 
	{
		// ERROR: connect did not "go through"
	}   
}
else
{
	// ERROR: connect did not "go through" for other non-recoverable reasons.
	switch (errno)
	{
		...
	}
}
