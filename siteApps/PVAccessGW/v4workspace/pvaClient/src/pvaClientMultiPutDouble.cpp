/* pvaClientMultiPutDouble.cpp */
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
static CreateRequest::shared_pointer  createRequest = CreateRequest::create();


PvaClientMultiPutDoublePtr PvaClientMultiPutDouble::create(
    PvaClientMultiChannelPtr const &pvaMultiChannel,
    PvaClientChannelArray const &pvaClientChannelArray)
{
    PvaClientMultiPutDoublePtr pvaClientMultiPutDouble(
         new PvaClientMultiPutDouble(pvaMultiChannel,pvaClientChannelArray));
    return pvaClientMultiPutDouble;
}

PvaClientMultiPutDouble::PvaClientMultiPutDouble(
     PvaClientMultiChannelPtr const &pvaClientMultiChannel,
     PvaClientChannelArray const &pvaClientChannelArray)
: pvaClientMultiChannel(pvaClientMultiChannel),
  pvaClientChannelArray(pvaClientChannelArray),
  nchannel(pvaClientChannelArray.size()),
  pvaClientPut(std::vector<PvaClientPutPtr>(nchannel,PvaClientPutPtr())),
  isPutConnected(false),
  isDestroyed(false)
{
}



PvaClientMultiPutDouble::~PvaClientMultiPutDouble()
{
    destroy();
}

void PvaClientMultiPutDouble::destroy()
{
    {
        Lock xx(mutex);
        if(isDestroyed) return;
        isDestroyed = true;
    }
    pvaClientChannelArray.clear();
}

void PvaClientMultiPutDouble::connect()
{
    shared_vector<epics::pvData::boolean> isConnected = pvaClientMultiChannel->getIsConnected();
    for(size_t i=0; i<nchannel; ++i)
    {
         if(isConnected[i]) {
               pvaClientPut[i] = pvaClientChannelArray[i]->createPut();
               pvaClientPut[i]->issueConnect();
         }
    }
    for(size_t i=0; i<nchannel; ++i)
    {
         if(isConnected[i]) {
               Status status = pvaClientPut[i]->waitConnect();
               if(status.isOK()) continue;
               string message = string("channel ") + pvaClientChannelArray[i]->getChannelName()
                   + " PvaChannelPut::waitConnect " + status.getMessage();
               throw std::runtime_error(message);
         }
    }
    isPutConnected = true;
}

void PvaClientMultiPutDouble::put(epics::pvData::shared_vector<double> const &data)
{
    if(!isPutConnected) connect();
    if(data.size()!=nchannel) {
         throw std::runtime_error("data has wrong size");
    }
    shared_vector<epics::pvData::boolean> isConnected = pvaClientMultiChannel->getIsConnected();
    for(size_t i=0; i<nchannel; ++i)
    {
         if(isConnected[i]) {
               PVStructurePtr pvTop = pvaClientPut[i]->getData()->getPVStructure();
               PVScalarPtr pvValue = pvTop->getSubField<PVScalar>("value");
               convert->fromDouble(pvValue,data[i]);
               pvaClientPut[i]->issuePut();
         }
         if(isConnected[i]) {
              Status status = pvaClientPut[i]->waitPut();
              if(status.isOK())  continue;
              string message = string("channel ") + pvaClientChannelArray[i]->getChannelName()
                   + " PvaChannelPut::waitPut " + status.getMessage();
              throw std::runtime_error(message);
         }
    }
}

}}
