/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
/* Marty Kraimer 2011.03 
 * This creates a CA context the first time a thread calls
 * caContextCreate::create and then calls ca_attach_context
 * from checkContext if the caller is a thread that is not the
 * thread that called caContextCreate,create.
 */

#ifndef CACONTEXT_H
#define CACONTEXT_H

#include <map>
#include <list>

#include <epicsThread.h>

#include <pv/lock.h>
#include <pv/requester.h>
#include <pv/sharedPtr.h>

class caContext;
typedef std::tr1::shared_ptr<caContext> caContextPtr;
class caContextCreate;

class caContext : public std::tr1::enable_shared_from_this<caContext> {
public:
    POINTER_DEFINITIONS(caContext);
    ~caContext();
    void release();
    void stop();
    void exception(std::string const &message);
    void checkContext();
private:
    std::list<epicsThreadId> auxThreadList ;
    epics::pvData::Mutex mutex;
    caContext(
       epics::pvData::RequesterPtr const & requester);
    epics::pvData::Requester::weak_pointer requester;
    epicsThreadId threadId;
    struct ca_client_context *context;
    int referenceCount;
    friend class caContextCreate;
};

class caContextCreate {
public:
    static caContextPtr get(epics::pvData::RequesterPtr const & requester);
private:
    static void erase(epicsThreadId id);
    static std::map<epicsThreadId,caContextPtr> contextMap;
    static epics::pvData::Mutex mutex;
    friend class caContext;
};

#endif  /* CACONTEXT_H */
