/* pvaClientProcess.cpp */
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


class ChannelProcessRequesterImpl : public ChannelProcessRequester
{
    PvaClientProcess * pvaClientProcess;
public:
    ChannelProcessRequesterImpl(PvaClientProcess * pvaClientProcess)
    : pvaClientProcess(pvaClientProcess) {}
    string getRequesterName()
    {return pvaClientProcess->getRequesterName();}
    void message(string const & message,MessageType messageType)
    {pvaClientProcess->message(message,messageType);}
    void channelProcessConnect(
        const Status& status,
        ChannelProcess::shared_pointer const & channelProcess)
    {pvaClientProcess->channelProcessConnect(status,channelProcess);}
    void processDone(
        const Status& status,
        ChannelProcess::shared_pointer const & channelProcess)
    {pvaClientProcess->processDone(status,channelProcess);}
};

PvaClientProcess::PvaClientProcess(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
: pvaClient(pvaClient),
  channel(channel),
  pvRequest(pvRequest),
  isDestroyed(false),
  connectState(connectIdle),
  processState(processIdle)
{
}

PvaClientProcess::~PvaClientProcess()
{
    destroy();
}

// from ChannelProcessRequester
string PvaClientProcess::getRequesterName()
{
     PvaClientPtr yyy = pvaClient.lock();
     if(!yyy) throw std::runtime_error("pvaClient was destroyed");
     return yyy->getRequesterName();
}

void PvaClientProcess::message(string const & message,MessageType messageType)
{
    if(isDestroyed) throw std::runtime_error("pvaClientProcess was destroyed");
    PvaClientPtr yyy = pvaClient.lock();
    if(!yyy) throw std::runtime_error("pvaClient was destroyed");
    yyy->message(message, messageType);
}

void PvaClientProcess::channelProcessConnect(
    const Status& status,
    ChannelProcess::shared_pointer const & channelProcess)
{
    if(isDestroyed) return;
    channelProcessConnectStatus = status;
    this->channelProcess = channelProcess;
    waitForConnect.signal();
    
}

void PvaClientProcess::processDone(
    const Status& status,
    ChannelProcess::shared_pointer const & channelProcess)
{
    if(isDestroyed) return;
    channelProcessStatus = status;
    waitForProcess.signal();
}


// from PvaClientProcess
void PvaClientProcess::destroy()
{
    {
        Lock xx(mutex);
        if(isDestroyed) return;
        isDestroyed = true;
    }
    if(channelProcess) channelProcess->destroy();
    channelProcess.reset();
}

void PvaClientProcess::connect()
{
    if(isDestroyed) throw std::runtime_error("pvaClientProcess was destroyed");
    issueConnect();
    Status status = waitConnect();
    if(status.isOK()) return;
    string message = string("channel ") + channel->getChannelName()
        + " PvaClientProcess::connect " + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientProcess::issueConnect()
{
    if(isDestroyed) throw std::runtime_error("pvaClientProcess was destroyed");
    if(connectState!=connectIdle) {
        string message = string("channel ") + channel->getChannelName()
            + " pvaClientProcess already connected ";
        throw std::runtime_error(message);
    }
    processRequester = ChannelProcessRequester::shared_pointer(new ChannelProcessRequesterImpl(this));
    connectState = connectActive;
    channelProcess = channel->createChannelProcess(processRequester,pvRequest);
}

Status PvaClientProcess::waitConnect()
{
    if(isDestroyed) throw std::runtime_error("pvaClientProcess was destroyed");
    if(connectState!=connectActive) {
        string message = string("channel ") + channel->getChannelName()
            + " pvaClientProcess illegal connect state ";
        throw std::runtime_error(message);
    }
    waitForConnect.wait();
    connectState = channelProcessConnectStatus.isOK() ? connected : connectIdle;
    return channelProcessConnectStatus;
}

void PvaClientProcess::process()
{
    if(isDestroyed) throw std::runtime_error("pvaClientProcess was destroyed");
    issueProcess();
    Status status = waitProcess();
    if(status.isOK()) return;
    string message = string("channel ") + channel->getChannelName()
        + " PvaClientProcess::process" + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientProcess::issueProcess()
{
    if(isDestroyed) throw std::runtime_error("pvaClientProcess was destroyed");
    if(connectState==connectIdle) connect();
    if(processState!=processIdle) {
        string message = string("channel ") + channel->getChannelName()
            + " PvaClientProcess::issueProcess process aleady active ";
        throw std::runtime_error(message);
    }
    processState = processActive;
    channelProcess->process();
}

Status PvaClientProcess::waitProcess()
{
    if(isDestroyed) throw std::runtime_error("pvaClientProcess was destroyed");
    if(processState!=processActive){
        string message = string("channel ") + channel->getChannelName()
            + " PvaClientProcess::waitProcess llegal process state";
        throw std::runtime_error(message);
    }
    waitForProcess.wait();
    processState = processIdle;
    return channelProcessStatus;
}

PvaClientProcessPtr PvaClientProcess::create(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
{
    PvaClientProcessPtr epv(new PvaClientProcess(pvaClient,channel,pvRequest));
    return epv;
}

}}
