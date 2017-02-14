/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
#ifndef DBGROUP_H
#define DBGROUP_H

#include <pv/channelBase.h>
#include <pv/convert.h>
#include <pv/timeStamp.h>
#include <pv/pvTimeStamp.h>
#include <pv/alarm.h>
#include <pv/pvAlarm.h>

#include "pvValue.h"

namespace epics { namespace pvaSrv { 

class DbGroup;
class DbGroupProvider;
class DbGroupGet;
typedef std::tr1::shared_ptr<DbGroup> DbGroupPtr;
typedef std::tr1::shared_ptr<DbGroupProvider> DbGroupProviderPtr;
typedef std::tr1::shared_ptr<DbGroupGet> DbGroupGetPtr;

class DbGroup :
    public epics::pvAccess::ChannelBase
{
public:
    POINTER_DEFINITIONS(DbGroup);
    DbGroup(DbGroupProviderPtr const & provider,
        epics::pvAccess::ChannelRequester::shared_pointer const & requester);
    bool create();
    virtual ~DbGroup() {}
    virtual void destroy();
    virtual void getField(
        epics::pvAccess::GetFieldRequester::shared_pointer const &requester,
        std::string const &subField);
    virtual epics::pvAccess::ChannelGet::shared_pointer createChannelGet(
        epics::pvAccess::ChannelGetRequester::shared_pointer const &channelGetRequester,
        epics::pvData::PVStructure::shared_pointer const &pvRequest);
private:
    DbGroupProviderPtr provider;
    epics::pvAccess::ChannelRequester::shared_pointer const & requester;
    pvValuePtrArrayPtr arrayPvValue;
    epics::pvData::StructureConstPtr structure;
    epics::pvData::Mutex mutex;
    epics::pvData::Event event;
    friend class DbGroupGet;
};

class DbGroupGet :
  public virtual epics::pvAccess::ChannelGet,
  public std::tr1::enable_shared_from_this<DbGroupGet>
{
public:
    POINTER_DEFINITIONS(DbGroupGet);
    DbGroupGet(
        DbGroupPtr const & channel,
        epics::pvAccess::ChannelGetRequester::shared_pointer const &channelGetRequester);
    virtual ~DbGroupGet();
    bool init(epics::pvData::PVStructure::shared_pointer const & pvRequest);
    virtual std::string getRequesterName();
    virtual void message(
        std::string const &message,
        epics::pvData::MessageType messageType);
    virtual void destroy();
    virtual void get(bool lastRequest);
    virtual void lock();
    virtual void unlock();
private:
    shared_pointer getPtrSelf()
    {
        return shared_from_this();
    }
    DbGroupPtr dbGroup;
    epics::pvAccess::ChannelGetRequester::shared_pointer  channelGetRequester;
    pvValuePtrArrayPtr arrayPvValue;
    epics::pvData::PVStructurePtr pvTop;
    epics::pvData::BitSetPtr bitSet;
    epics::pvData::Alarm alarm;
    epics::pvData::PVAlarm pvAlarm;
    epics::pvData::TimeStamp timeStamp;
    epics::pvData::PVTimeStamp pvTimeStamp;
};


class DbGroupProvider :
   public epics::pvAccess::ChannelBaseProvider
{
public:
    POINTER_DEFINITIONS(DbGroupProvider);
    /**
     * Create a PVStructure and add to channelProviderLocal.
     * @param requester The requester.
     * @param pvValueProvider Provider for value PVs.
     * @param channelName The channelName.
     * @param fieldNames An array of fieldNames for the top level PVStructure.
     * @param pvValueNames An array of DB record names.
     */
    DbGroupProvider(
         epics::pvData::RequesterPtr const & requester,
         epics::pvAccess::ChannelProvider::shared_pointer const &pvValueProvider,
         std::string const & channelName,
         epics::pvData::StringArrayPtr const & fieldNames,
         epics::pvData::StringArrayPtr const & pvValueNames);
    virtual ~DbGroupProvider();
    virtual void destroy();
    virtual epics::pvAccess::ChannelFind::shared_pointer channelFind(
        std::string const & channelName,
        epics::pvAccess::ChannelFindRequester::shared_pointer const & channelFindRequester);
    virtual epics::pvAccess::Channel::shared_pointer createChannel(
        std::string const & channelName,
        epics::pvAccess::ChannelRequester::shared_pointer  const & channelRequester,
        short priority,
        std::string const & address);
    virtual std::tr1::shared_ptr<epics::pvAccess::ChannelProvider> getChannelProvider()
        {return getPtrSelf();}
    virtual void cancelChannelFind() {}
    void dump();
private:
    epics::pvData::RequesterPtr requester;
    epics::pvAccess::ChannelProvider::shared_pointer pvValueProvider;
    std::string channelName;
    epics::pvData::StringArrayPtr fieldNames;
    epics::pvData::StringArrayPtr pvValueNames;
    epics::pvData::Mutex mutex;
    epics::pvData::Event event;
    friend class DbGroup;
};

}}

#endif  /* DBGROUP_H */
