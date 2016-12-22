/* pvaClient.cpp */
/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 * @date 2015.02
 */

#include <map>
#include <pv/createRequest.h>
#include <pv/clientFactory.h>
#include <pv/caProvider.h>

#define epicsExportSharedSymbols

#include <pv/pvaClient.h>

using std::tr1::static_pointer_cast;
using namespace epics::pvData;
using namespace epics::pvAccess;
using namespace epics::pvAccess::ca;
using namespace std;

namespace epics { namespace pvaClient { 

static FieldCreatePtr fieldCreate = getFieldCreate(); 
static const string pvaClientName = "pvaClient";
static const string defaultProvider = "pva";
static UnionConstPtr variantUnion = fieldCreate->createVariantUnion();

namespace pvaClientPvt {

    static size_t numberPvaClient = 0;
    static bool firstTime = true;
    static Mutex mutex;
    
    class StartStopClientFactory {
    public:
        static void PvaClientBeingConstructed()
        {
            bool saveFirst = false;
            { 
                 Lock xx(mutex);
                 ++numberPvaClient;
                 saveFirst = firstTime;
                 firstTime = false;
            }
            if(saveFirst) {
                ClientFactory::start();
                CAClientFactory::start();
            }
        }
    
        static void PvaClientBeingDestroyed() {
            size_t numLeft = 0;
            {
                 Lock xx(mutex);
                 --numberPvaClient;
                  numLeft = numberPvaClient;
            }
            if(numLeft<=0) {
                ClientFactory::stop();
                CAClientFactory::stop();
            }
        }
    };

} // namespace pvaClientPvt

class PvaClientChannelCache
{
public:
    PvaClientChannelCache(){}
    ~PvaClientChannelCache(){
         destroy();
     }
    void destroy() {
       pvaClientChannelMap.clear();
    }
    PvaClientChannelPtr getChannel(
        string const & channelName,
        string const & providerName);
    void addChannel(PvaClientChannelPtr const & pvaClientChannel);
    void removeChannel(string const & channelName,string const & providerName);
    void showCache();
    size_t cacheSize();
private:
    map<string,PvaClientChannelPtr> pvaClientChannelMap;
};
   
PvaClientChannelPtr PvaClientChannelCache::getChannel(
    string const & channelName,
    string const & providerName)
{
    string name = channelName + providerName;
    map<string,PvaClientChannelPtr>::iterator iter = pvaClientChannelMap.find(name);
    if(iter!=pvaClientChannelMap.end()) return iter->second;
    return PvaClientChannelPtr();
}

void PvaClientChannelCache::addChannel(PvaClientChannelPtr const & pvaClientChannel)
{
     Channel::shared_pointer channel = pvaClientChannel->getChannel();
     string name = channel->getChannelName()
         + channel->getProvider()->getProviderName();
     pvaClientChannelMap.insert(std::pair<string,PvaClientChannelPtr>(
         name,pvaClientChannel));
}

void PvaClientChannelCache::removeChannel(
        string const & channelName,
        string const & providerName)
{
    string name = channelName + providerName;
    map<string,PvaClientChannelPtr>::iterator iter = pvaClientChannelMap.find(name);
    if(iter!=pvaClientChannelMap.end()) pvaClientChannelMap.erase(iter);
}

void PvaClientChannelCache::showCache()
{
    map<string,PvaClientChannelPtr>::iterator iter;
    for(iter = pvaClientChannelMap.begin(); iter != pvaClientChannelMap.end(); ++iter)
    {
         PvaClientChannelPtr pvaChannel = iter->second;
         Channel::shared_pointer channel = pvaChannel->getChannel();
         string channelName = channel->getChannelName();
         string providerName = channel->getProvider()->getProviderName();
         cout << "channel " << channelName << " provider " << providerName << endl;
         cout << "  get and put cacheSize " << pvaChannel->cacheSize() << endl;
         pvaChannel->showCache();
    }
    
}

size_t PvaClientChannelCache::cacheSize()
{
    return pvaClientChannelMap.size();

}

using namespace epics::pvaClient::pvaClientPvt;

PvaClientPtr PvaClient::create()
{
    PvaClientPtr xx(new PvaClient());
    StartStopClientFactory::PvaClientBeingConstructed();
    return xx;
}


PvaClient::PvaClient()
:   pvaClientChannelCache(new PvaClientChannelCache()),
    isDestroyed(false)
{
}

PvaClient::~PvaClient() {
    destroy();
}

void PvaClient::destroy()
{
    {
        Lock xx(mutex);
        if(isDestroyed) return;
        isDestroyed = true;
    }
    pvaClientChannelCache.reset();
    StartStopClientFactory::PvaClientBeingDestroyed();
}

string PvaClient:: getRequesterName()
{
    static string name("pvaClient");
    RequesterPtr req = requester.lock();
    if(req) {
         return req->getRequesterName();
    }
    return name;
}

void  PvaClient::message(
        string const & message,
        MessageType messageType)
{
    RequesterPtr req = requester.lock();
    if(req) {
         req->message(message,messageType);
         return;
    }
    cout << getMessageTypeName(messageType) << " " << message << endl;
}

PvaClientChannelPtr PvaClient::channel(
        std::string const & channelName,
        std::string const & providerName,
        double timeOut)
{
    PvaClientChannelPtr pvaClientChannel = 
        pvaClientChannelCache->getChannel(channelName,providerName);
    if(pvaClientChannel) return pvaClientChannel;
    pvaClientChannel = createChannel(channelName,providerName);
    pvaClientChannel->connect(timeOut);
    pvaClientChannelCache->addChannel(pvaClientChannel);
    return pvaClientChannel;
}

PvaClientChannelPtr PvaClient::createChannel(string const & channelName, string const & providerName)
{
     return PvaClientChannel::create(getPtrSelf(),channelName,providerName);
}

void PvaClient::setRequester(RequesterPtr const & requester)
{
    this->requester = requester;
}

void PvaClient::clearRequester()
{
    requester = Requester::weak_pointer();
}

void PvaClient::showCache()
{
    pvaClientChannelCache->showCache();
}


size_t PvaClient::cacheSize()
{
    return pvaClientChannelCache->cacheSize();
}

}}

