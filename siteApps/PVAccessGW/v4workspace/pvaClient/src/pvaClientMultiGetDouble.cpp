/* pvaClientMultiGetDouble.cpp */
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


PvaClientMultiGetDoublePtr PvaClientMultiGetDouble::create(
    PvaClientMultiChannelPtr const &pvaMultiChannel,
    PvaClientChannelArray const &pvaClientChannelArray)
{
    PvaClientMultiGetDoublePtr pvaClientMultiGetDouble(
         new PvaClientMultiGetDouble(pvaMultiChannel,pvaClientChannelArray));
    return pvaClientMultiGetDouble;
}

PvaClientMultiGetDouble::PvaClientMultiGetDouble(
         PvaClientMultiChannelPtr const &pvaClientMultiChannel,
         PvaClientChannelArray const &pvaClientChannelArray)
: pvaClientMultiChannel(pvaClientMultiChannel),
  pvaClientChannelArray(pvaClientChannelArray),
  nchannel(pvaClientChannelArray.size()),
  doubleValue(shared_vector<double>(nchannel)),
  pvaClientGet(std::vector<PvaClientGetPtr>(nchannel,PvaClientGetPtr())),
  isGetConnected(false),
  isDestroyed(false)
{
}

PvaClientMultiGetDouble::~PvaClientMultiGetDouble()
{
    destroy();
}

void PvaClientMultiGetDouble::destroy()
{
    {
        Lock xx(mutex);
        if(isDestroyed) return;
        isDestroyed = true;
    }
    pvaClientChannelArray.clear();
}

void PvaClientMultiGetDouble::connect()
{
    shared_vector<epics::pvData::boolean> isConnected = pvaClientMultiChannel->getIsConnected();
    string request = "value";
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
               string message = string("channel ") + pvaClientChannelArray[i]->getChannelName()
                   + " PvaChannelGet::waitConnect " + status.getMessage();
               throw std::runtime_error(message);
         }
    }
    isGetConnected = true;
}

epics::pvData::shared_vector<double> PvaClientMultiGetDouble::get()
{
    if(!isGetConnected) connect();
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
               string message = string("channel ") + pvaClientChannelArray[i]->getChannelName()
                   + " PvaChannelGet::waitGet " + status.getMessage();
               throw std::runtime_error(message);
         }
    }
    
    for(size_t i=0; i<nchannel; ++i)
    {
        if(isConnected[i])
        {
            PVStructurePtr pvStructure = pvaClientGet[i]->getData()->getPVStructure();
            doubleValue[i] = convert->toDouble(pvStructure->getSubField<PVScalar>("value"));
        } else {
            doubleValue[i] = epicsNAN;
        }
    }
    return doubleValue;
}

}}
