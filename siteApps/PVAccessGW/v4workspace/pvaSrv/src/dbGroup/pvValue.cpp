/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

#include <pv/pvTimeStamp.h>
#include <pv/pvAlarm.h>

#include "pvValue.h"

namespace epics { namespace pvaSrv { 

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;

static string getRequestString("value,alarm,timeStamp");


PvValue::~PvValue() {}

PvValue::PvValue(
         RequesterPtr const &requester,
         ChannelProvider::shared_pointer const &channelProvider,
         string const &channelName)
: requester(requester),
  channelProvider(channelProvider),
  channelName(channelName),
  isConnected(false),
  status(Status::Ok),
  getRequest(getCreateRequest()->createRequest(getRequestString,requester))
{}

void PvValue::connect()
{
    event.tryWait(); // make sure event is empty
    channel = channelProvider->createChannel(channelName,getPtrSelf());
}

void PvValue::destroy()
{
   if(channel.get()!=NULL) channel->destroy();
}

Status PvValue::waitConnect()
{
    status = Status::Ok;
    //TODO: remove hard-coded timeout
    if(!event.wait(2.0)) {
         status= Status(Status::STATUSTYPE_ERROR, "waitConnect timeout");
    }
    return status;
}
PVFieldPtr PvValue::getValue()
{
    if(pvGetStructure.get()==NULL) {
        PVFieldPtr pvField;
        return pvField;
    }
    return pvGetStructure->getSubField("value");
}

Status PvValue::get()
{
    if(!isConnected) return status;
    event.tryWait(); 
    channelGet->get(false);
    return Status::Ok;
}

Status PvValue::waitGet()
{
    event.wait();
    return status;
}

Status PvValue::getTimeStamp(TimeStamp &timeStamp)
{
    if(pvGetStructure.get()==NULL) {
        return Status(Status::STATUSTYPE_ERROR, "no timeStamp field");
    }
    PVTimeStamp pvTimeStamp;
    PVFieldPtr pvField = pvGetStructure->getSubField("timeStamp");
    if(pvField.get()==NULL) {
        return Status(Status::STATUSTYPE_ERROR, "no timeStamp field");
    }
    if(!pvTimeStamp.attach(pvField)) {
        return Status(Status::STATUSTYPE_ERROR, "no timeStamp field");
    }
    pvTimeStamp.get(timeStamp);
    return Status::Ok;
}

Status PvValue::getAlarm(Alarm &alarm)
{
    if(pvGetStructure.get()==NULL) {
        return Status(Status::STATUSTYPE_ERROR, "no alarm field");
    }
    PVAlarm pvAlarm;
    PVFieldPtr pvField = pvGetStructure->getSubField("alarm");
    if(pvField.get()==NULL) {
        return Status(Status::STATUSTYPE_ERROR, "no alarm field");
    }
    if(!pvAlarm.attach(pvField)) {
        return Status(Status::STATUSTYPE_ERROR, "no alarm field");
    }
    pvAlarm.get(alarm);
    return Status::Ok;
}

String PvValue::getRequesterName()
{
    return requester->getRequesterName();
}

void PvValue::message(
    string const & message,
    MessageType messageType)
{
    requester->message(message,messageType);
}

void PvValue::channelCreated(
    const Status& status,
    Channel::shared_pointer const & channel)
{
    if(!status.isOK()) {
        this->status = status;
        requester->message(status.getMessage(),errorMessage);
        isConnected = false;
        event.signal();
    }
    this->channel = channel;
    isConnected = true;
    channelGet = channel->createChannelGet(getPtrSelf(),getRequest);
}

void PvValue::channelStateChange(
    Channel::shared_pointer const & channel, 
    Channel::ConnectionState connectionState)
{
    if(connectionState!=Channel::CONNECTED) {
        isConnected = false;
        status = Status(Status::STATUSTYPE_ERROR, "channel disconnected");
        return;
    }
    this->channel = channel;
    isConnected = true;
    return;
}

void PvValue::channelGetConnect(
    const Status& status,
    ChannelGet::shared_pointer const & channelGet,
    PVStructure::shared_pointer const & pvStructure,
    BitSet::shared_pointer const & bitSet)
{
    if(!status.isOK()) {
         this->status = status;
    } else {
        this->status = Status::Ok;
        this->channelGet = channelGet;
        pvGetStructure = pvStructure;
        PVFieldPtr pvValue = pvGetStructure->getSubField("value");
        PVFieldPtr pvAlarm = pvGetStructure->getSubField("alarm");
        PVFieldPtr pvTimeStamp = pvGetStructure->getSubField("timeStamp");
        bool allGood = true;
        if(pvValue.get()==NULL) allGood = false;
        if(!allGood) {
            string builder("channel ");
            builder += channelName;
            if(pvValue.get()==NULL) builder += " no value field ";
            if(pvAlarm.get()==NULL) builder += " no alarm field ";
            if(pvTimeStamp.get()==NULL) builder += " no timeStamp field ";
            this->status = Status(Status::STATUSTYPE_ERROR,builder);
            builder.clear();
            pvGetStructure->toString(&builder);
            printf("channel %s pvGetStructure\n%s\n",
                channelName.c_str(),builder.c_str());
        }
    }
    event.signal();
}

void PvValue::getDone(const Status& status)
{
    if(!status.isOK()) {
         this->status = status;
    } else {
         this->status = Status::Ok;
    }
    event.signal();
}

}}
