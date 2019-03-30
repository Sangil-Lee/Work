/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
/* Marty Kraimer 2011.03
 */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <algorithm>

#include <epicsExit.h>
#include <cadef.h>
#include <db_access.h>
#include <dbDefs.h>
#include <errlog.h>

#include "caContext.h"
#include "dbPvDebug.h"

using namespace epics::pvData;
using namespace epics::pvaSrv;
using std::string;

extern "C" {

static void exceptionCallback(struct exception_handler_args args)
{
    if(DbPvDebug::getLevel()>0) printf("caContext::exceptionCallback\n");
    caContext *context = static_cast<caContext *>(args.usr);
    string message(ca_message(args.stat));
    context->exception(message);
}

static void threadExitFunc(void *arg)
{
    if(DbPvDebug::getLevel()>0) printf("caContext::threadExitFunc\n");
    caContext * context = static_cast<caContext * >(arg);
    std::tr1::shared_ptr<caContext> self(context->shared_from_this());
    self->stop();
}

} // extern "C"

caContext::caContext(RequesterPtr const & requester)
:
  requester(requester),
  threadId(epicsThreadGetIdSelf()),
  context(0),
  referenceCount(0)
{
    if(DbPvDebug::getLevel()>0) printf("caContext::caContext\n");
    SEVCHK(ca_context_create(ca_enable_preemptive_callback),
        "caContext::caContext calling ca_context_create");
    int status = ca_add_exception_event(exceptionCallback,this);
    if(status!=ECA_NORMAL) {
        requester->message("ca_add_exception_event failed",warningMessage);
    }
    context = ca_current_context();
    epicsAtThreadExit(threadExitFunc,this);
}

caContext::~caContext()
{
    if(DbPvDebug::getLevel()>0) printf("caContext::~caContext\n");
}

// TODO commented out exceptions to avoid SIGSEGs (to reproduce: pvget -m counter01 and then CTRL+C the pvget)
void caContext::stop()
{
    if(DbPvDebug::getLevel()>0) printf("caContext::stop\n");
    epicsThreadId id = epicsThreadGetIdSelf();
    if(id!=threadId) {
        printf("caContext::stop not same thread\n");
        return;
    }
    {
        Lock xx(mutex);
        if(referenceCount!=0) {
            printf("caContext::stop referenceCount != 0 value %d\n",
                   referenceCount);
            return;
        }
    }
    caContextCreate::erase(threadId);
    ca_context_destroy();
}

typedef std::list<epicsThreadId>::iterator threadListIter;

void caContext::checkContext()
{
    if(DbPvDebug::getLevel()>0) printf("caContext::checkContext\n");
    epicsThreadId id = epicsThreadGetIdSelf();
    if(id==threadId) return;
    Lock xx(mutex);
    threadListIter iter = std::find(
       auxThreadList.begin(),auxThreadList.end(),id);
    if(iter!=auxThreadList.end()) return;
    auxThreadList.push_front(id);
    SEVCHK(ca_attach_context(context),
        "caContext::checkContext calling ca_context_create");
}

void caContext::release()
{
    if(DbPvDebug::getLevel()>0) printf("caContext::release referenceCount %d\n",referenceCount);
    Lock xx(mutex);
    referenceCount--;
}

void caContext::exception(string const &message)
{
    Lock xx(mutex);
    epics::pvData::RequesterPtr req(requester.lock());
    if(req) req->message(message,errorMessage);
    else errlogPrintf("caContext exception on dead PVA channel: %s\n", message.c_str());
}

typedef std::map<epicsThreadId,caContextPtr>::iterator contextMapIter;

std::map<epicsThreadId,caContextPtr>caContextCreate::contextMap;
Mutex caContextCreate::mutex;

caContextPtr caContextCreate::get(RequesterPtr const &requester)
{
    if(DbPvDebug::getLevel()>0) printf("caContext::get\n");
    Lock xx(mutex);
    epicsThreadId id = epicsThreadGetIdSelf();
    contextMapIter iter = contextMap.find(id);
    if(iter!=contextMap.end()) {
        caContextPtr context = iter->second;
        context->referenceCount++;
        return context;
    }
    caContextPtr context(new caContext(requester));
    contextMap[id] = context;
    context->referenceCount++;
    return context;
}

void caContextCreate::erase(epicsThreadId threadId)
{
    if(DbPvDebug::getLevel()>0) printf("caContext::erase\n");
    Lock xx(mutex);
    contextMap.erase(threadId);
}
