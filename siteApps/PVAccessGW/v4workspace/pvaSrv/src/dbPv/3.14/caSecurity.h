/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * pvAccessCPP is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */

#ifndef CASECURITY_H
#define CASECURITY_H

#ifdef epicsExportSharedSymbols
#   define casecurityEpicsExportSharedSymbols
#   undef epicsExportSharedSymbols
#endif

#include <string>

#include <asLib.h>

#include <pv/pvData.h>
#include <pv/security.h>


#ifdef casecurityEpicsExportSharedSymbols
#   define epicsExportSharedSymbols
#	undef casecurityEpicsExportSharedSymbols
#endif

#include <shareLib.h>

namespace epics {
    namespace pvaSrv {


    class CAServerChannelSecuritySession :
        public epics::pvAccess::ChannelSecuritySession
    {
    public:
        POINTER_DEFINITIONS(CAServerChannelSecuritySession);

        CAServerChannelSecuritySession(std::string const & channelName,
                                       const char * user,
                                       char * host)
            throw (epics::pvAccess::SecurityException);

        virtual ~CAServerChannelSecuritySession();

        /// closes this session
        virtual void close();

        // for every authroizeCreate... a release() must be called
        virtual void release(epics::pvAccess::pvAccessID ioid) {
            // noop
        }

        // bitSet w/ one bit
        virtual epics::pvData::Status authorizeCreateChannelProcess(
                epics::pvAccess::pvAccessID ioid, epics::pvData::PVStructure::shared_pointer const & /*pvRequest*/) {
            return epics::pvData::Status::Ok;
        }

        virtual epics::pvData::Status authorizeProcess(epics::pvAccess::pvAccessID /*ioid*/) {
            return epics::pvData::Status::Ok;
        }

        // bitSet w/ one bit (allowed, not allowed) and rest of the bit per field
        virtual epics::pvData::Status authorizeCreateChannelGet(
                epics::pvAccess::pvAccessID /*ioid*/, epics::pvData::PVStructure::shared_pointer const & /*pvRequest*/) {
            return epics::pvData::Status::Ok;
        }

        virtual epics::pvData::Status authorizeGet(epics::pvAccess::pvAccessID /*ioid*/) {
            if (!asCheckGet(m_asClientPvt))
                return m_noAccessStatus;
            else
                return epics::pvData::Status::Ok;
        }

        // read: bitSet w/ one bit (allowed, not allowed) and rest of the bit per field
        // write: bitSet w/ one bit (allowed, not allowed) and rest of the bit per field
        virtual epics::pvData::Status authorizeCreateChannelPut(
                epics::pvAccess::pvAccessID /*ioid*/, epics::pvData::PVStructure::shared_pointer const & /*pvRequest*/) {
            return epics::pvData::Status::Ok;
        }

        virtual epics::pvData::Status authorizePut(
                epics::pvAccess::pvAccessID /*ioid*/,
                epics::pvData::PVStructure::shared_pointer const & /*dataToPut*/,
                epics::pvData::BitSet::shared_pointer const & /*fieldsToPut*/) {
            if (!asCheckPut(m_asClientPvt))
                return m_noAccessStatus;
            else
                return epics::pvData::Status::Ok;
        }

        // read: bitSet w/ one bit (allowed, not allowed) and rest of the bit per field
        // write: bitSet w/ one bit (allowed, not allowed) and rest of the bit per field
        // process: bitSet w/ one bit (allowed, not allowed)
        virtual epics::pvData::Status authorizeCreateChannelPutGet(
                epics::pvAccess::pvAccessID /*ioid*/, epics::pvData::PVStructure::shared_pointer const & /*pvRequest*/) {
            return epics::pvData::Status::Ok;
        }

        virtual epics::pvData::Status authorizePutGet(
                epics::pvAccess::pvAccessID /*ioid*/,
                epics::pvData::PVStructure::shared_pointer const & /*dataToPut*/,
                epics::pvData::BitSet::shared_pointer const & /*fieldsToPut*/) {
            if (!asCheckGet(m_asClientPvt) || !asCheckPut(m_asClientPvt))
                return m_noAccessStatus;
            else
                return epics::pvData::Status::Ok;
        }

        // bitSet w/ one bit
        virtual epics::pvData::Status authorizeCreateChannelRPC(
                epics::pvAccess::pvAccessID /*ioid*/, epics::pvData::PVStructure::shared_pointer const & /*pvRequest*/) {
            return epics::pvData::Status::Ok;
        }

        // one could authorize per operation basis
        virtual epics::pvData::Status authorizeRPC(
                epics::pvAccess::pvAccessID /*ioid*/, epics::pvData::PVStructure::shared_pointer const & /*arguments*/) {
            return epics::pvData::Status::Ok;
        }

        // read: bitSet w/ one bit (allowed, not allowed) and rest of the bit per field
        virtual epics::pvData::Status authorizeCreateMonitor(
                epics::pvAccess::pvAccessID /*ioid*/, epics::pvData::PVStructure::shared_pointer const & /*pvRequest*/) {
            return epics::pvData::Status::Ok;
        }

        virtual epics::pvData::Status authorizeMonitor(epics::pvAccess::pvAccessID /*ioid*/) {
            if (!asCheckGet(m_asClientPvt))
                return m_noAccessStatus;
            else
                return epics::pvData::Status::Ok;
        }

        // read: bitSet w/ one bit (allowed, not allowed) and rest put/get/set length
        virtual epics::pvData::Status authorizeCreateChannelArray(
                epics::pvAccess::pvAccessID /*ioid*/, epics::pvData::PVStructure::shared_pointer const & /*pvRequest*/) {
            return epics::pvData::Status::Ok;
        }

        // use authorizeGet
        virtual epics::pvData::Status authorizePut(
                epics::pvAccess::pvAccessID /*ioid*/, epics::pvData::PVArray::shared_pointer const & /*dataToPut*/) {
            if (!asCheckPut(m_asClientPvt))
                return m_noAccessStatus;
            else
                return epics::pvData::Status::Ok;
        }

        virtual epics::pvData::Status authorizeSetLength(epics::pvAccess::pvAccessID /*ioid*/) {
            return epics::pvData::Status::Ok;
        }

        // introspection authorization
        virtual epics::pvData::Status authorizeGetField(epics::pvAccess::pvAccessID /*ioid*/, std::string const & /*subField*/) {
            return epics::pvData::Status::Ok;
        }


    private:

        static epics::pvData::Status m_noAccessStatus;

        ASCLIENTPVT m_asClientPvt;
    };


    struct NoChannelException : public epics::pvAccess::SecurityException
    {
        NoChannelException() : SecurityException("No such channel") {}
    };

    class epicsShareClass CAServerSecuritySession :
        public epics::pvAccess::SecuritySession
    {
    public:
        POINTER_DEFINITIONS(CAServerSecuritySession);

        CAServerSecuritySession(epics::pvAccess::SecurityPlugin::shared_pointer const & parent,
                                std::string const & user,
                                std::string const & host) :
            m_parent(parent),
            m_user(user)
        {
            strncpy(m_host, host.c_str(), 256-1);
        }

        virtual ~CAServerSecuritySession() {}

        // optional (can be null) initialization data for the remote party
        // client to server
        virtual epics::pvData::PVField::shared_pointer initializationData() {
            return epics::pvData::PVField::shared_pointer();
        }

        // get parent
        virtual std::tr1::shared_ptr<epics::pvAccess::SecurityPlugin> getSecurityPlugin() {
            return m_parent;
        }

        // can be called any time, for any reason
        virtual void messageReceived(epics::pvData::PVField::shared_pointer const & data) {
            // noop
        }

        /// closes this session
        virtual void close() {
            m_parent.reset();
        }

        // notification to the client on allowed requests (bitSet, a bit per request)
        virtual epics::pvAccess::ChannelSecuritySession::shared_pointer createChannelSession(std::string const & channelName)
            throw (epics::pvAccess::SecurityException)
        {
            try
            {
                return epics::pvAccess::ChannelSecuritySession::shared_pointer(
                            new CAServerChannelSecuritySession(channelName, m_user.c_str(), m_host)
                            );
            } catch (NoChannelException &nce) {
                // allow channels that do not live in db (e.g. a server hosts 2 providers)
                // TODO think about this, it is better to split servers
                // additional case: server RPC service (channelName == "server")
                return epics::pvAccess::NoSecurityPlugin::INSTANCE->createChannelSession(channelName);
            }
        }

    private:
        epics::pvAccess::SecurityPlugin::shared_pointer m_parent;
        std::string m_user;
        char m_host[256];
    };




    class epicsShareClass CAServerSecurityPlugin :
            public epics::pvAccess::SecurityPlugin,
            public std::tr1::enable_shared_from_this<CAServerSecurityPlugin>
    {
    public:
        POINTER_DEFINITIONS(CAServerSecurityPlugin);

        virtual ~CAServerSecurityPlugin() {}

        virtual std::string getId() const {
            return "ca";
        }

        virtual std::string getDescription() const {
            return "CA server security plug-in";
        }

        virtual bool isValidFor(osiSockAddr const & /*remoteAddress*/) const {
            return true;
        }

        virtual epics::pvAccess::SecuritySession::shared_pointer createSession(
                osiSockAddr const & /*remoteAddress*/,
                epics::pvAccess::SecurityPluginControl::shared_pointer const & control,
                epics::pvData::PVField::shared_pointer const & data) throw (epics::pvAccess::SecurityException);

    };


    }
}

#endif // CASECURITY_H
