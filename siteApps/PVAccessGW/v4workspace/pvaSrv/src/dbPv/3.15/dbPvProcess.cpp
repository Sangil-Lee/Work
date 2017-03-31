/**
 * Copyright - See the COPYRIGHT that is included with this distribution.
 * EPICS pvData is distributed subject to a Software License Agreement found
 * in file LICENSE that is included with this distribution.
 */
/**
 * @author mrk
 */
/* Marty Kraimer 2011.03 */
/* This connects to a DB record and presents the data as a PVStructure
 * It provides access to  value, alarm, display, and control.
 */

#include <cstddef>
#include <cstdlib>
#include <cstddef>
#include <string>
#include <cstdio>
#include <stdexcept>
#include <memory>

#include <pv/pvData.h>
#include <pv/pvAccess.h>

#include "dbPv.h"
#include "dbUtil.h"

using namespace epics::pvData;
using namespace epics::pvAccess;
using std::string;

namespace epics { namespace pvaSrv { 

DbPvProcess::DbPvProcess(
    DbPvPtr const &dbPv,
    ChannelProcessRequester::shared_pointer const &channelProcessRequester)
: dbUtil(DbUtil::getDbUtil()),
  dbPv(dbPv),
  channelProcessRequester(channelProcessRequester),
  recordString("record"),
  processString("process"),
  fieldString("field"),
  fieldListString("fieldList"),
  valueString("value"),
  beingDestroyed(false)
{
    if(DbPvDebug::getLevel()>0) printf("dbPvProcess::dbPvProcess\n");
}

DbPvProcess::~DbPvProcess()
{
    if(DbPvDebug::getLevel()>0) printf("dbPvProcess::~dbPvProcess\n");
}

bool DbPvProcess::init(epics::pvData::PVStructurePtr const & pvRequest)
{
    propertyMask = dbUtil->getProperties(
                channelProcessRequester,
                pvRequest,
                dbPv->getDbChannel(),
                true);
    if (propertyMask == dbUtil->noAccessBit) return false;

    pNotify.reset(new (struct processNotify)());
    struct processNotify *pn = pNotify.get();
    pn->chan = dbPv->getDbChannel();
    pn->requestType = processRequest;
    pn->doneCallback = this->notifyCallback;
    pn->usrPvt = this;
    if (propertyMask & dbUtil->blockBit) block = true;
    channelProcessRequester->channelProcessConnect(Status::Ok, getPtrSelf());
    return true;
}

string DbPvProcess::getRequesterName() {
    return channelProcessRequester->getRequesterName();
}

void DbPvProcess::message(string const &message,MessageType messageType)
{
    channelProcessRequester->message(message,messageType);
}

void DbPvProcess::destroy() {
    if(DbPvDebug::getLevel()>0) printf("dbPvProcess::destroy beingDestroyed %s\n",
         (beingDestroyed ? "true" : "false"));
    {
        Lock xx(mutex);
        if (beingDestroyed) return;
        beingDestroyed = true;
        if (pNotify) dbNotifyCancel(pNotify.get());
    }
}

void DbPvProcess::process()
{
    if (block) {
        dbProcessNotify(pNotify.get());
    } else {
        dbScanLock(dbChannelRecord(dbPv->getDbChannel()));
        dbProcess(dbChannelRecord(dbPv->getDbChannel()));
        dbScanUnlock(dbChannelRecord(dbPv->getDbChannel()));
    }
}

void DbPvProcess::notifyCallback(struct processNotify *pn)
{
    DbPvProcess * pdp = static_cast<DbPvProcess *>(pn->usrPvt);
    pdp->channelProcessRequester->processDone(Status::Ok, pdp->getPtrSelf());
}

void DbPvProcess::lock()
{}

void DbPvProcess::unlock()
{}

}}
