/* pvaClientPutGet.cpp */
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

class ChannelPutGetRequesterImpl : public ChannelPutGetRequester
{
    PvaClientPutGet * pvaClientPutGet;
public:
    ChannelPutGetRequesterImpl(PvaClientPutGet * pvaClientPutGet)
    : pvaClientPutGet(pvaClientPutGet) {}
    string getRequesterName()
    {return pvaClientPutGet->getRequesterName();}
    void message(string const & message,MessageType messageType)
    {pvaClientPutGet->message(message,messageType);}
    void channelPutGetConnect(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelPutGet::shared_pointer const & channelPutGet,
        epics::pvData::StructureConstPtr const & putStructure,
        epics::pvData::StructureConstPtr const & getStructure)
    {
         pvaClientPutGet->channelPutGetConnect(status,channelPutGet,putStructure,getStructure);
    }
    void putGetDone(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelPutGet::shared_pointer const & channelPutGet,
        epics::pvData::PVStructurePtr const & getPVStructure,
        epics::pvData::BitSetPtr const & getChangedBitSet)
    {
        pvaClientPutGet->putGetDone(status,channelPutGet,getPVStructure,getChangedBitSet);
    }
    void getPutDone(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelPutGet::shared_pointer const & channelPutGet,
        epics::pvData::PVStructurePtr const & putPVStructure,
        epics::pvData::BitSet::shared_pointer const & putBitSet)
    {
        pvaClientPutGet->getPutDone(status,channelPutGet,putPVStructure,putBitSet);
    }
    void getGetDone(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelPutGet::shared_pointer const & channelPutGet,
        epics::pvData::PVStructurePtr const & getPVStructure,
        epics::pvData::BitSet::shared_pointer const & getChangedBitSet)
    {
        pvaClientPutGet->getGetDone(status,channelPutGet,getPVStructure,getChangedBitSet);
    }
};

PvaClientPutGet::PvaClientPutGet(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
: pvaClient(pvaClient),
  channel(channel),
  pvRequest(pvRequest),
  isDestroyed(false),
  connectState(connectIdle),
  putGetState(putGetIdle)
{
}

PvaClientPutGet::~PvaClientPutGet()
{
    destroy();
}

void PvaClientPutGet::checkPutGetState()
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    if(connectState==connectIdle){
        connect();
        getPut();
    }
}

// from ChannelPutGetRequester
string PvaClientPutGet::getRequesterName()
{
     PvaClientPtr yyy = pvaClient.lock();
     if(!yyy) throw std::runtime_error("pvaClient was destroyed");
     return yyy->getRequesterName();
}

void PvaClientPutGet::message(string const & message,MessageType messageType)
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    PvaClientPtr yyy = pvaClient.lock();
    if(!yyy) throw std::runtime_error("pvaClient was destroyed");
    yyy->message(message, messageType);
}

void PvaClientPutGet::channelPutGetConnect(
    const Status& status,
    ChannelPutGet::shared_pointer const & channelPutGet,
    StructureConstPtr const & putStructure,
    StructureConstPtr const & getStructure)
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    channelPutGetConnectStatus = status;
    this->channelPutGet = channelPutGet;
    if(status.isOK()) {
        pvaClientPutData = PvaClientPutData::create(putStructure);
        pvaClientPutData->setMessagePrefix(channel->getChannelName());
        pvaClientGetData = PvaClientGetData::create(getStructure);
        pvaClientGetData->setMessagePrefix(channel->getChannelName());
    }
    waitForConnect.signal();
    
}

void PvaClientPutGet::putGetDone(
        const Status& status,
        ChannelPutGet::shared_pointer const & channelPutGet,
        PVStructurePtr const & getPVStructure,
        BitSetPtr const & getChangedBitSet)
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    channelPutGetStatus = status;
    if(status.isOK()) {
        pvaClientGetData->setData(getPVStructure,getChangedBitSet);
    }
    waitForPutGet.signal();
}

void PvaClientPutGet::getPutDone(
    const Status& status,
    ChannelPutGet::shared_pointer const & channelPutGet,
    PVStructurePtr const & putPVStructure,
    BitSetPtr const & putBitSet)
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    channelPutGetStatus = status;
    if(status.isOK()) {
        PVStructurePtr pvs = pvaClientPutData->getPVStructure();
        pvs->copyUnchecked(*putPVStructure,*putBitSet);
        BitSetPtr bs = pvaClientPutData->getChangedBitSet();
        bs->clear();
        *bs |= *putBitSet;
    }
    waitForPutGet.signal();
}

void PvaClientPutGet::getGetDone(
        const Status& status,
        ChannelPutGet::shared_pointer const & channelPutGet,
        PVStructurePtr const & getPVStructure,
        BitSetPtr const & getChangedBitSet)
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    channelPutGetStatus = status;
    if(status.isOK()) {
        pvaClientGetData->setData(getPVStructure,getChangedBitSet);
    }
    waitForPutGet.signal();
}



// from PvaClientPutGet
void PvaClientPutGet::destroy()
{
    {
        Lock xx(mutex);
        if(isDestroyed) return;
        isDestroyed = true;
    }
    if(channelPutGet) channelPutGet->destroy();
    channelPutGet.reset();
}

void PvaClientPutGet::connect()
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    issueConnect();
    Status status = waitConnect();
    if(status.isOK()) return;
    string message = string("channel ") + channel->getChannelName()
        + " PvaClientPutGet::connect " + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientPutGet::issueConnect()
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    if(connectState!=connectIdle) {
        string message = string("channel ") + channel->getChannelName()
            + " pvaClientPutGet already connected ";
        throw std::runtime_error(message);
    }
    putGetRequester = ChannelPutGetRequester::shared_pointer(new ChannelPutGetRequesterImpl(this));
    connectState = connectActive;
    channelPutGet = channel->createChannelPutGet(putGetRequester,pvRequest);
}

Status PvaClientPutGet::waitConnect()
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    if(connectState!=connectActive) {
        string message = string("channel ") + channel->getChannelName()
            + " pvaClientPutGet illegal connect state ";
        throw std::runtime_error(message);
    }
    waitForConnect.wait();
    connectState = channelPutGetConnectStatus.isOK() ? connected : connectIdle;
    return channelPutGetConnectStatus;
}


void PvaClientPutGet::putGet()
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    issuePutGet();
    Status status = waitPutGet();
    if(status.isOK()) return;
    string message = string("channel ") + channel->getChannelName()
        + " PvaClientPutGet::putGet " + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientPutGet::issuePutGet()
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    if(connectState==connectIdle) connect();
    if(putGetState!=putGetIdle) {
        string message = string("channel ") + channel->getChannelName()
            + " PvaClientPutGet::issueGet get or put aleady active ";
        throw std::runtime_error(message);
    }
    putGetState = putGetActive;
    channelPutGet->putGet(pvaClientPutData->getPVStructure(),pvaClientPutData->getChangedBitSet());
}


Status PvaClientPutGet::waitPutGet()
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    if(putGetState!=putGetActive){
        string message = string("channel ") + channel->getChannelName()
            + " PvaClientPutGet::waitPutGet llegal put state";
        throw std::runtime_error(message);
    }
    waitForPutGet.wait();
    putGetState = putGetIdle;
    return channelPutGetStatus;
}

void PvaClientPutGet::getGet()
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    issueGetGet();
    Status status = waitGetGet();
    if(status.isOK()) return;
    string message = string("channel ") + channel->getChannelName()
        + " PvaClientPutGet::getGet " + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientPutGet::issueGetGet()
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    if(connectState==connectIdle) connect();
    if(putGetState!=putGetIdle) {
        string message = string("channel ") + channel->getChannelName()
            + " PvaClientPutGet::issueGetGet aleady active ";
        throw std::runtime_error(message);
    }
    putGetState = putGetActive;
    channelPutGet->getGet();
}

Status PvaClientPutGet::waitGetGet()
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    if(putGetState!=putGetActive){
        string message = string("channel ") + channel->getChannelName()
            + " PvaClientPutGet::waitGetGet illegal state";
        throw std::runtime_error(message);
    }
    waitForPutGet.wait();
    putGetState = putGetIdle;
    return channelPutGetStatus;
}

void PvaClientPutGet::getPut()
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    issueGetPut();
    Status status = waitGetPut();
    if(status.isOK()) return;
    string message = string("channel ") + channel->getChannelName()
        + " PvaClientPutGet::getPut " + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientPutGet::issueGetPut()
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    if(connectState==connectIdle) connect();
    if(putGetState!=putGetIdle) {
        string message = string("channel ") + channel->getChannelName()
            + " PvaClientPutGet::issueGetPut aleady active ";
        throw std::runtime_error(message);
    }
    putGetState = putGetActive;
    channelPutGet->getPut();
}

Status PvaClientPutGet::waitGetPut()
{
    if(isDestroyed) throw std::runtime_error("pvaClientPutGet was destroyed");
    if(putGetState!=putGetActive){
         string message = string("channel ") + channel->getChannelName()
            + " PvaClientPutGet::waitGetPut illegal state";
        throw std::runtime_error(message);
    }
    waitForPutGet.wait();
    putGetState = putGetIdle;
    return channelPutGetStatus;
}

PvaClientGetDataPtr PvaClientPutGet::getGetData()
{
    checkPutGetState();
    return pvaClientGetData;
}

PvaClientPutDataPtr PvaClientPutGet::getPutData()
{
    checkPutGetState();
    return pvaClientPutData;
}

PvaClientPutGetPtr PvaClientPutGet::create(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
{
    PvaClientPutGetPtr epv(new PvaClientPutGet(pvaClient,channel,pvRequest));
    return epv;
}


}}
