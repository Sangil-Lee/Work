pvaClientCPP
============

pvaClient is a synchronous client interface to pvAccess,
which is callback based.
pvaClient is thus easier to use than pvAccess itself.

See documentation/pvaClientCPP.html for details.

Building
--------

If a proper RELEASE.local file exists one directory level above pvaClientCPP
then just type:

    make

If RELEASE.local does not exist then look at <b>configure/RELEASE</b>
for directions for how to build.

Examples
------------

Project exampleCPP has examples for pvaClientCPP

Status
------

* The API is for EPICS Version 4 release 4.5.0
* Everything defined in pvaClient.h is ready but see below for remaining work.
* Everything defined in pvaClientMultiChannel.h is ready but see below for remaining work.


pvaClientChannel
---------------

Channel::getField and channelArray are not supported for release 4.5

pvaClientMultiChannel
---------------

For release 4.5 support is available for multiDouble and NTMultiChannel.
In the future additional support should be provided that at least includes NTScalarMultiChannel.

Testing with some channels not connected has not been done.
