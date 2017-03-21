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

DbPvGet::DbPvGet(
    DbPvPtr const &dbPv,
    ChannelGetRequester::shared_pointer const &channelGetRequester)
: dbUtil(DbUtil::getDbUtil()),
  dbPv(dbPv),
  channelGetRequester(channelGetRequester),
  process(false),
  block(false),
  firstTime(true),
  propertyMask(0),
  beingDestroyed(false)
{
    if(DbPvDebug::getLevel()>0)printf("dbPvGet::dbPvGet\n");
}

DbPvGet::~DbPvGet()
{
    if(DbPvDebug::getLevel()>0)printf("dbPvGet::~dbPvGet\n");
}

bool DbPvGet::init(PVStructure::shared_pointer const &pvRequest)
{
    propertyMask = dbUtil->getProperties(
        channelGetRequester,
        pvRequest,
        dbPv->getDbChannel(),
        false);
    if (propertyMask == dbUtil->noAccessBit) return false;
    pvStructure = PVStructure::shared_pointer(
                dbUtil->createPVStructure(
                    channelGetRequester,
                    propertyMask,
                    dbPv->getDbChannel()));
    if (!pvStructure.get()) return false;
    dbUtil->getPropertyData(channelGetRequester,
                            propertyMask,
                            dbPv->getDbChannel(),
                            pvStructure);
    int numFields = pvStructure->getNumberFields();
    bitSet.reset(new BitSet(numFields));
    if (propertyMask & dbUtil->processBit) {
        process = true;
        pNotify.reset(new (struct processNotify)());
        struct processNotify *pn = pNotify.get();
        pn->chan = dbPv->getDbChannel();
        pn->requestType = processGetRequest;
        pn->getCallback  = this->getCallback;
        pn->doneCallback = this->doneCallback;
        pn->usrPvt = this;
        if (propertyMask & dbUtil->blockBit) block = true;
    }
    channelGetRequester->channelGetConnect(
                Status::Ok,
                getPtrSelf(),
                pvStructure->getStructure());
    return true;
}

string DbPvGet::getRequesterName() {
    return channelGetRequester->getRequesterName();
}

void DbPvGet::message(string const &message,MessageType messageType)
{
    channelGetRequester->message(message,messageType);
}

void DbPvGet::destroy() {
    if(DbPvDebug::getLevel()>0) printf("dbPvGet::destroy beingDestroyed %s\n",
         (beingDestroyed ? "true" : "false"));
    {
        Lock xx(mutex);
        if(beingDestroyed) return;
        beingDestroyed = true;
        if (pNotify) dbNotifyCancel(pNotify.get());
    }
}

void DbPvGet::get()
{
    if (DbPvDebug::getLevel()>0) printf("dbPvGet::get()\n");
    if (block && process) {
        dbProcessNotify(pNotify.get());
    } else {
        dbScanLock(dbChannelRecord(dbPv->getDbChannel()));
        if (process) dbProcess(dbChannelRecord(dbPv->getDbChannel()));

        Lock lock(dataMutex);
        bitSet->clear();
        status = dbUtil->get(
                    channelGetRequester,
                    propertyMask,
                    dbPv->getDbChannel(),
                    pvStructure,
                    bitSet,
                    0);
        dbScanUnlock(dbChannelRecord(dbPv->getDbChannel()));
        if (firstTime) {
            firstTime = false;
            bitSet->clear();
            bitSet->set(0);
        }
        lock.unlock();
        channelGetRequester->getDone(
            status,
            getPtrSelf(),
            pvStructure,
            bitSet);
    }
}

void DbPvGet::getCallback(struct processNotify *pn, notifyGetType type)
{
    DbPvGet * pdp = static_cast<DbPvGet *>(pn->usrPvt);

    if (pn->status == notifyCanceled) {
        if (DbPvDebug::getLevel() > 0) printf("dbPvGet::getCallback notifyCanceled\n");
        return;
    }
    Lock lock(pdp->dataMutex);
    pdp->bitSet->clear();
    pdp->status = pdp->dbUtil->get(
                pdp->channelGetRequester,
                pdp->propertyMask,
                pdp->dbPv->getDbChannel(),
                pdp->pvStructure,
                pdp->bitSet,
                0);
    if (pdp->firstTime) {
        pdp->firstTime = false;
        pdp->bitSet->clear();
        pdp->bitSet->set(0);
    }
    lock.unlock();
    if (!pdp->status.isSuccess()) pn->status = notifyError;
}

void DbPvGet::doneCallback(struct processNotify *pn)
{
    DbPvGet * pdp = static_cast<DbPvGet *>(pn->usrPvt);

    pdp->channelGetRequester->getDone(
                pdp->status,
                pdp->getPtrSelf(),
                pdp->pvStructure,
                pdp->bitSet);
}

void DbPvGet::lock()
{
    dataMutex.lock();
}

void DbPvGet::unlock()
{
    dataMutex.unlock();
}

}}
