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

DbGroup::DbGroup(
    DbGroupProviderPtr const & provider,
    ChannelRequester::shared_pointer const & requester)
: ChannelBase(provider,requester,provider->channelName),
  provider(provider),
  requester(requester),
  arrayPvValue(new pvValuePtrArray())
{}

bool DbGroup::create()
{
    size_t n = provider->fieldNames->size();
    arrayPvValue->reserve(n);
    for(size_t i=0; i<n; i++) {
        ValueChannelPtr valueChannel(
            new PvValue(
                 requester,
                 provider->pvValueProvider,
                 (*provider->pvValueNames)[i]));
        arrayPvValue->push_back(valueChannel);
    }
    for(size_t i=0; i<n; i++) {
        (*arrayPvValue)[i]->connect();
    }
    bool allOK = true;
    for(size_t i=0; i<n; i++) {
        Status status = (*arrayPvValue)[i]->waitConnect();
        if(!status.isOK()) {
             allOK = false;
        }
    }
    if(!allOK) {
        destroy();
        return false;
    }
    FieldConstPtrArray fields;
    fields.reserve(n+2);
    StringArray fieldNames;
    fieldNames.reserve(n+2);
    fields.push_back(standardField->alarm());
    fieldNames.push_back("alarm");
    fields.push_back(standardField->timeStamp());
    fieldNames.push_back("timeStamp");
    for(size_t i=0; i<n; i++) {
        FieldConstPtr valueField =
            (*arrayPvValue)[i]->getValue()->getField();
        fields.push_back(valueField);
        fieldNames.push_back((*provider->fieldNames)[i]);
    }
    structure = fieldCreate->createStructure(fieldNames,fields);
    return allOK;
}

void DbGroup::destroy()
{
    size_t n = arrayPvValue->size();
    for(size_t i=0; i<n; i++) {
       (*arrayPvValue)[i]->destroy();
    }
    arrayPvValue.reset();
}

void DbGroup::getField(
    GetFieldRequester::shared_pointer const &requester,
    string const &subField)
{
    // for now just return structure
    requester->getDone(Status::Ok,structure);
}

ChannelGet::shared_pointer DbGroup::createChannelGet(
    ChannelGetRequester::shared_pointer const &channelGetRequester,
    PVStructure::shared_pointer const &pvRequest)
{
    DbGroupPtr xxx =
        static_pointer_cast<DbGroup>(getPtrSelf());
    DbGroupGetPtr channelGet(
        new DbGroupGet(xxx,channelGetRequester));
    bool result = channelGet->init(pvRequest);
    if(!result) channelGet.reset();
    return channelGet;
}

}}
