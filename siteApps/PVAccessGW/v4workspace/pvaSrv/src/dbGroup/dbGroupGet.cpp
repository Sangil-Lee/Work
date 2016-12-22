/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */

#include <string>
#include <stdexcept>
#include <memory>

#include <pv/channelProviderLocal.h>
#include <pv/service.h>
#include <pv/serverContext.h>

#include "dbGroup.h"

namespace epics { namespace pvaSrv { 

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::tr1::static_pointer_cast;

static FieldCreatePtr fieldCreate = getFieldCreate();
static StandardFieldPtr standardField = getStandardField();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static StandardPVFieldPtr standardPVField = getStandardPVField();
static ConvertPtr convert = getConvert();

DbGroupGet::DbGroupGet(
    DbGroupPtr const & channel,
    ChannelGetRequester::shared_pointer const &channelGetRequester)
: dbGroup(channel),
  channelGetRequester(channelGetRequester)
{}

DbGroupGet::~DbGroupGet()
{}

bool DbGroupGet::init(PVStructure::shared_pointer const & pvRequest)
{
    pvTop = pvDataCreate->createPVStructure(dbGroup->structure);
    size_t n = pvTop->getStructure()->getNumberFields();
    bitSet.reset(new BitSet(n));
    channelGetRequester->channelGetConnect(
        Status::Ok,
        getPtrSelf(),
        pvTop,
        bitSet);
    return true;
}

String DbGroupGet::getRequesterName()
{
   return channelGetRequester->getRequesterName();
}

void DbGroupGet::message(String const &message,MessageType messageType)
{
    channelGetRequester->message(message,messageType);
}

void DbGroupGet::destroy()
{
    dbGroup->removeChannelGet(getPtrSelf());
}

void DbGroupGet::get(bool lastRequest)
{
    Alarm maxAlarm;
    pvValuePtrArray *valueChannels =
         dbGroup->arrayPvValue.get();
    const PVFieldPtrArray & pvFields = pvTop->getPVFields();
    size_t n = pvFields.size();
    for(size_t i=0; i<n-2; i++) {
        ValueChannelPtr valueChannel = (*valueChannels)[i];
        Status status = valueChannel->get();
    }
    for(size_t i=0; i<n-2; i++) {
        ValueChannelPtr valueChannel = (*valueChannels)[i];
        Status status = valueChannel->waitGet();
        PVFieldPtr pvValue = valueChannel->getValue();
        valueChannel->getAlarm(alarm);
        if(alarm.getSeverity()>maxAlarm.getSeverity()) maxAlarm = alarm;
        convert->copy(pvValue,pvFields[i+2]);
    }
    pvAlarm.attach(pvTop->getSubFieldT("alarm"));
    pvAlarm.set(maxAlarm);
    pvTimeStamp.attach(pvTop->getSubFieldT("timeStamp"));
    timeStamp.getCurrent();
    pvTimeStamp.set(timeStamp);
    bitSet->clear();
    bitSet->set(0);
    channelGetRequester->getDone(Status::Ok);
}

void DbGroupGet::lock()
{
}

void DbGroupGet::unlock()
{
}

}}
