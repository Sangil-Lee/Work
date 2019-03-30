/* pvaClientNTMultiGet.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2015.03
 */

#include <pv/standardField.h>
#include <pv/convert.h>
#include <epicsMath.h>

#define epicsExportSharedSymbols

#include <pv/pvaClientMultiChannel.h>

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::nt;
using namespace std;

namespace epics { namespace pvaClient { 

static ConvertPtr convert = getConvert();
static FieldCreatePtr fieldCreate = getFieldCreate();
static PVDataCreatePtr pvDataCreate = getPVDataCreate();
static StandardFieldPtr standardField = getStandardField();


PvaClientNTMultiGetPtr PvaClientNTMultiGet::create(
    PvaClientMultiChannelPtr const &pvaMultiChannel,
    PvaClientChannelArray const &pvaClientChannelArray,
    PVStructurePtr const &  pvRequest)
{
    UnionConstPtr u = fieldCreate->createVariantUnion();
    PvaClientNTMultiGetPtr pvaClientNTMultiGet(
         new PvaClientNTMultiGet(u,pvaMultiChannel,pvaClientChannelArray,pvRequest));
    return pvaClientNTMultiGet;
}

PvaClientNTMultiGet::PvaClientNTMultiGet(
         UnionConstPtr const & u,
         PvaClientMultiChannelPtr const &pvaClientMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray,
         epics::pvData::PVStructurePtr const &  pvRequest)
: pvaClientMultiChannel(pvaClientMultiChannel),
  pvaClientChannelArray(pvaClientChannelArray),
  pvRequest(pvRequest),
  nchannel(pvaClientChannelArray.size()),
  pvaClientNTMultiData(
       PvaClientNTMultiData::create(
           u,
           pvaClientMultiChannel,
           pvaClientChannelArray,
           pvRequest)),
  isConnected(false),
  isDestroyed(false)
{
}

PvaClientNTMultiGet::~PvaClientNTMultiGet()
{
    destroy();
}

void PvaClientNTMultiGet::destroy()
{
    {
        Lock xx(mutex);
        if(isDestroyed) return;
        isDestroyed = true;
    }
    pvaClientChannelArray.clear();
}

void PvaClientNTMultiGet::connect()
{
    pvaClientGet.resize(nchannel);
    shared_vector<epics::pvData::boolean> isConnected = pvaClientMultiChannel->getIsConnected();
    string request = "value";
    if(pvRequest->getSubField("field.alarm")) request += ",alarm";
    if(pvRequest->getSubField("field.timeStamp")) request += ",timeStamp";
    for(size_t i=0; i<nchannel; ++i)
    {
         if(isConnected[i]) {
               pvaClientGet[i] = pvaClientChannelArray[i]->createGet(request);
               pvaClientGet[i]->issueConnect();
         }
    }
    for(size_t i=0; i<nchannel; ++i)
    {
         if(isConnected[i]) {
               Status status = pvaClientGet[i]->waitConnect();
               if(status.isOK()) continue;
               string message = string("channel ") +pvaClientChannelArray[i]->getChannelName() 
                    + " PvaChannelGet::waitConnect " + status.getMessage();
               throw std::runtime_error(message);
         }
    }
    this->isConnected = true;
}

void PvaClientNTMultiGet::get()
{
    if(!isConnected) connect();
    shared_vector<epics::pvData::boolean> isConnected = pvaClientMultiChannel->getIsConnected();
    
    for(size_t i=0; i<nchannel; ++i)
    {
         if(isConnected[i]) {
               pvaClientGet[i]->issueGet();
         }
    }
    for(size_t i=0; i<nchannel; ++i)
    {
         if(isConnected[i]) {
               Status status = pvaClientGet[i]->waitGet();
               if(status.isOK()) continue;
               string message = string("channel ") +pvaClientChannelArray[i]->getChannelName() 
                    + " PvaChannelGet::waitGet " + status.getMessage();
               throw std::runtime_error(message);
         }
    }
    pvaClientNTMultiData->startDeltaTime();
    for(size_t i=0; i<nchannel; ++i)
    {
         if(isConnected[i]) {
              pvaClientNTMultiData->setPVStructure(pvaClientGet[i]->getData()->getPVStructure(),i);
         }
    }
    pvaClientNTMultiData->endDeltaTime();
}

PvaClientNTMultiDataPtr PvaClientNTMultiGet::getData()
{
    return pvaClientNTMultiData;
}

}}
