/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#ifndef PVVALUE_H
#define PVVALUE_H

#include <pv/lock.h>
#include <pv/event.h>
#include <pv/pvData.h>
#include <pv/alarm.h>
#include <pv/timeStamp.h>
#include <pv/pvAccess.h>

namespace epics { namespace pvaSrv { 

class PvValue;
typedef std::tr1::shared_ptr<PvValue> ValueChannelPtr;
typedef std::vector<ValueChannelPtr> pvValuePtrArray;
typedef std::tr1::shared_ptr<pvValuePtrArray> pvValuePtrArrayPtr;

class PvValue :
    public epics::pvAccess::ChannelRequester,
    public epics::pvAccess::ChannelGetRequester,
    public std::tr1::enable_shared_from_this<PvValue>
{
public:
    POINTER_DEFINITIONS(PvValue);
    PvValue(
         epics::pvData::RequesterPtr const &requester,
         epics::pvAccess::ChannelProvider::shared_pointer const &channelProvider,
         std::string const &channelName);
    virtual ~PvValue();
    void connect();
    virtual void destroy();
    epics::pvData::Status waitConnect();
    epics::pvData::PVFieldPtr getValue();
    epics::pvData::Status get();
    epics::pvData::Status waitGet();
    epics::pvData::Status getTimeStamp(
        epics::pvData::TimeStamp &timeStamp);
    epics::pvData::Status getAlarm(
        epics::pvData::Alarm &alarm);
    virtual std::string getRequesterName();
    virtual void message(
        std::string const & message,
        epics::pvData::MessageType messageType);
    virtual void channelCreated(
        const epics::pvData::Status& status,
        epics::pvAccess::Channel::shared_pointer const & channel);
    virtual void channelStateChange(
        epics::pvAccess::Channel::shared_pointer const & channel, 
        epics::pvAccess::Channel::ConnectionState connectionState);
    virtual void channelGetConnect(
        const epics::pvData::Status& status,
        epics::pvAccess::ChannelGet::shared_pointer const & channelGet,
        epics::pvData::PVStructure::shared_pointer const & pvStructure,
        epics::pvData::BitSet::shared_pointer const & bitSet);
    virtual void getDone(const epics::pvData::Status& status);
private:
    shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
    epics::pvData::RequesterPtr requester;
    epics::pvAccess::ChannelProvider::shared_pointer channelProvider;
    std::string channelName;
    bool isConnected;
    epics::pvData::Status status;
    epics::pvData::PVStructurePtr getRequest;
    epics::pvAccess::Channel::shared_pointer channel;
    epics::pvAccess::ChannelGet::shared_pointer channelGet;
    epics::pvData::PVStructurePtr pvGetStructure;
    epics::pvData::Mutex mutex;
    epics::pvData::Event event;
};

}}

#endif  /* PVVALUE_H */
