#include <epicsUnitTest.h>
#include <testMain.h>

#include <pv/inetAddressUtil.h>
#include <pv/logger.h>

#include <pv/byteBuffer.h>
#include <pv/pvType.h>

#include <epicsAssert.h>
#include <osiSock.h>

#include <iostream>
#include <cstring>

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace std;

void test_getSocketAddressList()
{
    testDiag("Test getSocketAddressList()");

    auto_ptr<InetAddrVector> vec(getSocketAddressList("127.0.0.1   10.10.12.11:1234 192.168.3.4", 555));

    testOk1(static_cast<size_t>(3) == vec->size());

    osiSockAddr addr;
    addr = vec->at(0);
    testOk1(AF_INET == addr.ia.sin_family);
    testOk1(htons(555) == addr.ia.sin_port);
    testOk1(htonl(0x7F000001) == addr.ia.sin_addr.s_addr);
    testOk1("127.0.0.1:555" == inetAddressToString(addr));

    addr = vec->at(1);
    testOk1(AF_INET == addr.ia.sin_family);
    testOk1(htons(1234) == addr.ia.sin_port);
    testOk1(htonl(0x0A0A0C0B) == addr.ia.sin_addr.s_addr);
    testOk1("10.10.12.11:1234" == inetAddressToString(addr));

    addr = vec->at(2);
    testOk1(AF_INET == addr.ia.sin_family);
    testOk1(htons(555) == addr.ia.sin_port);
    testOk1(htonl(0xC0A80304) == addr.ia.sin_addr.s_addr);
    testOk1("192.168.3.4:555" == inetAddressToString(addr));




    auto_ptr<InetAddrVector> vec1(getSocketAddressList("172.16.55.160", 6789, vec.get()));

    testOk1(static_cast<size_t>(4) == vec1->size());

    addr = vec1->at(0);
    testOk1(AF_INET == addr.ia.sin_family);
    testOk1(htons(6789) == addr.ia.sin_port);
    testOk1(htonl(0xAC1037A0) == addr.ia.sin_addr.s_addr);
    testOk1("172.16.55.160:6789" == inetAddressToString(addr));

    addr = vec1->at(1);
    testOk1(AF_INET == addr.ia.sin_family);
    testOk1(htons(555) == addr.ia.sin_port);
    testOk1(htonl(0x7F000001) == addr.ia.sin_addr.s_addr);
    testOk1("127.0.0.1:555" == inetAddressToString(addr));

    addr = vec1->at(2);
    testOk1(AF_INET == addr.ia.sin_family);
    testOk1(htons(1234) == addr.ia.sin_port);
    testOk1(htonl(0x0A0A0C0B) == addr.ia.sin_addr.s_addr);
    testOk1("10.10.12.11:1234" == inetAddressToString(addr));

    addr = vec1->at(3);
    testOk1(AF_INET == addr.ia.sin_family);
    testOk1(htons(555) == addr.ia.sin_port);
    testOk1(htonl(0xC0A80304) == addr.ia.sin_addr.s_addr);
    testOk1("192.168.3.4:555" == inetAddressToString(addr));


    // empty
    auto_ptr<InetAddrVector> vec2(getSocketAddressList("", 1111));
    testOk1(static_cast<size_t>(0) == vec2->size());

    // just spaces
    auto_ptr<InetAddrVector> vec3(getSocketAddressList("   ", 1111));
    testOk1(static_cast<size_t>(0) == vec3->size());

    // leading spaces
    auto_ptr<InetAddrVector> vec4(getSocketAddressList("     127.0.0.1   10.10.12.11:1234 192.168.3.4", 555));

    testOk1(static_cast<size_t>(3) == vec4->size());

    addr = vec4->at(0);
    testOk1(AF_INET == addr.ia.sin_family);
    testOk1(htons(555) == addr.ia.sin_port);
    testOk1(htonl(0x7F000001) == addr.ia.sin_addr.s_addr);
    testOk1("127.0.0.1:555" == inetAddressToString(addr));

    addr = vec4->at(1);
    testOk1(AF_INET == addr.ia.sin_family);
    testOk1(htons(1234) == addr.ia.sin_port);
    testOk1(htonl(0x0A0A0C0B) == addr.ia.sin_addr.s_addr);
    testOk1("10.10.12.11:1234" == inetAddressToString(addr));

    addr = vec4->at(2);
    testOk1(AF_INET == addr.ia.sin_family);
    testOk1(htons(555) == addr.ia.sin_port);
    testOk1(htonl(0xC0A80304) == addr.ia.sin_addr.s_addr);
    testOk1("192.168.3.4:555" == inetAddressToString(addr));
}

void test_ipv4AddressToInt()
{
    testDiag("Test ipv4AddressToInt()");

    auto_ptr<InetAddrVector> vec(getSocketAddressList("127.0.0.1   10.10.12.11:1234 192.168.3.4", 555));

    testOk1(static_cast<size_t>(3) == vec->size());

    testOk1((int32)0x7F000001 == ipv4AddressToInt((vec->at(0))));
    testOk1((int32)0x0A0A0C0B == ipv4AddressToInt((vec->at(1))));
    testOk1((int32)0xC0A80304 == ipv4AddressToInt((vec->at(2))));
}

void test_intToIPv4Address()
{
    testDiag("Test intToIPv4Address()");

    auto_ptr<osiSockAddr> paddr(intToIPv4Address(0x7F000001));
    testOk1((uintptr_t)0 != (uintptr_t)paddr.get());
    testOk1(AF_INET == paddr->ia.sin_family);
    testOk1("127.0.0.1:0" == inetAddressToString(*paddr.get()));

    paddr.reset(intToIPv4Address(0x0A0A0C0B));
    testOk1((uintptr_t)0 != (uintptr_t)paddr.get());
    testOk1(AF_INET == paddr->ia.sin_family);
    testOk1("10.10.12.11:0" == inetAddressToString(*paddr.get()));
}

void test_encodeAsIPv6Address()
{
    testDiag("Test encodeAsIPv6Address()");

    auto_ptr<ByteBuffer> buff(new ByteBuffer(32, EPICS_ENDIAN_LITTLE));

    char src[] = { (char)0, (char)0, (char)0, (char)0, (char)0, (char)0,
                   (char)0, (char)0, (char)0, (char)0, (char)0xFF, (char)0xFF,
                   (char)0x0A, (char)0x0A, (char)0x0C, (char)0x0B
                 };

    auto_ptr<osiSockAddr> paddr(intToIPv4Address(0x0A0A0C0B));
    testOk1((uintptr_t)0 != (uintptr_t)paddr.get());
    osiSockAddr addr = *paddr;

    encodeAsIPv6Address(buff.get(), &addr);
    testOk1(static_cast<size_t>(16) == buff->getPosition());

    testOk1(strncmp(buff->getArray(), src, 16) == 0);
}

void test_isMulticastAddress()
{
    testDiag("Test test_isMulticastAddress()");

    auto_ptr<InetAddrVector> vec(getSocketAddressList("127.0.0.1 255.255.255.255 0.0.0.0 224.0.0.0 239.255.255.255 235.3.6.3", 0));

    testOk1(static_cast<size_t>(6) == vec->size());

    testOk1(!isMulticastAddress(&vec->at(0)));
    testOk1(!isMulticastAddress(&vec->at(1)));
    testOk1(!isMulticastAddress(&vec->at(2)));
    testOk1(isMulticastAddress(&vec->at(3)));
    testOk1(isMulticastAddress(&vec->at(4)));
    testOk1(isMulticastAddress(&vec->at(5)));
}

void test_getBroadcastAddresses()
{
    testDiag("Test getBroadcastAddresses()");

    osiSockAttach();

    SOCKET socket = epicsSocketCreate(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    auto_ptr<InetAddrVector> broadcasts(getBroadcastAddresses(socket, 6678));
    // at least one is expected, in case of no network connection a fallback address is returned
    testOk1(static_cast<size_t>(0) < broadcasts->size());
    //testDiag("getBroadcastAddresses() returned %zu entry/-ies.", broadcasts->size());
    epicsSocketDestroy(socket);

    // debug
    for(size_t i = 0; i<broadcasts->size(); i++) {
        testDiag("%s", inetAddressToString(broadcasts->at(i)).c_str());
    }

}

void test_getLoopbackNIF()
{
    testDiag("Test getLoopbackNIF()");

    osiSockAddr addr;
    unsigned short port = 5555;

    int defaultValue = getLoopbackNIF(addr, "", port);

    testOk1(defaultValue);
    testOk1(AF_INET == addr.ia.sin_family);
    testOk1(htons(port) == addr.ia.sin_port);
    testOk1(htonl(INADDR_LOOPBACK) == addr.ia.sin_addr.s_addr);

    defaultValue = getLoopbackNIF(addr, "10.0.0.1:7777", port);

    testOk1(!defaultValue);
    testOk1(AF_INET == addr.ia.sin_family);
    testOk1(htons(7777) == addr.ia.sin_port);
    testOk1(htonl(0x0A000001) == addr.ia.sin_addr.s_addr);
}

#ifdef _WIN32
// needed for ip_mreq
#include <Ws2tcpip.h>
#endif

void test_multicastLoopback()
{
    testDiag("Test test_multicast()");

    osiSockAttach();

    SOCKET socket = epicsSocketCreate(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    testOk1(socket != INVALID_SOCKET);
    if (socket == INVALID_SOCKET)
        return;

    unsigned short port = 5555;

    // set SO_REUSEADDR or SO_REUSEPORT, OS dependant
    epicsSocketEnableAddressUseForDatagramFanout(socket);

    osiSockAddr bindAddr;
    bindAddr.ia.sin_family = AF_INET;
    bindAddr.ia.sin_port = ntohs(port);
    bindAddr.ia.sin_addr.s_addr = htonl(INADDR_ANY);

    int status = ::bind(socket, (sockaddr*)&(bindAddr.sa), sizeof(sockaddr));
    if (status)
    {
        char errStr[64];
        epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
        fprintf(stderr, "Failed to bind: %s\n", errStr);
        epicsSocketDestroy(socket);
        return;
    }

    osiSockAddr loAddr;
    getLoopbackNIF(loAddr, "", port);

    osiSockAddr mcastAddr;
    aToIPAddr("224.0.0.128", port, &mcastAddr.ia);

    struct ip_mreq imreq;
    memset(&imreq, 0, sizeof(struct ip_mreq));

    imreq.imr_multiaddr.s_addr = mcastAddr.ia.sin_addr.s_addr;
    imreq.imr_interface.s_addr = loAddr.ia.sin_addr.s_addr;

    // join multicast group on default interface
    status = ::setsockopt(socket, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                          (char*)&imreq, sizeof(struct ip_mreq));
    if (status)
    {
        char errStr[64];
        epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
        fprintf(stderr, "Error setting IP_ADD_MEMBERSHIP: %s\n", errStr);
    }
    testOk(status == 0, "IP_ADD_MEMBERSHIP set");



    SOCKET sendSocket = epicsSocketCreate(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    testOk1(sendSocket != INVALID_SOCKET);
    if (sendSocket == INVALID_SOCKET)
        return;

    // set the multicast outgoing interface
    status = ::setsockopt(sendSocket, IPPROTO_IP, IP_MULTICAST_IF,
                          (char*)&loAddr.ia.sin_addr, sizeof(struct in_addr));
    if (status)
    {
        char errStr[64];
        epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
        fprintf(stderr, "Error setting IP_MULTICAST_IF: %s\n", errStr);
    }
    testOk(status == 0, "IP_MULTICAST_IF set");

    // send multicast traffic to myself too
    unsigned char mcast_loop = 1;
    status = ::setsockopt(sendSocket, IPPROTO_IP, IP_MULTICAST_LOOP,
                          (char*)&mcast_loop, sizeof(unsigned char));
    if (status)
    {
        char errStr[64];
        epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
        fprintf(stderr, "Error setting IP_MULTICAST_LOOP: %s\n", errStr);
    }
    testOk(status == 0, "IP_MULTICAST_LOOP set");

    // put some data in buffer
#define MAX_BUFFER_SIZE 1024
    char txbuff[MAX_BUFFER_SIZE];
    strcpy(txbuff, "mcastTest");

    // send multicast packet
    size_t len = strlen(txbuff);
    status = ::sendto(sendSocket, txbuff, len, 0,
                      &(mcastAddr.sa), sizeof(sockaddr));
    if (status < 0)
    {
        char errStr[64];
        epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
        fprintf(stderr, "Multicast send error: %s\n", errStr);
    }
    testOk((size_t)status == len, "Multicast send");


    // set timeout in case message is not sent
    struct timeval timeout;
    memset(&timeout, 0, sizeof(struct timeval));
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;

    status = ::setsockopt (socket, SOL_SOCKET, SO_RCVTIMEO,
                           (char*)&timeout, sizeof(timeout));
    if (status)
    {
        char errStr[64];
        epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
        fprintf(stderr, "Error setting SO_RCVTIMEO: %s\n", errStr);
    }
    testOk(status == 0, "SO_RCVTIMEO set");

    char rxbuff[MAX_BUFFER_SIZE];

    osiSockAddr fromAddress;
    osiSocklen_t addrStructSize = sizeof(sockaddr);

    // receive packet from socket
    status = ::recvfrom(socket, rxbuff, MAX_BUFFER_SIZE, 0,
                        (sockaddr*)&fromAddress, &addrStructSize);
    if (status < 0)
    {
        char errStr[64];
        epicsSocketConvertErrnoToString(errStr, sizeof(errStr));
        fprintf(stderr, "Multicast recv error: %s\n", errStr);
    }
    testOk((size_t)status == len, "Multicast recv");
    testOk(strncmp(rxbuff, txbuff, len) == 0, "Multicast content matches");

    // shutdown sockets?
    epicsSocketDestroy(sendSocket);
    epicsSocketDestroy(socket);
}

MAIN(testInetAddressUtils)
{
    testPlan(83);
    testDiag("Tests for InetAddress utils");

    test_getSocketAddressList();
    test_ipv4AddressToInt();
    test_intToIPv4Address();
    test_encodeAsIPv6Address();
    test_isMulticastAddress();
    test_getBroadcastAddresses();
    test_getLoopbackNIF();

    test_multicastLoopback();

    return testDone();
}

