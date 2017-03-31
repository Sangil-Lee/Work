/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * pvAccessCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#ifdef _WIN32
// needed for ip_mreq
#include <ws2tcpip.h>
#endif

#include <pv/blockingUDP.h>
#include <pv/pvaConstants.h>
#include <pv/inetAddressUtil.h>
#include <pv/logger.h>
#include <pv/likely.h>

#include <pv/byteBuffer.h>
#include <pv/lock.h>

#include <osdSock.h>
#include <osiSock.h>
#include <epicsThread.h>

#include <cstdio>
#include <sys/types.h>

using namespace epics::pvData;
using namespace std;
using std::tr1::static_pointer_cast;

namespace epics {
namespace pvAccess {

#ifdef __vxworks
inline int sendto(int s, const char *buf, size_t len, int flags, const struct sockaddr *to, int tolen)
{
    return ::sendto(s, const_cast<char*>(buf), len, flags, const_cast<struct sockaddr *>(to), tolen);
}
#endif

// reserve some space for CMD_ORIGIN_TAG message
#define RECEIVE_BUFFER_PRE_RESERVE (PVA_MESSAGE_HEADER_SIZE + 16)

PVACCESS_REFCOUNT_MONITOR_DEFINE(blockingUDPTransport);

BlockingUDPTransport::BlockingUDPTransport(bool serverFlag,
        ResponseHandler::shared_pointer const & responseHandler, SOCKET channel,
        osiSockAddr& bindAddress,
        short /*remoteTransportRevision*/) :
    _closed(),
    _responseHandler(responseHandler),
    _channel(channel),
    _bindAddress(bindAddress),
    _sendAddresses(0),
    _ignoredAddresses(0),
    _tappedNIF(0),
    _sendToEnabled(false),
    _localMulticastAddressEnabled(false),
    _receiveBuffer(new ByteBuffer(MAX_UDP_RECV+RECEIVE_BUFFER_PRE_RESERVE)),
    _sendBuffer(new ByteBuffer(MAX_UDP_RECV)),
    _lastMessageStartPosition(0),
    _clientServerWithEndianFlag(
        (serverFlag ? 0x40 : 0x00) | ((EPICS_BYTE_ORDER == EPICS_ENDIAN_BIG) ? 0x80 : 0x00))
{
    PVACCESS_REFCOUNT_MONITOR_CONSTRUCT(blockingUDPTransport);
    assert(_responseHandler.get());

    osiSocklen_t sockLen = sizeof(sockaddr);
    // read the actual socket info
    int retval = ::getsockname(_channel, &_remoteAddress.sa, &sockLen);
    if(retval<0) {
        // error obtaining remote address, fallback to bindAddress
        _remoteAddress = _bindAddress;

        char strBuffer[64];
        epicsSocketConvertErrnoToString(strBuffer, sizeof(strBuffer));
        LOG(logLevelDebug, "getsockname error: %s.", strBuffer);
        _remoteName = "<unknown>:0";
    } else {
        char strBuffer[64];
        sockAddrToA(&_remoteAddress.sa, strBuffer, sizeof(strBuffer));
        _remoteName = strBuffer;
    }
}

BlockingUDPTransport::~BlockingUDPTransport() {
    PVACCESS_REFCOUNT_MONITOR_DESTRUCT(blockingUDPTransport);

    close(true); // close the socket and stop the thread.

    // TODO use auto_ptr class members

    if (_sendAddresses) delete _sendAddresses;
    if (_ignoredAddresses) delete _ignoredAddresses;
    if (_tappedNIF) delete _tappedNIF;
}

void BlockingUDPTransport::start() {

    string threadName = "UDP-rx " + inetAddressToString(_bindAddress);

    if (IS_LOGGABLE(logLevelTrace))
    {
        LOG(logLevelTrace, "Starting thread: %s.", threadName.c_str());
    }

    _thread.reset(new epicsThread(*this, threadName.c_str(),
                                  epicsThreadGetStackSize(epicsThreadStackSmall),
                                  epicsThreadPriorityMedium));
    _thread->start();
}

void BlockingUDPTransport::close() {
    close(true);
}

void BlockingUDPTransport::close(bool waitForThreadToComplete) {
    {
        Lock guard(_mutex);
        if(_closed.get()) return;
        _closed.set();
    }

    if (IS_LOGGABLE(logLevelDebug))
    {
        LOG(logLevelDebug,
            "UDP socket %s closed.",
            inetAddressToString(_bindAddress).c_str());
    }

    epicsSocketSystemCallInterruptMechanismQueryInfo info  =
        epicsSocketSystemCallInterruptMechanismQuery ();
    switch ( info )
    {
    case esscimqi_socketCloseRequired:
        epicsSocketDestroy ( _channel );
        break;
    case esscimqi_socketBothShutdownRequired:
    {
        /*int status =*/ ::shutdown ( _channel, SHUT_RDWR );
        /*
        if ( status ) {
            char sockErrBuf[64];
            epicsSocketConvertErrnoToString (
                sockErrBuf, sizeof ( sockErrBuf ) );
        LOG(logLevelDebug,
            "UDP socket %s failed to shutdown: %s.",
            inetAddressToString(_bindAddress).c_str(), sockErrBuf);
        }
        */
        epicsSocketDestroy ( _channel );
    }
    break;
    case esscimqi_socketSigAlarmRequired:
    // not supported anymore anyway
    default:
        epicsSocketDestroy(_channel);
    }


    // wait for send thread to exit cleanly
    if (_thread.get() && waitForThreadToComplete)
    {
        if (!_shutdownEvent.wait(5.0))
        {
            LOG(logLevelError,
                "Receive thread for UDP socket %s has not exited.",
                inetAddressToString(_bindAddress).c_str());
        }
    }
}

void BlockingUDPTransport::enqueueSendRequest(TransportSender::shared_pointer const & sender) {
    Lock lock(_sendMutex);

    _sendToEnabled = false;
    _sendBuffer->clear();
    sender->lock();
    try {
        sender->send(_sendBuffer.get(), this);
        sender->unlock();
        endMessage();
        if(!_sendToEnabled)
            send(_sendBuffer.get());
        else
            send(_sendBuffer.get(), _sendTo);
    } catch(...) {
        sender->unlock();
    }
}


void BlockingUDPTransport::flushSendQueue()
{
    // noop (note different sent addresses are possible)
}

void BlockingUDPTransport::startMessage(int8 command, size_t /*ensureCapacity*/, int32 payloadSize) {
    _lastMessageStartPosition = _sendBuffer->getPosition();
    _sendBuffer->putByte(PVA_MAGIC);
    _sendBuffer->putByte(PVA_VERSION);
    _sendBuffer->putByte(_clientServerWithEndianFlag);
    _sendBuffer->putByte(command); // command
    _sendBuffer->putInt(payloadSize);
}

void BlockingUDPTransport::endMessage() {
    //we always (for now) send by packet, so no need for this here...
    //alignBuffer(PVA_ALIGNMENT);
    _sendBuffer->putInt(
        _lastMessageStartPosition+(sizeof(int16)+2),
        _sendBuffer->getPosition()-_lastMessageStartPosition-PVA_MESSAGE_HEADER_SIZE);
}

void BlockingUDPTransport::run() {
    // This function is always called from only one thread - this
    // object's own thread.

    osiSockAddr fromAddress;
    osiSocklen_t addrStructSize = sizeof(sockaddr);
    Transport::shared_pointer thisTransport = shared_from_this();

    try {

        char* recvfrom_buffer_start = (char*)(_receiveBuffer->getArray()+RECEIVE_BUFFER_PRE_RESERVE);
        size_t recvfrom_buffer_len =_receiveBuffer->getSize()-RECEIVE_BUFFER_PRE_RESERVE;
        while(!_closed.get())
        {
            int bytesRead = recvfrom(_channel,
                                     recvfrom_buffer_start, recvfrom_buffer_len,
                                     0, (sockaddr*)&fromAddress,
                                     &addrStructSize);

            if(likely(bytesRead>0)) {
                // successfully got datagram
                bool ignore = false;
                if(likely(_ignoredAddresses!=0))
                {
                    for(size_t i = 0; i <_ignoredAddresses->size(); i++)
                    {
                        if((*_ignoredAddresses)[i].ia.sin_addr.s_addr==fromAddress.ia.sin_addr.s_addr)
                        {
                            ignore = true;
                            break;
                        }
                    }
                }

                if(likely(!ignore)) {
                    _receiveBuffer->setPosition(RECEIVE_BUFFER_PRE_RESERVE);
                    _receiveBuffer->setLimit(RECEIVE_BUFFER_PRE_RESERVE+bytesRead);

                    try {
                        processBuffer(thisTransport, fromAddress, _receiveBuffer.get());
                    } catch(std::exception& e) {
                        LOG(logLevelError,
                            "an exception caught while in UDP receiveThread at %s:%d: %s",
                            __FILE__, __LINE__, e.what());
                    } catch (...) {
                        LOG(logLevelError,
                            "unknown exception caught while in UDP receiveThread at %s:%d.",
                            __FILE__, __LINE__);
                    }
                }
            }
            else if (unlikely(bytesRead == -1)) {

                int socketError = SOCKERRNO;

                // interrupted or timeout
                if (socketError == SOCK_EINTR ||
                        socketError == EAGAIN ||        // no alias in libCom
                        // windows times out with this
                        socketError == SOCK_ETIMEDOUT ||
                        socketError == SOCK_EWOULDBLOCK)
                    continue;

                if (socketError == SOCK_ECONNREFUSED || // avoid spurious ECONNREFUSED in Linux
                        socketError == SOCK_ECONNRESET)     // or ECONNRESET in Windows
                    continue;

                // log a 'recvfrom' error
                if(!_closed.get())
                {
                    char errStr[64];
                    epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
                    LOG(logLevelError, "Socket recvfrom error: %s.", errStr);
                }

                close(false);
                break;
            }

        }
    } catch(...) {
        // TODO: catch all exceptions, and act accordingly
        close(false);
    }

    if (IS_LOGGABLE(logLevelTrace))
    {
        string threadName = "UDP-rx "+inetAddressToString(_bindAddress);
        LOG(logLevelTrace, "Thread '%s' exiting.", threadName.c_str());
    }

    _shutdownEvent.signal();
}

bool BlockingUDPTransport::processBuffer(Transport::shared_pointer const & transport,
        osiSockAddr& fromAddress, ByteBuffer* receiveBuffer) {


    // handle response(s)
    while(likely((int)receiveBuffer->getRemaining()>=PVA_MESSAGE_HEADER_SIZE)) {

        //
        // read header
        //

        // first byte is PVA_MAGIC
        int8 magic = receiveBuffer->getByte();
        if(unlikely(magic != PVA_MAGIC))
            return false;

        // second byte version
        int8 version = receiveBuffer->getByte();

        int8 flags = receiveBuffer->getByte();
        if (flags & 0x80)
        {
            // 7th bit set
            receiveBuffer->setEndianess(EPICS_ENDIAN_BIG);
        }
        else
        {
            receiveBuffer->setEndianess(EPICS_ENDIAN_LITTLE);
        }

        // command ID and paylaod
        int8 command = receiveBuffer->getByte();
        // TODO check this cast (size_t must be 32-bit)
        size_t payloadSize = receiveBuffer->getInt();

        // control message check (skip message)
        if (flags & 0x01)
            continue;

        size_t nextRequestPosition = receiveBuffer->getPosition() + payloadSize;

        // payload size check
        if(unlikely(nextRequestPosition>receiveBuffer->getLimit())) return false;

        // CMD_ORIGIN_TAG filtering
        // NOTE: from design point of view this is not a right place to process application message here
        if (unlikely(command == CMD_ORIGIN_TAG))
        {
            // enabled?
            if (_tappedNIF)
            {
                // 128-bit IPv6 address
                osiSockAddr originNIFAddress;
                if (decodeAsIPv6Address(receiveBuffer, &originNIFAddress))
                {
                    originNIFAddress.ia.sin_family = AF_INET;

                    /*
                    LOG(logLevelDebug, "Got CMD_ORIGIN_TAG message form %s tagged as %s.",
                        inetAddressToString(fromAddress, true).c_str(),
                        inetAddressToString(originNIFAddress, false).c_str());
                    */

                    // filter
                    if (originNIFAddress.ia.sin_addr.s_addr != htonl(INADDR_ANY))
                    {
                        bool accept = false;
                        for(size_t i = 0; i < _tappedNIF->size(); i++)
                        {
                            if((*_tappedNIF)[i].ia.sin_addr.s_addr == originNIFAddress.ia.sin_addr.s_addr)
                            {
                                accept = true;
                                break;
                            }
                        }

                        // ignore messages from non-tapped NIFs
                        if (!accept)
                            return false;
                    }
                }
            }
        }
        else
        {
            // handle
            _responseHandler->handleResponse(&fromAddress, transport,
                                             version, command, payloadSize,
                                             _receiveBuffer.get());
        }

        // set position (e.g. in case handler did not read all)
        receiveBuffer->setPosition(nextRequestPosition);
    }

    // all ok
    return true;
}

bool BlockingUDPTransport::send(const char* buffer, size_t length, const osiSockAddr& address)
{
    if (IS_LOGGABLE(logLevelDebug))
    {
        LOG(logLevelDebug, "Sending %d bytes to %s.",
            length, inetAddressToString(address).c_str());
    }

    int retval = sendto(_channel, buffer,
                        length, 0, &(address.sa), sizeof(sockaddr));
    if(unlikely(retval<0))
    {
        char errStr[64];
        epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
        LOG(logLevelDebug, "Socket sendto to %s error: %s.",
            inetAddressToString(address).c_str(), errStr);
        return false;
    }

    return true;
}

bool BlockingUDPTransport::send(ByteBuffer* buffer, const osiSockAddr& address) {

    buffer->flip();

    if (IS_LOGGABLE(logLevelDebug))
    {
        LOG(logLevelDebug, "Sending %d bytes to %s.",
            buffer->getRemaining(), inetAddressToString(address).c_str());
    }

    int retval = sendto(_channel, buffer->getArray(),
                        buffer->getLimit(), 0, &(address.sa), sizeof(sockaddr));
    if(unlikely(retval<0))
    {
        char errStr[64];
        epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
        LOG(logLevelDebug, "Socket sendto to %s error: %s.",
            inetAddressToString(address).c_str(), errStr);
        return false;
    }

    // all sent
    buffer->setPosition(buffer->getLimit());

    return true;
}

bool BlockingUDPTransport::send(ByteBuffer* buffer, InetAddressType target) {
    if(!_sendAddresses) return false;

    buffer->flip();

    bool allOK = true;
    for(size_t i = 0; i<_sendAddresses->size(); i++) {

        // filter
        if (target != inetAddressType_all)
            if ((target == inetAddressType_unicast && !_isSendAddressUnicast[i]) ||
                    (target == inetAddressType_broadcast_multicast && _isSendAddressUnicast[i]))
                continue;

        if (IS_LOGGABLE(logLevelDebug))
        {
            LOG(logLevelDebug, "Sending %d bytes to %s.",
                buffer->getRemaining(), inetAddressToString((*_sendAddresses)[i]).c_str());
        }

        int retval = sendto(_channel, buffer->getArray(),
                            buffer->getLimit(), 0, &((*_sendAddresses)[i].sa),
                            sizeof(sockaddr));
        if(unlikely(retval<0))
        {
            char errStr[64];
            epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
            LOG(logLevelDebug, "Socket sendto to %s error: %s.",
                inetAddressToString((*_sendAddresses)[i]).c_str(), errStr);
            allOK = false;
        }
    }

    // all sent
    buffer->setPosition(buffer->getLimit());

    return allOK;
}

size_t BlockingUDPTransport::getSocketReceiveBufferSize() const {
    // Get value of the SO_RCVBUF option for this DatagramSocket,
    // that is the buffer size used by the platform for input on
    // this DatagramSocket.

    int sockBufSize = -1;
    osiSocklen_t intLen = sizeof(int);

    int retval = getsockopt(_channel, SOL_SOCKET, SO_RCVBUF, (char *)&sockBufSize, &intLen);
    if(unlikely(retval<0))
    {
        char errStr[64];
        epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
        LOG(logLevelError, "Socket getsockopt SO_RCVBUF error: %s.", errStr);
    }

    return (size_t)sockBufSize;
}


void BlockingUDPTransport::join(const osiSockAddr & mcastAddr, const osiSockAddr & nifAddr)
{
    struct ip_mreq imreq;
    memset(&imreq, 0, sizeof(struct ip_mreq));

    imreq.imr_multiaddr.s_addr = mcastAddr.ia.sin_addr.s_addr;
    imreq.imr_interface.s_addr = nifAddr.ia.sin_addr.s_addr;

    // join multicast group on the given interface
    int status = ::setsockopt(_channel, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                              (char*)&imreq, sizeof(struct ip_mreq));
    if (status)
    {
        char errStr[64];
        epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
        throw std::runtime_error(
            string("Failed to join to the multicast group '") +
            inetAddressToString(mcastAddr) + "' on network interface '" +
            inetAddressToString(nifAddr, false) + "': " + errStr);
    }
}

void BlockingUDPTransport::setMutlicastNIF(const osiSockAddr & nifAddr, bool loopback)
{
    // set the multicast outgoing interface
    int status = ::setsockopt(_channel, IPPROTO_IP, IP_MULTICAST_IF,
                              (char*)&nifAddr.ia.sin_addr, sizeof(struct in_addr));
    if (status)
    {
        char errStr[64];
        epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
        throw std::runtime_error(
            string("Failed to set multicast network interface '") +
            inetAddressToString(nifAddr, false) + "': " + errStr);
    }

    // send multicast traffic to myself too
    unsigned char mcast_loop = (loopback ? 1 : 0);
    status = ::setsockopt(_channel, IPPROTO_IP, IP_MULTICAST_LOOP,
                          (char*)&mcast_loop, sizeof(unsigned char));
    if (status)
    {
        char errStr[64];
        epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
        throw std::runtime_error(
            string("Failed to enable multicast loopback on network interface '") +
            inetAddressToString(nifAddr, false) + "': " + errStr);
    }

}

void initializeUDPTransports(bool serverFlag,
                             BlockingUDPTransportVector& udpTransports,
                             const IfaceNodeVector& ifaceList,
                             const ResponseHandler::shared_pointer& responseHandler,
                             BlockingUDPTransport::shared_pointer& sendTransport,
                             int32& listenPort,
                             bool autoAddressList,
                             const std::string& addressList,
                             const std::string& ignoreAddressList)
{
    TransportClient::shared_pointer nullTransportClient;
    auto_ptr<BlockingUDPConnector> connector(new BlockingUDPConnector(serverFlag, true, true));

    //
    // Create UDP transport for sending (to all network interfaces)
    //

    osiSockAddr anyAddress;
    anyAddress.ia.sin_family = AF_INET;
    anyAddress.ia.sin_port = htons(0);
    anyAddress.ia.sin_addr.s_addr = htonl(INADDR_ANY);

    sendTransport = static_pointer_cast<BlockingUDPTransport>(connector->connect(
                        nullTransportClient, responseHandler,
                        anyAddress, PVA_PROTOCOL_REVISION,
                        PVA_DEFAULT_PRIORITY));
    if (!sendTransport)
    {
        THROW_BASE_EXCEPTION("Failed to initialize UDP transport.");
    }

    // to allow automatic assignment of listen port (for testing)
    if (listenPort == 0)
    {
        listenPort = ntohs(sendTransport->getRemoteAddress()->ia.sin_port);
        LOG(logLevelDebug, "Dynamic listen UDP port set to %d.", listenPort);
    }

    // TODO current implementation shares the port (aka beacon and search port)
    int32 sendPort = listenPort;

    //
    // compile auto address list - where to send packets
    //

    InetAddrVector autoBCastAddr;
    for (IfaceNodeVector::const_iterator iter = ifaceList.begin(); iter != ifaceList.end(); iter++)
    {
        ifaceNode node = *iter;

        if (node.ifaceBCast.ia.sin_family != AF_UNSPEC)
        {
            node.ifaceBCast.ia.sin_port = htons(sendPort);
            autoBCastAddr.push_back(node.ifaceBCast);
        }
    }

    //
    // set send address list
    //

    if (!addressList.empty())
    {
        // if auto is true, add it to specified list
        if (!autoAddressList)
            autoBCastAddr.clear();

        auto_ptr<InetAddrVector> list(getSocketAddressList(addressList, sendPort, &autoBCastAddr));
        if (list.get() && list->size())
        {
            sendTransport->setSendAddresses(list.get());
        }
        /*
        else
        {
            // fallback
            // set default (auto) address list
            sendTransport->setSendAddresses(&autoBCastAddr);
        }
        */
    }
    else if (autoAddressList)
    {
        // set default (auto) address list
        sendTransport->setSendAddresses(&autoBCastAddr);
    }


    sendTransport->start();
    udpTransports.push_back(sendTransport);

    // debug output of broadcast addresses
    InetAddrVector* blist = sendTransport->getSendAddresses();
    if (!blist || !blist->size())
        LOG(logLevelError,
            "No broadcast addresses found or specified - empty address list!");
    else
        for (size_t i = 0; i < blist->size(); i++)
            LOG(logLevelDebug,
                "Broadcast address #%d: %s.", i, inetAddressToString((*blist)[i]).c_str());


    // TODO configurable local NIF, address
    osiSockAddr loAddr;
    getLoopbackNIF(loAddr, "", 0);

    // TODO configurable local multicast address
    std::string mcastAddress("224.0.0.128");

    osiSockAddr group;
    aToIPAddr(mcastAddress.c_str(), listenPort, &group.ia);

    //
    // set ignore address list
    //
    auto_ptr<InetAddrVector> ignoreAddressVector;
    if (!ignoreAddressList.empty())
        ignoreAddressVector.reset(getSocketAddressList(ignoreAddressList, 0, 0));

    //
    // Setup UDP trasport(s) (per interface)
    //

    InetAddrVector tappedNIF;

    for (IfaceNodeVector::const_iterator iter = ifaceList.begin(); iter != ifaceList.end(); iter++)
    {
        ifaceNode node = *iter;

        LOG(logLevelDebug, "Setting up UDP for interface %s, broadcast %s.",
            inetAddressToString(node.ifaceAddr, false).c_str(),
            inetAddressToString(node.ifaceBCast, false).c_str());
        try
        {
            // where to bind (listen) address
            osiSockAddr listenLocalAddress;
            listenLocalAddress.ia.sin_family = AF_INET;
            listenLocalAddress.ia.sin_port = htons(listenPort);
            listenLocalAddress.ia.sin_addr.s_addr = node.ifaceAddr.ia.sin_addr.s_addr;

            BlockingUDPTransport::shared_pointer transport = static_pointer_cast<BlockingUDPTransport>(connector->connect(
                        nullTransportClient, responseHandler,
                        listenLocalAddress, PVA_PROTOCOL_REVISION,
                        PVA_DEFAULT_PRIORITY));
            if (!transport)
                continue;
            listenLocalAddress = *transport->getRemoteAddress();

            if (ignoreAddressVector.get() && ignoreAddressVector->size())
                transport->setIgnoredAddresses(ignoreAddressVector.get());

            tappedNIF.push_back(listenLocalAddress);


            BlockingUDPTransport::shared_pointer transport2;

            if(node.ifaceBCast.ia.sin_family == AF_UNSPEC ||
                    node.ifaceBCast.ia.sin_addr.s_addr == listenLocalAddress.ia.sin_addr.s_addr) {
                LOG(logLevelWarn, "Unable to find broadcast address of interface %s.", inetAddressToString(node.ifaceAddr, false).c_str());
            }
#if !defined(_WIN32)
            else
            {
                /* An oddness of BSD sockets (not winsock) is that binding to
                 * INADDR_ANY will receive unicast and broadcast, but binding to
                 * a specific interface address receives only unicast.  The trick
                 * is to bind a second socket to the interface broadcast address,
                 * which will then receive only broadcasts.
                 */

                osiSockAddr bcastAddress;
                bcastAddress.ia.sin_family = AF_INET;
                bcastAddress.ia.sin_port = htons(listenPort);
                bcastAddress.ia.sin_addr.s_addr = node.ifaceBCast.ia.sin_addr.s_addr;

                transport2 = static_pointer_cast<BlockingUDPTransport>(connector->connect(
                                 nullTransportClient, responseHandler,
                                 bcastAddress, PVA_PROTOCOL_REVISION,
                                 PVA_DEFAULT_PRIORITY));
                if (transport2)
                {
                    /* The other wrinkle is that nothing should be sent from this second
                     * socket. So replies are made through the unicast socket.
                     *
                    transport2->setReplyTransport(transport);
                    */
                    // NOTE: search responses all always send from sendTransport

                    if (ignoreAddressVector.get() && ignoreAddressVector->size())
                        transport2->setIgnoredAddresses(ignoreAddressVector.get());

                    tappedNIF.push_back(bcastAddress);
                }
            }
#endif

            transport->setMutlicastNIF(loAddr, true);
            transport->setLocalMulticastAddress(group);

            transport->start();
            udpTransports.push_back(transport);

            if (transport2)
            {
                transport2->start();
                udpTransports.push_back(transport2);
            }
        }
        catch (std::exception& e)
        {
            THROW_BASE_EXCEPTION_CAUSE("Failed to initialize UDP transport.", e);
        }
        catch (...)
        {
            THROW_BASE_EXCEPTION("Failed to initialize UDP transport.");
        }
    }


    //
    // Setup local multicasting
    //

    // WIN32 do not allow binding to multicast address, use any address w/ port
#if defined(_WIN32)
    anyAddress.ia.sin_port = htons(listenPort);
#endif

    BlockingUDPTransport::shared_pointer localMulticastTransport;
    try
    {
        // NOTE: multicast receiver socket must be "bound" to INADDR_ANY or multicast address
        localMulticastTransport = static_pointer_cast<BlockingUDPTransport>(connector->connect(
                                      nullTransportClient, responseHandler,
#if !defined(_WIN32)
                                      group,
#else
                                      anyAddress,
#endif
                                      PVA_PROTOCOL_REVISION,
                                      PVA_DEFAULT_PRIORITY));
        if (!localMulticastTransport)
            throw std::runtime_error("Failed to bind UDP socket.");

        localMulticastTransport->setTappedNIF(&tappedNIF);
        localMulticastTransport->join(group, loAddr);
        localMulticastTransport->start();
        udpTransports.push_back(localMulticastTransport);

        LOG(logLevelDebug, "Local multicast enabled on %s/%s.",
            inetAddressToString(loAddr, false).c_str(),
            inetAddressToString(group).c_str());
    }
    catch (std::exception& ex)
    {
        LOG(logLevelDebug, "Failed to initialize local multicast, functionality disabled. Reason: %s.", ex.what());
    }
}


}
}
