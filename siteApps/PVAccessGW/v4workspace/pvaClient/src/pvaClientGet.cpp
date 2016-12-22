/* pvaClientGet.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2015.02
 */

#include <pv/event.h>

#define epicsExportSharedSymbols

#include <pv/pvaClient.h>

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace std;

namespace epics { namespace pvaClient {


class ChannelGetRequesterImpl : public ChannelGetRequester
{
    PvaClientGet * pvaClientGet;
public:
    ChannelGetRequesterImpl(PvaClientGet * pvaClientGet)
    : pvaClientGet(pvaClientGet) {}
    string getRequesterName()
    {return pvaClientGet->getRequesterName();}
    void message(string const & message,MessageType messageType)
    {pvaClientGet->message(message,messageType);}
    void channelGetConnect(
        const Status& status,
        ChannelGet::shared_pointer const & channelGet,
        StructureConstPtr const & structure)
    {pvaClientGet->channelGetConnect(status,channelGet,structure);}
    void getDone(
        const Status& status,
        ChannelGet::shared_pointer const & channelGet,
        PVStructurePtr const & pvStructure,
        BitSetPtr const & bitSet)
    {pvaClientGet->getDone(status,channelGet,pvStructure,bitSet);}
};

PvaClientGet::PvaClientGet(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
: pvaClient(pvaClient),
  channel(channel),
  pvRequest(pvRequest),
  isDestroyed(false),
  connectState(connectIdle),
  getState(getIdle)
{
}

PvaClientGet::~PvaClientGet()
{
    destroy();
}

void PvaClientGet::checkGetState()
{
    if(isDestroyed) throw std::runtime_error("pvaClientGet was destroyed");
    if(connectState==connectIdle) connect();
    if(getState==getIdle) get();
}

// from ChannelGetRequester
string PvaClientGet::getRequesterName()
{
     PvaClientPtr yyy = pvaClient.lock();
     if(!yyy) throw std::runtime_error("pvaClient was destroyed");
     return yyy->getRequesterName();
}

void PvaClientGet::message(string const & message,MessageType messageType)
{
    if(isDestroyed) throw std::runtime_error("pvaClientGet was destroyed");
    PvaClientPtr yyy = pvaClient.lock();
    if(!yyy) throw std::runtime_error("pvaClient was destroyed");
    yyy->message(message, messageType);
}

void PvaClientGet::channelGetConnect(
    const Status& status,
    ChannelGet::shared_pointer const & channelGet,
    StructureConstPtr const & structure)
{
    if(isDestroyed) throw std::runtime_error("pvaClientGet was destroyed");
    channelGetConnectStatus = status;
    this->channelGet = channelGet;
    if(status.isOK()) {
        pvaClientData = PvaClientGetData::create(structure);
        pvaClientData->setMessagePrefix(channel->getChannelName());
        connectState = connected;
    }
    waitForConnect.signal();
    
}

void PvaClientGet::getDone(
    const Status& status,
    ChannelGet::shared_pointer const & channelGet,
    PVStructurePtr const & pvStructure,
    BitSetPtr const & bitSet)
{
    if(isDestroyed) throw std::runtime_error("pvaClientGet was destroyed");
    channelGetStatus = status;
    if(status.isOK()) {
        pvaClientData->setData(pvStructure,bitSet);
    }
    waitForGet.signal();
}


// from PvaClientGet
void PvaClientGet::destroy()
{
    {
        Lock xx(mutex);
        if(isDestroyed) return;
        isDestroyed = true;
    }
    if(channelGet) channelGet->destroy();
    channelGet.reset();
}

void PvaClientGet::connect()
{
    if(isDestroyed) throw std::runtime_error("pvaClientGet was destroyed");
    issueConnect();
    Status status = waitConnect();
    if(status.isOK()) return;
    string message = string("channel ") + channel->getChannelName() 
         + " PvaClientGet::connect " + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientGet::issueConnect()
{
    if(isDestroyed) throw std::runtime_error("pvaClientGet was destroyed");
    if(connectState!=connectIdle) {
        string message = string("channel ") + channel->getChannelName() 
            + " pvaClientGet already connected ";
        throw std::runtime_error(message);
    }
    getRequester = ChannelGetRequester::shared_pointer(new ChannelGetRequesterImpl(this));
    connectState = connectActive;
    channelGet = channel->createChannelGet(getRequester,pvRequest);
}

Status PvaClientGet::waitConnect()
{
    if(isDestroyed) throw std::runtime_error("pvaClientGet was destroyed");
    if(connectState==connected) return channelGetConnectStatus;
    if(connectState!=connectActive) {
        string message = string("channel ") + channel->getChannelName() 
            + " pvaClientGet illegal connect state ";
        throw std::runtime_error(message);
    }
    waitForConnect.wait();
    connectState = channelGetConnectStatus.isOK() ? connected : connectIdle;
    return channelGetConnectStatus;
}

void PvaClientGet::get()
{
    if(isDestroyed) throw std::runtime_error("pvaClientGet was destroyed");
    issueGet();
    Status status = waitGet();
    if(status.isOK()) return;
    string message = string("channel ") + channel->getChannelName() 
            + " PvaClientGet::get " + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientGet::issueGet()
{
    if(isDestroyed) throw std::runtime_error("pvaClientGet was destroyed");
    if(connectState==connectIdle) connect();
    if(getState!=getIdle) {
        string message = string("channel ") + channel->getChannelName() 
            + " PvaClientGet::issueGet get aleady active ";
        throw std::runtime_error(message);
    }
    getState = getActive;
    channelGet->get();
}

Status PvaClientGet::waitGet()
{
    if(isDestroyed) throw std::runtime_error("pvaClientGet was destroyed");
    if(getState!=getActive){
        string message = string("channel ") + channel->getChannelName() 
            + " PvaClientGet::waitGet llegal get state";
        throw std::runtime_error(message);
    }
    waitForGet.wait();
    getState = getIdle;
    if(channelGetStatus.isOK()) {
        return Status::Ok;
    }
    return channelGetStatus;
}
PvaClientGetDataPtr PvaClientGet::getData()
{
    checkGetState();
    return pvaClientData;
}

PvaClientGetPtr PvaClientGet::create(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
{
    PvaClientGetPtr epv(new PvaClientGet(pvaClient,channel,pvRequest));
    return epv;
}

}}
