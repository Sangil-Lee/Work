/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * pvAccessCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#include <pv/baseChannelRequester.h>

using namespace epics::pvData;
using std::string;

namespace epics {
namespace pvAccess {

const Status BaseChannelRequester::okStatus = Status();
const Status BaseChannelRequester::badCIDStatus(Status::STATUSTYPE_ERROR, "bad channel id");
const Status BaseChannelRequester::badIOIDStatus(Status::STATUSTYPE_ERROR, "bad request id");
const Status BaseChannelRequester::noReadACLStatus(Status::STATUSTYPE_ERROR, "no read access");
const Status BaseChannelRequester::noWriteACLStatus(Status::STATUSTYPE_ERROR, "no write access");
const Status BaseChannelRequester::noProcessACLStatus(Status::STATUSTYPE_ERROR, "no process access");
const Status BaseChannelRequester::otherRequestPendingStatus(Status::STATUSTYPE_ERROR, "other request pending");
const Status BaseChannelRequester::notAChannelRequestStatus(Status::STATUSTYPE_ERROR, "not a channel request");

const int32 BaseChannelRequester::NULL_REQUEST = -1;

BaseChannelRequester::BaseChannelRequester(
    ServerContextImpl::shared_pointer const & context,
    ServerChannelImpl::shared_pointer const & channel,
    const pvAccessID ioid,
    Transport::shared_pointer const & transport) :
    _ioid(ioid),
    _transport(transport),
    _channel(channel),
    _context(context),
    _pendingRequest(BaseChannelRequester::NULL_REQUEST)
{

}

bool BaseChannelRequester::startRequest(int32 qos)
{
    Lock guard(_mutex);
    if (_pendingRequest != NULL_REQUEST)
    {
        return false;
    }
    _pendingRequest = qos;
    return true;
}

void BaseChannelRequester::stopRequest()
{
    Lock guard(_mutex);
    _pendingRequest = NULL_REQUEST;
}

int32 BaseChannelRequester::getPendingRequest()
{
    Lock guard(_mutex);
    return _pendingRequest;
}

string BaseChannelRequester::getRequesterName()
{
    std::stringstream name;
    name << typeid(*_transport).name() << "/" << _ioid;
    return name.str();
}

void BaseChannelRequester::message(std::string const & message, epics::pvData::MessageType messageType)
{
    BaseChannelRequester::message(_transport, _ioid, message, messageType);
}

void BaseChannelRequester::message(Transport::shared_pointer const & transport, const pvAccessID ioid, const string message, const MessageType messageType)
{
    TransportSender::shared_pointer sender(new BaseChannelRequesterMessageTransportSender(ioid, message, messageType));
    transport->enqueueSendRequest(sender);
}

void BaseChannelRequester::sendFailureMessage(const int8 command, Transport::shared_pointer const & transport, const pvAccessID ioid, const int8 qos, const Status status)
{
    TransportSender::shared_pointer sender(new BaseChannelRequesterFailureMessageTransportSender(command, transport, ioid, qos, status));
    transport->enqueueSendRequest(sender);
}

BaseChannelRequesterMessageTransportSender::BaseChannelRequesterMessageTransportSender(const pvAccessID ioid, const string message,const epics::pvData::MessageType messageType):
    _ioid(ioid),
    _message(message),
    _messageType(messageType)
{
}

void BaseChannelRequesterMessageTransportSender::send(ByteBuffer* buffer, TransportSendControl* control)
{
    control->startMessage((int8)18, sizeof(int32)/sizeof(int8) + 1);
    buffer->putInt(_ioid);
    buffer->putByte((int8)_messageType);
    epics::pvData::SerializeHelper::serializeString(_message, buffer, control);
}

void BaseChannelRequesterMessageTransportSender::lock()
{
    // noop
}

void BaseChannelRequesterMessageTransportSender::unlock()
{
    // noop
}

BaseChannelRequesterFailureMessageTransportSender::BaseChannelRequesterFailureMessageTransportSender(const int8 command,
        Transport::shared_pointer const & transport, const pvAccessID ioid, const int8 qos, const Status& status) :
    _command(command),
    _ioid(ioid),
    _qos(qos),
    _status(status),
    _transport(transport)
{
}

void BaseChannelRequesterFailureMessageTransportSender::send(ByteBuffer* buffer, TransportSendControl* control)
{
    control->startMessage(_command, sizeof(int32)/sizeof(int8) + 1);
    buffer->putInt(_ioid);
    buffer->put(_qos);
    _status.serialize(buffer, control);
}

void BaseChannelRequesterFailureMessageTransportSender::lock()
{
    // noop
}

void BaseChannelRequesterFailureMessageTransportSender::unlock()
{
    // noop
}


}
}
