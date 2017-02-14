/* pvaClientNTMultiData.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2015.03
 */

#include <epicsMath.h>

#define epicsExportSharedSymbols

#include <pv/pvaClientMultiChannel.h>

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::nt;
using namespace std;

namespace epics { namespace pvaClient { 

static PVDataCreatePtr pvDataCreate = getPVDataCreate();


PvaClientNTMultiDataPtr PvaClientNTMultiData::create(
    epics::pvData::UnionConstPtr const & u,
    PvaClientMultiChannelPtr const &pvaMultiChannel,
    PvaClientChannelArray const &pvaClientChannelArray,
    PVStructurePtr const &  pvRequest)
{
    return PvaClientNTMultiDataPtr(
         new PvaClientNTMultiData(u,pvaMultiChannel,pvaClientChannelArray,pvRequest));
}

PvaClientNTMultiData::PvaClientNTMultiData(
         epics::pvData::UnionConstPtr const & u,
         PvaClientMultiChannelPtr const &pvaClientMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray,
         epics::pvData::PVStructurePtr const &  pvRequest)
: pvaClientMultiChannel(pvaClientMultiChannel),
  pvaClientChannelArray(pvaClientChannelArray),
  nchannel(pvaClientChannelArray.size()),
  gotAlarm(false),
  gotTimeStamp(false),
  isDestroyed(false)
{
    PVFieldPtr pvValue = pvRequest->getSubField("field.value");
    if(!pvValue) {
        throw std::runtime_error("pvRequest did not specify value");
    }
    topPVStructure.resize(nchannel);
    unionValue.resize(nchannel);
    for(size_t i=0; i< nchannel; ++i) {
         topPVStructure[i] = PVStructurePtr();
         unionValue[i] = pvDataCreate->createPVUnion(u);
    }
    NTMultiChannelBuilderPtr builder = NTMultiChannel::createBuilder();
    builder->value(u)->addIsConnected();
    if(pvRequest->getSubField("field.alarm"))
    {
         gotAlarm = true;
         builder->addAlarm();
         builder->addSeverity();
         builder->addStatus();
         builder->addMessage();
         severity.resize(nchannel);
         status.resize(nchannel);
         message.resize(nchannel);
         
    }
    if(pvRequest->getSubField("field.timeStamp")) {
        gotTimeStamp = true;
        builder->addTimeStamp();
        builder->addSecondsPastEpoch();
        builder->addNanoseconds();
        builder->addUserTag();
        secondsPastEpoch.resize(nchannel);
        nanoseconds.resize(nchannel);
        userTag.resize(nchannel);
    }
    ntMultiChannelStructure = builder->createStructure();
}


PvaClientNTMultiData::~PvaClientNTMultiData()
{
    destroy();
}

void PvaClientNTMultiData::destroy()
{
    {
        Lock xx(mutex);
        if(isDestroyed) return;
        isDestroyed = true;
    }
    pvaClientChannelArray.clear();
}

void PvaClientNTMultiData::setStructure(StructureConstPtr const & structure,size_t index)
{
    FieldConstPtr field = structure->getField("value");
    if(!field) {
        string message = "channel "
           + pvaClientChannelArray[index]->getChannel()->getChannelName()
           + " does not have top level value field";
        throw std::runtime_error(message);
    }
}

void PvaClientNTMultiData::setPVStructure(
        PVStructurePtr const &pvStructure,size_t index)
{
    topPVStructure[index] = pvStructure;
}


size_t PvaClientNTMultiData::getNumber()
{
    return nchannel;
}

void PvaClientNTMultiData::startDeltaTime()
{
    for(size_t i=0; i<nchannel; ++i)
    {
        topPVStructure[i] = PVStructurePtr();
        if(gotAlarm)
        {
            alarm.setSeverity(noAlarm);
            alarm.setStatus(noStatus);
            alarm.setMessage("");
            severity[i] = invalidAlarm;
            status[i] = undefinedStatus;
            message[i] = "not connected";
        }
        if(gotTimeStamp)
        {
            timeStamp.getCurrent();
            secondsPastEpoch[i] = 0;
            nanoseconds[i] = 0;
            userTag[i] = 0;
        }
    }
}


void PvaClientNTMultiData::endDeltaTime()
{
    for(size_t i=0; i<nchannel; ++i)
    {
        PVStructurePtr pvst = topPVStructure[i];
        if(!pvst) {
              unionValue[i] = PVUnionPtr();
        } else {
              unionValue[i]->set(pvst->getSubField("value"));
        }
        if(gotAlarm)
        {
              severity[i] = pvst->getSubField<PVInt>("alarm.severity")->get();
              status[i] = pvst->getSubField<PVInt>("alarm.status")->get();
              message[i] = pvst->getSubField<PVString>("alarm.message")->get();
        }
        if(gotTimeStamp)
        {
              secondsPastEpoch[i] = pvst->getSubField<PVLong>("timeStamp.secondsPastEpoch")->get();
              nanoseconds[i] = pvst->getSubField<PVInt>("timeStamp.nanoseconds")->get();
              userTag[i] = pvst->getSubField<PVInt>("timeStamp.userTag")->get();
        }
             
    }
}

TimeStamp PvaClientNTMultiData::getTimeStamp()
{
    pvTimeStamp.get(timeStamp);
    return timeStamp;
}

NTMultiChannelPtr PvaClientNTMultiData::getNTMultiChannel()
{
    PVStructurePtr pvStructure = pvDataCreate->createPVStructure(ntMultiChannelStructure);
    NTMultiChannelPtr ntMultiChannel = NTMultiChannel::wrap(pvStructure);
    ntMultiChannel->getChannelName()->replace(pvaClientMultiChannel->getChannelNames());
    shared_vector<epics::pvData::PVUnionPtr> val(nchannel);
    for(size_t i=0; i<nchannel; ++i) val[i] = unionValue[i];
    ntMultiChannel->getValue()->replace(freeze(val));
    shared_vector<epics::pvData::boolean> connected = pvaClientMultiChannel->getIsConnected();
    shared_vector<epics::pvData::boolean> isConnected(nchannel);
    for(size_t i=0; i<nchannel; ++i) isConnected[i] = connected[i];
    ntMultiChannel->getIsConnected()->replace(freeze(isConnected));
    if(gotAlarm)
    {
        shared_vector<int32> sev(nchannel);
        for(size_t i=0; i<nchannel; ++i) sev[i] = severity[i];
        ntMultiChannel->getSeverity()->replace(freeze(sev));
        shared_vector<int32> sta(nchannel);
        for(size_t i=0; i<nchannel; ++i) sta[i] = status[i];
        ntMultiChannel->getStatus()->replace(freeze(sta));
        shared_vector<string> mes(nchannel);
        for(size_t i=0; i<nchannel; ++i) mes[i] = message[i];
        ntMultiChannel->getMessage()->replace(freeze(mes));
    }
    if(gotTimeStamp)
    {
        shared_vector<int64> sec(nchannel);
        for(size_t i=0; i<nchannel; ++i) sec[i] = secondsPastEpoch[i];
        ntMultiChannel->getSecondsPastEpoch()->replace(freeze(sec));
        shared_vector<int32> nano(nchannel);
        for(size_t i=0; i<nchannel; ++i) nano[i] = nanoseconds[i];
        ntMultiChannel->getNanoseconds()->replace(freeze(nano));
        shared_vector<int32> tag(nchannel);
        for(size_t i=0; i<nchannel; ++i) tag[i] = userTag[i];
        ntMultiChannel->getUserTag()->replace(freeze(tag));
    }
    return ntMultiChannel;
}

}}
