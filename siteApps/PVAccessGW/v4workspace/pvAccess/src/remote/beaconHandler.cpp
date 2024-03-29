/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * pvAccessCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/beaconHandler.h>
#include <pv/transportRegistry.h>

using namespace std;
using namespace epics::pvData;
using namespace epics::pvAccess;

namespace epics {
namespace pvAccess {

BeaconHandler::BeaconHandler(Context::shared_pointer const & context,
                             std::string const & protocol,
                             const osiSockAddr* responseFrom) :
    _context(Context::weak_pointer(context)),
    _protocol(protocol),
    _responseFrom(*responseFrom),
    _mutex(),
    _serverGUID(),
    _serverChangeCount(-1),
    _first(true)
{

}

BeaconHandler::~BeaconHandler()
{
}

void BeaconHandler::beaconNotify(osiSockAddr* /*from*/, int8 remoteTransportRevision,
                                 TimeStamp* timestamp, GUID const & guid, int16 sequentalID,
                                 int16 changeCount,
                                 PVFieldPtr /*data*/)
{
    bool networkChanged = updateBeacon(remoteTransportRevision, timestamp, guid, sequentalID, changeCount);
    if (networkChanged)
        changedTransport();
}

bool BeaconHandler::updateBeacon(int8 /*remoteTransportRevision*/, TimeStamp* /*timestamp*/,
                                 GUID const & guid, int16 /*sequentalID*/, int16 changeCount)
{
    Lock guard(_mutex);
    // first beacon notification check
    if (_first)
    {
        _first = false;
        _serverGUID = guid;
        _serverChangeCount = changeCount;

        // new server up..
        _context.lock()->newServerDetected();

        return false;
    }

    bool networkChange = (memcmp(_serverGUID.value, guid.value, sizeof(guid.value)) != 0);
    if (networkChange)
    {
        // update startup time and change count
        _serverGUID = guid;
        _serverChangeCount = changeCount;

        _context.lock()->newServerDetected();

        return true;
    }
    else if (_serverChangeCount != changeCount)
    {
        // update change count
        _serverChangeCount = changeCount;

        // TODO be more specific (possible optimizations)
        _context.lock()->newServerDetected();

        return true;
    }

    return false;
}

void BeaconHandler::changedTransport()
{
    auto_ptr<TransportRegistry::transportVector_t> transports =
        _context.lock()->getTransportRegistry()->get(_protocol, &_responseFrom);
    if (!transports.get())
        return;

    // notify all
    for (TransportRegistry::transportVector_t::iterator iter = transports->begin();
            iter != transports->end();
            iter++)
    {
        (*iter)->changedTransport();
    }
}

}
}

