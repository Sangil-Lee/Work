/* pvaClientMonitor.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2015.03
 */

#include <sstream>
#include <pv/event.h>
#include <pv/bitSetUtil.h>

#define epicsExportSharedSymbols

#include <pv/pvaClient.h>

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace std;

namespace epics { namespace pvaClient {


class ChannelMonitorRequester : public MonitorRequester
{
    PvaClientMonitor * pvaClientMonitor;
public:
    ChannelMonitorRequester(PvaClientMonitor * pvaClientMonitor)
    : pvaClientMonitor(pvaClientMonitor) {}
    string getRequesterName()
    {return pvaClientMonitor->getRequesterName();}
    void message(string const & message,MessageType messageType)
    {pvaClientMonitor->message(message,messageType);}
    void monitorConnect(
        const Status& status,
        Monitor::shared_pointer const & monitor,
        StructureConstPtr const & structure)
    {pvaClientMonitor->monitorConnect(status,monitor,structure);}
    void monitorEvent(MonitorPtr const & monitor)
    {
         pvaClientMonitor->monitorEvent(monitor);
    }
    void unlisten(MonitorPtr const & monitor)
    {pvaClientMonitor->unlisten();}
};

PvaClientMonitor::PvaClientMonitor(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
: pvaClient(pvaClient),
  channel(channel),
  pvRequest(pvRequest),
  isDestroyed(false),
  connectState(connectIdle),
  userPoll(false),
  userWait(false)
{
}

PvaClientMonitor::~PvaClientMonitor()
{
    destroy();
}

void PvaClientMonitor::checkMonitorState()
{
    if(isDestroyed) throw std::runtime_error("pvaClientMonitor was destroyed");
    if(connectState==connectIdle) connect();
    if(connectState==connected) start();
}

// from MonitorRequester
string PvaClientMonitor::getRequesterName()
{
     PvaClientPtr yyy = pvaClient.lock();
     if(!yyy) throw std::runtime_error("pvaClient was destroyed");
     return yyy->getRequesterName();
}

void PvaClientMonitor::message(string const & message,MessageType messageType)
{
    if(isDestroyed) throw std::runtime_error("pvaClientMonitor was destroyed");
    PvaClientPtr yyy = pvaClient.lock();
    if(!yyy) throw std::runtime_error("pvaClient was destroyed");
    yyy->message(message, messageType);
}

void PvaClientMonitor::monitorConnect(
    const Status& status,
    Monitor::shared_pointer const & monitor,
    StructureConstPtr const & structure)
{
    if(isDestroyed) throw std::runtime_error("pvaClientMonitor was destroyed");
    connectStatus = status;
    this->monitor = monitor;
    if(status.isOK()) {
        pvaClientData = PvaClientMonitorData::create(structure);
        pvaClientData->setMessagePrefix(channel->getChannelName());
    }
    waitForConnect.signal();
    
}

void PvaClientMonitor::monitorEvent(MonitorPtr const & monitor)
{
    if(isDestroyed) throw std::runtime_error("pvaClientMonitor was destroyed");
    PvaClientMonitorRequesterPtr req = pvaClientMonitorRequester.lock();
    if(req) req->event(getPtrSelf());
    if(userWait) waitForEvent.signal();
}

void PvaClientMonitor::unlisten()
{
    destroy();
}

void PvaClientMonitor::destroy()
{
    {
        Lock xx(mutex);
        if(isDestroyed) return;
        isDestroyed = true;
    }
    if(monitor) monitor->destroy();
    monitor.reset();
    monitorElement.reset();
}

void PvaClientMonitor::connect()
{
    if(isDestroyed) throw std::runtime_error("pvaClientMonitor was destroyed");
    issueConnect();
    Status status = waitConnect();
    if(status.isOK()) return;
    string message = string("channel ") + channel->getChannelName() 
         + " PvaClientMonitor::connect " + status.getMessage();
    throw std::runtime_error(message);
}

void PvaClientMonitor::issueConnect()
{
    if(isDestroyed) throw std::runtime_error("pvaClientMonitor was destroyed");
    if(connectState!=connectIdle) {
        string message = string("channel ") + channel->getChannelName() 
            + " pvaClientMonitor already connected ";
        throw std::runtime_error(message);
    }
    monitorRequester = ChannelMonitorRequester::shared_pointer(new ChannelMonitorRequester(this));
    connectState = connectActive;
    monitor = channel->createMonitor(monitorRequester,pvRequest);
}

Status PvaClientMonitor::waitConnect()
{
    if(isDestroyed) throw std::runtime_error("pvaClientMonitor was destroyed");
    if(connectState!=connectActive) {
        string message = string("channel ") + channel->getChannelName() 
            + " pvaClientMonitor illegal connect state ";
        throw std::runtime_error(message);
    }
    waitForConnect.wait();
    connectState = connectStatus.isOK() ? connected : connectIdle;
    return connectStatus;
}

void PvaClientMonitor::setRequester(PvaClientMonitorRequesterPtr const & pvaClientMonitorrRequester)
{
    this->pvaClientMonitorRequester = pvaClientMonitorrRequester;
}

void PvaClientMonitor::start()
{
    if(isDestroyed) throw std::runtime_error("pvaClientMonitor was destroyed");
    if(connectState==monitorStarted) return;
    if(connectState==connectIdle) connect();
    if(connectState!=connected) throw std::runtime_error("PvaClientMonitor::start illegal state");
    connectState = monitorStarted;
    monitor->start();
}


void PvaClientMonitor::stop()
{
    if(isDestroyed) throw std::runtime_error("pvaClientMonitor was destroyed");
    if(connectState!=monitorStarted) return;
    connectState = connected;
    monitor->stop();
}

bool PvaClientMonitor::poll()
{
    checkMonitorState();
    if(connectState!=monitorStarted) throw std::runtime_error("PvaClientMonitor::poll illegal state");
    if(userPoll) throw std::runtime_error("PvaClientMonitor::poll did not release last");
    monitorElement = monitor->poll();
    if(!monitorElement) return false;
    userPoll = true;
    pvaClientData->setData(monitorElement);
   return true;
}

bool PvaClientMonitor::waitEvent(double secondsToWait)
{
    if(isDestroyed) throw std::runtime_error("pvaClientMonitor was destroyed");
    if(connectState!=monitorStarted) throw std::runtime_error("PvaClientMonitor::waitEvent illegal state");
    if(poll()) return true;
    userWait = true;
    if(secondsToWait==0.0) {
        waitForEvent.wait();
    } else {
        waitForEvent.wait(secondsToWait);
    }
    userWait = false;
    return poll();
}

void PvaClientMonitor::releaseEvent()
{
    if(isDestroyed) throw std::runtime_error("pvaClientMonitor was destroyed");
    if(connectState!=monitorStarted) throw std::runtime_error("PvaClientMonitor::poll illegal state");
    if(!userPoll) throw std::runtime_error("PvaClientMonitor::releaseEvent did not call poll");
    userPoll = false;
    monitor->release(monitorElement);
}

PvaClientMonitorDataPtr PvaClientMonitor::getData()
{
    checkMonitorState();
    return pvaClientData;
}

PvaClientMonitorPtr PvaClientMonitor::create(
        PvaClientPtr const &pvaClient,
        Channel::shared_pointer const & channel,
        PVStructurePtr const &pvRequest)
{
    PvaClientMonitorPtr epv(new PvaClientMonitor(pvaClient,channel,pvRequest));
    return epv;
}

}}
