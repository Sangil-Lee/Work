/* pvaClientMultiChannel.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2015.02
 */

#include <map>
#include <pv/event.h>
#include <pv/lock.h>

#define epicsExportSharedSymbols

#include <pv/pvaClientMultiChannel.h>



using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace std;

namespace epics { namespace pvaClient {

static FieldCreatePtr fieldCreate = getFieldCreate();
static CreateRequest::shared_pointer  createRequestPvt = CreateRequest::create();

PvaClientMultiChannelPtr PvaClientMultiChannel::create(
   PvaClientPtr const &pvaClient,
   epics::pvData::shared_vector<const string> const & channelNames,
   string const & providerName,
   size_t maxNotConnected)
{
    return PvaClientMultiChannelPtr(
        new PvaClientMultiChannel(pvaClient,channelNames,providerName,maxNotConnected));
}


PvaClientMultiChannel::PvaClientMultiChannel(
    PvaClientPtr const &pvaClient,
    epics::pvData::shared_vector<const string> const & channelName,
    string const & providerName,
    size_t maxNotConnected)
: pvaClient(pvaClient),
  channelName(channelName),
  providerName(providerName),
  maxNotConnected(maxNotConnected),
  numChannel(channelName.size()),
  numConnected(0),
  pvaClientChannelArray(PvaClientChannelArray(numChannel,PvaClientChannelPtr())),
  isConnected(shared_vector<epics::pvData::boolean>(numChannel,false)),
  createRequest(CreateRequest::create()),
  isDestroyed(false)
{
}

PvaClientMultiChannel::~PvaClientMultiChannel()
{
    destroy();
}

void PvaClientMultiChannel::destroy()
{
    {
        Lock xx(mutex);
        if(isDestroyed) return;
        isDestroyed = true;
    }
    pvaClientChannelArray.clear();
}

void PvaClientMultiChannel::checkConnected()
{
    if(numConnected==0){
        Status status = connect(3.0);
        if(status.isOK()) return;
        throw std::runtime_error("pvaClientMultiChannel connect failure");
    }
}

epics::pvData::shared_vector<const string> PvaClientMultiChannel::getChannelNames()
{
    if(isDestroyed) throw std::runtime_error("pvaClientMultiChannel was destroyed");
    return channelName;
}

Status PvaClientMultiChannel::connect(double timeout)
{
    if(isDestroyed) throw std::runtime_error("pvaClientMultiChannel was destroyed");
    for(size_t i=0; i< numChannel; ++i) {
        pvaClientChannelArray[i] = pvaClient->createChannel(channelName[i],providerName);
        pvaClientChannelArray[i]->issueConnect();
    }
    Status returnStatus = Status::Ok;
    Status status = Status::Ok;
    size_t numBad = 0;
    for(size_t i=0; i< numChannel; ++i) {
	if(numBad==0) {
            status = pvaClientChannelArray[i]->waitConnect(timeout);
        } else {
            status = pvaClientChannelArray[i]->waitConnect(.001);
        }
        if(status.isOK()) {
            ++numConnected;
            isConnected[i] = true;
            continue;
        }
        if(returnStatus.isOK()) returnStatus = status;
        ++numBad;
        if(numBad>maxNotConnected) break;
    }
    return numBad>maxNotConnected ? returnStatus : Status::Ok;
}


bool PvaClientMultiChannel::allConnected()
{
    if(isDestroyed) throw std::runtime_error("pvaClientMultiChannel was destroyed");
    return (numConnected==numChannel) ? true : false;
}

bool PvaClientMultiChannel::connectionChange()
{
    if(isDestroyed) throw std::runtime_error("pvaClientMultiChannel was destroyed");
    for(size_t i=0; i<numChannel; ++i) {
         PvaClientChannelPtr pvaClientChannel = pvaClientChannelArray[i];
         Channel::shared_pointer channel = pvaClientChannel->getChannel();
         Channel::ConnectionState stateNow = channel->getConnectionState();
         bool connectedNow = stateNow==Channel::CONNECTED ? true : false;
         if(connectedNow!=isConnected[i]) return true;
    }
    return false;
}

epics::pvData::shared_vector<epics::pvData::boolean>  PvaClientMultiChannel::getIsConnected()
{
    if(isDestroyed) throw std::runtime_error("pvaClientMultiChannel was destroyed");
    for(size_t i=0; i<numChannel; ++i) {
         PvaClientChannelPtr pvaClientChannel = pvaClientChannelArray[i];
         if(!pvaClientChannel) {
              isConnected[i] = false;
              continue;
         }
         Channel::shared_pointer channel = pvaClientChannel->getChannel();
         Channel::ConnectionState stateNow = channel->getConnectionState();
         isConnected[i] = (stateNow==Channel::CONNECTED) ? true : false;
    }
    return isConnected;
}

PvaClientChannelArray PvaClientMultiChannel::getPvaClientChannelArray()
{
    if(isDestroyed) throw std::runtime_error("pvaClientMultiChannel was destroyed");
    return pvaClientChannelArray;
}

PvaClientPtr PvaClientMultiChannel::getPvaClient()
{
    if(isDestroyed) throw std::runtime_error("pvaClientMultiChannel was destroyed");
    return pvaClient;
}

 

PvaClientMultiGetDoublePtr PvaClientMultiChannel::createGet()
{
    checkConnected();
    return PvaClientMultiGetDouble::create(getPtrSelf(),pvaClientChannelArray);
}


PvaClientMultiPutDoublePtr PvaClientMultiChannel::createPut()
{
    checkConnected();
    return PvaClientMultiPutDouble::create(getPtrSelf(),pvaClientChannelArray);
}


PvaClientMultiMonitorDoublePtr PvaClientMultiChannel::createMonitor()
{
    checkConnected();
     return PvaClientMultiMonitorDouble::create(getPtrSelf(), pvaClientChannelArray);
}

PvaClientNTMultiPutPtr PvaClientMultiChannel::createNTPut()
{
    checkConnected();
    return PvaClientNTMultiPut::create(getPtrSelf(), pvaClientChannelArray);
}


PvaClientNTMultiGetPtr PvaClientMultiChannel::createNTGet()
{
    return createNTGet("value,alarm,timeStamp");
}

PvaClientNTMultiGetPtr PvaClientMultiChannel::createNTGet(std::string const &request)
{
    checkConnected();
    PVStructurePtr pvRequest = createRequest->createRequest(request);
    if(!pvRequest) {
        string message = " PvaClientMultiChannel::createNTGet invalid pvRequest: "
             + createRequest->getMessage();
        throw std::runtime_error(message);
    }
    return PvaClientNTMultiGet::create(getPtrSelf(), pvaClientChannelArray,pvRequest);
}


PvaClientNTMultiMonitorPtr PvaClientMultiChannel::createNTMonitor()
{
    return createNTMonitor("value,alarm,timeStamp");
}

PvaClientNTMultiMonitorPtr PvaClientMultiChannel::createNTMonitor(std::string const &request)
{
    checkConnected();
    PVStructurePtr pvRequest = createRequest->createRequest(request);
    if(!pvRequest) {
        string message = " PvaClientMultiChannel::createNTMonitor invalid pvRequest: "
             + createRequest->getMessage();
        throw std::runtime_error(message);
    }
    return PvaClientNTMultiMonitor::create(getPtrSelf(), pvaClientChannelArray,pvRequest);
}


}}
