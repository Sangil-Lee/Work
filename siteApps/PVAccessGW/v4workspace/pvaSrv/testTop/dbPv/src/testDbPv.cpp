/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

/* Author: Marty Kraimer */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <memory>

#include <cantProceed.h>
#include <epicsStdio.h>
#include <epicsMutex.h>
#include <epicsEvent.h>
#include <epicsThread.h>
#include <iocsh.h>


#include <pv/pvIntrospect.h>
#include <pv/pvData.h>
#include <pv/pvAccess.h>

#define epicsExportSharedSymbols
#include <epicsExport.h>
#include "dbPv.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvaSrv;
using std::string;

class FindRequester : public ChannelFindRequester {
public:
    FindRequester(){}
    virtual ~FindRequester(){}
    virtual void channelFindResult(
        const Status &status,
        ChannelFind::shared_pointer const &channelFind,
        bool wasFound);
};

void FindRequester::channelFindResult(
    const Status &status,
    ChannelFind::shared_pointer const &channelFind,
    bool wasFound)
{
    string message = status.getMessage();
    printf("channelFindResult status %s wasFound %s\n",
        message.c_str(),
        (wasFound ? "true" : "false"));
}

class MyRequester :
    public virtual Requester,
    public ChannelRequester,
    public ChannelGetRequester
{
public:
    POINTER_DEFINITIONS(MyRequester);
    MyRequester()
    : name("testDbPv"),
      channelPtr(Channel::shared_pointer()),
      channelGetPtr(ChannelGet::shared_pointer()),
      pvStructurePtr(PVStructure::shared_pointer()),
      bitSetPtr(BitSet::shared_pointer())
    { }
    Channel::shared_pointer const & getChannel() {return channelPtr;}
    virtual ~MyRequester() { }
    virtual string getRequesterName()
    {
        return name;
    }
    virtual void message(string const &message,MessageType messageType)
    {
        string typeName = getMessageTypeName(messageType);
        printf("ChannelRequester message %s messageType %s\n",
            message.c_str(),typeName.c_str());
    }
    virtual void channelCreated(
        const Status &status,
        Channel::shared_pointer const &channel)
    {
        channelPtr = channel;
        string message = status.getMessage();
        bool isOK = status.isOK();
        printf("channelCreated status %s statusOK %s\n",
        message.c_str(),
        (isOK ? "true" : "false"));
    }
    virtual void channelStateChange(
        Channel::shared_pointer const & c,
        Channel::ConnectionState connectionState)
    {
        string state = Channel::ConnectionStateNames[connectionState];
        printf("channelStateChange %s\n",state.c_str());
    }
    virtual void channelGetConnect(
        const Status &status,
        ChannelGet::shared_pointer const &channelGet,
        StructureConstPtr const &structure)
    {
        channelGetPtr = channelGet;
        printf("channelGetConnect statusOK %s\n",
            (status.isOK() ? "true" : "false"));
    }
    virtual void getDone(
        const Status &status,
        ChannelGet::shared_pointer const & channelGet,
        PVStructurePtr const &pvStructure,
        BitSetPtr const & bitSet)
    {
        pvStructurePtr = pvStructure;
        bitSetPtr = bitSet;
        printf("getDone statusOK %s\n",
            (status.isOK() ? "true" : "false"));
        std::cout << *pvStructure << std::endl;
    }
private:
    string name;
    Channel::shared_pointer channelPtr;
    ChannelGet::shared_pointer channelGetPtr;
    PVStructure::shared_pointer pvStructurePtr;
    BitSet::shared_pointer bitSetPtr;
};

static const iocshArg testArg0 = { "pvName", iocshArgString };
static const iocshArg *testArgs[] = {
    &testArg0};

static const iocshFuncDef testDbPvFuncDef = {
    "testDbPv", 1, testArgs};
static void testDbPvCallFunc(const iocshArgBuf *args)
{
    char *pvName = args[0].sval;
    printf("testDbPv pvName %s\n",pvName);
    string channelName(pvName);
    printf("channelName %s\n",channelName.c_str());
    ChannelProvider::shared_pointer channelProvider =
        getChannelProviderRegistry()->getProvider("dbPv");
    string providerName = channelProvider->getProviderName();
    printf("providerName %s\n",providerName.c_str());
    FindRequester::shared_pointer findRequester
        = FindRequester::shared_pointer(new FindRequester());
    printf("findRequester %p %p\n",&findRequester,findRequester.get());
    channelProvider->channelFind(
        channelName,
        findRequester);
    MyRequester::shared_pointer  myRequester
         = MyRequester::shared_pointer(new MyRequester());
    Channel::shared_pointer channel = channelProvider->createChannel(
         channelName,
         myRequester,
         0,
         "");
    
    CreateRequest::shared_pointer createRequest = CreateRequest::create();
    PVStructure::shared_pointer pvRequest = createRequest->createRequest(
        "record[process=true]field(value,timeStamp,alarm)");
    ChannelGet::shared_pointer channelGet = channel->createChannelGet(
        myRequester,pvRequest);
    channelGet->get();
    channelGet->destroy();
    if(channel) {
        channel->destroy();
    }
}

static void testDbPvRegister(void)
{
    static int firstTime = 1;
    if (firstTime) {
        firstTime = 0;
        iocshRegister(&testDbPvFuncDef, testDbPvCallFunc);
    }
}

extern "C" {
    epicsExportRegistrar(testDbPvRegister);
}
