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
using namespace epics::pvIOC;
using std::tr1::static_pointer_cast;

static ChannelProviderLocalPtr localProvider;
static ChannelFind::shared_pointer noChannelFind;
static Channel::shared_pointer noChannel;

static string dbGroupProvider("dbGroup");

DbGroupProvider::~DbGroupProvider() {}

DbGroupProvider::DbGroupProvider(
     RequesterPtr const & requester,
     ChannelProvider::shared_pointer const &pvValueProvider,
     string const &channelName,
     StringArrayPtr const & fieldNames,
     StringArrayPtr const & pvValueNames)
: ChannelBaseProvider(dbGroupProvider),
  requester(requester),
  pvValueProvider(pvValueProvider),
  channelName(channelName),
  fieldNames(fieldNames),
  pvValueNames(pvValueNames)
{}

void DbGroupProvider::dump()
{
    printf("dbGroupProvider channelName %s\n", channelName.c_str());
    printf("channelName %s\n", channelName.c_str());
    size_t n = fieldNames->size();
    for(size_t i=0; i<n; i++) {
    string fieldName = (*fieldNames.get())[i];
    string pvValueName = (*pvValueNames.get())[i];
    printf("fieldName %s pvValueName %s\n",
    fieldName.c_str(),pvValueName.c_str());
    }
}

void DbGroupProvider::destroy()
{}

ChannelFind::shared_pointer DbGroupProvider::channelFind(
    string const & channelName,
    ChannelFindRequester::shared_pointer const & channelFindRequester)
{
    bool result =
        ((channelName.compare(this->channelName)==0) ? true : false);
    channelFound(result,channelFindRequester);
    return ChannelFind::shared_pointer();
}

Channel::shared_pointer DbGroupProvider::createChannel(
    string const & channelName,
    ChannelRequester::shared_pointer  const & channelRequester,
    short priority,
    string const & address)
{
    if(!(channelName.compare(this->channelName)==0) ? true : false) {
        channelNotCreated(channelRequester);
        return noChannel;
    }
    DbGroupProviderPtr xxx =
        static_pointer_cast<DbGroupProvider>(getPtrSelf());
    DbGroup::shared_pointer channel(
        new DbGroup(xxx,channelRequester));
    if(!channel->create()) {
        channelNotCreated(channelRequester);
        return noChannel;
    } 
    channelCreated(channel);
    return channel;
}

}}
